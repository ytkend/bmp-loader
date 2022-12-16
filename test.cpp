#include <iostream>
#include <algorithm>
#include <utility>
#include <cmath>
#include "bmp.h"

template <class T>
std::vector<T> normalize(const std::vector<T>& in, T min, T max)
{
    auto minmax_val = std::minmax_element(in.data(), in.data() + in.size());
    auto min_val = *minmax_val.first;
    auto max_val = *minmax_val.second;
    auto a = (max - min) / (max_val - min_val);
    std::vector<T> out(in.size());
    std::transform(in.begin(), in.end(), out.begin(),
                   [=](auto&& i){return (i - min_val) * a + min;});
    return out;
}

template <class T>
std::vector<uint8_t> to_uint8(const std::vector<T>& in)
{
    std::vector<T> in_norm = normalize<T>(in, 0, 255);
    std::vector<uint8_t> out(in_norm.begin(), in_norm.end());
    return out;
}

int main(int argc, char* argv[])
{
    // load a bmp image
    std::string load_bmp_path = "../data/Parrots.bmp";
    std::cout << "load: " << load_bmp_path << std::endl;
    BMP bmp_image(load_bmp_path);
    std::cout << "width: " << bmp_image.width << std::endl;
    std::cout << "height: " << bmp_image.height << std::endl;
    std::cout << "channels: " << bmp_image.channels<< std::endl;

    // save a bmp image
    std::string save_bmp_path = "./test.bmp";
    std::cout << "save: " << save_bmp_path << std::endl;
    bmp_image.save(save_bmp_path);

    // generate grayscale data
    int width = 128;
    int height = 128;
    std::vector<float> gray_data(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            gray_data[x + y * width] = std::sqrt(x*x + y*y);
        }
    }
    auto gray_data_u8 = to_uint8(gray_data);

    // save a grayscale image
    BMP gray_image(width, height);
    gray_image.data = std::move((gray_data_u8));
    std::string save_gray_bmp_path = "./test_gray.bmp";
    std::cout << "save: " << save_gray_bmp_path << std::endl;
    gray_image.save(save_gray_bmp_path);

    return 0;
}
