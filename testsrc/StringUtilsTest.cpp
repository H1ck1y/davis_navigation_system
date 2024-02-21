#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    std::string Base = "hello";
    //end == 0 or end is bigger than strlen
    EXPECT_EQ(StringUtils::Slice(Base,1),"ello");
    EXPECT_EQ(StringUtils::Slice(Base,0),"hello");
    EXPECT_EQ(StringUtils::Slice(Base,-1),"o");
    EXPECT_EQ(StringUtils::Slice(Base,-5),"hello");
    EXPECT_EQ(StringUtils::Slice(Base,-5,6),"hello");
    EXPECT_EQ(StringUtils::Slice(Base,0,6),"hello");
    //end != 0
    EXPECT_EQ(StringUtils::Slice(Base,-5,4),"hell");
    EXPECT_EQ(StringUtils::Slice(Base,1,5),"ello");//regular
    EXPECT_EQ(StringUtils::Slice(Base,1,-3),"e");//end negative
    EXPECT_EQ(StringUtils::Slice(Base,-5,5),"hello");//start negative
    EXPECT_EQ(StringUtils::Slice(Base,-5,-1),"hell");//both negative
    EXPECT_EQ(StringUtils::Slice(Base,1,-4),""); //edge case
    EXPECT_EQ(StringUtils::Slice(Base,5,2),"");//end < start, wrong 
    EXPECT_EQ(StringUtils::Slice(Base,5,-5),"");// end <start, wrong
    EXPECT_EQ(StringUtils::Slice(Base,-1,1),"");// end <start, wrong
    EXPECT_EQ(StringUtils::Slice(Base,-6,5),"");//start < 0 after trasferring
    EXPECT_EQ(StringUtils::Slice(Base,-7,1),"");//start < 0 after transferring
    EXPECT_EQ(StringUtils::Slice(Base,5,5),"");//start >= len ,end =len
    Base = "";
    EXPECT_EQ(StringUtils::Slice(Base,5,5),"");
}

TEST(StringUtilsTest, Capitalize){
    std::string Base = "hello";
    EXPECT_EQ(StringUtils::Capitalize(Base),"Hello");
    Base = "?hello";
    EXPECT_EQ(StringUtils::Capitalize(Base),"?hello");
    Base = " hello";
    EXPECT_EQ(StringUtils::Capitalize(Base)," Hello");
    Base = " ;hello";
    EXPECT_EQ(StringUtils::Capitalize(Base)," ;hello");
    Base = "Hello";
    EXPECT_EQ(StringUtils::Capitalize(Base),"Hello");
    Base = "";
    EXPECT_EQ(StringUtils::Capitalize(Base),"");
    Base = "HELLO";
    EXPECT_EQ(StringUtils::Capitalize(Base),"Hello");
    Base = "HELLO wOrld";
    EXPECT_EQ(StringUtils::Capitalize(Base),"Hello world");

}

TEST(StringUtilsTest, Upper){
    std::string Base = "";
    EXPECT_EQ(StringUtils::Upper(Base),"");
    Base = "hello";
    EXPECT_EQ(StringUtils::Upper(Base),"HELLO");
    Base = "Hello myf";
    EXPECT_EQ(StringUtils::Upper(Base),"HELLO MYF");
    Base = " hEloo ?ff";
    EXPECT_EQ(StringUtils::Upper(Base)," HELOO ?FF");
    Base = " 123345";
    EXPECT_EQ(StringUtils::Upper(Base)," 123345");

}

TEST(StringUtilsTest, Lower){
    std::string Base ="";
    EXPECT_EQ(StringUtils::Lower(Base),"");
    Base = "HELLO";
    EXPECT_EQ(StringUtils::Lower(Base),"hello");
    Base = "HeLLO fF";
    EXPECT_EQ(StringUtils::Lower(Base),"hello ff");
    Base = " ?''HeLLO fF";
    EXPECT_EQ(StringUtils::Lower(Base)," ?''hello ff");
    Base = " 123345";
    EXPECT_EQ(StringUtils::Lower(Base)," 123345");
}

TEST(StringUtilsTest, LStrip){
    std::string Base="";
    EXPECT_EQ(StringUtils::LStrip(Base),"");
    Base = " Hello";
    EXPECT_EQ(StringUtils::LStrip(Base),"Hello");
    Base = "He2342llo";
    EXPECT_EQ(StringUtils::LStrip(Base),"He2342llo");
    Base = " coding is so tiredLOL 123432QRWQRWE   ";
    EXPECT_EQ(StringUtils::LStrip(Base),"coding is so tiredLOL 123432QRWQRWE   ");
    Base = "     !!!!! Istill have 9 fuctions!      ";
    EXPECT_EQ(StringUtils::LStrip(Base),"!!!!! Istill have 9 fuctions!      ");
}

