#include "libresparser.h"

#include "libresparser.h"


// res_parser.c
// Simple lexer + parser for Valve-style .res files.
// Compile: gcc -std=c11 -O2 res_parser.c -o res_parser

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum res_token_kind_e {
    RES_TOKEN_EOF,
    RES_TOKEN_LBRACE,
    RES_TOKEN_RBRACE,
    RES_TOKEN_STRING,
    RES_TOKEN_IDENTIFIER
} res_token_kind_t;

typedef struct res_token_s {
    res_token_kind_t kind;
    char* text; // allocated, for TK_STRING and TK_IDENT
    int line;
} res_token_t;

typedef struct {
    const char* src;
    size_t pos;
    int line;
} LexerState;

// Node kinds: either a block (name + children) or a key-value pair
typedef enum { NODE_BLOCK, NODE_KV } NodeKind;

typedef struct ResNode {
    NodeKind kind;
    char* name; // for blocks: block name; for kv: key
    char* value; // only for kv nodes
    struct ResNode** children;
    size_t child_count;
} ResNode;

// ---------- Utilities ----------
static char* dupstr(const char* s) { if (!s) return NULL; size_t n = strlen(s) + 1; char* r = malloc(n); memcpy(r, s, n); return r; }
static void* xmalloc(size_t n) { void* p = malloc(n); if (!p) { fprintf(stderr, "out of memory\n");exit(1); } return p; }

// ---------- Lexer ----------
static void skip_whitespace_and_comments(LexerState* L) {
    for (;;) {
        char c = L->src[L->pos];
        if (c == '\0') return;
        if (c == '/') {
            if (L->src[L->pos + 1] == '/') {
                // skip until end of line
                L->pos += 2;
                while (L->src[L->pos] && L->src[L->pos] != '\n') L->pos++;
                continue;
            }
            // not a comment, break to handle slash as normal
            return;
        }
        if (c == '\n') { L->line++; L->pos++; continue; }
        if (isspace((unsigned char)c)) { L->pos++; continue; }
        return;
    }
}

static res_token_t next_token(LexerState* L) {
    skip_whitespace_and_comments(L);
    res_token_t t = { RES_TOKEN_EOF, NULL, L->line };
    char c = L->src[L->pos];
    if (c == '\0') { t.kind = RES_TOKEN_EOF; return t; }
    if (c == '{') { L->pos++; t.kind = RES_TOKEN_LBRACE; return t; }
    if (c == '}') { L->pos++; t.kind = RES_TOKEN_RBRACE; return t; }
    if (c == '"') {
        // parse quoted string
        L->pos++; // skip opening "
        size_t start = L->pos;
        char buf[4096];
        size_t bi = 0;
        while (L->src[L->pos] && L->src[L->pos] != '"') {
            if (L->src[L->pos] == '\\') {
                L->pos++;
                char esc = L->src[L->pos];
                if (esc == 'n') buf[bi++] = '\n';
                else if (esc == 't') buf[bi++] = '\t';
                else if (esc == '"') buf[bi++] = '"';
                else if (esc == '\\') buf[bi++] = '\\';
                else buf[bi++] = esc;
                L->pos++;
            }
            else {
                buf[bi++] = L->src[L->pos++];
            }
            if (bi + 1 >= sizeof(buf)) break;
        }
        buf[bi] = '\0';
        if (L->src[L->pos] == '"') L->pos++;
        t.kind = RES_TOKEN_STRING;
        t.text = dupstr(buf);
        return t;
    }
    // identifier / bare token: letters, digits, underscore, dot, slash, colon, hyphen
    if (isalnum((unsigned char)c) || c == '_') {
        size_t s = L->pos;
        while (L->src[L->pos] && (isalnum((unsigned char)L->src[L->pos]) || strchr("_-.:/\\", L->src[L->pos]))) L->pos++;
        size_t len = L->pos - s;
        char* tmp = xmalloc(len + 1);
        memcpy(tmp, L->src + s, len);
        tmp[len] = '\0';
        t.kind = RES_TOKEN_IDENTIFIER;
        t.text = tmp;
        return t;
    }
    // unknown single char -> treat as ident of that char
    L->pos++;
    char tbuf[2] = { c, 0 };
    t.kind = RES_TOKEN_IDENTIFIER;
    t.text = dupstr(tbuf);
    return t;
}

// ---------- Parser ----------
typedef struct {
    LexerState* L;
    res_token_t cur;
} Parser;

static void token_free(res_token_t* t) { if (t->text) free(t->text); t->text = NULL; }

static void advance(Parser* P) {
    token_free(&P->cur);
    P->cur = next_token(P->L);
}

static void expect(Parser* P, res_token_kind_t k) {
    if (P->cur.kind != k) {
        fprintf(stderr, "Syntax error (line %d): unexpected token\n", P->cur.line);
        exit(1);
    }
    advance(P);
}

