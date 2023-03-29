#ifndef HASHMAPHPP
#define HASHMAPHPP

#define DEFUALT_INITIAL_SIZE 16
#define FACTOR 2
#define MAX_LOAD_FACTOR 0.75
#define MIN_LOAD_FACTOR 0.25

#include <utility>
#include <vector>
#include <functional>
#include <string>
#include <stdexcept>
#include <algorithm>

using std::make_pair;
using std::vector;
using std::string;
using std::invalid_argument;
using std::out_of_range;
using std::swap;
using std::for_each;
using std::find_if;
using std::all_of;
using std::any_of;

template <typename KeyT, typename ValueT>
class HashMap
{
private:
	typedef std::pair<KeyT, ValueT> pair;
	/// <summary>
	/// helper class to store the data.
	/// </summary>
	class Bucket
	{
	public:
		int _capacity, _size;
		pair* _data;
		Bucket();
		~Bucket();
		bool contains_key(const KeyT& key) const;
		void insert(const KeyT& key, const ValueT& value);
		void remove(const KeyT& key);
		void clear();
		ValueT& operator[](const KeyT& key);
		const ValueT& operator[](const KeyT& key) const;
	private:
	};

	int _capacity, _size;
	Bucket* _buckets;
	ValueT _trash; // for return on unfound key on const [] op

	std::hash<KeyT> _hasher;
	int hash(const KeyT& key) const;
	void validate_key(const KeyT& key) const noexcept(false);

	static int find_capacity(int size);

	HashMap(int capacity); // private helper ctor
	HashMap(int capacity, const HashMap& other); // private copy ctor

public:
	/// <summary>
	/// builds an empty HashMap with size 16.
	/// </summary>
	HashMap();
	/// <summary>
	/// builds an empty HashMap and initializes the data in the HashMap.
	/// </summary>
	/// <param name="keys">vector of keys.</param>
	/// <param name="values">vector of values.</param>
	HashMap(const vector<KeyT>& keys, const vector<ValueT>& values) noexcept(false);
	/// <summary>
	/// copy constructor.
	/// </summary>
	/// <param name="other"></param>
	HashMap(const HashMap& other);
	/// <summary>
	/// destructor.
	/// </summary>
	virtual ~HashMap();

	/// <summary>
	/// Gets the element count in the map.
	/// </summary>
	/// <returns>the size of the map.</returns>
	int size() const;
	/// <summary>
	/// Gets the capacity of the map.
	/// </summary>
	/// <returns>the capacity of the map.</returns>
	int capacity() const;
	/// <summary>
	/// Indicates if the map is empty or not.
	/// </summary>
	/// <returns>true if the map is empty, false otherwise.</returns>
	bool empty() const;

	/// <summary>
	/// inserts a new element to the map if it's not already in the map.
	/// </summary>
	/// <param name="key">the key of the new element.</param>
	/// <param name="value">the value of the new element.</param>
	/// <returns>true if the element was inserted, false otherwise.</returns>
	bool insert(const KeyT& key, const ValueT& value);
	/// <summary>
	/// Indicates weather the map contains the given key or not.
	/// </summary>
	/// <param name="key">the key to search for.</param>
	/// <returns>true if the map contains the key, false otherwise.</returns>
	bool contains_key(const KeyT& key) const;
	/// <summary>
	/// Gets the value of the given key. 
	/// throws an exception if the key is not in the map.
	/// </summary>
	/// <param name="key">the key to search for.</param>
	/// <returns>the value of the given key.</returns>
	/// <exception cref="std::out_of_range">thrown if the key is not in the map.</exception>
	ValueT& at(const KeyT& key) noexcept(false);
	/// <summary>
	/// Gets the value of the given key. 
	/// throws an exception if the key is not in the map.
	/// </summary>
	/// <param name="key">the key to search for.</param>
	/// <returns>the value of the given key.</returns>
	/// <exception cref="std::out_of_range">
	/// thrown if the key is not in the map.
	/// </exception>
	const ValueT& at(const KeyT& key) const noexcept(false);
	/// <summary>
	/// Erases the element with the given key from the map.
	/// </summary>
	/// <param name="key">the key of the element to erase.</param>
	/// <returns>true on success, false otherwise.</returns>
	virtual bool erase(const KeyT& key);
	/// <summary>
	/// Gets the load factor of the map.
	/// </summary>
	/// <returns>the load factor of the map.</returns>
	double get_load_factor() const;
	/// <summary>
	/// Gets the bucket's size of a given key.
	/// </summary>
	/// <param name="key">the key to search for.</param>
	/// <returns>the bucket's size of the given key.</returns>
	/// <exception cref="std::out_of_range">
	/// thrown if the key is not in the map.
	/// </exception>
	int bucket_size(const KeyT& key) const noexcept(false);
	/// <summary>
	/// Gets the bucket's index of a given index.
	/// </summary>
	/// <param name="key">the key to search for.</param>
	/// <returns>the bucket's index of the given key.</returns>
	/// <exception cref="std::out_of_range">
	/// thrown if the key is not in the map.
	/// </exception>
	int bucket_index(const KeyT& key) const noexcept(false);
	/// <summary>
	/// Clears the map entirely.
	/// </summary>
	void clear();

