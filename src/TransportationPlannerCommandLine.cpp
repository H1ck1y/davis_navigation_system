#include "TransportationPlannerCommandLine.h"
#include <queue>
#include <iostream>
#include <cmath>
#include <unordered_map>
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
            std::cout << "command: " << command << std::endl;
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
                outsink->Write(convertstringintovector("Node " + node_index_str + + ": id = " + std::to_string(id) + " is at " + location + "\n"));
                outsink->Put('>');
                outsink->Put(' ');
            }
            else if (command.substr(0, 9) == "shortest "){
                std::string first_str = "";
                std::string second_str = "";
                int start = 9;

                for (; start < command.length(); ++start){
                    if (command[start] == ' '){
                        break;
                    }
                    first_str += command[start];
                }
                std::cout << "start: " << start << "\n";
                
                second_str = command.substr(start + 1);
                int first = std::stoi(first_str);
                int second = std::stoi(second_str);
                
                std::vector<CTransportationPlanner::TNodeID> res = {};
                double distance = planner->FindShortestPath(first, second, res);
                double factor = std::pow(10.0, 1);
                distance = std::round(distance * factor) / factor;
                std::string dis_str = std::to_string(distance);
                std::string o_str = "";
                start = 0;
                for (; start < dis_str.length(); ++start){
                    if (dis_str[start] == '.'){
                        break;
                    }
                    o_str += dis_str[start];
                }
                o_str += dis_str.substr(start, 2);
                outsink->Write(convertstringintovector("Shortest path is " + o_str + " mi.\n"));
                outsink->Put('>');
                outsink->Put(' ');
            }
            else if (command.substr(0, 8) == "fastest "){
                std::string first_str = "";
                std::string second_str = "";
                int start = 8;

                for (; start < command.length(); ++start){
                    if (command[start] == ' '){
                        break;
                    }
                    first_str += command[start];
                }
                std::cout << "start: " << start << "\n";
                
                second_str = command.substr(start + 1);
                int first = std::stoi(first_str);
                int second = std::stoi(second_str);

                std::vector<CTransportationPlanner::TTripStep> res = {};
                double hours = planner->FindFastestPath(first, second, res);
                double minutes = hours * 60;
                double factor = std::pow(10.0, 1);
                minutes = std::round(minutes * factor) / factor;
                std::string dis_str = std::to_string(minutes);
                std::string o_str = "";
                start = 0;
                for (; start < dis_str.length(); ++start){
                    if (dis_str[start] == '.'){
                        break;
                    }
                    o_str += dis_str[start];
                }
                o_str += dis_str.substr(start, 2);
                outsink->Write(convertstringintovector("Fastest path takes " + o_str + " min.\n"));
                outsink->Put('>');
                outsink->Put(' ');
            }

            else if (command == "print") {

                std::vector<CTransportationPlanner::TTripStep> path = {};
                std::vector< std::string > result_vector = {};
                planner->GetPathDescription(path, result_vector);
                std::string o_str = "";
                for (auto string : result_vector){
                    o_str += string;
                    o_str += "\n";
                }
                outsink->Write(convertstringintovector(o_str));
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


