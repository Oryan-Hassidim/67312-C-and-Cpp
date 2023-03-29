#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include "HashMap.hpp"
#include "Dictionary.hpp"


using std::cout;
using std::endl;
using std::string;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<T>& sp)
{
	os << "sp: " << *sp;
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::unique_ptr<T>& up)
{
	os << "up: " << *up;
	return os;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p)
{
	os << p.first << ": " << p.second;
	return os;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const HashMap<T, U>& m)
{
	os << "HashMap:\n\tcapacity: " << m.capacity() << "\n\tsize: " << m.size() << "\n\tload factor: " << m.get_load_factor() << endl;
	for (const auto& pair : m)
		os << pair << endl;
	return os;
}


int main()
{
	HashMap<int, int> map;

	for (int i = 0; i < 50; i++)
	{
		map[i] = rand() % 100;
	}

	for (const auto& i : map)
	{
		cout << i << endl;
	}

	const HashMap<int, int> m2 = map;
	for (auto& i : m2)
	{
		cout << i << endl;
	}
	for (int i = 0; i < m2.size(); i++)
	{
		cout << m2[i] << endl;
	}

	std::for_each(map.begin(), map.end(), [](const auto& p) { cout << p << endl; });

	HashMap<string, std::shared_ptr<int>> m3;

	for (int i = 0; i < 168; i++)
	{
		string key = std::to_string(i);
		m3[key] = std::make_shared<int>(i);
	}

	cout << m3 << endl;

	for (int i = 0; i < 168; i += 3)
	{
		string key = std::to_string(i);
		m3.erase(key);
	}

	cout << m3 << endl;
	// m3.clear();

	map.clear();



	return 0;
}