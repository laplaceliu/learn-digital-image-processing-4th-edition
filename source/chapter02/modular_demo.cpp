#include <core/core.hpp>
#include <iostream>
#include <iomanip>

// 演示新的模块化设计
void demonstrate_modular_design() {
    std::cout << "=== Modular Header-Only Library Demo ===" << std::endl;
    
    // 使用core命名空间
    using namespace core;
    
    // 基础类型
    Size size(640, 480);
    Point2i pt(100, 200);
    Vec3b color({255, 128, 64});
    Scalar scalar(1.0, 0.5, 0.25);
    
    std::cout << "Size: " << size << ", Area: " << size.area() << std::endl;
    std::cout << "Point: " << pt << std::endl;
    std::cout << "Color: (" << (int)color[0] << "," << (int)color[1] << "," << (int)color[2] << ")" << std::endl;
    std::cout << "Scalar: " << scalar << std::endl;
    
    // 矩阵操作
    Matrix mat = matrix_ops::zeros(3, 3, DataType::FLOAT32);
    std::cout << "\nZero matrix:" << std::endl;
    mat.print<float>();
    
    Matrix identity = matrix_ops::eye(3, DataType::FLOAT32);
    std::cout << "\nIdentity matrix:" << std::endl;
    identity.print<float>();
    
    // 图像操作
    Image img = image_ops::createGrayImage(size, 128);
    std::cout << "\nCreated grayscale image: " << img.width() << "x" << img.height() 
              << ", channels: " << img.channels() << std::endl;
    
    Image color_img = image_ops::createColorImage(Size(100, 100), colors::RED);
    std::cout << "Created color image: " << color_img.width() << "x" << color_img.height() 
              << ", channels: " << color_img.channels() << std::endl;
}

void demonstrate_utils_namespace() {
    std::cout << "\n=== Utils Namespace Demo ===" << std::endl;
    
    using namespace utils;
    
    // 便利函数
    Size size(200, 100);
    Image gray_img = createGrayImage(size, 100);
    Image color_img = createColorImage(Size(100, 100), colors::GREEN);
    
    std::cout << "Created images using utils namespace" << std::endl;
    PRINT_IMG_INFO(gray_img);
    PRINT_IMG_INFO(color_img);
    
    // 图像变换
    Image flipped = flipHorizontal(color_img);
    Image rotated = rotate90(color_img, true);
    Image resized = resize(gray_img, Size(400, 200));
    
    std::cout << "Applied transformations:" << std::endl;
    std::cout << "Flipped: " << flipped.width() << "x" << flipped.height() << std::endl;
    std::cout << "Rotated: " << rotated.width() << "x" << rotated.height() << std::endl;
    std::cout << "Resized: " << resized.width() << "x" << resized.height() << std::endl;
}

void demonstrate_advanced_features() {
    std::cout << "\n=== Advanced Features Demo ===" << std::endl;
    
    using namespace core;
    
    // 矩阵运算
    Matrix a = matrix_ops::zeros(2, 2, DataType::FLOAT32);
    Matrix b = matrix_ops::eye(2, DataType::FLOAT32);
    
    // 手动填充矩阵a
    float* a_data = a.ptr<float>();
    a_data[0] = 1.0f; a_data[1] = 2.0f;
    a_data[2] = 3.0f; a_data[3] = 4.0f;
    
    std::cout << "Matrix A:" << std::endl;
    a.print<float>();
    
    std::cout << "Matrix B (Identity):" << std::endl;
    b.print<float>();
    
    Matrix c = matrix_ops::add(a, b);
    std::cout << "A + B:" << std::endl;
    c.print<float>();
    
    // 图像通道操作
    Vec3b pixel({255, 128, 64});
    Image rgb_img(100, 100, 3);
    
    // 使用便利宏遍历像素
    FOR_EACH_PIXEL_CH(rgb_img) {
        rgb_img.at<uint8_t>(y, x, 0) = pixel[0];
        rgb_img.at<uint8_t>(y, x, 1) = pixel[1];
        rgb_img.at<uint8_t>(y, x, 2) = pixel[2];
    }
    
    std::cout << "\nCreated RGB image and filled with color" << std::endl;
    
    // 分离通道
    std::vector<Image> channels = rgb_img.splitChannels();
    std::cout << "Split image into " << channels.size() << " channels" << std::endl;
    
    // 合并通道
    Image merged = Image::mergeChannels(channels);
    std::cout << "Merged channels back: " << merged.width() << "x" << merged.height() 
              << "x" << merged.channels() << std::endl;
    
    // ROI操作
    Rect roi(20, 20, 60, 60);
    Image roi_img = rgb_img.pixel_roi(roi);
    std::cout << "Extracted ROI: " << roi_img.width() << "x" << roi_img.height() 
              << "x" << roi_img.channels() << std::endl;
}

