#pragma once
#include "../Pixel.h"
#include "ImageType.h"

#pragma pack(push, 1)
struct BMPFileHeader {
  uint16_t fileType{ 0x4D42 }; // File type always BM which is 0x4D42
  uint32_t fileSize{ 0 };      // Size of the file (in bytes)
  uint16_t reserved1{ 0 };     // Reserved, always 0
  uint16_t reserved2{ 0 };     // Reserved, always 0
  uint32_t offsetData{ 0 };    // Start position of pixel dataBytes (bytes from the beginning of the file)
};

struct BMPInfoHeader {
  uint32_t size{ 0 };               // Size of this header (in bytes)
  int32_t width{ 0 };               // Width of bitmap in pixels
  int32_t height{ 0 };              // Height of bitmap in pixels
                                    //   (if positive, bottom-up, with origin in lower left corner)
                                    //   (if negative, top-down, with origin in upper left corner)
  uint16_t planes{ 1 };             // Number of planes for the target device, this is always 1
  uint16_t bitCount{ 0 };           // No. of bits per pixel
  uint32_t compression{ 0 };        // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
                                    // 0 - for uncompressed images
  uint32_t sizeImage{ 0 };
  int32_t xPixelsPerMeter{ 0 };
  int32_t yPixelsPerMeter{ 0 };
  uint32_t colorsUsed{ 0 };         // No. color indexes in the color table.
                                    // Use 0 for the max number of colors allowed by bit_count
  uint32_t colorsImportant{ 0 };    // No. of colors used for displaying the bitmap. If 0 all colors are required
};

struct BMPColorHeader {
  uint32_t redMask{ 0x00ff0000 };          // Bit mask for the red channel
  uint32_t greenMask{ 0x0000ff00 };        // Bit mask for the green channel
  uint32_t blueMask{ 0x000000ff };         // Bit mask for the blue channel
  uint32_t alphaMask{ 0xff000000 };        // Bit mask for the alpha channel
  uint32_t colorSpaceType{ 0x73524742 };   // Default "sRGB" (0x73524742)
  uint32_t unused[16]{ 0 };                // Unused dataBytes for sRGB color space
};
#pragma pack(pop)


class BMP : public ImageType {
public:
  BMPFileHeader fileHeader;
  BMPInfoHeader infoHeader;
  BMPColorHeader colorHeader;
  std::vector<uint8_t> rawData;

  BMP(const char *fname);
  BMP();

  void read(const char *fname) override;
  void write(const char *fname) override;

private:
  uint32_t rowStride{ 0 };

  uint32_t makeStrideAligned(int alignStride) const;

  void updateRawData();

  void writeHeaders(std::ofstream &of) const;
  void writeHeadersAndData(std::ofstream &of);

  // Check if the pixel dataBytes is stored as BGRA and if the color space type is sRGB
  void checkColorHeader(BMPColorHeader &bmpColorHeader) const;
};

extern "C" {
  ImageType *create_image_type();
}