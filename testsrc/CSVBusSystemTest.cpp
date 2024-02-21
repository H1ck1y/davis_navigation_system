#include "CSVBusSystem.h"
#include <gtest/gtest.h>
#include "StringDataSource.h"

TEST(CSVBusSystem, SimpleTest1){
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id\n1,1001");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id");
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    CCSVBusSystem BusSystem(CSVReaderStops, CSVReaderRoutes);

    EXPECT_EQ(BusSystem.StopCount(),1);
    EXPECT_EQ(BusSystem.RouteCount(),0);
    EXPECT_EQ(BusSystem.StopByIndex(0)->ID(),1);
    EXPECT_EQ(BusSystem.StopByIndex(0)->NodeID(),1001);
    EXPECT_EQ(BusSystem.StopByID(1)->NodeID(),1001);
}

TEST(CSVBusSystem, SimpleTest2){
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id\n1,1001\n2,1002\n3,1003");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id\nA,1\nA,2\nA,3");
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops, ',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes, ',');
    CCSVBusSystem BusSystem(CSVReaderStops, CSVReaderRoutes);

    EXPECT_EQ(BusSystem.StopCount(),3);
    EXPECT_EQ(BusSystem.StopByID(1)->ID(),1);
    EXPECT_EQ(BusSystem.StopByID(1)->NodeID(),1001);    
    EXPECT_EQ(BusSystem.StopByID(2)->ID(),2);
    EXPECT_EQ(BusSystem.StopByID(2)->NodeID(),1002);
    EXPECT_EQ(BusSystem.StopByIndex(0)->ID(),1);
    EXPECT_EQ(BusSystem.StopByIndex(0)->NodeID(),1001);
    EXPECT_EQ(BusSystem.StopByIndex(1)->ID(),2);
    EXPECT_EQ(BusSystem.StopByIndex(1)->NodeID(),1002);
    EXPECT_EQ(BusSystem.RouteCount(),1);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->StopCount(),3);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->Name(),"A");
    EXPECT_EQ(BusSystem.RouteByIndex(0)->GetStopID(0),1);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->GetStopID(1),2);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->GetStopID(2),3);
    EXPECT_EQ(BusSystem.RouteByName("A")->GetStopID(0),1);
    EXPECT_EQ(BusSystem.RouteByName("A")->GetStopID(1),2);
    EXPECT_EQ(BusSystem.RouteByName("A")->GetStopID(2),3);
}

TEST(CSVBusSystem, AdvancedRouteTest1){
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id\n1,1001\n2,1002\n3,1003");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id\nA,1\nA,2\nA,3\nB,1\nB,3");
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops, ',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes, ',');
    CCSVBusSystem BusSystem(CSVReaderStops, CSVReaderRoutes);

    EXPECT_EQ(BusSystem.StopCount(),3);
    EXPECT_EQ(BusSystem.StopByID(1)->ID(),1);
    EXPECT_EQ(BusSystem.StopByID(1)->NodeID(),1001);    
    EXPECT_EQ(BusSystem.StopByID(2)->ID(),2);
    EXPECT_EQ(BusSystem.StopByID(2)->NodeID(),1002);
    EXPECT_EQ(BusSystem.StopByIndex(0)->ID(),1);
    EXPECT_EQ(BusSystem.StopByIndex(0)->NodeID(),1001);
    EXPECT_EQ(BusSystem.StopByIndex(1)->ID(),2);
    EXPECT_EQ(BusSystem.StopByIndex(1)->NodeID(),1002);
    EXPECT_EQ(BusSystem.RouteCount(),2);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->StopCount(),3);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->Name(),"A");
    EXPECT_EQ(BusSystem.RouteByIndex(0)->GetStopID(0),1);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->GetStopID(1),2);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->GetStopID(2),3);
    EXPECT_EQ(BusSystem.RouteByIndex(1)->StopCount(),2);
    EXPECT_EQ(BusSystem.RouteByIndex(1)->Name(),"B");
    EXPECT_EQ(BusSystem.RouteByIndex(1)->GetStopID(0),1);
    EXPECT_EQ(BusSystem.RouteByIndex(1)->GetStopID(1),3);
    EXPECT_EQ(BusSystem.RouteByName("A")->GetStopID(0),1);
    EXPECT_EQ(BusSystem.RouteByName("A")->GetStopID(1),2);
    EXPECT_EQ(BusSystem.RouteByName("A")->GetStopID(2),3);
    EXPECT_EQ(BusSystem.RouteByName("B")->GetStopID(0),1);
    EXPECT_EQ(BusSystem.RouteByName("B")->GetStopID(1),3);
}


TEST(CSVBusSystem, ComplexTest){
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id\n1,1001\n22177,2599137935\n22078,1190339479\n22164,95718668");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id\nA,22169\nA,22000\nB,22179\nB,22167\nD,22240\nD,22173\nD,22256\nA,666");
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    CCSVBusSystem BusSystem(CSVReaderStops, CSVReaderRoutes);

    EXPECT_EQ(BusSystem.StopCount(),4);
    EXPECT_EQ(BusSystem.StopByID(1)->ID(),1);
    EXPECT_EQ(BusSystem.StopByID(1)->NodeID(),1001);    
    EXPECT_EQ(BusSystem.StopByID(22078)->ID(),22078);
    EXPECT_EQ(BusSystem.StopByID(22078)->NodeID(),1190339479);
    EXPECT_EQ(BusSystem.StopByIndex(1)->ID(),22177);
    EXPECT_EQ(BusSystem.StopByIndex(1)->NodeID(),2599137935);
    EXPECT_EQ(BusSystem.StopByIndex(3)->ID(),22164);
    EXPECT_EQ(BusSystem.StopByIndex(3)->NodeID(),95718668);
    EXPECT_EQ(BusSystem.RouteCount(),3);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->StopCount(),3);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->Name(),"A");
    EXPECT_EQ(BusSystem.RouteByIndex(0)->GetStopID(0),22169);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->GetStopID(2),666);
    EXPECT_EQ(BusSystem.RouteByIndex(1)->StopCount(),2);
    EXPECT_EQ(BusSystem.RouteByName("A")->GetStopID(2),666);
    EXPECT_EQ(BusSystem.RouteByName("B")->GetStopID(1),22167);
}

TEST(CSVBusSystem, InvalidInputTest){
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id\n1,1001\n22177,2599137935\n22078,1190339479\n22164,95718668");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id\nA,22169\nA,22000\nB,22179\nB,22167\nD,22240\nD,22173\nD,22256\nA,666");
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    CCSVBusSystem BusSystem(CSVReaderStops, CSVReaderRoutes);

    EXPECT_EQ(BusSystem.RouteByIndex(0)->GetStopID(10), std::numeric_limits<CBusSystem::TStopID>::max());
    EXPECT_EQ(BusSystem.RouteByIndex(1)->GetStopID(3), std::numeric_limits<CBusSystem::TStopID>::max());
    EXPECT_EQ(BusSystem.RouteByIndex(5), nullptr);
    EXPECT_EQ(BusSystem.RouteByName("C"), nullptr);
}

