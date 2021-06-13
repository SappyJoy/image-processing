#pragma once

#include <vector>
#include <string>

/**
 * Base image type class
 */
class ImageType {
public:
  /**
   * 2D array of image pixels
   */
  std::vector<std::vector<Pixel>> data;
  std::vector<std::string> extensions;
  int width;
  int height;

  /**
   * Reads the content of image
   * @param fname Filename of image
   */
  virtual void read(const char *fname) = 0;

  /**
   * Creates or rewrites new image
   * @param fname Filename of image
   */
  virtual void write(const char *fname) = 0;

  virtual ~ImageType() = default;
};

using create_image_type_fn = std::add_pointer<ImageType *()>::type;