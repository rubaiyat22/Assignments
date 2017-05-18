#ifndef GRAPH_H_
#define GRAPH_H_

#include <queue>
#include <limits>

struct AdjacentVertex
{
	int v_num_;
	double cost_;

	AdjacentVertex (const int & v, const double & c) : v_num_ {v}, cost_{c}{}
};

struct Vertex
{
	int vertex_num_;
	bool known_;
	double dist_;
	int indigree_;
	int top_num_;
	Vertex *path_; 
	std::vector <AdjacentVertex> adj_list_;

	Vertex (const int & v_num, const bool & k, const double & d, const int i, const int t_n, Vertex * p)
		: vertex_num_ {v_num}, known_{k}, dist_ {d}, indigree_ {i}, top_num_{t_n}, path_ {p} {}	
};

struct CompareVertices
{
	bool operator() (const Vertex * v1, const Vertex * v2) const
	{
		return v1 -> dist_ > v2 -> dist_;
	}
};

class Graph
{
public:
	Graph(){}

	~Graph()
	{ 
		ClearGraph();
	}

	Graph(const Graph & rhs): vertices_ (rhs.vertices_.size())
	{
		for (size_t i = 1; i < rhs.vertices_.size(); i++)
		{
			vertices_[i] = new Vertex(rhs.vertices_[i] -> vertex_num_, rhs.vertices_[i] -> known_,
			rhs.vertices_[i] -> dist_, rhs.vertices_[i] -> indigree_, rhs.vertices_[i] -> top_num_, rhs.vertices_[i] -> path_);
			this->vertices_[i]->adj_list_ = rhs.vertices_[i]->adj_list_;
		}	
	}

	Graph & operator= (const Graph & rhs)
	{
		Graph copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	Graph(const int & num_of_vertices) : vertices_ (num_of_vertices + 1)
	{
		size_t inf = std::numeric_limits<int>::max();
		for (size_t i = 1; i < num_of_vertices + 1; i++)
		{
			vertices_[i] = new Vertex (i, false, inf, 0, 0, nullptr);
		}
	}

	void Insert(const int & v, const int & adj_v, const double & cost)
	{
		AdjacentVertex adj (adj_v, cost);
		vertices_[v] -> adj_list_.push_back (adj);
	}

	void CheckAdjacency(const int & v1, const int & v2)
	{
		bool connection = false;
		for (size_t i = 0; i < vertices_[v1] -> adj_list_.size(); i++)
		{
			if (vertices_[v1] -> adj_list_[i].v_num_ == v2)
			{
				connection = true;
				std::cout << v1 <<" "<< v2 <<": Connected, weight of edge is " << vertices_[v1] -> adj_list_[i].cost_ << std::endl;
				break;
			}
		}
		if (!connection)
			std::cout << v1 <<" "<< v2 <<": Not Connected " << std::endl;
	}

	void Dijkstra(const int & v)
	{
		vertices_[v] -> dist_ = 0;
		std::priority_queue <Vertex*, std::vector<Vertex*>, CompareVertices> min_priority_queue;
		min_priority_queue.push(vertices_[v]);

		while (!(min_priority_queue.empty()))
		{
			Vertex * v = min_priority_queue.top();
			min_priority_queue.pop();
			v -> known_ = true;

			for (int i = 0; i < v -> adj_list_.size(); i++)
			{
				int v_num = v -> adj_list_[i].v_num_;
				Vertex * unknown_v = vertices_[v_num];
				if (!unknown_v -> known_)
				{
					double c = v -> adj_list_[i].cost_;
					if (v -> dist_ + c < unknown_v -> dist_)
					{ 
						unknown_v -> dist_ = v -> dist_ + c;
						unknown_v -> path_ = v;
					}
					min_priority_queue.push (unknown_v);
				}
			}
		}

		for (size_t i = 1; i < vertices_.size(); i++)
		{
			std::cout << vertices_[i] -> vertex_num_ <<": ";
			double shortest_path_cost = vertices_[i] -> dist_;
			PrintShortestPaths(vertices_[i]);
			if (shortest_path_cost != std::numeric_limits<int>::max())
				std::cout << "(Cost: " << shortest_path_cost << ")" << std::endl;
			else
				std::cout << "(No path)" << std::endl;
		}
		
	}

	void TopologicalSort()
	{
		CalculateIndigrees();
		TopSort();
	}

private:
	std::vector <Vertex*> vertices_;

	void ClearGraph()
	{
		for (size_t i = 1; i < vertices_.size(); i++)
		{
			delete vertices_[i];
			vertices_[i] = nullptr; 
		}
	}

	void CalculateIndigrees()
	{	
		for (size_t i = 1; i < vertices_.size(); i++)
		{
			for (size_t j = 0; j < vertices_[i] -> adj_list_.size(); j++)
			{
				int adj_v_num = vertices_[i] -> adj_list_[j].v_num_;
				vertices_[adj_v_num] -> indigree_++;
			}
		}
	}

	void PrintShortestPaths (Vertex * v)
	{
		if (v -> path_ !=  nullptr)
		{
			PrintShortestPaths( v -> path_);
		}
		std::cout << v -> vertex_num_ << " ";
	}

	void TopSort()
	{
		std::queue <Vertex*> a_queue;
		int count = 0;

		for (size_t i = 1; i < vertices_.size(); i++)
		{
			if (vertices_[i] -> indigree_ == 0)
				a_queue.push(vertices_[i]);
		}

		while (!(a_queue.empty()))
		{
			Vertex * v = a_queue.front();
			std::cout << v -> vertex_num_ <<", ";
			a_queue.pop();
			v -> top_num_ = ++count;
			for (int j = 0; j < v -> adj_list_.size(); j++)
			{
				Vertex * w = vertices_[v ->adj_list_[j]. v_num_];
				w -> indigree_--;
				if (w -> indigree_ == 0)
					a_queue.push (w);
			}
		}
		if (count != vertices_.size()-1)
			std::cout << "Cycle Found "<< std::endl;
		else
			std::cout << std::endl;
	}
};
#endif