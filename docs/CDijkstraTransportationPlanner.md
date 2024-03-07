## datamembers
```
    std::shared_ptr<SConfiguration> config;
    //an instance of class Sconfiguration
    std::shared_ptr<CStreetMap> streetmap;
    //an instance of class streetmap
    std::shared_ptr<CBusSystem> bussystem;
    //an instance of class CBusSystem    
    std::vector <std::shared_ptr<CStreetMap::SNode>> orderednodes;
    // a vector stores the all of pointers to the node structs 
    std::unordered_map<TNodeID, int>  bikestop;
    //use for bike_pathrouter bikenodestnodeid<->Tvertexidhttps://github.com/yihandiao/ECS34P4/blob/main/docs/CDijkstraTransportationPlanner.md
    std::unordered_map<TNodeID, int> stopsinnodeid;
    // useforbus stoptnodeid <-> Tvertexid
    std::unordered_map<TNodeID, int>  pathroutermap;
    // usefor basicgraph  alloftnodeid <-> Tvertexid
    std::shared_ptr<CPathRouter> pathrouter = std::make_shared<CDijkstraPathRouter>();
    //create a graph for the bus&walk map, an instance of CPathrouter
    std::shared_ptr<CPathRouter> bikepathrouter = std::make_shared<CDijkstraPathRouter>();
    // create a graph for the bycicle, an instance of CPathrouter
    std::shared_ptr<CBusSystemIndexer> bushelper;
    // an instance of class BusSystemIndexer
    double defaultbikespeed;
    double defaultwalkspeed;
    double busstoptime ;
    double defaultbusspeed ;

    struct pair_hash {
       template <class T1, class T2>
       std::size_t operator () (const std::pair<T1,T2> &pair) const { // this functions is used to genearate the hash value in a map for the key
          auto hash1 = std::hash<T1>{}(pair.first);
          auto hash2 = std::hash<T2>{}(pair.second);
      return hash1 ^ hash2; // XOR
    }
    };

   std::unordered_map<std::pair<TNodeID, TNodeID>, bool, pair_hash> routes;// this map stores if there is a route between A->B 
```

## member functions
# int searchforindex(std::unordered_map <TNodeID, int>& map, TNodeID id);
```
//this function will return the correspoding Tvertexid of the an unorderedmap stored nodes' Tnodeid
parameter: std::unordered_map <TNodeID, int>& map, TNodeID id
return type: int Tvertexid
usage: searchforinde(2394829084)
expected output: 2
```

# void checkuniquebikenode(TNodeID node);
```
//this function will check if the Tnodeid of bike is a key in the bikestop unorderedmap,if it is, push it into the bikestop map. If it is not, do not push and do not update value
parameter: TNodeID node
return type: void
expectedoutput : create a  Tnodeid<->node into the unorderedmap stored TNodeId

```


 # void build_bikegraph()
 ```
//this function will build a bikegraph by adding the bike nodes as vertex, and add edge between two bike nodes
  it will also implement a bike unorderedmap creatting relationship Tvertexid <-> Tnodeid
  it will add edge by checking if we can ride bikes in both directions of if it is allowed to ride bikes

parameter: no
return type: no
expected output: a correct graph contains correct bike nodes and bike ways
```


# void build_graph()
 ```
//this function will build a basicgraph by adding the all nodes of all ways in davis as vertex, and add edge between every two nodes
  it will also implement a basic unorderedmap creatting relationship Tvertexid <-> Tnodeid
  it will add edge by checking if we can go in both directions.

parameter: no
return type: no
expected output: a correct graph contains all correct nodes and ways

 ```

# void add_busedge() 
 ```
//this function will adding the bus edge into the basic graph, it will connect all the busstops of all routes in one direction, and add edge
between two stops of each routes
it will also create an unordermap to store Tnodeid and Tvertexid of all bus stops which have routes
it will also create an unordremap which key is a pair<src,dest> , value is bool to represent there exists a route in that direction

parameter: no
return type: no
expectedoutput: successfully add the edge for the busstop
 ```


# void  findKeyByValue(const std::unordered_map<TNodeID, int>& map, int value)
 ```
//this function will find the key via the value of the key, it is used for find corresponding Tvertexid for the given Tnodeid
parameter: const std::unordered_map<TNodeID, int>& map, int value
return type: void
usage: findkeyByValue(bikenode, 10)
expectedouput: return the Tnodeid of Tvertexid 1 in the graph of bikegraph
 ```

# double FindShortestBusPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path2)
```
// this function will find the shortestbus between src and dest based on the basic graph
it is achived by converting Tnodeid src and dest into Tvertexid in basic graph
then it will carry out the dijkstra algorithm to generate the shortest path
then it will store the path into the vector

parameter: TNodeID src, TNodeID dest, std::vector< TNodeID > &path2
return type: double the distance
expectedout output: 10..etc
```

# double FindShortestBikePath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path3)
```
// this function will find the shortestbus between src and dest based on the bike graph
it is achived by converting Tnodeid src and dest into Tvertexid in basic graph
then it will carry out the dijkstra algorithm to generate the shortest path
then it will store the path into the vector

parameter: TNodeID src, TNodeID dest, std::vector< TNodeID > &path3
return type: double, the distance
expected output: 20..etc
```
# double convertMPHtoNumeric(const std::string& speedStr) 
```
// this function will convert the staff lile 20 mph -> 20 in order to get the speed
parameter: const std::string& speedStr
usage: convertMPHtoNumeric("20 mph")
return type: double
expected output: 20
```

# double getSpeedLimitforway(TNodeID start, TNodeID end)
```
// this function will get the speed of the way
it needs to check if both of nodes are on that way
if it is, it will read on that way struct and get the speed by call hasarrtibute()&& getarrtibute("speedmax")
then it will call convertNPHtoNumeric to convert it into the double

parameter: TNodeID start, TNodeID end
usage: getSpeedLimitforway(31407438103, 1890324413)
expectedoutput: 20
```

# double calclutebustraveltime(TNodeID src, TNodeID dest, std::vector< TTripStep > &travelbybus)
```
//this function will calculate the bustraveltime between src and dest, and finish the tripstep
it will first calclulate the shortespath, and store nodes of that path in that vector

after implementating the nodes on that vector, we need to check which portion to take bus and which portion
to walk. It also needs to get the speed for every pieces of edge for calculating the time as well as the stop time


parameter: TNodeID src, TNodeID dest, std::vector< TTripStep > &travelbybus
return type: double
expectedoutput: 20 etc .....
```

# double calculatebycicletime(TNodeID src, TNodeID dest, std::vector< TTripStep > &travelbybike)
```
// this function will calculate the bycicletraveltime between src and dest,and finish the tripstep
it will calculate the shoretest path on the bycicle graph. Then we can get a vector contain lots of nodes
we calculate the time via the speed.

parameter: TNodeID src, TNodeID dest, std::vector< TTripStep > &travelbybike
return type: double
expected output: 20
```

# double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path)
```
// this function will help getting the final result by comparing the bike time and walk+bus time
also, it will check if nodes are valid first.
expectedoutput: the shortest time

parameter: TNodeID src, TNodeID dest, std::vector< TTripStep > &path
return type: double
```




 
