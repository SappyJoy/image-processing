#pragma once

#include "Effect.h"

class SepiaEffect : public Effect {
public:
  void apply(int width, int height, std::vector<std::vector<Pixel>> &data) override;

  std::string name() override {
    return "sepia";
  }
};

extern "C" {
  Effect *create_effect();
}