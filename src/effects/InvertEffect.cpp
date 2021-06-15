//
// Created by sappy-joy on 20/05/2021.
//

#include "../../include/effects/InvertEffect.h"

void InvertEffect::apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) {
  for (int32_t y = 0; y < height; y++) {
    for (int32_t x = 0; x < width; x++) {
      data[y][x].R = int16_t(255 - data[y][x].R);
      data[y][x].G = int16_t(255 - data[y][x].G);
      data[y][x].B = int16_t(255 - data[y][x].B);
    }
  }
}

extern "C" {
  Effect *create_effect() {
    return new InvertEffect;
  }
}