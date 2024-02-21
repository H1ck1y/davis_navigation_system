#include "OpenStreetMap.h"
#include "map"
#include <iostream>

struct COpenStreetMap::SImplementation{
    std::shared_ptr<CXMLReader> src;
    struct SSNode : public CStreetMap::SNode{
            TNodeID   nodeid;
            TLocation nodelocation;
            int attrnumber = 0;
            std::map<std::string, std::string> attr_contaniner;
            
            TNodeID ID() const noexcept override {
                return nodeid;
            }
            TLocation Location() const noexcept override {
                return nodelocation;
            }
            std::size_t AttributeCount() const noexcept override {
                return attrnumber;
            }
            std::string GetAttributeKey(std::size_t index) const noexcept override { 
                if (attr_contaniner.size() <= index){    
                    return "";
                }else{
                    auto pair = attr_contaniner.begin();//https://stackoverflow.com/questions/7856453/accessing-map-value-by-index
                    std::advance(pair, index);//map does not have the index, so use this way to go foward
                    return pair->first;

                }
            }
            bool HasAttribute(const std::string &key) const noexcept override {
                if (attr_contaniner.count(key) == 0){ // this can check if it has the key we need
                return false;
                }else{
                    return true;
                }
            }

            std::string GetAttribute(const std::string &key) const noexcept override {
                auto pair = attr_contaniner.find(key);// in the map search the key for the corresponding pair
                if (pair == attr_contaniner.end()){
                    return "";
                }else{
                    return pair->second;
                } 
            }
    };
    

    struct SSWay: public CStreetMap::SWay{
            TWayID wayid;
            std::vector<TNodeID> nodeids; 
            int attrnumber = 0;
            std::multimap<std::string, std::string> attr_container;
            
            TWayID ID() const noexcept override {
                return wayid;
            }
            std::size_t NodeCount() const noexcept override {
                return nodeids.size();
            }
            TNodeID GetNodeID(std::size_t index) const noexcept override {
                if (index < nodeids.size()) {
                    return nodeids[index]; 
                }else{
                    return InvalidNodeID;}
            
            }

            std::size_t AttributeCount() const noexcept override {
                return attrnumber;
            }
            
            std::string GetAttributeKey(std::size_t index) const noexcept override {
                if (attr_container.size() <= index){    
                    return "";
                }else{
                    auto pair = attr_container.begin();//https://stackoverflow.com/questions/7856453/accessing-map-value-by-index
                    std::advance(pair, index);
                    return pair->first;

                }
            }
            
            bool HasAttribute(const std::string &key) const noexcept override {
                if (attr_container.count(key) == 0){ 
                    return false;
                }else{
                    return true;
                }
            }
            
            std::string GetAttribute(const std::string &key) const noexcept override {
                std::string result;
                auto pair = attr_container.find(key);
                if (pair == attr_container.end()){
                    return "";
                }else{
                    return pair->second;
                } ;
            }
    };
    std::vector<std::shared_ptr<SSNode>> nodes; // use this to hold all node structs
    std::vector<std::shared_ptr<SSWay>> ways; //use this to hold all way structs
    

    std::shared_ptr<SSNode> CreateSimpleNode(SXMLEntity& entity){
        auto new_node = std::make_shared<SSNode>();
        /*for (size_t i = 0; i < entity.DAttributes.size(); ++i) {
            const auto& attribute = entity.DAttributes[i];
            new_node->attr_contaniner[attribute.first] = attribute.second;
        }*/
        new_node->nodeid = std::stoull(entity.DAttributes[0].second); // get the nodeid
        new_node->nodelocation = std::make_pair(stod(entity.DAttributes[1].second),stod(entity.DAttributes[2].second)); //get the location
        return new_node;
    }


    void add_attr(SXMLEntity& entity, std::shared_ptr<SImplementation::SSNode> node){ //deal with nested xml tag
            std::string key = entity.DAttributes[0].second; // this is for dealing with elements in tags
            std::string value = entity.DAttributes[1].second;
            node->attr_contaniner[key] = value;
            node->attrnumber += 1;
    }

