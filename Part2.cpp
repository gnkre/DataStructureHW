#include "Part2.h"
#include <iostream>
#include <fstream>
const int MAX_FOR_DIJKSTRA = INT_MAX;

int abs(int a)
{
	if (a >= 0)
		return a;
	return a * -1;
}

void PartII::read(std::string file) 
{ 
	std::cout << "Part II reading..." << std::endl;
	std::fstream fle;
	fle.open(file, std::ios::in);
	fle >> num_of_vertex;
	fle >> num_of_edge;
	graph.resize(num_of_vertex);
	for (int i = 0; i < num_of_edge; i++)
	{
		int from, to, weight;
		fle >> from >> to >> weight;
		graph[from].push_back(ToWeight(to, weight));
	}
	//std::cin >> num_of_vertex;
	//std::cin >> num_of_edge;
	//graph.resize(num_of_vertex);
	//for (int i = 0; i < num_of_edge; i++)
	//{
	//	int from, to, weight;
	//	std::cin >> from >> to >> weight;
	//	graph[from].push_back(ToWeight(to, weight));
	//}
	//std::cout << graph.size() << " vertices are created." << std::endl;
	return;
}
void PartII::solve() 
{ 
	std::cout << "Part II solving..." << std::endl;
	dijkstra_ans = dijkstra();
	bellmanFord_ans = bellmanFord();
		
	return;
}

void PartII::MinHeap::swap(Vertex& a, Vertex& b)
{
	Vertex temp = a;
	a = b;
	b = temp;
	return;
}

void PartII::MinHeap::initHeapWithNumOfVertex(int num_of_vertex_in, std::vector<int> dist_vec)
{
	heap.resize(num_of_vertex_in);
	size_of_heap = num_of_vertex_in;
	for (int i = 0; i < num_of_vertex_in; i++)
	{
		heap[i] = Vertex(i, dist_vec[i]);
	}
	return;
}

void PartII::MinHeap::min_heapify(int index)
{
	int left = (2 * index) + 1;
	int right = (2 * index) + 2;
	int smallest = index;
	if (left < size_of_heap && heap[left].distance < heap[index].distance)
	{
		smallest = left;
	}
	if (right < size_of_heap && heap[right].distance < heap[index].distance)
	{
		smallest = right;
	}
	if (smallest != index)
	{
		swap(heap[smallest], heap[index]);
		min_heapify(smallest);
	}
}

int PartII::MinHeap::extractHeap()
{
	if (size_of_heap == 0)
	{
		return -1;
	}
	if (size_of_heap == 1)
	{
		size_of_heap--;
		return heap[0].vertex;
	}

	Vertex root = heap[0];
	heap[0] = heap[size_of_heap - 1];
	size_of_heap--;
	min_heapify(0);

	return root.vertex;
}

bool PartII::MinHeap::isThisEmpty()
{
	if (size_of_heap == 0)
	{
		return true;
	}
	return false;
}

void PartII::MinHeap::decreaseVertexDistance(int vertex_in, int new_dist)
{
	int target_index = 0;
	for (int i = 0; i < size_of_heap; i++)
	{
		if (heap[i].vertex == vertex_in)
		{
			target_index = i;
		}
	}

	if (new_dist > heap[target_index].distance)
	{
		std::cout << "Min heap decrease function error";
		return;
	}
	heap[target_index].distance = new_dist;
	while (target_index > 0 && heap[getParentIndex(target_index)].distance > heap[target_index].distance)
	{
		swap(heap[target_index], heap[getParentIndex(target_index)]);
		target_index = getParentIndex(target_index);
	}
	return;
}

int PartII::dijkstra()
{
	dijkstra_init();
	MinHeap min_heap;
	min_heap.initHeapWithNumOfVertex(num_of_vertex, dijkstra_distance);
	
	while (min_heap.isThisEmpty() == false)
	{
		int vertex = min_heap.extractHeap();
		for (auto itr = graph[vertex].begin(); itr != graph[vertex].end(); itr++)
		{
			if (dijkstra_relax(vertex, (*itr).to, abs((*itr).weight)) == true)
			{
				min_heap.decreaseVertexDistance((*itr).to, dijkstra_distance[(*itr).to]);
			}
		}
	}
	//std::cout << dijkstra_distance[num_of_vertex - 1];
	int ans = dijkstra_distance[num_of_vertex - 1];
	dijkstra_distance.clear();
	min_heap.heap.clear();
	return ans;
}

void PartII::dijkstra_init()
{
	dijkstra_distance.resize(num_of_vertex);

	for (int i = 0; i < num_of_vertex; i++)
	{
		dijkstra_distance[i] = MAX_FOR_DIJKSTRA;
	}

	dijkstra_distance[0] = 0;
	return;
}

bool PartII::dijkstra_relax(int from, int to, int weight)
{
	if (dijkstra_distance[to] > (dijkstra_distance[from] + weight))
	{
		dijkstra_distance[to] = dijkstra_distance[from] + weight;
		return true;
	}
	return false;
}

void PartII::bellmanFord_init()
{
	bellmanFord_distance.resize(num_of_vertex);
	for (int i = 0; i < num_of_vertex; i++)
	{
		bellmanFord_distance[i] = MAX_FOR_DIJKSTRA;
	}
	bellmanFord_distance[0] = 0;
	return;
}

bool PartII::bellmanFord_relax(int from, int to, int weight)
{
	if (bellmanFord_distance[to] > (bellmanFord_distance[from] + weight))
	{
		bellmanFord_distance[to] = bellmanFord_distance[from] + weight;
		return true;
	}
	return false;
}

int PartII::bellmanFord()
{
	bellmanFord_init();
	for (int i = 0; i < num_of_vertex - 1; i++)
	{
		for (int j = 0; j < num_of_vertex; j++)
		{
			for (auto itr = graph[j].begin(); itr != graph[j].end(); itr++)
			{
				bellmanFord_relax(j, (*itr).to, (*itr).weight);
			}
		}
	}

	for (int i = 0; i < num_of_vertex; i++)
	{
		for (auto itr = graph[i].begin(); itr != graph[i].end(); itr++)
		{
			if (bellmanFord_relax(i, (*itr).to, (*itr).weight) == true)
			{
				//std::cout << "Negative";
				return -1;
			}
		}
	}
	//std::cout << bellmanFord_distance[num_of_vertex - 1];
	return bellmanFord_distance[num_of_vertex - 1];
}

void PartII::write(std::string file) 
{ 
	std::cout << "Part II writing..." << std::endl; 
	std::fstream fleo;
	fleo.open(file, std::ios::out);
	fleo << dijkstra_ans;
	fleo << "\n";
	if (bellmanFord_ans == -1)
	{
		fleo << "Negative loop detected!";
	}
	else
	{
		fleo << bellmanFord_ans;
	}
	return;
}