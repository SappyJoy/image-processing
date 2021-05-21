//
// Created by sappy-joy on 21/05/2021.
//

#ifndef IMAGE_PROCESSING_SRC_GLITCHEFFECT_H
#define IMAGE_PROCESSING_SRC_GLITCHEFFECT_H

#include <vector>
#include "../include/effects/Effect.h"
class GlitchEffect : public Effect {
  void apply(int width, int height, std::vector<std::vector<Pixel>> &data) override;
};

#endif //IMAGE_PROCESSING_SRC_GLITCHEFFECT_H
