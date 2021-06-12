//
// Created by sappy-joy on 20/05/2021.
//

#include <cmath>
#include "../../include/effects/DitheringEffect.h"
void DitheringEffect::apply(int width, int height, std::vector<std::vector<Pixel>> &data) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int oldR = data[y][x].R;
      int oldG = data[y][x].G;
      int oldB = data[y][x].B;

      //factor is the number of colors available in your color palette
      //using which u color your image.
      //eg if factor = 1, then there are 2 possible values 0 and 1.
      //Hence the number of colors in your color palette of each type ( ie R,G and B) are only 2.
      int factor = 1;

      //quantise the values to a shorter range of values
      int newR = (int)round(factor * (double)oldR / 255) * (255 / factor);
      int newG = (int)round(factor * (double)oldG / 255) * (255 / factor);
      int newB = (int)round(factor * (double)oldB / 255) * (255 / factor);

      data[y][x] = Pixel(newR, newG, newB);

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

extern "C" {
  Effect *create_effect() {
    return new DitheringEffect;
  }
}