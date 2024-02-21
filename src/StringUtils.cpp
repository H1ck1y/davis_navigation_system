#include "StringUtils.h"

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    std::string result =  "";
    int newstart = start;
    int newend = end;
     if (str == ""){
        return "";
    }
    if (newstart < 0){
             newstart = str.length()+ newstart;}
    if (newend < 0){
             newend = str.length() + newend;}
    if (newend == 0 || newend > str.length()){ 
        result = str.substr(newstart, str.length() - newstart);
    } else {
        if (newend <= newstart || newstart < 0 || newstart >= str.length()||newend < 0){
        return "";
        }
        result = str.substr(newstart,newend - newstart);}
    return result; //come up with the idea of using substr in https://cplusplus.com/reference/string/string/substr/
}

std::string Capitalize(const std::string &str) noexcept{
    std::string result = str;
    if (str == ""){
        result = "";
    }
    else{
        for (int i = 0; i<str.length(); i++){
            if (str[i] != ' '&& 'a' <= str[i]<= 'z'){
             result[i] = std::toupper(str[i]);
             for (int j = i + 1; j< str.length(); j++){
                if ('A' <= str[j] <='Z'){
                    result[j] = std:: tolower(str[j]);}
             }// come up with the idea of using toupper in https://cplusplus.com/reference/cctype/toupper/
             break;}
        }
    }
    return result;
}

std::string Upper(const std::string &str) noexcept{
    std::string result = str;
    if (str == ""){
        return "";
    }else{
        for(int i = 0; i<str.length();  i++){
            if('a' <=str[i] <= 'z'){
                result[i] = std::toupper(str[i]);
            }
        }
    }
    return result;
}

std::string Lower(const std::string &str) noexcept{
    std::string result = str;
    if (str == ""){
        return "";
    }else{
        for(int i = 0; i<str.length();  i++){
            if('A' <=str[i] <= 'Z'){
                result[i] = std::tolower(str[i]);
            }
        }
    }
    return result;
}

std::string LStrip(const std::string &str) noexcept{
    std::string result = str;
    if (str == ""){
        return "";
    }else{
        for(int i = 0; i<str.length(); i++){
            if(str[i]!= ' '){
                result = Slice(result,i);    
                break;
            }
       }
    }
    return result;
}

std::string RStrip(const std::string &str) noexcept{
    std::string result = str;
    if (str == ""){
        return "";
    }else{
        for(int i = str.length()-1; i>=0; i--){
            if(str[i]!= ' '){
                result = Slice(result,0,i+1);    
                break;
            }
       }
    }
    return result;
}


