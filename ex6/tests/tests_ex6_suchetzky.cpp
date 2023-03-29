//
// Created by talsu on 08/06/2022.
//UPDATED 14/06
#include "string"
#include <iostream>
#include "assert.h"
#include <stdexcept>
#include "../Dictionary.hpp"
#include "../HashMap.hpp"

// Checks default constructor and methods.
void sanity_check()
{
	//Checks default constructor initialize.
	HashMap<float, size_t> A;
	HashMap<float, size_t> B;
	assert(A.capacity() == 16);
	assert(A.size() == 0);
	assert(A.empty() == true); // might change according to implementation.
	std::cout << "Test 1: default constructor initialize - PASSED." << std::endl;

	//Checks contain and insert.
	assert(A.contains_key(5.5) == false);
	assert(A.insert(5.5, 100) == true);
	assert(A.insert(2.5, 30) == true);
	assert(A.contains_key(5.5) == true);
	assert(A.insert(5.5, 70) == false);

	assert(B.insert(4.3, 30) == true);
	assert(B.insert(4.3, 35) == false);
	assert(B.insert(5.3, 40) == true);
	assert(B.contains_key(4.3) == true);
	assert(B.contains_key(9) == false);

	std::cout << "Test 2: insert and contain - PASSED." << std::endl;

	//Checks at.
	assert(A.at(5.5) == 100);
	assert(B.at(5.3) == 40);
	// at exceptions.
	try
	{
		A.at(5);
		std::cout << "Problem in at, needs to throw exception" << std::endl;
		return;
	}
	catch (...)
	{
	}
	try
	{
		B.at(5);
		std::cout << "Problem in at, needs to throw exception" << std::endl;
		return;
	}
	catch (...)
	{
	}

	std::cout << "Test 3: at - PASSED." << std::endl;

	//Checks erase.
	assert(A.erase(5.5) == true);
	assert(A.contains_key(5.5) == false);
	assert(A.size() == 1);
	assert(A.capacity() == 4);
	assert(A.erase(2.5) == true);
	assert(A.erase(2.5) == false);
	assert(A.empty() == true);

	std::cout << "Test 4: erase - PASSED." << std::endl;

	//Checks get_load_factor.
	assert(A.get_load_factor() == 0);
	assert(B.get_load_factor() == 0.125);

	std::cout << "Test 5: get_load_factor - PASSED." << std::endl;

	//Checks Bucket_size.
#ifdef __linux__
	assert(B.bucket_size(4.3) == 1);
	assert(B.bucket_size(5.3) == 1);
	try
	{
		A.bucket_size(4.3);
		std::cout << "Problem in bucket_size, needs to throw exception" <<
			std::endl;
		return;
	}
	catch (...)
	{
	};
	std::cout << "Test 6: Bucket_size - PASSED." << std::endl;


	//Checks Bucket_index.
	assert(B.bucket_index(4.3) == 11);
	assert(B.bucket_index(5.3) == 5);
	try
	{
		A.bucket_size(4.3);
		std::cout << "Problem in bucket_index, needs to throw exception" <<
			std::endl;
		return;
	}
	catch (...)
	{
	};

	try
	{
		A.bucket_size(5.5);
		std::cout << "Problem in bucket_index, needs to throw exception" <<
			std::endl;
		return;
	}
	catch (...)
	{
	};

	std::cout << "Test 7: Bucket_index - PASSED." << std::endl;
#else
	std::cout << "Test 6 and 7: Bucket checks - on linux only." << std::endl;
#endif


	//Checks clear.
	assert(B.size() == 2);
	B.clear();
	assert(B.size() == 0);
	assert(B.empty() == true);
	assert(B.capacity() == 16);

	std::cout << "Test 8: clear - PASSED." << std::endl;

	//Checks copy constractor.
	A.insert(1, 2);
	A.insert(2, 3);
	HashMap<float, size_t> C = A;
	assert(C.size() == 2);
	assert(C.capacity() == 4);
	assert(C.contains_key(1) == true);
	assert(C.contains_key(2) == true);
	assert(C.contains_key(3) == false);

	std::cout << "Test 9: copy constractor - PASSED." << std::endl;

	//Checks operator =.
	HashMap<float, size_t> D = A;
	assert(D.size() == 2);
	assert(D.capacity() == 4);
	assert(D.contains_key(1) == true);
	assert(D.empty() == false);

	std::cout << "Test 10: operator = PASSED." << std::endl;


	//Checks operator [] (const and non const).
	assert(D[1] == 2);
	assert(D[2] == 3);
	D[1] = 5;
	assert(D[1] == 5);
	A[1] = 7;
	assert(A[1] == 7);
	A[4] = 5; //new place
	assert(A[4] == 5);

	std::cout << "Test 11: operator [] PASSED." << std::endl;

	//Checks operator == !=.
	assert(!(A == D));
	assert(A == A);
	assert(A != D);
	assert(B != A);
	assert(D != C);

	std::cout << "Test 12: operators == != PASSED." << std::endl;

	//Checks constractor 1
	std::vector<float> Keys = { 1.3, 2.3, 8 };
	std::vector<size_t> Values = { 100, 200, 800 };
	HashMap<float, size_t> try_constractor1(Keys, Values);
	assert(try_constractor1.size() == 3);
	assert(try_constractor1.capacity() == 16);
	assert(try_constractor1.insert(5, 8) == true);

	std::cout << "Test 13: constractor 1 PASSED." << std::endl;
	std::cout << std::endl;
	std::cout << "All sanity checks PASSED." << std::endl;
	std::cout << std::endl;
}