    std::shared_ptr<SSWay> CreateSimpleWay(SXMLEntity& entity){ // this is for the way struct
        auto new_way = std::make_shared<SSWay>();
        new_way -> wayid = std::stoull(entity.DAttributes[0].second);
        return new_way;
    }
    
    void add_attr2(SXMLEntity& entity, std::shared_ptr<SImplementation::SSWay> way){ //deal with nested xml tag
            std::string key = entity.DAttributes[0].second;
            std::string value = entity.DAttributes[1].second;
            way->attr_container.insert(std::make_pair(key, value));
            way ->attrnumber += 1;
    }
    
    void dealwith_node_inway(SXMLEntity& entity,std::shared_ptr<SImplementation::SSWay> way,std::vector<TNodeID> &nodes){
            nodes.push_back(std::stoull(entity.DAttributes[0].second));// deal with nd tag
    }
};


COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src) {
    DImplementation = std::make_unique<SImplementation>();
    DImplementation-> src = src;
    std::shared_ptr<SImplementation::SSNode> a_node = nullptr;
    std::shared_ptr<SImplementation::SSWay> a_way = nullptr;
    bool node = false; //check which struct is processing since we need to add some attributes for nested xmlstring
    bool way = false;
    SXMLEntity entity;
    while(DImplementation->src->ReadEntity(entity)){ // while read the end of the entity
        if (entity.DType == SXMLEntity::EType::StartElement) {
            if (entity.DNameData == "node") {
                a_node = DImplementation->CreateSimpleNode(entity);
                node = true;
                way = false; // Reset way flag
            } else if (entity.DNameData == "way") {
                way = true;
                node = false; 
                a_way = DImplementation->CreateSimpleWay(entity);

            } else if (node) {
                DImplementation->add_attr(entity,a_node);
            } else if (way) { // this is for solving node cases 
                if(entity.DNameData == "nd"){
                    DImplementation->dealwith_node_inway(entity,a_way,a_way->nodeids);
                }else{// this is for solving 
                    DImplementation->add_attr2(entity,a_way);
                }
            }
        } else if (entity.DType == SXMLEntity::EType::EndElement) {// push the nodes or ways into vectors while only reaching the end
            if (a_node && entity.DNameData == "node") {
                node = false; // finish pushing change the processing statuse into false
                DImplementation->nodes.push_back(a_node);
                a_node = nullptr;
            } else if (a_way && entity.DNameData == "way") {
                way = false;
                DImplementation->ways.push_back(a_way);
                a_way = nullptr;
            }
       
        }

    }
}



COpenStreetMap::~COpenStreetMap(){
}
   


std::size_t COpenStreetMap::NodeCount() const noexcept {
    return  DImplementation -> nodes.size();
}



std::size_t  COpenStreetMap::WayCount() const noexcept {
    return DImplementation -> ways.size();
}

std::shared_ptr<CStreetMap::SNode>  COpenStreetMap::NodeByIndex(std::size_t index) const noexcept { 
    if(index >= DImplementation->nodes.size()) {
        return nullptr;
    }
     return DImplementation->nodes[index];
}
        
std::shared_ptr<CStreetMap::SNode>  COpenStreetMap::NodeByID(TNodeID id) const noexcept {
    for(int i = 0; i< DImplementation->nodes.size();i++){
        if (DImplementation-> nodes[i]-> nodeid == id){
            return DImplementation->nodes[i];
        }
    }
    return nullptr;
}
        
std::shared_ptr<CStreetMap::SWay>   COpenStreetMap::WayByIndex(std::size_t index)const noexcept {
    if(index >= DImplementation->ways.size()) {
        return nullptr;
    }
    return DImplementation->ways[index];
}

        

std::shared_ptr<CStreetMap::SWay>   COpenStreetMap::WayByID(TWayID id) const noexcept {
    for(int i = 0; i< DImplementation->ways.size();i++){ // use linear way to search
        if (DImplementation-> ways[i]-> wayid == id){
            return DImplementation->ways[i];
        }
    }
    return nullptr;
}

