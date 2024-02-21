#include "CSVBusSystem.h"
#include <vector>
#include <map>
#include <memory>
#include <iostream>



struct CCSVBusSystem::SImplementation{
    
    struct SSSTOP: public CBusSystem::SStop{
    TStopID stopID;
    CStreetMap::TNodeID nodeID;
    TStopID ID() const noexcept override{
        return stopID;
        
    }
        
    CStreetMap::TNodeID NodeID() const noexcept override{
        return nodeID;

    }

};


    struct SSRoute: public CBusSystem::SRoute{
        std::string routeName;
        std::vector<TStopID> stopIDs;
        std::string Name() const noexcept override{
           return routeName;
        }

        std::size_t StopCount() const noexcept override{
           return stopIDs.size();
        }

        TStopID GetStopID(std::size_t index) const noexcept override{
            if (index > stopIDs.size()){
                return CBusSystem::InvalidStopID;
            }
            else{
                return stopIDs[index];
            }
        }

    };
    
    std::shared_ptr< CDSVReader > stopsrc;
    std::shared_ptr<CDSVReader > routesrc;
    std::vector<std::shared_ptr<CBusSystem::SStop>> stops;
    std::vector<std::shared_ptr<CBusSystem::SRoute>> routes;
    std::map<TStopID, std::shared_ptr<CBusSystem::SStop>> stopcontainer;
    std::map<std::string, std::shared_ptr<CCSVBusSystem::SImplementation::SSRoute>> routecontainer;
    


std::shared_ptr<SSSTOP> CreateStop(std::vector<std::string>& stopCollection){
    auto stopNode = std::make_shared<SSSTOP>();
    stopNode->stopID = std::stoull(stopCollection[0]);
    stopNode->nodeID = std::stoull(stopCollection[1]);
    return stopNode;
}

std::shared_ptr<CBusSystem::SRoute> CreateRoute(std::string route){
    auto  routeNode = std::make_shared<SSRoute>();
    return routeNode;
}

void AddRoute(std::vector<std::string>& routeCollection){
    std::string route_name = routeCollection[0];
    unsigned long long stop_id = std::stoull(routeCollection[1]);

    if (routecontainer.count(route_name) == 0){
        auto routeNode = std::make_shared<SSRoute>();
        routeNode->routeName = route_name;
        routeNode->stopIDs.push_back(stop_id);
        routecontainer.insert(std::make_pair(route_name, routeNode));
    }
    else{
        routecontainer[route_name]->stopIDs.push_back(stop_id);
    }
}

void addstoptothemap(std::shared_ptr<SSSTOP>& stopnode, std::map<TStopID, std::shared_ptr<CBusSystem::SStop>>& stopcontainer){
     TStopID key = stopnode-> stopID;
     std::shared_ptr<SSSTOP> value = stopnode;
     stopcontainer.insert(std::make_pair(key, value));
}


};

CCSVBusSystem::CCSVBusSystem(std::shared_ptr< CDSVReader > stopsrc, std::shared_ptr<CDSVReader > routesrc){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation-> stopsrc = stopsrc;
    DImplementation-> routesrc = routesrc;
    std::vector<std::string> stopCollection;
    std::vector<std::string> routeCollection;
    std::shared_ptr<SImplementation::SSSTOP> a_stopnode = nullptr;
    std::shared_ptr<SImplementation::SSRoute> a_routenode = nullptr;

    stopsrc->ReadRow(stopCollection);//skip the first line

    while (!stopsrc->End()){
        stopsrc->ReadRow(stopCollection);
        a_stopnode = DImplementation->CreateStop(stopCollection);
        DImplementation->stops.push_back(a_stopnode);
        DImplementation->addstoptothemap(a_stopnode,DImplementation->stopcontainer);
        stopCollection.clear();
    }

    routesrc->ReadRow(routeCollection);

    while (!routesrc->End()){
        routesrc->ReadRow(routeCollection);
        DImplementation->AddRoute(routeCollection);
        routeCollection.clear();
    }
}

CCSVBusSystem::~CCSVBusSystem(){}

std::size_t CCSVBusSystem::StopCount() const noexcept {
    auto stopcount = DImplementation->stops.size();
    return stopcount;
}

std::size_t CCSVBusSystem::RouteCount() const noexcept {
    return DImplementation->routecontainer.size();
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept{
    return DImplementation->stops[index];
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept {
    std::shared_ptr<CBusSystem::SStop> node;
    node = DImplementation->stopcontainer.at(id);
    return node;
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept{
    if (index > RouteCount()){
        return nullptr;
    }
    else{
        for (auto it = DImplementation->routecontainer.begin(); it != DImplementation->routecontainer.end(); ++it){
            if (index == 0){
                return it->second;
            }
            index--;
        }
    }
    return nullptr;
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name)const noexcept{
    if (DImplementation->routecontainer.count(name) == 0){
        return nullptr;
    }
    else{
        return DImplementation->routecontainer[name];
    }
}