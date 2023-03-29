#include <iostream>
#include <string>
#include <iomanip>

#include "Matrix.h"

using std::string;
using std::cout;
using std::endl;


int main(int argc, char* argv[])
{
	Matrix m(2, 3);
	float p = 1;
	for (int i = 0; i < m.get_rows(); i++)
	{
		for (int j = 0; j < m.get_cols(); j++)
		{
			m(i, j) = p++;
		}
	}
	m.plain_print();
	cout << endl;

	for (int i = 0; i < 6; i++)
	{
		m[i] += m[i];
	}
	
	m.plain_print();
	cout << endl;
	
	m.transpose();

	m.plain_print();
	cout << endl;

	m.transpose();

	m.plain_print();
	cout << endl;

	m = m * 2;
	
	m.plain_print();
	cout << endl;

	m = 0.25 * m;
	
	m.plain_print();
	cout << endl;

	Matrix m2 = m;
	m.transpose();
	
	m.plain_print();
	cout << endl;
	
	m2.plain_print();
	cout << endl;

	m += m2.transpose();

	m.transpose().plain_print();
	cout << endl;
	
	return 0;
}
