#include "res_parser.h"

#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <RebuiltSimon/Allocators/arena.h>
#include <RebuiltSimon/SDK/Logger/logger.h>

typedef struct res_token_s {
    res_token_kind_t kind;
    char* text;
    size_t position;
    size_t line;
} res_token_t;

typedef struct lexer_s {
    const char* src;
    size_t position;
    size_t line;
    arena_allocator_t* arena;
} lexer_t;

typedef struct parser_s {
    lexer_t* lexer;
    res_token_t current;
} parser_t;

static void skip_whitespace_and_comments(lexer_t* lexer) {
    for (;;) {
        char c = lexer->src[lexer->position];
        /* EOF */
        if (c == '\0') {
            return;
        }
        /* Comments */
        if (c == '/') {
            if (lexer->src[lexer->position + 1] == '/') {
                lexer->position += 2;
                while (lexer->src[lexer->position] && lexer->src[lexer->position] != '\n') lexer->position++;
                continue;
            }
            return;
        }
        /* Newline */
        if (c == '\n') {
            lexer->line++; lexer->position++;
            continue;
        }
        /* Whitespace */
        if (isspace((unsigned char)c)) {
            lexer->position++;
            continue;
        }

        return;
    }
}

static res_token_t next_token(lexer_t* lexer) {
    skip_whitespace_and_comments(lexer);
    res_token_t t = { RES_TOKEN_EOF, NULL, lexer->line };
    char c = lexer->src[lexer->position];
    if (c == '\0') { t.kind = RES_TOKEN_EOF; return t; }
    if (c == '{') { lexer->position++; t.kind = RES_TOKEN_LBRACE; return t; }
    if (c == '}') { lexer->position++; t.kind = RES_TOKEN_RBRACE; return t; }
    if (c == '"') {
        size_t start = ++lexer->position;
        while (lexer->src[lexer->position] && lexer->src[lexer->position] != '"') lexer->position++;
        size_t len = lexer->position - start;
        char* tmp = arena_alloc(lexer->arena, len + 1);
        if (!tmp) {
            LOG_ERROR("Arena couldn't allocate memory or out of memory");
            return (res_token_t) { 0 };
        }
        memcpy(tmp, lexer->src + start, len);
        tmp[len] = '\0';
        if (lexer->src[lexer->position] == '"') lexer->position++;
        else {
            LOG_ERROR("String never ended");
            return (res_token_t){ 0 };
        };
        t.kind = RES_TOKEN_STRING;
        t.text = tmp;
        return t;
    }
    if (isalnum(c)) {
        size_t start = lexer->position;
        while (lexer->src[lexer->position] && isalnum(lexer->src[lexer->position])) lexer->position++;
        size_t len = lexer->position - start;
        char* tmp = arena_alloc(lexer->arena, len + 1);
        if (!tmp) {
            LOG_ERROR("Arena couldn't allocate memory or out of memory");
            return (res_token_t){ 0 };
        }
        memcpy(tmp, lexer->src + start, len);
        tmp[len] = '\0';
        t.kind = RES_TOKEN_IDENTIFIER;
        t.text = tmp;
        return t;
    }
    return (res_token_t) { 0 };
}

static void advance(parser_t* parser) {
    parser->current = next_token(parser->lexer);
}

static void expect(parser_t* parser, res_token_kind_t k) {
    if (parser->current.kind != k) {
        LOG_ERROR("Syntax error (line %d): unexpected token", parser->current.line);
        return;
    }
    advance(parser);
}

static res_node_t* resnode_new(arena_allocator_t* arena, NodeKind kind) {
    res_node_t* node = arena_alloc(arena, sizeof(*node));
    if (!node) {
        LOG_ERROR("Arena couldn't allocate memory or out of memory");
        return NULL;
    }
    node->kind = kind;
    node->name = NULL;
    node->value = NULL;
    node->children = NULL;
    node->child_count = 0;
    node->child_capacity = 0;

    return node;
}

#define DEFAULT_CHILD_NODE_CAPACITY 4

