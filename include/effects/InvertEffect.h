#pragma once

#include "Effect.h"

class InvertEffect : public Effect {
public:
  void apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) override;

  std::string name() const override {
    return "invert";
  }
};

extern "C" {
  Effect *create_effect();
}