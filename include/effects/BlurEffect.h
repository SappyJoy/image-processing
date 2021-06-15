#pragma once

#include "Effect.h"

class BlurEffect : public Effect {
public:
  void apply(int width, int height, std::vector<std::vector<Pixel>> &data) override;

  void readParameters(boost::program_options::options_description &desc, int argc, char **argv) override;

  std::string name() override {
    return "blur";
  }

private:
  double sigma; // Blur degree
};

extern "C" {
  Effect *create_effect();
}