#include "DijkstraTransportationPlanner.h"
#include <algorithm>


struct CDijkstraTransportationPlanner::SImplementation{
    std::shared_ptr<SConfiguration> config;
    std::shared_ptr<CStreetMap> streetmap;
    std::shared_ptr<CBusSystem> bussystem;
    std::shared_ptr<CPathRouter> pathrouter;
    std::vector <std::shared_ptr<CStreetMap::SNode>> orderednodes;

    
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


   double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
         //第一步 你要加点 用addvertex,参数any 就应该是nodestruct 这时候会返回vertexid, 每一个vertexid 对应一个 nodestruct nodestruct里又有nodeid
         //然后你要根据waystruct 里的信息 addedge 肯定是要用循环 way 不是有node吗 
         //然后你要考虑那个way属性 是单边还是双边 如果是双边 你得被addedge里 bi 参数打开 你看一下 讲义里的很多assumption
         //*** 这个时候图差不多就构造好了但是findshortespath返回的是vertexid 不是nodeid 等于说你得把vector里的东西转换回去 用gettag,得到那个节点 然后 节点->nodeid(),然后塞进这个path里
         //vertexid 就是按你插入顺序 第一个是0 第二个是1
        return pathrouter->FindShortestPath(src,dest,path);
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

}

double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path){

}

bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const{

}