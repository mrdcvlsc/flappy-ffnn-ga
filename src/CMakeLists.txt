# https://github.com/SFML/cmake-sfml-project/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.16)
project(flappy_ffnn_ga LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_SILENT OFF)

FetchContent_Declare(SFML
    URL https://github.com/SFML/SFML/archive/refs/tags/2.6.0.tar.gz
)

FetchContent_Declare(Eigen
    URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
)

set(EIGEN_BUILD_DOC OFF)
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)

FetchContent_MakeAvailable(SFML Eigen)

file(GLOB SOURCES "*.cpp")

add_executable(flappy_ffnn_ga main.cc ${SOURCES})
target_link_libraries(flappy_ffnn_ga PRIVATE sfml-graphics sfml-window Eigen3::Eigen)
target_compile_features(flappy_ffnn_ga PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET flappy_ffnn_ga
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:flappy_ffnn_ga>
        VERBATIM)
endif()

file(COPY calibril.ttf DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})