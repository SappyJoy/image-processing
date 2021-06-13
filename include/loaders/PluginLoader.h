#pragma once

#include <memory>

#include "../effects/Effect.h"
#include "../../include/filetypes/ImageType.h"

/**
 * Plugin Loader loads plugin with specified location and factory function
 */
class PluginLoader {
private:
  template<class T, class Fn>
  static std::unique_ptr<T> loadPlugin(const char * pluginName, const char * fnName);
public:
  /**
   * Creates object of class that implement base class Effect using factory function `create_effect`
   * @param pluginName Location of file in system
   * @return Unique pointer on effect
   */
  static std::unique_ptr<Effect> loadEffect(const char * pluginName);

  /**
 * Creates object of class that implement base class ImageType using factory function `create_image_type`
 * @param pluginName Location of file in system
 * @return Unique pointer on image type
 */
  static std::unique_ptr<ImageType> loadImageType(const char * pluginName);
};