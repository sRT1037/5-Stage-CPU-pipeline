#include "header.cpp" 


int32_t main(){

    uint8_t Icache[256];
    int8_t reg[16];
    int8_t Dcache[256];
    

    //read the data from files and store in Icache and reg and Dcache ;
    ifstream ICHE("input/ICache.txt");
    for(int i=0;i<256;i++){
        string val;
        ICHE>>val;
        
        Icache[i]=static_cast<uint8_t> (stoi(val,nullptr,16));
        // cout<<int(Icache[i])<<' '<<i<<endl;
    }
    ICHE.close();


    // cout<<int(Icache[0])<<endl;
    // return 0;
    
    

    ifstream DCHE("input/DCache.txt");

    for(int i=0;i<256;i++){
        string val;
        getline(DCHE,val);
        // cout<<val<<endl;
        // if (!getline(DCHE, val)) {
        //     cerr << "Failed to read line " << i << endl;
        //     break;
        // }
        // if(i==32)cout<<i<<' '<<val.size()<<endl;
        Dcache[i]= static_cast<int8_t> (stoi(val,nullptr,16)) ;
        // cout<<int(Dcache[i])<<' '<<i<<endl;
        
    }

    

    DCHE.close();

   
    

    ifstream Reg("input/RF.txt");

    for(int i=0;i<16;i++){
        string val;
        Reg>>val;
        reg[i]=static_cast<int8_t>(stoi(val,nullptr,16));
        // cout<<int(reg[i])<<' '<<i<<endl;
    }
   
    Reg.close();
    
    // cout<<"HI"<<endl;
    // return 0;

    //initialisation of buffer objects;

    FetchBuffer fb;
    DecodeBuffer db;
    ExecuteBuffer eb;
    MemBuffer mb;
    WBuffer wb;

    // variable required to maintain pipeline like flages or stalls;

    int clockcycle=0;

    bool fetch=true;
    bool execute=0;
    bool decode=0;
    bool mem=0;
    bool writeback=0;

    int stallCause=-1; //-1 is no stall 
                        // stall value 3 for halt
                        // stall value 2 for data dependencies
                        //stall value 1 for control hazard

    int total_ins=0;
    int arthimetic_ins=0;
    int logical_ins=0;
    int shift_ins=0;
    int mem_ins=0;
    int control_ins=0;
    int halt_ins=0;
    int load_imm=0;
    double CPI=0.00;
    int Totall_stalls=0;

    int RAW_stalls=0;
    int Control_Stalls=0;
    int pc=0;

    // cout<<"HI10"<<endl;
    // return 0;
    
    int flag[16]={0}; //flag is nonzero when the coressponding register is alredy in  pipeline
    for(int i=0;i<16;i++)flag[i]=0;

    for(;;){
        // first write beck op should be completed befor mem , mem before exe , exe before decode, decode before fetch
        clockcycle++;
        if(stallCause!=3)fetch=true;

        if(writeback){
            if(wb.opcode==15){
                // cout<<"hi"<<endl;
                break;
            }
            if(wb.opcode>=0&&wb.opcode<=10){
                reg[wb.rd]=wb.ALUoutput;
                flag[wb.rd]--;
            }
            if(wb.opcode==11){
                reg[wb.rd]=wb.LMD;
                flag[wb.rd]--;
            }

        }else{

        }

        if(mem){
            writeback=true;
            wb.INS_NO=mb.INS_NO;
            wb.opcode=mb.opcode;
            wb.IR=mb.IR;
            wb.ALUoutput=mb.ALUoutput;
            wb.rd=mb.rd;
            if(mb.opcode == 11){
                wb.LMD=Dcache[mb.ALUoutput];
            }
            if(mb.opcode == 12){
                Dcache[mb.ALUoutput]=mb.B;
            }
        }else{
            writeback=false;
        }


        if(execute){
            mem=true;
            mb.INS_NO=eb.INS_NO;
            mb.IR=eb.IR;
            mb.opcode=eb.opcode;
            mb.rd=eb.rd;
            uint8_t opcode=eb.opcode;
            int8_t alu;
            total_ins++;
            // cout<<total_ins<<endl;
            if(opcode == 0){
                alu=eb.A+eb.B;
                mb.ALUoutput=alu;
                arthimetic_ins++;
            }else if(opcode == 1){
                alu=eb.A-eb.B;
                mb.ALUoutput=alu;
                arthimetic_ins++;
            }else if(opcode == 2){
                alu=eb.A * eb.B;
                mb.ALUoutput=alu;
                arthimetic_ins++;
            }else if(opcode == 3){
                alu=eb.A+1;
                mb.ALUoutput=alu;
                arthimetic_ins++;
            }else if(opcode == 4){
                alu=eb.A&eb.B;
                mb.ALUoutput=alu;
                logical_ins++;
            }else if(opcode == 5){
                alu=eb.A|eb.B;
                mb.ALUoutput=alu;
                logical_ins++;
            }else if(opcode == 6){
                alu=eb.A ^ eb.B;
                mb.ALUoutput=alu;
                logical_ins++;
            }else if(opcode == 7){
                alu= ~ eb.A ;
                mb.ALUoutput=alu;
                logical_ins++;
            }else if(opcode == 8){
                alu=eb.A <<eb.imm;
                mb.ALUoutput=alu;
                shift_ins++;
            }else if(opcode == 9){
                uint8_t val=static_cast<uint8_t> (eb.A);
                val>>=(eb.imm);
                alu=static_cast<int8_t> (val);
                mb.ALUoutput=alu;
                shift_ins++;
            }else if(opcode == 10){
                alu=eb.simm;
                mb.ALUoutput=alu;
                load_imm++;
            }else if(opcode == 11){
                alu=eb.A+eb.simm;
                mb.ALUoutput=alu;
                mem_ins++;
            }else if(opcode == 12){
                alu=eb.A+eb.simm;
                mb.ALUoutput=alu;
                mem_ins++;
                mb.B=eb.B;
            }else if(opcode == 13){
                pc=pc+ 2*eb.simm;
                control_ins++;
                fetch=false;
            }else if(opcode == 14){
                if(eb.A==0){
                    pc=pc+ 2*eb.simm;
                    fetch=false;
                    control_ins++;
                }else{
                    fetch=false;
                    control_ins++;
                }
            }else if(opcode == 15){
                decode=false;
                halt_ins++;
            }




        }else{
            mem=false;
        }


        if(decode){
            execute=true;

            // decode the decode buffer contents;
            uint16_t IR=db.IR;
            uint8_t Opcode=(IR >>12);
            eb.IR=db.IR;
            eb.opcode=Opcode;
            eb.INS_NO=db.INS_NO;


            uint8_t rd,rs1,rs2;
            uint8_t imm;
            int8_t simm;
            int8_t A,B; //contents of registers

            

            if(Opcode>=0&&Opcode<=6&&Opcode!=3){
                rs2=IR & 0b00001111;
                rs1=(IR >> 4)& 0b00001111;
                rd= (IR >> 8) & 0b00001111;
                if(flag[rs1]||flag[rs2]){
                    stallCause=2;
                    execute=false;
                    fetch=false;
                }else{
                    flag[rd]++;
                    eb.A=reg[rs1];
                    eb.B=reg[rs2];
                    eb.rd=rd;
                }
            }else if(Opcode==3){
                rd=(IR >>8)&0b00001111;
                if(flag[rd]){
                    stallCause=2;
                    fetch=false;
                    execute=false;
                }else{
                    eb.A=reg[rd];
                    // eb.imm=0b00000001;
                    eb.rd=rd;
                    flag[eb.rd]++;
                }
            }else if(Opcode==7){
                rd=(IR>>8)&0b00001111;
                rs1=(IR>>4)&0b00001111;
                if(flag[rs1]){
                    stallCause=2;
                    fetch=false;
                    execute=false;
                }else{
                    flag[rd]++;
                    eb.A=reg[rs1];
                    eb.rd=rd;
                }
            }else if(Opcode==8||Opcode==9){
                rd=(IR>>8)&0b00001111;
                rs1=(IR>>4)&0b00001111;
                imm=IR;
                imm&=0b00001111;
                if(flag[rs1]){
                    stallCause=2;
                    fetch=false;
                    execute=false;
                }else{
                    flag[rd]++;
                    eb.A=reg[rs1];
                    eb.imm=imm;
                    eb.rd=rd;
                }
            }else if(Opcode==10){
                rd=(IR >> 8)&0b00001111;
                simm=static_cast<int8_t> (uint8_t(IR));
                 
                flag[rd]++;
                eb.rd=rd;
                eb.simm=simm;

            }else if(Opcode==11){
                rd=(IR>>8)&0b00001111;
                rs1=(IR>>4)&0b00001111;

                uint8_t imm=IR;

               
                imm&=0b0001111;
                if((imm&0b00001000)!=0)simm|=0b11110000;
                 simm= static_cast<int8_t> (imm);

                if(flag[rs1]){
                    fetch=false;
                    execute=false;
                    stallCause=2;
                }else{
                    flag[rd]++;
                    eb.A=reg[rs1];
                    eb.simm=simm;
                    eb.rd=rd;
                } 
            }else if(Opcode==12){

                rs1=(IR>>4)&0b00001111;
                rs2=(IR>>8)&0b00001111;

                uint8_t imm=IR;

                imm&=0b0001111;
                if((imm&0b00001000)!=0)simm|=0b11110000;
                simm= static_cast<int8_t> (imm);

                if(flag[rs1]||flag[rs2]){
                    fetch=false;
                    execute=false;
                    stallCause=2;
                }else{
                    eb.A=reg[rs1];
                    eb.simm=simm;
                    eb.B=reg[rs2];
                }

            }else if(Opcode==13){
                uint8_t val=(IR >>4);
                val&=0b11111111;
                simm=static_cast<int8_t>(val);
                eb.simm=simm;
                fetch=false;
                stallCause=1;
            }else if(Opcode==14){
                rs1=(IR >>8);
                rs1&=0b00001111;

                uint8_t val=IR;
                simm=static_cast<int8_t> (val);
                if(flag[rs1]){
                    fetch=false;
                    execute=false;
                    stallCause=2;
                }else{
                    stallCause=1;
                    eb.A=reg[rs1];
                    eb.simm=simm;
                    fetch=false;
                }
            }else if(Opcode==15){
                stallCause=3;
                fetch=false;
            }
            
        }else{
            execute=false;
        }


        if(fetch){
            uint16_t instr=0x0000;
            uint16_t front=Icache[pc];
            uint16_t back=Icache[pc+1];
            
            decode=true;

            db.IR=((front<<8)|back);
            db.INS_NO=pc;
            pc+=2;
        }else{
            // fetch operation not executed in the current tage means decode op shoulf be false in next cycle;
            decode=false;
            if(stallCause==2){
                decode=1;
                RAW_stalls++,Totall_stalls++;
            }else if(stallCause==1){
                Totall_stalls++,Control_Stalls++;
            }
        }
    }
    cout<<"HI"<<endl;
    ofstream changedChache("output/DCache.txt");
    ofstream outINS("output/Output.txt");

    for(int i=0;i<256;i++){
        int8_t dc=Dcache[i];
        int Char=((dc>>4)&0b00001111);

        if(Char>=10){
            changedChache<<char('a'+Char-10);
        }else{
            changedChache<<char('0'+Char);
        }
        Char= (dc & 0b00001111);
        if(Char>=10){
            changedChache<<char('a'+Char-10)<<'\n';
        }else{
            changedChache<<char('0'+Char)<<'\n';
        }
    }

    CPI=(1.0*clockcycle)/(1.0*total_ins);

    outINS<<"Total number of Instruction executed     :"<<total_ins<<'\n';
    outINS<<"Number of Instructions in each class\n";

    outINS<<"Arithemetic instructions                 :"<<arthimetic_ins<<'\n';
    outINS<<"Logical instructions                     :"<<logical_ins<<'\n';
    outINS<<"Shift instructions                       :"<<shift_ins<<'\n';
    outINS<<"Memory instructions                      :"<<mem_ins<<'\n';
    outINS<<"Load immediate instructions              :"<<load_imm<<'\n';
    outINS<<"Control instructions                     :"<<control_ins<<'\n';
    outINS<<"Halt instructions                        :"<<halt_ins<<'\n';
    outINS<<"Cycles per instruction                   :"<<CPI<<'\n';
    outINS<<"Total number of stalls                   :"<<Totall_stalls<<'\n';
    outINS<<"Data Stalls                              :"<<RAW_stalls<<'\n';
    outINS<<"Control Stalls                           :"<<Control_Stalls<<'\n';

    changedChache.close();
    outINS.close();

    return 0;
}