#pragma once

class FileType {
public:
  std::vector<std::vector<Pixel>> data;
  int width;
  int height;

  virtual void read(const char *fname) = 0;
  virtual void write(const char *fname) = 0;
};