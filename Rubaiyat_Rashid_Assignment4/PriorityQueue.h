#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <iostream>
#include <vector>
#include "Dsexceptions.h"
#include "QuadraticProbing.h"

// Priority queue class
//
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS******************************************
// void Insert( x )       			--> Insert x into queue and hash table
// void NewInsert( x )    			--> Insert x into queue and hash table
// DeleteMin( )           			--> Return and remove smallest item
// Comparable FindMin( )  			--> Return smallest item
// bool IsEmpty( )        			--> Return true if empty; else false
// void MakeEmpty( )      			--> Remove all items
// void Merge( rhs )      			--> Absorb rhs into this heap
// bool Find(x) 					--> returns true if x found, otherwise false
// ******************ERRORS*****************************************************
// Throws UnderflowException as warranted

template <typename Comparable>
class PriorityQueue
{
public:
	PriorityQueue( ) : the_trees_( DEFAULT_TREES )
	{
		for( auto & root : the_trees_ )
			root = nullptr;
		current_size_ = 0;
	}
	
	PriorityQueue( const Comparable & item ) : the_trees_( 1 ), current_size_{ 1 }
		{ the_trees_[ 0 ] = new BinomialNode{ item, nullptr, nullptr, nullptr }; }

	PriorityQueue( const PriorityQueue & rhs ): the_trees_( rhs.the_trees_.size( ) ),current_size_{ rhs.current_size_ }
	{ 
		for( int i = 0; i < rhs.the_trees_.size( ); ++i )
			the_trees_[ i ] = Clone( rhs.the_trees_[ i ] );
	}

	PriorityQueue( PriorityQueue && rhs ): the_trees_{ std::move( rhs.the_trees ) }, current_size_{ rhs.current_size_ }{}

	~PriorityQueue( )
		{ MakeEmpty( ); }

	// Assignment operator ; Deep Copy
	PriorityQueue & operator=( const PriorityQueue & rhs )
	{
		PriorityQueue copy = rhs;
		std::swap( *this, copy );
		return *this;
	}
			

	//move assignment operator
	PriorityQueue & operator=( PriorityQueue && rhs )
	{
		std::swap( current_size_, rhs.current_size_ );
		std::swap( the_trees_, rhs.the_trees_ );
		
		return *this;
	}
	
	//returns true if empty, otherwise false
	bool IsEmpty( ) const
		{ return current_size_ == 0; }

	
	//Returns minimum item.
	//Throws UnderflowException if empty.
	const Comparable & FindMin( ) const
	{
		if( IsEmpty( ) )
			throw UnderflowException{ "FindMin() is called on an empty queue" };

		return the_trees_[ FindMinIndex( ) ]->element_;
	}
	
	//Insert item x into the priority queue; does not allow duplicates
	void Insert( const Comparable & x )
	{ 
		PriorityQueue one_item{ x };
		hash_table_.Insert (x, one_item.the_trees_[0]);
		Merge( one_item );
	}

	//Insert item x into the priority queue; does not allows duplicates.
	void Insert( Comparable && x )
	{ 
		PriorityQueue one_item{ std::move( x ) }; 
		hash_table_.Insert (std::move (x), one_item.the_trees_[0]);
		Merge( one_item ); 
	}
	

	//Remove the smallest item from the priority queue.
	//Throws UnderflowException if empty.
	void DeleteMin( )
	{
		Comparable x;
		DeleteMin( x );
	}

	//Remove the minimum item and place it in min_item.
	//Throws UnderflowException if empty.
	void DeleteMin( Comparable & min_item )
	{
		if( IsEmpty( ) )
			throw UnderflowException{"DeleteMin() is called on an empty queue"};

		int min_index = FindMinIndex( );
		min_item = the_trees_[ min_index ]->element_;

		BinomialNode *old_root = the_trees_[ min_index ];
		BinomialNode *deleted_tree = old_root->left_child_;
		delete old_root;

		// Construct H''
		PriorityQueue deleted_queue;
		deleted_queue.the_trees_.resize( min_index );
		deleted_queue.current_size_ = ( 1 << min_index ) - 1;
		for( int j = min_index - 1; j >= 0; --j )
		{
			deleted_queue.the_trees_[ j ] = deleted_tree;
			deleted_tree = deleted_tree -> next_sibiling_;
			deleted_queue.the_trees_[ j ] -> next_sibiling_ = nullptr;
			deleted_queue.the_trees_[ j ] -> parent_ = nullptr; //added
		}

		// Construct H'
		the_trees_[ min_index ] = nullptr;
		current_size_ -= deleted_queue.current_size_ + 1;

		Merge( deleted_queue );
	}

