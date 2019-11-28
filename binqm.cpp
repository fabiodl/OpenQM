#include "implicant.h"
#include "qm.h"
#include <iostream>
#include <filesystem>
#include <cmath>
#include <cstdlib>
#include <fstream>


using namespace std;
namespace fs = std::filesystem;


inline static void printRevbin(char* buff,size_t val,size_t width){
  for (size_t i=0;i<width;i++){
    buff[i]=(val&(1<<i))?'1':'0';
  }
}


int main(int argc,char** argv){

  if (argc<3){
    cout<<"Usage: "<<argv[0]<<" n_inputs n_outputs inputfile"<<std::endl;
    return EXIT_SUCCESS;
  }
  size_t n_inputs=static_cast<size_t>(atoi(argv[1]));
  size_t n_outputs=static_cast<size_t>(atoi(argv[2]));

  size_t output_bytes=ceil(n_outputs/8.0);
  size_t expectedSize=output_bytes*(1<<n_inputs);
  auto path=fs::path(argv[3]);
  size_t fileSize=fs::file_size(path);
  
  if (fileSize !=expectedSize){
    std::cerr<<"File size is"<<fileSize<<" expected "<<expectedSize<<std::endl;
    return EXIT_FAILURE;
  }

  ifstream inpf(argv[3],ios::binary); 

  
  std::vector<std::vector<Implicant> > lists(n_outputs);

  char buffer[n_inputs+1];
  buffer[n_inputs]=0;

  std::cout<<"Reading..."<<std::flush;
  for (size_t i=0;i<expectedSize;i++){
    printRevbin(buffer,i,n_inputs);
    size_t val=0;
    for (size_t j=0;j<output_bytes;j++){
      val=(val<<8)|inpf.get();
    }
    for (size_t j=0;j<n_outputs;j++){
      if (val&(1<<j)){
        lists[j].emplace_back(Implicant(buffer));
      }
    }
  }
  std::cout<<"complete"<<std::endl;
  for (size_t j=0;j<n_outputs;j++){
    std::vector<Implicant> solution = makeQM(lists[j], {});    
    cout << endl << "Solution:" << endl;  
    for(Implicant i : solution) {
      cout << i << " (" << i.getStrCoverage() << ")" << endl;
    }
    cout << endl << "Expression: " << getBooleanExpression(solution) << endl;
  }
  return EXIT_SUCCESS;
  
}
