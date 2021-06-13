//
// Created by sappy-joy on 20/05/2021.
//

#include "../../include/effects/GrayScaleEffect.h"
void GrayScaleEffect::apply(int width, int height, std::vector<std::vector<Pixel>> &data) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto averageGray = (int16_t)(((int)data[y][x].R + data[y][x].G + data[y][x].B) / 3);

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