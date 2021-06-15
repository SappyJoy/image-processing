//
// Created by sappy-joy on 20/05/2021.
//

#include <cmath>
#include "../../include/effects/DitheringEffect.h"

void DitheringEffect::apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) {
  for (int32_t y = 0; y < height; y++) {
    for (int32_t x = 0; x < width; x++) {
      int16_t oldR = data[y][x].R;
      int16_t oldG = data[y][x].G;
      int16_t oldB = data[y][x].B;

      //quantise the values to a shorter range of values
      auto newR = int16_t((int32_t)round(factor * (double)oldR / 255) * (255 / factor));
      auto newG = int16_t((int32_t)round(factor * (double)oldG / 255) * (255 / factor));
      auto newB = int16_t((int32_t)round(factor * (double)oldB / 255) * (255 / factor));

      data[y][x] = Pixel((int16_t)newR, (int16_t)newG, (int16_t)newB);

      //Dithering algo for pixel correction begins from here

      //errors for each color after quantization
      Pixel quant_error = Pixel(int16_t(oldR - newR), int16_t(oldG - newG), int16_t(oldB - newB));

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
void DitheringEffect::readParameters(boost::program_options::options_description &desc, int32_t argc, char **argv) {
  namespace po = boost::program_options;
  desc.add_options()
      ("factor", po::value<int>()->default_value(1), "number of possible values of each color")
  ;
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
  po::notify(vm);

  if (vm.count("factor")) {
    factor = vm["factor"].as<int32_t>();
    if (factor <= 0 || factor > 256)
      throw std::runtime_error("Factor should be in range [1, 256]");
  }
}

extern "C" {
  Effect *create_effect() {
    return new DitheringEffect;
  }
}