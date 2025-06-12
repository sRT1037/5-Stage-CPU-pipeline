#include <bits/stdc++.h>
using namespace std;

// buffer befor the fetch op
struct FetchBuffer{
    uint8_t INS_NO;
};

//buffer between fetch and decode
struct DecodeBuffer{
    uint16_t IR;
    uint8_t INS_NO;
};

//buffer between Decode op and Excute op
struct ExecuteBuffer{
    uint8_t INS_NO;
    uint8_t opcode;
    int8_t simm; // signed immediate for load or store op
    uint8_t rd;
    uint8_t imm;
    int8_t A;
    int8_t B;
    uint16_t IR;
};

// buffer in between execute and mem op
struct MemBuffer{
    uint8_t INS_NO;
    uint16_t IR;
    uint8_t opcode;
    uint8_t ALUoutput;
    uint8_t rd; //for load op
    uint8_t B; //usefull in case of store op
};

// buffer in between mem op and write back op;
struct WBuffer{
    uint8_t INS_NO;
    uint16_t IR;
    uint8_t opcode;
    uint8_t rd;
    int8_t ALUoutput; 
    int8_t LMD; //in load operation
};

