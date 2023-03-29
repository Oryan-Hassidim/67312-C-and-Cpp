#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#include <memory>
#include "RecommenderSystem.h"

using std::unique_ptr;
using std::string;

class RecommenderSystemLoader {

private:
	static bool is_valid_feature(double num);
	static bool proccess_line(string& line,
		string& name, int& year, vector<double>& features);
public:
	RecommenderSystemLoader() = delete;
	/**
	 * loads movies by the given format for movies with their feature's score
	 * @param movies_file_path a path to the file of the movies
	 * @return smart pointer to a RecommenderSystem which was created with
	 * those movies
	 */
	static unique_ptr<RecommenderSystem> create_rs_from_movies_file
	(const string& movies_file_path) noexcept (false);
};

#endif //RECOMMENDERSYSTEMLOADER_H
