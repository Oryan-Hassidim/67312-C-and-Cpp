#include "RecommenderSystemLoader.h"
#include "RSUsersLoader.h"
#include <limits>

using std::cout;
using std::endl;


int main() {
	Movie m("A", 1999);
	cout << m << endl;
	auto rs = RecommenderSystemLoader::create_rs_from_movies_file("RecommenderSystemLoader_input.txt");
	cout << *rs << endl;
	auto users = RSUsersLoader::create_users_from_file("RSUsersLoader_input.txt", std::move(rs));
	users[0].add_movie_to_rs("B", 2000, { 3,3,4,9 }, 10); // same as star wars
	try
	{
		users[0].add_movie_to_rs("C", 2000, { -3,3,4,9 }, 10);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	try
	{
		users[0].add_movie_to_rs("C", 2000, { 3,std::numeric_limits<double>().quiet_NaN(),4,9 }, 10);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	try
	{
		users[0].add_movie_to_rs("C", 2000, { 3,3,4,9 }, 12);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	try
	{
		users[0].add_movie_to_rs("C", 2000, { 3,3,4,9 }, std::numeric_limits<double>().quiet_NaN());
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	cout << users[1] << endl;
	cout << *users[1].get_recommendation_by_content() << endl;
	cout << *users[1].get_recommendation_by_cf(2) << endl;

	for (auto& user : users)
	{
		cout << user << endl;
	}
	
	cout << users[0] << endl;
}