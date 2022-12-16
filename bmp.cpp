#include "bmp.h"
#include <fstream>
#include <stdexcept>

BMP::BMP(int width, int height, int channels): width(width), height(height), channels(channels), data(size())
{}

BMP::BMP(const std::string &path)
{
    load(path);
}

BMP::~BMP()
{}

int BMP::size() const
{
    return width * height * channels;
}

void BMP::load(const std::string& path)
{
    // open file
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    // read BMPFileHeader
    BMPFileHeader file_header;
    file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    if (file_header.file_type != 0x4D42) {
        throw std::runtime_error("Unrecognized file format");
    }

    // read BMPInfoHeader
    BMPInfoHeader info_header;
    file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));
    if (info_header.compression != 0) {
        throw std::runtime_error("Compressed files are not supported");
    }

    // read bmp data
    int padded_width_size = ((info_header.width * info_header.bit_count + 31) / 32) * 4;
    int data_size = padded_width_size * info_header.height;
    std::vector<uint8_t> pixel_data(data_size);
    file.seekg(file_header.offset_data, std::ios::beg);
    file.read(reinterpret_cast<char*>(pixel_data.data()), pixel_data.size());

    // assign width, height, and channels
    width = info_header.width;
    height = info_header.height;
    channels = info_header.bit_count / 8;

    // assing raw pixel data to std::vector<uint8_t>
    data.resize(width * height * channels);
    for (int i = 0; i < info_header.height; ++i) {
        auto ptr = pixel_data.data() + i * padded_width_size;
        std::copy(ptr, ptr + width * channels, &data.at(i * width * channels));
    }
};

void BMP::save(const std::string& path)
{
    // open file
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    // header
    BMPFileHeader file_header;
    BMPInfoHeader info_header {
            .width = width,
            .height = height,
            .bit_count = static_cast<uint16_t>(8 * channels)
    };

    // color palette (for grayscale)
    uint32_t color_palette[256] = {0};
    if (info_header.bit_count == 8) {
        for (auto i = 1; i < 256; ++i) {
            color_palette[i] = color_palette[i - 1] + 0x00010101u;
        }
    }

    // pad data
    int padded_width_size = ((info_header.width * info_header.bit_count + 31) / 32) * 4;
    uint32_t data_size = padded_width_size * info_header.height;
    std::vector<uint8_t> pixel_data(data_size);
    for (int i = 0; i < height; ++i) {
        auto ptr = &data.at(i * width * channels);
        std::copy(ptr, ptr + width * channels, &pixel_data[i * padded_width_size]);
    }

    // header size, offset, file size
    uint32_t header_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    file_header.offset_data = (info_header.bit_count == 8) ? header_size + sizeof(color_palette) : header_size;
    file_header.file_size = file_header.offset_data + data_size;

    // write header and padded data
    file.write(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    file.write(reinterpret_cast<char*>(&info_header), sizeof(info_header));
    if (info_header.bit_count == 8) {
        file.write(reinterpret_cast<char*>(color_palette), sizeof(color_palette));
    }
    file.write(reinterpret_cast<const char*>(pixel_data.data()), data_size);
}
