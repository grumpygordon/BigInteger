#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stddef.h>
#include <memory>
#include <vector>

template <typename T>
class vector {

public:
    vector();

    ~vector();

    vector(size_t);

    vector(size_t, T);

    vector(vector const &) noexcept;

    vector &operator=(vector const &) noexcept;

    T back() const;

    T const &operator[](size_t) const;

    bool operator==(vector const &) const;

    T &operator[](size_t);

    void pop_back();

    void push_back(T);

    void reverse();

    void resize(size_t);

    void resize(size_t, T);

    void swap(vector &);

    void swap(vector<T> &a, vector<T> &b);

    size_t size() const;

private:
    bool small = true;

    union {
        std::shared_ptr<std::vector<T> > shared;
        T num;
    };

    void take_control();

	void increase();
};


#endif // VECTOR_H
