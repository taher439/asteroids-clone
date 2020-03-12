cmake_minimum_required(VERSION 3.6)

add_subdirectory(SDL2)

set(INCLUDE_DIR_ 
    Project/include)
set(SOURCE_DIR_
    Project/src)

include_directories(Project/include)

project(ASTEROID LANGUAGES CXX)
function(COMPILATION)
    cmake_parse_arguments( COMPILATION_PREFIX "USE_SDL" "EXEC" "SRC" ${ARGN})
    add_executable(${COMPILATION_PREFIX_EXEC} "")
    add_definitions(-D AZERTY)
    add_definitions(-D DEBUG)
    target_sources(${COMPILATION_PREFIX_EXEC} PRIVATE ${COMPILATION_PREFIX_SRC})
    target_include_directories(${COMPILATION_PREFIX_EXEC} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIR_}})
    set_target_properties(${COMPILATION_PREFIX_EXEC} PROPERTIES CXX_STANDARD 17)
    if (${COMPILATION_PREFIX_USE_SDL})
        target_link_libraries(${COMPILATION_PREFIX_EXEC} PRIVATE SDL2main SDL2-static)
    endif()
endfunction()

# ajouter les sources ici
set(SRC_FILES_
    ${SOURCE_DIR_}/asteroid.cpp  
    ${SOURCE_DIR_}/game.cpp  
    ${SOURCE_DIR_}/main.cpp  
    ${SOURCE_DIR_}/player.cpp  
    ${SOURCE_DIR_}/sdl_wrapper.cpp
    ${INCLUDE_DIR_}/asteroid.h
    ${INCLUDE_DIR_}/game.h
    ${INCLUDE_DIR_}/player.h
    ${INCLUDE_DIR_}/sdl_wrapper.h)

# fonction de compilation
# syntaxe ( EXEC nom_executable SRC sources  [USE_SDL] )
compilation(EXEC main SRC ${SRC_FILES_} USE_SDL)
