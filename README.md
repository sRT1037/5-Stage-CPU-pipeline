
# 5-Stage Pipelined CPU Simulator

## Project Overview
This project implements a basic 5-stage in-order pipelined CPU simulator in C++. The pipeline follows the standard stages (IF, ID, EX, MEM, WB) and uses simple stalling to handle data hazards without forwarding or out-of-order execution. It provides a clear demonstration of how in-order instruction flow and hazard resolution work in a classic pipelined architecture.



### Input: 
The program reads the instruction cache (input/ICache.txt), data cache (input/DCache.txt), and register file (input/RF.txt) at startup.

### Execution:
 It simulates a 5-stage pipelined CPU (IF, ID, EX, MEM, WB),       processing instructions cycle by cycle with appropriate data forwarding and hazard handling.

### Output: 
At the end of execution, the updated data cache and register file are written to output/DCache.txt and output/RF.txt.

 
