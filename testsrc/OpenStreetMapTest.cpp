#include "OpenStreetMap.h"
#include <gtest/gtest.h>
#include "StringDataSource.h"


TEST(OpenStreetMap, SimpleExampleTest){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"1\" lat=\"1.0\" lon=\"-1.0\"/>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    
    EXPECT_EQ(StreetMap.NodeCount(),1);
    EXPECT_EQ(StreetMap.NodeByIndex(0)->ID(),1);
    EXPECT_EQ(StreetMap.NodeByIndex(0)->Location(), std::make_pair(1.0, -1.0));
    EXPECT_EQ(StreetMap.NodeByIndex(0)->AttributeCount(), 0);
    EXPECT_EQ(StreetMap.NodeByIndex(0)->GetAttributeKey(0),"");// equal to
    EXPECT_EQ(StreetMap.NodeByIndex(0)->GetAttributeKey(1),""); //greater than 

}

TEST(OpenStreetMap, NODETest1_mutiplenodes){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"95711493\" lat=\"38.5517235\" lon=\"-121.724133\"/>"
	                                                    "<node id=\"95711495\" lat=\"38.5522833\" lon=\"-121.724551\"/>"
	                                                    "<node id=\"95711496\" lat=\"38.5525465\" lon=\"-121.7244427\"/>"
	                                                    "<node id=\"95711498\" lat=\"38.5526953\" lon=\"-121.7233585\"/>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    
    EXPECT_EQ(StreetMap.NodeCount(),4);
    EXPECT_EQ(StreetMap.NodeByIndex(3)->ID(),95711498);
    EXPECT_EQ(StreetMap.NodeByIndex(2)->Location(), std::make_pair(38.5525465, -121.7244427));
    EXPECT_EQ(StreetMap.NodeByID(95711496)->Location(), std::make_pair(38.5525465, -121.7244427));

}


TEST(OpenStreetMap, NODETest_withtags){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5589195694\" lat=\"38.5668595\" lon=\"-121.7426575\"/>"
	                                                    "<node id=\"5589195695\" lat=\"38.5678795\" lon=\"-121.7429863\"/>"
	                                                    "<node id=\"5595530857\" lat=\"38.5549217\" lon=\"-121.7164936\">"
		                                                        "<tag k=\"highway\" v=\"stop\"/>"
	                                                    "</node>"
	                                                    "<node id=\"5595530858\" lat=\"38.5549283\" lon=\"-121.716192\">"
		                                                        "<tag k=\"highway\" v=\"stop\"/>"
	                                                    "</node>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    EXPECT_EQ(StreetMap.NodeCount(),4);
    EXPECT_EQ(StreetMap.NodeByIndex(2)->GetAttribute("highway"),"stop");
    EXPECT_EQ(StreetMap.NodeByIndex(3)->GetAttribute("highway"),"stop");
    EXPECT_EQ(StreetMap.NodeByIndex(2)->AttributeCount(), 1);
    EXPECT_EQ(StreetMap.NodeByIndex(3)->AttributeCount(), 1);
    EXPECT_EQ(StreetMap.NodeByIndex(0)->AttributeCount(), 0);
    EXPECT_TRUE(StreetMap.NodeByIndex(2)->HasAttribute("highway"));
    EXPECT_FALSE(StreetMap.NodeByIndex(0)->HasAttribute("highway"));
}


