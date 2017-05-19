/**
Name: Rubaiyat Rashid
Class: CSCI 335
Professor: Ioannis Stamos
Purpose: Assignment 5
Description: This file reads inputs from a text file containing vertices, adjacent vertices and corresponding costs.
A graph is created using those inputs. Then Dijkstra() is called using a given starting vertex from the console input.
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "Graph.h"
using namespace std;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] <<" <GRAPH_FILE> <STARTING_VERTEX>" << endl;
		return 0;
	}
	const string create_graph_file (argv[1]);
	int starting_vertex = atoi(argv[2]);

	ifstream input (create_graph_file);
	string line; 
	int number_of_vertices; 
	input >> number_of_vertices;
	cout <<"Number of vertices: "<< number_of_vertices << endl;
	Graph new_graph (number_of_vertices);//object of graph class with total number of vertices as a parameter
	
	cout << "Dijkstra... " << endl;
	int vertex;
	int adjacent_vertex;
	double cost;
	while (getline(input, line))
	{
		stringstream str_stream(line);
		str_stream >> vertex;
		while (str_stream >> adjacent_vertex && str_stream >> cost)
		{	
			new_graph.Insert(vertex, adjacent_vertex, cost);
		}
	}

	new_graph.Dijkstra(starting_vertex);

	return 0;
}