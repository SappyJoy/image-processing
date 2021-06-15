//
// Created by sappy-joy on 21/05/2021.
//

#include <random>
#include "../../include/effects/GlitchEffect.h"
void shiftImage(int32_t start, int32_t shiftLength, int32_t width, int height, std::vector<std::vector<Pixel>> &data) {
  for (int32_t i = start + shiftLength; i < width * height * 3; i++) {
    data[(i/3-shiftLength)/width][(i/3-shiftLength)%width][(i-shiftLength)%3] = data[i/(width*3)][(i/3)%width][i%3];
  }
}

void GlitchEffect::apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  auto shiftCount = int32_t(mersenne() % 20);
  for (int32_t i = 0; i < shiftCount; i++) {
    auto start = int32_t(mersenne() % (width * height * 3));
    auto shiftLength = int32_t(mersenne() % 300);
    shiftImage(start, shiftLength, width, height, data);
  }
}

extern "C" {
  Effect *create_effect() {
    return new GlitchEffect;
  }
}