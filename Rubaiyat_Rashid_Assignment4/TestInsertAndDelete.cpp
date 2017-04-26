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
	int a_value;

	PriorityQueue <int> a_priority_queue;

	while (input_values >> a_value)
		a_priority_queue.Insert(a_value);

	cout << "Success inserting " << a_priority_queue.TotalNumberOfElements() << " elements into the queue.";
	cout << " The minimum element is "<< a_priority_queue.FindMin() << ". " << endl;

	ifstream delete_input(input_file_delete);

	while (delete_input >> a_value)
	{
		if (a_priority_queue.Remove(a_value))
			cout << a_value << " deleted from queue " << endl;
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