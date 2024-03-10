# include "BusSystemIndexer.h"
#include <vector>
#include <algorithm>


struct CBusSystemIndexer::SImplementation{
   std::shared_ptr<CBusSystem> bussystem;
   std::vector <std::shared_ptr<CBusSystemIndexer::SStop>> orderedstops;
   std::vector <std::shared_ptr<CBusSystemIndexer::SRoute>> orderedroutes;

};




CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation-> bussystem = bussystem;
    for (std::size_t i = 0; i < bussystem->StopCount(); i++) {
        auto stop = bussystem->StopByIndex(i);
        DImplementation-> orderedstops.push_back(stop);} // put all of the stops into orederedstops
    

    std::sort(DImplementation->orderedstops.begin(), DImplementation->orderedstops.end(), //https://stackoverflow.com/questions/2758080/how-to-sort-an-stl-vector
    [](const std::shared_ptr<CBusSystemIndexer::SStop>& a, const std::shared_ptr<CBusSystemIndexer::SStop>& b) {
            return a->ID() < b->ID(); // this is the lamda function used to sort nodes based on their node ids
        });
    

    for (std::size_t i = 0; i < bussystem->RouteCount(); i++) {
        auto route = bussystem->RouteByIndex(i);    
        DImplementation-> orderedroutes.push_back(route);}  // get all the routes and put it into the container

    
    std::sort(DImplementation->orderedroutes.begin(), DImplementation->orderedroutes.end(), //https://stackoverflow.com/questions/2758080/how-to-sort-an-stl-vector
    [](const std::shared_ptr<CBusSystemIndexer::SRoute>& a, const std::shared_ptr<CBusSystemIndexer::SRoute>& b) {
            return a->Name() < b->Name();// this is the lamda function used to sort nodes based on their route strings 
        });

}


CBusSystemIndexer:: ~CBusSystemIndexer(){

}

std::size_t  CBusSystemIndexer::StopCount() const noexcept{
    // this is for calculating the total stop counts
    std::size_t stopc = DImplementation ->bussystem ->StopCount();
    return stopc;

}


std::size_t  CBusSystemIndexer::RouteCount() const noexcept{
    // this is for calculating all the routes
    std::size_t routec = DImplementation ->bussystem ->RouteCount();
    return routec;

}

std::shared_ptr<CBusSystemIndexer::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept{
    // this will get the nodes based on the index in the container
    if (index > DImplementation-> orderedstops.size()){
        return nullptr;
    }
    auto stop = DImplementation->orderedstops[index]; // stored in the map, directly acceess it 
    return stop;
}

std::shared_ptr<CBusSystemIndexer::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept{
     // this will get the routes based on the index in the container
    if (index > DImplementation-> orderedroutes.size()){
        return nullptr;
    }
    auto route = DImplementation->orderedroutes[index];// stored in the map, directly acceess it 
    return route;

}


std::shared_ptr<CBusSystemIndexer::SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept{
    // this use linear way to find node id of each stop
    for (int i = 0; i < DImplementation->orderedstops.size(); i++){
        if (id == DImplementation->orderedstops[i]->NodeID()){// get each stop first and get its node id
            auto node = DImplementation->orderedstops[i];
            return node;
        }
    }
    return nullptr;
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept{
    auto srcnode = CBusSystemIndexer::StopByNodeID(src);
    auto destnode = CBusSystemIndexer::StopByNodeID(dest);
    auto srcstop = srcnode->ID();
    auto deststop = destnode->ID();

    bool found = false;
    for (int i = 0 ; i < DImplementation->orderedroutes.size(); i++){
        bool srccheck = false;
        bool destcheck = false;
        for(int j = 0; j< DImplementation->orderedroutes[i]->StopCount();j++){// this will check if current routes contains src
            if (DImplementation->orderedroutes[i]->GetStopID(j) == srcstop){
                srccheck = true;
            }
            if (DImplementation->orderedroutes[i]->GetStopID(j) == deststop){// this will check if current routes contain dest
                destcheck = true;
            }
      
        }
        if (srccheck && destcheck){// if both contains then find it 
                routes.insert(DImplementation->orderedroutes[i]);
                found = true;
            }

    }
    return found;
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept{
    std::unordered_set<std::shared_ptr<SRoute>> container;
    return RoutesByNodeIDs(src, dest, container); // simply called previoud function

}








