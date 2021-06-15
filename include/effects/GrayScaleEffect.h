#pragma once

#include "Effect.h"

class GrayScaleEffect : public Effect {
public:
  void apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) override;

  std::string name() const noexcept override {
    return "grayscale";
  }
};

extern "C" {
  Effect *create_effect();
}