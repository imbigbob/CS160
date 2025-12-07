cmake_minimum_required(VERSION "3.26")

project(CS160-Finance_manager)

FILE(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS main.cpp src/*.cpp)

set(TARGET_NAME CSG)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${TARGET_NAME} ${SOURCES})

# JSONCPP include
#include_directories(${CMAKE_SOURCE_DIR}/jsoncpp)
# add_compile_definitions(NOMINMAX)
# ----- SFML -----
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.6.0/lib/cmake/SFML)
set(SFML_COMPONENTS graphics window system)

find_package(SFML 2.6 COMPONENTS ${SFML_COMPONENTS} REQUIRED)

set(jsoncpp_DIR "C:/msys64/ucrt64/lib/cmake/jsoncpp")
find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(${TARGET_NAME} PRIVATE JsonCpp::JsonCpp)
if(SFML_FOUND)
  foreach(LIB ${SFML_COMPONENTS})
    target_link_libraries(${TARGET_NAME} PRIVATE sfml-${LIB})
  endforeach()
endif()

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)