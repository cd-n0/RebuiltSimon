#include <RebuiltSimon/Allocators/arena.h>
#include <RebuiltSimon/SDK/Logger/logger.h>
#include "res_parser.h"

int ENGINE_FONT_HEIGHT;

int get_engine_font_height(int window_height) {
    arena_allocator_t* arena = arena_create(0);
    res_node_t* root = res_parse_file(arena, "cryoffear\\resource\\trackerscheme.res");
    res_node_t* scheme = NULL;
    for (size_t i = 0; i < root->child_count; i++) {
        res_node_t* node = root->children[i];
        if (node->kind == NODE_BLOCK && node->name && strcmp(node->name, "Scheme") == 0) { scheme = node; break; }
    }
    if (!scheme) { LOG_ERROR("Scheme block not found"); arena_destroy(arena); return 0; }

    res_node_t* fonts = NULL;
    for (size_t i = 0; i < scheme->child_count; i++) {
        res_node_t* node = scheme->children[i];
        if (node->kind == NODE_BLOCK && node->name && strcmp(node->name, "Fonts") == 0) { fonts = node; break; }
    }
    if (!fonts) { LOG_ERROR("Fonts block not found"); arena_destroy(arena); return 0; }

    res_node_t* engine = NULL;
    for (size_t i = 0; i < fonts->child_count; i++) {
        res_node_t* node = fonts->children[i];
        if (node->kind == NODE_BLOCK && node->name && strcmp(node->name, "EngineFont") == 0) { engine = node; break; }
    }
    if (!engine) { LOG_ERROR("EngineFont block not found"); arena_destroy(arena); return 0; }

    int result = 0;
    for (size_t i = 0; i < engine->child_count; i++) {
        res_node_t* entry = engine->children[i];
        if (entry->kind != NODE_BLOCK) continue;

        char* name = NULL;
        int tall = -1, weight = -1, dropshadow = 0;
        int yres_min = -1, yres_max = -1;

        for (size_t j = 0; j < entry->child_count; j++) {
            res_node_t* kv = entry->children[j];
            if (kv->kind != NODE_KV) continue;
            else if (strcmp(kv->name, "tall") == 0) { tall = atoi(kv->value); }
            else if (strcmp(kv->name, "yres") == 0) {
                if (sscanf(kv->value ? kv->value : "", "%d %d", &yres_min, &yres_max) != 2) { LOG_ERROR("Couldn't parse yres values"); }
            }
        }

        if (window_height >= yres_min && window_height <= yres_max) {
            result = tall;
            break;
        }
    }
    arena_destroy(arena);
    ENGINE_FONT_HEIGHT = result;

    return result;
}
