#include "DijkstraTransportationPlanner.h"
#include <algorithm>
#include <vector>
#include "GeographicUtils.h"
#include "iostream"
#include "DijkstraPathRouter.h"
#include "BusSystemIndexer.h"
#include <climits> 
#include <utility>
#include <limits>
#include <tuple>


struct CDijkstraTransportationPlanner::SImplementation{
    std::shared_ptr<SConfiguration> config;
    std::shared_ptr<CStreetMap> streetmap;
    std::shared_ptr<CBusSystem> bussystem;    
    std::vector <std::shared_ptr<CStreetMap::SNode>> orderednodes;
    std::vector<std::shared_ptr<CBusSystem::SStop>> stops;
    std::vector<TNodeID> stopsinnodeid;
    std::shared_ptr<CPathRouter> pathrouter = std::make_shared<CDijkstraPathRouter>();
    std::shared_ptr<CPathRouter> buspathrouter = std::make_shared<CDijkstraPathRouter>();
    std::shared_ptr<CPathRouter> bikepathrouter = std::make_shared<CDijkstraPathRouter>();
    std::shared_ptr<CBusSystemIndexer> bushelper;
    double defaultbikespeed;
    double defaultwalkspeed;
    double busstoptime ;
    double defaultbusspeed ;
    std::vector<TNodeID> bikestop;

    
    std::size_t NodeCount()const noexcept{
        return streetmap->NodeCount();
    }

    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept{
        if (index >orderednodes.size()){
                return nullptr;
              }
        auto stop = orderednodes[index];
                return stop;
    }

    int searchforindex(std::vector <std::shared_ptr<CStreetMap::SNode>> orderednodes, TNodeID id){
        for (int i = 0; i < orderednodes.size(); i++){
            if(orderednodes[i] ->ID() == id){
                return i;
            }
        }
        return -1;
    }

    int searchforindexbus(std::vector <std::shared_ptr<CBusSystem::SStop>> stops, TNodeID id){
        for (int i = 0; i < stops.size(); i++){
            if(stops[i] ->NodeID() == id){
                return i;
            }
        }
        return -1;
    }


    int searchforindexbike(std::vector<TNodeID> bike, TNodeID id){
        for (int i = 0; i < bike.size(); i++){
            if(bike[i] == id){
                return i;
            }
        }
        return -1;
    }

    void checkuniquebikenode(TNodeID node) {
        if (std::find(bikestop.begin(), bikestop.end(), node) == bikestop.end()) {
            bikestop.push_back(node);
        }
    }

    void build_bikegraph(){
        for (int i=0; i < streetmap->WayCount(); i++){
             auto way = streetmap->WayByIndex(i);
             if (way->HasAttribute("bicycle") && way->GetAttribute("bicycle") == "no") {
                 continue; 
             }

            for (int j = 0; j < way->NodeCount(); j++) {
                TNodeID nodeID = way->GetNodeID(j);
                checkuniquebikenode(nodeID); // Check and add node if unique
            }
        }
        for (TNodeID nodeID : bikestop) {
            bikepathrouter->AddVertex(streetmap->NodeByID(nodeID)); //successfully add bikenodes into the vectors
        }


        for (int i = 0; i< streetmap ->WayCount();i++){
            bool bi = true;
            if (streetmap->WayByIndex(i)->HasAttribute("bicycle") && streetmap->WayByIndex(i)->GetAttribute("bicycle") == "no") {
                 continue; 
             }
            if (streetmap->WayByIndex(i)->HasAttribute("oneway") && streetmap->WayByIndex(i)-> GetAttribute("oneway") == "yes"){
                bi = false;
            }
            for (int j = 0; j< streetmap->WayByIndex(i)->NodeCount()-1; j++){
                TNodeID srcnodeid = streetmap -> WayByIndex(i)->GetNodeID(j);
                int srcvertexid = searchforindexbike(bikestop,srcnodeid);
                TNodeID destnodeid = streetmap -> WayByIndex(i)->GetNodeID(j+1);
                int destvertexid = searchforindexbike(bikestop,destnodeid);
                double distance = SGeographicUtils::HaversineDistanceInMiles(streetmap->NodeByID(srcnodeid)->Location(),streetmap->NodeByID(destnodeid)->Location());
                //std::cout<< "bikesrcvertexid " << srcvertexid <<std::endl;
                //std::cout<< "bikedestvertexid " << destvertexid <<std::endl;
                //std::cout<< "status " << bi <<std::endl;
                bikepathrouter->AddEdge(srcvertexid ,destvertexid, distance, bi);
            }
        }
    }

