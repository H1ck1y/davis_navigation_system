# datamember
 std::shared_ptr<CDataSource> cmdsrc; 
 std::shared_ptr<CDataSink> outsink;
 std::shared_ptr<CDataSink> errsink; // this is for storing error information
 std::shared_ptr<CDataFactory> results;
 std::shared_ptr<CTransportationPlanner> planner;
 std::queue<std::string> buffer;// this is the buffer contains the command string
 std::queue<std::vector<CTransportationPlanner::TTripStep>> savingbuffer;// this is the buffer records the fastest or shortest command
# memberfunction

## std::vector<char> convertstringintovector(std::string info); 
```
//this converts the string into vector cuz we write method can only support vector of string
parameter: std::string info
return type: std::vector<char>
example: "adfasdf" -> {a,b,f,a,s,d,f}
```
## std::string ToString(CTransportationPlanner::ETransportationMode mode)
```
// this function converts different transportation method into string
parameter: CTransportationPlanner::ETransportationMode mode
return type:  std::string
example:  walk -> "walk"
```


## void savingpath();
```
// this function are used for saving command. it will get the contents saving in the savingbuffer and vonverts into the information we need
parameter: no
return type: no
example: (OutputSink->String(),"> "
          "Fastest path takes 82.5 min.\n"
           "> "
          "Path saved to <results>/123_456_1.375000hr.csv\n"
           "> ");
          SaveSink->String(),"mode,node_id\n"
                                 "Walk,10\n"
                                 "Walk,9\n"
                                 "Bus,8\n"
                                 "Bus,7\n"
                                 "Walk,6");

```

## void printpath();
```
// this function will print the paths. It will print the information stored in the saving buffer.
parameter: no
return type: no
example:  std::vector< std::string > ExpectedDescription = {"Start at 38d 23' 60\" N, 121d 43' 12\" W",
                                                        "Bike N toward Main St. for 6.9 mi",
                                                        "Bike W along Main St. for 4.3 mi",
                                                        "Bike N along B St. for 3.5 mi",
                                                        "End at 38d 32' 60\" N, 121d 47' 60\" W"};
           EXPECT_EQ(OutputSink->String(),"> "
                                    "Fastest path takes 39.0 min.\n"
                                    "> "
                                    "Start at 38d 23' 60\" N, 121d 43' 12\" W\n"
                                    "Bike N toward Main St. for 6.9 mi\n"
                                    "Bike W along Main St. for 4.3 mi\n"
                                    "Bike N along B St. for 3.5 mi\n"
                                    "End at 38d 32' 60\" N, 121d 47' 60\" W\n"
                                    "> ");

```

##  void validnode(std::string command);
```
// this function will check if the command related to node is valid. then it will output the node information
parameter: std::string command
return type: no
example:  auto InputSource = std::make_shared<CStringDataSource>("node 0\nexit\n");
OutputSink->String(),"> "
                                    "Node 0: id = 1234 is at 38d 36' 0\" N, 121d 46' 48\" W\n"
                                    "> ");
```



## void getshortest(std::string command);
```
// this function will check if the command related to shortest is valid. then it will ouput shortest path information between two nmodes
parameter:  std::string command
return type: no
example: auto InputSource = std::make_shared<CStringDataSource>( "shortest 123 456\n"
                                                            "exit\n");
         EXPECT_EQ(OutputSink->String(),"> "
                                    "Shortest path is 5.2 mi.\n"
                                    "> ");
```



##  void getfastest(std::string command);
```
// this fucntion will check if the command related to fastest is valid, then it will ouput the fastest path information between two nodes
parameter:  std::string command
return type: no
example:     std::make_shared<CStringDataSource>( "fastest 123 456\n"
                                                            "exit\n");
               OutputSink->String(),"> "
                                    "Fastest path takes 39.0 min.\n"
                                    "> ");
```


## bool ProcessCommands();
```
// this function will process all of the commands by poping one command from the buffer each time
   then commands will separate into different categories
parameter: no
return type: bool
usage: like a main function used all functions aboved
```

## CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> cmdsrc, std::shared_ptr<CDataSink> outsink,  std::shared_ptr<CDataSink> errsink,std::shared_ptr<CDataFactory> results,std::shared_ptr<CTransportationPlanner> planner)  
```
// this is the constructor functions that will inttialize the interface needed for this class
   it will import all of the commands form cmdsrc, and push different commands into buffer
   corresponding outputs will be put into dirrent sinks
parameter: std::shared_ptr<CDataSource> cmdsrc, std::shared_ptr<CDataSink> outsink,  std::shared_ptr<CDataSink> errsink,std::shared_ptr<CDataFactory> results,std::shared_ptr<CTransportationPlanner> planner
return type: no
```                                                             
                                                                     
                                                                    

