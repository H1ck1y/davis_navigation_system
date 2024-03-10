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
    std::queue<std::string> buffer;// this is the buffer contains the command string
    std::queue<std::vector<CTransportationPlanner::TTripStep>> savingbuffer;// this is the buffer records the fastest or shortest command

    std::vector<char> convertstringintovector(std::string info){ //this converts the string into vector cuz we write method can only support vector of string
        std::vector<char> charVector;
        for (char ch: info){
            charVector.push_back(ch);
        }
        return charVector;
    }


    std::string ToString(CTransportationPlanner::ETransportationMode mode) {
        // this function will convert numeric type into string 
        switch (mode) {
            case CTransportationPlanner::ETransportationMode::Walk: 
                return "Walk";
            case CTransportationPlanner::ETransportationMode::Bike: 
                return "Bike";
            case CTransportationPlanner::ETransportationMode::Bus: 
                return "Bus";
            default: return "";
        }
    }

    void savingpath(){
        // this function is for saving utility
            if (savingbuffer.empty()) {
                    errsink->Write(convertstringintovector("No valid path to save, see help.\n"));
                    return; //this part will check if there is something to save
            }
            std::string name = "123_456_1.375000hr.csv";
            auto saveSink = results->CreateSink(name);
            outsink->Write(convertstringintovector("Path saved to <results>/" + name + "\n"));

            auto ExpectedSteps = savingbuffer.front(); // check the saving buffer and put it into the saving sink
            savingbuffer.pop();
            saveSink->Write(convertstringintovector("mode,node_id\n"));
            for (size_t i = 0; i < ExpectedSteps.size(); i++) {
                    std::string mode = ToString(ExpectedSteps[i].first);// get each part of the information and combine them together 
                    std::string node = std::to_string(ExpectedSteps[i].second);
            if (i < ExpectedSteps.size() - 1) {
                    saveSink->Write(convertstringintovector(mode + "," + node + "\n"));
            } else {
                saveSink->Write(convertstringintovector(mode + "," + node));
            }
        }
    }

    void printpath(){
        // this function will implement print command
        if (savingbuffer.empty()){ // if there is no path in the saving buffer, then return
            errsink->Write(convertstringintovector("No valid path to print, see help.\n"));
            return;
        }

        std::vector<CTransportationPlanner::TTripStep> path = {};
        std::vector< std::string > result_vector = {};
        planner->GetPathDescription(path, result_vector); // call the function get path description to get the result
        std::string o_str = "";
        for (auto string : result_vector){
            o_str += string;
            o_str += "\n";
        }
        outsink->Write(convertstringintovector(o_str));
    }

    void validnode(std::string command){
        // this function will implement the command of node
        if (command.substr(0,5) != "node "){
             errsink->Write(convertstringintovector("Invalid node command, see help.\n"));
            return;
        }
        try{
            std::string node_index_str = command.substr(5);
            int node_index = std::stoi(node_index_str);
            int id = planner->SortedNodeByIndex(node_index)->ID();// get the id 
            auto location_pair = planner->SortedNodeByIndex(node_index)->Location();
            std::string location = SGeographicUtils::ConvertLLToDMS(location_pair);// get the location
            outsink->Write(convertstringintovector("Node " + node_index_str + + ": id = " + std::to_string(id) + " is at " + location + "\n"));
        }catch (std::invalid_argument& e){
            errsink->Write(convertstringintovector("Invalid node parameter, see help.\n"));
            return;
        }
    }
    void getshortest(std::string command){
        // this function will implement the command of shortest
        if (command.substr(0,9) != "shortest "){
             errsink->Write(convertstringintovector("Invalid shortest command, see help.\n"));// invalid input
            return;
        }
        try{
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
            int first = std::stoi(first_str);// convert into the first nodeid
            int second = std::stoi(second_str);// convert into the second nodeid
                
            std::vector<CTransportationPlanner::TNodeID> res = {};
            double distance = planner->FindShortestPath(first, second, res);
            double factor = std::pow(10.0, 1);// this is for converting 25.00000000000000->25.0
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
        }catch(std::invalid_argument& e){// if it throws out any error, break
             errsink->Write(convertstringintovector("Invalid shortest parameter, see help.\n"));
             return;
        }
    }

    void getfastest(std::string command){
        if (command.substr(0,8) != "fastest "){
            //if the first 9 character is not this, invalid input
             errsink->Write(convertstringintovector("Invalid fastest command, see help.\n"));
            return;
        }
        try{
            std::string first_str = "";
            std::string second_str = "";
            int start = 8;

            for (; start < command.length(); ++start){
                if (command[start] == ' '){
                    break;
                }
                first_str += command[start];  // this for getting the node
            }
            std::cout << "start: " << start << "\n";
                
            second_str = command.substr(start + 1);
            int first = std::stoi(first_str);
            int second = std::stoi(second_str);

            std::vector<CTransportationPlanner::TTripStep> res = {};
            double hours = planner->FindFastestPath(first, second, res);// call the function Findfastestpath
            savingbuffer.push(res);
            double minutes = hours * 60;
            double factor = std::pow(10.0, 1);
            minutes = std::round(minutes * factor) / factor; // calculat the time needed for the fastest path
            std::string dis_str = std::to_string(minutes);
            std::string o_str = "";
            start = 0;
            for (; start < dis_str.length(); ++start){ // change the time into specific format
                if (dis_str[start] == '.'){
                        break;
                }
                o_str += dis_str[start];
            }
            o_str += dis_str.substr(start, 2);
            outsink->Write(convertstringintovector("Fastest path takes " + o_str + " min.\n"));
        }catch(std::invalid_argument& e){
            errsink->Write(convertstringintovector("Invalid fastest parameter, see help.\n"));
        }

    }



    bool ProcessCommands(){
        // this function is the main function, based on different commands have different outputs
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
            else if (command.find("node") != std::string::npos) {
                validnode(command);
                outsink->Put('>');
                outsink->Put(' ');
            }
            else if (command.find("shortest") != std::string::npos){
                getshortest(command);
                outsink->Put('>');
                outsink->Put(' ');

            }
            else if (command.find("fastest") != std::string::npos){
                getfastest(command);
                outsink->Put('>');
                outsink->Put(' ');
            }

            else if (command == "print") {
                printpath();
                outsink->Put('>');
                outsink->Put(' ');
            }
            else if (command == "save"){
                savingpath();
                outsink->Put('>');
                outsink->Put(' ');
            }
            else{// this is for unknown commands
                errsink->Write(convertstringintovector("Unknown command \"" + command + "\" type help for help.\n"));
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


