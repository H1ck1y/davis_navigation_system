#include <gtest/gtest.h>
#include "DijkstraPathRouter.h"

CDijkstraPathRouter graph1;

TEST(CDijkstraPathRouter, test){
    EXPECT_EQ(graph1.AddVertex(std::string("v1")),0);
    EXPECT_EQ(graph1.AddVertex(std::string("v2")),1);
    EXPECT_EQ(graph1.AddVertex(std::string("v3")),2);
    EXPECT_EQ(graph1.AddVertex(std::string("v4")),3);
    EXPECT_EQ(graph1.AddVertex(std::string("v5")),4);
    EXPECT_EQ(graph1.AddVertex(std::string("v6")),5);
    
    EXPECT_EQ(std::any_cast<std::string>(graph1.GetVertexTag(0)), "v1");
    EXPECT_EQ(std::any_cast<std::string>(graph1.GetVertexTag(1)), "v2");
    EXPECT_EQ(std::any_cast<std::string>(graph1.GetVertexTag(2)), "v3");
    EXPECT_EQ(std::any_cast<std::string>(graph1.GetVertexTag(3)), "v4");
    EXPECT_EQ(std::any_cast<std::string>(graph1.GetVertexTag(4)), "v5");
    EXPECT_EQ(std::any_cast<std::string>(graph1.GetVertexTag(5)), "v6");

    EXPECT_TRUE(graph1.AddEdge(0,1,5));
    EXPECT_TRUE(graph1.AddEdge(0,4,4));
    EXPECT_TRUE(graph1.AddEdge(1,3,9));
    EXPECT_TRUE(graph1.AddEdge(1,2,2));
    EXPECT_TRUE(graph1.AddEdge(2,4,6));
    EXPECT_TRUE(graph1.AddEdge(3,0,2));
    EXPECT_TRUE(graph1.AddEdge(4,3,7));
    EXPECT_TRUE(graph1.AddEdge(4,1,6));
    EXPECT_TRUE(graph1.AddEdge(4,5,5));
    EXPECT_TRUE(graph1.AddEdge(5,2,2));
    
    std::vector<CDijkstraPathRouter::TVertexID> path;
    std::vector<CDijkstraPathRouter::TVertexID> expected_path = {0, 4, 3};
    EXPECT_EQ(graph1.FindShortestPath(0, 3, path), 11);
    EXPECT_EQ(path, expected_path);

    expected_path = {0,4,5};
    EXPECT_EQ(graph1.FindShortestPath(0, 5, path), 9);
    EXPECT_EQ(path, expected_path);
    
    expected_path = {0,1,2};
    EXPECT_EQ(graph1.FindShortestPath(0, 2, path), 7);
    EXPECT_EQ(path, expected_path);

    expected_path = {0,1};
    EXPECT_EQ(graph1.FindShortestPath(0, 1, path), 5);
    EXPECT_EQ(path, expected_path);

    expected_path = {0,4};
    EXPECT_EQ(graph1.FindShortestPath(0, 4, path), 4);
    EXPECT_EQ(path, expected_path);




}


