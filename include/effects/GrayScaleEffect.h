#pragma once

#include "Effect.h"
class GrayScaleEffect : public Effect {
public:
  void apply(int width, int height, std::vector<std::vector<Pixel>> &data) override;

  std::string name() override {
    return "grayscale";
  }
};

extern "C" {
  Effect *create_effect();
}