#ifndef DOUBLE_HASHING_H
#define DOUBLE_HASHING_H

// Class HashTableDouble - Double Hashing
// This class is Inherited from class Hashtable
// Please refer to QuadraticProbing.h for methods which HashTableDouble will be inherited from
// Overriden Function: FindPos(x)
// Added Function: Hash2(X)

// Double hashing implementation.
template <typename HashedObj>
class HashTableDouble : public HashTable <HashedObj>
{
protected:
	// Computes the i'th probe by the linear probing function f(i) = i*HASH2(X)
	size_t FindPos(const HashedObj & x) const 
	{
		size_t offset = Hash2(x);  //offset is Hash2(X)
		size_t current_pos = this -> InternalHash(x);

		this -> number_of_probes_  = 1;
		while (this -> array_[current_pos].info_ != this -> EMPTY && this -> array_[current_pos].element_ != x) 
		{
			this -> total_collisions_++;
			current_pos += offset;  // Compute ith probe.
			if (current_pos >= this -> array_.size())
				current_pos -= this -> array_.size();
			this -> number_of_probes_++;
		}
		return current_pos;
	}

	// r = 73
	// Computes Hash2(x) = R - x mod R  and returns it's value
	size_t Hash2(const HashedObj & x) const
	{
		static std::hash<HashedObj> hf;
		return 73 - hf(x) % 73;
	}
};
#endif