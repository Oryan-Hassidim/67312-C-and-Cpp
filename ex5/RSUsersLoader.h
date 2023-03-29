#ifndef USERFACTORY_H
#define USERFACTORY_H
#include "RSUser.h"
#include "RecommenderSystem.h"

using std::string;
using std::vector;
using std::unique_ptr;


class RSUsersLoader
{
private:
	static bool is_valid_rank(double num);
	static void proccess_filrst_line(string& line,
		vector<pair<sp_movie, double>>& rank_vec,
		RecommenderSystem* rs);
	static bool proccess_line(const string& line,
		string& name, vector<pair<sp_movie, double>>& rank_vec);
public:
	RSUsersLoader() = delete;
	/**
	 *
	 * loads users by the given format with their movie's ranks
	 * @param users_file_path a path to the file of the users and their movie ranks
	 * @param rs RecommendingSystem for the Users
	 * @return vector of the users created according to the file
	 */
	static vector<RSUser> create_users_from_file(
		const string& users_file_path, unique_ptr<RecommenderSystem> rs)
		noexcept(false);

};


#endif //USERFACTORY_H
