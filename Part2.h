#ifndef PARTII_H
#include <vector>
#include <queue>
#include <fstream>
#include "SolverBase.h"

class PartII : public SolverBase
{
    struct ToWeight
    {
        int to;
        int weight;
        ToWeight(int to_in, int weight_in) :to(to_in), weight(weight_in) {};
    };
    class MinHeap
    {
    public:
        struct Vertex
        {
            int vertex;
            int distance;
            Vertex(int ver_in, int dist_in) :vertex(ver_in), distance(dist_in) {};
            Vertex() :vertex(0), distance(0) {};
        };
        std::vector<Vertex> heap;
        int size_of_heap;
        void swap(Vertex& a, Vertex& b);
        void initHeapWithNumOfVertex(int num_of_vertex_in, std::vector<int> dist_vec);
        int getParentIndex(int node_in)
        {
            return (node_in - 1) / 2;
        }
        int extractHeap();
        void min_heapify(int index);
        void decreaseVertexDistance(int vertex_in, int new_dist);
        bool isThisEmpty();
        MinHeap() { size_of_heap = 0; };
    };
public:
    int num_of_vertex;
    int num_of_edge;
    int dijkstra_ans;
    int bellmanFord_ans;
    std::vector<std::vector<ToWeight>> graph;
    void read(std::string);
    void solve();
    void write(std::string);

    int dijkstra();
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_queue;
    std::vector<int> dijkstra_distance;
    void dijkstra_init();
    bool dijkstra_relax(int from, int to, int weight);

    int bellmanFord();
    std::vector<int> bellmanFord_distance;
    void bellmanFord_init();
    bool bellmanFord_relax(int from, int to, int weight);
};

#define PARTII_H
#endif