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
	Graph new_graph (number_of_vertices);
	/*
	new_graph.CheckAdjacency(4, 1);
	new_graph.CheckAdjacency(3, 4);
	new_graph.CheckAdjacency(1, 5);
	new_graph.CheckAdjacency(5, 1);
	new_graph.CheckAdjacency(1, 3);
	*/
	int vertex;
	int adjacent_vertex;
	double cost;
	while (getline(input, line))
	{
		stringstream str_stream(line);
		str_stream >> vertex;
		//cout << "vertex " << vertex << " ";

		while (str_stream >> adjacent_vertex && str_stream >> cost)
		{	
			new_graph.Insert(vertex, adjacent_vertex, cost);
			//cout << adjacent_vertex << " " << cost << endl;
		}
		//cout << endl;
	}

	ifstream query(adjacency_query_file);
	while (getline(query, line))
	{
		stringstream str_stream(line);
		while (str_stream >> vertex && str_stream >> adjacent_vertex)
		{	
			//cout << vertex << " " << adjacent_vertex << endl;
			new_graph.CheckAdjacency(vertex, adjacent_vertex);
		}
		//cout << endl;
	}

	return 0;
}