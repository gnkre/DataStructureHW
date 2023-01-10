#include "Part1.h"
#include <iostream>

void PartI::read(std::string file) 
{ 
	std::cout << "Part I reading..." << std::endl; 
	std::fstream fle;
	fle.open(file, std::ios::in);
	fle >> num_of_vertex;
	fle >> num_of_edges;
	graph.resize(num_of_vertex);
	FFI.resize(num_of_vertex);
	int weight = 0;
	for (int i = 0; i < num_of_edges; i++)
	{
		int from, to;
		fle >> from >> to >> weight;
		graph[from].push_back(to);
	}
	//std::cin >> num_of_vertex;
	//std::cin >> num_of_edges;
	//graph.resize(num_of_vertex);  // WARNING: NO BUFFER SPACE THERE, BEWARE OF 1 INDEX
	//FFI.resize(num_of_vertex);
	//int weight = 0;
	//for (int i = 0; i < num_of_edges; i++)
	//{
	//	int from, to;
	//	std::cin >> from >> to >> weight;
	//	graph[from].push_back(to);
	//}
	//std::cout << graph.size() << " vertices are created." << std::endl;
	return;
}

void PartI::searchByDFSAndUpdateFinishVector(bool first_time)
{
	if (first_time)
	{
		if_found = new bool[num_of_vertex];
		//init the array
		for (int i = 0; i < num_of_vertex; i++)
		{
			if_found[i] = false;
		}

		int time = 0;
		//DFS from zero
		for (int i = 0; i < num_of_vertex; i++)
		{
			if (if_found[i] == false)
			{
				DFS(i, time, true, 0);
			}
		}
		return;
	}
	else
	{
		if_found = new bool[num_of_vertex];
		coarseGraph.resize(num_of_vertex);
		//init the array
		for (int i = 0; i < num_of_vertex; i++)
		{
			if_found[i] = false;
		}

		int time = 0;
		//DFS from zero
		for (int i = 0; i < num_of_vertex; i++)
		{
			if (if_found[FFI[i].index] == false)
			{
				DFS(FFI[i].index, time, false, FFI[i].index);
				scc_queue.push_back(FFI[i].index);
			}
		}
		return;
	}
}

void PartI::DFS(int vertex_in, int& time, bool first_time, int predecessor)
{
	if (first_time)
	{
		FFI[vertex_in].found = ++time;
		FFI[vertex_in].index = vertex_in;
		if_found[vertex_in] = true;
		for (auto itr = graph[vertex_in].begin(); itr != graph[vertex_in].end(); itr++)
		{
			if (if_found[*itr] == false)
			{
				DFS(*itr, time, true, 0);
			}
		}
		FFI[vertex_in].finish = ++time;
		return;
	}
	else
	{
		coarseGraph[vertex_in].first = vertex_in;
		coarseGraph[vertex_in].second = predecessor;
		if_found[vertex_in] = true;
		for (auto itr = graph_transpose[vertex_in].begin(); itr != graph_transpose[vertex_in].end(); itr++)
		{
			if (if_found[*itr] == false)
			{
				isAyclic = false;
				DFS(*itr, time, false, predecessor);
			}
		}
	}
	return;
}

void PartI::makeGraphTransposeUsingNewVector(vector<vector<int>>& new_gT)
{
	new_gT.resize(num_of_vertex);
	for (int i = 0; i < num_of_vertex; i++)
	{
		for (auto itr = graph[i].begin(); itr != graph[i].end(); itr++)
		{
			new_gT[*itr].push_back(i);
		}
	}
	return;
}

void PartI::swap(FoundFinishIndex& F1, FoundFinishIndex& F2)
{
	FoundFinishIndex temp = F1;
	F1 = F2;
	F2 = temp;
	return;
}

void PartI::sortFFIFromBigtoSmallByFinish()
{
	int size = FFI.size();
	for (int i = 0; i <size; i++)
	{
		int max = i;
		for (int j = i + 1; j < size; j++)
		{
			if (FFI[j].finish > FFI[max].finish)
				max = j;
		}
		if (i != max)
		{
			swap(FFI[i], FFI[max]);
		}
	}
	return;
}

