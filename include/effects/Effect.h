#pragma once
#include "../Pixel.h"
class Effect {
public:
  virtual void apply(int width, int height, std::vector<std::vector<Pixel>> &data) = 0;
};

