//
// Created by sappy-joy on 15/06/2021.
//

#include <iostream>
#include "../../include/effects/BlurEffect.h"

// Algorithm 1
// O(n * r^2)
/*
void BlurEffect::apply(int width, int height, std::vector<std::vector<Pixel>> &data) {
  double r = 2;
  int rs = ceil(r * 2.57); // Significant radius
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      for (int color = 0; color < 3; color++) {
        double val = 0;
        double wsum = 0;
        for (int iy = y - rs; iy < y + rs + 1; iy++) {
          for (int ix = x - rs; ix < x + rs + 1; ix++) {
            int X = std::min(width - 1, std::max(0, ix));
            int Y = std::min(height - 1, std::max(0, iy));
            int dsq = (ix - x) * (ix - x) + (iy - y) * (iy - y);
            double weight = std::exp(-dsq / (2 * r * r)) / (M_PI * 2 * r * r);
            val += data[Y][X][color] * weight;
            wsum += weight;
          }
        }
        data[y][x][color] = round(val / wsum);
      }
    }
  }
}
*/

/**
 *
 * @param sigma Standard deviation
 * @param n Number of boxes
 * @return
 */

std::vector<int> boxesForGauss(double sigma, int n) {
  double wIdeal = std::sqrt((12 * sigma * sigma / n) + 1);  // Ideal averaging filter width
  int wl = std::floor(wIdeal);
  if (wl % 2 == 0) wl--;
  int wu = wl + 2;

  double mIdeal = (12*sigma*sigma - n*wl*wl - 4*n*wl - 3*n) / (-4*wl - 4);
  int m = (int)std::round(mIdeal);

  std::vector<int> sizes;
  sizes.reserve(n);
  for (int i = 0; i < n; i++)
    sizes.push_back(i < m ? wl : wu);

  return sizes;
}

// Algorithm 2
/*
void boxBLur2(int width, int height, std::vector<std::vector<Pixel>> &data, int r) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      for (int color = 0; color < 3; color++) {
        int val = 0;
        for (int iy = y - r; iy < y + r + 1; iy++) {
          for (int ix = x - r; ix < x + r + 1; ix++) {
            int X = std::min(width - 1, std::max(0, ix));
            int Y = std::min(height - 1, std::max(0, iy));
            val += data[Y][X][color];
          }
        }
        data[y][x][color] = val / ((r+r+1)*(r+r+1));
      }
    }
  }
}

void BlurEffect::apply(int width, int height, std::vector<std::vector<Pixel>> &data) {
  auto bxs = boxesForGauss(sigma, 3);
  boxBLur2(width, height, data, (bxs[0]-1)/2);
  boxBLur2(width, height, data, (bxs[1]-1)/2);
  boxBLur2(width, height, data, (bxs[2]-1)/2);
}
*/

// Algorithm 3
void boxBlurHorizontal(int width, int height, std::vector<std::vector<Pixel>> &data, int r) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      for (int color = 0; color < 3; color++) {
        int val = 0;
        for (int ix = x - r; ix < x + r + 1; ix++) {
          int X = std::min(width - 1, std::max(0, ix));
          val += data[y][X][color];
        }
        data[y][x][color] = val / (r+r+1);
      }
    }
  }
}

void boxBlurTotal(int width, int height, std::vector<std::vector<Pixel>> &data, int r) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      for (int color = 0; color < 3; color++) {
        int val = 0;
        for (int iy = y - r; iy < y + r + 1; iy++) {
          int Y = std::min(height - 1, std::max(0, iy));
          val += data[Y][x][color];
        }
        data[y][x][color] = val / (r+r+1);
      }
    }
  }
}

void boxBLur3(int width, int height, std::vector<std::vector<Pixel>> &data, int r) {
  boxBlurHorizontal(width, height, data, r);
  boxBlurTotal(width, height, data, r);
}

void BlurEffect::apply(int width, int height, std::vector<std::vector<Pixel>> &data) {
 auto bxs = boxesForGauss(sigma, 3);
 boxBLur3(width, height, data, (bxs[0]-1)/2);
 boxBLur3(width, height, data, (bxs[1]-1)/2);
 boxBLur3(width, height, data, (bxs[2]-1)/2);
}

// Algorithm 4
/*
void boxBlurHorizontal(int width, int height, std::vector<std::vector<Pixel>> &data, int r) {
  double iarr = 1 / (double)(r+r+1);

  for (int color = 0; color < 3; color++) {
    for (int y = 0; y < height; y++) {
      int ti = y * width;
      int li = ti;
      int ri = ti + r;
      int fv = data[y][0][color];
      int lv = data[y][width-1][color];
      int val = (r + 1) * fv;
      for (int x = 0; x < r; x++)
        val += data[y][x][color];
      for (int x = 0; x <= r; x++) {
        val += data[ri/width][ri%width][color] - fv;
        data[ti/width][ti%width][color] = std::round(val * iarr);
        ri++;
        ti++;
      }
      for (int x = r+1; x < width-r; x++) {
        val += data[ri/width][ri%width][color] - data[li/width][li%width][color];
        data[ti/width][ti%width][color] = std::round(val * iarr);
        ri++;
        li++;
        ti++;
      }
      for (int x = width-r; x < width; x++) {
        val += lv - data[li/width][li%width][color];
        data[ti/width][ti%width][color] = std::round(val * iarr);
        li++;
        ti++;
      }
    }
  }
}

void boxBlurTotal(int width, int height, std::vector<std::vector<Pixel>> &data, int r) {
  double iarr = 1 / (double)(r+r+1);

  for (int color = 0; color < 3; color++) {
    for (int x = 0; x < width; x++) {
      int ti = x;
      int li = ti;
      int ri = ti + r*width;
      int fv = data[0][x][color];
      int lv = data[height-1][x][color];
      int val = (r + 1) * fv;
      for (int y = 0; y < r; y++)
        val += data[y][x][color];
      for (int y = 0; y <= r; y++) {
        val += data[ri/width][ri%width][color] - fv;
        data[ti/width][ti%width][color] = std::round(val * iarr);
        ri += width;
        ti += width;
      }
      for (int y = r+1; y < height-r; y++) {
        val += data[ri/width][ri%width][color] - data[li/width][li%width][color];
        data[ti/width][ti%width][color] = std::round(val * iarr);
        ri += width;
        li += width;
        ti += width;
      }
      for (int y = height-r; y < height; y++) {
        val += lv - data[li/width][li%width][color];
        data[ti/width][ti%width][color] = std::round(val * iarr);
        li += width;
        ti += width;
      }
    }
  }
}

void boxBLur(int width, int height, std::vector<std::vector<Pixel>> &data, int r) {
  boxBlurHorizontal(width, height, data, r);
  boxBlurTotal(width, height, data, r);
}

void BlurEffect::apply(int width, int height, std::vector<std::vector<Pixel>> &data) {
  auto bxs = boxesForGauss(sigma, 3);
  boxBLur(width, height, data, (bxs[0]-1)/2);
  boxBLur(width, height, data, (bxs[1]-1)/2);
  boxBLur(width, height, data, (bxs[2]-1)/2);
}
*/

void BlurEffect::readParameters(boost::program_options::options_description &desc, int argc, char **argv) {
  namespace po = boost::program_options;
  desc.add_options()
      ("sigma", po::value<double>()->default_value(1.25), "Blur degree (preferably 0.7 to 10)")
  ;
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
  po::notify(vm);

  if (vm.count("sigma")) {
    sigma = vm["sigma"].as<double>();
  }
}

extern "C" {
  Effect *create_effect() {
    return new BlurEffect;
  }
}