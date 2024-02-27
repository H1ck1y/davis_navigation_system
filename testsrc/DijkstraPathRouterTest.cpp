#include <gtest/gtest.h>
#include "DijkstraPathRouter.h"

CDijkstraPathRouter graph1;
CDijkstraPathRouter graph2;
CDijkstraPathRouter graph3;

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

TEST(CDijkstraPathRouter, test2){ //have circle
    EXPECT_EQ(graph2.AddVertex('a'),0);
    EXPECT_EQ(graph2.AddVertex('b'),1);
    EXPECT_EQ(graph2.AddVertex('c'),2);
    EXPECT_EQ(graph2.AddVertex('d'),3);
    EXPECT_EQ(graph2.AddVertex('e'),4);
    EXPECT_EQ(graph2.AddVertex('f'),5);
    EXPECT_EQ(graph2.AddVertex('g'),6);

    EXPECT_EQ(graph2.VertexCount(),7);
    
    EXPECT_EQ(std::any_cast<char>(graph2.GetVertexTag(0)), 'a');
    EXPECT_EQ(std::any_cast<char>(graph2.GetVertexTag(1)), 'b');
    EXPECT_EQ(std::any_cast<char>(graph2.GetVertexTag(2)), 'c');
    EXPECT_EQ(std::any_cast<char>(graph2.GetVertexTag(3)), 'd');
    EXPECT_EQ(std::any_cast<char>(graph2.GetVertexTag(4)), 'e');
    EXPECT_EQ(std::any_cast<char>(graph2.GetVertexTag(5)), 'f');
    EXPECT_EQ(std::any_cast<char>(graph2.GetVertexTag(6)), 'g');
    std::any invalid = graph2.GetVertexTag(7);//bad case
    EXPECT_FALSE(invalid.has_value());


    EXPECT_TRUE(graph2.AddEdge(0,1,15));
    EXPECT_TRUE(graph2.AddEdge(0,2,2));
    EXPECT_TRUE(graph2.AddEdge(0,3,12));
    EXPECT_TRUE(graph2.AddEdge(1,4,6));
    EXPECT_TRUE(graph2.AddEdge(2,4,8));
    EXPECT_TRUE(graph2.AddEdge(2,5,4));
    EXPECT_TRUE(graph2.AddEdge(3,6,3));
    EXPECT_TRUE(graph2.AddEdge(4,6,9));
    EXPECT_TRUE(graph2.AddEdge(5,6,10));
    EXPECT_TRUE(graph2.AddEdge(5,3,5));
    EXPECT_TRUE(graph2.AddEdge(6,1,4));

    EXPECT_FALSE(graph2.AddEdge(100,1,4));//SRC NOT EXIST
    EXPECT_FALSE(graph2.AddEdge(1,9,4));//DEST NOT EXIST
    EXPECT_FALSE(graph2.AddEdge(1,3,-4));//NEGATIVE WEIGHT


    std::vector<CDijkstraPathRouter::TVertexID> path;
    std::vector<CDijkstraPathRouter::TVertexID> expected_path = {0, 1};
    EXPECT_EQ(graph2.FindShortestPath(0, 1, path), 15);
    EXPECT_EQ(path, expected_path);

    expected_path = {0, 2};
    EXPECT_EQ(graph2.FindShortestPath(0, 2, path), 2);
    EXPECT_EQ(path, expected_path);

    expected_path = {0,2,5,3};
    EXPECT_EQ(graph2.FindShortestPath(0, 3, path), 11);
    EXPECT_EQ(path, expected_path);

    expected_path = {0,2,4};
    EXPECT_EQ(graph2.FindShortestPath(0, 4, path), 10);
    EXPECT_EQ(path, expected_path);

    expected_path = {0,2,5};
    EXPECT_EQ(graph2.FindShortestPath(0, 5, path), 6);
    EXPECT_EQ(path, expected_path);

    expected_path = {0,2,5,3,6};
    EXPECT_EQ(graph2.FindShortestPath(0, 6, path), 14);
    EXPECT_EQ(path, expected_path);

}


TEST(CDijkstraPathRouter, test3){
    EXPECT_EQ(graph3.AddVertex(std::string("v1")),0);
    EXPECT_EQ(graph3.AddVertex(std::string("v2")),1);
    EXPECT_EQ(graph3.AddVertex(std::string("v3")),2);
    EXPECT_EQ(graph3.AddVertex(std::string("v4")),3);
    EXPECT_TRUE(graph3.AddEdge(0,1,15,true));
    EXPECT_TRUE(graph3.AddEdge(1,2,10,true));
    EXPECT_TRUE(graph3.AddEdge(2,3,10,true));
    EXPECT_TRUE(graph3.AddEdge(3,0,7,true));
    std::vector<CDijkstraPathRouter::TVertexID> path;
    std::vector<CDijkstraPathRouter::TVertexID> expected_path = {0, 3};
    EXPECT_EQ(graph3.FindShortestPath(0, 3, path), 7);
    EXPECT_EQ(path, expected_path);
    EXPECT_EQ(graph3.FindShortestPath(1, 0, path), 15);
    expected_path = {1, 0};
    EXPECT_EQ(path, expected_path);


}