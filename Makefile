CC= gcc
CXX = g++
INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TEST_SRC_DIR = ./testsrc
CXXFLAGS = -std=c++17 -I $(INC_DIR)
LDFLAGS = -lgmock -lgtest -lgtest_main -lpthread -lexpat

all : directories runtests

runtests: $(BIN_DIR)/teststrutils  $(BIN_DIR)/teststrdatasource  $(BIN_DIR)/teststrdatasink  $(BIN_DIR)/testdsv  $(BIN_DIR)/testxml  $(BIN_DIR)/testosm $(BIN_DIR)/testcsvbs $(BIN_DIR)/testfiledatass $(BIN_DIR)/testcsvbsi $(BIN_DIR)/testkml $(BIN_DIR)/testdpr $(BIN_DIR)/testtp $(BIN_DIR)/testtpcl $(BIN_DIR)/testspeed
	$(BIN_DIR)/teststrutils
	$(BIN_DIR)/teststrdatasource
	$(BIN_DIR)/teststrdatasink
	$(BIN_DIR)/testdsv
	$(BIN_DIR)/testxml
	$(BIN_DIR)/testkml
	$(BIN_DIR)/testosm
	$(BIN_DIR)/testcsvbs
	$(BIN_DIR)/testfiledatass
	$(BIN_DIR)/testcsvbsi
	$(BIN_DIR)/testdpr
	$(BIN_DIR)/testtp
	$(BIN_DIR)/testtpcl
	$(BIN_DIR)/testspeed
	


$(BIN_DIR)/teststrutils: $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o
	$(CXX) -o $(BIN_DIR)/teststrutils $(CXXFLAGS) $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o $(LDFLAGS)

$(BIN_DIR)/teststrdatasource: $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o
	$(CXX) -o $(BIN_DIR)/teststrdatasource $(CXXFLAGS) $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o $(LDFLAGS)

$(BIN_DIR)/teststrdatasink: $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o
	$(CXX) -o $(BIN_DIR)/teststrdatasink $(CXXFLAGS) $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o $(LDFLAGS)

$(BIN_DIR)/testdsv: $(OBJ_DIR)/DSVWriter.o  $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/testdsv $(CXXFLAGS) $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/StringUtils.o  $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)

$(BIN_DIR)/testxml: $(OBJ_DIR)/XMLWriter.o  $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/testxml $(CXXFLAGS) $(OBJ_DIR)/XMLWriter.o  $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/StringUtils.o  $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)

$(BIN_DIR)/testosm: $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/OpenStreetMapTest.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/testosm $(CXXFLAGS) $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/OpenStreetMapTest.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)

$(BIN_DIR)/testcsvbs: $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/testcsvbs $(CXXFLAGS) $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)

$(BIN_DIR)/testfiledatass: $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSSTest.o
	$(CXX) -o $(BIN_DIR)/testfiledatass $(CXXFLAGS) $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSSTest.o $(LDFLAGS)

$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o 
	$(CXX) -o $(BIN_DIR)/testcsvbsi $(CXXFLAGS) $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)

$(BIN_DIR)/testkml: $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/KMLTest.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o
	$(CXX) -o $(BIN_DIR)/testkml $(CXXFLAGS) $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/KMLTest.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(LDFLAGS)


$(BIN_DIR)/testdpr: $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/DijkstraPathRouterTest.o
	$(CXX) -o $(BIN_DIR)/testdpr $(CXXFLAGS) $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/DijkstraPathRouterTest.o $(LDFLAGS)

$(BIN_DIR)/testtp: $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o  $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/BusSystemIndexer.o
	$(CXX) -o $(BIN_DIR)/testtp $(CXXFLAGS) $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o  $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/BusSystemIndexer.o $(LDFLAGS)


