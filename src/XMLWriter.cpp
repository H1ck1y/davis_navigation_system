#include "XMLWriter.h" 
#include "StringUtils.h"
#include "stack"


struct CXMLWriter::SImplementation{
    std::shared_ptr< CDataSink > sink;
    std::stack<std::string> checker;
    bool  firstwrite;

};


CXMLWriter::CXMLWriter(std::shared_ptr< CDataSink > sink){
    DImplementation  = std::make_unique<SImplementation>();
    DImplementation-> sink = sink;
    DImplementation ->checker = {};
}


CXMLWriter::~CXMLWriter(){}


bool CXMLWriter::Flush(){
    std::string xmlstring; // the left element in the stack should be inputed
    std::vector <char> container = {};
    while(!DImplementation->checker.empty()){
        xmlstring += "</" + DImplementation->checker.top() + ">";
        DImplementation ->checker.pop();
    }
    for (int i = 0; i<xmlstring.length();i++){//saperate xmlstring into the vector write it into the sink
            container.push_back(xmlstring[i]);
    }
    DImplementation -> sink -> Write(container);
    return true;
}


bool  CXMLWriter::WriteEntity(const SXMLEntity &entity){
    std::string xmlstirng;
    std::string attribute ="";
    std::vector <char> container = {};

    auto dealWithSpecial = [](const std::string &attribute) -> std::string{// got the idea of lambda function by asking gpt
        std::string result;
        for (int i = 0; i < attribute.length(); i++){ // escape character need to be converted
            char character = attribute[i];
            if (character == '\"') {
                result += "&quot;";} 
            else if (character == '&') {
                result += "&amp;";} 
            else if (character == '<') {
                result += "&lt;";} 
            else if (character== '>') {
                result += "&gt;";} 
            else if (character == '\'') {
                result += "&apos;";} 
            else {
                result += character;}
        }
    return result;};

    
    if(entity.DType == SXMLEntity::EType::EndElement){ 
        if (DImplementation->checker.empty()||  //cannot input end element in the first
            entity.DNameData != DImplementation ->checker.top()|| // end element mush match
            !entity.DAttributes.empty()){ // end elemeent cannot have attributes
            return false;
        }else{
            DImplementation ->checker.pop();
            xmlstirng = "</" + dealWithSpecial(entity.DNameData) + ">";
        }
    }// check for end first
    else if (entity.DType == SXMLEntity::EType::StartElement)
    {
        for(int i=0; i < entity.DAttributes.size() ;i++){
            attribute += " " + dealWithSpecial(entity.DAttributes[i].first) + "=\"" + dealWithSpecial(entity.DAttributes[i].second) + "\"";
            xmlstirng = "<" + dealWithSpecial(entity.DNameData)+ attribute + ">";
        }
        DImplementation->checker.push(entity.DNameData);// need to push the name into stack
        attribute = "";
    }//consider the start
    else if(entity.DType == SXMLEntity::EType::CompleteElement){ // deal with complete element
        for(int i=0; i < entity.DAttributes.size() ;i++){
            attribute += " " + dealWithSpecial(entity.DAttributes[i].first) + "=\"" + dealWithSpecial(entity.DAttributes[i].second) + "\"";
            xmlstirng = "<" + dealWithSpecial(entity.DNameData)+ attribute + "/>";
        }
        attribute = "";
    }else{ // deal with chardata
        if(!entity.DAttributes.empty()){
            return false;
        }else{
        xmlstirng = dealWithSpecial(entity.DNameData);}

    }

    for (int i = 0; i<xmlstirng.length();i++){//saperate xmlstring into the vector write it into the sink
        container.push_back(xmlstirng[i]);
    
    }
    DImplementation -> sink -> Write(container);
    return true;
}

