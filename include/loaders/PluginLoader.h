#pragma once

#include <memory>

#include "../effects/Effect.h"
#include "../../include/filetypes/ImageType.h"


class PluginLoader {
private:
  template<class T, class Fn>
  static std::unique_ptr<T> loadPlugin(const char * pluginName, const char * fnName);
public:
  static std::unique_ptr<Effect> loadEffect(const char * pluginName);
  static std::unique_ptr<ImageType> loadImageType(const char * pluginName);
};