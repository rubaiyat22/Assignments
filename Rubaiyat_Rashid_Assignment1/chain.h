/***************************************************************************************************************/
//Name: Rubaiyat Rashid
//Professor: Ioannis Stamos
//Class: CSCI 335 
//Purpose: Assignment #1
//Description: The class Chain will store a series of items in the given order of any size. This is a templated
//             implementation of Chain class using a Dynamic Array as the underlying data structure.
/*************************************************************************************************************/

#ifndef CSCI335_HOMEWORK1_CHAIN_
#define CSCI335_HOMEWORK1_CHAIN_

#include <iostream>
#include <cstddef>
#include <sstream>
#include <string>

namespace teaching_project {
// Templated Implementation of Chain Class using Dynamic Array
// Sample usage:
//    Chain <String> a_chain {"Hello world"}
//    cout << a_chain.size() << endl;
//    cout << a_chain << endl;
//    Chain <int> x, y;
//    x.ReadChain();
//    y.ReadChain();
//    cout << x + y << endl;
//    a_chain[0] = "New String";
template<typename Object>
class Chain 
{
public:
  // Default "big five" -- you have to alter them for your assignment.
  // That means that you will remove the "= default" statement.
  //  and you will provide an implementation.

  // Zero-parameter constructor.
  Chain(): size_ {0}, array_ {nullptr}
  {

  }

  // Copy-constructor.
  Chain(const Chain &rhs)
  {
    size_ = rhs.size_;
    array_ = new Object [size_];
    for (size_t i = 0; i < size_; i++)
      array_[i] = rhs.array_[i];
  }

  // Copy-assignment. If you have already written
  // the copy-constructor and the move-constructor
  // you can just use:
  // {
  // Chain copy = rhs; 
  // std::swap(*this, copy);
  // return *this;
  // }
  Chain& operator=(const Chain &rhs)
  {
    if (this != &rhs)
    {
      Chain copy = rhs;
      std::swap (*this, copy);
    }
    return *this;
  }

  // Move-constructor. 
  Chain(Chain &&rhs)
  {
    size_ = rhs.size_;
    array_ = rhs.array_;
    rhs.size_ = 0;
    rhs.array_ = nullptr;
  }

  // Move-assignment.
  // Just use std::swap() for all variables.
  Chain& operator=(Chain &&rhs)
  {
    std::swap (size_, rhs.size_);
    std::swap (array_, rhs.array_);
    return *this;
  }

  ~Chain() 
  {
    delete [] array_;
    array_ = nullptr;   
  }

  // End of big-five.

  // One parameter constructor.
  Chain(const Object& item) 
  {
    size_ = 1;
    array_ = new Object [size_];
    array_[size_-1] = item;
  }

  // Read a chain from standard input.
  void ReadChain() 
  {
    std::string line;
    //takes a string standard input
    std::getline (std::cin, line);       

    //gets rid of the '[' and ']' from the string
    line = line.substr(1,line.size()-2);
    std::stringstream str_stream(line);  

    std::string chain_size;
    std::getline(str_stream, chain_size,':');  //reads up to the column to determine the size of the chain
    size_t c_size;
    std::stringstream size_converter(chain_size);  //converting the first char to size
    size_converter >> c_size;
    size_ = c_size;

    array_ = new Object [size_];  

    for (size_t i = 0; i < size_; i++)
      str_stream >> array_[i];     //reading the content of string to array
  }

  // @param No Parameters
  // @return the Size of the chain
  size_t size() const 
  { 
    return size_;
  }

  // @location: an index to a location in the chain.
  // @returns the Object at @location.
  // const version.
  // abort() if out-of-range.
  const Object& operator[](size_t location) const 
  { 
    if (location < 0 && location > size_-1)
      abort();
    return array_[location];
  }

   // @location: an index to a location in the range.
   // @returns the Object at @location.
   // non-const version.
   // abort() if out-of-range.
   Object& operator[](size_t location) 
   { 
    if (location < 0 && location > size_-1)
      abort();

    return array_[location];
   }

   //  @c1: A chain.
   //  @c2: A second chain.
   //  @return the concatenation of the two chains.
  friend Chain operator+(const Chain &c1, const Chain &c2) 
  {
    Chain temp_object;  //a new object of class Chain made to store the values from both objects

    temp_object.size_ = c1.size() + c2.size();

    temp_object.array_ = new Object[temp_object.size()];   

    //in each iteration, the values from the either objects is stored to temp_object
    for (size_t i = 0; i < temp_object.size(); i++)
    {
      if (i < c1.size())
        temp_object.array_[i] = c1.array_[i]; //elements from c1 is added to the temp_object.array_
      else
        temp_object.array_[i] = c2.array_[i-c2.size()+1];  //elements from c2 is added to the array_
    }
    return temp_object;
  }

  //  @a_chain: A chain.
  //  @obj: New element.
  //  @return A chain with a new element added to the end.
  friend Chain operator+(const Chain &a_chain, const Object & obj)
  {
    Chain temp_object; 
    temp_object.size_ = a_chain.size_ + 1;   //incrementing the size of array by 1
    temp_object.array_ = new Object [temp_object.size_];

    for (size_t i = 0; i < a_chain.size_; i++)
      temp_object.array_[i] = a_chain.array_[i]; //elements from a_chain is added to temp_object.array

    temp_object.array_[temp_object.size_-1] = obj;   //adds the given element to the end of temp_object.array

    return temp_object;
  }
  
   // Overloading the << operator.
  friend std::ostream &operator<<(std::ostream &out, const Chain &a_chain) 
  {
    out << "[";

    for (size_t i = 0; i < a_chain.size();i++)
    {
      if (i != a_chain.size()-1)
        out << a_chain.array_[i] << " "; 
      else
        out << a_chain.array_[i];
    }
    out << "]" << std::endl;

    return out;
  }
  
private:  
  size_t size_;     // number of elements that will be in the chain
  Object *array_;    //dynamic aray to store each given element in the chain
};

}  // namespace teaching_project
#endif // CSCI_335_HOMEWORK1_CHAIN_
