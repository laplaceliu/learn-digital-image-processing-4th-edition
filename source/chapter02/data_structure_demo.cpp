#include <core/core.hpp>
#include <iostream>
#include <iomanip>

void demonstrate_basic_types() {
    std::cout << "=== Basic Types Demonstration ===" << std::endl;
    
    // Point2d
    Point2i pt1(10, 20);
    Point2f pt2(15.5f, 25.5f);
    Point2d pt3 = pt1 + pt2;
    std::cout << "Point operations: " << pt3.x << ", " << pt3.y << std::endl;
    
    // Size
    Size size(640, 480);
    std::cout << "Image size: " << size.width << "x" << size.height 
              << " (area: " << size.area() << ")" << std::endl;
    
    // Rect
    Rect rect(100, 100, 200, 150);
    std::cout << "Rectangle: (" << rect.x << "," << rect.y 
              << ") size: " << rect.width << "x" << rect.height << std::endl;
    std::cout << "Contains (150,120): " << (rect.contains(Point2i(150,120)) ? "yes" : "no") << std::endl;
    
    // Vec
    Vec3b color({255, 128, 64});
    Vec3f color_f(1.0f, 0.5f, 0.25f);
    std::cout << "RGB color: " << (int)color[0] << "," << (int)color[1] << "," << (int)color[2] << std::endl;
    
    // Scalar
    Scalar red(255, 0, 0);
    std::cout << "Red scalar: " << red[0] << "," << red[1] << "," << red[2] << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_matrix() {
    std::cout << "=== Matrix Operations ===" << std::endl;
    
    // 创建矩阵
    Matrix mat(3, 3, DataType::UINT8);
    
    // 填充数据
    for(int i = 0; i < mat.rows(); ++i) {
        for(int j = 0; j < mat.cols(); ++j) {
            mat.at<uint8_t>(i, j) = i * 10 + j;
        }
    }
    
    std::cout << "3x3 Matrix:" << std::endl;
    for(int i = 0; i < mat.rows(); ++i) {
        for(int j = 0; j < mat.cols(); ++j) {
            std::cout << std::setw(3) << (int)mat.at<uint8_t>(i, j) << " ";
        }
        std::cout << std::endl;
    }
    
    // 创建ROI
    Rect roi(1, 1, 2, 2);
    Matrix roi_mat = mat.roi(roi);
    
    std::cout << "\nROI Matrix (1,1 to 2,2):" << std::endl;
    for(int i = 0; i < roi_mat.rows(); ++i) {
        for(int j = 0; j < roi_mat.cols(); ++j) {
            std::cout << std::setw(3) << (int)roi_mat.at<uint8_t>(i, j) << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "Matrix info: " << mat.rows() << "x" << mat.cols() 
              << ", type size: " << mat.elemSize() << " bytes" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_image() {
    std::cout << "=== Image Operations ===" << std::endl;
    
    // 创建灰度图像
    Image gray_image(5, 5, 1);  // 5x5, 1 channel
    
    // 填充梯度
    for(int y = 0; y < gray_image.height(); ++y) {
        for(int x = 0; x < gray_image.width(); ++x) {
            uint8_t value = (x + y) * 25;
            gray_image.set_pixel(x, y, 0, value);
        }
    }
    
    std::cout << "Grayscale image (5x5):" << std::endl;
    for(int y = 0; y < gray_image.height(); ++y) {
        for(int x = 0; x < gray_image.width(); ++x) {
            std::cout << std::setw(3) << (int)gray_image.get_pixel(x, y) << " ";
        }
        std::cout << std::endl;
    }
    
    // 创建彩色图像
    Image color_image(3, 3, 3);  // 3x3, 3 channels
    
    // 填充彩色数据
    for(int y = 0; y < color_image.height(); ++y) {
        for(int x = 0; x < color_image.width(); ++x) {
            Vec3b pixel({static_cast<uint8_t>(x * 85), 
                        static_cast<uint8_t>(y * 85), 
                        static_cast<uint8_t>((x + y) * 42)});
            color_image.set3(y, x, pixel);
        }
    }
    
    std::cout << "\nColor image (3x3, RGB):" << std::endl;
    for(int y = 0; y < color_image.height(); ++y) {
        for(int x = 0; x < color_image.width(); ++x) {
            Vec3b pixel = color_image.at3<uint8_t>(y, x);
            std::cout << "(" << (int)pixel[0] << "," << (int)pixel[1] << "," << (int)pixel[2] << ") ";
        }
        std::cout << std::endl;
    }
    
    // ROI操作
    Rect roi(1, 1, 2, 2);
    Image roi_image = color_image.roi(roi);
    
    std::cout << "\nColor image ROI (1,1 to 2,2):" << std::endl;
    for(int y = 0; y < roi_image.height(); ++y) {
        for(int x = 0; x < roi_image.width(); ++x) {
            Vec3b pixel = roi_image.at3<uint8_t>(y, x);
            std::cout << "(" << (int)pixel[0] << "," << (int)pixel[1] << "," << (int)pixel[2] << ") ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "Color image info: " << color_image.width() << "x" << color_image.height() 
              << ", " << color_image.channels() << " channels" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_type_comparisons() {
    std::cout << "=== Type Size Comparisons ===" << std::endl;
    
    std::cout << "Data type sizes:" << std::endl;
    std::cout << "  UINT8:  " << dataTypeSize(DataType::UINT8) << " bytes" << std::endl;
    std::cout << "  INT16:  " << dataTypeSize(DataType::INT16) << " bytes" << std::endl;
    std::cout << "  FLOAT32: " << dataTypeSize(DataType::FLOAT32) << " bytes" << std::endl;
    std::cout << "  FLOAT64: " << dataTypeSize(DataType::FLOAT64) << " bytes" << std::endl;
    
    // 创建不同类型的矩阵
    Matrix mat_uint8(100, 100, DataType::UINT8);
    Matrix mat_float32(100, 100, DataType::FLOAT32);
    
    std::cout << "\nMemory usage for 100x100 matrix:" << std::endl;
    std::cout << "  UINT8:    " << mat_uint8.total() * mat_uint8.elemSize() << " bytes" << std::endl;
    std::cout << "  FLOAT32:  " << mat_float32.total() * mat_float32.elemSize() << " bytes" << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "Digital Image Processing Data Structures Demo" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << std::endl;
    
    demonstrate_basic_types();
    demonstrate_matrix();
    demonstrate_image();
    demonstrate_type_comparisons();
    
    std::cout << "=== Essential Data Structures for Image Processing ===" << std::endl;
    std::cout << "1. Matrix/Array - Core data container for any dimensional data" << std::endl;
    std::cout << "2. Image - Specialized Matrix for 2D images with channel support" << std::endl;
    std::cout << "3. Point - 2D/3D coordinates for spatial operations" << std::endl;
    std::cout << "4. Size - Image dimensions" << std::endl;
    std::cout << "5. Rect - Rectangular regions for ROI operations" << std::endl;
    std::cout << "6. Vec - Fixed-size vectors for pixel values" << std::endl;
    std::cout << "7. Scalar - Multi-component values for colors and constants" << std::endl;
    std::cout << "8. DataType - Support for various pixel formats (uint8, float32, etc.)" << std::endl;
    
    return 0;
}