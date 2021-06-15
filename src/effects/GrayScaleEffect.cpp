//
// Created by sappy-joy on 20/05/2021.
//

#include "../../include/effects/GrayScaleEffect.h"
void GrayScaleEffect::apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) {
  for (int32_t y = 0; y < height; y++) {
    for (int32_t x = 0; x < width; x++) {
      auto averageGray = (int16_t)(((int32_t)data[y][x].R + data[y][x].G + data[y][x].B) / 3);

      data[y][x].R = averageGray;
      data[y][x].G = averageGray;
      data[y][x].B = averageGray;
    }
  }
}

extern "C" {
  Effect *create_effect() {
    return new GrayScaleEffect;
  }
}