#pragma once

#include "Effect.h"

class BlurEffect : public Effect {
public:
  void apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) override;

  void readParameters(boost::program_options::options_description &desc, int32_t argc, char **argv) override;

  std::string name() const noexcept override {
    return "blur";
  }

private:
  double sigma; // Blur degree
};

extern "C" {
  Effect *create_effect();
}