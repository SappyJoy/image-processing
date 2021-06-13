#pragma once

#include "Effect.h"
class DitheringEffect : public Effect {
public:
  void apply(int width, int height, std::vector<std::vector<Pixel>> &data) override;

  std::string name() override {
    return "dithering";
  }
};

extern "C" {
  Effect *create_effect();
}