$(BIN_DIR)/testtpcl: $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/TPCommandLineTest.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DijkstraTransportationPlanner.o   $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/BusSystemIndexer.o
	$(CXX) -o $(BIN_DIR)/testtpcl $(CXXFLAGS) $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/TPCommandLineTest.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DijkstraTransportationPlanner.o  $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/BusSystemIndexer.o $(LDFLAGS)


$(BIN_DIR)/testspeed: $(OBJ_DIR)/speedtest.o  $(OBJ_DIR)/StandardDataSink.o $(OBJ_DIR)/StandardDataSource.o $(OBJ_DIR)/StandardErrorDataSink.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/DijkstraTransportationPlanner.o
	$(CXX) -o $(BIN_DIR)/testspeed $(CXXFLAGS) $(OBJ_DIR)/speedtest.o $(OBJ_DIR)/StandardDataSink.o $(OBJ_DIR)/StandardDataSource.o $(OBJ_DIR)/StandardErrorDataSink.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(LDFLAGS)





$(OBJ_DIR)/StringUtils.o: $(SRC_DIR)/StringUtils.cpp  $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtils.o -c $(CXXFLAGS) $(SRC_DIR)/StringUtils.cpp

$(OBJ_DIR)/StringUtilsTest.o: $(TEST_SRC_DIR)/StringUtilsTest.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtilsTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/StringUtilsTest.cpp


$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/StringDataSource.o -c $(CXXFLAGS) $(SRC_DIR)/StringDataSource.cpp

$(OBJ_DIR)/StringDataSourceTest.o: $(TEST_SRC_DIR)/StringDataSourceTest.cpp  $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/StringDataSourceTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/StringDataSourceTest.cpp




$(OBJ_DIR)/StringDataSink.o: $(SRC_DIR)/StringDataSink.cpp $(INC_DIR)/StringDataSink.h
	$(CXX) -o $(OBJ_DIR)/StringDataSink.o -c $(CXXFLAGS) $(SRC_DIR)/StringDataSink.cpp


$(OBJ_DIR)/StringDataSinkTest.o: $(TEST_SRC_DIR)/StringDataSinkTest.cpp  $(INC_DIR)/StringDataSink.h
	$(CXX) -o $(OBJ_DIR)/StringDataSinkTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/StringDataSinkTest.cpp



$(OBJ_DIR)/DSVWriter.o: $(SRC_DIR)/DSVWriter.cpp $(INC_DIR)/DSVWriter.h $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/DSVWriter.o -c $(CXXFLAGS) $(SRC_DIR)/DSVWriter.cpp


$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DSVReader.h $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/DSVReader.o -c $(CXXFLAGS) $(SRC_DIR)/DSVReader.cpp



$(OBJ_DIR)/DSVTest.o: $(TEST_SRC_DIR)/DSVTest.cpp  $(INC_DIR)/DSVWriter.h  $(INC_DIR)/StringDataSink.h $(INC_DIR)/DSVReader.h  $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/DSVTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/DSVTest.cpp



$(OBJ_DIR)/XMLWriter.o: $(SRC_DIR)/XMLWriter.cpp $(INC_DIR)/XMLWriter.h $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/XMLWriter.o -c $(CXXFLAGS) $(SRC_DIR)/XMLWriter.cpp
	

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/XMLReader.h $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/XMLReader.o -c $(CXXFLAGS) $(SRC_DIR)/XMLReader.cpp


$(OBJ_DIR)/XMLTest.o: $(TEST_SRC_DIR)/XMLTest.cpp  $(INC_DIR)/XMLWriter.h  $(INC_DIR)/StringDataSink.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/XMLReader.h
	$(CXX) -o $(OBJ_DIR)/XMLTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/XMLTest.cpp