	/// <summary>
	/// Assignment operator.
	/// </summary>
	/// <param name="other">the map to copy.</param>
	/// <returns>a reference to the map.</returns>
	HashMap<KeyT, ValueT>& operator=(const HashMap<KeyT, ValueT>& other);
	/// <summary>
	/// subscript operator.
	/// </summary>
	/// <param name="key">the key to search for.</param>
	/// <returns>the value of the given key.</returns>
	ValueT& operator[](const KeyT& key);
	/// <summary>
	/// subscript operator.
	/// </summary>
	/// <param name="key">the key to search for.</param>
	/// <returns>the value of the given key.</returns>
	const ValueT& operator[](const KeyT& key) const;
	/// <summary>
	/// equality operator.
	/// </summary>
	/// <param name="rhs">the map to compare to.</param>
	/// <returns>true if the maps are equal, false otherwise.</returns>
	bool operator==(const HashMap<KeyT, ValueT>& rhs) const;
	/// <summary>
	/// inequality operator.
	/// </summary>
	/// <param name="rhs">the map to compare to.</param>
	/// <returns>true if the maps are not equal, false otherwise.</returns>
	bool operator!=(const HashMap<KeyT, ValueT>& rhs) const;


	class ConstIterator
	{
	private:
		const Bucket* m_ptr;
		int i, j, cap;
		void validate() // moves to the closest exist index
		{
			while (i < cap && j == m_ptr[i]._size)
			{
				j = 0;
				++i;
			}
		}
	public:
		typedef const pair value_type;
		typedef const pair& reference;
		typedef const pair* pointer;
		typedef int difference_type;
		typedef std::forward_iterator_tag iterator_category;
		ConstIterator(const Bucket* ptr, int i, int j, int cap)
			: m_ptr(ptr), i(i), j(j), cap(cap)
		{
			validate();
		}
		const pair& operator*() const
		{
			return m_ptr[i]._data[j];
		}
		const pair* operator->()
		{
			return m_ptr[i]._data + j;
		}
		ConstIterator& operator++()
		{
			++j;
			validate();
			return *this;
		}
		ConstIterator operator++(int)
		{
			ConstIterator tmp = *this;
			++(*this);
			return tmp;
		}
		friend bool operator== (const ConstIterator& a, const ConstIterator& b) {
			return a.m_ptr == b.m_ptr
				&& a.i == b.i
				&& a.j == b.j;
		};
		friend bool operator!= (const ConstIterator& a, const ConstIterator& b) {
			return !(a == b);
		};

	};

	typedef ConstIterator const_iterator;

	const_iterator begin() const
	{
		return cbegin();
	}
	const_iterator end() const
	{
		return cend();
	}
	const_iterator cbegin() const
	{
		return ConstIterator(_buckets, 0, 0, _capacity);
	}
	const_iterator cend() const
	{
		return ConstIterator(_buckets, _capacity, 0, _capacity);
	}
};

template<typename KeyT, typename ValueT>
inline HashMap<KeyT, ValueT>::Bucket::Bucket()
	: _capacity(0), _size(0), _data(nullptr)
{
}

template<typename KeyT, typename ValueT>
inline HashMap<KeyT, ValueT>::Bucket::~Bucket()
{
	if (_capacity > 0)
	{
		delete[] _data;
	}
}

template<typename KeyT, typename ValueT>
inline bool HashMap<KeyT, ValueT>::Bucket::contains_key(const KeyT& key) const
{
	return any_of(_data, _data + _size, [&key](const pair& pair)
		{ return pair.first == key; });
}

