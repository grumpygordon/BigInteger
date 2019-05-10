#include "vector.h"
#include <algorithm>

template<typename T>
vector<T>::vector() : vector(0) {}

template<typename T>
vector<T>::vector(size_t a) : vector(a, 0) {}

template<typename T>
vector<T>::vector(size_t a, T w) {
    small = (a == 1);
    if (small) {
        num = w;
    } else {
        new(&shared) std::shared_ptr<std::vector<T> >(new std::vector<T>(a, w));
    }
}

template<typename T>
vector<T>::vector(vector<T> const &w) noexcept {
    small = w.small;
    if (small) {
        num = w.num;
    } else {
        new(&shared) std::shared_ptr<std::vector<T> >(w.shared);
    }
}

template<typename T>
size_t vector<T>::size() const {
    return (small ? 1 : shared.get()->size());
}

template<typename T>
vector<T> &vector<T>::operator=(vector<T> const &w) noexcept {
    if (!small) {
        if (!w.small) {
            shared = w.shared;
        } else {
            shared.~shared_ptr();
            num = w.num;
        }
    } else {
        if (!w.small) {
            new(&shared) std::shared_ptr<std::vector<T> >(w.shared);
        } else {
            num = w.num;
        }
    }
    small = w.small;
    return *this;
}

template<typename T>
T vector<T>::back() const {
    if (small) return num;
    return shared.get()->back();
}

template<typename T>
void vector<T>::increase() {
	if (small) {
		small = false;
	    new(&shared) std::shared_ptr<std::vector<T> >(new std::vector<T>(1, num));
    }
}

template<typename T>
T const &vector<T>::operator[](size_t a) const {
    return (small ? num : (*shared)[a]);
}

template<typename T>
bool vector<T>::operator==(vector<T> const &w) const {
	return small == w.small && (small ? num == w.num : *shared == *w.shared);
}

template<typename T>
T &vector<T>::operator[](size_t i) {
    if (small) return num;

    take_control();
    return (*shared)[i];
}

#include <cassert>

template<typename T>
void vector<T>::pop_back() {
    if (small) {
		assert(0);
        num = 0;
        return;
    }
    take_control();
    if (shared->size() == 2) {
        small = true;
        T w = (*shared)[0];
        shared.reset();
		num = w;
        return;
    }
    shared->pop_back();
}

template<typename T>
void vector<T>::push_back(T x) {
    if (small) {
		increase();
        shared->push_back(x);
        return;
    }
    take_control();
    shared->push_back(x);
}

template<typename T>
void vector<T>::reverse() {
    if (small) return;
    take_control();
    std::reverse(shared->begin(), shared->end());
}

template<typename T>
void vector<T>::resize(size_t size) {
    resize(size, 0);
}

template<typename T>
void vector<T>::resize(size_t a, T w) {
	if (small) {
		if (a == 0) {
			assert(0);
		}
		if (a == 1)
			return;
		increase();
	}
	shared->resize(a, w);
}

template<typename T>
void vector<T>::take_control() {
    if (!shared.unique()) {
        shared.reset(new std::vector<T>(*shared));
    }
}

template<typename T>
vector<T>::~vector() {
	if (!small) {
		shared.~shared_ptr();
	}
}

template<typename T>
void vector<T>::swap(vector<T> &w) {
    vector<T> tmp(w);
    w = *this;
    *this = tmp;
}

template<typename T>
void swap(vector<T> &a, vector<T> &b) {
	a.swap(b);
}