	//Make the priority queue logically empty.
	void MakeEmpty( )
	{
		current_size_ = 0;
		for( auto & root : the_trees_ )
			MakeEmpty( root );
	}

	//Merge rhs into the priority queue.
	//rhs becomes empty. rhs must be different from this.
	//Exercise 6.35 needed to make this operation more efficient.
	void Merge( PriorityQueue & rhs )
	{
		if( this == &rhs )    // Avoid aliasing problems
			return;

		current_size_ += rhs.current_size_;

		if( current_size_ > Capacity( ) )
		{
			int old_num_trees = the_trees_.size( );
			int new_num_trees = std::max( the_trees_.size( ), rhs.the_trees_.size( ) ) + 1;
			the_trees_.resize( new_num_trees );
			for( size_t i = old_num_trees; i < new_num_trees; ++i )
				the_trees_[ i ] = nullptr;
		}

		BinomialNode *carry = nullptr;
		for( size_t i = 0, j = 1; j <= current_size_; ++i, j *= 2 )
		{
			BinomialNode *t1 = the_trees_[ i ];
			BinomialNode *t2 = i < rhs.the_trees_.size( ) ? rhs.the_trees_[ i ] : nullptr;

			int which_case = t1 == nullptr ? 0 : 1;
			which_case += t2 == nullptr ? 0 : 2;
			which_case += carry == nullptr ? 0 : 4;

			switch( which_case )
			{
				case 0: /* No trees */
				case 1: /* Only this */
					break;
				case 2: /* Only rhs */
					the_trees_[ i ] = t2;
					rhs.the_trees_[ i ] = nullptr;
					break;
				case 4: /* Only carry */
					the_trees_[ i ] = carry;
					carry = nullptr;
					break;
				case 3: /* this and rhs */
					carry = CombineTrees( t1, t2 );
					the_trees_[ i ] = rhs.the_trees_[ i ] = nullptr;
					break;
				case 5: /* this and carry */
					carry = CombineTrees( t1, carry );
					the_trees_[ i ] = nullptr;
					break;
				case 6: /* rhs and carry */
					carry = CombineTrees( t2, carry );
					rhs.the_trees_[ i ] = nullptr;
					break;
				case 7: /* All three */
					the_trees_[ i ] = carry;
					carry = CombineTrees( t1, t2 );
					rhs.the_trees_[ i ] = nullptr;
					break;
			}
		}

		for( auto & root : rhs.the_trees_ )
			root = nullptr;
		rhs.current_size_ = 0;
	}

	//checks if x exists in hashtable
	//returns true if exists, otherwise false
	bool Find(const Comparable & x)
	{
		BinomialNode * node = hash_table_.GetElement(x);
		if (hash_table_.Contains(x) && node -> element_ == x)
			return true;
		return false;	
	}    

	//returns the total number of elements available in the queue
	size_t TotalNumberOfElements() const 
	{
		return current_size_;
	}

	//if x does not exist in the queue/ hashtable, it returns false
	//if x is equal to minimum element, DeleteMin() is called 
	//otherwise, retrieves binary node from hashtable
	//element at binarynode is assigned to the lowest possible number i.e -1 then percolateUp() is called
	//Now DeleteMin() is called and then x is removed from hashtable
	bool Remove (Comparable & x)
	{
		if (!(hash_table_.Contains(x)))
			return false;

		BinomialNode * & a_binomial_node = hash_table_.GetElement(x);

		if (x != FindMin())   
		{
			a_binomial_node -> element_ = -1;
			PercolateUp(a_binomial_node);
			DeleteMin();
		}	
		else  // if x is equal to the Minium element, PercolateUp() is not necessary
			DeleteMin();

		hash_table_.Remove(x);

		return true;
	}

