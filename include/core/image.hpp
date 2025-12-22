#ifndef CORE_IMAGE_HPP
#define CORE_IMAGE_HPP

#include "matrix.hpp"
#include "vector_types.hpp"
#include <stdexcept>
#include <memory>

// 专门的图像类（基于Matrix）
class Image {
private:
    Matrix mat_;
    int channels_;
    
public:
    // 构造函数
    Image() : channels_(0) {}
    
    Image(int width, int height, int channels = 1, DataType dtype = DataType::UINT8)
        : mat_(height, width * channels, dtype), channels_(channels) {}
    
    Image(const Size& size, int channels = 1, DataType dtype = DataType::UINT8)
        : Image(size.width, size.height, channels, dtype) {}
    
    explicit Image(const Matrix& mat, int channels = 1) 
        : mat_(mat), channels_(channels) {}
    
    // 从现有图像创建
    Image(const Image& other) = default;
    Image(Image&& other) = default;
    Image& operator=(const Image& other) = default;
    Image& operator=(Image&& other) = default;
    
    // 基本属性
    int width() const { return cols(); }
    int height() const { return rows(); }
    int cols() const { return mat_.cols() / channels_; }
    int rows() const { return mat_.rows(); }
    int channels() const { return channels_; }
    DataType type() const { return mat_.type(); }
    Size size() const { return Size(cols(), rows()); }
    bool empty() const { return mat_.empty() || channels_ <= 0; }
    
    // 数据访问
    void* data() { return mat_.data(); }
    const void* data() const { return mat_.data(); }
    
    template<typename T>
    T* ptr(int row = 0) { return mat_.ptr<T>(row); }
    
    template<typename T>
    const T* ptr(int row = 0) const { return mat_.ptr<T>(row); }
    
    // 像素访问（单通道）
    template<typename T = uint8_t>
    T at(int y, int x, int channel = 0) const {
        return mat_.at<T>(y, x * channels_ + channel);
    }
    
    template<typename T = uint8_t>
    T& at(int y, int x, int channel = 0) {
        return mat_.at<T>(y, x * channels_ + channel);
    }
    
    // 像素访问（多通道）
    template<typename T = uint8_t>
    Vec<T, 3> at3(int y, int x) const {
        if(channels_ < 3) throw std::runtime_error("Image has less than 3 channels");
        
        const T* pixel = ptr<T>(y) + x * channels_;
        return Vec<T, 3>({pixel[0], pixel[1], pixel[2]});
    }
    
    template<typename T = uint8_t>
    void set3(int y, int x, const Vec<T, 3>& pixel) {
        if(channels_ < 3) throw std::runtime_error("Image has less than 3 channels");
        
        T* ptr_pixel = ptr<T>(y) + x * channels_;
        ptr_pixel[0] = pixel[0];
        ptr_pixel[1] = pixel[1];
        ptr_pixel[2] = pixel[2];
    }
    
    template<typename T = uint8_t>
    Vec<T, 4> at4(int y, int x) const {
        if(channels_ < 4) throw std::runtime_error("Image has less than 4 channels");
        
        const T* pixel = ptr<T>(y) + x * channels_;
        return Vec<T, 4>({pixel[0], pixel[1], pixel[2], pixel[3]});
    }
    
    template<typename T = uint8_t>
    void set4(int y, int x, const Vec<T, 4>& pixel) {
        if(channels_ < 4) throw std::runtime_error("Image has less than 4 channels");
        
        T* ptr_pixel = ptr<T>(y) + x * channels_;
        ptr_pixel[0] = pixel[0];
        ptr_pixel[1] = pixel[1];
        ptr_pixel[2] = pixel[2];
        ptr_pixel[3] = pixel[3];
    }
    
    // 简化的像素访问（8位）
    uint8_t get_pixel(int x, int y, int channel = 0) const {
        if (x < 0 || x >= width() || y < 0 || y >= height() || channel < 0 || channel >= channels_) {
            return 0;
        }
        return at<uint8_t>(y, x, channel);
    }
    
    void set_pixel(int x, int y, int channel, uint8_t value) {
        if (x < 0 || x >= width() || y < 0 || y >= height() || channel < 0 || channel >= channels_) {
            return;
        }
        at<uint8_t>(y, x, channel) = value;
    }
    
