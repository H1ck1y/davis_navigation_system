# include "DSVWriter.h" 
#include "StringUtils.h"
struct CDSVWriter::SImplementation
{
    std::shared_ptr< CDataSink > sink;
    char delimiter;
    bool quoteall = false;
    bool Firstwrite;
};


CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall){
    DImplementation  = std::make_unique<SImplementation>() ;
    DImplementation->sink = sink;
    DImplementation->delimiter = delimiter;
    DImplementation->quoteall = quoteall;
    DImplementation->Firstwrite = true;
}
CDSVWriter:: ~CDSVWriter(){
}


// Returns true if the row is successfully written, one string per column
// should be put in the row vector

 bool  CDSVWriter::WriteRow(const std::vector< std::string > &row){
    std::string result;
    std::string del;
    if (DImplementation->delimiter == '\"'){  //convert " into delimiter if it is a delimiter
        DImplementation->delimiter = ',';

    }
    del.assign(1,DImplementation->delimiter);
    std::vector <std::string> container = row;
    std::vector <char> container2 = {};

    for(int i = 0; i< container.size();i++){
      if (container[i].find('\"')!= std::string::npos){
      container[i] = StringUtils::Replace(container[i],"\"", "\"\""); //replace " with "" first
      }

      if (DImplementation->quoteall == true){  // if quote all, then quote all of them 
        container [i] = "\"" + container[i] + "\"";
      }else{
      //https://blog.csdn.net/Hao_ge_666/article/details/90695409
      if (container[i].find(DImplementation->delimiter)!= std::string::npos||
                            container[i].find('\n')!= std::string::npos||
                            container[i].find('\"')!= std::string::npos){ //add quote for special character
          container [i] = "\"" + container[i] + "\"";           
         }                         
      }
  }
  result = StringUtils::Join (del, container); 

  if (DImplementation ->Firstwrite){
      DImplementation ->Firstwrite = false;
  }else{
      container2.push_back('\n');
  }
  for (int i = 0; i<result.length();i++){//saperate final string into the vector write it into the sink
      container2.push_back(result[i]);
  }
  DImplementation -> sink -> Write(container2);
  return true;
}