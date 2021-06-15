//
// Created by sappy-joy on 20/05/2021.
//

#include <cmath>
#include "../../include/effects/DitheringEffect.h"

void DitheringEffect::apply(int width, int height, std::vector<std::vector<Pixel>> &data) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int16_t oldR = data[y][x].R;
      int16_t oldG = data[y][x].G;
      int16_t oldB = data[y][x].B;

      //quantise the values to a shorter range of values
      auto newR = int16_t((int)round(factor * (double)oldR / 255) * (255 / factor));
      auto newG = int16_t((int)round(factor * (double)oldG / 255) * (255 / factor));
      auto newB = int16_t((int)round(factor * (double)oldB / 255) * (255 / factor));

      data[y][x] = Pixel((int16_t)newR, (int16_t)newG, (int16_t)newB);

      //Dithering algo for pixel correction begins from here

      //errors for each color after quantization
      Pixel quant_error = Pixel(oldR - newR, oldG - newG, oldB - newB);

      if (x < width-1)
        data[y    ][x + 1] += quant_error * ((double)7 / 16);
      if (y < height-1 && x > 0)
        data[y + 1][x - 1] += quant_error * ((double)3 / 16);
      if (y < height-1)
        data[y + 1][x    ] += quant_error * ((double)5 / 16);
      if (y < height-1 && x < width-1)
        data[y + 1][x + 1] += quant_error * ((double)1 / 16);

    }
  }
}
void DitheringEffect::readParameters(boost::program_options::options_description &desc, int argc, char **argv) {
  namespace po = boost::program_options;
  desc.add_options()
      ("factor", po::value<int>()->default_value(1), "number of possible values of each color")
  ;
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
  po::notify(vm);

  if (vm.count("factor")) {
    factor = vm["factor"].as<int>();
  }
}

extern "C" {
  Effect *create_effect() {
    return new DitheringEffect;
  }
}