template<typename KeyT, typename ValueT>
inline void HashMap<KeyT, ValueT>::Bucket::insert(const KeyT& key, const ValueT& value)
{
	++_size;
	if (_capacity < _size) // resize the array
	{
		_capacity = _capacity == 0 ? 1 : _capacity * 2;
		auto new_data = new pair[_capacity];
		for (int i = 0; i < _size - 1; i++)
		{
			new_data[i] = _data[i];
		}
		(_data != nullptr ? delete[] _data : void());
		_data = new_data;
	}
	_data[_size - 1] = pair(key, value);
}

template<typename KeyT, typename ValueT>
inline void HashMap<KeyT, ValueT>::Bucket::remove(const KeyT& key)
{
	int i = 0;
	for (; !(_data[i].first == key); i++) {
	}
	for (; i < _size - 1; i++)
	{
		_data[i] = _data[i + 1];
	}
	_data[i] = pair();
	--_size;
}

template<typename KeyT, typename ValueT>
inline void HashMap<KeyT, ValueT>::Bucket::clear()
{
	for_each(_data, _data + _size, [](pair& p) { p = pair(); });
	_size = 0;
}

template<typename KeyT, typename ValueT>
inline ValueT& HashMap<KeyT, ValueT>::Bucket::operator[](const KeyT& key)
{
	return find_if(_data, _data + _size,
		[&key](const pair& pair) { return pair.first == key; })
		->second;
}

template<typename KeyT, typename ValueT>
inline const ValueT& HashMap<KeyT, ValueT>::Bucket::operator[](const KeyT& key) const
{
	return find_if(_data, _data + _size,
		[&key](const pair& pair) { return pair.first == key; })
		->second;
}

template<typename KeyT, typename ValueT>
inline int HashMap<KeyT, ValueT>::hash(const KeyT& key) const
{
	return _hasher(key) & (_capacity - 1);
}

template<typename KeyT, typename ValueT>
inline void HashMap<KeyT, ValueT>::validate_key(const KeyT& key) const noexcept(false)
{
	if (!contains_key(key))
	{
		throw out_of_range("The given key doesn't exist in the map!");
	}
}

template<typename KeyT, typename ValueT>
inline int HashMap<KeyT, ValueT>::find_capacity(int size)
{
	int capacity = DEFUALT_INITIAL_SIZE;
	while (MAX_LOAD_FACTOR <= (double)size / capacity)
	{
		capacity <<= 1;
	}
	return capacity;
}

template<typename KeyT, typename ValueT>
inline HashMap<KeyT, ValueT>::HashMap(int capacity)
	: _capacity(capacity), _size(0), _buckets(nullptr), _trash(), _hasher()
{
	_buckets = new Bucket[_capacity];
}

template<typename KeyT, typename ValueT>
inline HashMap<KeyT, ValueT>::HashMap(int capacity, const HashMap& other) : HashMap(capacity)
{
	for_each(other.begin(), other.end(), [this](const pair& pair)
		{ (*this)[pair.first] = pair.second; });
}

template<typename KeyT, typename ValueT>
inline HashMap<KeyT, ValueT>::HashMap() : HashMap(DEFUALT_INITIAL_SIZE)
{
}

template<typename KeyT, typename ValueT>
inline HashMap<KeyT, ValueT>::HashMap(const vector<KeyT>& keys, const vector<ValueT>& values)
noexcept(false)
	: _capacity(0), _size(0), _buckets(nullptr), _trash(), _hasher()
{
	size_t keys_size = keys.size();
	if (keys_size != values.size())
	{
		throw invalid_argument("keys and values vectors must be of same size!");
	}
	_capacity = find_capacity(_size);
	_buckets = new Bucket[_capacity];
	for (size_t i = 0; i < keys_size; i++)
	{
		(*this)[keys[i]] = values[i];
	}
}

template<typename KeyT, typename ValueT>
inline HashMap<KeyT, ValueT>::HashMap(const HashMap& other)
	: HashMap(other._capacity, other)
{
}

template<typename KeyT, typename ValueT>
inline HashMap<KeyT, ValueT>::~HashMap()
{
	if (_capacity > 0)
	{
		delete[] _buckets;
	}
}

template<typename KeyT, typename ValueT>
inline int HashMap<KeyT, ValueT>::size() const
{
	return _size;
}

template<typename KeyT, typename ValueT>
inline int HashMap<KeyT, ValueT>::capacity() const
{
	return _capacity;
}

template<typename KeyT, typename ValueT>
inline bool HashMap<KeyT, ValueT>::empty() const
{
	return _size == 0;
}