void demonstrate_type_safety() {
    std::cout << "\n=== Type Safety Demo ===" << std::endl;
    
    using namespace core;
    
    // 不同类型的矩阵
    Matrix mat_uint8(2, 2, DataType::UINT8);
    Matrix mat_float32(2, 2, DataType::FLOAT32);
    
    // 填充数据
    mat_uint8.setTo(Scalar(128));
    mat_float32.setTo(Scalar(1.5f));
    
    std::cout << "UINT8 matrix:" << std::endl;
    mat_uint8.print<uint8_t>();
    
    std::cout << "FLOAT32 matrix:" << std::endl;
    mat_float32.print<float>();
    
    // 类型转换
    Matrix converted = mat_float32.convertTo(DataType::UINT8);
    std::cout << "FLOAT32 -> UINT8:" << std::endl;
    converted.print<uint8_t>();
    
    // 向量操作
    Vec3f vec_f(1.5f, 2.5f, 3.5f);
    Vec3b vec_b = vec_f;  // 自动类型转换
    Vec3i vec_i = vec_f;  // 自动类型转换
    
    std::cout << "\nVector type conversions:" << std::endl;
    std::cout << "Float: (" << vec_f[0] << ", " << vec_f[1] << ", " << vec_f[2] << ")" << std::endl;
    std::cout << "Byte: (" << (int)vec_b[0] << ", " << (int)vec_b[1] << ", " << (int)vec_b[2] << ")" << std::endl;
    std::cout << "Int: (" << vec_i[0] << ", " << vec_i[1] << ", " << vec_i[2] << ")" << std::endl;
    
    // 向量数学运算
    Vec3f normalized = vec_f.normalized();
    float length = vec_f.length();
    Vec3f doubled = vec_f * 2.0f;
    
    std::cout << "\nVector operations:" << std::endl;
    std::cout << "Length: " << length << std::endl;
    std::cout << "Normalized: (" << normalized[0] << ", " << normalized[1] << ", " << normalized[2] << ")" << std::endl;
    std::cout << "Doubled: (" << doubled[0] << ", " << doubled[1] << ", " << doubled[2] << ")" << std::endl;
}

int main() {
    std::cout << "Modular Header-Only Image Processing Library" << std::endl;
    std::cout << "===========================================" << std::endl;
    
    demonstrate_modular_design();
    demonstrate_utils_namespace();
    demonstrate_advanced_features();
    demonstrate_type_safety();
    
    std::cout << "\n=== Library Features Summary ===" << std::endl;
    std::cout << "✓ Header-only design - no separate compilation needed" << std::endl;
    std::cout << "✓ Modular structure - clear separation of concerns" << std::endl;
    std::cout << "✓ Type safety - template-based design" << std::endl;
    std::cout << "✓ Convenience functions - utils namespace" << std::endl;
    std::cout << "✓ Memory safety - RAII and bounds checking" << std::endl;
    std::cout << "✓ Performance - minimal copying, direct data access" << std::endl;
    std::cout << "✓ Extensibility - easy to add new operations" << std::endl;
    std::cout << "✓ Debugging support - print functions and macros" << std::endl;
    
    return 0;
}