    void build_busgraph(){
        for(int i = 0; i< bushelper->StopCount(); i++){ //push all the stops node into vectors
            stops.push_back(bushelper->SortedStopByIndex(i));//stops for recording bus reflex <-> vertexid
            stopsinnodeid.push_back(bushelper->SortedStopByIndex(i)->NodeID());//store the bus stop in node id
            buspathrouter->AddVertex(bushelper-> SortedStopByIndex(i)); //add vertex into bus router
            
        }

        for(int i=0 ; i< bushelper-> RouteCount();i++){
            for (int j=0 ; j<bushelper->SortedRouteByIndex(i)->StopCount()-1;j++){
                TNodeID stopid = bushelper->SortedRouteByIndex(i)->GetStopID(j);//this is stopid
                auto  stopstruct = bussystem->StopByID(stopid);// find stopstruct
                int srcindex = searchforindexbus(stops, stopstruct->NodeID());// find corresspoiding vertexid 
                TNodeID stopid2 = bushelper->SortedRouteByIndex(i)->GetStopID(j+1);
                auto  stopstruct2 = bussystem->StopByID(stopid2);
                int destindex = searchforindexbus(stops, stopstruct2->NodeID());
                double distance = SGeographicUtils::HaversineDistanceInMiles(streetmap->NodeByID(stopstruct->NodeID())->Location(),streetmap->NodeByID(stopstruct2->NodeID())->Location());//calculate distance
                buspathrouter->AddEdge(srcindex,destindex,distance);// add edge
            }
        }
    }



    void build_graph(){
        std::vector<int> srcVertexIds;
        std::vector<int> destVertexIds;
        std::vector<double> distances;
        std::vector<bool> biDirectionals;
        for (int i = 0; i< orderednodes.size(); i++){
                pathrouter->AddVertex(orderednodes[i]);
            
        }  
        for (int i = 0; i< streetmap ->WayCount();i++){
            bool bi = true;
            if (streetmap->WayByIndex(i)->HasAttribute("oneway") && streetmap->WayByIndex(i)-> GetAttribute("oneway") == "yes"){
                bi = false;
            }
            for (int j = 0; j< streetmap->WayByIndex(i)->NodeCount()-1; j++){
                TNodeID srcnodeid = streetmap -> WayByIndex(i)->GetNodeID(j);
                int srcvertexid = searchforindex(orderednodes,srcnodeid);
                
                TNodeID destnodeid = streetmap -> WayByIndex(i)->GetNodeID(j+1);
                int destvertexid = searchforindex(orderednodes,destnodeid);
                double distance = SGeographicUtils::HaversineDistanceInMiles(streetmap->NodeByID(srcnodeid)->Location(),streetmap->NodeByID(destnodeid)->Location());
                pathrouter->AddEdge(srcvertexid ,destvertexid, distance, bi);
            }
        }

    }

    double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
        path.clear();
        std::vector<TNodeID> tempPath; 
        if(streetmap->NodeCount() == 0 || streetmap->WayCount() == 0){
            return CPathRouter::NoPathExists;
        }
        int srctarget =  searchforindex(orderednodes,src);
        int destarget =  searchforindex(orderednodes,dest);
        double result = pathrouter->FindShortestPath(srctarget, destarget, path);
        
