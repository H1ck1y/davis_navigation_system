#include <gtest/gtest.h>
#include "DSVWriter.h"
#include "StringDataSink.h"
#include "DSVReader.h"
#include "StringDataSource.h"

auto sink1 = std::make_shared<CStringDataSink>();
CDSVWriter writer1(sink1, '/');
auto sink2 = std::make_shared<CStringDataSink>(); // test for quoteall
CDSVWriter writer2(sink2, '/', true);
auto sink3 = std::make_shared<CStringDataSink>(); // test for quotation mark as delimiter
CDSVWriter writer3(sink3, '\"', true);


TEST(DSVWriter, WriteRowTest){

    std::string result;

    std::vector<std::string> base1 = {"ECS", "MAT", "UWP"};
    EXPECT_TRUE(writer1.WriteRow(base1));
    result += "ECS/MAT/UWP";
    EXPECT_EQ(sink1->String(), result);

    std::vector<std::string> base2 = {"\"CHN\"", "FRE", "JPN"};
    EXPECT_TRUE(writer1.WriteRow(base2));
    result += "\n\"\"\"CHN\"\"\"/FRE/JPN";
    EXPECT_EQ(sink1->String(), result);

    std::vector<std::string> base3 = {"/PHY"};
    EXPECT_TRUE(writer1.WriteRow(base3));
    result += "\n\"/PHY\"";
    EXPECT_EQ(sink1->String(), result);

    std::vector<std::string> base4 = {"/CHE\n"};
    EXPECT_TRUE(writer1.WriteRow(base4));
    result += "\n\"/CHE\n\"";
    EXPECT_EQ(sink1->String(), result);

    std::vector<std::string> base5 = {};
    EXPECT_TRUE(writer1.WriteRow(base5));
    result += "\n";
    EXPECT_EQ(sink1->String(), result);

    std::vector<std::string> base6 = {"/CHI\"\"\"\"\n"};
    EXPECT_TRUE(writer1.WriteRow(base6));
    result += "\n\"/CHI\"\"\"\"\"\"\"\"\n\"";
    EXPECT_EQ(sink1->String(), result);

    result = "";

    EXPECT_TRUE(writer2.WriteRow(base1));

    result += "\"ECS\"/\"MAT\"/\"UWP\"";

    EXPECT_EQ(sink2->String(), result);

    EXPECT_TRUE(writer2.WriteRow(base2));
    result += "\n\"\"\"CHN\"\"\"/\"FRE\"/\"JPN\"";
    EXPECT_EQ(sink2->String(), result);

    EXPECT_TRUE(writer2.WriteRow(base3));
    result += "\n\"/PHY\"";
    EXPECT_EQ(sink2->String(), result);

    EXPECT_TRUE(writer2.WriteRow(base4));
    result += "\n\"/CHE\n\"";
    EXPECT_EQ(sink2->String(), result);

    result = "";

    EXPECT_TRUE(writer3.WriteRow(base1));

    result += "\"ECS\",\"MAT\",\"UWP\"";

    EXPECT_EQ(sink3->String(), result);

    EXPECT_TRUE(writer3.WriteRow(base2));
    result += "\n\"\"\"CHN\"\"\",\"FRE\",\"JPN\"";
    EXPECT_EQ(sink3->String(), result);

    EXPECT_TRUE(writer3.WriteRow(base3));
    result += "\n\"/PHY\"";
    EXPECT_EQ(sink3->String(), result);

    EXPECT_TRUE(writer3.WriteRow(base4));
    result += "\n\"/CHE\n\"";
    EXPECT_EQ(sink3->String(), result);
}


TEST(DSVReader,ReadRowTest){
    //test first sink
    std::string dsvstring = sink1->String();
    auto source1 = std::make_shared<CStringDataSource>(dsvstring);
    CDSVReader reader1(source1, '/');
    std::vector<std::string> expectedrow1 = {"ECS", "MAT", "UWP"};
    std::vector<std::string> row;
    EXPECT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row, expectedrow1);
    std::vector<std::string> expectedrow2 = {"\"CHN\"", "FRE", "JPN"};
    EXPECT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row, expectedrow2);
    std::vector<std::string> expectedrow3 = {"/PHY"};
    EXPECT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row, expectedrow3);
    std::vector<std::string> expectedrow4 = {"/CHE\n"};
    EXPECT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row, expectedrow4);
    EXPECT_FALSE(reader1.ReadRow(row));
    std::vector<std::string> expectedrow6 = {"/CHI\"\"\"\"\n"};
    EXPECT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row, expectedrow6);

    // Test for second sink
    dsvstring = sink2->String();
    auto source2 = std::make_shared<CStringDataSource>(dsvstring);
    CDSVReader reader2(source2, '/');

    EXPECT_TRUE(reader2.ReadRow(row));
    EXPECT_EQ(row, expectedrow1);

    EXPECT_TRUE(reader2.ReadRow(row));
    EXPECT_EQ(row, expectedrow2);

    EXPECT_TRUE(reader2.ReadRow(row));
    EXPECT_EQ(row, expectedrow3);

    EXPECT_TRUE(reader2.ReadRow(row));
    EXPECT_EQ(row, expectedrow4);

    EXPECT_FALSE(reader2.ReadRow(row));

    // Test for third sink
    dsvstring = sink3->String();
    auto source3 = std::make_shared<CStringDataSource>(dsvstring);
    CDSVReader reader3(source3, '\"');

    EXPECT_TRUE(reader3.ReadRow(row));
    EXPECT_EQ(row, expectedrow1);

    EXPECT_TRUE(reader3.ReadRow(row));
    EXPECT_EQ(row, expectedrow2);

    EXPECT_TRUE(reader3.ReadRow(row));
    EXPECT_EQ(row, expectedrow3);

    EXPECT_TRUE(reader3.ReadRow(row));
    EXPECT_EQ(row, expectedrow4);

    EXPECT_FALSE(reader3.ReadRow(row));


}