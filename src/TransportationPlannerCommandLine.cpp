#include "TransportationPlannerCommandLine.h"
#include <queue>
#include <iostream>
#include "StringUtils.h"
#include "GeographicUtils.h"

struct CTransportationPlannerCommandLine:: SImplementation{
    std::shared_ptr<CDataSource> cmdsrc;
    std::shared_ptr<CDataSink> outsink;
    std::shared_ptr<CDataSink> errsink;
    std::shared_ptr<CDataFactory> results;
    std::shared_ptr<CTransportationPlanner> planner;
    std::queue<std::string> buffer;

    std::vector<char> convertstringintovector(std::string info){
        std::vector<char> charVector;
        for (char ch: info){
            charVector.push_back(ch);
        }
        return charVector;
    }

    bool ProcessCommands(){
        outsink->Put('>');
        outsink->Put(' ');
        while (!buffer.empty()) {
            std::string command = buffer.front();
            buffer.pop(); 
            if (command == "exit") {
                continue; 
            }
            else if (command == "help") {
                std::string information = "------------------------------------------------------------------------\nhelp     Display this help menu\nexit     Exit the program\ncount    Output the number of nodes in the map\nnode     Syntax \"node [0, count)\" \n         Will output node ID and Lat/Lon for node\nfastest  Syntax \"fastest start end\" \n         Calculates the time for fastest path from start to end\nshortest Syntax \"shortest start end\" \n         Calculates the distance for the shortest path from start to end\nsave     Saves the last calculated path to file\nprint    Prints the steps for the last calculated path\n";
                outsink->Write(convertstringintovector(information));
                outsink->Put('>');
                outsink->Put(' ');
            }
            else if (command == "count") {
                int node_count = planner->NodeCount();
                std::string o_string = std::to_string(node_count) + " nodes\n";
                outsink->Write(convertstringintovector(o_string));
                outsink->Put('>');
                outsink->Put(' ');
            }
            else if (command.substr(0, 5) == "node ") {
                std::string node_index_str = command.substr(5);
                int node_index = std::stoi(node_index_str);
                int id = planner->SortedNodeByIndex(node_index)->ID();
                auto location_pair = planner->SortedNodeByIndex(node_index)->Location();
                std::string location = SGeographicUtils::ConvertLLToDMS(location_pair);
                outsink->Write(convertstringintovector("Node " + node_index_str + + ": id = " + std::to_string(id) + " " + location + "\n"));
                outsink->Put('>');
                outsink->Put(' ');
            }
        }
 

   
        return true;
    }

};


CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> cmdsrc, 
                                                                     std::shared_ptr<CDataSink> outsink, 
                                                                     std::shared_ptr<CDataSink> errsink, 
                                                                     std::shared_ptr<CDataFactory> results, 
                                                                    std::shared_ptr<CTransportationPlanner> planner)
{                      DImplementation = std::make_unique<SImplementation>();
                       DImplementation->cmdsrc = cmdsrc;
                       DImplementation->outsink = outsink;
                       DImplementation -> errsink = errsink;
                       DImplementation -> results = results;
                       DImplementation -> planner = planner;

        std::string cammandlinestring;
        std::string currentCommand;
        while (!(DImplementation->cmdsrc->End())){ // generate the commandlinestring
            char currentChar;
            DImplementation->cmdsrc->Get(currentChar);
            cammandlinestring += currentChar;
        }
        for (char ch :cammandlinestring) {
          if (ch == '\n') {
                if (!currentCommand.empty()) {
                    DImplementation->buffer.push(currentCommand);
                    currentCommand.clear(); 
            }
          }else{currentCommand += ch;} //push the command into the buffer
        }

                                                                    }



CTransportationPlannerCommandLine:: ~CTransportationPlannerCommandLine(){

}


 bool CTransportationPlannerCommandLine::ProcessCommands(){
        return DImplementation->ProcessCommands();

 }