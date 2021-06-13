#pragma once

#include "Effect.h"

class GlitchEffect : public Effect {
public:
  void apply(int width, int height, std::vector<std::vector<Pixel>> &data) override;

  std::string name() override {
    return "glitch";
  }
};

extern "C" {
  Effect *create_effect();
}