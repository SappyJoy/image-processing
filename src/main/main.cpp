#include <iostream>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <boost/program_options.hpp>

#include "../../include/filetypes/BMP.h"
#include "../../include/effects/Effect.h"
#include "../../include/loaders/PluginLoader.h"

namespace fs = std::filesystem;
namespace po = boost::program_options;

/**
 * Prints all available effects that found in src/effects/ folder
 * @param effects map<name of effect, effect pointer>
 */
void printEffects(std::unordered_map<std::string, std::unique_ptr<Effect>> &effects) {
  if (effects.empty()) {
    std::cout << "There is no available effects.\n";
    return;
  }
  std::cout << "Available effects:\n";
  for (auto & effect : effects) {
    printf("\t%s\n", effect.first.c_str());
  }
}

/**
 * Prints all available extensions that found in src/filetypes/ folder
 * @param extensions set<extension string>
 */
void printExtensions(std::unordered_set<std::string> &extensions) {
  if (extensions.empty()) {
    std::cout << "There is no supported image types.\n";
    return;
  }
  std::cout << "Available image types:\n";

  auto iter = extensions.begin();
  printf("\t%s", iter->c_str());
  iter++;
  do {
    printf(", %s", iter->c_str());
  } while (++iter != extensions.end());
  printf("\n");
}

/**
 * Loads all effects from src/effects/ folder
 * @return map<name of effect, effect pointer>
 */
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

/**
 * Loads all image types from src/filetypes/ folder
 * @return set<extension string>
 */
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

  const char *ext;
  std::string inputFilename;
  std::string outputFilename;
  std::string effectName;

  auto effects = loadEffects();
  auto imageTypes = loadImageTypes();

  // Find all extensions
  std::unordered_set<std::string> allExtensions;
  for (auto &type : imageTypes)
    for (auto &extension : type->extensions)
      allExtensions.insert(extension);

  // Process program options
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("effect,e", po::value<std::string>(), "effect name")
        ("input,i", po::value<std::string>(), "set input file name")
        ("output,o", po::value<std::string>(), "set output file name")
    ;

    // Required options
    po::positional_options_description p;
    p.add("effect", 1);
    p.add("input", 1);

    // Collect all options
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).allow_unregistered().run(), vm);
    po::notify(vm);

    po::options_description effectDesc("Options for effect");
    if (vm.count("effect")) {
      effectName = vm["effect"].as<std::string>();
      effects[effectName]->readParameters(effectDesc, argc, argv);
    }

    if (vm.count("input")) {
      inputFilename = vm["input"].as<std::string>();
      ext = fs::path(inputFilename).extension().c_str();
    }

    if (!vm.count("effect") || !vm.count("input") || vm.count("help")) {
      std::cout << "Usage: effect effect_name input_file_name [options]\n";
      std::cout << desc << "\n";
      if (vm.count("effect"))
        std::cout << effectDesc << "\n";
      printEffects(effects);
      std::cout << "\n";
      printExtensions(allExtensions);
      return 0;
    }

    if (vm.count("output")) {
      outputFilename = vm["output"].as<std::string>();
    } else {
      outputFilename = inputFilename.substr(0, inputFilename.size()-std::string(ext).size()) + "_" + effectName + ext;
    }

  } catch (std::exception &e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Exception of unknown type!\n";
  }

  if (effects.find(effectName) == effects.end()) {
    std::cerr << "Wrong effect name" << std::endl;
    return -2;
  }

  if (allExtensions.find(ext) == allExtensions.end()) {
    std::cerr << "This filetype is not supported" << std::endl;
    return -3;
  }

  ImageType *img;

  // Find our image type
  for (auto &type : imageTypes)
    for (auto &extension : type->extensions)
      if (ext == extension)
        img = type.get();

  // Try to read image
  try {
    img->read(inputFilename.c_str());
  } catch (std::runtime_error &error) {
    std::cerr << error.what() << std::endl;
    return -4;
  }

  // Apply effect
  std::unique_ptr<Effect> effect;
  auto start = std::chrono::system_clock::now();
  effects[effectName]->apply(img->width, img->height, img->data);
  auto end = std::chrono::system_clock::now();

  // stats
  float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
  std::cout << "Time: " << elapsed << " ms" << std::endl;

  // Try to write image
  try {
    img->write(outputFilename.c_str());
  } catch (std::runtime_error &error) {
    std::cerr << error.what() << std::endl;
    return -5;
  }
  std::cout << "Successfully written to " << outputFilename << std::endl;
  return 0;
}