#include <iostream>
#include <limits>
#include <numeric>
#include <fstream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::ostream;
using std::vector;

//template <typename T>
//ostream& operator<<(ostream& os, vector<T> v)
//{
//	for (size_t i = 0; i < v.size() - 1; ++i)
//	{
//		os << v[i] << ", ";
//	}
//	os << v[v.size() - 1];
//	return os;
//}
//
//int FirstMissingPositive(vector<int>& vec)
//{
//	int invalid = 0, trash = vec.size();
//
//	while (invalid <= trash - 1)
//	{
//		cout << vec << " ~ invalid: " << invalid << " ~ trash: " << trash << endl;
//		if (vec[invalid] == invalid)
//			++invalid;
//		else if (vec[invalid] < 0 || vec[invalid] >= trash
//			|| vec[invalid] == vec[vec[invalid]])
//			swap(vec[invalid], vec[--trash]);
//		else
//			swap(vec[invalid], vec[vec[invalid]]);
//	}
//	return invalid;
//}

#include <iostream>
void bar(long& x) {
	std::cout << "bar(long &x)" << std::endl;
}
void bar(long&& x) {
	std::cout << "bar(long &&x)" << std::endl;
}
void foo(long& x) {
	std::cout << "foo(long &x)" << std::endl;
	bar(x);
}
void foo(long&& x) {
	std::cout << "foo(long &&x)" << std::endl;
	bar(x);
}
int main() {
	int x = 10;
	foo(x);
	foo(10);
	foo((long)&x);
	return 0;
}
