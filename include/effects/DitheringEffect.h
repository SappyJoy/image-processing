#pragma once

#include "Effect.h"

class DitheringEffect : public Effect {
public:
  void apply(int width, int height, std::vector<std::vector<Pixel>> &data) override;

  void readParameters(boost::program_options::options_description &desc, int argc, char **argv) override;

  std::string name() override {
    return "dithering";
  }

private:
  //factor is the number of colors available in your color palette
  //using which u color your image.
  //eg if factor = 1, then there are 2 possible values 0 and 1.
  //Hence the number of colors in your color palette of each type ( ie R,G and B) are only 2.
  int factor = 1;
};

extern "C" {
  Effect *create_effect();
}