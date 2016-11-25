#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;  // for calling cin/cout
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
    public:
        bitset<32> ReadData1, ReadData2; 
     	RF()
    	{ 
          Registers.resize(32);  // resize vector
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            // implement the funciton by you.
            ReadData1 = Registers[RdReg1.to_ulong()];
            ReadData2 = Registers[RdReg2.to_ulong()];
            if (WrtEnable[0]) {
                Registers[WrtReg.to_ulong()] = WrtData;
            }
         }
		 
	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("/Users/bryancheng/Documents/NYU poly/Computer Architecture I/Labs/lab1/lab1/lab1/RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   
                 // implement the ALU operations by you.
                 switch (ALUOP.to_ulong()) {
                     case ADDU:
                         ALUresult = oprand1.to_ulong() + oprand2.to_ulong();
                         break;
                     case SUBU:
                         ALUresult = oprand1.to_ulong() - oprand2.to_ulong();
                         break;
                     case AND:
                         ALUresult = oprand1 & oprand2;
                         break;
                     case OR:
                         ALUresult = oprand1 | oprand2;
                         break;
                     case NOR:
                         ALUresult = oprand1 ^ oprand2;
                         break;
                     default:
                         break;
                 }
                 return ALUresult;
               }
};

class INSMem
{
    public:
        bitset<32> Instruction;
        INSMem() {
            IMem.resize(MemSize);
            ifstream imem;
            string line;
            int i=0;
            imem.open("/Users/bryancheng/Documents/NYU poly/Computer Architecture I/Labs/lab1/lab1/lab1/imem.txt");
            if (imem.is_open()) {
                while (getline(imem,line)) {
                    string temp = line.substr(0, line.size()-1);
                    IMem[i] = bitset<8>(temp);
                    i++;
                }
            }
            else cout<<"Unable to open file";
            imem.close();
        }
    
        bitset<32> ReadMemory (bitset<32> ReadAddress) {
            // implement by you. (Read the byte at the ReadAddress and the following three byte).
            long ReadAddressLong = ReadAddress.to_ulong();
            for (int i = 31; i >= 0; i-=8) {
                bitset<8> ImemByte = IMem[ReadAddressLong];
                ReadAddressLong++;
                for (int j = 0; j < 8; j++) {
                    Instruction[i-j] = ImemByte[7-j];
                }
            }
            return Instruction;
        }
      
      private:
           vector<bitset<8> > IMem;
      
};
      
class DataMem    
{
    public:
        bitset<32> readdata;
        DataMem()
        {
            DMem.resize(MemSize);
            ifstream dmem;
            string line;
            int i=0;
            dmem.open("/Users/bryancheng/Documents/NYU poly/Computer Architecture I/Labs/lab1/lab1/lab1/dmem.txt"); // check directory
            if (dmem.is_open())
            {
            while (getline(dmem,line))
                {
                string temp = line.substr(0, line.size()-1);    // change
                DMem[i] = bitset<8>(temp);  // change
                i++;
                }
            }
            else cout<<"Unable to open file";
            dmem.close();
        }
    
        bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
        {
            // implement by you.
            long AddressLong = Address.to_ulong();
            if (readmem[0]) {
                for (int i = 31; i >= 0; i-=8) {
                    cout << "DMemAddress ";
                    cout << AddressLong;
                    cout << '\n';
                    bitset<8> ImemByte = DMem[AddressLong];
                    AddressLong++;
                    for (int j = 0; j < 8; j++) {
                        readdata[i-j] = ImemByte[7-j];
                    }
                }
            } else if (writemem[0]) {
                for (int i = 31; i >= 0; i-=8) {
                    bitset<8> tempByte;
                    for (int j = 0; j <= 7; j++) {
                        tempByte[7-j] = WriteData[i-j];
                    }
                    cout << "store to DMem[";
                    cout << AddressLong;
                    cout << "]: ";
                    cout << tempByte;
                    cout << '\n';
                    DMem[AddressLong] = tempByte;
                    AddressLong++;
                }
            }
            return readdata;
          }   
                     
          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("/Users/bryancheng/Documents/NYU poly/Computer Architecture I/Labs/lab1/lab1/lab1/dmemresult.txt"); // check
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {     
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"Unable to open file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};


// main methods
bitset<6> getOpcode(bitset<32> Instruction) {
    bitset<6> opcode;
    for (int i = 0; i <= 5; i++) {
        opcode[5-i] = Instruction[31-i];
    }
    return opcode;
}

bitset<6> getFunct(bitset<32> Instruction) {
    bitset<6> funct;
    for (int i = 0; i <= 5; i++) {
        funct[i] = Instruction[i];
    }
    return funct;
}

bitset<5> getRs(bitset<32> Instruction) {
    bitset<5> Rs;
    for (int i = 0; i <= 4; i++) {
        Rs[4-i] = Instruction[25-i];
    }
    return Rs;
}

bitset<5> getRt(bitset<32> Instruction) {
    bitset<5> Rt;
    for (int i = 0; i <= 4; i++) {
        Rt[4-i] = Instruction[20-i];
    }
    return Rt;
}

bitset<5> getRd(bitset<32> Instruction) {
    bitset<5> Rd;
    for (int i = 0; i <= 4; i++) {
        Rd[4-i] = Instruction[15-i];
    }
    return Rd;
}

bitset<32> getExtendedImm(bitset<32> Instruction) {
    bitset<32> extendedImm;
    if (Instruction[15] == 0) {
        extendedImm = 0;
    } else {
        extendedImm = 1;
    }
    for(int i = 0; i <= 15; i++) {
        extendedImm[15-i] = Instruction[15-i];
    }
    return extendedImm;
}

bitset<32> concat(bitset<32> PC, bitset<32> Instruction) {
    bitset<32> concatenated = 0;
    for (int i = 0; i < 4; i++) {
        concatenated[31-i] = PC[31-i];
    }
    for (int i = 0; i < 26; i++) {
        concatenated[27-i] = Instruction[25-i];
    }
    return concatenated;
}

int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    bitset<32> PC = 0;
    printf("ready\n");
    while (1)
	{
        // Fetch
        bitset<32> Instruction = myInsMem.ReadMemory(PC);
        cout << "Instruction: ";
        cout << Instruction;
        cout << "\n";
        PC = PC.to_ulong() + 4;
        
        bitset<6> opcode = getOpcode(Instruction);
        bitset<5> Rs = getRs(Instruction); // 25:21
        bitset<5> Rt = getRt(Instruction); // 20:16
        bitset<5> Rd = getRd(Instruction); // 15:11
        
		// If current insturciton is "11111111111111111111111111111111", then break;
        if (Instruction.to_ulong() == 4294967295) break;
        
		// decode(Read RF)
        switch (opcode.to_ulong()) {
            case 0:  // R-type
                cout << "R-Type\n";
                myRF.ReadWrite(Rs, Rt, Rd, 0, 0);
                break;
            case 2:  // J
                cout << "J-type\n";
                PC = concat(PC, Instruction);
                break;
            default: // I-type
                cout << "I-type\n";
                myRF.ReadWrite(Rs, Rt, Rt, 0, 0);
                break;
        }
        cout << "ReadData1: ";
        cout << myRF.ReadData1;
        cout << "\n";
        cout << "ReadData2: ";
        cout << myRF.ReadData2;
        cout << "\n";
        
		// Execute
        bitset<32> myALUResult = 0;
        if (opcode == 0) { //R type
            bitset<6> funct = getFunct(Instruction);
            cout << "funct: ";
            cout << funct.to_ulong();
            cout << '\n';
            switch (funct.to_ulong()) {
                case 33:
                    myALUResult = myALU.ALUOperation(ADDU, myRF.ReadData1, myRF.ReadData2);
                    cout << "ADDUResult: ";
                    cout << myALUResult;
                    cout << '\n';
                    break;
                case 35:
                    myALUResult = myALU.ALUOperation(SUBU, myRF.ReadData1, myRF.ReadData2);
                    break;
                case 36:
                    myALUResult = myALU.ALUOperation(AND, myRF.ReadData1, myRF.ReadData2);
                    break;
                case 37:
                    myALUResult = myALU.ALUOperation(OR, myRF.ReadData1, myRF.ReadData2);
                    break;
                case 39:
                    myALUResult = myALU.ALUOperation(NOR, myRF.ReadData1, myRF.ReadData2);
                    break;
                default:
                    break;
            }
            myRF.ReadWrite(Rs, Rt, Rd, myALUResult, 1);
        } else {
            bitset<32> extendedImm = getExtendedImm(Instruction);
            cout << "extendedImm: ";
            cout << extendedImm;
            cout << "\n";
            switch (opcode.to_ulong()) {
                case 9:
                    myALUResult = myALU.ALUOperation(ADDU, myRF.ReadData1, extendedImm);
                    break;
                case 4:
                    myALUResult = myALU.ALUOperation(SUBU, myRF.ReadData1, extendedImm);
                    if (myALUResult == 0) {
                        extendedImm = extendedImm << 2;
                        PC = PC.to_ulong() + extendedImm.to_ulong();
                    }
                    break;
                case 35: // lw
                    myALUResult = myALU.ALUOperation(ADDU, myRF.ReadData1, extendedImm);    // address
                    cout << "ALUResult: ";
                    cout << myALUResult;
                    cout << "\n";
                    break;
                case 43: // sw
                    myALUResult = myALU.ALUOperation(ADDU, myRF.ReadData1, extendedImm);
                    cout << "ALUResult: ";
                    cout << myALUResult;
                    cout << "\n";
                    break;
                default:
                    break;
            }
            //  myRF.ReadWrite(Rs, Rt, Rt, WrtData, 1);
        }
        
        
		// Read/Write Mem
        bitset<32> readDataMem;
        switch (opcode.to_ulong()) {
            case 35:
                readDataMem = myDataMem.MemoryAccess(myALUResult, 0, 1, 0);     // read only
                cout << "DataMem: ";
                cout << readDataMem;
                cout << "\n";
                break;
            case 43:
                myDataMem.MemoryAccess(myALUResult, myRF.ReadData2, 0, 1);
                break;
            default:
                break;
        }
		
		// Write back to RF
        if (opcode == 0) {
            myRF.ReadWrite(Rs, Rt, Rd, myALUResult, 1);
        } else {
            switch (opcode.to_ulong()) {
                case 35:
                    cout << "write back to R";
                    cout << Rt.to_ulong();
                    cout << '\n';
                    myRF.ReadWrite(Rs, Rt, Rt, readDataMem, 1);
                    break;
                
                default:
                    break;
            }
        }
        cout << '\n';
        myRF.OutputRF(); // dump RF;
    }
    
    myDataMem.OutputDataMem(); // dump data mem
      
    return 0;
        
}

