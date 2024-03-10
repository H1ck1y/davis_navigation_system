#include "DijkstraPathRouter.h"
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>

struct CDijkstraPathRouter:: SImplementation{ // use the adjacentlists to represent the graph
    struct vertex{
        std::any attribute;
        std::map <TVertexID, double>  adjlist;
    };

    std::map <TVertexID, vertex> vertices;
        
    struct Pri { // define a self comparison for priority queue  
        bool operator()(const std::pair<double, TVertexID>& a, 
                        const std::pair<double, TVertexID>& b) const {
            return a.first > b.first;
        }
    };


    TVertexID AddVertex(std::any tag) noexcept{// add attributes into the graph
        vertex new_vertex;
        new_vertex.attribute = tag;
        TVertexID new_vertexid = vertices.size();
        vertices[new_vertexid] = new_vertex;
        return new_vertexid;
    }
     

    std::any GetVertexTag(CDijkstraPathRouter::TVertexID id) const noexcept{
        auto target_vertex = vertices.find(id); // check the attributes in the addge
        if( target_vertex != vertices.end()) {
            return target_vertex->second.attribute;
        }
        return std::any();
    }

    bool AddEdge(CDijkstraPathRouter::TVertexID src, CDijkstraPathRouter::TVertexID dest, double weight, bool bidir) noexcept{
        if (dest == InvalidVertexID || src == InvalidVertexID || weight < 0){
            return false;
        }    // achieve this by addding nodes into the adjacent lists
        if (vertices.find(src) == vertices.end() || vertices.find(dest) == vertices.end()) {
            return false;
        }
        vertices[src].adjlist[dest] = weight;
        if (bidir == true){ // if we need to add in both directions then add the edge
           vertices[dest].adjlist[src] = weight;
        }
        return true;
    }

    double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{
        //https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
        //https://stackoverflow.com/questions/17898342/priority-queue-in-dijkstras-algorithm
        std::unordered_map<TVertexID, double> dist;
        std::unordered_map<TVertexID, TVertexID> prev;
        std::priority_queue<std::pair<double, TVertexID>, 
                            std::vector<std::pair<double, TVertexID>>, 
                            Pri> Q; // priority uses heap to generate the path

        const double INFINITY = std::numeric_limits<double>::infinity();
        for (const auto& vertex_pair : vertices) {
            dist[vertex_pair.first] = INFINITY;
            prev[vertex_pair.first] = InvalidVertexID;
        }
        dist[src] = 0;
        Q.push({0, src});
        while (!Q.empty()) { //irretation until the queue is empty
            TVertexID u = Q.top().second;
            Q.pop();

            if (u == dest) {
                break;
            }

            for (const auto& edge : vertices.at(u).adjlist) {
                TVertexID v = edge.first;
                double weight = edge.second;
                double alt = dist[u] + weight;

                if (alt < dist[v]) { // keep updating the shortest based on the previous shortest path
                    dist[v] = alt;
                    prev[v] = u;
                    Q.push({alt, v});
                }
            }
        }
        path.clear();
        TVertexID current = dest;
        if (dist[current] == INFINITY) {
            return INFINITY;
        }
        while (current != InvalidVertexID) { // use the traceback array to generate the result
            path.push_back(current);
            current = prev[current];
        }
        std::reverse(path.begin(), path.end());

        return dist[dest];
    }
    
};


CDijkstraPathRouter::CDijkstraPathRouter(){
    DImplementation = std::make_unique<SImplementation>();
}

CDijkstraPathRouter:: ~CDijkstraPathRouter(){}

std::size_t  CDijkstraPathRouter::VertexCount() const noexcept{
        return DImplementation->vertices.size();

}

CDijkstraPathRouter::TVertexID  CDijkstraPathRouter::AddVertex(std::any tag) noexcept{
        return DImplementation->AddVertex(tag);
}

std::any CDijkstraPathRouter::GetVertexTag(CDijkstraPathRouter::TVertexID id) const noexcept{
        return DImplementation->GetVertexTag(id);
}

bool CDijkstraPathRouter::AddEdge(CDijkstraPathRouter::TVertexID src, CDijkstraPathRouter::TVertexID dest, double weight, bool bidir) noexcept{
         return DImplementation->AddEdge(src, dest ,weight, bidir);
}
        
bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept{
        return true;

}

double CDijkstraPathRouter::FindShortestPath(CDijkstraPathRouter::TVertexID src, CDijkstraPathRouter::TVertexID dest, std::vector<CDijkstraPathRouter::TVertexID> &path) noexcept{
        return DImplementation->FindShortestPath(src,dest,path);

}