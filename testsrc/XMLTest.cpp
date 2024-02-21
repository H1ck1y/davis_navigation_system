#include <gtest/gtest.h>
#include "XMLWriter.h"
#include "XMLReader.h"
#include "StringDataSink.h"
#include "StringDataSource.h"
#include <iostream>


auto sink1 = std::make_shared<CStringDataSink>();
CXMLWriter writer1(sink1); //sink1 for very very regular



TEST(XMLWriter, WriteRowTestRegular){ // this example is provided by TA: Alex
    std::string result = "";
    SXMLEntity first;
    first.DType = SXMLEntity::EType::StartElement;
    first.DNameData = "example";
    first.SetAttribute("attr", "Hello World");
    EXPECT_TRUE(writer1.WriteEntity(first));
    result += "<example attr=\"Hello World\">";
    EXPECT_EQ(sink1->String(), result);

    SXMLEntity second;
    second.DType = SXMLEntity::EType::CharData;
    second.DNameData = "wow";
    EXPECT_TRUE(writer1.WriteEntity(second));
    result += "wow";
    EXPECT_EQ(sink1->String(), result);

    SXMLEntity third;
    third.DType = SXMLEntity::EType::CompleteElement; 
    third.DNameData = "ok";
    third.SetAttribute("name","Bob");
    result += "<ok name=\"Bob\"/>";
    EXPECT_TRUE(writer1.WriteEntity(third));
    EXPECT_EQ(sink1->String(), result);

    SXMLEntity si;
    si.DType = SXMLEntity::EType::EndElement;
    si.DNameData = "example";
    EXPECT_TRUE(writer1.WriteEntity(si));
    result += "</example>";
    EXPECT_EQ(sink1->String(), result);

   

}


auto sink2 = std::make_shared<CStringDataSink>();
CXMLWriter writer2(sink2); //sink2 for special character

TEST(XMLWriter, WriteRowTestSpecialCharater){
    std::string result2 ="";
    SXMLEntity first;


    SXMLEntity second;
    second.DType = SXMLEntity::EType::StartElement; // we have special character here and need to convert them
    second.DNameData = "PE";
    second.SetAttribute("sport","football>basketball");
    second.SetAttribute("player","CR7<Messi");
    EXPECT_TRUE(writer2.WriteEntity(second));
    result2 += "<PE sport=\"football&gt;basketball\" player=\"CR7&lt;Messi\">";
    EXPECT_EQ(sink2->String(), result2);

    SXMLEntity third; // end cannot have attributes
    third.DType = SXMLEntity::EType::EndElement; 
    third.DNameData = "PE";
    third.SetAttribute("sport","football>basketball");
    third.SetAttribute("player","CR7<Messi");
    EXPECT_FALSE(writer2.WriteEntity(third));

    SXMLEntity si; //chardata cannot have attributes
    si.DType = SXMLEntity::EType::CharData;
    si.DNameData = "\"test cases\" are so difficult to consider \'all\'"; 
    si.SetAttribute("sport","football>basketball");
    si.SetAttribute("player","CR7<Messi");
    EXPECT_FALSE(writer2.WriteEntity(si));

    SXMLEntity five;
    five.DType = SXMLEntity::EType::CharData;
    five.DNameData = "\"test cases\" are so difficult to consider \'all\'";
    result2 += "&quot;test cases&quot; are so difficult to consider &apos;all&apos;";
    EXPECT_TRUE(writer2.WriteEntity(five));
    EXPECT_EQ(sink2->String(), result2);

    EXPECT_TRUE(writer2.Flush());
    result2 +="</PE>";
    EXPECT_EQ(sink2->String(), result2); //test for flush input all the end element 

    first.DType = SXMLEntity::EType::CompleteElement; //not test
    first.DNameData = "market";
    first.SetAttribute("fruit","grapes&pears");
    first.SetAttribute("veg","potato&tomato");
    EXPECT_TRUE(writer2.WriteEntity(first));
    result2 += "<market fruit=\"grapes&amp;pears\" veg=\"potato&amp;tomato\"/>";
    EXPECT_EQ(sink2->String(), result2);
}



auto sink3 = std::make_shared<CStringDataSink>();
CXMLWriter writer3(sink3);

TEST(XMLWriter, WriteRowTest_speciallogic){
    std::string result3 ="";
    SXMLEntity first;
    first.DType = SXMLEntity::EType::EndElement;
    first.DNameData = "market";
    EXPECT_FALSE(writer3.WriteEntity(first));

    SXMLEntity second;
    second.DType = SXMLEntity::EType::StartElement;
    second.DNameData = "USA";
    second.SetAttribute("school","ucd ucla ucsd");
    EXPECT_TRUE(writer3.WriteEntity(second));
    result3 += "<USA school=\"ucd ucla ucsd\">";
    EXPECT_EQ(sink3->String(), result3);

    SXMLEntity third;
    third.DType = SXMLEntity::EType::StartElement;
    third.DNameData = "China";
    third.SetAttribute("city","NJ BJ SH");
    EXPECT_TRUE(writer3.WriteEntity(third));
    result3 += "<China city=\"NJ BJ SH\">";
    EXPECT_EQ(sink3->String(), result3);

    SXMLEntity si;
    si.DType = SXMLEntity::EType::EndElement; //test for end does not match
    si.DNameData = "USA";
    EXPECT_FALSE(writer3.WriteEntity(si));

    EXPECT_TRUE(writer3.Flush()); //correct order for flush
    result3 += "</China>";
    result3 += "</USA>";
    EXPECT_EQ(sink3->String(), result3);
}

TEST(XMLReader, Reader1StartTEST){
    std::string xmlString = sink1->String();
    auto source = std::make_shared<CStringDataSource>(xmlString);
    CXMLReader reader(source);
    SXMLEntity entity;
    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "example");
    std::pair<std::string, std::string> testPair = {"attr", "Hello World"};
    EXPECT_EQ(entity.DAttributes, std::vector<SXMLEntity::TAttribute>{testPair});

    EXPECT_TRUE(reader.ReadEntity(entity, true));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "ok");
    testPair = {"name","Bob"};
    EXPECT_EQ(entity.DAttributes, std::vector<SXMLEntity::TAttribute>{testPair});

    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "ok");

    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "example");
    EXPECT_TRUE(entity.DAttributes.empty());


}


TEST(XMLReader, Reader2StartTEST){
    std::string xmlString = sink2->String();
    auto source = std::make_shared<CStringDataSource>(xmlString);
    CXMLReader reader(source);
    SXMLEntity entity;
    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "PE");
    std::pair<std::string, std::string> testPair1 = {"sport", "football>basketball"};
    std::pair<std::string, std::string> testPair2 = {"player", "CR7<Messi"};
    std::vector<SXMLEntity::TAttribute> testAttributes = {testPair1, testPair2};
    EXPECT_EQ(entity.DAttributes, testAttributes);

    EXPECT_TRUE(reader.ReadEntity(entity,true));//Skip is true, so igore the chardata
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "PE");
    EXPECT_TRUE(entity.DAttributes.empty());
}


TEST(XMLReader, Reader3StartTEST){
    std::string xmlString = sink3->String();
    auto source = std::make_shared<CStringDataSource>(xmlString);
    CXMLReader reader(source);
    SXMLEntity entity;
    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "USA");
    std::pair<std::string, std::string> testPair = {"school", "ucd ucla ucsd"};
    EXPECT_EQ(entity.DAttributes, std::vector<SXMLEntity::TAttribute>{testPair});
}
