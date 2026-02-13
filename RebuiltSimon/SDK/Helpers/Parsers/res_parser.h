#ifndef RES_PARSER_H_
#define RES_PARSER_H_

#include <RebuiltSimon/Allocators/arena.h>

typedef enum res_token_kind_e {
    RES_TOKEN_IDENTIFIER = 256,
    RES_TOKEN_STRING,
    RES_TOKEN_LBRACE,
    RES_TOKEN_RBRACE,
    RES_TOKEN_EOF
} res_token_kind_t;

typedef enum { NODE_BLOCK, NODE_KV } NodeKind;

typedef struct res_node_s {
    NodeKind kind;
    char* name;
    char* value;
    struct res_node_s** children;
    size_t child_count;
    size_t child_capacity;
} res_node_t;

#ifdef __cplusplus
extern "C" {
#endif
    res_node_t* res_parse_file(arena_allocator_t* arena, const char* path);
    void res_print_node(res_node_t* node, int indent);
#ifdef __cplusplus
}
#endif

#endif /* RES_PARSER_H_ */