static ResNode* resnode_new(NodeKind kind) {
    ResNode* n = xmalloc(sizeof(*n));
    n->kind = kind;
    n->name = NULL;
    n->value = NULL;
    n->children = NULL;
    n->child_count = 0;
    return n;
}

static void resnode_add_child(ResNode* parent, ResNode* c) {
    parent->children = realloc(parent->children, sizeof(ResNode*) * (parent->child_count + 1));
    parent->children[parent->child_count++] = c;
}

static ResNode* parse_block(Parser* P);

static ResNode* parse_value_or_block(Parser* P, const char* keyname) {
    // if next is LBRACE, parse block with name keyname
    if (P->cur.kind == RES_TOKEN_LBRACE) {
        advance(P); // consume {
        ResNode* blk = resnode_new(NODE_BLOCK);
        blk->name = dupstr(keyname);
        // parse until matching }
        while (P->cur.kind != RES_TOKEN_RBRACE && P->cur.kind != RES_TOKEN_EOF) {
            ResNode* child = parse_block(P);
            if (child) resnode_add_child(blk, child);
        }
        if (P->cur.kind == RES_TOKEN_RBRACE) advance(P); // consume }
        return blk;
    }
    // otherwise, parse a value (string or ident) and create KV node
    if (P->cur.kind == RES_TOKEN_STRING || P->cur.kind == RES_TOKEN_IDENTIFIER) {
        ResNode* kv = resnode_new(NODE_KV);
        kv->name = dupstr(keyname);
        kv->value = dupstr(P->cur.text);
        advance(P);
        return kv;
    }
    fprintf(stderr, "Syntax error (line %d): expected value or block for key '%s'\n", P->cur.line, keyname);
    exit(1);
}

// parse a block or key/value. Caller expects cur token to be either STRING/TK_IDENT (name) or LBRACE unexpected.
static ResNode* parse_block(Parser* P) {
    if (P->cur.kind == RES_TOKEN_EOF) return NULL;
    if (P->cur.kind == RES_TOKEN_RBRACE) return NULL;
    if (P->cur.kind == RES_TOKEN_LBRACE) {
        // anonymous block? produce block with empty name and parse inside
        advance(P);
        ResNode* blk = resnode_new(NODE_BLOCK);
        blk->name = dupstr("");
        while (P->cur.kind != RES_TOKEN_RBRACE && P->cur.kind != RES_TOKEN_EOF) {
            ResNode* c = parse_block(P);
            if (c) resnode_add_child(blk, c);
        }
        if (P->cur.kind == RES_TOKEN_RBRACE) advance(P);
        return blk;
    }
    if (P->cur.kind == RES_TOKEN_STRING || P->cur.kind == RES_TOKEN_IDENTIFIER) {
        char* name = dupstr(P->cur.text);
        advance(P);
        // if next is LBRACE -> block with name
        if (P->cur.kind == RES_TOKEN_LBRACE) {
            ResNode* blk = resnode_new(NODE_BLOCK);
            blk->name = name;
            advance(P); // consume {
            while (P->cur.kind != RES_TOKEN_RBRACE && P->cur.kind != RES_TOKEN_EOF) {
                ResNode* child = parse_block(P);
                if (child) resnode_add_child(blk, child);
            }
            if (P->cur.kind == RES_TOKEN_RBRACE) advance(P); // consume }
            return blk;
        }
        else {
            // parse value(s). The resource format typically has a single value token (string or ident).
            if (P->cur.kind == RES_TOKEN_STRING || P->cur.kind == RES_TOKEN_IDENTIFIER) {
                ResNode* kv = resnode_new(NODE_KV);
                kv->name = name;
                kv->value = dupstr(P->cur.text);
                advance(P);
                return kv;
            }
            else {
                // it's valid to have key followed by block-like constructs (e.g., nested blocks without braces) but we keep simple:
                return parse_value_or_block(P, name);
            }
        }
    }
    // unexpected token
    fprintf(stderr, "Syntax error (line %d): unexpected token in parse_block\n", P->cur.line);
    exit(1);
}

static ResNode* parse_all(Parser* P) {
    ResNode* root = resnode_new(NODE_BLOCK);
    root->name = dupstr("__root__");
    while (P->cur.kind != RES_TOKEN_EOF) {
        ResNode* n = parse_block(P);
        if (n) resnode_add_child(root, n);
    }
    return root;
}

// ---------- Printing / freeing ----------
static void print_indent(int d) { for (int i = 0;i < d;i++) putchar(' '); }

static void print_node(ResNode* n, int indent) {
    if (n->kind == NODE_BLOCK) {
        print_indent(indent);
        printf("Block: \"%s\"\n", n->name ? n->name : "");
        for (size_t i = 0;i < n->child_count;i++) print_node(n->children[i], indent + 2);
    }
    else {
        print_indent(indent);
        printf("KV: \"%s\" = \"%s\"\n", n->name ? n->name : "", n->value ? n->value : "");
    }
}

