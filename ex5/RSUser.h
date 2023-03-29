#ifndef USER_H
#define USER_H
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include "Movie.h"

#define RATE_MIN 1.0
#define RATE_MAX 10.0

using std::unordered_map;
using std::string;
using std::iterator;
using std::pair;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;

class RecommenderSystem;
typedef unordered_map<sp_movie, double, hash_func, equal_func> rank_map;

// typedef for a iterable of pairs - movies to their ranks
typedef vector<pair<sp_movie, double>> rank_list;

class RSUser
{
private:
	string name;
	shared_ptr<RecommenderSystem> rs;
	rank_map ranks;
public:
	/**
	 * Constructor for the class
	 */
	RSUser(const string& name, const rank_list& rank_vec, shared_ptr<RecommenderSystem>& rs);

	/**
	 * a getter for the user's name
	 * @return the username
	 */
	const string& get_name() const;

	/**
	 * a getter for the ranks map
	 * @return
	 */
	const rank_map& get_ranks() const;


	/**
	 * function for adding a movie to the DB
	 * @param name name of movie
	 * @param year year it was made
	 * @param features a vector of the movie's features
	 * @param rate the user rate for this movie
	 */
	void add_movie_to_rs(const string& name, int year,
		const vector<double>& features, double rate);

	/**
	 * returns a recommendation according to the movie's content
	 * @return recommendation
	 */
	sp_movie get_recommendation_by_content() const;

	/**
	 * returns a recommendation according to the similarity recommendation method
	 * @param k the number of the most similar movies to calculate by
	 * @return recommendation
	 */
	sp_movie get_recommendation_by_cf(int k) const;

	/**
	 * predicts the score for a given movie
	 * @param name the name of the movie
	 * @param year the year the movie was created
	 * @param k the parameter which represents the number of the most similar movies to predict the score by
	 * @return predicted score for the given movie
	 */
	double get_prediction_score_for_movie(const string& name, int year, int k) const;

	/**
	 * output stream operator
	 * @param os the output stream
	 * @param user the user
	 * @return output stream
	 */
	friend ostream& operator<<(ostream& os, const RSUser& user);
};



#endif //USER_H
