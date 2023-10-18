#ifndef MRDCVLSC_FLAPPY_FFNN_GA_HPP
#define MRDCVLSC_FLAPPY_FFNN_GA_HPP

#include <cstddef>
#include <cinttypes>

static constexpr size_t FPS_LIMIT = 60;

static constexpr size_t WINDOW_WIDTH = 640;
static constexpr size_t WINDOW_HEIGHT = 360;

static constexpr float BIRD_SIZE = 20.f;
static constexpr float BIRD_STARTING_X_POS = static_cast<float>(WINDOW_WIDTH) / 4.f;
static constexpr float BIRD_STARTING_Y_POS = static_cast<float>(WINDOW_HEIGHT) / 2.f;

#endif