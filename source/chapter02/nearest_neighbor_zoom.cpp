#include <core/core.hpp>
#include <iostream>
#include <cmath>
#include <fstream>

// 最近邻插值放大：scale为放大倍数
Image nearestNeighborZoom(const Image& img, float scale) {
    int M = img.height();
    int N = img.width();
    int newM = M * scale;
    int newN = N * scale;
    Image res(newN, newM, img.channels()); // 注意：width, height顺序
    
    for (int i = 0; i < newM; i++) {
        for (int j = 0; j < newN; j++) {
            // 映射到原始图像浮点坐标
            float origX = i / scale;
            float origY = j / scale;
            // 取最近整数坐标
            int x = floor(origX + 0.5);
            int y = floor(origY + 0.5);
            // 边界处理：防止越界
            x = x < 0 ? 0 : (x >= M ? M-1 : x);
            y = y < 0 ? 0 : (y >= N ? N-1 : y);
            
            // 复制所有通道
            for (int c = 0; c < img.channels(); c++) {
                res.set_pixel(j, i, c, img.get_pixel(y, x, c)); // 注意坐标顺序
            }
        }
    }
    return res;
}

int main() {
    std::cout << "Chapter 2.4: Nearest Neighbor Interpolation" << std::endl;
    std::cout << "============================================" << std::endl;
    
    // 加载图像
    std::string filename = "test.jpg";
    auto img = ImageLoader::load_from_file(filename);
    
    if (!img || img->empty()) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        std::cout << "Please make sure 'test.jpg' exists in the current directory." << std::endl;
        return 1;
    }
    
    std::cout << "Loaded image: " << img->width() << "x" << img->height() 
              << ", " << img->channels() << " channels" << std::endl;
    
    // 放大10倍
    float scale = 10.0f;
    std::cout << "Scaling image by factor of " << scale << "..." << std::endl;
    
    auto zoomed_img = nearestNeighborZoom(*img, scale);
    
    std::cout << "Resulting image: " << zoomed_img.width() << "x" << zoomed_img.height() 
              << ", " << zoomed_img.channels() << " channels" << std::endl;
    
    // 保存结果图像（简化为PGM格式，只保存第一个通道）
    std::string output_filename = "zoomed_test.pgm";
    std::ofstream outfile(output_filename);
    if (outfile.is_open()) {
        // 写入PGM头部
        outfile << "P2\n";
        outfile << zoomed_img.width() << " " << zoomed_img.height() << "\n";
        outfile << "255\n";
        
        // 写入像素数据（只取第一个通道，如果是彩色图像的话）
        for (int y = 0; y < zoomed_img.height(); y++) {
            for (int x = 0; x < zoomed_img.width(); x++) {
                int pixel_val = static_cast<int>(zoomed_img.get_pixel(x, y, 0));
                outfile << pixel_val;
                if (x < zoomed_img.width() - 1) outfile << " ";
            }
            outfile << "\n";
        }
        outfile.close();
        std::cout << "Saved zoomed image as: " << output_filename << std::endl;
    } else {
        std::cerr << "Failed to save image file." << std::endl;
    }
    
    return 0;
}