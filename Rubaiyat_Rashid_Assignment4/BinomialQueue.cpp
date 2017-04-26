#ifndef BINOMIAL_QUEUE_H
#define BINOMIAL_QUEUE_H

#include <iostream>
#include <vector>
#include "dsexceptions.h"
//#include "QuadraticProbing.h"
using namespace std;

// Binomial queue class
//
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void Insert( x )       --> Insert x
// DeleteMin( )           --> Return and remove smallest item
// Comparable findMin( )  --> Return smallest item
// bool IsEmpty( )        --> Return true if empty; else false
// void MakeEmpty( )      --> Remove all items
// void Merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinomialQueue
{
	public:
		BinomialQueue( ) : the_trees_( DEFAULT_TREES )
		{
			for( auto & root : the_trees_ )
				root = nullptr;
			current_size_ = 0;
		}
		
		BinomialQueue( const Comparable & item ) : the_trees_( 1 ), current_size_{ 1 }
			{ the_trees_[ 0 ] = new BinomialNode{ item, nullptr, nullptr }; }

		BinomialQueue( const BinomialQueue & rhs ): the_trees( rhs.the_trees_.size( ) ),current_size_{ rhs.current_size_ }
		{ 
			for( int i = 0; i < rhs.the_trees_.size( ); ++i )
					the_trees_[ i ] = Clone( rhs.the_trees_[ i ] );
		}

		BinomialQueue( BinomialQueue && rhs )
			: the_trees{ std::move( rhs.the_trees ) }, current_size_{ rhs.current_size_ }
		{ 
		}

		~BinomialQueue( )
			{ MakeEmpty( ); }

		
		/**
		 * Deep copy.
		 */
		BinomialQueue & operator=( const BinomialQueue & rhs )
		{
			BinomialQueue copy = rhs;
			std::swap( *this, copy );
			return *this;
		}
				
		/**
		 * Move.
		 */
		BinomialQueue & operator=( BinomialQueue && rhs )
		{
			std::swap( current_size_, rhs.current_size_ );
			std::swap( the_trees_, rhs.the_trees_ );
			
			return *this;
		}
		
		/**
		 * Return true if empty; false otherwise.
		 */
		bool IsEmpty( ) const
			{ return current_size_ == 0; }

		/**
		 * Returns minimum item.
		 * Throws UnderflowException if empty.
		 */
		const Comparable & findMin( ) const
		{
			if( IsEmpty( ) )
					throw UnderflowException{ };

			return the_trees_[ FindMinIndex( ) ]->element;
		}
		
		/**
		 * Insert item x into the priority queue; allows duplicates.
		 */
		void Insert( const Comparable & x )
			{ BinomialQueue oneItem{ x }; Merge( oneItem ); }

		/**
		 * Insert item x into the priority queue; allows duplicates.
		 */
		void Insert( Comparable && x )
			{ BinomialQueue oneItem{ std::move( x ) }; Merge( oneItem ); }
		
		/**
		 * Remove the smallest item from the priority queue.
		 * Throws UnderflowException if empty.
		 */
		void DeleteMin( )
		{
			Comparable x;
			DeleteMin( x );
		}

		/**
		 * Remove the minimum item and place it in minItem.
		 * Throws UnderflowException if empty.
		 */
		void DeleteMin( Comparable & minItem )
		{
			if( IsEmpty( ) )
					throw UnderflowException{ };

			int minIndex = FindMinIndex( );
			minItem = the_trees_[ minIndex ]->element;

			BinomialNode *oldRoot = the_trees_[ minIndex ];
			BinomialNode *deletedTree = oldRoot->left_child_;
			delete oldRoot;

			// Construct H''
			BinomialQueue deletedQueue;
			deletedQueue.the_trees_.resize( minIndex );
			deletedQueue.current_size_ = ( 1 << minIndex ) - 1;
			for( int j = minIndex - 1; j >= 0; --j )
			{
					deletedQueue.the_trees_[ j ] = deletedTree;
					deletedTree = deletedTree->next_sibiling_;
					deletedQueue.the_trees_[ j ]->next_sibiling_ = nullptr;
			}

			// Construct H'
			the_trees_[ minIndex ] = nullptr;
			current_size_ -= deletedQueue.current_size_ + 1;

			Merge( deletedQueue );
		}

		/**
		 * Make the priority queue logically empty.
		 */
		void MakeEmpty( )
		{
			current_size_ = 0;
			for( auto & root : the_trees_ )
				MakeEmpty( root );
		}

		/**
		 * Merge rhs into the priority queue.
		 * rhs becomes empty. rhs must be different from this.
		 * Exercise 6.35 needed to make this operation more efficient.
		 */
		void Merge( BinomialQueue & rhs )
		{
			if( this == &rhs )    // Avoid aliasing problems
				return;

			current_size_ += rhs.current_size_;

			if( current_size_ > Capacity( ) )
			{
				int oldNumTrees = the_trees_.size( );
				int newNumTrees = max( the_trees_.size( ), rhs.the_trees_.size( ) ) + 1;
				the_trees_.resize( newNumTrees );
				for( int i = oldNumTrees; i < newNumTrees; ++i )
						the_trees_[ i ] = nullptr;
			}

			BinomialNode *carry = nullptr;
			for( int i = 0, j = 1; j <= current_size_; ++i, j *= 2 )
			{
				BinomialNode *t1 = the_trees_[ i ];
				BinomialNode *t2 = i < rhs.the_trees_.size( ) ? rhs.the_trees_[ i ] : nullptr;

				int whichCase = t1 == nullptr ? 0 : 1;
				whichCase += t2 == nullptr ? 0 : 2;
				whichCase += carry == nullptr ? 0 : 4;

				switch( whichCase )
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
						carry = combineTrees( t1, t2 );
						the_trees_[ i ] = rhs.the_trees_[ i ] = nullptr;
						break;
					case 5: /* this and carry */
						carry = combineTrees( t1, carry );
						the_trees_[ i ] = nullptr;
						break;
					case 6: /* rhs and carry */
						carry = combineTrees( t2, carry );
						rhs.the_trees_[ i ] = nullptr;
						break;
					case 7: /* All three */
						the_trees_[ i ] = carry;
						carry = combineTrees( t1, t2 );
						rhs.the_trees_[ i ] = nullptr;
						break;
				}
			}

			for( auto & root : rhs.the_trees_ )
				root = nullptr;
			rhs.current_size_ = 0;
		}    


	private:
		struct BinomialNode
		{
			Comparable    element;
			BinomialNode *left_child_;
			BinomialNode *next_sibiling_;
			//BinomialNode *parent;

			BinomialNode( const Comparable & e, BinomialNode *lt, BinomialNode *rt )
				: element{ e }, left_child_{ lt }, next_sibiling_{ rt } { }
			
			BinomialNode( Comparable && e, BinomialNode *lt, BinomialNode *rt )
				: element{ std::move( e ) }, left_child_{ lt }, next_sibiling_{ rt } { }
		};

		const static int DEFAULT_TREES = 1;

		vector<BinomialNode *> the_trees_;  // An array of tree roots
		int current_size_;                  // Number of items in the priority queue
		HashTable<pair<int, BinomialNode*>> hash_table_;
		
		/**
		 * Find index of tree containing the smallest item in the priority queue.
		 * The priority queue must not be empty.
		 * Return the index of tree containing the smallest item.
		 */
		int FindMinIndex( ) const
		{
			int i;
			int minIndex;

			for( i = 0; the_trees_[ i ] == nullptr; ++i )
					;

			for( minIndex = i; i < the_trees_.size( ); ++i )
					if( the_trees_[ i ] != nullptr && the_trees_[ i ]->element < the_trees_[ minIndex ]->element )
							minIndex = i;

			return minIndex;
		}

		/**
		 * Return the Capacity.
		 */
		int Capacity( ) const
			{ return ( 1 << the_trees_.size( ) ) - 1; }

		/**
		 * Return the result of merging equal-sized t1 and t2.
		 */
		BinomialNode * combineTrees( BinomialNode *t1, BinomialNode *t2 )
		{
			if( t2->element < t1->element )
					return combineTrees( t2, t1 );
			t2->next_sibiling_ = t1->left_child_;
			t1->left_child_ = t2;
			return t1;
		}

		/**
		 * Make a binomial tree logically empty, and free memory.
		 */
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

		/**
		 * Internal method to Clone subtree.
		 */
		BinomialNode * Clone( BinomialNode * t ) const
		{
			if( t == nullptr )
				return nullptr;
			else
				return new BinomialNode{ t->element, Clone( t->left_child_), Clone( t->next_sibiling_ ) };
		}
};

#endif