TEST(OpenStreetMap, Way_simple_tests){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<way id=\"10746816\">"
		                                                "<nd ref=\"95711580\"/>"
		                                                "<nd ref=\"1789394193\"/>"
		                                                "<nd ref=\"275386362\"/>"
		                                                "<nd ref=\"3965149210\"/>"
		                                                "<nd ref=\"275386363\"/>"
		                                                "<nd ref=\"95710975\"/>"
		                                                "<nd ref=\"3965149209\"/>"
		                                                "<nd ref=\"3965149211\"/>"
		                                                "<nd ref=\"95718583\"/>"
		                                                "<nd ref=\"95718584\"/>"
		                                                "<tag k=\"highway\" v=\"residential\"/>"
		                                                "<tag k=\"name\" v=\"Union Drive\"/>"
	                                                    "</way>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    EXPECT_EQ(StreetMap.WayCount(),1);
    EXPECT_EQ(StreetMap.WayByIndex(0)->ID(),10746816);
    EXPECT_EQ(StreetMap.WayByIndex(0)->NodeCount(),10);
    EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttribute("name"),"Union Drive");
    EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttribute("ref"),"");
    EXPECT_EQ(StreetMap.WayByIndex(0)->GetNodeID(2),275386362);
    EXPECT_EQ(StreetMap.WayByIndex(0)->GetNodeID(8),95718583);
    EXPECT_EQ(StreetMap.WayByIndex(0)->AttributeCount(),2);
    EXPECT_TRUE(StreetMap.WayByIndex(0)->HasAttribute("name"));
    EXPECT_FALSE(StreetMap.WayByIndex(0)->HasAttribute("hahahha"));
    EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(0),"highway");
    EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(1),"name");
    EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(2),"");
    EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(5),"");
    EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(17),"");

}

TEST(OpenStreetMap, Way_complex_tests){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<way id=\"10746816\">"
		                                                "<nd ref=\"95711580\"/>"
		                                                "<nd ref=\"1789394193\"/>"
		                                                "<tag k=\"highway\" v=\"residential\"/>"
		                                                "<tag k=\"name\" v=\"Union Drive\"/>"
	                                                    "</way>"
                                                        "<way id=\"10747761\">"
		                                                "<nd ref=\"95713574\"/>"
		                                                "<nd ref=\"95722418\"/>"
		                                                "<nd ref=\"1919602440\"/>"
		                                                "<tag k=\"highway\" v=\"residential\"/>"
		                                                "<tag k=\"name\" v=\"Colusa Avenue\"/>"
	                                                    "</way>"
                                                        "<way id=\"10747763\">"
		                                                "<nd ref=\"95714499\"/>"
		                                                "<nd ref=\"2599137951\"/>"
		                                                "<nd ref=\"5027298034\"/>"
		                                                "<nd ref=\"95719939\"/>"
		                                             	"<tag k=\"bicycle\" v=\"yes\"/>"
		                                                "<tag k=\"cycleway\" v=\"lane\"/>"
		                                                "<tag k=\"highway\" v=\"tertiary\"/>"
		                                                "<tag k=\"maxspeed\" v=\"25 mph\"/>"
		                                                "<tag k=\"maxspeed:type\" v=\"sign\"/>"
		                                                "<tag k=\"name\" v=\"East 8th Street\"/>"
		                                                "<tag k=\"oneway\" v=\"no\"/>"
		                                                "<tag k=\"surface\" v=\"asphalt\"/>"
	                                                    "</way>"
                                                        "</osm>");
   auto Reader = std::make_shared<CXMLReader>(InStream);
   COpenStreetMap StreetMap(Reader);
   EXPECT_EQ(StreetMap.WayCount(),3);
   EXPECT_EQ(StreetMap.WayByIndex(0)->ID(),10746816);
   EXPECT_EQ(StreetMap.WayByIndex(0)->NodeCount(),2); 
   EXPECT_EQ(StreetMap.WayByIndex(0)->AttributeCount(),2);
   EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(0),"highway");
   EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(1),"name");
   EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(2),""); //equal to
   EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(3),""); //greater than
   EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttribute("highway"),"residential");
   EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttribute("name"),"Union Drive");
   EXPECT_EQ(StreetMap.WayByID(10746816)->GetAttribute("name"),"Union Drive");
   //
   EXPECT_EQ(StreetMap.WayByIndex(1)->ID(),10747761);
   EXPECT_EQ(StreetMap.WayByIndex(1)->NodeCount(),3); 
   EXPECT_EQ(StreetMap.WayByIndex(1)->AttributeCount(),2);
   EXPECT_EQ(StreetMap.WayByIndex(1)->GetAttributeKey(0),"highway");
   EXPECT_EQ(StreetMap.WayByIndex(1)->GetAttributeKey(1),"name");
   EXPECT_EQ(StreetMap.WayByIndex(1)->GetAttributeKey(2),"");
   EXPECT_EQ(StreetMap.WayByIndex(1)->GetAttribute("highway"),"residential");
   EXPECT_EQ(StreetMap.WayByIndex(1)->GetAttribute("name"),"Colusa Avenue");
   //
   EXPECT_EQ(StreetMap.WayByIndex(2)->ID(),10747763);
   EXPECT_EQ(StreetMap.WayByIndex(2)->NodeCount(),4); 
   EXPECT_EQ(StreetMap.WayByIndex(2)->AttributeCount(),8);
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(0),"bicycle");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(1),"cycleway");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(2),"highway");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(3),"maxspeed");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(4),"maxspeed:type");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(5),"name");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(6),"oneway");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(7),"surface");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(8),"");//equal to 
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttributeKey(9),"");// greater than
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttribute("bicycle"),"yes");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttribute("cycleway"),"lane");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttribute("highway"),"tertiary");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttribute("maxspeed"),"25 mph");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttribute("maxspeed:type"),"sign");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttribute("name"),"East 8th Street");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttribute("oneway"),"no");
   EXPECT_EQ(StreetMap.WayByIndex(2)->GetAttribute("surface"),"asphalt");
   EXPECT_EQ(StreetMap.WayByID(10747763)->GetAttribute("surface"),"asphalt");
}