static void free_node(ResNode* n) {
    if (!n) return;
    if (n->name) free(n->name);
    if (n->value) free(n->value);
    for (size_t i = 0;i < n->child_count;i++) free_node(n->children[i]);
    free(n->children);
    free(n);
}

// ---------- Main / IO ----------
static char* read_file(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) { perror("fopen"); return NULL; }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = xmalloc(sz + 1);
    if (fread(buf, 1, sz, f) != (size_t)sz) { perror("fread"); fclose(f); free(buf); return NULL; }
    buf[sz] = '\0';
    fclose(f);
    return buf;
}


int NPRINTF_Y_MARGIN;
int NPRINTF_FONT_HEIGHT;
int INFO_BACKGROUND_Y_MARGIN;
int INFO_BACKGROUND_WIDTH;

engine_font_t get_engine_font(int window_height) {
    char* s = read_file("cryoffear\\resource\\trackerscheme.res");
    if (!s) return (engine_font_t) { 0 };
    LexerState Ls = { s, 0, 1 };
    Parser Pstate = { &Ls, {RES_TOKEN_EOF, NULL, 1} };
    advance(&Pstate); // load first token
    ResNode* root = parse_all(&Pstate);

    // find Scheme block (top-level), then Fonts, then EngineFont
    ResNode* scheme = NULL;
    for (size_t i = 0;i < root->child_count;i++) {
        ResNode* c = root->children[i];
        if (c->kind == NODE_BLOCK && c->name && strcmp(c->name, "Scheme") == 0) { scheme = c; break; }
    }
    if (!scheme) { fprintf(stderr, "Scheme block not found\n"); free_node(root); free(s); return (engine_font_t) { 0 }; }

    ResNode* fonts = NULL;
    for (size_t i = 0;i < scheme->child_count;i++) {
        ResNode* c = scheme->children[i];
        if (c->kind == NODE_BLOCK && c->name && strcmp(c->name, "Fonts") == 0) { fonts = c; break; }
    }
    if (!fonts) { fprintf(stderr, "Fonts block not found\n"); free_node(root); free(s); return (engine_font_t) { 0 }; }

    ResNode* engine = NULL;
    for (size_t i = 0;i < fonts->child_count;i++) {
        ResNode* c = fonts->children[i];
        if (c->kind == NODE_BLOCK && c->name && strcmp(c->name, "EngineFont") == 0) { engine = c; break; }
    }
    if (!engine) { fprintf(stderr, "EngineFont block not found\n"); free_node(root); free(s); return (engine_font_t) { 0 }; }

    // iterate entries ("1","2",...)
    engine_font_t result = { 0 };
    int found = 0;
    for (size_t i = 0;i < engine->child_count && !found;i++) {
        ResNode* entry = engine->children[i];
        if (entry->kind != NODE_BLOCK) continue;

        // temp holders
        char* name = NULL;
        int tall = -1, weight = -1, dropshadow = 0;
        int yres_min = -1, yres_max = -1;

        for (size_t j = 0;j < entry->child_count;j++) {
            ResNode* kv = entry->children[j];
            if (kv->kind != NODE_KV) continue;
            if (strcmp(kv->name, "name") == 0) { free(name); name = dupstr(kv->value); }
            else if (strcmp(kv->name, "tall") == 0) { tall = atoi(kv->value); }
            else if (strcmp(kv->name, "weight") == 0) { weight = atoi(kv->value); }
            else if (strcmp(kv->name, "dropshadow") == 0) { dropshadow = atoi(kv->value); }
            else if (strcmp(kv->name, "yres") == 0) {
                // parse two ints from kv->value like "480 599"
                int a = 0, b = 0;
                if (sscanf(kv->value, "%d %d", &a, &b) >= 1) { yres_min = a; yres_max = b == 0 ? a : b; }
            }
        }

        if (yres_min == -1 && yres_max == -1) {
            // If no yres given, treat as match-all (optional). Here skip.
            continue;
        }

        if (window_height >= yres_min && window_height <= yres_max) {
            result.name = name ? name : dupstr("");
            result.tall = tall;
            result.weight = weight;
            result.dropshadow = dropshadow;
            result.yres_min = yres_min;
            result.yres_max = yres_max;
            found = 1;
            // don't free name because used in result
        }
        else {
            if (name) free(name);
        }
    }

    if (!found) {
        result = (engine_font_t){ 0 };
    }

    NPRINTF_Y_MARGIN = 25;
    NPRINTF_FONT_HEIGHT = result.tall;
    INFO_BACKGROUND_Y_MARGIN = NPRINTF_FONT_HEIGHT;
    INFO_BACKGROUND_WIDTH = 9 * result.tall;

    free_node(root);
    token_free(&Pstate.cur);
    free(s);
    return result;
}