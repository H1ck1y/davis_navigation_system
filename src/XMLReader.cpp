#include "XMLReader.h"
#include "StringUtils.h"
#include <expat.h>
#include <iostream>
#include <queue>

struct CXMLReader::SImplementation{
    std::shared_ptr<CDataSource> src;
    std::queue<SXMLEntity> buffer;
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) {
        DImplementation = std::make_unique<SImplementation>();
        DImplementation->src = src;
        DImplementation->buffer = {};
}

CXMLReader::~CXMLReader() {}

bool CXMLReader::End() const {
    return DImplementation->src->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    std::string xmlString;
    while (!End()){ // generate the xmlstring
        char currentChar;
        DImplementation->src->Get(currentChar);
        xmlString += currentChar;
    }

    XML_Parser parser = XML_ParserCreate(nullptr); //initialize the pharser

    void* userData = &(DImplementation->buffer); //create the userdata

    XML_SetUserData(parser, userData);

    auto StartElementHandler = [](void *userData, const XML_Char *name, const XML_Char **atts){ // Define the start element
        auto buffer = static_cast<std::queue<SXMLEntity>*>(userData);
        SXMLEntity bufferEntity;
        bufferEntity.DType = SXMLEntity::EType::StartElement;
        bufferEntity.DNameData = name;

        for (int i = 0; atts[i] != nullptr; i += 2) { // increment 2 to guarantee we read a pair each time
            bufferEntity.SetAttribute(atts[i], atts[i + 1]);
        }
        buffer->push(bufferEntity);
    };

    auto CharacterDataHandler = [](void *userData, const XML_Char *s, int len){ // Define the char element
        auto buffer = static_cast<std::queue<SXMLEntity>*>(userData);
        SXMLEntity bufferEntity;
        bufferEntity.DType = SXMLEntity::EType::CharData;
        bufferEntity.DNameData.append(s, len);
        buffer->push(bufferEntity);
    };

    auto EndElementHandler = [](void *userData, const XML_Char *name){ // Define the end element
        auto buffer = static_cast<std::queue<SXMLEntity>*>(userData);
        SXMLEntity bufferEntity;
        bufferEntity.DType = SXMLEntity::EType::EndElement;
        bufferEntity.DNameData = name;

        buffer->push(bufferEntity);
    };




    //https://blog.csdn.net/qq_34759481/article/details/81184437 

    XML_SetElementHandler(parser,StartElementHandler,EndElementHandler);//  set up the call back function end & start
    XML_SetCharacterDataHandler(parser,CharacterDataHandler); //set up the call back funtion chardata
    XML_Parse(parser, xmlString.c_str(), xmlString.length(), XML_TRUE); //pass the xmlstring into pharser
    XML_ParserFree(parser); // release the parser
    
    if (DImplementation->buffer.empty()) {
        return false; 
    }
    while (!DImplementation->buffer.empty()) {
        const auto &top = DImplementation->buffer.front();
        if (skipcdata && top.DType == SXMLEntity::EType::CharData) {
            DImplementation->buffer.pop();
            continue;
        }
        entity = top;
        DImplementation->buffer.pop();
        return true; 
    }
    return false;
}
