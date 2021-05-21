//
// Created by sappy-joy on 20/05/2021.
//

#include <fstream>
#include <iostream>
#include "../../include/filetypes/BMP.h"

BMP::BMP(const char *fname) {
  read(fname);
}

//BMP::BMP(int32_t width, int32_t height, bool hasAlpha) {
//
//}

void BMP::checkColorHeader(BMPColorHeader &bmpColorHeader) {
  BMPColorHeader expectedColorHeader;
  if (expectedColorHeader.redMask != bmpColorHeader.redMask ||
      expectedColorHeader.greenMask != bmpColorHeader.greenMask ||
      expectedColorHeader.blueMask != bmpColorHeader.blueMask ||
      expectedColorHeader.alphaMask != bmpColorHeader.alphaMask)
    throw std::runtime_error("Unexpected color mask format! The program expects the pixel dataBytes to be in the BGRA format");

  if (expectedColorHeader.colorSpaceType != bmpColorHeader.colorSpaceType)
    throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
}

uint32_t BMP::makeStrideAligned(int alignStride) {
  uint32_t newStride = rowStride;
  while (newStride % alignStride != 0) {
    newStride++;
  }
  return newStride;
//  return rowStride + alignStride - (rowStride % alignStride);
}

void BMP::read(const char *fname) {
  std::ifstream inp{ fname, std::ios_base::binary };
  if (inp) {
    inp.read((char *)&fileHeader, sizeof(fileHeader));
    if (fileHeader.fileType != 0x4D42)
      throw std::runtime_error("Error! Unrecognized file format.");

    inp.read((char *)&infoHeader, sizeof(infoHeader));

    // BMPColorHeader is used only for transparent images
    if (infoHeader.bitCount == 32) {
      // Check if the file has bit mask color information
      if (infoHeader.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
        inp.read((char *)&colorHeader, sizeof(BMPColorHeader));
        checkColorHeader(colorHeader);
      } else {
        std::cerr << "Warning! The file \"" << fname << "\" does not seem to contain bit mask information\n";
        throw std::runtime_error("Error! Unrecognized file format.");
      }
    }

    width = infoHeader.width;
    height = infoHeader.height;

    // Jump to the pixel dataBytes location
    inp.seekg(fileHeader.offsetData, inp.beg);

    // Adjust the header fields for output.
    // Some editors will put extra info in the image file, we only save the headers and the dataBytes.
    if (infoHeader.bitCount == 32) {
      infoHeader.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
      fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
    } else {
      infoHeader.size = sizeof(BMPInfoHeader);
      fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    }
    fileHeader.fileSize = fileHeader.offsetData;

    if (infoHeader.height < 0)
      throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");

    rawData.resize(infoHeader.width * infoHeader.height * infoHeader.bitCount / 8);

    // Here we check if we need to take into account row padding
    if (infoHeader.width % 4 == 0) {
      inp.read((char *)rawData.data(), rawData.size());
      fileHeader.fileSize += rawData.size();
    } else {
      rowStride = infoHeader.width * infoHeader.bitCount / 8;
      uint32_t newStride = makeStrideAligned(4);
      std::vector<uint8_t> paddingRow(newStride - rowStride);

      for (int y = 0; y < infoHeader.height; y++) {
        inp.read((char*)(rawData.data() + rowStride * y), rowStride);
        inp.read((char *)paddingRow.data(), paddingRow.size());
      }
      fileHeader.fileSize += rawData.size() + infoHeader.height * paddingRow.size();
    }

    data.resize(infoHeader.height);
    for (int y = 0; y < infoHeader.height; y++) {
      data[y].resize(infoHeader.width);
      for (int x = 0; x < infoHeader.width; x++) {
        data[y][x].B = rawData[(y*infoHeader.width + x) * infoHeader.bitCount / 8];
        data[y][x].G = rawData[(y*infoHeader.width + x) * infoHeader.bitCount / 8 + 1];
        data[y][x].R = rawData[(y*infoHeader.width + x) * infoHeader.bitCount / 8 + 2];
      }
    }
  } else
    throw std::runtime_error("Unable to open the input image file.");
}

void BMP::write(const char *fname) {
  std::ofstream of{ fname, std::ios_base::binary };
  if (of) {
    if (infoHeader.bitCount == 32) {
      writeHeadersAndData(of);
    } else if (infoHeader.bitCount == 24) {
      if (infoHeader.width % 4 == 0) {
        writeHeadersAndData(of);
      } else {
        uint32_t newStride = makeStrideAligned(4);
        std::vector<uint8_t> paddingRow(newStride - rowStride);

        writeHeaders(of);
        updateRawData();

        for (int y = 0; y < infoHeader.height; ++y) {
          of.write((const char*)(rawData.data() + rowStride * y), rowStride);
          of.write((const char*)paddingRow.data(), paddingRow.size());
        }
      }
    } else {
      throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
    }
  } else
    throw std::runtime_error("Unable to open the output image file.");
}

void BMP::writeHeaders(std::ofstream &of) {
  of.write((const char*)&fileHeader, sizeof(fileHeader));
  of.write((const char*)&infoHeader, sizeof(infoHeader));
  if(infoHeader.bitCount == 32) {
    of.write((const char*)&colorHeader, sizeof(colorHeader));
  }
}

void BMP::writeHeadersAndData(std::ofstream &of) {
  writeHeaders(of);
  updateRawData();
  of.write((const char*)rawData.data(), rawData.size());
}

void BMP::updateRawData() {
  for (int y = 0; y < infoHeader.height; y++) {
    for (int x = 0; x < infoHeader.width; x++) {
      rawData[(y*infoHeader.width + x) * infoHeader.bitCount / 8] = data[y][x].B;
      rawData[(y*infoHeader.width + x) * infoHeader.bitCount / 8 + 1] = data[y][x].G;
      rawData[(y*infoHeader.width + x) * infoHeader.bitCount / 8 + 2] = data[y][x].R;
    }
  }
}