TEST(StringUtilsTest, RStrip){
    std::string Base="";
    EXPECT_EQ(StringUtils::RStrip(Base),"");
    Base = "Hello ";
    EXPECT_EQ(StringUtils::RStrip(Base),"Hello");
    Base = "     NO CODE NEXT QUARTER PLS!!!   ";
    EXPECT_EQ(StringUtils::RStrip(Base),"     NO CODE NEXT QUARTER PLS!!!");
    Base = "     adsfasd5233 I AM MAD 34344";
    EXPECT_EQ(StringUtils::RStrip(Base),"     adsfasd5233 I AM MAD 34344");
    Base = " are there any other possibilities for testing??????   ??  ";
    EXPECT_EQ(StringUtils::RStrip(Base)," are there any other possibilities for testing??????   ??");
}

TEST(StringUtilsTest, Strip){
    std::string Base="";
    EXPECT_EQ(StringUtils::Strip(Base),"");
    Base="   Heloo   ";
    EXPECT_EQ(StringUtils::Strip(Base),"Heloo");
    Base="  this one is easy  ";
    EXPECT_EQ(StringUtils::Strip(Base),"this one is easy");
    Base="     ??123 WE just need to combine first2";
    EXPECT_EQ(StringUtils::Strip(Base),"??123 WE just need to combine first2");
    Base="cannotcomeup with more cases//p99                      ";
    EXPECT_EQ(StringUtils::Strip(Base),"cannotcomeup with more cases//p99");
}

TEST(StringUtilsTest, Center){
    std::string Base="";
    EXPECT_EQ(StringUtils::Center(Base,10),"          ");
    EXPECT_EQ(StringUtils::Center(Base,10,'*'),"**********");
    Base="Hello";
    EXPECT_EQ(StringUtils::Center(Base,4,'*'),"Hello");
    EXPECT_EQ(StringUtils::Center(Base,5,'*'),"Hello");
    EXPECT_EQ(StringUtils::Center(Base,6,'*'),"Hello*");
    EXPECT_EQ(StringUtils::Center(Base,7,'*'),"*Hello*");
    Base=" banana";
    EXPECT_EQ(StringUtils::Center(Base,15,'?'),"???? banana????");
    EXPECT_EQ(StringUtils::Center(Base,15),"     banana    ");
    EXPECT_EQ(StringUtils::Center(Base,16,'*'),"**** banana*****");
}

TEST(StringUtilsTest, LJust){
    std::string Base="";
    EXPECT_EQ(StringUtils::LJust(Base,10,'*'),"**********");
    EXPECT_EQ(StringUtils::LJust(Base,10),"          ");
    Base="hello";
    EXPECT_EQ(StringUtils::LJust(Base,5),"hello");
    EXPECT_EQ(StringUtils::LJust(Base,4),"hello");
    EXPECT_EQ(StringUtils::LJust(Base,6),"hello ");
    EXPECT_EQ(StringUtils::LJust(Base,7,'*'),"hello**");
    Base="b a?nana";
    EXPECT_EQ(StringUtils::LJust(Base,0),"b a?nana");
    EXPECT_EQ(StringUtils::LJust(Base,8),"b a?nana");
    EXPECT_EQ(StringUtils::LJust(Base,15),"b a?nana       ");
    EXPECT_EQ(StringUtils::LJust(Base,15,'*'),"b a?nana*******");
}

TEST(StringUtilsTest, RJust){
    std::string Base="";
    EXPECT_EQ(StringUtils::RJust(Base,10,'*'),"**********");
    EXPECT_EQ(StringUtils::RJust(Base,10),"          ");
    Base="hello";
    EXPECT_EQ(StringUtils::RJust(Base,5),"hello");
    EXPECT_EQ(StringUtils::RJust(Base,4),"hello");
    EXPECT_EQ(StringUtils::RJust(Base,6)," hello");
    EXPECT_EQ(StringUtils::RJust(Base,7,'*'),"**hello");
    Base="b a?nana";
    EXPECT_EQ(StringUtils::RJust(Base,0),"b a?nana");
    EXPECT_EQ(StringUtils::RJust(Base,8),"b a?nana");
    EXPECT_EQ(StringUtils::RJust(Base,15),"       b a?nana");
    EXPECT_EQ(StringUtils::RJust(Base,15,'*'),"*******b a?nana");
}

