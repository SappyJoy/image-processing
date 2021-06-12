#pragma once

class ImageType {
public:
  std::vector<std::vector<Pixel>> data;
  std::vector<std::string> extensions;
  int width;
  int height;

  virtual void read(const char *fname) = 0;
  virtual void write(const char *fname) = 0;

  virtual ~ImageType() = default;
};

using create_image_type_fn = std::add_pointer<ImageType *()>::type;