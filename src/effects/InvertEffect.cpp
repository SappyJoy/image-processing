//
// Created by sappy-joy on 20/05/2021.
//

#include "../../include/effects/InvertEffect.h"
void InvertEffect::apply(int width, int height, std::vector<std::vector<Pixel>> &data) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      data[y][x].R = 255 - data[y][x].R;
      data[y][x].G = 255 - data[y][x].G;
      data[y][x].B = 255 - data[y][x].B;
    }
  }
}
