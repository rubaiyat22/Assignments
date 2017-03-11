/**
Name: Rubaiyat Rashid
Class: CSCI 335
Professor: Ioannis Stamos
Purpose: Assignment 2
Description: query_tree.cc is used to load objects of SequenceMap into either BST or AVL.
Recognition sequences are from stdin and tested to see if they exist in the tree.

*/

#include "AvlTree.h"
#include "BinarySearchTree.h"
#include "SequenceMap.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

namespace {

// @db_filename: an input filename.
// @a_tree: an input tree of the type TreeType. It is assumed to be
//  empty.
template <typename TreeType>
void QueryTree(const string &db_filename, TreeType &a_tree) 
{
  ifstream database_file(db_filename);
  string input;
  int line_number = 0;
  while (getline(database_file, input))
  {
    line_number++;
    if (line_number > 10)
    {
      input = input.substr(0,input.size()-2);  //getting rid of extra '/' at the end of each line
      stringstream str_stream(input);
      string an_enz_acro;
      getline(str_stream, an_enz_acro, '/');
      string a_reco_seq;
      while (getline(str_stream, a_reco_seq, '/'))
      {
        const SequenceMap new_sequence_map (a_reco_seq, an_enz_acro);
        a_tree.insert(new_sequence_map);
      } 
    }   
  }
  database_file.close();

  // Testing Input Part a //
  string std_input;
  while (getline(cin,std_input))
  {
    const SequenceMap new_map (std_input, "");

    if (a_tree.contains(new_map))
      cout << a_tree.Find(new_map);
    else
      cout << "Not Found " << endl;

  }
}
}  // namespace

int main(int argc, char **argv) 
{
  if (argc != 3) 
  {
    cout << "Usage: " << argv[0] << " <databasefilename> <tree-type>" << endl;
    return 0;
  }
  const string db_filename(argv[1]);
  const string param_tree(argv[2]);
  cout << "Input filename is " << db_filename << endl;
  cout << "Type of tree is " << param_tree << endl;
  if (param_tree == "BST") 
  {
    BinarySearchTree<SequenceMap> a_tree;
    QueryTree(db_filename, a_tree);
  } 
  else if (param_tree == "AVL") 
  {
    AvlTree<SequenceMap> a_tree;
    QueryTree(db_filename, a_tree);
  } 
  else 
  {
    cout << "Uknown tree type " << param_tree << " (User should provide BST, or AVL)" << endl;
  }

  return 0;
}
