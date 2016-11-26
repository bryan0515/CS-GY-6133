/*
 Cache Simulator
 Level one L1 and level two L2 cache parameters are read from file (block size, line per set and set per cache).
 The 32 bit address is divided into tag bits (t), set index bits (s) and block offset bits (b)
 s = log2(#sets)   b = log2(block size)  t=32-s-b
 */

/* I'm assuming it has a 4GB memory */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>

using namespace std;
//access state:
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss




struct config{
    int L1blocksize;
    int L1setsize;
    int L1size;
    int L2blocksize;
    int L2setsize;
    int L2size;
};

class cache {
public:
    
    int indexBits;
    int offSetBits;
    int tagBits;
    int virtualCachSize;
    
    bool *valids;
    int *tags;
    
    /* size is KB. Haven't been converted yet!!! */
    
//    int arr[x][y] ---> int arr[x*y];
//    
//    arr[2][4]= something ---> arr[2+4*x]= something;

    
    cache(int blocksize,
          int setsize,
          int size) {
        int blocks = size / blocksize / setsize;
        &valids = new bool[setsize * blocks];
        &tags = new int[setsize * blocks];
        indexBits = log(blocks) /log(2);
        offSetBits = log(blocksize) / log(2);
        tagBits = 32 - indexBits - offSetBits;
        virtualCachSize = size / setsize;
    }
    
    bool read(unsigned int addr) {
        int tag = getIndex(addr);
        int inidex = getTag(addr);
        return valids[index] == 1 && tags[index] == tag;
    }
    
    bool write() {
        return false;
    }
    
private:
    
    int getIndex(unsigned int addr) {
        return addr / virtualCachSize;
    }
    
    int getTag(unsigned int addr) {
        return addr % virtualCachSize;
    }

};


int main(int argc, char* argv[]){
    
    
    
    config cacheconfig;
    ifstream cache_params;
    string dummyLine;
    cache_params.open(argv[1]);
    while(!cache_params.eof())  // read config file
    {
        cache_params>>dummyLine;
        cache_params>>cacheconfig.L1blocksize;
        cache_params>>cacheconfig.L1setsize;
        cache_params>>cacheconfig.L1size;
        cache_params>>dummyLine;
        cache_params>>cacheconfig.L2blocksize;
        cache_params>>cacheconfig.L2setsize;
        cache_params>>cacheconfig.L2size;
    }
    

    // initialize the hirearch cache system with those configs
    cache L1 = cache(cacheconfig.L1blocksize, cacheconfig.L1setsize, cacheconfig.L1setsize);
    cache L2 = cache(cacheconfig.L2blocksize, cacheconfig.L2setsize, cacheconfig.L2setsize);
    
    
    
    int L1AcceState =0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
    int L2AcceState =0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;
    
    
    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string(argv[2]) + ".out";
    
    traces.open(argv[2]);
    tracesout.open(outname.c_str());
    
    string line;
    string accesstype;  // the Read/Write access type from the memory trace;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;
    
    if (traces.is_open()&&tracesout.is_open()){
        while (getline (traces,line)){   // read mem access file and access Cache
            
            istringstream iss(line);
            if (!(iss >> accesstype >> xaddr)) {break;}
            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);
            
            
            // access the L1 and L2 Cache according to the trace;
            if (accesstype.compare("R")==0)
                
            {
                //Implement by you:
                // read access to the L1 Cache,
                //  and then L2 (if required),
                //  update the L1 and L2 access state variable;
                if (!L1.read()) {
                    L2.read();
                }
                
                
                
                
                
                
            }
            else
            {
                //Implement by you:
                // write access to the L1 Cache,
                //and then L2 (if required),
                //update the L1 and L2 access state variable;
                if (!L1.write()) {
                    L2.write();
                }
                
                
                
                
                
            }
            
            
            
            tracesout<< L1AcceState << " " << L2AcceState << endl;  // Output hit/miss results for L1 and L2 to the output file;
            
            
        }
        traces.close();
        tracesout.close();
    }
    else cout<< "Unable to open trace or traceout file ";
    
    
    
    
    
    
    
    return 0;
}
