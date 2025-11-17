add_executable(injector main.c)

add_dependencies(injector RebuiltSimon)

target_compile_definitions(injector PRIVATE "-DDLL_PATH=\"$<TARGET_FILE:RebuiltSimon>\"")