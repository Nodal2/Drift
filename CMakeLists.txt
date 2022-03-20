cmake_minimum_required( VERSION 3.15 )

project( drift )
include_directories( include )
file( GLOB_RECURSE SOURCES src/*.cpp )

find_package( OpenGL REQUIRED )

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory( dependancy/glfw )
add_subdirectory( dependancy/glad )
add_subdirectory( dependancy/glm )
add_subdirectory( dependancy/assimp )
include_directories( dependancy/stb_image/include )
add_library( stb_image dependancy/stb_image/lib/stb_image.cpp )

add_executable( drift app/main.cpp ${SOURCES} )

target_link_libraries( drift OpenGL::GL )
target_link_libraries( drift glfw )
target_link_libraries( drift glad )
target_link_libraries( drift glm )
target_link_libraries( drift assimp )
target_link_libraries( drift stb_image )


