/**
Name: Rubaiyat Rashid
Class: CSCI 335
Professor: Ioannis Stamos
Purpose: Assignment 2
Description: test_tree.cc is a test file that takes input from rebase210.txt to load objects of
SequenceMap into either BST or AVL. Sequences of tests are done with inputs from sequences.txt
like searching for items from the file and removing every other items from the file. Number of nodes,
average depth are calculated before and after the removal. 
*/

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "dsexceptions.h"
#include "SequenceMap.h"
#include <algorithm>
using namespace std;       

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

/**
  Functions Added:
    <> Find(const Comparable & x)
      <> Find(const Comparable & x, AvlNode* t)
    <> CountTreeNodes()
      <> CountTreeNodes(AvlNode *t)
    <> CalculateInternalPathLength()
      <> CalculateInternalPathLength(AvlNode *t)
  Member Variable Added:
    <> recursion_calls_
      <> Function: RecursionCalls()
*/

template <typename Comparable>
class BinarySearchTree
{
  public:
    BinarySearchTree( ) : root{ nullptr }, recursion_calls_ {0}
    { }

    /**
     * Copy constructor
     */
    BinarySearchTree( const BinarySearchTree & rhs ) : root{ nullptr }
    {
      root = clone( rhs.root );
    }

    /**
     * Move constructor
     */
    BinarySearchTree( BinarySearchTree && rhs ) : root{ rhs.root }
    {
      rhs.root = nullptr;
    }
    
    /**
     * Destructor for the tree
     */
    ~BinarySearchTree( )
    {
      makeEmpty( );
    }

    /**
     * Copy assignment
     */
    BinarySearchTree & operator=( const BinarySearchTree & rhs )
    {
      BinarySearchTree copy = rhs;
      std::swap( *this, copy );
      return *this;
    }
        
    /**
     * Move assignment
     */
    BinarySearchTree & operator=( BinarySearchTree && rhs )
    {
      std::swap( root, rhs.root );       
      return *this;
    }

