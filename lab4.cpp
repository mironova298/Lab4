#include <vector>
#include <iostream>

class MultidimensionalArray {
public:
    MultidimensionalArray(std::vector<int> shape) {
        shape_ = shape;
        size_ = 1;
        for (int i = 0; i < shape_.size(); i++) {
            size_ *= shape_[i];
        }
        data_ = new int[size_];
    }

    ~MultidimensionalArray() {
        delete[] data_;
    }

    
    int& operator()(std::vector<int> indices) {
        int index = get_index(indices);
        return data_[index];
    }

    
    int& operator[](std::vector<int> indices) {
        int index = get_hilbert_index(indices);
        return data_[index];
    }

    
    std::vector<int> get_indices(int index) {
        std::vector<int> indices(shape_.size());
        int remainder = index;
        for (int i = shape_.size() - 1; i >= 0; i--) {
            int size = size_ / shape_[i];
            int quotient = remainder / size;
            remainder = remainder % size;
            indices[i] = quotient;
        }
        return indices;
    }

private:
    std::vector<int> shape_;
    int size_;
    int* data_;

    int get_index(std::vector<int> indices) {
        int index = 0;
        int multiplier = 1;
        for (int i = shape_.size() - 1; i >= 0; i--) {
            index += indices[i] * multiplier;
            multiplier *= shape_[i];
        }
        return index;
    }

    int get_hilbert_index(std::vector<int> indices) {
        int index = 0;
        int multiplier = 1;
        std::vector<int> coords(shape_.size());
        for (int i = shape_.size() - 1; i >= 0; i--) {
            int hilbert_size = 1 << i;
            int x = indices[i] / hilbert_size;
            int y = indices[i] % hilbert_size;
            coords[i] = x;
            coords[i] <<= 1;
            coords[i] |= y;
        }
        int n = 1 << shape_.size();
        for (int i = 0; i < n; i++) {
            std::vector<int> hilbert_coords(shape_.size());
            int x = i;
            for (int j = shape_.size() - 1; j >= 0; j--) {
                hilbert_coords[j] = x & 1;
                x >>= 1;
            }
            int hilbert_index = get_index(hilbert_coords);
            index += coords[hilbert_index] * multiplier;
            multiplier <<= 2;
        }
        return index;
    }
};