    // 批量像素设置
    void set_pixel(int x, int y, const Vec3b& pixel) {
        if(channels_ >= 3) {
            set_pixel(x, y, 0, pixel[0]);
            set_pixel(x, y, 1, pixel[1]);
            set_pixel(x, y, 2, pixel[2]);
        }
    }
    
    void set_pixel(int x, int y, const Vec4b& pixel) {
        if(channels_ >= 4) {
            set_pixel(x, y, 0, pixel[0]);
            set_pixel(x, y, 1, pixel[1]);
            set_pixel(x, y, 2, pixel[2]);
            set_pixel(x, y, 3, pixel[3]);
        }
    }
    
    // ROI操作
    Image roi(const Rect& region) const {
        // 调整ROI到实际像素坐标
        Rect pixel_region = Rect(
            region.x * channels_, 
            region.y, 
            region.width * channels_, 
            region.height
        );
        
        Matrix roi_mat = mat_.roi(pixel_region);
        return Image(roi_mat, channels_);
    }
    
    // ROI操作（像素坐标）
    Image pixel_roi(const Rect& pixel_region) const {
        Rect mat_region = Rect(
            pixel_region.x * channels_, 
            pixel_region.y, 
            pixel_region.width * channels_, 
            pixel_region.height
        );
        
        Matrix roi_mat = mat_.roi(mat_region);
        return Image(roi_mat, channels_);
    }
    
    // 克隆
    Image clone() const {
        return Image(mat_.clone(), channels_);
    }
    
    // 创建图像
    void create(int width, int height, int channels = 1, DataType dtype = DataType::UINT8) {
        channels_ = channels;
        mat_.create(height, width * channels, dtype);
    }
    
    void create(const Size& size, int channels = 1, DataType dtype = DataType::UINT8) {
        create(size.width, size.height, channels, dtype);
    }
    
    void release() {
        mat_.release();
        channels_ = 0;
    }
    
    // 类型转换
    Image convertTo(DataType newType) const {
        return Image(mat_.convertTo(newType), channels_);
    }
    
    // 填充操作
    void setTo(Scalar value) {
        mat_.setTo(value);
    }
    
    void zeros() { mat_.zeros(); }
    void ones() { setTo(Scalar(1)); }
    
    // 通道操作
    Image channel(int c) const {
        if(c < 0 || c >= channels_) {
            throw std::out_of_range("Channel index out of range");
        }
        
        Image result(cols(), rows(), 1, type());
        
        for(int y = 0; y < rows(); ++y) {
            for(int x = 0; x < cols(); ++x) {
                result.at<uint8_t>(y, x, 0) = at<uint8_t>(y, x, c);
            }
        }
        
        return result;
    }
    
    std::vector<Image> splitChannels() const {
        std::vector<Image> channels;
        for(int c = 0; c < channels_; ++c) {
            channels.push_back(channel(c));
        }
        return channels;
    }
    
    // 从单通道图像合并
    static Image mergeChannels(const std::vector<Image>& channels) {
        if(channels.empty()) {
            return Image();
        }
        
        int height = channels[0].height();
        int width = channels[0].width();
        DataType dtype = channels[0].type();
        int num_channels = static_cast<int>(channels.size());
        
        // 验证所有通道大小和类型一致
        for(const auto& ch : channels) {
            if(ch.height() != height || ch.width() != width || ch.type() != dtype) {
                throw std::invalid_argument("All channels must have same size and type");
            }
        }
        
        Image result(width, height, num_channels, dtype);
        
        for(int c = 0; c < num_channels; ++c) {
            for(int y = 0; y < height; ++y) {
                for(int x = 0; x < width; ++x) {
                    result.at<uint8_t>(y, x, c) = channels[c].at<uint8_t>(y, x, 0);
                }
            }
        }
        
        return result;
    }
    
    // 底层Matrix访问
    const Matrix& matrix() const { return mat_; }
    Matrix& matrix() { return mat_; }
    
    // 比较操作
    bool operator==(const Image& other) const {
        return mat_ == other.mat_ && channels_ == other.channels_;
    }
    
    bool operator!=(const Image& other) const { return !(*this == other); }
    
