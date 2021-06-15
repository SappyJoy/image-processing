#pragma once

#include <cstdint>
/**
 * Representation of a pixel having 3 colors with values from -256 to 255
 * (so the pixels can be subtracted)
 */
struct Pixel {
  int16_t R;
  int16_t G;
  int16_t B;

  Pixel() = default;
  Pixel(int16_t R, int16_t G, int16_t B) : R(R), G(G), B(B) { }

  Pixel &operator+=(Pixel p) {
    this->add(p);
    return *this;
  }

  Pixel&operator-=(Pixel p) {
    this->sub(p);
    return *this;
  }

  Pixel operator+(Pixel const &p) const {
    Pixel tmp(*this);
    tmp.add(p);
    return tmp;
  }

  Pixel operator-(Pixel const &p) const {
    Pixel tmp(*this);
    tmp.sub(p);
    return tmp;
  }

  Pixel operator+(int16_t p) const {
    Pixel tmp(*this);
    tmp.add(Pixel(p, p, p));
    return tmp;
  }

  Pixel operator-(int16_t p) const {
    Pixel tmp(*this);
    tmp.sub(Pixel(p, p, p));
    return tmp;
  }

  Pixel operator*(double p) const {
    if (p < 0) p = 0;
    Pixel tmp(*this);
    tmp.R = std::min(tmp.R * p, 255.0);
    tmp.G = std::min(tmp.G * p, 255.0);
    tmp.B = std::min(tmp.B * p, 255.0);
    return tmp;
  }

  int16_t operator[](size_t index) const {
    if (index == 0)
      return R;
    if (index == 1)
      return G;
    if (index == 2)
      return B;
    throw std::runtime_error("You can't use index " + std::to_string(index) + ".");
  }

  int16_t &operator[](size_t index) {
    if (index == 0)
      return R;
    if (index == 1)
      return G;
    if (index == 2)
      return B;
    throw std::runtime_error("You can't use index " + std::to_string(index) + ".");
  }

private:
  void add(Pixel p) {
    R = std::min((int)R + p.R, 255);
    G = std::min((int)G + p.G, 255);
    B = std::min((int)B + p.B, 255);
  }
  void sub(Pixel p) {
    R = std::max((int)R - p.R, 0);
    G = std::max((int)G - p.G, 0);
    B = std::max((int)B - p.B, 0);
  }
};