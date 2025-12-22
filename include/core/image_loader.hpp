#ifndef CORE_IMAGE_LOADER_HPP
#define CORE_IMAGE_LOADER_HPP

#include "image.hpp"
#include <string>
#include <memory>
#include <vector>
#include <fstream>

// STB_IMAGE_IMPLEMENTATION在cpp文件中定义
#include "../stb/stb_image.h"

class ImageLoader {
public:
    // 从文件加载图像
    static std::shared_ptr<Image> load_from_file(const std::string& filename, int desired_channels = 0) {
        int width, height, channels;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, desired_channels);
        
        if (!data) {
            std::cerr << "Error: Failed to load image '" << filename << "': " << stbi_failure_reason() << std::endl;
            return nullptr;
        }
        
        // 如果指定了desired_channels，使用该值，否则使用原始通道数
        int actual_channels = desired_channels > 0 ? desired_channels : channels;
        
        // 创建Image对象并复制数据
        auto image = std::make_shared<Image>(width, height, actual_channels);
        std::copy(data, data + width * height * actual_channels, image->ptr<uint8_t>());
        
        // 释放stb分配的内存
        stbi_image_free(data);
        
        return image;
    }
    
    // 从内存加载图像
    static std::shared_ptr<Image> load_from_memory(const unsigned char* buffer, int len, int desired_channels = 0) {
        int width, height, channels;
        unsigned char* data = stbi_load_from_memory(buffer, len, &width, &height, &channels, desired_channels);
        
        if (!data) {
            std::cerr << "Error: Failed to load image from memory: " << stbi_failure_reason() << std::endl;
            return nullptr;
        }
        
        int actual_channels = desired_channels > 0 ? desired_channels : channels;
        
        auto image = std::make_shared<Image>(width, height, actual_channels);
        std::copy(data, data + width * height * actual_channels, image->ptr<uint8_t>());
        
        stbi_image_free(data);
        
        return image;
    }
    
    // 获取图像信息（不加载完整图像数据）
    static bool get_image_info(const std::string& filename, int& width, int& height, int& channels) {
        return stbi_info(filename.c_str(), &width, &height, &channels) != 0;
    }
    
    // 检查文件格式是否支持
    static bool is_supported_format(const std::string& filename) {
        // 获取文件扩展名
        std::string ext = filename.substr(filename.find_last_of('.') + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        // STB支持的格式
        const std::string supported_formats[] = {
            "jpg", "jpeg", "png", "bmp", "tga", "psd", "gif", "hdr", "pic", "pnm"
        };
        
        for (const auto& format : supported_formats) {
            if (ext == format) {
                return true;
            }
        }
        
        return false;
    }
    
    // 保存图像为PPM格式（简化实现）
    static bool save_as_ppm(const Image& image, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
            return false;
        }
        
        if (image.channels() == 1) {
            // PGM (grayscale)
            file << "P2\n";
            file << image.width() << " " << image.height() << "\n";
            file << "255\n";
            
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    file << static_cast<int>(image.at<uint8_t>(y, x, 0));
                    if (x < image.width() - 1) file << " ";
                }
                file << "\n";
            }
        } else if (image.channels() == 3) {
            // PPM (color)
            file << "P3\n";
            file << image.width() << " " << image.height() << "\n";
            file << "255\n";
            
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    for (int c = 0; c < 3; ++c) {
                        file << static_cast<int>(image.at<uint8_t>(y, x, c));
                        if (c < 2 || x < image.width() - 1) file << " ";
                    }
                }
                file << "\n";
            }
        } else {
            std::cerr << "Error: Only 1 or 3 channel images supported for PPM saving" << std::endl;
            return false;
        }
        
        file.close();
        return true;
    }
    
    // 保存图像为二进制PPM格式
    static bool save_as_ppm_binary(const Image& image, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
            return false;
        }
        
        if (image.channels() == 1) {
            // PGM binary
            file << "P5\n";
            file << image.width() << " " << image.height() << "\n";
            file << "255\n";
            
            for (int y = 0; y < image.height(); ++y) {
                file.write(reinterpret_cast<const char*>(image.ptr<uint8_t>(y)), image.width());
            }
        } else if (image.channels() == 3) {
            // PPM binary
            file << "P6\n";
            file << image.width() << " " << image.height() << "\n";
            file << "255\n";
            
            for (int y = 0; y < image.height(); ++y) {
                file.write(reinterpret_cast<const char*>(image.ptr<uint8_t>(y)), image.width() * 3);
            }
        } else {
            std::cerr << "Error: Only 1 or 3 channel images supported for PPM binary saving" << std::endl;
            return false;
        }
        
        file.close();
        return true;
    }
    
    // 保存图像为PGM格式（灰度）
    static bool save_as_pgm(const Image& image, const std::string& filename) {
        if (image.channels() != 1) {
            std::cerr << "Error: Only single channel images can be saved as PGM" << std::endl;
            return false;
        }
        return save_as_ppm(image, filename);
    }
    
    // 保存图像为PGM二进制格式
    static bool save_as_pgm_binary(const Image& image, const std::string& filename) {
        if (image.channels() != 1) {
            std::cerr << "Error: Only single channel images can be saved as PGM binary" << std::endl;
            return false;
        }
        return save_as_ppm_binary(image, filename);
    }
    
private:
    ImageLoader() = default; // 静态类，禁止实例化
};

// 图像保存工具类
namespace image_saver {
    
inline bool save(const Image& image, const std::string& filename) {
    // 根据扩展名选择保存格式
    std::string ext = filename.substr(filename.find_last_of('.') + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == "ppm") {
        return ImageLoader::save_as_ppm(image, filename);
    } else if (ext == "pgm") {
        return ImageLoader::save_as_pgm(image, filename);
    } else {
        std::cerr << "Error: Unsupported output format: " << ext << std::endl;
        std::cerr << "Supported formats: .ppm, .pgm" << std::endl;
        return false;
    }
}

inline bool save_binary(const Image& image, const std::string& filename) {
    std::string ext = filename.substr(filename.find_last_of('.') + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == "ppm") {
        return ImageLoader::save_as_ppm_binary(image, filename);
    } else if (ext == "pgm") {
        return ImageLoader::save_as_pgm_binary(image, filename);
    } else {
        std::cerr << "Error: Unsupported output format: " << ext << std::endl;
        std::cerr << "Supported formats: .ppm, .pgm" << std::endl;
        return false;
    }
}

} // namespace image_saver

#endif // CORE_IMAGE_LOADER_HPP