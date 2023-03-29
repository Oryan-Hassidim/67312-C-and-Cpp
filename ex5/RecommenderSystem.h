#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include <map>
#include <unordered_map>
#include <functional>

#define FEATURE_MIN 1.0
#define FEATURE_MAX 10.0


using std::map;
using std::pair;
using std::function;

typedef vector<double> vec;
typedef bool (*sp_movies_less_then_func)(sp_movie, sp_movie);
typedef map<sp_movie, vec, sp_movies_less_then_func> movie_map;
typedef function<bool(movie_map::const_reference)> filter_pair_func;
typedef function<double(movie_map::const_reference)> eval_pair_func;

class RecommenderSystem
{
private:
	// function to order sp_movie by movie comparison.
	static bool sp_movies_less_then(sp_movie a, sp_movie b);
	// calculates the norm of a given vector.
	static double norm(vec vec);
	// normalizes ranks_map around its average.
	static void normalize(rank_map& ranks);
	// adds v2 multiplied by factor to v1.
	static void add(vec& v1, double factor, const vec& v2);
	// calculates the inner product of two vectors.
	static double inner_product(const vec& v1, const vec& v2);
	// creates vector of zeros with a given size.
	static vec zeros(size_t size);
	// returns true if the given user watched the given movie.
	static bool user_watched(const RSUser& user, const sp_movie& movie);
	// find the k largest movies in the database according to a given filter and evaluation function.
	vector<pair<sp_movie, double>> find_k_largest(filter_pair_func, eval_pair_func, int k);
	// validates features vector
	static bool validate_features(const vec& vec);


	movie_map movies;
public:
	explicit RecommenderSystem();
	/**
	 * adds a new movie to the system
	 * @param name name of movie
	 * @param year year it was made
	 * @param features features for movie
	 * @return shared pointer for movie in system
	 */
	sp_movie add_movie(const string& name, int year, const vec& features);


	/**
	 * a function that calculates the movie with highest score based on movie features
	 * @param ranks user ranking to use for algorithm
	 * @return shared pointer to movie in system
	 */
	sp_movie recommend_by_content(const RSUser& user);

	/**
	 * a function that calculates the movie with highest predicted score based on ranking of other movies
	 * @param ranks user ranking to use for algorithm
	 * @param k
	 * @return shared pointer to movie in system
	 */
	sp_movie recommend_by_cf(const RSUser& user, int k);


	/**
	 * Predict a user rating for a movie given argument using item cf procedure with k most similar movies.
	 * @param user_rankings: ranking to use
	 * @param movie: movie to predict
	 * @param k:
	 * @return score based on algorithm as described in pdf
	 */
	double predict_movie_score(const RSUser& user, const sp_movie& movie,
		int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string& name, int year) const;


	friend ostream& operator<<(ostream& os, const RecommenderSystem& rs);
};


#endif //RECOMMENDERSYSTEM_H