template<typename KeyT, typename ValueT>
inline bool HashMap<KeyT, ValueT>::insert(const KeyT& key, const ValueT& value)
{
	if (contains_key(key))
	{
		return false;
	}
	(*this)[key] = value;
	return true;
}

template<typename KeyT, typename ValueT>
inline bool HashMap<KeyT, ValueT>::contains_key(const KeyT& key) const
{
	return _buckets[hash(key)].contains_key(key);
}

template<typename KeyT, typename ValueT>
inline ValueT& HashMap<KeyT, ValueT>::at(const KeyT& key) noexcept(false)
{
	validate_key(key);
	return (*this)[key];
}

template<typename KeyT, typename ValueT>
inline const ValueT& HashMap<KeyT, ValueT>::at(const KeyT& key) const noexcept(false)
{
	validate_key(key);
	return (*this)[key];
}

template<typename KeyT, typename ValueT>
inline bool HashMap<KeyT, ValueT>::erase(const KeyT& key)
{
	if (!contains_key(key))
	{
		return false;
	}

	_buckets[hash(key)].remove(key);
	--_size;
	if (get_load_factor() < MIN_LOAD_FACTOR && _capacity > 1)
	{
		// rehash
		auto new_capacity = _capacity;
		while (new_capacity > 1 && (double)_size / new_capacity < MIN_LOAD_FACTOR)
		{
			new_capacity >>= 1;
		}
		auto new_hash_map = HashMap(new_capacity, *this);
		swap(_buckets, new_hash_map._buckets);
		swap(_capacity, new_hash_map._capacity);
	}
	return true;
}

template<typename KeyT, typename ValueT>
inline double HashMap<KeyT, ValueT>::get_load_factor() const
{
	return (double)_size / _capacity;
}

template<typename KeyT, typename ValueT>
inline int HashMap<KeyT, ValueT>::bucket_size(const KeyT& key) const noexcept(false)
{
	validate_key(key);
	return _buckets[hash(key)]._size;
}

template<typename KeyT, typename ValueT>
inline int HashMap<KeyT, ValueT>::bucket_index(const KeyT& key) const noexcept(false)
{
	validate_key(key);
	return hash(key);
}

template<typename KeyT, typename ValueT>
inline void HashMap<KeyT, ValueT>::clear()
{
	for_each(_buckets, _buckets + _capacity, [](Bucket& b) { b.clear(); });
	_size = 0;
}

template<typename KeyT, typename ValueT>
inline HashMap<KeyT, ValueT>& HashMap<KeyT, ValueT>::operator=(const HashMap<KeyT, ValueT>& other)
{
	if (this != &other)
	{
		// copy and swap
		HashMap<KeyT, ValueT> temp(other);
		swap(_buckets, temp._buckets);
		swap(_size, temp._size);
		swap(_capacity, temp._capacity);
	}
	return *this;
}

template<typename KeyT, typename ValueT>
inline ValueT& HashMap<KeyT, ValueT>::operator[](const KeyT& key)
{
	Bucket& bucket = _buckets[hash(key)];
	if (bucket.contains_key(key))
	{
		return bucket[key];
	}
	// insert new
	bucket.insert(key, ValueT());
	++_size;
	if (get_load_factor() > MAX_LOAD_FACTOR)
	{
		// re-hash
		auto new_hash_map = HashMap(_capacity << 1, *this);
		swap(_buckets, new_hash_map._buckets);
		swap(_capacity, new_hash_map._capacity);
	}
	return (*this)[key];
}

template<typename KeyT, typename ValueT>
inline const ValueT& HashMap<KeyT, ValueT>::operator[](const KeyT& key) const
{
	Bucket& bucket = _buckets[hash(key)];
	// return trash if key not found, that the user can modify
	return bucket.contains_key(key) ? bucket[key] : _trash;
}

template<typename KeyT, typename ValueT>
inline bool HashMap<KeyT, ValueT>::operator==(const HashMap<KeyT, ValueT>& rhs) const
{
	return _size == rhs._size
		&& all_of(rhs.cbegin(), rhs.cend(), [this](const pair& pair)
			{ return this->contains_key(pair.first)
			&& (*this)[pair.first] == pair.second; });
}

template<typename KeyT, typename ValueT>
inline bool HashMap<KeyT, ValueT>::operator!=(const HashMap<KeyT, ValueT>& rhs) const
{
	return !(operator==(rhs));
}

#endif // HASHMAPHPP