std::string Strip(const std::string &str) noexcept{
    std::string result = str;
    result = RStrip(result);
    result = LStrip(result);
    return result;
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    std::string result = str;
    int lentgh_fill = (width - str.length()) / 2; 
    bool check_dividle;
    if ((width - str.length()) % 2 == 0){
        check_dividle =  true;
    }else {
        check_dividle = false ;
    }
    if ( str == "" )
      {return std::string(width, fill);}//got the idea to use std::string(width,fill)this https://cplusplus.com/reference/string/string/string/
    if(str.length() >= width){
        return result;
    }
    else{
        if (fill == ' '){
            if (check_dividle == true){
                result = std::string(lentgh_fill, fill)+ result + std::string(lentgh_fill,fill);
            }else {
                result = std::string(lentgh_fill, fill)+ result + std::string(lentgh_fill + 1,fill);
            }
        }else{
            if (check_dividle == true){
                result = std::string(lentgh_fill, fill)+ result + std::string(lentgh_fill,fill);
            }else {
                result = std::string(lentgh_fill, fill)+ result + std::string(lentgh_fill + 1,fill);
        }
    }
    return result;
    }
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    int lentgh_fill = (width - str.length());
    std::string result = str;
    if ( str == "" )
      {return std::string(width, fill);}
    if(str.length() >= width){
        return result;
    }else{
        if (fill == ' '){
            result = result + std::string(lentgh_fill, fill);
        }else {
            result = result + std::string(lentgh_fill, fill);
        }
    }
    return result;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    int lentgh_fill = (width - str.length());
    std::string result = str;
    if ( str == "" )
      {return std::string(width, fill);}
    if(str.length() >= width){
        return result;
    }else{
        if (fill == ' '){
            result = std::string(lentgh_fill, fill) + result;
        }else {
            result = std::string(lentgh_fill, fill) + result;
        }
    }
    return result;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    std::string result = str;
    std::string special_result = rep;
    std::string olds = old;
    std::string reps = rep;
    int len_old = olds.length();
    int i = 0;
    if (olds == ""){ //when olds is none, according to python compiler, add new before and after each character but cannot overadding.
        for (int j = 0; j< str.length(); j++){
                special_result = special_result + str[j] + reps;    
            }
        return special_result;
    }
    while(i <= result.length()-len_old){ // this is normal case
            if (Slice(result,i,i+len_old) == old){
               result = result.substr(0,i) + reps + result.substr(i+len_old);
               i = i + reps.length();
            }else{
                i = i + 1;
            }
    }
    return result;
}
    

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    std:: string result = str;
    std:: string splt_sign;
    std::vector<std::string> v = {};
    int i = 0;
    int prev = 0;
    int countfor_del = 0;
    if (str == ""){
        return {};
    }
    if (splt==""){
        splt_sign = " ";
    }else{
        splt_sign = splt;
    }

    int len_splt = splt_sign.length();

    while(i <= result.length()- len_splt){ 
            if(result[i] == '\"'){
                countfor_del = countfor_del + 1;
            }
            if (Slice(result,i,i+len_splt) == splt_sign && countfor_del %2 == 0){
                v.push_back(result.substr(prev, i-prev));
                prev = i + len_splt;
                i = prev;
            }else{
                 i = i + 1;
            }
    }//this is only for adding the string before splt

    v.push_back(result.substr(prev));//so add the last one
    return v;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    std::string result = ""; 
    if (vect.size() == 0){
        return "";
    }
    for(int i = 0; i< vect.size();i++)//learn to access elements from vector https://www.includehelp.com/stl/different-ways-to-access-elements-from-a-vector-in-cpp-stl.aspx
        result = result + vect[i] + str;
    if (str!=""){
    for (int j = 0;j<str.length(); j++)
        result.pop_back();}//learn how to delete the last item https://stackoverflow.com/questions/2310939/remove-last-character-from-c-string
    return result;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    if (str == "")
    {
        return "" ;
    }
    std::string result = str;
    int i = 0;
    while(i< result.length()){
        if(result[i] != '\t'){
          i = i + 1;
        }else{
            int space_num = (tabsize - i % tabsize);
            result = result.substr(0,i) +  std::string(space_num,' ')+ result.substr(i+1);
            i = i + space_num;
        }
    }
    return result;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    //https://en.wikipedia.org/wiki/Levenshtein_distance#Iterative_with_full_matrix
    std::string left_convert = left;
    std::string right_convert = right;
    int left_len = left_convert.length();
    int right_len = right_convert.length();
    int cost;
    int minimum;
    if (ignorecase == true){
        left_convert = Lower(left);
        right_convert = Lower(right);}
    std::vector<std::vector<int> > matric(left_len + 1, std::vector<int> (right_len + 1)); // create a matric like by using 2D vector
    for (int i = 0; i < left_len + 1; i++){// need to consider empty string so plus 1
        matric[i][0] = i;
    }
    for (int j = 0; j< right_len + 1; j++){
        matric[0][j] = j;
    }
    for (int j = 1; j <right_len  + 1; j++){ //start form second colume and second row
        for (int i = 1; i < left_len + 1 ;i++){
            if (left_convert[i-1] ==  right_convert[j-1]){
                cost = 0;
            }else{
                cost = 1;
            }

             minimum = matric[i - 1][j] + 1;
            if (matric[i][j - 1] + 1 < minimum) { 
            minimum = matric[i][j - 1] + 1;
            }
            if (matric[i - 1][j - 1] + cost < minimum) {
            minimum = matric[i - 1][j - 1] + cost;
            }

            matric[i][j] = minimum;// take a square of 2*2 as a unit and implement the blank of right diagnol one;
        }
    }
    return matric[left_len][right_len];
}

};