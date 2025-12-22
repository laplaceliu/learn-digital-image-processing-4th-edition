#ifndef CORE_MATRIX_HPP
#define CORE_MATRIX_HPP

#include "basic_types.hpp"
#include "vector_types.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <cstring>
#include <stdexcept>

// 核心矩阵类（类似cv::Mat）
class Matrix {
private:
    int rows_;
    int cols_;
    DataType dtype_;
    std::vector<uint8_t> data_;
    size_t step_;  // 每行字节数
    
public:
    // 构造函数
    Matrix() : rows_(0), cols_(0), dtype_(DataType::UINT8), step_(0) {}
    
    Matrix(int rows, int cols, DataType dtype = DataType::UINT8)
        : rows_(rows), cols_(cols), dtype_(dtype) {
        if(rows <= 0 || cols <= 0) {
            rows_ = cols_ = 0;
            step_ = 0;
            return;
        }
        step_ = cols_ * dataTypeSize(dtype_);
        data_.resize(static_cast<size_t>(rows_) * step_);
    }
    
    Matrix(const Size& size, DataType dtype = DataType::UINT8)
        : Matrix(size.height, size.width, dtype) {}
    
    // 拷贝/移动构造
    Matrix(const Matrix& other) = default;
    Matrix(Matrix&& other) = default;
    Matrix& operator=(const Matrix& other) = default;
    Matrix& operator=(Matrix&& other) = default;
    
    // 基本属性
    int rows() const { return rows_; }
    int cols() const { return cols_; }
    Size size() const { return Size(cols_, rows_); }
    DataType type() const { return dtype_; }
    size_t elemSize() const { return dataTypeSize(dtype_); }
    size_t step() const { return step_; }
    bool empty() const { return rows_ <= 0 || cols_ <= 0; }
    size_t total() const { return static_cast<size_t>(rows_) * cols_; }
    
    // 数据访问
    void* data() { return data_.data(); }
    const void* data() const { return data_.data(); }
    
    template<typename T>
    T* ptr(int row = 0) {
        if(row < 0 || row >= rows_) throw std::out_of_range("Row index out of range");
        return reinterpret_cast<T*>(data_.data() + static_cast<size_t>(row) * step_);
    }
    
    template<typename T>
    const T* ptr(int row = 0) const {
        if(row < 0 || row >= rows_) throw std::out_of_range("Row index out of range");
        return reinterpret_cast<const T*>(data_.data() + static_cast<size_t>(row) * step_);
    }
    
    // 元素访问
    template<typename T>
    T at(int row, int col) const {
        if(row < 0 || row >= rows_ || col < 0 || col >= cols_)
            throw std::out_of_range("Index out of range");
        return ptr<T>(row)[col];
    }
    
    template<typename T>
    T& at(int row, int col) {
        if(row < 0 || row >= rows_ || col < 0 || col >= cols_)
            throw std::out_of_range("Index out of range");
        return ptr<T>(row)[col];
    }
    
    // 创建ROI
    Matrix roi(const Rect& region) const {
        if(!contains(region)) {
            throw std::invalid_argument("ROI region out of matrix bounds");
        }
        
        Matrix result;
        result.rows_ = region.height;
        result.cols_ = region.width;
        result.dtype_ = dtype_;
        result.step_ = step_;
        
        // 复制ROI数据
        const uint8_t* src_ptr = data_.data() + static_cast<size_t>(region.y) * step_ + 
                                static_cast<size_t>(region.x) * elemSize();
        result.data_.resize(static_cast<size_t>(result.rows_) * result.step_);
        
        for(int i = 0; i < result.rows_; ++i) {
            std::copy(src_ptr + i * step_, 
                     src_ptr + i * step_ + result.cols_ * elemSize(),
                     result.data_.data() + i * result.step_);
        }
        
        return result;
    }
    
    // 检查矩形是否在矩阵范围内
    bool contains(const Rect& region) const {
        return region.x >= 0 && region.y >= 0 && 
               region.x + region.width <= cols_ && 
               region.y + region.height <= rows_;
    }
    
