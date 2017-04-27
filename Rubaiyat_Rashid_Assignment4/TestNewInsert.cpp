/**
Name: Rubaiyat Rashid
Class: CSCI 335
Professor: Ioannis Stamos
Purpose: Assignment 4
Description: TestNewInsert does everything TestInsertAndDelete except in this case the values are inserted with a
			new insert function NewInsert(). 
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
		a_priority_queue.NewInsert(a_value);
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