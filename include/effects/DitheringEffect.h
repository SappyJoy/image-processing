#pragma once

#include "Effect.h"

class DitheringEffect : public Effect {
public:
  void apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) override;

  void readParameters(boost::program_options::options_description &desc, int32_t argc, char **argv) override;

  std::string name() const noexcept override {
    return "dithering";
  }

private:
  //factor is the number of colors available in your color palette
  //using which u color your image.
  //eg if factor = 1, then there are 2 possible values 0 and 1.
  //Hence the number of colors in your color palette of each type ( ie R,G and B) are only 2.
  int32_t factor = 1;
};

extern "C" {
  Effect *create_effect();
}