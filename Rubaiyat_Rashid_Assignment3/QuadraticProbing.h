#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

namespace 
{
// Internal method to test if a positive number is prime.
bool IsPrime(size_t n) 
{
	if( n == 2 || n == 3 )
		return true;

	if( n == 1 || n % 2 == 0 )
		return false;
	
	for(unsigned int i = 3; i * i <= n; i += 2 )
		if( n % i == 0 )
			return false;
	
	return true;
}

// Internal method to return a prime number at least as large as n.
int NextPrime(size_t n) 
{
	if (n % 2 == 0)
		++n;  
	while (!IsPrime(n)) n += 2;  
		return n;
}

}  // namespace

// HashTable class- Quadratic Probing
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS****************************************
// void insert( x )       --> Insert x into HashTable
// void remove( x )       --> Remove x from Hashtable
// bool contains( x )     --> Return true if x is present
// MakeEmpty()            --> Clears up Hashtable
// TotalNumOfElements()   --> Returns number of elements present in Hashtable
// TableSize()            --> Returns current Table Size
// TotalNumOfCollisions() --> Returns total number of collisions
// NumOfProbes()          --> Returns number of probes for each search
// **************************************************************************

// Sample Uses:
//    <> HashTable <string> new_hash_table;
//	  <> new_hash_table. Insert(new word);
//    <> new_hash_table.Contains(new word);
//	  <> cout << new_hash_table.TotalNumOfElements() << endl;

// Quadratic probing implementation.
template <typename HashedObj>
class HashTable 
{
public:
	enum EntryType {ACTIVE, EMPTY, DELETED};

	explicit HashTable(size_t size = 101) : array_(NextPrime(size))
	{ 
		MakeEmpty();
		total_collisions_ = 0;
		number_of_probes_ = 0; 
	}
	
	bool Contains(const HashedObj & x) const 
	{
		number_of_probes_ = 0;
		return IsActive(FindPos(x));
	}
	
	//Sets every item in HashTable to EMPTY
	void MakeEmpty() 
	{
		current_size_ = 0;
		for (auto &entry : array_)
			entry.info_ = EMPTY;
	}

	//If no empty position found, return false
	//else, sets element_ to x and info_ to active
	//if table is half full, rehash is called
	bool Insert(const HashedObj & x) 
	{
		// Insert x as active
		size_t current_pos = FindPos(x);
		if (IsActive(current_pos))
			return false;
		
		array_[current_pos].element_ = x;
		array_[current_pos].info_ = ACTIVE;
		
		// Rehash; see Section 5.5
		if (++current_size_ > array_.size() / 2)
			Rehash();    
		return true;
	}
	
	//If no empty position found, return false
	//else, sets element_ to x and info_ to active
	//if table is half full, rehash is called
	bool Insert(HashedObj && x) 
	{
		// Insert x as active
		size_t current_pos = FindPos(x);
		if (IsActive(current_pos))
			return false;
		
		array_[current_pos] = std::move(x);
		array_[current_pos].info_ = ACTIVE;

		// Rehash; see Section 5.5
		if (++current_size_ > array_.size() / 2)
			Rehash();

		return true;
	}

	//if a position containing x is empty, returns false
	//else, sets info_ of x to DELETED and returns true
	bool Remove(const HashedObj & x) 
	{
		size_t current_pos = FindPos(x);
		if (!IsActive(current_pos))
			return false;

		array_[current_pos].info_ = DELETED;
		return true;
	}

	size_t TotalNumOfElements() const
	{
		return current_size_;
	}

	int TableSize() const
	{
		return array_.size();
	}

	size_t TotalNumOfCollisions () const
	{
		return total_collisions_;
	}

	size_t NumOfProbes() const
	{
		return number_of_probes_;
	}

protected:        
	struct HashEntry 
	{
		HashedObj element_;
		EntryType info_;
		
		HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
		:element_{e}, info_{i} { }
		
		HashEntry(HashedObj && e, EntryType i = EMPTY)
		:element_{std::move(e)}, info_{ i } {}
	};
		
	std::vector<HashEntry> array_;  // array which will store items 
	size_t current_size_;   // current number of items in table
	mutable size_t total_collisions_;  // variable which keeps track of collisions for search, insert, delete and rehash
	mutable size_t number_of_probes_; // variable which is used to keep count of probes for each search

	//returns true if a position is occuped; false otherwise
	bool IsActive(size_t current_pos) const
	{ 
		return array_[current_pos].info_ == ACTIVE; 
	}

	//Finds a position where x can be inserted
	//counts number of probes and collisions 
	virtual size_t FindPos(const HashedObj & x) const 
	{
		size_t offset = 1;
		size_t current_pos = InternalHash(x); //Hash1

		number_of_probes_ ++;

		while (array_[current_pos].info_ != EMPTY && array_[current_pos].element_ != x) 
		{
			number_of_probes_++;
			total_collisions_++;
			current_pos += offset;  // Compute ith probe.
			offset += 2;
			if (current_pos >= array_.size())
				current_pos -= array_.size();
		}
		return current_pos;
	}

	//rehashes item of array_ by doubling it's size 
	void Rehash() 
	{
		std::vector<HashEntry> old_array = array_;

		// Create new double-sized, empty table.
		array_.resize(NextPrime(2 * old_array.size()));
		for (auto & entry : array_)
			entry.info_ = EMPTY;
		
		// Copy table over.
		current_size_ = 0;
		for (auto & entry :old_array)
		{
			if (entry.info_ == ACTIVE)
	 			Insert(std::move(entry.element_));
		}
	}
	
	//calculates Hash(x) = x mod TableSize
	virtual size_t InternalHash(const HashedObj & x) const 
	{
		static std::hash<HashedObj> hf;
		return hf(x) % array_.size();
	}

};
#endif  // QUADRATIC_PROBING_H