    // 调试信息
    void printInfo(const std::string& name = "Image") const {
        std::cout << name << ": " << width() << "x" << height() 
                  << "x" << channels() 
                  << ", type=" << dataTypeName(type()) << std::endl;
        mat_.printInfo(name + ".matrix");
    }
    
    // 像素迭代器（简化版本）
    class PixelIterator {
    private:
        Image* image_;
        int x_, y_;
        int width_, height_;
        
    public:
        PixelIterator(Image* img, int x = 0, int y = 0) 
            : image_(img), x_(x), y_(y), width_(img->width()), height_(img->height()) {}
        
        bool isValid() const { 
            return x_ >= 0 && x_ < width_ && y_ >= 0 && y_ < height_; 
        }
        
        void next() {
            x_++;
            if(x_ >= width_) {
                x_ = 0;
                y_++;
            }
        }
        
        int x() const { return x_; }
        int y() const { return y_; }
        
        template<typename T = uint8_t>
        Vec<T, 3> pixel3() const { 
            return image_->template at3<T>(y_, x_); 
        }
        
        template<typename T = uint8_t>
        T pixel(int channel) const { 
            return image_->template at<T>(y_, x_, channel); 
        }
    };
    
    PixelIterator begin() { return PixelIterator(this, 0, 0); }
};

// 图像操作命名空间
namespace image_ops {
    
// 创建彩色图像
inline Image createColorImage(const Size& size, const Vec3b& color) {
    Image result(size.width, size.height, 3);
    result.setTo(Scalar(color[0], color[1], color[2]));
    return result;
}

// 创建灰度图像
inline Image createGrayImage(const Size& size, uint8_t value = 0) {
    Image result(size.width, size.height, 1);
    result.setTo(Scalar(value));
    return result;
}

// 缩放图像（简单的最近邻插值）
inline Image resize(const Image& src, const Size& new_size) {
    Image result(new_size.width, new_size.height, src.channels(), src.type());
    
    float scaleX = static_cast<float>(src.width()) / new_size.width;
    float scaleY = static_cast<float>(src.height()) / new_size.height;
    
    for(int y = 0; y < new_size.height; ++y) {
        for(int x = 0; x < new_size.width; ++x) {
            int srcX = static_cast<int>(x * scaleX);
            int srcY = static_cast<int>(y * scaleY);
            
            for(int c = 0; c < src.channels(); ++c) {
                result.at<uint8_t>(y, x, c) = src.at<uint8_t>(srcY, srcX, c);
            }
        }
    }
    
    return result;
}

// 水平翻转
inline Image flipHorizontal(const Image& src) {
    Image result(src.width(), src.height(), src.channels(), src.type());
    
    for(int y = 0; y < src.height(); ++y) {
        for(int x = 0; x < src.width(); ++x) {
            for(int c = 0; c < src.channels(); ++c) {
                result.at<uint8_t>(y, src.width() - 1 - x, c) = src.at<uint8_t>(y, x, c);
            }
        }
    }
    
    return result;
}

// 垂直翻转
inline Image flipVertical(const Image& src) {
    Image result(src.width(), src.height(), src.channels(), src.type());
    
    for(int y = 0; y < src.height(); ++y) {
        for(int x = 0; x < src.width(); ++x) {
            for(int c = 0; c < src.channels(); ++c) {
                result.at<uint8_t>(src.height() - 1 - y, x, c) = src.at<uint8_t>(y, x, c);
            }
        }
    }
    
    return result;
}

// 旋转90度
inline Image rotate90(const Image& src, bool clockwise = true) {
    Image result(src.height(), src.width(), src.channels(), src.type());
    
    if(clockwise) {
        for(int y = 0; y < src.height(); ++y) {
            for(int x = 0; x < src.width(); ++x) {
                for(int c = 0; c < src.channels(); ++c) {
                    result.at<uint8_t>(x, src.height() - 1 - y, c) = src.at<uint8_t>(y, x, c);
                }
            }
        }
    } else {
        for(int y = 0; y < src.height(); ++y) {
            for(int x = 0; x < src.width(); ++x) {
                for(int c = 0; c < src.channels(); ++c) {
                    result.at<uint8_t>(src.width() - 1 - x, y, c) = src.at<uint8_t>(y, x, c);
                }
            }
        }
    }
    
    return result;
}

} // namespace image_ops

#endif // CORE_IMAGE_HPP