void iterator_check()
{
	std::cout << "make sure is prints 100, 200, 300, 400, 500:" << std::endl;
	std::cout << "order doesn't matter." << std::endl;

	std::vector<float> Keys = { 1.3, 2.3, 3.3, 4.3, 5.3 };
	std::vector<size_t> Values = { 100, 200, 300, 400, 500 };
	HashMap<float, size_t> try_constractor1(Keys, Values);
	for (const auto k : try_constractor1)
	{
		std::cout << k.second << std::endl;
	}
	std::cout << "Test 16: first iterator check - PASSED." << std::endl;

	std::vector<int> Keys_1;
	std::vector<std::string> Values_1;
	for (int i = 0; i < 1000; i++)
	{
		Keys_1.push_back(i);
		Values_1.push_back("a");
	}
	HashMap<int, std::string> try_constractor2(Keys_1, Values_1);
	int tmp = 0;
	for (const auto& z : try_constractor2)
	{
		tmp += z.first;
		assert(z.second == "a");
		tmp++;
	};
	assert(tmp == 500500);
	std::cout << "Test 17: second iterator check - PASSED." << std::endl;

	auto test_jumps = try_constractor1.begin();


}

void overload_check()
{
	HashMap<int, int> A;
	for (int i = 0; i < 100; i++)
	{
		A.insert(i, i);
	}
	assert(A.capacity() == 256);
	assert(A.size() == 100);
	std::cout << "Test 14: overload_check - PASSED." << std::endl;

}

void decrease_check()
{
	HashMap<int, int> A;
	for (int i = 0; i < 100; i++)
	{
		A.insert(i, i);
	}
	for (int j = 0; j < 50; j++)
	{
		A.erase(j);
	}
	assert(A.size() == 50);
	assert(A.capacity() == 128);

	for (int j = 50; j < 95; j++)
	{
		A.erase(j);
	}
	assert(A.size() == 5);
	assert(A.capacity() == 16);

	std::cout << "Test 15: decrease_check PASSED." << std::endl;
	std::cout << std::endl;

}

void string_type_check()
{
	HashMap<std::string, std::string> A;
	A.insert("a", "A");
	A.insert("b", "B");
	A.insert("c", "C");
	A.insert("hello", "HELLO");
	assert(A.size() == 4);
	assert(A.capacity() == 16);

}

void some_weird_cases()
{
	std::vector<std::pair<std::string, int>> Values = { {"a", 5},
													   {"b", 6},
													   {"c", 7} };
	std::vector<size_t> Keys = { 100, 200, 300 };
	HashMap<size_t, std::pair<std::string, int>> try_constractor1(Keys, Values);
	for (const auto& x : try_constractor1)
	{
		assert(x.first % 100 == 0);
	}
	std::cout << "Test 18: wierd Values - PASSED." << std::endl;

	HashMap<int, int> B;
	for (int i = -100; i < 900; i += 5)
	{
		B.insert(i, 5);
	}
	for (int i = 0; i < 500; ++i)
	{
		B.erase(i);
	}
	assert(B.empty() == false);
	std::cout << "Test 19: weird insert and erase - PASSED." << std::endl;

	HashMap<int, int> A;
	for (int i = 100; i < 110; i++)
	{
		A.insert(i, 5);
		A.erase(i);
	}
	auto x = A.capacity();
	assert(A.capacity() == 1);
	std::cout << "Test 20: weirder insert and erase - PASSED." << std::endl;

	HashMap<int, int> C;
	for (int i = 100; i < 110; i++)
	{
		C.insert(i, 5 + i);
	}

	for (int i = 100; i < 110; i++)
	{
		C.at(i) = 1;
	}
	for (auto item : C)
	{
		assert(item.second == 1);
	}
	std::cout << "Test 21: at has insert functionality - PASSED." << std::endl;

}

void test_dict()
{

	Dictionary A = Dictionary();
	assert(A.capacity() == 16);
	A.insert("A", "a");
	A.insert("B", "b");
	assert(A.size() == 2);
	A.erase("A");
	A.erase("B");
	assert(A.empty());
	std::cout << "Test 22: default dictionary - PASSED." << std::endl;

	try
	{
		A.erase("A");
	}
	catch (InvalidKey& e)
	{
		std::cout << "Test 23: InvalidKey - PASSED." << std::endl;
	}

	std::vector<std::string> keys = { "a","b","C" };
	std::vector<std::string> values = { "A","B","C" };
	Dictionary C = Dictionary(keys, values);

	std::cout << "Test 24: dictionary with values - PASSED." << std::endl;

	Dictionary B = Dictionary();
	B.insert("a", "1");
	B.insert("b", "2");
	B.insert("c", "3");
	B["a"] = "5";

	std::vector<std::pair<std::string, std::string>> K = { {"c",""},{"a","9"},
														 {"b","b"} };
	auto first = K.begin();
	auto last = K.end();
	B.update(first, last);
	assert(B["a"] == "9");
	assert(B["b"] == "b");
	assert(B["c"] == "");

	std::cout << "Test 25: update methode basic tests - PASSED." << std::endl;



}

int main()
{
	sanity_check();
	overload_check();
	decrease_check();
	string_type_check();
	iterator_check();
	some_weird_cases();
	test_dict();
}