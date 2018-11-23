#include "../../src/matrix.cpp"

int main() {
	Matrix<double> a(3, 3, {0, 1, 2, 3, 4, 5, 6, 7, 8});
	Matrix<double> b(3, 3, {10, 11, 12, 13, 14, 15, 16, 17, 18});
	Matrix<double> c(3, 2, {5, 4, 3, 2, 1, 0});
	
	std::cout << "A:\n" << a << std::endl;
	std::cout << "B:\n" << b << std::endl;
	std::cout << "C:\n" << c << std::endl;
	std::cout << "A + B:\n" << a + b << std::endl;
	std::cout << "A - B:\n" << a - b << std::endl;
	std::cout << "A * B:\n" << a * b << std::endl;
	std::cout << "A ^ C:\n" << (a ^ c) << std::endl;
	std::cout << "~C:\n" << ~c << std::endl;
	std::cout << "-A:\n" << -a << std::endl;
	std::cout << "A += B:\n" << (a += b) << std::endl;
	std::cout << "A:\n" << a << std::endl;
	std::cout << "A -= B:\n" << (a -= b) << std::endl;
	std::cout << "A:\n" << a << std::endl;
	std::cout << "A *= B:\n" << (a *= b) << std::endl;
	std::cout << "A:\n" << a << std::endl;
	std::cout << "A is same size as B: " << a.isSameSize(b) << std::endl;
	std::cout << "A is same size as C: " << a.isSameSize(c) << std::endl;
	std::cout << "A has a cross product with C: " << a.existsCrossProduct(c) << std::endl;
	std::cout << "A has a cross product with ~C: " << a.existsCrossProduct(~c) << std::endl;
	std::cout << "A(0, 1): " << a(0, 1) << std::endl;
	a(0, 1) = 10;
	std::cout << "A(0, 1) = 10: " << a(0, 1) << std::endl;

}