    // 类型转换
    Matrix convertTo(DataType newType) const {
        if(newType == dtype_) return clone();
        
        Matrix result(rows_, cols_, newType);
        
        // 简单的类型转换实现
        switch(dtype_) {
            case DataType::UINT8:
                convertFrom<uint8_t>(result, newType);
                break;
            case DataType::INT8:
                convertFrom<int8_t>(result, newType);
                break;
            case DataType::UINT16:
                convertFrom<uint16_t>(result, newType);
                break;
            case DataType::INT16:
                convertFrom<int16_t>(result, newType);
                break;
            case DataType::FLOAT32:
                convertFrom<float>(result, newType);
                break;
            case DataType::FLOAT64:
                convertFrom<double>(result, newType);
                break;
        }
        
        return result;
    }
    
    // 克隆
    Matrix clone() const {
        Matrix result;
        result.rows_ = rows_;
        result.cols_ = cols_;
        result.dtype_ = dtype_;
        result.step_ = step_;
        result.data_ = data_;
        return result;
    }
    
    // 重置大小
    void create(int rows, int cols, DataType dtype = DataType::UINT8) {
        rows_ = rows;
        cols_ = cols;
        dtype_ = dtype;
        if(rows <= 0 || cols <= 0) {
            step_ = 0;
            data_.clear();
        } else {
            step_ = cols_ * dataTypeSize(dtype_);
            data_.resize(static_cast<size_t>(rows_) * step_);
        }
    }
    
    void create(const Size& size, DataType dtype = DataType::UINT8) {
        create(size.height, size.width, dtype);
    }
    
    void release() {
        rows_ = cols_ = 0;
        step_ = 0;
        data_.clear();
    }
    
    // 填充操作
    void setTo(Scalar value) {
        switch(dtype_) {
            case DataType::UINT8:
                fillValue<uint8_t>(value);
                break;
            case DataType::FLOAT32:
                fillValue<float>(value);
                break;
            case DataType::FLOAT64:
                fillValue<double>(value);
                break;
            default:
                throw std::runtime_error("Unsupported data type for setTo");
        }
    }
    
    void zeros() { setTo(Scalar(0)); }
    void ones() { setTo(Scalar(1)); }
    
    // 比较操作
    bool operator==(const Matrix& other) const {
        return rows_ == other.rows_ && cols_ == other.cols_ && 
               dtype_ == other.dtype_ && data_ == other.data_;
    }
    
    bool operator!=(const Matrix& other) const { return !(*this == other); }
    
    // 调试信息
    void printInfo(const std::string& name = "Matrix") const {
        std::cout << name << ": " << rows_ << "x" << cols_ 
                  << ", type=" << dataTypeName(dtype_) 
                  << ", step=" << step_ << " bytes" << std::endl;
    }
    
    template<typename T>
    void print(int maxRows = 10, int maxCols = 10) const {
        int printRows = std::min(rows_, maxRows);
        int printCols = std::min(cols_, maxCols);
        
        for(int i = 0; i < printRows; ++i) {
            for(int j = 0; j < printCols; ++j) {
                std::cout << std::setw(8) << at<T>(i, j) << " ";
            }
            if(cols_ > maxCols) std::cout << "...";
            std::cout << std::endl;
        }
        if(rows_ > maxRows) std::cout << "..." << std::endl;
    }
    
private:
    // 类型转换辅助函数
    template<typename SrcT>
    void convertFrom(Matrix& dst, DataType dstType) const {
        switch(dstType) {
            case DataType::UINT8:
                convertToType<SrcT, uint8_t>(dst);
                break;
            case DataType::INT16:
                convertToType<SrcT, int16_t>(dst);
                break;
            case DataType::FLOAT32:
                convertToType<SrcT, float>(dst);
                break;
            case DataType::FLOAT64:
                convertToType<SrcT, double>(dst);
                break;
            default:
                throw std::runtime_error("Unsupported destination type");
        }
    }
    
    template<typename SrcT, typename DstT>
    void convertToType(Matrix& dst) const {
        const SrcT* src = ptr<SrcT>();
        DstT* d = dst.ptr<DstT>();
        
        size_t total_elements = total();
        for(size_t i = 0; i < total_elements; ++i) {
            d[i] = static_cast<DstT>(src[i]);
        }
    }
    
    template<typename T>
    void fillValue(const Scalar& value) {
        T* d = ptr<T>();
        size_t total_elements = total();
        T val = static_cast<T>(value[0]);
        
        for(size_t i = 0; i < total_elements; ++i) {
            d[i] = val;
        }
    }
};

