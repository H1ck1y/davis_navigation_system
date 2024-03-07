# Datamembers
  ## TNodeID = CStreetMap::TNodeID;
  ## enum class ETransportationMode {Walk, Bike, Bus};
      this is for recording the ways to travel around
  ## using TTripStep = std::pair<ETransportationMode, TNodeID>;
      this is for recording the the tnodeid corresponding to their transportation methods
      ex: {<Walk,10>, <Bus,12>,<Bike,11>}


  ## struct SConfiguration{
```
            virtual ~SConfiguration(){};
            // this is the destructor of class Sconfiguration
            virtual std::shared_ptr<CStreetMap> StreetMap() const noexcept = 0;
            // this returns a pointer to the streetmap class
            virtual std::shared_ptr<CBusSystem> BusSystem() const noexcept = 0;
            // this returns a pointer to the bussystem class
            virtual double WalkSpeed() const noexcept = 0;
            // this helps getting the default walkspeed
            virtual double BikeSpeed() const noexcept = 0;
            //this helps getting the default bikespeed
            virtual double DefaultSpeedLimit() const noexcept = 0;
            //this helps getting the default speedlimit on the way for the bus
            virtual double BusStopTime() const noexcept = 0;
            //this helps getting the default busstop time
            virtual int PrecomputeTime() const noexcept = 0;
            //this helps getting the precompute time.
```
        };

# member functions

## virtual ~CTransportationPlanner(){};
```
//this is the destructor, an object will be destructed automatically at the end
```

## virtual std::size_t NodeCount() const noexcept = 0;
```
// this for the nodecounts. This will return the total number of nodes of a graph
return type:  std::size_t
usage : transportaion-> NodeCount();
expected output : 10 11 etc..
```

## virtual std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept = 0;
```
// this one will return a pointer to the nodestruct. Nodestruct contains the specific information via the index in the container
return type:  std::shared_ptr<CStreetMap::SNode>
parameter : std::size_t index
usage: tranportation -> sorteNodeByIndex(0);
expectedoutput: a pointer pointed to the node at index 0  in the container
```
## virtual double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path) = 0;
```
// this one will return a specific distance
return type:  double
parameter : TNodeID src  TNodeID dest std::vector< TNodeID > &path
usage:  std::vector< TNodeID > path
     transportation->(10, 9, path)
expected output: 20 miles path:{10,4,5,,23,2,324,12,9}
```

## virtual double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) = 0;
```
// this one will return the fastedpath between src and dest, use a vector to record the transportation method and corresponding nodes
return type: double
parameter: TNodeID src  TNodeID dest  std::vector< TTripStep > &path
usage : std::vector< TTripStep > &path
        transportation->(10,9,path)
expected output: 20 miles path{10🚶,4🚌,5🚲,...etc...9🚶}

```

## virtual bool GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const = 0;
```
//this one will generate the path description via the the path vector we get before
return type:  bool
parameter: const std::vector< TTripStep > &path    std::vector< std::string > &desc
usage: transportation-> GetPathDescription(path,pathdesctiption)
expected outout:"Start at 38d 23' 60\" N, 121d 43' 12\" W",
                 "Bike N toward Main St. for 6.9 mi",
                 "Bike W along Main St. for 4.3 mi",
                 "Bike N along B St. for 3.5 mi",
                 "End at 38d 32' 60\" N, 121d 47' 60\" W"
```