TEST(StringUtilsTest, Replace){
    std::string Base="hello world";
    EXPECT_EQ(StringUtils::Replace(Base,"hello","hi"),"hi world");
    EXPECT_EQ(StringUtils::Replace(Base,"h","uo"),"uoello world");
    EXPECT_EQ(StringUtils::Replace(Base,"ll","hi"),"hehio world");
    EXPECT_EQ(StringUtils::Replace(Base,"l","hi"),"hehihio worhid");
    Base = "i I like computerc";
    EXPECT_EQ(StringUtils::Replace(Base,"h","c"),"i I like computerc");
    EXPECT_EQ(StringUtils::Replace(Base,"h"," "),"i I like computerc");
    Base = "i need to take a rest";
    EXPECT_EQ(StringUtils::Replace(Base," ","??"),"i??need??to??take??a??rest");
    EXPECT_EQ(StringUtils::Replace(Base," ",""),"ineedtotakearest");
    EXPECT_EQ(StringUtils::Replace(Base,"","22"),"22i22 22n22e22e22d22 22t22o22 22t22a22k22e22 22a22 22r22e22s22t22");
    Base = "  i need to take a rest";
    EXPECT_EQ(StringUtils::Replace(Base,"","22"),"22 22 22i22 22n22e22e22d22 22t22o22 22t22a22k22e22 22a22 22r22e22s22t22");
}

TEST(StringUtilsTest, Split){
    std::string Base="hello";
    std::vector<std::string> v = {"he", "o"};
    EXPECT_EQ(StringUtils::Split(Base,"ll"),v);
    v = {"he","","o"};
    EXPECT_EQ(StringUtils::Split(Base,"l"),v);
    Base=" hello world happy ending ";
    v = {"", "hello", "world", "happy", "ending", ""};
    EXPECT_EQ(StringUtils::Split(Base,""),v);
    Base="hello world";
    v = {"hello", "world"};
    EXPECT_EQ(StringUtils::Split(Base,""),v);
    v = {"","ello world"};
    EXPECT_EQ(StringUtils::Split(Base,"h"),v);
    Base="I like computerc";
    v = {"I like ", "omputer", ""};  
    EXPECT_EQ(StringUtils::Split(Base,"c"),v);
    v = {"I", "like", "computerc"};
    EXPECT_EQ(StringUtils::Split(Base),v);
    Base = "";
    v = {};
    EXPECT_EQ(StringUtils::Split(Base),v);
}


TEST(StringUtilsTest, Join){
    std::vector<std::string> v = {"a", "b","c","d"};
    EXPECT_EQ(StringUtils::Join("!",v),"a!b!c!d");
    EXPECT_EQ(StringUtils::Join(" ",v),"a b c d");
    EXPECT_EQ(StringUtils::Join("",v),"abcd");
    v = {};
    EXPECT_EQ(StringUtils::Join("?",v),"");
    v = {"happy", "every","day"};
    EXPECT_EQ(StringUtils::Join("!!",v),"happy!!every!!day");
    EXPECT_EQ(StringUtils::Join("!!!",v),"happy!!!every!!!day");
}

TEST(StringUtilsTest, ExpandTabs){
    std::string Base="";
    EXPECT_EQ(StringUtils::ExpandTabs(Base,4),"");
    Base = "he\tllo";
    EXPECT_EQ(StringUtils::ExpandTabs(Base,8),"he      llo");
    Base = "H\te\tl\tl\to";
    EXPECT_EQ(StringUtils::ExpandTabs(Base,2),"H e l l o");
    EXPECT_EQ(StringUtils::ExpandTabs(Base,4),"H   e   l   l   o");
    Base = "\tH\te\tl\tl\to";
    EXPECT_EQ(StringUtils::ExpandTabs(Base,4),"    H   e   l   l   o");
    Base = "Hll\t\t\tlllllll\to";
    EXPECT_EQ(StringUtils::ExpandTabs(Base,2),"Hll     lllllll o");
    Base = "He\tl\tlo b\toyy";
    EXPECT_EQ(StringUtils::ExpandTabs(Base,4),"He  l   lo b    oyy");
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(StringUtils::EditDistance("karolin","kathrin"),3);
    EXPECT_EQ(StringUtils::EditDistance("karolin","kathriN"),4);
    EXPECT_EQ(StringUtils::EditDistance("KArolin","kathriN"),6);
    EXPECT_EQ(StringUtils::EditDistance("KArolin","kathriN", true),3);
    EXPECT_EQ(StringUtils::EditDistance("back","booking"),5);
    EXPECT_EQ(StringUtils::EditDistance("back","booKing"),6);
    EXPECT_EQ(StringUtils::EditDistance("Back","booKing"),7);
    EXPECT_EQ(StringUtils::EditDistance("Back","booKing",true),5);
    EXPECT_EQ(StringUtils::EditDistance("Back",""),4);
    EXPECT_EQ(StringUtils::EditDistance("",""),0);
}
