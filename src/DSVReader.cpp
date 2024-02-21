#include "DSVReader.h"
#include "StringUtils.h"
#include <iostream>


struct CDSVReader::SImplementation
{
    std::shared_ptr< CDataSource > src;
    char delimiter;
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter) {
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->src = src;
    DImplementation->delimiter = delimiter;
}

CDSVReader::~CDSVReader() {}

bool CDSVReader::End() const {
    return DImplementation->src->End();
}


bool CDSVReader::ReadRow(std::vector< std::string > &row){

    char fisrt_for_get;
    std::string del;
    del = DImplementation->delimiter;
    std::string line = "";
    int countfor_quote = 0;
    
    if (del == "\""){ 
        del = ",";
    }

    while (true){
        if (End()) {
        break;
    }
        DImplementation->src->Get(fisrt_for_get);
        if(fisrt_for_get == '\"'){ //This counter is to keep track the number of " 
            ++countfor_quote;
        }
        if(fisrt_for_get == '\n' && countfor_quote % 2 == 0){ //This is to check whether this newline char is a real one
            break;
        }
        line += fisrt_for_get; //Keep append the current char to end of our line
    }
    if (line == ""){ //No data is in this line
        return false;
    }
    
    row = StringUtils::Split(line,del); //Spilt the line based on our delimiter, the Split() function is overwritten to check the real delimiter

    for(int i = 0; i< row.size(); i++){
        if(row[i].find("\"") != std::string::npos){ //Remove the fake " in the each string of the line
            row[i] = row[i].substr(1,row[i].length()-2);       
        }
        if(row[i].find("\"") != std::string::npos){ //Recover the remaining "" to "
            row[i] = StringUtils::Replace(row[i],"\"\"", "\"");
        }
    }

    return true;
}