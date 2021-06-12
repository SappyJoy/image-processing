//
// Created by sappy-joy on 21/05/2021.
//

#include <random>
#include "../../include/effects/GlitchEffect.h"
void shiftImage(int start, int shiftLength, int width, int height, std::vector<std::vector<Pixel>> &data) {
  for (int i = start + shiftLength; i < width * height * 3; i++) {
    data[(i/3-shiftLength)/width][(i/3-shiftLength)%width][(i-shiftLength)%3] = data[i/(width*3)][(i/3)%width][i%3];
  }
}

void GlitchEffect::apply(int width, int height, std::vector<std::vector<Pixel>> &data) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  int shiftCount = mersenne() % 20;
  for (int i = 0; i < shiftCount; i++) {
    int start = mersenne() % (width * height * 3);
    int shiftLength = mersenne() % 300;
    shiftImage(start, shiftLength, width, height, data);
  }
}

extern "C" {
  Effect *create_effect() {
    return new GlitchEffect;
  }
}