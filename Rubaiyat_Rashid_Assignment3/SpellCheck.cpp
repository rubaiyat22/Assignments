/**
Name: Rubaiyat Rashid
Class: CSCI 335
Professor: Ioannis Stamos
Purpose: Assignment 3
Description: SpellCheck uses Quadratic Probing to spell check words from document1_short.txt/ document1.txt.
A object of class HashTableQuadratic created and 100k+ words from wordsEn.txt are inserted into the table.
Then, words from document1_short.txt/ document1.txt which are not found in table spell checked by one of 
3 methods: swapping adjacent characters, removing characters from every possible position, or inserting
alphabets a to z to every possible position of the word. The spellchecking for each misspelled word stops 
when one correction is found.
*/
#include "QuadraticProbing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

// @ hash_table - Object of class HashTable
// @ dictionary_filename- file which contains 100K+ words
// reads from file and adds items to hashtable
template <typename HashTableType>
void LoadDictionary(HashTableType & hash_table, const string &dictionary_filename)
{
	ifstream dictionary (dictionary_filename);
	string word;

	while (dictionary >> word)
		hash_table.Insert(word);

	dictionary.close();
}

// @ hash_table - Object of class Hashtable
// @ s - word which will be spellchecked by swapping
// spellchecks word by swapping adjacent chars
// @return - if a word is found in hashtable, stops swapping and returns true
// @return - else spellcheck by swapping fails and returns false
template <typename HashTableType>
bool SpellCheckBySwappingChars(const HashTableType & hash_table, const string &s)
{
	string word = s;
	for (unsigned int i=0; i < s.size()-1; i++)
	{
		swap (word[i], word[i+1]);
		if (hash_table.Contains(word))
		{
			cout << s << " -> " << word << " (Case C) "<< endl;
			return true;
		}
		swap (word[i+1], word[i]);
	}
	return false;
}

// @ hash_table - Object of class Hashtable
// @ s - word which will be spellchecked removing chars
// spellchecks word by removing every possible char in word
// @return - if a word is found in hashtable, stops removing chars and returns true
// @return - else spellcheck by removing chars fails and returns false
template <typename HashTableType>
bool SpellCheckByRemovingChars(const HashTableType & hash_table,const string &s)
{
	string word = s;
	for (unsigned int i=0; i < s.size(); i++)
	{
		word= word.erase(i,1);
		if (hash_table.Contains(word))
		{
			cout << s << " -> " << word << " (Case B) " << endl;
			return true;
		}
		word = s;
	}
	return false;
} 

// @ hash_table - Object of class Hashtable
// @ s - word which will be spellchecked removing chars
// spellchecks word by adding chars a to z at every possible position in word
// @return - if a word is found in hashtable, stops adding chars and returns true
// @return - else spellcheck by adding chars fails and returns false
template <typename HashTableType>
bool SpellCheckByAddingChars(const HashTableType & hash_table, const string &s)
{
	string word = s;
	for (unsigned int i=0; i < s.size(); i++)
	{
		for (unsigned int j=97; j <= 122; j++)
		{
			word.insert(i,1,j);
			if (hash_table.Contains(word))
			{
				cout << s << " -> " <<  word << " (Case A) "<< endl;
				return true;
			}
			word = word.erase(i,1);
		}
	}
	return false;
}

//@ s - word which will be edited
// Removes necessary punctuations from word such as ", ' etc
// If a char is upper case, it will be lower cased for the purpose of consistency
void RemovePunctuations (string & s)
{
	if (s[s.size()-1] == '"')
		s.erase(s.size()-1, 1);
	for (unsigned int i=0; i < s.size(); i++)
	{
		if(ispunct(s[i]))
			s = s.erase(i,1);
		if (isalpha(s[i]))
			s[i] = tolower(s[i]);
	}
}

int main(int argc, char **argv)
{
	if (argc != 3) 
	{
		cout << "Usage: " << argv[0] << " <documentfile> <dictionaryfile> " << endl;
		return 0;
	}

	const string document_file(argv[1]);
	const string dictionary_file(argv[2]);

	cout <<"Document filename: " << document_file << endl;
	cout <<"Dictionary filename: " << dictionary_file << endl;

	HashTable <string> new_hash_table;  //object of Quadratic Probing class
	LoadDictionary(new_hash_table,dictionary_file);
	
	ifstream query_test (document_file);
	string word;
	bool word_found;
	cout << "Spell Checking..." << endl;
	while (query_test >> word)
	{
		if (word != "")
			RemovePunctuations(word);

		if (!new_hash_table.Contains(word))
		{
			word_found = SpellCheckBySwappingChars(new_hash_table, word);
			if (!word_found)
				word_found = SpellCheckByRemovingChars(new_hash_table, word);
			if (!word_found)
				SpellCheckByAddingChars(new_hash_table, word);
		}
	}
	query_test.close();
	return 0;
}