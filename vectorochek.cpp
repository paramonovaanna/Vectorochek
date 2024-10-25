#include <cassert>
#include <memory>
#include <stdexcept>
#include <algorithm>

template<typename T>
class Vectorochek {
public:
    // Constructors
    Vectorochek() {
    }

    Vectorochek(size_t size, const T& value = T()) {
        Reserve(size);
        for (size_t i = 0; i < size; ++i) {
            PushBack(value);
        }
    }

    Vectorochek(const Vectorochek& other) {
        Reserve(other.Capacity());
        for (size_t i = 0; i < other.Size(); ++i) {
            PushBack(other[i]);
        }
    }

    Vectorochek(Vectorochek&& other) noexcept {
        Swap(other);
    }

    // operator=(s)

    Vectorochek& operator=(const Vectorochek& other) {
        if (this != std::addressof(other)) {
            Vectorochek temp(other);
            Swap(temp);
        }
        return *this;
    }

    Vectorochek& operator=(Vectorochek&& other) noexcept {
        if (this != std::addressof(other)) {
            Vectorochek temp(std::move(other));
            Swap(temp);
        }
        return *this;
    }

    // Destructor
    ~Vectorochek() {
        Clear();
        delete[] data_;
        capacity_ = 0;
    }

    // Modifiers

    void PushBack(const T& value) {
        if (size_ == capacity_) {
            if (capacity_ == 0) {
                capacity_ = 1;
            }
            Reserve(capacity_ * 2);
        }
        (*this)[size_] = value;
        ++size_;
    }

    void PopBack() {
        (*this)[size_ - 1].~T();
        --size_;
    }

    void Clear() {
        while (!Empty()) {
            PopBack();
        }
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }
        T* tmp = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            *(tmp + i) = std::move((*this)[i]);
        }
        delete[] data_;
        data_ = reinterpret_cast<std::byte*>(tmp);
        capacity_ = new_capacity;
    }

    void Resize(size_t new_size, const T& value = T()) {
        if (new_size == size_) {
            return;
        }
        if (new_size < size_) {
            while (size_ > new_size) {
                PopBack();
            }
            return;
        }
        while (size_ < new_size) {
            PushBack(value);
        }
        
    }

    void Swap(Vectorochek& other) {
        std::swap(data_, other.data_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
    }

    // Observers

    [[nodiscard]] size_t Size() const {
        return size_;
    }

    [[nodiscard]] size_t Capacity() const {
        return capacity_;
    }

    [[nodiscard]] bool Empty() const {
        return size_ == 0;
    }

    T* Data() {
        return reinterpret_cast<T*>(data_);
    }

    [[nodiscard]] const T* Data() const {
        return reinterpret_cast<T*>(data_);
    }

    T& Front() {
        return (*this)[0];
    }

    [[nodiscard]] const T& Front() const {
        return (*this)[0];
    }

    T& Back() {
        return (*this)[size_ - 1];
    }

    [[nodiscard]] const T& Back() const {
        return (*this)[size_ - 1];
    }

    T& At(size_t pos) {
        if (pos >= Size()) {
            throw std::out_of_range("Index out of range");
        }
        return (*this)[pos];
    }

    [[nodiscard]] const T& At(size_t pos) const {
        if (pos >= Size()) {
            throw std::out_of_range("Index out of range");
        }
        return (*this)[pos];
    }

    T& operator[](size_t index) {
        return *reinterpret_cast<T*>(data_ + index * sizeof(T));
    }

    [[nodiscard]] const T& operator[](size_t index) const {
        return *reinterpret_cast<T*>(data_ + index * sizeof(T));
    }

private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    std::byte* data_ = nullptr;
};