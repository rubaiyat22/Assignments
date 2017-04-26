#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

// Class HashTableLinear - Linear Probing
// This class is Inherited from class Hashtable
// Please refer to QuadraticProbing.h for methods from parent class which HashTableLinear will be inherited from 
// Overriden Function: FindPos(x)

// Linear probing implementation.
template <typename HashedObj>
class HashTableLinear : public HashTable <HashedObj>
{
protected:
	// Computes the i'th probe by the linear probing function f(i) = i
	size_t FindPos(const HashedObj & x) const 
	{
		size_t offset = 1;
		size_t current_pos = this->InternalHash(x);

		this -> number_of_probes_ ++;

		while (this -> array_[current_pos].info_ != this -> EMPTY && this -> array_[current_pos].element_ != x) 
		{
			this -> number_of_probes_++;
			this -> total_collisions_++;
			current_pos += offset; //Compute ith probe.
			if (current_pos >= this -> array_.size())
				current_pos -= this -> array_.size();
		}
		return current_pos;
	}
};
#endif