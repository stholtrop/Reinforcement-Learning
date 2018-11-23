#ifndef MATRIX
#define MATRIX
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <random>

template <typename T>
class Matrix {
	private:

		// Member variables

		std::vector<T> data;

	public:

		// Member variables

		const size_t rows;
		const size_t columns;

		// Constructors

  		Matrix(const size_t x, const size_t y) : data(x * y), rows(x), columns(y) {}

		Matrix(const size_t x, const size_t y, std::vector<T> contents) : data(contents), rows(x), columns(y) {}

		// Random

		static Matrix<T> initializeRandom(const size_t rows, const size_t columns, const T min=-1, const T max=1) {
			std::default_random_engine generator;
			std::uniform_real_distribution<T> distribution(min, max);
			auto gen = [&distribution, &generator](){
				return distribution(generator);
			};
			std::vector<T> v(rows*columns);
			
		}

		// Operators

		const T& operator()(const int x, const int y) const {
			return data[x + y * columns];
		}

		T& operator()(const int x, const int y) {
			return data[x + y * columns];
		}

		const T& operator[](const int x) const {
			return data[x];
		}

		T& operator[](const int x) {
			return data[x];
		}

		// Metrics on all the following operators are assumed to be correct

		// Cross product
		Matrix<T> operator^(const Matrix<T>& m) {
			std::vector<T> newData(rows * m.columns);
			for (unsigned int i = 0; i < rows * m.columns; i++) {
				int x = i / m.columns;
				int y = i % m.columns;
				for (unsigned int j = 0; j < columns; j++)
				   newData[i] += data[x * columns + j] * m.data[j * m.columns + y];
			}
			return Matrix<T>(rows, m.columns, newData);
		}

		// Hadamard product
		Matrix<T> operator*(const Matrix<T>& m) {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = data[i] * m.data[i];
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T> operator+(const Matrix<T>& m) {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = data[i] + m.data[i];
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T> operator-(const Matrix<T>& m) {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = data[i] - m.data[i];
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T>& operator*=(const Matrix<T>& m) {
			for (unsigned int i = 0; i < rows * columns; i++)
				data[i] *= m[i];
			return *this;
		}

		Matrix<T>& operator+=(const Matrix<T>& m) {
			for (unsigned int i = 0; i < rows * columns; i++)
				data[i] += m[i];
			return *this;
		}

		Matrix<T>& operator-=(const Matrix<T>& m) {
			for (unsigned int i = 0; i < rows * columns; i++)
				data[i] -= m[i];
			return *this;
		}

		// Unary operators

		Matrix<T> operator-() {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = -data[i];
			return Matrix<T>(rows, columns, newData);
		}

		// Transpose

		Matrix<T> transpose() {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows; i++)
				for (unsigned int j = 0; j < columns; j++)
					newData[j * rows + i] = data[i * columns + j];
			return Matrix<T>(columns, rows, newData);
		}

		// To string

		std::string toString() const {
			std::ostringstream oss;
			oss << '[';
			for (unsigned int i = 0; i < rows; i++) {
				if (i != 0)
					oss << ' ';
				oss << "[ ";
				for (unsigned int j = 0; j < columns; j++)
					oss << data[i * columns + j] << ' ';
				oss << ']';
				if (i != rows - 1)
					oss << '\n';
			}
			oss << ']';
			return oss.str();
		}

		bool isSameSize(const Matrix<T>& m) {
			return rows == m.rows && columns == m.columns;
		}

		bool existsCrossProduct(const Matrix<T>& m) {
			return columns == m.rows;
		}

};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
	return os << m.toString();
}
#endif
