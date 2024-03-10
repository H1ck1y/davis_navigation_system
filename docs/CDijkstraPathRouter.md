# datamembers
```
struct vertex{
        std::any attribute;
        std::map <TVertexID, double>  adjlist;
};// this defines a vertex which has the attribute

std::map <TVertexID, vertex> vertices; // this stores the adjlist for the map
        
struct Pri { // define a self comparison for priority queue which increases the preformance of the dijkstra algortim
        bool operator()(const std::pair<double, TVertexID>& a, 
                        const std::pair<double, TVertexID>& b) const {
            return a.first > b.first;
        }
    };

```
# Memberfunctions

TVertexID  std::size_t VertexCount() const noexcept = 0;
```
//this is total number of vertexes in the graph
parameter : no
return type: std::size_t
expectedouput : 10 etc
```
virtual TVertexID AddVertex(std::any tag) noexcept = 0;
```
// this function will add vertex to a graph where the vertex of the graph depends on the parameter, it can be a node struct or a number
parameter: std::any tag
return type: TvertexID
expectedoutput: successfully add the vertex into the graph, that graph has the attribute of the parameter
```
std::any GetVertexTag(TVertexID id) const noexcept = 0;
```
parameter: TvertexID id
return type: std::any
expected output: output the attribute of that vertex, such as a pointer to the node
```
 bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir = false) noexcept = 0;
```
// this function add an edge betweeen two edges along with the distacnce between two nodes.
parameter: TVertexID src, TVertexID dest, double weight, bool bidir = false(bidir is the parameter if we need to add an edge between A->  B and B->A)
return type: bool value to decide if we add it successfully

```
bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept = 0;
```
// this is for setting the precompute time. Useful in the future. for the speedtest
```
double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept = 0;
```
// this function will find the shortestpath between src && dest and store the path in the vector by using dijskra algorithm
parameter: TverteixId src, Tvertexid dest, std::vector<TVertexID> &path
return type: double
expectedoutput: a distance from src to dest
```
