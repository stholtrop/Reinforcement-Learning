#include "matrix.cpp"
#include <fstream>

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
	std::cout << "C:\n" << c.transpose() << std::endl;
	std::cout << "-A:\n" << -a << std::endl;	std::cout << "A += B:\n" << (a += b) << std::endl;
	std::cout << "A:\n" << a << std::endl;
	std::cout << "A -= B:\n" << (a -= b) << std::endl;
	std::cout << "A:\n" << a << std::endl;
	std::cout << "A *= B:\n" << (a *= b) << std::endl;
	std::cout << "A:\n" << a << std::endl;
	std::cout << "A is same size as B: " << a.isSameSize(b) << std::endl;
	std::cout << "A is same size as C: " << a.isSameSize(c) << std::endl;
	std::cout << "A has a cross product with C: " << a.existsCrossProduct(c) << std::endl;
	std::cout << "A has a cross product with C: " << a.existsCrossProduct(c.transpose()) << std::endl;
	std::cout << "A(0, 1): " << a(0, 1) << std::endl;
	a(0, 1) = 10;
	std::cout << "A(0, 1) = 10: " << a(0, 1) << std::endl;
	double da = a(0, 1);
	std::cout << "da: " << da << std::endl;
	da = 1;
	std::cout << "da: " << da << std::endl;
	std::cout << "A(0, 1) = 1?: " << a(0, 1) << std::endl;
	a[3] = 11;
	std::cout << "A(0, 1) = 11?: " << a(0, 1) << std::endl;
	double dz = a[3];
	dz -= 5;
	std::cout << "A(0, 1) = 6?: " << a(0, 1) << std::endl;
	std::cout << "Retrieve columns and rows" << std::endl;
	std::cout << b.getRow(0) << std::endl << b.getColumn(0) <<std::endl;
	std::cout << "Testing SSV reading and writing capabilities" << std::endl;
	a.writeToFile("Test.ssv");
	std::cout << "Write of A successful" << std::endl;
	Matrix<double> read_matrix = Matrix<double>::readFromFile("Test.ssv");
	std::cout << "Read successful" << std::endl;
	std::cout << read_matrix << std::endl;
	std::cout << "Writing A and B" << std::endl;
	std::ofstream file("Test2.ssv");
	a.writeToFile(file);
	b.writeToFile(file);
	file.close();
	std::cout << "Reading A and B" << std::endl;
	std::ifstream file2("Test2.ssv");
	std::cout << 	Matrix<double>::readFromFile(file2) << std::endl << Matrix<double>::readFromFile(file2);
}
