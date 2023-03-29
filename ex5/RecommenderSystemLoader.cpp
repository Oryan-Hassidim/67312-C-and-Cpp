#include "RecommenderSystemLoader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>

#define NAME_DELIM '-'
#define FEATURE_DELIM ' '

using std::make_unique;
using std::move;
using std::ifstream;
using std::ios;
using std::getline;
using std::istringstream;
using std::modf;
using std::unique_ptr;
using std::to_string;

bool RecommenderSystemLoader::is_valid_feature(double num)
{
	return num >= FEATURE_MIN && num <= FEATURE_MAX;
}

bool RecommenderSystemLoader::proccess_line(string& line,
	string& name, int& year, vector<double>& features)
{
	line.replace(line.find(NAME_DELIM), 1, " ");
	istringstream is(line);

	is >> name;

	is >> year;

	// push the features.
	double feature = 0;
	while (is >> feature)
	{
		if (!is_valid_feature(feature))
		{
			return false;
		}
		features.push_back(feature);
	}
	return true;
}

unique_ptr<RecommenderSystem> RecommenderSystemLoader::create_rs_from_movies_file(
	const string& movies_file_path) noexcept(false)
{
	ifstream is(movies_file_path, ios::in);
	if (!is.is_open())
	{
		throw std::invalid_argument("Error: Failed to open file");
	}
	
	unique_ptr<RecommenderSystem> rs = make_unique<RecommenderSystem>();
	string name, line;
	vector<double> features;
	int year = 0, line_num = 1;

	while (getline(is, line))
	{
		if (!proccess_line(line, name, year, features))
		{
			is.close();
			throw std::range_error("feature must be between 1 to 10. \nline: " +
				to_string(line_num) + ".");
		}
		rs->add_movie(name, year, features);
		features.clear();
		++line_num;
	}

	is.close();

	return rs;
}
