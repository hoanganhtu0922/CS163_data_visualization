cmake_minimum_required (VERSION 3.16)

project("Project")

add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)

#Automatically pick up files added to src
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(Project ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(Project PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

add_subdirectory("raylib")

target_link_libraries(${PROJECT_NAME} PUBLIC raylib)