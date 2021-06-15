//
// Created by sappy-joy on 15/06/2021.
//

#include "../../include/effects/SepiaEffect.h"
void SepiaEffect::apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) {
  for (int32_t y = 0; y < height; y++) {
    for (int32_t x = 0; x < width; x++) {
      double R = data[y][x].R * 0.393 + data[y][x].G * 0.769 + data[y][x].B * 0.189;
      double G = data[y][x].R * 0.349 + data[y][x].G * 0.686 + data[y][x].B * 0.168;
      double B = data[y][x].R * 0.272 + data[y][x].G * 0.534 + data[y][x].B * 0.131;

      data[y][x].R = std::min((int16_t)255, (int16_t)R);
      data[y][x].G = std::min((int16_t)255, (int16_t)G);
      data[y][x].B = std::min((int16_t)255, (int16_t)B);
    }
  }
}

extern "C" {
  Effect *create_effect() {
    return new SepiaEffect;
  }
}