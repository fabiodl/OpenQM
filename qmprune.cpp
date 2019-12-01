#include "implicant.h"
#include "qm.h"
#include <iostream>
#include <filesystem>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <bitset>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;


static inline void tokenize(std::vector<std::string>& tokens,const std::string& s){
  std::stringstream ss;
  ss<<s;
  while(ss){
    std::string t;
    ss>>t;
    if (t.length()>0){
      tokens.emplace_back(t);
    }
  }
}

int main(int argc,char** argv){
    
  if (argc<3){
    cout<<"Usage: "<<argv[0]<<" n_inputs n_outputs inputfile [namesfile]"<<std::endl;
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


  if (n_outputs>8){
    std::cerr<<"n_outputs>8 currently unimplemented"<<std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::string> innames,outnames;

  if (argc<5){
    
    for (size_t i=0;i<n_inputs;i++){    
      std::string name;
      name.push_back('a'+i);
      innames.push_back(name);
    }


    for (size_t i=0;i<n_outputs;i++){    
      std::stringstream ss;
      ss<<"o"<<std::dec<<i;
      outnames.push_back(ss.str());
    }


  }else{
    ifstream namefile(argv[4]);
    std::string inputNames,outputNames;
    getline(namefile, inputNames);
    getline(namefile, outputNames);
    tokenize(innames,inputNames);
    tokenize(outnames,outputNames);    
    if (innames.size()!=n_inputs){
      std::cerr<<"Names file has "<<innames.size()<<" input names, expected "<<n_inputs<<" inputs"<<std::endl;
      return EXIT_FAILURE;
    }
    if (outnames.size()!=n_outputs){
      std::cerr<<"Names file has "<<outnames.size()<<" output names, expected "<<n_outputs<<" outputs"<<std::endl;
      return EXIT_FAILURE;
    }
    
  }
  

  ifstream inpf(argv[3],ios::binary);

  
  std::vector<uint8_t> data(expectedSize);
  inpf.read((char*)data.data(),expectedSize);
  

  std::vector<uint8_t> influences(n_inputs,0);
  
  for (size_t i=0;i<n_inputs;i++){
    //std::cout<<"Examine input "<<i<<std::endl;
    for (size_t upperBits=0;upperBits<static_cast<size_t>(1<<(n_inputs-i-1));upperBits++){    
      for (size_t lowerBits=0;lowerBits<static_cast<size_t>(1<<i);lowerBits++){

        size_t laddr=(upperBits<<(i+1))|lowerBits;
        size_t haddr=laddr|(1<<i);
        influences[i]|=data[laddr]^data[haddr];
        /*std::cout<<"checking "<< bitset<4>(laddr) <<"(="<<std::hex<<(uint)data[laddr]<<std::dec<<")"
                 <<"vs"<<bitset<4>(haddr)<<"(="<<std::hex<<(uint)data[haddr]<<std::dec<<")"
                 <<"xor"<<std::hex<<(uint)(data[laddr]^data[haddr])<<" infl " <<(uint)influences[i]<<dec
                 <<endl;*/        
      }
    }    
  }

  /*
  for (size_t i=0;i<n_inputs;i++){
    std::cout<<"Input "<<std::dec<<i<<" influences "<<std::hex<<(uint)influences[i]<<std::dec<<std::endl;
  }
  */
  
  for (size_t j=0;j<n_outputs;j++){
    std::vector<Implicant> list;
    std::vector<char> buffer(n_inputs+1);
    std::vector<size_t> realInputs;
    for (size_t i=0;i<n_inputs;i++){
      if (influences[i]& (1<<j)){
        realInputs.push_back(i);
      }
    }
    size_t realSize=realInputs.size();
    if (!realSize){
      std::cout<<outnames[j]<<"="<<((data[0] & (1<<j))?'1':'0')<<std::endl;
      continue;
    }
    
    buffer[realSize]=0;

    std::vector<string> realNames(realSize);
    for (size_t b=0;b<realSize;b++){
      realNames[b]=innames[realInputs[b]];
    }

    for (size_t comb=0;comb<static_cast<size_t>(1<<realSize);++comb){
      size_t addr=0;
      for (size_t b=0;b<realSize;b++){        
        if (comb&1<<b){
          addr|=1<<realInputs[b];
        }
      }
      if (data[addr]&(1<<j)){
        for (size_t b=0;b<realSize;++b){
          buffer[realSize-1-b]=(comb&(1<<b))?'1':'0';
        }
        list.emplace_back(Implicant(buffer.data()));        
      }      
    }
    std::vector<Implicant> solution = makeQM(list, {});    
    cout << outnames[j]<<"="<<getVerilogExpression(solution,realNames)<<";"<<endl;

   
  }//for j:noutputs

  
  
}