void PartI::coarseGraphUseSmallestVertexAsIndex()
{
	int scc_size = scc_queue.size();
	int CG_size = coarseGraph.size();
	int min = 0;
	for (int i = 0; i < scc_size; i++)
	{
		min = scc_queue[i];
		for (int j = 0; j < CG_size; j++)
		{
			if (coarseGraph[j].second == scc_queue[i] && coarseGraph[j].first < min)
			{
				min = coarseGraph[j].first;
			}
		}
		if (min != scc_queue[i])
		{
			for (int j = 0; j < CG_size; j++)
			{
				if (coarseGraph[j].second == scc_queue[i])
				{
					coarseGraph[j].second = min;
				}
			}
		}
		scc_queue[i] = min;
	}
}

void PartI::solve() 
{ 
	std::cout << "Part I solving..." << std::endl; 
	isAyclic = true;
	for (int i = 0; i < num_of_vertex; i++)
	{
		sortIntVectorSmallToBig(graph[i]);
	}
	searchByDFSAndUpdateFinishVector(true); // results saved to FFI
	makeGraphTransposeUsingNewVector(graph_transpose); //results saved to parameter
	sortFFIFromBigtoSmallByFinish(); //sort FFI
	searchByDFSAndUpdateFinishVector(false);
	if (isAyclic)
	{
		return;
	}
	coarseGraphUseSmallestVertexAsIndex();

	scc_size = scc_queue.size();
	sortIntVectorSmallToBig(scc_queue);
	if (scc_size > 1)
	{
		for (int i = 0; i < scc_size; i++)
		{
			for (int j = 0; j < scc_size; j++)
			{
				if (i == j)
				{
					continue;
				}
				int weight = findEdges(scc_queue[i], scc_queue[j]);
				if (weight != 0)
				{
					q.push_back(FromToWeight(i, j, weight));
				}
			}
		}
	}
	//sortFromToWeightbyFrom(q);
	//int q_size = q.size();
	//std::cout << scc_size << ' ' << q_size << std::endl;
	//for (int i = 0; i < q.size(); i++)
	//{
	//	std::cout << q[i].from << ' ' << q[i].to << ' ' << q[i].weight << std::endl;
	//}


	return;
}

void PartI::sortIntVectorSmallToBig(vector<int>& v)
{
	int size = v.size();
	for (int i = 0; i < size; i++)
	{
		int min = i;
		for (int j = i + 1; j < size; j++)
		{
			if (v[j] < v[min])
			{
				min = j;
			}
		}
		if (i != min)
		{
			std::swap(v[i], v[min]);
		}
	}
	return;
}

void PartI::swapFTW(FromToWeight& a, FromToWeight& b)
{
	FromToWeight temp = a;
	a = b;
	b = temp;
	return;
}

void PartI::sortFromToWeightbyFrom(vector<FromToWeight>& q)
{
	int size = q.size();
	for (int i = 0; i < size; i++)
	{
		int min = i;
		for (int j = i + 1; j < size; j++)
		{
			if (q[j].from < q[min].from)
				min = j;
		}
		if (i != min)
		{
			swapFTW(q[i], q[min]);
		}
	}
	return;
}
int PartI::findEdges(int from, int to)
{
	vector<int> too;
	int ans = 0;
	for (int i = 0; i < num_of_vertex; i++)
	{
		if (coarseGraph[i].second == to)
		{
			too.push_back(coarseGraph[i].first);
		}
	}
	int size = too.size();

	for (int i = 0; i < num_of_vertex; i++)
	{
		if (coarseGraph[i].second == from)
		{
			for (auto itr = graph[coarseGraph[i].first].begin(); itr != graph[coarseGraph[i].first].end(); itr++)
			{
				for (int j = 0; j < size; j++)
				{
					if (too[j] == (*itr))
					{
						ans++;
					}
				}
			}
		}
	}
	return ans;
}

void PartI::write(std::string file) 
{ 
	std::cout << "Part I writing..." << std::endl;
	std::fstream fleo;
	fleo.open(file, std::ios::out);
	if (isAyclic)
	{
		for (int i = 0; i < num_of_vertex; i++)
		{
			fleo << FFI[i].index << ' ';
		}
		return;
	}

	int q_size = q.size();
	fleo << scc_size << ' ' << q_size << "\n";
	for (int i = 0; i < q.size(); i++)
	{
		fleo << q[i].from << ' ' << q[i].to << ' ' << q[i].weight << "\n";
	}
	return;
}