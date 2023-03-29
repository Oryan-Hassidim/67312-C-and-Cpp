#include "RecommenderSystem.h"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <limits>
#include <cassert>
#include "RSUser.h"

using std::multimap;
using std::copy;
using std::make_shared;
using std::make_pair;
using std::next;
using std::accumulate;
using std::for_each;
using std::isnan;
using std::abs;

typedef rank_map::reference pair_ref;

bool RecommenderSystem::sp_movies_less_then(sp_movie a, sp_movie b)
{
	return *a < *b;
}

double RecommenderSystem::norm(vec vec)
{
	return std::sqrt(accumulate(vec.begin(), vec.end(), 0.0,
		[](double sum, double val) { return sum + val * val; }));
}

void RecommenderSystem::normalize(rank_map& ranks)
{
	auto folder = [](pair<int, double> so_far, rank_map::const_reference pair)
	{
		if (!isnan(pair.second))
		{
			so_far.first++;
			so_far.second += pair.second;
		};
		return so_far;
	};
	auto result = accumulate(ranks.begin(), ranks.end(), make_pair(0, 0.0), folder);
	auto avg = result.second / result.first;
	auto normalizer = [avg](rank_map::reference pair) { pair.second -= avg; };
	for_each(ranks.begin(), ranks.end(), normalizer);
}

void RecommenderSystem::add(vec& v1, double factor, const vec& v2)
{
	auto size = v1.size();
	for (size_t i = 0; i < size; i++)
	{
		v1[i] += factor * v2[i];
	}
}

double RecommenderSystem::inner_product(const vec& v1, const vec& v2)
{
	double sum = 0;
	for (size_t i = 0; i < v1.size(); i++)
	{
		sum += v1[i] * v2[i];
	}
	return sum;
}

vec RecommenderSystem::zeros(size_t size)
{
	return vec(size, 0.0);
}

bool RecommenderSystem::user_watched(const RSUser& user, const sp_movie& movie)
{
	const auto& ranks = user.get_ranks();
	auto found = ranks.find(movie);
	return found != ranks.end() && !isnan(found->second);
}

vector<pair<sp_movie, double>> RecommenderSystem::find_k_largest(
	filter_pair_func filter, eval_pair_func eval, int k)
{
	// create a map to save the k largest so far.
	multimap<double, sp_movie> result;
	auto cur = movies.begin();
	// take the first k that filtered.
	do
	{
		if (filter(*cur))
		{
			result.emplace(eval(*cur), cur->first);
		}
		++cur;
	} while (result.size() < (size_t)k);
	double min = result.begin()->first;
	// save the k largest in the map.
	for (; cur != movies.end(); ++cur)
	{
		if (!filter(*cur))
		{
			continue;
		}
		double score = eval(*cur);
		if (score > min)
		{
			result.erase(result.begin());
			result.emplace(eval(*cur), cur->first);
			min = result.begin()->first;
		}
	}
	// return the map as a vector.
	vector<pair<sp_movie, double>> final_result;
	for (const auto& pair : result)
	{
		final_result.push_back(make_pair(pair.second, pair.first));
	}
	return final_result;
}

bool RecommenderSystem::validate_features(const vec& vec)
{
	return std::find_if(vec.begin(), vec.end(), 
		[](const double& val)
		{ return isnan(val) || val < FEATURE_MIN || FEATURE_MAX<val; }) == vec.end();
}

RecommenderSystem::RecommenderSystem()
	: movies(sp_movies_less_then)
{
}

sp_movie RecommenderSystem::add_movie(const string& name, int year,
	const vec& features)
{
	if (!validate_features(features))
	{
		throw std::invalid_argument("invalid feature");
	}
	sp_movie movie = make_shared<Movie>(name, year);
	movies[movie] = features;
	return movie;
}

sp_movie RecommenderSystem::recommend_by_content(const RSUser& user)
{
	rank_map rank(user.get_ranks());
	// normalize the user's ranks.
	normalize(rank);
	// vector of the user's features.
	vec user_vec = zeros(movies.begin()->second.size());
	for (auto& pair : rank)
	{
		if (user_watched(user, pair.first))
		{
			add(user_vec, pair.second, movies[pair.first]);
		}
	}
	double user_vec_norm = norm(user_vec);

	// find the most similar unwatched movie.
	auto filter = [&user](movie_map::const_reference& pair)
	{ return !user_watched(user, pair.first); };
	auto eval = [&user_vec, user_vec_norm](movie_map::const_reference& pair)
	{ return inner_product(user_vec, pair.second)
		/ (user_vec_norm * norm(pair.second)); };
	auto result = find_k_largest(filter, eval, 1);
	return result.begin()->first;
}

sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k)
{
	filter_pair_func filter = [&user](movie_map::const_reference& pair)
	{ return !user_watched(user, pair.first); };
	eval_pair_func eval = [&user, this, k](movie_map::const_reference& pair)
	{return predict_movie_score(user, pair.first, k); };
	return find_k_largest(filter, eval, 1).begin()->first;
}

double RecommenderSystem::predict_movie_score(const RSUser& user,
	const sp_movie& movie, int k)
{
	const auto& user_ranks = user.get_ranks();
	if (user_watched(user, movie))
	{
		return user_ranks.at(movie);
	}
	const auto& movie_rank = movies[movie];
	auto movie_norm = norm(movie_rank);

	// find the k most similar movies.
	filter_pair_func filter = [&user, &movie](movie_map::const_reference pair)
	{return user_watched(user, pair.first); };
	eval_pair_func eval =
		[&movie_rank, movie_norm](movie_map::const_reference pair)
	{return inner_product(movie_rank, pair.second)
		/ (movie_norm * norm(pair.second)); };
	auto n = find_k_largest(filter, eval, k);

	// calculate the prediction
	double numerator = accumulate(n.begin(), n.end(), 0.0,
		[&user_ranks](double so_far, const pair< sp_movie, double>& pair)
		{return so_far + pair.second * user_ranks.at(pair.first); });
	double enominator = accumulate(n.begin(), n.end(), 0.0,
		[&user_ranks](double so_far, const pair< sp_movie, double>& pair)
		{return so_far + pair.second; });

	return numerator / enominator;
}

sp_movie RecommenderSystem::get_movie(const string& name, int year) const
{
	sp_movie key = make_shared<Movie>(name, year);
	auto res = movies.find(key);
	return res == movies.end() ? nullptr : res->first;
}

ostream& operator<<(ostream& os, const RecommenderSystem& rs)
{
	for (const auto& pair : rs.movies)
	{
		os << *pair.first;
	}
	return os;
}
