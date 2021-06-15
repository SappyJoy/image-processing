#pragma once
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>

#include "../Pixel.h"

class Effect {
public:
  virtual void apply(int32_t width, int32_t height, std::vector<std::vector<Pixel>> &data) = 0;

  virtual void readParameters(boost::program_options::options_description &desc, int32_t argc, char *argv[]) { };

  virtual ~Effect() = default;

  virtual std::string name() const = 0;
};

using create_effect_fn = std::add_pointer<Effect *()>::type;