# FOR PRODUCTION

cmake_minimum_required(VERSION 3.21)
project(flappy_ffnn_ga LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

set(FETCHCONTENT_QUIET off)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3.4.0
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

set(EIGEN_BUILD_DOC OFF)
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)

FetchContent_MakeAvailable(Eigen)

include_directories(SFML-2.6.0/include)
include_directories(Eigen-3.4.0/include)

file(GLOB SOURCES "*.cpp")

add_executable(flappy_ffnn_ga main.cc ${SOURCES})

target_link_libraries(flappy_ffnn_ga PRIVATE sfml-graphics sfml-window Eigen3::Eigen)

target_compile_features(flappy_ffnn_ga PRIVATE cxx_std_17)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET flappy_ffnn_ga POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:flappy_ffnn_ga> $<TARGET_FILE_DIR:flappy_ffnn_ga> COMMAND_EXPAND_LISTS)
endif()

# if(WIN32)
#     add_custom_command(
#         TARGET CMakeSFMLProject
#         COMMENT "Copy OpenAL DLL"
#         PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
#         VERBATIM)
# endif()

file(COPY calibril.ttf DESTINATION ${CMAKE_BINARY_DIR})