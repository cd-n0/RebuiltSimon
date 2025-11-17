add_subdirectory ("ResParser")
include(FetchContent)

FetchContent_Declare(
  minhook
  GIT_REPOSITORY https://github.com/TsudaKageyu/minhook
  GIT_TAG master
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(minhook)

set(COMMON_SOURCES
    "${CMAKE_SOURCE_DIR}/memory/memory.h"
    "${CMAKE_SOURCE_DIR}/memory/memory.c"
    "main.c"
    "pch.c"
    "pch.h"
    "globals.h"
    "globals.c"
    "rebuilt_simon.h"
    "rebuilt_simon.c"
    "cvars.h"
    "cvars.c"
    "Features/Visual/hud.c"
    "Features/Visual/hud.h"
    "Features/Visual/info.c"
    "Features/Visual/info.h"
    "Hooks/hooks.h"
    "Hooks/hooks.c"
    "Hooks/Server/hook_list.h"
    "Hooks/Server/PM_PreventMegaBunnyJumping.c"
    "Hooks/Server/PM_Unduck.c"
    "Hooks/Client/hook_list.h"
    "Hooks/Client/CL_CreateMove.c"
    "Hooks/Client/Host_ValidSave.c"
    "Hooks/Client/HUD_Shutdown.c"
    "Hooks/Client/HUD_VidInit.c"
    "Hooks/Client/HUD_Redraw.c"
    "Hooks/Client/HUD_Frame.c"
    "Hooks/Client/HUD_Reset.c"
    "SDK/structures.h"
    "SDK/GoldSrc/convars.h"
    "SDK/GoldSrc/convars.c"
    "SDK/GoldSrc/input.h"
    "SDK/GoldSrc/input.c"
    "SDK/Helpers/SpriteUtils/sprite_utils.c"
    "SDK/Helpers/SpriteUtils/sprite_utils.h"
    "SDK/Helpers/Entities/player.h"
    "SDK/Helpers/Entities/player.c"
    "SDK/Helpers/Parsers/parsers.h"
    "SDK/Helpers/Parsers/parsers.c"
    "Features/Movement/autohop.c"
    "Features/Movement/autohop.h"
    "Features/Movement/ducktap.c"
    "Features/Movement/ducktap.h"
    "Features/Movement/yaw.c"
    "Features/Movement/yaw.h"
    "Features/Cheats/disable_stamina.c"
    "Features/Cheats/disable_stamina.h"
)

set(COMMON_INCLUDES
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/cl_dll"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/common"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/devtools"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/dlls"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/dmc"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/engine"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/external"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/game"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/game_shared"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/network"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/pm_shared"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/projects"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/public"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/utils"
    "${CMAKE_SOURCE_DIR}/halflife_sdk/extended"
)

set(COMMON_DEFINITIONS
    "_CRT_SECURE_NO_WARNINGS"
)

add_library(RebuiltSimon SHARED "${COMMON_SOURCES}")
target_compile_definitions(RebuiltSimon PUBLIC "CONSOLE=1" "${COMMON_DEFINITIONS}")
target_link_libraries(RebuiltSimon PRIVATE minhook ResParser)
target_include_directories(RebuiltSimon PUBLIC minhook ResParser)
target_include_directories(RebuiltSimon PUBLIC "${COMMON_INCLUDES}")

add_library(RebuiltSimonCoFManager SHARED  "${COMMON_SOURCES}")
target_compile_definitions(RebuiltSimonCoFManager PUBLIC "COF_MANAGER=1" "${COMMON_DEFINITIONS}")
target_link_libraries(RebuiltSimonCoFManager PRIVATE minhook ResParser)
target_include_directories(RebuiltSimonCoFManager PUBLIC minhook ResParser)
target_include_directories(RebuiltSimonCoFManager PUBLIC "${COMMON_INCLUDES}")

set_property(TARGET RebuiltSimon PROPERTY C_STANDARD 99)
set_property(TARGET RebuiltSimonCoFManager PROPERTY C_STANDARD 99)

if (CMAKE_VERSION VERSION_GREATER 3.16)
    target_precompile_headers(RebuiltSimon PRIVATE "pch.h")
endif()
