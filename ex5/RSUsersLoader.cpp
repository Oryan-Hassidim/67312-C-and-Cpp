#include "RSUsersLoader.h"
#include <sstream>
#include <fstream>
#include <array>
#include <algorithm>
#include <cmath>
#include <limits>

#define NAME_DELIM '-'
#define BEGIN 0Ui64

using std::replace;
using std::size_t;
using std::string;
using std::vector;
using std::array;
using std::ifstream;
using std::ios;
using std::getline;
using std::istringstream;
using std::modf;
using std::to_string;
using std::make_pair;
using std::make_shared;

bool RSUsersLoader::is_valid_rank(double num)
{
	return num >= RATE_MIN && num <= RATE_MAX;
}

void RSUsersLoader::proccess_filrst_line(string& line,
	vector<pair<sp_movie, double>>& rank_vec, RecommenderSystem* rs)
{
	replace(line.begin(), line.end(), '-', ' ');
	istringstream is(line);
	string name;
	int year;
	while (true)
	{
		is >> name;
		is >> year;

		sp_movie movie = rs->get_movie(name, year);
		rank_vec.emplace_back(movie, 0.0);
		if (is.eof())
		{
			break;
		}
	}
}


bool RSUsersLoader::proccess_line(const string& line,
	string& name, vector<pair<sp_movie, double>>& rank_vec)
{
	istringstream is_s(line);
	istringstream is_d(line);

	is_s >> name;
	is_d >> name;

	// push the rank_list.
	string rank_str;
	double rank = 0;
	int i = -1;
	while (!is_s.eof())
	{
		is_s >> rank_str;
		if (rank_str == "NA")
		{
			is_d >> rank_str;
			rank = std::numeric_limits<double>::quiet_NaN();
		}
		else
		{
			is_d >> rank;
			if (!is_valid_rank(rank))
			{
				return false;
			}
		}
		rank_vec[++i].second = rank;
	}
	return true;
}

vector<RSUser> RSUsersLoader::create_users_from_file(
	const string& users_file_path, unique_ptr<RecommenderSystem> rs)
	noexcept(false)
{
	vector<RSUser> users;

	vector<pair<sp_movie, double>> rank_vec;
	ifstream is(users_file_path, ios::in);

	if (!is.is_open())
	{
		throw std::invalid_argument("Error: Failed to open file");
	}

	string line, name;
	getline(is, line);
	proccess_filrst_line(line, rank_vec, rs.get());

	shared_ptr<RecommenderSystem> sp_rs = std::move(rs);

	while (getline(is, line))
	{
		if (!proccess_line(line, name, rank_vec))
		{
			is.close();
			throw std::invalid_argument("Error: Invalid input");
		}
		RSUser user(name, rank_vec, sp_rs);
		users.push_back(user);
	}

	is.close();

	return users;
}
