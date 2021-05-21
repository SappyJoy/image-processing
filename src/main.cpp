#include <iostream>
#include "../include/filetypes/BMP.h"
#include "../include/effects/Effect.h"
#include "../include/effects/GrayScaleEffect.h"
#include "../include/effects/InvertEffect.h"
#include "../include/effects/DitheringEffect.h"
#include "GlitchEffect.h"

int main() {
  BMP img("../res/thankyou.bmp");

  Effect *effect;
  GlitchEffect dither;
  effect = &dither;
  GrayScaleEffect g;
//  g.apply(img.width, img.height, img.data);
  InvertEffect invert;
//  invert.apply(img.width, img.height, img.data);

  effect->apply(img.width, img.height, img.data);

  img.write("../res/thankyou_copy.bmp");
  return 0;
}
