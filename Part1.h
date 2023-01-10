#ifndef PARTI_H
#include "SolverBase.h"
#include <vector>
#include <fstream>

using namespace std;

class PartI : public SolverBase
{
    //Structures
    struct FoundFinishIndex
    {
        int found;
        int finish;
        int index;
        FoundFinishIndex(int fo_in, int fi_in, int ind_in) :found(fo_in), finish(fi_in), index(ind_in) {};
        FoundFinishIndex() :found(0), finish(0), index(0) {};
    };
    struct FromToWeight
    {
        int from;
        int to;
        int weight;
        FromToWeight(int from_in, int to_in, int weight_in) :from(from_in), to(to_in), weight(weight_in) {};
        FromToWeight() :from(0), to(0), weight(0) {};
    };

    //Variables
    int num_of_vertex;
    int num_of_edges;
    int scc_size;
    vector<vector<int>> graph;
    vector<pair<int, int>> coarseGraph;
    vector<vector<int>> graph_transpose;
    vector<int> scc_queue;
    vector<FoundFinishIndex> FFI;
    bool* if_found;
    bool isAyclic = true;
    vector<FromToWeight> q;

    //Functions
    int findEdges(int from, int to);
    void coarseGraphUseSmallestVertexAsIndex();
    void searchByDFSAndUpdateFinishVector(bool first_time);
    void DFS(int vertex_in, int& time, bool first_time, int predecessor);
    void makeGraphTransposeUsingNewVector(vector<vector<int>>& new_gT);
    void sortFFIFromBigtoSmallByFinish();
    void swap(FoundFinishIndex& F1, FoundFinishIndex& F2);
    void sortFromToWeightbyFrom(vector<FromToWeight>& q);
    void swapFTW(FromToWeight& a, FromToWeight& b);
    void sortIntVectorSmallToBig(vector<int>& v);
    
public:
    void read(std::string);
    void solve();
    void write(std::string);
};

#define PARTI_H
#endif