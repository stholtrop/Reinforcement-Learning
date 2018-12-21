#ifndef MATRIX
#define MATRIX
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include <fstream>
#include <ctime>

template <typename T>
class Matrix {
	private:

		// Member variables

		std::vector<T> data;

	public:

		// Member variables

		size_t rows;
		size_t columns;

		// Constructors
		// Empty constructor
		Matrix() {}
		// Initializers
  	Matrix(const size_t x, const size_t y) : data(x * y), rows(x), columns(y) {}
		Matrix(const size_t x, const size_t y, std::vector<T> contents) : data(contents), rows(x), columns(y) {}

		// Matrix read and write
		static Matrix<T> readFromFile(const std::string &filename) {
			std::ifstream file;
			try {
				file.open(filename);
			} catch (const std::ifstream::failure &e) {
				std::cerr << "Error reading file" << std::endl;
				throw "Error reading file";
			}
			Matrix<T> m = Matrix<T>::readFromFile(file);
			file.close();
			return m;
		}

		static Matrix<T> readFromFile(std::ifstream &file) {
			std::vector<T> data;
			size_t rows, columns;
			file >> rows >> columns;
			while (data.size() < rows * columns) {
				T temp;
				file >> temp;
				data.push_back(temp);
			}
			return Matrix<T>(rows, columns, data);
		}

		void writeToFile(const std::string &filename) {
			std::ofstream file;
			try {
				file.open(filename);
			} catch (const std::ofstream::failure &e) {
				std::cerr << "Error reading file" << std::endl;
				throw "Error writing file";
			}
			writeToFile(file);
			file.close();
		}

		void writeToFile(std::ofstream &file) {
			file << rows << " " << columns << " ";
			for (auto i : data) {
				file << i << " ";
			}
		}

		// Random

		static Matrix<T> initializeRandom(const size_t rows, const size_t columns, const T min=-1, const T max=1) {
			std::default_random_engine generator(time(0));
			std::uniform_real_distribution<T> distribution(min, max);
			auto gen = [&distribution, &generator](){
				return distribution(generator);
			};
			std::vector<T> v(rows*columns);
			std::generate(v.begin(), v.end(), gen);
			return Matrix(rows, columns, v);
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
		Matrix<T> operator^(const Matrix<T>& m) const {
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
		Matrix<T> operator*(const Matrix<T>& m) const {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = data[i] * m.data[i];
			return Matrix<T>(rows, columns, newData);
		}
		Matrix<T> operator*(const T num) const {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = data[i] * num;
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T> operator+(const Matrix<T>& m) const {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = data[i] + m.data[i];
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T> operator+(const T num) const {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = data[i] + num;
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T> operator-(const Matrix<T>& m) const {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = data[i] - m.data[i];
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T> operator-(const T num) const {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = data[i] - num;
			return Matrix<T>(rows, columns, newData);
		}


		Matrix<T>& operator*=(const Matrix<T>& m) {
			for (unsigned int i = 0; i < rows * columns; i++)
				data[i] *= m[i];
			return *this;
		}

		Matrix<T>& operator*=(const T num) {
			for (unsigned int i = 0; i < rows * columns; i++)
				data[i] *= num;
			return *this;
		}

		Matrix<T>& operator+=(const Matrix<T>& m) {
			for (unsigned int i = 0; i < rows * columns; i++)
				data[i] += m[i];
			return *this;
		}

		Matrix<T>& operator+=(const T num) {
			for (unsigned int i = 0; i < rows * columns; i++)
				data[i] += num;
			return *this;
		}

		Matrix<T>& operator-=(const Matrix<T>& m) {
			for (unsigned int i = 0; i < rows * columns; i++)
				data[i] -= m[i];
			return *this;
		}

		Matrix<T>& operator-=(const T num) {
			for (unsigned int i = 0; i < rows * columns; i++)
				data[i] -= num;
			return *this;
		}

		void operator=(const Matrix<T>& m){
			columns = m.columns;
			rows = m.rows;
			data = m.data;
		}


		// Unary operators

		Matrix<T> operator-() const {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = -data[i];
			return Matrix<T>(rows, columns, newData);
		}

		// Map, apply function

		Matrix<T> apply(const std::function<T(T)>& function) const {
			std::vector<T> newData(rows*columns);
			for (unsigned int i = 0; i < rows * columns; i++)
				newData[i] = function(data[i]);
			return Matrix<T>(rows, columns, newData);
		}

		static std::function<Matrix<T>(const Matrix<T>&)> wrap(const std::function<T(T)>& function) {
			return [function] (const Matrix<T>& m) {
				return m.apply(function);
			};
		}

		// Transpose

		Matrix<T> transpose() const {
			std::vector<T> newData(rows * columns);
			for (unsigned int i = 0; i < rows; i++)
				for (unsigned int j = 0; j < columns; j++)
					newData[j * rows + i] = data[i * columns + j];
			return Matrix<T>(columns, rows, newData);
		}

		Matrix<T> reshape(const int i, const int j) const {
			return Matrix<T>(i, j, data);
		}

		// Retrieve row or column

		Matrix<T> getRow(const unsigned int index) {
			// Index is nth row
			if (index > rows-1) {
				return Matrix<T>();
			}
			std::vector<T> newData;
			for (unsigned int i = index * columns; i < (index+1)*columns && i < rows*columns; i++) {
				newData.push_back(data[i]);
			}
			return Matrix<T>(1, columns, newData);
		}

		Matrix<T> getColumn(const unsigned int index) {
			// Index is nth column
			std::vector<T> newData;
			for (unsigned int i = index; i < columns*rows; i += columns) {
				newData.push_back(data[i]);
			}
			return Matrix<T>(rows, 1, newData);
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

		bool isCorrupted() {
			return columns * rows != data.size();
		}

		std::string dimensions() const {
			std::ostringstream oss;
			oss << rows << 'x' << columns;
			return oss.str();
		}
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
	return os << m.toString();
}

template<typename T>
inline Matrix<T> operator+(int i, const Matrix<T>& m) {
	return m + i;
}

template<typename T>
inline Matrix<T> operator-(int i, const Matrix<T>& m) {
	return m - i;
}

template<typename T>
inline Matrix<T> operator*(int i, const Matrix<T>& m) {
	return m * i;
}

#endif
