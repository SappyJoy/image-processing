#pragma once

#include "Effect.h"

class InvertEffect : public Effect {
public:
  void apply(int width, int height, std::vector<std::vector<Pixel>> &data) override;

  std::string name() override {
    return "invert";
  }
};

extern "C" {
  Effect *create_effect();
}