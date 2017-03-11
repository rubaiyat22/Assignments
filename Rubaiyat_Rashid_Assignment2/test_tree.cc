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

#include "AvlTree.h"
#include "BinarySearchTree.h"
#include "SequenceMap.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>

using namespace std;

namespace {

// @db_filename: an input database filename.
// @seq_filename: an input sequences filename.
// @a_tree: an input tree of the type TreeType. It is assumed to be
//  empty.
template <typename TreeType>
void TestTree(const string & db_filename, const string & seq_filename, TreeType & a_tree) 
{
  ifstream database_file(db_filename);
  string input;
  int line_number = 0;
  // constructs a search tree 
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

  double total_nodes = a_tree.CountTreeNodes();
  cout << "(2) Number of nodes in tree: " << total_nodes << endl;
  double average_depth = a_tree.CalculateInternalPathLength()/total_nodes;
  cout << "(3a) Average Depth: " << average_depth << endl;
  cout << "(3b) Ratio of Average depth to log N base 2: " << average_depth/log2(total_nodes) << endl;

  // Tests input from Sequences.txt
  // Searches for Nodes containing recognition sequences
  ifstream sequence_file1(seq_filename);
  string input_sequence;
  double found_count = 0;
  double search_recursion_calls = 0;

  while (getline(sequence_file1, input_sequence))
  {
    const SequenceMap new_map1 (input_sequence, "");
    if (a_tree.contains(new_map1))
    {
      found_count++;
      search_recursion_calls += a_tree.RecursionCalls(); //adds recursion call for every search call 
    }
  } 
  sequence_file1.close(); 

  cout << "(4a) Total number of successful queries: " << found_count << endl;
  cout << "(4b) Average number of recursion calls for search : " << search_recursion_calls / found_count << endl;

  ifstream sequence_file2(seq_filename);
  string input_seq_rem;
  int num_of_sequences = 0; 
  double remove_recursion_calls = 0; 
  double num_of_remove_calls = 0;  //to count the times remove() is called 

  // Removes every other items from sequence .txt
  while (getline(sequence_file2, input_seq_rem))
  {
    num_of_sequences++;
    const SequenceMap new_map2 (input_seq_rem, "");
    if (num_of_sequences % 2 != 0)
    {
        num_of_remove_calls++;
        a_tree.remove(new_map2);
        remove_recursion_calls += a_tree.RecursionCalls();  //adds recursive calls for every remove
    }
  }
  sequence_file2.close(); 

  int removed_items_num = total_nodes - a_tree.CountTreeNodes();
  cout << "(5a) Number of successful removes: " << removed_items_num << endl;
  cout << "(5b) Average number of recursion calls for remove: " << remove_recursion_calls/num_of_remove_calls << endl;

  total_nodes = a_tree.CountTreeNodes();
  average_depth = a_tree.CalculateInternalPathLength()/total_nodes;

  cout << "(6a) Number of Nodes in tree after removal:" << total_nodes << endl;
  cout << "(6b) Average depth of tree after removal: " << average_depth << endl;
  cout << "(6c) Ratio of Average depth to log N base 2: " << average_depth/log2(total_nodes) << endl;

}

}  // namespace

int main(int argc, char **argv) 
{
  if (argc != 4) 
  {
    cout << "Usage: " << argv[0] << " <databasefilename> <queryfilename> <tree-type>" << endl;
    return 0;
  }
  const string db_filename(argv[1]);
  const string seq_filename(argv[2]);
  const string param_tree(argv[3]);
  cout << "Input file is " << db_filename << ", and sequences file is " << seq_filename << endl;
  cout << "Type of tree is " << param_tree << endl;
  if (param_tree == "BST") 
  {
    BinarySearchTree<SequenceMap> a_tree;
    TestTree(db_filename, seq_filename, a_tree);
  } 
  else if (param_tree == "AVL") 
  {
    AvlTree<SequenceMap> a_tree;
    TestTree(db_filename, seq_filename, a_tree);
  } 
  else 
  {
    cout << "Uknown tree type " << param_tree << " (User should provide BST, or AVL)" << endl;
  }
  return 0;
}
