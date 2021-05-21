#pragma once

#include <vector>
#include "Effect.h"
class InvertEffect : public Effect {
public:
  void apply(int width, int height, std::vector<std::vector<Pixel>> &data) override;
};
