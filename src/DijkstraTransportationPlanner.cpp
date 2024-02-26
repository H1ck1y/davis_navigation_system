#include "DijkstraTransportationPlanner.h"
#include <algorithm>
#include "GeographicUtils.h"
#include "iostream"
#include "DijkstraPathRouter.h"


struct CDijkstraTransportationPlanner::SImplementation{
    std::shared_ptr<SConfiguration> config;
    std::shared_ptr<CStreetMap> streetmap;
    std::shared_ptr<CBusSystem> bussystem;    
    std::vector <std::shared_ptr<CStreetMap::SNode>> orderednodes;
    std::shared_ptr<CPathRouter> pathrouter = std::make_shared<CDijkstraPathRouter>();

    
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
    }

   double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
        bool bi = false;
        if(streetmap->NodeCount() == 0 || streetmap->WayCount() == 0){
            return CPathRouter::NoPathExists;
        }
        for (int i = 0; i< orderednodes.size(); i++){
            std::cout << "Ordered Node ID: " << orderednodes[i]->ID() << std::endl;
            pathrouter->AddVertex(orderednodes[i]);
            
        }
        for (int i = 0; i< streetmap ->WayCount();i++){
            if (streetmap->WayByIndex(i)->HasAttribute("oneway") && streetmap->WayByIndex(i)-> GetAttribute("oneway") == "no"){
                bi = true;
            }
            for (int j = 0; j< streetmap->WayByIndex(i)->NodeCount()-1; j++){
                TNodeID srcnodeid = streetmap -> WayByIndex(i)->GetNodeID(j);
                int srcvertexid = searchforindex(orderednodes,srcnodeid);
                std::cout << "srcvertexID: " << srcvertexid << std::endl;
                TNodeID destnodeid = streetmap -> WayByIndex(i)->GetNodeID(j+1);
                int destvertexid = searchforindex(orderednodes,destnodeid);
                 std::cout << "destvertexID: " << destvertexid << std::endl;
                double distance = SGeographicUtils::HaversineDistanceInMiles(streetmap->NodeByID(srcnodeid)->Location(),streetmap->NodeByID(destnodeid)->Location());
                pathrouter->AddEdge(srcvertexid ,destvertexid, distance, bi);
            }
        }
        int srctarget =  searchforindex(orderednodes,src);
        int destarget =  searchforindex(orderednodes,dest);
        double result = pathrouter->FindShortestPath(srctarget, destarget, path);
        for (int i =0; i < orderednodes.size(); i++){
            path[i] = orderednodes[i]->ID();
        }
        return result;
   }

};



CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation-> config = config;
    DImplementation ->streetmap = config->StreetMap();
    DImplementation ->bussystem = config->BusSystem();
    for (std::size_t i = 0; i < DImplementation->streetmap->NodeCount(); i++) {
        auto node = DImplementation->streetmap->NodeByIndex(i);
        DImplementation-> orderednodes.push_back(node);}

    std::sort(DImplementation->orderednodes.begin(), DImplementation->orderednodes.end(), //https://stackoverflow.com/questions/2758080/how-to-sort-an-stl-vector
    [](const std::shared_ptr<CStreetMap::SNode>& a, const std::shared_ptr<CStreetMap::SNode>& b) {
            return a->ID() < b->ID();
        });

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

}

bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const{

}