#pragma once
#include <string>
#include <vector>
#include "../Pixel.h"

class Effect {
public:
  virtual void apply(int width, int height, std::vector<std::vector<Pixel>> &data) = 0;

  virtual ~Effect() = default;

  virtual std::string name() = 0;
};

using create_effect_fn = std::add_pointer<Effect *()>::type;