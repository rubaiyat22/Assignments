/**
Name: Rubaiyat Rashid
Class: CSCI 335
Professor: Ioannis Stamos
Purpose: Assignment 3
Description: CreateAndTestHash test the 3 hash tables: Linear probing, Quadratic Probing, and Double Hashing.
For each hash table, items are read from words.txt and inserted into table. Once inserted, Number of collisions,
Number of items, size of hash table, load factor and average number of collisions is outputted. Then, number 
of items are read from query_words.txt to test whether they exist int the table or not. For each successful
or unsuscessful find, number of probes needed is also outputted. 
*/

#include "QuadraticProbing.h"
#include "LinearProbing.h"
#include "DoubleHashing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// @ hash_table - Object of a Hash Table
// @ words_filenam - file which contains 10k+ words
// @ query_filename - file which contains some words to be searched in Hashtable
// Tests several components of HashTable such as Collisions, Average Collisions, Numbe of items and Load Factor
// Also searches for items in HashTable
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) 
{
	cout << "TestFunctionForHashTables..." << endl;
	cout << "Words filename: " << words_filename << endl;
	cout << "Query filename: " << query_filename << endl;
	hash_table.MakeEmpty();  
	
	ifstream input_filename (words_filename);
	string word;

	while (input_filename >> word)
		hash_table.Insert(word);

	input_filename.close();

	double collisions = hash_table.TotalNumOfCollisions ();
	double total_items = hash_table.TotalNumOfElements();
	double table_size = hash_table.TableSize();

	cout << "Collisions: " << collisions << endl;
	cout << "Number of items: " << total_items << endl;
	cout << "Size of hash table: " << table_size << endl;
	cout << "Load factor: " << total_items/table_size << endl;
	cout << "Avg. number of collisions: " << collisions/total_items << endl;

	ifstream query_file (query_filename);
	string query_word;

	while (query_file >> query_word)
	{
		if (hash_table.Contains(query_word))
			cout << query_word << "- Found! -->> " << "Number of probes: " << hash_table.NumOfProbes() << endl;
		else
			cout << query_word << "- Not Found! -->> " << "Number of probes " << hash_table.NumOfProbes() << endl;
	}
	query_file.close();
}

// Sample main for program CreateAndTestHash
int main(int argc, char **argv) 
{
	if (argc != 4) 
	{
		cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
		return 0;
	}
	  
	const string words_filename(argv[1]);
	const string query_filename(argv[2]);
	const string param_flag(argv[3]);

	if (param_flag == "linear") 
	{
		HashTableLinear <string> linear_probing_table;
		TestFunctionForHashTable(linear_probing_table, words_filename, query_filename);    
	} 
	else if (param_flag == "quadratic") 
	{
		HashTable<string> quadratic_probing_table;
		TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename);    
	} 
	else if (param_flag == "double") 
	{
		HashTableDouble<string> double_probing_table;
		TestFunctionForHashTable(double_probing_table, words_filename, query_filename);    
	} 
	else 
		cout << "Unknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;

	return 0;
}
