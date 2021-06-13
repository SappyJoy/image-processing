#include "../../include/loaders/PluginLoader.h"
#include <dlfcn.h>
#include <iostream>

template<class T, class Fn>
std::unique_ptr<T> PluginLoader::loadPlugin(const char *effect_name, const char *fnName) {
  void * effect_lib = dlopen(effect_name, RTLD_NOW | RTLD_LOCAL);
  if (!effect_lib) {
    std::cerr << "Failed to load plugin \"" << effect_name << "\": " << dlerror() << std::endl;
    std::exit(-1);
  }

  Fn effect_fn = reinterpret_cast<Fn>(dlsym(effect_lib, fnName));
  if (!effect_fn) {
    std::cerr << "Failed to load plugin \"" << effect_name << "\" factory function: " << dlerror() << std::endl;
    std::exit(-1);
  }

  return std::unique_ptr<T>(effect_fn());
}

std::unique_ptr<Effect> PluginLoader::loadEffect(const char *pluginName) {
  return loadPlugin<Effect, create_effect_fn>(pluginName, "create_effect");
}
std::unique_ptr<ImageType> PluginLoader::loadImageType(const char *pluginName) {
  return loadPlugin<ImageType, create_image_type_fn>(pluginName, "create_image_type");
}