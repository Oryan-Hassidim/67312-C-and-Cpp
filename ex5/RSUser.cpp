#include "RSUser.h"
#include "RecommenderSystem.h"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <limits>
#include <cassert>

using std::endl;
using std::isnan;

RSUser::RSUser(
	const string& name,
	const rank_list& rank_vec,
	shared_ptr<RecommenderSystem>& rs)
	: name(name), rs(rs),
	ranks(2 * rank_vec.size(), sp_movie_hash, sp_movie_equal)
{
	for (const auto& pair : rank_vec)
	{
		ranks[pair.first] = pair.second;
	}
}

const string& RSUser::get_name() const
{
	return name;
}

const rank_map& RSUser::get_ranks() const
{
	return ranks;
}

void RSUser::add_movie_to_rs(const string& name, int year,
	const vector<double>& features, double rate)
{
	if (isnan(rate) || rate < RATE_MIN || rate > RATE_MAX)
	{
		throw std::invalid_argument("Invalid rate");
	}
	rs->add_movie(name, year, features);
	ranks[rs->get_movie(name, year)] = rate;
}

sp_movie RSUser::get_recommendation_by_content() const
{
	return rs->recommend_by_content(*this);
}

sp_movie RSUser::get_recommendation_by_cf(int k) const
{
	return rs->recommend_by_cf(*this, k);
}

double RSUser::get_prediction_score_for_movie(const string& name, int year, int k) const
{
	auto movie = rs->get_movie(name, year);
	return rs->predict_movie_score(*this, movie, k);
}

ostream& operator<<(ostream& os, const RSUser& user)
{
	os << "name: " << user.name << endl << *user.rs << endl;
	return os;
}