	//This new insert function does not create a new queue, rather just a new Binary node
	//NewMerge() is called with the new node as a parameter
	void NewInsert(Comparable & x)
	{
		BinomialNode * new_node = new BinomialNode {x, nullptr, nullptr, nullptr};
		hash_table_.Insert(x, new_node);
		NewMerge(new_node);
	}

private:
	struct BinomialNode
	{
		Comparable element_;
		BinomialNode *left_child_;
		BinomialNode *next_sibiling_;
		BinomialNode *parent_;

		BinomialNode( const Comparable & e, BinomialNode *lt, BinomialNode *rt, BinomialNode *pt)
			: element_{ e }, left_child_{ lt }, next_sibiling_{ rt }, parent_ {pt} { }
		
		BinomialNode( Comparable && e, BinomialNode *lt, BinomialNode *rt, BinomialNode *pt)
			: element_{ std::move( e ) }, left_child_{ lt }, next_sibiling_{ rt }, parent_ {pt} { }
	};

	const static int DEFAULT_TREES = 1;

	std::vector<BinomialNode *> the_trees_;  // An array of tree roots
	size_t current_size_;                  // Number of items in the priority queue
	HashTable<Comparable, BinomialNode*> hash_table_;  // A hashtable containing all the elements in the queue
	
	
	//Find index of tree containing the smallest item in the priority queue.
	//The priority queue must not be empty.
	//Return the index of tree containing the smallest item.
	size_t FindMinIndex( ) const
	{
		size_t i = 0;
		size_t min_index = 0;

		for( i = 0; the_trees_[ i ] == nullptr; ++i )
			;

		for( min_index = i; i < the_trees_.size( ); ++i )
			if( the_trees_[ i ] != nullptr && the_trees_[ i ]->element_ < the_trees_[ min_index ]->element_ )
				min_index = i;

		return min_index;
	}

	//Return the Capacity.
	size_t Capacity( ) const
		{ return ( 1 << the_trees_.size( ) ) - 1; }

	//Return the result of merging equal-sized t1 and t2.
	BinomialNode * CombineTrees( BinomialNode *t1, BinomialNode *t2 )
	{
		if( t2->element_ < t1->element_ )
			return CombineTrees( t2, t1 );

		t2 -> next_sibiling_ = t1 -> left_child_; 
		t2 -> parent_ = t1;  //t1 becomes the parent of t2
		t1->left_child_ = t2; 
		return t1;
	}

	//Make a binomial tree logically empty, and free memory.
	void MakeEmpty( BinomialNode * & t )
	{
		if( t != nullptr )
		{
			MakeEmpty( t->left_child_);
			MakeEmpty( t->next_sibiling_ );
			delete t;
			t = nullptr;
		}
	}

	//Internal method to Clone subtree.
	BinomialNode * Clone( BinomialNode * t ) const
	{
		if( t == nullptr )
			return nullptr;
		else
			return new BinomialNode{ t->element_, Clone( t->left_child_), Clone( t->next_sibiling_ ) };
	}

	// the element at t is pushed to the root and pointers stored in hash table are changed
	void PercolateUp (BinomialNode* t)
	{
		while (t -> parent_ != nullptr && t != nullptr)
		{
			Comparable temp = t -> element_;
			t -> element_ = t -> parent_ -> element_;
			t -> parent_ -> element_ = temp;

			hash_table_.ChangeElement(t -> element_, t);
			t = t -> parent_;
		}
	}

	//Merges a_element
	//if there is a empty spot, t is stored there
	//otherwise, CombineTrees() is called to combine carry and pointer to node at position i of the_trees_
	void NewMerge(BinomialNode * a_element)
	{
		current_size_ += 1;
		the_trees_.push_back(nullptr);
		BinomialNode *t = a_element;
		for( size_t i = 0; i < current_size_; ++i)
		{
			if (the_trees_[i] == nullptr)
			{
				the_trees_[i] = t;
				break;
			}
			t = CombineTrees(the_trees_[i], t);
			the_trees_[i] = nullptr;
		}
	}
};

#endif