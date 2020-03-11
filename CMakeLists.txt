cmake_minimum_required(VERSION 3.6)

add_subdirectory(SDL2)

project(ASTEROID LANGUAGES CXX)
function(COMPILATION)
    cmake_parse_arguments( COMPILATION_PREFIX "USE_SDL" "EXEC" "SRC" ${ARGN})
    add_executable(${COMPILATION_PREFIX_EXEC} "")
    add_definitions(-D AZERTY)
    add_definitions(-D DEBUG)
    target_sources(${COMPILATION_PREFIX_EXEC} PRIVATE ${COMPILATION_PREFIX_SRC})
    target_include_directories(${COMPILATION_PREFIX_EXEC} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}})
    set_target_properties(${COMPILATION_PREFIX_EXEC} PROPERTIES CXX_STANDARD 17)
    if (${COMPILATION_PREFIX_USE_SDL})
        target_link_libraries(${COMPILATION_PREFIX_EXEC} PRIVATE SDL2main SDL2-static)
    endif()
endfunction()

# ajouter les sources ici
set(SRC_FILES_
    Project/asteroid.cpp  
    Project/asteroid.h  
    Project/game.cpp  
    Project/game.h  
    Project/main.cpp  
    Project/player.cpp  
    Project/player.h 
    Project/sdl_wrapper.cpp  
    Project/sdl_wrapper.h)

# fonction de compilation
# syntaxe ( EXEC nom_executable SRC sources  [USE_SDL] )
compilation(EXEC main SRC ${SRC_FILES_}  USE_SDL)
