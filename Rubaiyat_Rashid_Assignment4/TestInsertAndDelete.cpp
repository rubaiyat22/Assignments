/**
Name: Rubaiyat Rashid
Class: CSCI 335
Professor: Ioannis Stamos
Purpose: Assignment 4
Description:TestInsertAndDelete creates a object of class Priority Queue and inserts unsigned integers to
			the queue. It outputs the number of elements inserted and the minimum number in the queue.
			Then, it reads unsigned integers from a file to test deletion. Once removed is called with the value read
		 	from file as a parameter, the value is put through Find() to see if it still exists. Before 
		 	searching in queue, it checks whether or not the queue is empty.
*/
#include <iostream>
#include <fstream>
#include "PriorityQueue.h"
using namespace std;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] <<" <input_file_to_create_queue> <input_file_to_check_delete>" << endl;
		return 0;
	}	

	const string input_file_create (argv[1]);
	const string input_file_delete (argv[2]);

	ifstream input_values (input_file_create);

	if (!input_values)
	{
		cout << "Unable to open file " << input_file_create << endl;
		return 0;
	}

	int a_value;
	PriorityQueue <int> a_priority_queue;

	while (input_values >> a_value)
		a_priority_queue.Insert(a_value);

	cout << "Success inserting " << a_priority_queue.TotalNumberOfElements() << " elements into the queue.";
	cout << " The minimum element is "<< a_priority_queue.FindMin() << ". " << endl;

	ifstream delete_input(input_file_delete);
	if (!delete_input)
	{
		cout << "Unable to open file " << input_file_delete << endl;
		return 0;
	}

	while (delete_input >> a_value)
	{
		if (a_priority_queue.Remove(a_value))
			cout << a_value << " deleted from queue " << endl;
		else
			continue;
		if (a_priority_queue.Find(a_value))
		{	
			cout << "Serious problem with deletion routine - ";
			if (!(a_priority_queue.IsEmpty()))
				cout << "Minimum element is " << a_priority_queue.FindMin() << endl;
			else
				cout << a_value << " was the last element to be deleted from queue. " << endl;
		}
		else
		{
			cout << "Deletion Successful - ";
			if (!(a_priority_queue.IsEmpty()))
				cout << "Minimum element is " << a_priority_queue.FindMin() << endl;
			else
				cout << a_value << " was the last element to be deleted from queue. " << endl;
		}
	}

	delete_input.close();

	return 0;
}