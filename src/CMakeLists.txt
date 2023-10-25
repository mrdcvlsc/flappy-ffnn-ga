# FOR PRODUCTION

cmake_minimum_required(VERSION 3.21)
project(flappy-ffnn-ga LANGUAGES CXX)

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
set( OFF)

FetchContent_MakeAvailable(Eigen)

include_directories(SFML-2.6.0/include)
include_directories(Eigen-3.4.0/include)

file(GLOB SOURCES "*.cpp")

add_executable(flappy-ffnn-ga main.cc ${SOURCES})

target_link_libraries(flappy-ffnn-ga PRIVATE sfml-graphics sfml-window Eigen3::Eigen)

target_compile_features(flappy-ffnn-ga PRIVATE cxx_std_20)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET flappy-ffnn-ga POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:flappy-ffnn-ga> $<TARGET_FILE_DIR:flappy-ffnn-ga> COMMAND_EXPAND_LISTS)
endif()

file(COPY calibril.ttf DESTINATION ${CMAKE_BINARY_DIR})