        for (auto index : path) {
            tempPath.push_back(orderednodes[index]->ID());
        }
        path = tempPath;
        return result;
   }

    double FindShortestBusPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path2){
        path2.clear();
        std::vector<TNodeID> tempPath; 
        int srctarget =  searchforindexbus(stops,src);
        int destarget =  searchforindexbus(stops,dest);
        double result =  buspathrouter->FindShortestPath(srctarget, destarget, path2);
        
        for (auto index : path2) {
            tempPath.push_back(stops[index]->NodeID());
        }
        path2 = tempPath;
        return result;
    }

    double FindShortestBikePath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path3){
        path3.clear();
        std::vector<TNodeID> tempPath; 
        int srctarget =  searchforindexbike(bikestop,src);
        //std::cout << "srctarget" << srctarget << std::endl;
        int destarget =  searchforindexbike(bikestop,dest);
        //std::cout << "desttarget" << destarget << std::endl;
        double result =  bikepathrouter->FindShortestPath(srctarget, destarget, path3);
        
        for (auto index : path3) {
            tempPath.push_back(stops[index]->NodeID());
        }
        path3 = tempPath;
        return result;
    }

    
    std::tuple<TNodeID, double, std::vector<TNodeID>> findNearestBusStopfromsrcAndDistance(TNodeID src, const std::vector<TNodeID>& stopsInNodeId) {
        TNodeID nearestStopId;
        double nearestDistance = std::numeric_limits<double>::max();
        std::vector<TNodeID> nearestPath;

        for (const TNodeID& stopId : stopsInNodeId) {
            std::vector<TNodeID> pathToCurrentBusStop;
            double currentDistance = FindShortestPath(src, stopId, pathToCurrentBusStop);
            if (currentDistance == 0.0) {
                return std::make_tuple(stopId, 0.0, pathToCurrentBusStop);
            }

            if (currentDistance < nearestDistance) {
                nearestDistance = currentDistance;
                nearestStopId = stopId;
                nearestPath = pathToCurrentBusStop;
            }
        }

        return std::make_tuple(nearestStopId, nearestDistance, nearestPath);
    }

    std::tuple<TNodeID, double, std::vector<TNodeID>> findNearestBusStopfromdestAndDistance(TNodeID dest, const std::vector<TNodeID>& stopsInNodeId) {
        TNodeID nearestStopId;
        double nearestDistance = std::numeric_limits<double>::max();
        std::vector<TNodeID> nearestPath;

        for (const TNodeID& stopId : stopsInNodeId) {
            std::vector<TNodeID> pathToCurrentBusStop;
            double currentDistance = FindShortestPath(stopId, dest, pathToCurrentBusStop);
            if (currentDistance == 0.0) {
                return std::make_tuple(stopId, 0.0, pathToCurrentBusStop);
            }

            if (currentDistance < nearestDistance) {
                nearestDistance = currentDistance;
                nearestStopId = stopId;
                nearestPath = pathToCurrentBusStop;
            }
        }

    return std::make_tuple(nearestStopId, nearestDistance, nearestPath);
    }
    
    
    double convertMPHtoNumeric(const std::string& speedStr) { //convert "20 mph into 20"
        int spaceposition = speedStr.find(" ");
        std::string numberPart = speedStr.substr(0, spaceposition);
        double speed = std::stod(numberPart);
        return speed;
    }
    
    
    double getSpeedLimitforway(TNodeID start, TNodeID end){
        double speed = defaultbusspeed;
        for (int i= 0; i < streetmap->WayCount();i++){
            bool startFound = false, endFound = false;
            for (int j =0; j < streetmap->WayByIndex(i)->NodeCount()-1;j++){
                if (streetmap->WayByIndex(i)->GetNodeID(j) == start){
                    startFound = true;
                }
                if (streetmap->WayByIndex(i)->GetNodeID(j) == end){
                    endFound = true;
                }
            }
            if (startFound && endFound){
                speed = convertMPHtoNumeric(streetmap->WayByIndex(i)->GetAttribute("maxspeed"));
            }
        }
        return speed;
    }
    
    double calclutebustraveltime(TNodeID src, TNodeID dest, std::vector< TTripStep > &travelbybus){
        std::vector<TNodeID> buspath;
        
        //walk to the nerest busstop
        std::tuple<TNodeID, double, std::vector<TNodeID>> busstopnearsrc = findNearestBusStopfromsrcAndDistance(src,stopsinnodeid);
        for (auto node : std::get<2>(busstopnearsrc)) { //record the walknode
            travelbybus.push_back(TTripStep(ETransportationMode::Walk, node));
        }
        double timewalktosrc = std::get<1>(busstopnearsrc)/defaultwalkspeed;
        //walk to the destination
        std::tuple<TNodeID, double, std::vector<TNodeID>>  busstopneardest = findNearestBusStopfromdestAndDistance(dest,stopsinnodeid);
        double timewalktodest = std::get<1>(busstopneardest)/defaultwalkspeed;
        // this find the shortest path for the bus route and return the distance
        for(int i=1; i< std::get<2>(busstopneardest).size();i++){
            travelbybus.push_back(TTripStep(ETransportationMode::Walk, std::get<2>(busstopneardest)[i]));
        }
        double bustraveldistance = FindShortestBusPath(std::get<0>(busstopnearsrc),std::get<0>(busstopneardest),buspath);
        for (int i = 1; i<buspath.size() ;i++){//start from 1 since it has repeate nodes with
            travelbybus.push_back(TTripStep(ETransportationMode::Bus, buspath[i]));
        }
        double timeonbus = 0;
        for (int i= 0; i< buspath.size()-1; i++ ){
            double speed = getSpeedLimitforway(buspath[i],buspath[i+1]);
            double distance = SGeographicUtils::HaversineDistanceInMiles(streetmap->NodeByID(buspath[i])->Location(),streetmap->NodeByID(buspath[i+1])->Location());
            timeonbus += distance/speed;
        }
        return timeonbus + timewalktodest + timewalktodest + (busstoptime*(buspath.size()-1)/ 3600.0);

    }


    double calculatebycicletime(TNodeID src, TNodeID dest, std::vector< TTripStep > &travelbybike){
        std::vector<TNodeID> bikepath;
        std::tuple<TNodeID, double, std::vector<TNodeID>> bikestopnearsrc = findNearestBusStopfromsrcAndDistance(src,bikestop);
        for (int i = 0; i< std::get<2>(bikestopnearsrc).size()-1; i++) { //record the walknode do not need the last one
            travelbybike.push_back(TTripStep(ETransportationMode::Walk, std::get<2>(bikestopnearsrc)[i]));
        }
        double timewalktosrc = std::get<1>(bikestopnearsrc)/defaultwalkspeed;
        std::tuple<TNodeID, double, std::vector<TNodeID>>  bikestopneardest = findNearestBusStopfromdestAndDistance(dest,bikestop);
        for (int i = 1; i< std::get<2>(bikestopneardest).size(); i++) { //record the walknode do not need the first one
            travelbybike.push_back(TTripStep(ETransportationMode::Walk, std::get<2>(bikestopneardest)[i]));
        }
        double timewalktodest = std::get<1>(bikestopneardest)/defaultwalkspeed;
        double biketraveldistance = FindShortestBikePath(std::get<0>(bikestopnearsrc),std::get<0>(bikestopneardest),bikepath);
        for (auto node : bikepath) { //record the walknode
            travelbybike.push_back(TTripStep(ETransportationMode::Bike, node));
        }
        //std::cout<< "biketraveldistanve " << biketraveldistance<< std::endl;
        double timeonbike = biketraveldistance/defaultbikespeed;
        return timeonbike + timewalktodest + timewalktosrc;

    }
    
    
    double calclutewalktraveltime(TNodeID src, TNodeID dest, std::vector< TTripStep > &travlebyfoot){// walk form src to dest via foot
        std::vector<TNodeID> usualpath;
        double walktraveldistance = FindShortestPath(src,dest,usualpath);
        double time = walktraveldistance/defaultwalkspeed;
        for (auto node : usualpath){
            travlebyfoot.push_back(TTripStep(ETransportationMode::Walk,node));
        }
        return time;
    }




    double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path){
        std::vector<TTripStep> walkPath, bikePath, busPath;
        if(streetmap->NodeCount() == 0 || streetmap->WayCount() == 0){
            return CPathRouter::NoPathExists;
        }
        double walktime =  calclutewalktraveltime(src,dest,walkPath);
        double biketime =  calculatebycicletime(src,dest,bikePath);
        double bustime = calclutebustraveltime(src,dest,busPath);
        double mintime = std::min({walktime, biketime, bustime});
        if (mintime == walktime){
            path = walkPath;
        }
        else if (mintime == biketime){
            path = bikePath;
        }
        else{
            path = busPath;
        }
        return mintime;
    }
};



CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation-> config = config;
    DImplementation ->streetmap = config->StreetMap();
    DImplementation ->bussystem = config->BusSystem();
    DImplementation -> defaultbikespeed = config ->BikeSpeed();
    DImplementation -> defaultwalkspeed = config ->WalkSpeed();
    DImplementation -> busstoptime = config -> BusStopTime();
    DImplementation -> defaultbusspeed = config ->DefaultSpeedLimit();
    DImplementation ->bushelper = std::make_shared<CBusSystemIndexer>(DImplementation->bussystem);
    for (std::size_t i = 0; i < DImplementation->streetmap->NodeCount(); i++) {
        auto node = DImplementation->streetmap->NodeByIndex(i);
        DImplementation-> orderednodes.push_back(node);}

    std::sort(DImplementation->orderednodes.begin(), DImplementation->orderednodes.end(), //https://stackoverflow.com/questions/2758080/how-to-sort-an-stl-vector
    [](const std::shared_ptr<CStreetMap::SNode>& a, const std::shared_ptr<CStreetMap::SNode>& b) {
            return a->ID() < b->ID();
        });

    DImplementation->build_graph();
    DImplementation->build_busgraph();
    DImplementation->build_bikegraph();

}



CDijkstraTransportationPlanner:: ~CDijkstraTransportationPlanner(){


}



std::size_t CDijkstraTransportationPlanner::NodeCount()const noexcept{
    return DImplementation->NodeCount();
}



std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept{

    return DImplementation->SortedNodeByIndex(index);
}


double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
    return DImplementation->FindShortestPath(src,dest,path);
}

double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path){
    return DImplementation->FindFastestPath(src,dest,path);
}

bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const{
    return true;
}