#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "HashMap.hpp"
#include <string>

using std::string;

/// <summary>
/// an error type for throwing on dictionary erase errors
/// </summary>
class InvalidKey : public std::invalid_argument
{
public:
	InvalidKey(const string& key)
		: invalid_argument("The key " + key + "doesn't exist in the dictionary.")
	{}
	InvalidKey()
		: invalid_argument("The key doesn't exist in the dictionary.")
	{}
};


class Dictionary : public HashMap<string, string>
{
private:
	typedef std::pair<string, string> pair;
public:
	// ctors
	Dictionary() {}
	Dictionary(const vector<string>& keys, const vector<string>& values) : HashMap(keys, values) {}
	Dictionary(const Dictionary& other) : HashMap(other) {}
	/// <summary>
	/// Erases the element with the given key from the dictionary.
	/// If the key doesn't exist, throws an InvalidKey exception.
	/// </summary>
	/// <param name="key">the key of the element to erase.</param>
	/// <returns>true on success, false otherwise.</returns>
	/// <exception cref="InvalidKey">thrown if the key doesn't exist.</exception>
	bool erase(const string& key) noexcept(false) override;

	/// <summary>
	/// Updates the value of the elements with the given key according
	/// of a given iterator.
	/// </summary>
	/// <typeparam name="InIt">the iterator type.</typeparam>
	/// <param name="begin">the begin iterator.</param>
	/// <param name="end">the end iterator.</param>
	template <typename InIt> void update(InIt begin, InIt end);
};

bool Dictionary::erase(const string& key)
{
	if (!contains_key(key))
	{
		throw InvalidKey(key);
	}
	return HashMap::erase(key);
}

template<typename InIt>
inline void Dictionary::update(InIt begin, InIt end)
{
	for_each(begin, end, [this](const pair& p)
		{ (*this)[p.first] = p.second; });
}

#endif // !DICTIONARY_HPP