$(OBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp $(INC_DIR)/OpenStreetMap.h 
	$(CXX) -o $(OBJ_DIR)/OpenStreetMap.o -c $(CXXFLAGS) $(SRC_DIR)/OpenStreetMap.cpp


$(OBJ_DIR)/OpenStreetMapTest.o: $(TEST_SRC_DIR)/OpenStreetMapTest.cpp  $(INC_DIR)/StreetMap.h $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/OpenStreetMapTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/OpenStreetMapTest.cpp


$(OBJ_DIR)/CSVBusSystem.o: $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/CSVBusSystem.h 
	$(CXX) -o $(OBJ_DIR)/CSVBusSystem.o -c $(CXXFLAGS) $(SRC_DIR)/CSVBusSystem.cpp

$(OBJ_DIR)/CSVBusSystemTest.o: $(TEST_SRC_DIR)/CSVBusSystemTest.cpp  $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVBusSystemTest.cpp


$(OBJ_DIR)/FileDataSource.o: $(SRC_DIR)/FileDataSource.cpp $(INC_DIR)/FileDataSource.h 
	$(CXX) -o $(OBJ_DIR)/FileDataSource.o -c $(CXXFLAGS) $(SRC_DIR)/FileDataSource.cpp


$(OBJ_DIR)/FileDataSink.o: $(SRC_DIR)/FileDataSink.cpp $(INC_DIR)/FileDataSink.h 
	$(CXX) -o $(OBJ_DIR)/FileDataSink.o -c $(CXXFLAGS) $(SRC_DIR)/FileDataSink.cpp


$(OBJ_DIR)/FileDataFactory.o: $(SRC_DIR)/FileDataFactory.cpp $(INC_DIR)/FileDataFactory.h $(INC_DIR)/FileDataSink.h $(INC_DIR)/FileDataSource.h 
	$(CXX) -o $(OBJ_DIR)/FileDataFactory.o -c $(CXXFLAGS) $(SRC_DIR)/FileDataFactory.cpp


$(OBJ_DIR)/FileDataSSTest.o: $(TEST_SRC_DIR)/FileDataSSTest.cpp $(INC_DIR)/FileDataFactory.h  $(INC_DIR)/FileDataSink.h $(INC_DIR)/FileDataSource.h 
	$(CXX) -o $(OBJ_DIR)/FileDataSSTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/FileDataSSTest.cpp


$(OBJ_DIR)/BusSystemIndexer.o: $(SRC_DIR)/BusSystemIndexer.cpp $(INC_DIR)/BusSystemIndexer.h 
	$(CXX) -o $(OBJ_DIR)/BusSystemIndexer.o -c $(CXXFLAGS) $(SRC_DIR)/BusSystemIndexer.cpp


$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp $(INC_DIR)/BusSystemIndexer.h  $(INC_DIR)/StringUtils.h  $(INC_DIR)/StringDataSource.h  $(INC_DIR)/CSVBusSystem.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemIndexerTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp

$(OBJ_DIR)/KMLWriter.o: $(SRC_DIR)/KMLWriter.cpp $(INC_DIR)/KMLWriter.h  $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/KMLWriter.o -c $(CXXFLAGS) $(SRC_DIR)/KMLWriter.cpp


$(OBJ_DIR)/KMLTest.o: $(TEST_SRC_DIR)/KMLTest.cpp $(INC_DIR)/KMLWriter.h $(INC_DIR)/StringUtils.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/XMLWriter.h $(INC_DIR)/StringDataSink.h
	$(CXX) -o $(OBJ_DIR)/KMLTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/KMLTest.cpp


$(OBJ_DIR)/DijkstraPathRouter.o: $(SRC_DIR)/DijkstraPathRouter.cpp $(INC_DIR)/DijkstraPathRouter.h  
	$(CXX) -o $(OBJ_DIR)/DijkstraPathRouter.o -c $(CXXFLAGS) $(SRC_DIR)/DijkstraPathRouter.cpp


$(OBJ_DIR)/DijkstraPathRouterTest.o: $(TEST_SRC_DIR)/DijkstraPathRouterTest.cpp $(INC_DIR)/DijkstraPathRouter.h  
	$(CXX) -o $(OBJ_DIR)/DijkstraPathRouterTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/DijkstraPathRouterTest.cpp


$(OBJ_DIR)/GeographicUtils.o: $(SRC_DIR)/GeographicUtils.cpp $(INC_DIR)/GeographicUtils.h  
	$(CXX) -o $(OBJ_DIR)/GeographicUtils.o -c $(CXXFLAGS) $(SRC_DIR)/GeographicUtils.cpp


$(OBJ_DIR)/DijkstraTransportationPlanner.o: $(SRC_DIR)/DijkstraTransportationPlanner.cpp $(INC_DIR)/DijkstraTransportationPlanner.h  $(INC_DIR)/BusSystemIndexer.h
	$(CXX) -o $(OBJ_DIR)/DijkstraTransportationPlanner.o -c $(CXXFLAGS) $(SRC_DIR)/DijkstraTransportationPlanner.cpp


$(OBJ_DIR)/CSVOSMTransportationPlannerTest.o: $(TEST_SRC_DIR)/CSVOSMTransportationPlannerTest.cpp $(INC_DIR)/XMLReader.h $(INC_DIR)/StringUtils.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/OpenStreetMap.h $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/DijkstraTransportationPlanner.h $(INC_DIR)/GeographicUtils.h $(INC_DIR)/TransportationPlannerConfig.h
	$(CXX) -o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVOSMTransportationPlannerTest.cpp


$(OBJ_DIR)/TransportationPlannerCommandLine.o: $(SRC_DIR)/TransportationPlannerCommandLine.cpp $(INC_DIR)/TransportationPlannerCommandLine.h $(INC_DIR)/GeographicUtils.h
	$(CXX) -o $(OBJ_DIR)/TransportationPlannerCommandLine.o -c $(CXXFLAGS) $(SRC_DIR)/TransportationPlannerCommandLine.cpp


$(OBJ_DIR)/TPCommandLineTest.o: $(TEST_SRC_DIR)/TPCommandLineTest.cpp $(INC_DIR)/TransportationPlannerCommandLine.h $(INC_DIR)/StringDataSink.h $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/TPCommandLineTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/TPCommandLineTest.cpp

$(OBJ_DIR)/StandardDataSink.o: $(SRC_DIR)/StandardDataSink.cpp $(INC_DIR)/StandardDataSink.h
	$(CXX) -o $(OBJ_DIR)/StandardDataSink.o -c $(CXXFLAGS) $(SRC_DIR)/StandardDataSink.cpp

$(OBJ_DIR)/StandardDataSource.o: $(SRC_DIR)/StandardDataSource.cpp $(INC_DIR)/StandardDataSource.h
	$(CXX) -o $(OBJ_DIR)/StandardDataSource.o -c $(CXXFLAGS) $(SRC_DIR)/StandardDataSource.cpp

$(OBJ_DIR)/StandardErrorDataSink.o: $(SRC_DIR)/StandardErrorDataSink.cpp $(INC_DIR)/StandardErrorDataSink.h
	$(CXX) -o $(OBJ_DIR)/StandardErrorDataSink.o -c $(CXXFLAGS) $(SRC_DIR)/StandardErrorDataSink.cpp

$(OBJ_DIR)/speedtest.o: $(SRC_DIR)/speedtest.cpp   $(INC_DIR)/TransportationPlannerConfig.h $(INC_DIR)/DijkstraTransportationPlanner.h $(INC_DIR)/OpenStreetMap.h $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/FileDataFactory.h $(INC_DIR)/StandardDataSource.h $(INC_DIR)/StandardDataSink.h $(INC_DIR)/StandardErrorDataSink.h $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/speedtest.o -c $(CXXFLAGS) $(SRC_DIR)/speedtest.cpp

directories:
	mkdir -p  $(OBJ_DIR)
	mkdir -p  $(BIN_DIR)


clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)