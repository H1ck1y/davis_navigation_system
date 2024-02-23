#include "DijkstraPathRouter.h"
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>

struct CDijkstraPathRouter:: SImplementation{
    struct vertex{
        std::any attribute;
        std::map <TVertexID, double>  adjlist;
    };

    std::map <TVertexID, vertex> vertices;


    TVertexID AddVertex(std::any tag) noexcept{
        vertex new_vertex;
        new_vertex.attribute = tag;
        TVertexID new_vertexid = vertices.size();
        vertices[new_vertexid] = new_vertex;
        return new_vertexid;
    }
     

    std::any GetVertexTag(CDijkstraPathRouter::TVertexID id) const noexcept{
        auto target_vertex = vertices.find(id);
        if( target_vertex != vertices.end()) {
            return target_vertex->second.attribute;
        }
        return std::any();
    }

    bool AddEdge(CDijkstraPathRouter::TVertexID src, CDijkstraPathRouter::TVertexID dest, double weight, bool bidir) noexcept{
        if (dest == InvalidVertexID || src == InvalidVertexID || weight < 0){
            return false;
        }
        if (vertices.find(src) == vertices.end() || vertices.find(dest) == vertices.end()) {
            return false;
        }
        vertices[src].adjlist[dest] = weight;
        if (bidir == true){
           vertices[dest].adjlist[src] = weight;
        }
        return true;
    }

    double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{//https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
        std::unordered_map<TVertexID, double> dist;
        std::unordered_map<TVertexID, TVertexID> prev;
        std::set<std::pair<double, TVertexID>> Q;
        const double INFINITY = std::numeric_limits<double>::infinity();
        for (auto i = vertices.begin(); i != vertices.end(); ++i){
            dist[i->first] = INFINITY;
            prev[i->first] = InvalidVertexID;
            Q.insert({INFINITY, i->first});// add v to Q
        }

        dist[src] = 0; //dist[source] ← 0
        Q.insert({0, src}); 

        while (!Q.empty()){
            TVertexID u = Q.begin()->second;
            Q.erase(Q.begin());
            if (u == dest) break;

            for (auto edge = vertices.at(u).adjlist.begin(); edge != vertices.at(u).adjlist.end(); ++edge){
                TVertexID v = edge->first; 
                double weight = edge->second; 
                double alt = dist[u] + weight;
                if (alt < dist[v]) {
                    Q.erase({dist[v], v}); 
                    dist[v] = alt;
                    prev[v] = u;
                    Q.insert({dist[v], v});
                }

            }
        }
    path.clear();
    TVertexID current = dest;
    if (dist[current] == INFINITY) {
        return INFINITY; 
    }

    while (current != InvalidVertexID) {
        path.push_back(current);
        if (current == src) break;
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

}

double CDijkstraPathRouter::FindShortestPath(CDijkstraPathRouter::TVertexID src, CDijkstraPathRouter::TVertexID dest, std::vector<CDijkstraPathRouter::TVertexID> &path) noexcept{
        return DImplementation->FindShortestPath(src,dest,path);

}