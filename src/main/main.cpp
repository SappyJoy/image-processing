#include <iostream>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "../../include/filetypes/BMP.h"
#include "../../include/effects/Effect.h"
#include "../../include/loaders/PluginLoader.h"

namespace fs = std::filesystem;

void help(std::unordered_map<std::string, std::unique_ptr<Effect>> & effects) {
  std::cout << "effect [effect name] [image]" << std::endl;
  std::cout << "Available effects:" << std::endl;
  for (auto & effect : effects) {
    printf("\t%s\n", effect.first.c_str());
  }
}

std::unordered_map<std::string, std::unique_ptr<Effect>> loadEffects() {
  std::unordered_map<std::string, std::unique_ptr<Effect>> effects;
  for (const auto & file : fs::directory_iterator("src/effects/")) {
    if (!file.is_regular_file()) continue;

    const auto & path = file.path();
    if (path.extension() != ".so") continue;

    auto effect = PluginLoader::loadEffect(path.string().c_str());
    auto filename = path.filename().string();
    effects[effect->name()] = std::move(effect);
  }

  return effects;
}

std::unordered_set<std::unique_ptr<ImageType>> loadImageTypes() {
  std::unordered_set<std::unique_ptr<ImageType>> imgTypes;
  for (const auto & file : fs::directory_iterator("src/filetypes/")) {
    if (!file.is_regular_file()) continue;

    const auto & path = file.path();
    if (path.extension() != ".so") continue;

    auto img = PluginLoader::loadImageType(path.string().c_str());
    auto filename = path.filename().string();
    imgTypes.insert(std::move(img));
  }

  return imgTypes;
}

int main(int argc, char * argv[]) {
  const char *ext = fs::path(argv[2]).extension().c_str();
  std::string newFilename =
      std::string(argv[2]).substr(0, std::string(argv[2]).size()-std::string(ext).size()) + "_copy" + ext;

  auto effects = loadEffects();
  auto imageTypes = loadImageTypes();

  if (argc < 3) {
    help(effects);
    return -1;
  }

  if (effects.find(argv[1]) == effects.end()) {
    std::cout << "Wrong effect name" << std::endl;
    help(effects);
    return -2;
  }

  ImageType *img;

  std::unordered_set<std::string> allExtensions;
  for (auto &type : imageTypes)
    for (auto &extension : type->extensions) {
      allExtensions.insert(extension);
      if (ext == extension) {
        img = type.get();
      }
    }

  if (allExtensions.find(ext) == allExtensions.end()) {
    std::cout << "This filetype is not supported" << std::endl;
    return -3;
  }

  try {
    img->read(argv[2]);
  } catch (std::runtime_error &error) {
    std::cerr << error.what() << std::endl;
    return -4;
  }

  std::unique_ptr<Effect> effect;
  effects[argv[1]]->apply(img->width, img->height, img->data);

  try {
    img->write(newFilename.c_str());
  } catch (std::runtime_error &error) {
    std::cerr << error.what() << std::endl;
    return -5;
  }
  std::cout << "Successfully written to " << newFilename << std::endl;
  return 0;
}