TEST(OpenStreetMap, EdgeCase_tests){
 auto InStream = std::make_shared<CStringDataSource>(   "<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5589195694\" lat=\"38.5668595\" lon=\"-121.7426575\"/>"
                                                        "<way id=\"10746816\">"
		                                                "<nd ref=\"95711580\"/>"
		                                                "<nd ref=\"1789394193\"/>"
		                                                "<tag k=\"highway\" v=\"residential\"/>"
		                                                "<tag k=\"name\" v=\"Union Drive\"/>"
	                                                    "</way>"
	                                                    "<node id=\"5589195695\" lat=\"38.5678795\" lon=\"-121.7429863\"/>"
	                                                    "<node id=\"5595530857\" lat=\"38.5549217\" lon=\"-121.7164936\">"
		                                                        "<tag k=\"highway\" v=\"stop\"/>"
	                                                    "</node>"
                                                        "<way id=\"10747761\">"
		                                                "<nd ref=\"95713574\"/>"
		                                                "<nd ref=\"95722418\"/>"
		                                                "<nd ref=\"1919602440\"/>"
		                                                "<tag k=\"highway\" v=\"residential\"/>"
		                                                "<tag k=\"name\" v=\"Colusa Avenue\"/>"
	                                                    "</way>"
	                                                    "<node id=\"5595530858\" lat=\"38.5549283\" lon=\"-121.716192\">"
		                                                        "<tag k=\"highway\" v=\"stop\"/>"
	                                                    "</node>"
                                                        "</osm>");
 auto Reader = std::make_shared<CXMLReader>(InStream);
 COpenStreetMap StreetMap(Reader);


 EXPECT_EQ(StreetMap.NodeByIndex(4),nullptr);
 EXPECT_EQ(StreetMap.WayByIndex(2),nullptr);
 EXPECT_EQ(StreetMap.NodeByID(4),nullptr);
 EXPECT_EQ(StreetMap.WayByID(4),nullptr);
 EXPECT_EQ(StreetMap.NodeByID(5589195695)->ID(),5589195695);
 EXPECT_EQ(StreetMap.NodeByIndex(0)->ID(),5589195694);
 EXPECT_EQ(StreetMap.NodeByIndex(2)->GetAttributeKey(0),"highway");
 EXPECT_EQ(StreetMap.NodeByIndex(0)->GetAttribute("happy"),"");
 EXPECT_EQ(StreetMap.WayByIndex(0)->GetAttributeKey(0),"highway");
 //EXPECT_EQ(StreetMap.WayByIndex(0)->GetNodeID(100),StreetMap.InvalidNodeID);
}
