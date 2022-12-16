#pragma once

#include <cstdint>
#include <vector>
#include <string>

#pragma pack(push, 1)
struct BMPFileHeader
{
    uint16_t file_type      {0x4d42};
    uint32_t file_size      {0};
    uint16_t reserved1      {0};
    uint16_t reserved2      {0};
    uint32_t offset_data    {0};
};

struct BMPInfoHeader
{
    uint32_t size               {sizeof(BMPInfoHeader)};
    int32_t width               {0};
    int32_t height              {0};

    uint16_t planes             {1};
    uint16_t bit_count          {0};
    uint32_t compression        {0};    // uncompressed
    uint32_t size_image         {0};    // uncompressed
    int32_t x_pixels_per_meter  {0};
    int32_t y_pixels_per_meter  {0};
    uint32_t colors_used        {0};
    uint32_t colors_important   {0};
};
#pragma pack(pop)

struct BMP
{
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<uint8_t> data;

    BMP(int width, int height, int channels = 1);
    BMP(const std::string& path);
    ~BMP();
    int size() const;
    void load(const std::string& path);
    void save(const std::string& path);
};
