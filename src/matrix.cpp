#include <vector>
#include <cmath>

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

		// Operators

		T operator()(int x, int y) {
			return data[x + y * columns];
		}

		T operator[](int x){
			return data[x];
		}

		// Metrics on all the following operators are assumed to be correct

		// Outer product
		Matrix<T> operator^(const Matrix<T> &m) {
			std::vector<T> newData(rows * m.columns);
			for (int i = 0; i < rows * m.columns; i++) {
				int x = i / rows;
				int y = i % m.columns;
				for (int j = 0; j < columns; j++)
				   newData[i] += data[x * columns + j] * m.data[j * m.columns + i % m.columns];
			}
			return Matrix<T>(rows, m.columns, newData);
		}

		// Hadamard product
		Matrix<T> operator*(const Matrix<T> &m) {
			std::vector<T> newData(rows * columns);
			for (int i = 0; i < rows * columns; i++)
				newData[i] = data[i] * m.data[i];
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T> operator+(const Matrix<T> &m) {
			std::vector<T> newData(rows * columns);
			for (int i = 0; i < rows * columns; i++)
				newData[i] = data[i] + m.data[i];
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T> operator-(const Matrix<T> &m) {
			std::vector<T> newData(rows * columns);
			for (int i = 0; i < rows * columns; i++)
				newData[i] = data[i] - m.data[i];
			return Matrix<T>(rows, columns, newData);
		}

		Matrix<T>& operator*=(const Matrix<T> &m) {
			for (int i = 0; i < rows * columns; i++)
				data[i] *= m[i];
			return this;
		}
		
		Matrix<T>& operator+=(const Matrix<T> &m) { 
			for (int i = 0; i < rows * columns; i++)
				data[i] += m[i];
			return this;
		} 
		
		Matrix<T>& operator-=(const Matrix<T> &m) { 
			for (int i = 0; i < rows * columns; i++)
				data[i] -= m[i];
			return this;
		}

		// Unary operators

		Matrix<T> operator-() {
			std::vector<T> newData(rows * columns);
			for (int i = 0; i < rows * columns; i++)
				newData[i] = -data[i];
			return Matrix<T>(rows, columns, newData);
		}

		// Transpose

		Matrix<T> operator~() {
			std::vector<T> newData(rows * columns);
			for (int i = 0; i < rows; i++)
				for (int j = 0; j < columns; i++) 
					newData[j * rows + i] = data[i * columns + j];
			return Matrix<T>(columns, rows, newData);
		}

		bool isSameSize(const Matrix<T> &m) {
			return rows == m.rows && columns == m.columns;
		}

		bool hasOuterProduct(const Matrix<T> &m) {
			return columns == m.rows;
		}

};