static void resnode_add_child(arena_allocator_t* arena, res_node_t* parent, res_node_t* child_node) {
    size_t new_count = parent->child_count + 1;

    if (new_count > parent->child_capacity) {
        parent->child_capacity = parent->child_capacity ? parent->child_capacity * 2 : DEFAULT_CHILD_NODE_CAPACITY;
        res_node_t** new_arr = arena_alloc(arena, sizeof(*new_arr) * parent->child_capacity);
        if (!new_arr) {
            LOG_ERROR("Arena couldn't allocate memory or out of memory");
            return;
        }
        memcpy(new_arr, parent->children, parent->child_count * sizeof(*new_arr));
        parent->children = new_arr;
    }
    parent->children[parent->child_count] = child_node;
    parent->child_count = new_count;
}

static res_node_t* parse_block(parser_t* parser) {
    arena_allocator_t* arena = parser->lexer->arena;
    switch (parser->current.kind) {
        case RES_TOKEN_EOF:
        case RES_TOKEN_RBRACE: {
            return NULL;
        }
        case RES_TOKEN_LBRACE: {
            advance(parser);
            res_node_t* block = resnode_new(arena, NODE_BLOCK);
            while (parser->current.kind != RES_TOKEN_RBRACE && parser->current.kind != RES_TOKEN_EOF) {
                res_node_t* node = parse_block(parser);
                if (node) resnode_add_child(arena, block, node);
            }
            if (parser->current.kind == RES_TOKEN_RBRACE) advance(parser);
            return block;
        }
        case RES_TOKEN_STRING:
        case RES_TOKEN_IDENTIFIER: {
            char* name = parser->current.text;
            advance(parser);
            switch (parser->current.kind) {
               case RES_TOKEN_LBRACE: {
                    res_node_t* block = resnode_new(arena, NODE_BLOCK);
                    block->name = name;
                    advance(parser);
                    while (parser->current.kind != RES_TOKEN_RBRACE && parser->current.kind != RES_TOKEN_EOF) {
                        res_node_t* child = parse_block(parser);
                        if (child) resnode_add_child(arena, block, child);
                    }
                    if (parser->current.kind == RES_TOKEN_RBRACE) advance(parser);
                    return block;
                }
                case RES_TOKEN_STRING:
                case RES_TOKEN_IDENTIFIER: {
                    res_node_t* kv = resnode_new(arena, NODE_KV);
                    kv->name = name;
                    kv->value = parser->current.text;
                    return kv;
                }
                case RES_TOKEN_RBRACE:
                    return NULL;
            }
        }
    }
    
    LOG_ERROR("Syntax error (line %d, pos %d): unexpected token in parse block", parser->lexer->line, parser->lexer->position);
    return NULL;
}

static res_node_t* parse_all(parser_t* parser) {
    advance(parser);
    arena_allocator_t* arena = parser->lexer->arena;
    res_node_t* root = resnode_new(arena, NODE_BLOCK);
    root->name = arena_strdup(arena, "__root__");
    while (parser->current.kind != RES_TOKEN_EOF) {
        res_node_t* node = parse_block(parser);
        if (node) resnode_add_child(arena, root, node);
    }
    return root;
}

static void print_indent(int d) { for (int i = 0; i < d; i++) putchar(' '); }

void res_print_node(res_node_t* node, int indent) {
    if (node->kind == NODE_BLOCK) {
        print_indent(indent);
        printf("Block: \"%s\"\n", node->name);
        for (size_t i = 0; i < node->child_count; i++) res_print_node(node->children[i], indent + 4);
    }
    else {
        print_indent(indent);
        printf("KV: \"%s\" = \"%s\"\n", node->name, node->value);
    }
}

res_node_t* res_parse_file(arena_allocator_t* arena, const char* path) {
    if (!arena || !path) return NULL;
    size_t length = 0;
    void* source = arena_read_file(arena, path, &length);
    if (!source || !length) return 0;

    lexer_t lexer = { source, 0, 1, arena };
    parser_t parser = { &lexer, {RES_TOKEN_EOF, NULL, 0, 1} };
    res_node_t* root = parse_all(&parser);
    return root;
}