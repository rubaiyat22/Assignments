/**
Name: Rubaiyat Rashid
Class: CSCI 335
Professor: Ioannis Stamos
Purpose: Assignment 5
Description: This file reads inputs from a text file containing vertices, adjacent vertices and correspodings costs.
A graph is created using those inputs. Then, some vertices and adjacent vertices are read from a query file to test
whether the vertices and adjacent vertices are connected. If connected, the cost of the edge is outputted. 
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"
using namespace std;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] <<" <GRAPH_FILE> <ADJACENCY_QUERY_FILE>" << endl;
		return 0;
	}
	const string create_graph_file (argv[1]);
	const string adjacency_query_file (argv[2]);

	ifstream input (create_graph_file);
	string line; 
	int number_of_vertices; 
	input >> number_of_vertices;
	cout <<"Number of vertices: "<< number_of_vertices << endl;
	Graph new_graph (number_of_vertices); //object of graph class with total number of vertices as a parameter

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

	ifstream query(adjacency_query_file);
	cout << "Testing for Adjacency..." << endl;
	while (getline(query, line))
	{
		stringstream str_stream(line);
		while (str_stream >> vertex && str_stream >> adjacent_vertex)
		{	
			new_graph.CheckAdjacency(vertex, adjacent_vertex);
		}
	}

	return 0;
}