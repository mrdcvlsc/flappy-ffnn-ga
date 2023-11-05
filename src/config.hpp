#ifndef MRDCVLSC_FLAPPY_FFNN_GA_HPP
#define MRDCVLSC_FLAPPY_FFNN_GA_HPP

#include <chrono>
#include <random>

#include <cstddef>
#include <cstdint>
#include <cinttypes>

static constexpr float VIEW_MOVE_DISTANCE = 2.f;

static constexpr size_t WINDOW_WIDTH = 640;
static constexpr size_t WINDOW_HEIGHT = 360;

static std::mt19937 rand_engine(std::chrono::system_clock::now().time_since_epoch().count());

#endif