    /**
     * Find the smallest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMin( ) const
    {
      if( isEmpty( ) )
        throw UnderflowException{ };
      return findMin( root )->element;
    }

    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMax( ) const
    {
      if( isEmpty( ) )
        throw UnderflowException{ };
      return findMax( root )->element;
    }

    /**
     * Returns true if x is found in the tree.
     */
    bool contains( const Comparable & x )
    {
      recursion_calls_ = 0;
      return contains( x, root );
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty( ) const
    {
      return root == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree( ostream & out = cout ) const
    {
      if( isEmpty( ) )
        out << "Empty tree" << endl;
      else
        printTree( root, out );
    }

    /**
     * Make the tree logically empty.
     */
    void makeEmpty( )
    {
      makeEmpty( root );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert(const Comparable & x )
    {
      insert( x, root );
    }
     
    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( Comparable && x )
    {
      insert( std::move( x ), root );
    }
    
    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove( const Comparable & x )
    {
      recursion_calls_ = 0;
      remove( x, root );
    }
    /**
     * returns node element once found 
     */
    const Comparable & Find(const Comparable & x)
    {
      return Find(x, root)-> element;
    } 

    /**
     * returns number of nodes
     */
    int CountTreeNodes()
    {
      return CountTreeNodes(root);
    }

    /**
     * returns internal path length of the tree
     */
    int CalculateInternalPathLength()
    {
      return CalculateInternalPathLength(root);
    }

    /**
     * returns recursion calls to specific function
     */
    size_t RecursionCalls() const
    {
      return recursion_calls_ - 1;  
    }

  private:
    struct BinaryNode
    {
      Comparable element;
      BinaryNode *left;
      BinaryNode *right;

      BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt )
        : element{ theElement }, left{ lt }, right{ rt } { }
        
      BinaryNode( Comparable && theElement, BinaryNode *lt, BinaryNode *rt )
        : element{ std::move( theElement ) }, left{ lt }, right{ rt } { }
    };

    BinaryNode *root;
    size_t recursion_calls_;  //variable to keep track of recursion calls made to functions 

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     * If duplicate, x is not inserted into tree as a new node; it is merged with another node
     */
    void insert(const Comparable & x, BinaryNode * & t )
    {
      if( t == nullptr )
        t = new BinaryNode{ x, nullptr, nullptr };
      else if( x < t->element )
        insert( x, t->left );
      else if( t->element < x )
        insert( x, t->right );
      else
        t->element.Merge(x);
    }
    
    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     * If duplicate, x is not inserted into tree as a new node; it is merged with another node
     */
    void insert( Comparable && x, BinaryNode * & t )
    {
      if( t == nullptr )
        t = new BinaryNode{ std::move( x ), nullptr, nullptr };
      else if( x < t->element )
        insert( std::move( x ), t->left );
      else if( t->element < x )
        insert( std::move( x ), t->right );
      else
        t->element.Merge(x);
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     * Counts the recursion calls made to remove
     */
    void remove( const Comparable & x, BinaryNode * & t )
    {
      recursion_calls_++;

      if( t == nullptr )
        return;   // Item not found; do nothing
      if( x < t->element )
        remove( x, t->left );
      else if( t->element < x )
        remove( x, t->right );
      else if( t->left != nullptr && t->right != nullptr ) // Two children
      {
        t->element = findMin( t->right )->element;
        remove( t->element, t->right );
      }
      else
      {
        BinaryNode *oldNode = t;
        t = ( t->left != nullptr ) ? t->left : t->right;
        delete oldNode;
      }
    }

    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    BinaryNode * findMin( BinaryNode *t ) const
    {
      if( t == nullptr )
        return nullptr;
      if( t->left == nullptr )
        return t;
      return findMin( t->left );
    }

    /**
     * Internal method to find the largest item in a subtree t.
     * Return node containing the largest item.
     */
    BinaryNode * findMax( BinaryNode *t ) const
    {
      if( t != nullptr )
        while( t->right != nullptr )
          t = t->right;
      return t;
    }


    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the subtree.
     * counts the reucrsion calls made to contains
     */
    bool contains( const Comparable & x, BinaryNode *t )
    {
      recursion_calls_++;
      if( t == nullptr )
        return false;
      else if( x < t->element )
        return contains( x, t->left );
      else if( t->element < x )
        return contains( x, t->right );
      else
        return true;    // Match
    }
/****** NONRECURSIVE VERSION*************************
    bool contains( const Comparable & x, BinaryNode *t ) const
    {
        while( t != nullptr )
            if( x < t->element )
                t = t->left;
            else if( t->element < x )
                t = t->right;
            else
                return true;    // Match

        return false;   // No match
    }
*****************************************************/

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty( BinaryNode * & t )
    {
      if( t != nullptr )
      {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
      }
      t = nullptr;
    }

    /**
     * Internal method to print a subtree rooted at t in sorted order.
     */
    void printTree( BinaryNode *t, ostream & out ) const
    {
      if( t != nullptr )
      {
        printTree( t->left, out );
        out << t->element << endl;
        printTree( t->right, out );
      }
    }

    /**
     * Internal method to clone subtree.
     */
    BinaryNode * clone( BinaryNode *t ) const
    {
      if( t == nullptr )
        return nullptr;
      else
        return new BinaryNode{ t->element, clone( t->left ), clone( t->right ) };
    }

    //------------------------- Functions Added ------------------------------------//

    /**
     * @t - current node
     * calculates total number of nodes in a tree
     * returns a int number corresponding to the number of nodes in tree
     */
    int CountTreeNodes(BinaryNode *t) const
    {
      if (t==nullptr)
        return 0;
      else
        return 1 + CountTreeNodes(t->left) + CountTreeNodes(t->right);
    }

    /**
     * @t - current node
     * calculates internal path lenth of tree
     * returns a int number corresponding to the total internal path length
     */
    int CalculateInternalPathLength(BinaryNode *t) const
    {
      if (t==nullptr)
        return 0;
      return CalculateInternalPathLength(t->left)+ CalculateInternalPathLength(t->right) + CountTreeNodes (t)-1;
    }

    /**
     * @t - current node 
     * @x - Object of Sequence map
     * returns pointer to node containing x
     */
    BinaryNode* Find(const Comparable & x, BinaryNode* t) const
    {
      if (t == nullptr)
        return nullptr;
      else if (x < t->element)
        return Find (x, t->left);
      else if (t->element < x)
        return Find (x, t->right);
      else
        return t;
    }
};

#endif
