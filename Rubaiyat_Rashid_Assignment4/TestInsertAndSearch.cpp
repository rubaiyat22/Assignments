/**
Name: Rubaiyat Rashid
Class: CSCI 335
Professor: Ioannis Stamos
Purpose: Assignment 4
Description:TestInsertAndSearch creates a object of class Priority Queue and inserts unsigned integers 
			to the queue from a file. It outputs the number of elements inserted and the minimum number
			in the queue. Then, it reads integers from a file to test whether they exist in the queue or
			not. If a element is found, then the string "Found" is outputted corresponding to the value.
			Other wise, the string "Not Found " is ouputted. 
*/
#include <iostream>
#include <fstream>
#include "PriorityQueue.h"
using namespace std;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] <<" <input_file_to_create_queue> <input_file_to_check_search>" << endl;
		return 0;
	}	

	const string input_file_create (argv[1]);
	const string input_file_search (argv[2]);

	cout << input_file_search << endl;
	ifstream input_values (input_file_create);

	if (!input_values)
	{
		cout << "Unable to open file " << input_file_create << endl;
		return 0;
	}
	
	int a_value;	
	PriorityQueue <int> a_priority_queue;

	while (input_values >> a_value)
	{
		a_priority_queue.Insert(a_value);
	}

	cout << "Success inserting " << a_priority_queue.TotalNumberOfElements() << " elements into the queue.";
	cout << " The minimum element is "<< a_priority_queue.FindMin() << ". " << endl;

	input_values.close();

	ifstream search_input(input_file_search);
	if (!search_input)
	{
		cout << "Unable to open file " << input_file_search << endl;
		return 0;
	}

	while (search_input >> a_value)
	{
		if (a_priority_queue.Find(a_value))
			cout << "Found " << a_value << endl;
		else
			cout << "Not Found " << a_value << endl;
	}
	search_input.close();

	return 0;
}