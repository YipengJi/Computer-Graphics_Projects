
#pragma once

#include <vector>
#include <algorithm>
#include <functional>

template<typename T>
class Array2D {
public:
	using size2d = std::pair<size_t, size_t>;

	Array2D(size2d const& new_shape)
	: shape(new_shape)
	, storage(new_shape.first * new_shape.second)
	{
	}

	size2d const& size() const {
		return shape;
	}

	T* data_ptr() {
		return storage.data();
	}

	T const* data_ptr() const {
		return storage.data();
	}

	void fill(T const& value) {
		for(T& elem : storage) {
			elem = value;
		}
	}

	T& operator()(size_t const row, size_t const col) {
		return storage[index(row, col)];
	}

	T& operator()(size2d const& pt) {
		return storage[index(pt.first, pt.second)];
	}

	T const& operator()(size_t const row, size_t const col) const {
		return storage[index(row, col)];
	}

	T const& operator()(size2d const& pt) const {
		return storage[index(pt.first, pt.second)];
	}

	template<typename DEST>
	Array2D<DEST> map(std::function<DEST(size_t, size_t, T const&)> f) const {
		Array2D<DEST> aout(shape);

		for(size_t col = 0; col < shape.first; col++) {
			for(size_t row = 0; row < shape.second; row++) {
				aout(row, col) = f(row, col, (*this)(row, col));
			}
		}

		// the copy is maybe automatically optimized out
		// https://stackoverflow.com/questions/17473753/c11-return-value-optimization-or-move/17473874
		return aout;
	}

	Array2D<T>& operator*=(T const& other) {
		for(T& elem : storage) {
			elem *= other;
		}
		return *this;
	}

private:
	size2d shape;
	std::vector<T> storage;

	inline size_t index(size_t const row, size_t const col) const {
		return row + col * shape.first;
	}
};