// 矩阵运算函数
namespace matrix_ops {
    
// 创建零矩阵
inline Matrix zeros(int rows, int cols, DataType dtype = DataType::FLOAT32) {
    Matrix m(rows, cols, dtype);
    m.zeros();
    return m;
}

inline Matrix zeros(const Size& size, DataType dtype = DataType::FLOAT32) {
    return zeros(size.height, size.width, dtype);
}

// 创建单位矩阵
inline Matrix eye(int size, DataType dtype = DataType::FLOAT32) {
    Matrix m(size, size, dtype);
    m.zeros();
    
    switch(dtype) {
        case DataType::UINT8: {
            uint8_t* d = m.ptr<uint8_t>();
            for(int i = 0; i < size; ++i) d[i * size + i] = 255;
            break;
        }
        case DataType::FLOAT32: {
            float* d = m.ptr<float>();
            for(int i = 0; i < size; ++i) d[i * size + i] = 1.0f;
            break;
        }
        case DataType::FLOAT64: {
            double* d = m.ptr<double>();
            for(int i = 0; i < size; ++i) d[i * size + i] = 1.0;
            break;
        }
        default:
            throw std::runtime_error("Unsupported data type for eye matrix");
    }
    
    return m;
}

// 矩阵转置（仅支持方阵）
inline Matrix transpose(const Matrix& src) {
    if(src.rows() != src.cols()) {
        throw std::runtime_error("Transpose only supported for square matrices");
    }
    
    Matrix result(src.cols(), src.rows(), src.type());
    
    switch(src.type()) {
        case DataType::FLOAT32: {
            const float* s = src.ptr<float>();
            float* d = result.ptr<float>();
            int n = src.rows();
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j) {
                    d[j * n + i] = s[i * n + j];
                }
            }
            break;
        }
        case DataType::FLOAT64: {
            const double* s = src.ptr<double>();
            double* d = result.ptr<double>();
            int n = src.rows();
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j) {
                    d[j * n + i] = s[i * n + j];
                }
            }
            break;
        }
        default:
            throw std::runtime_error("Unsupported data type for transpose");
    }
    
    return result;
}

// 矩阵相加
inline Matrix add(const Matrix& a, const Matrix& b) {
    if(a.size() != b.size() || a.type() != b.type()) {
        throw std::invalid_argument("Matrix sizes or types don't match");
    }
    
    Matrix result(a.rows(), a.cols(), a.type());
    
    switch(a.type()) {
        case DataType::FLOAT32: {
            const float* pa = a.ptr<float>();
            const float* pb = b.ptr<float>();
            float* pr = result.ptr<float>();
            size_t total = a.total();
            for(size_t i = 0; i < total; ++i) {
                pr[i] = pa[i] + pb[i];
            }
            break;
        }
        case DataType::FLOAT64: {
            const double* pa = a.ptr<double>();
            const double* pb = b.ptr<double>();
            double* pr = result.ptr<double>();
            size_t total = a.total();
            for(size_t i = 0; i < total; ++i) {
                pr[i] = pa[i] + pb[i];
            }
            break;
        }
        default:
            throw std::runtime_error("Unsupported data type for add");
    }
    
    return result;
}

// 矩阵相减
inline Matrix subtract(const Matrix& a, const Matrix& b) {
    if(a.size() != b.size() || a.type() != b.type()) {
        throw std::invalid_argument("Matrix sizes or types don't match");
    }
    
    Matrix result(a.rows(), a.cols(), a.type());
    
    switch(a.type()) {
        case DataType::FLOAT32: {
            const float* pa = a.ptr<float>();
            const float* pb = b.ptr<float>();
            float* pr = result.ptr<float>();
            size_t total = a.total();
            for(size_t i = 0; i < total; ++i) {
                pr[i] = pa[i] - pb[i];
            }
            break;
        }
        case DataType::FLOAT64: {
            const double* pa = a.ptr<double>();
            const double* pb = b.ptr<double>();
            double* pr = result.ptr<double>();
            size_t total = a.total();
            for(size_t i = 0; i < total; ++i) {
                pr[i] = pa[i] - pb[i];
            }
            break;
        }
        default:
            throw std::runtime_error("Unsupported data type for subtract");
    }
    
    return result;
}

} // namespace matrix_ops

#endif // CORE_MATRIX_HPP