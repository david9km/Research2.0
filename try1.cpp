/*
 * Example from original Pin paper (PLDI'05):
 * https://doi.org/10.1145/1065010.1065034
 */

#include "pin.H"
#include <map>
#include <stdio.h>
#include <iostream>
FILE *trace;
UINT32 sizeArray[81];
UINT32 readArray[81];
FILE *output;
// Print a memory write record

//try to have user only implement what is in the functions that they specify
//try to also have it print out the disas of the code


VOID RecordMemWrite(VOID *ip, VOID *addr, UINT32 size)
{
    UINT32 x = sizeArray[size];
    x++;
    sizeArray[size] = x;
    fprintf(trace, "%p: W %p %d\n", ip, addr, size);
}
VOID RecordMemRead(VOID *ip, VOID *addr, UINT32 size)
{
    UINT32 x = readArray[size];
    x++;
    readArray[size] = x;
    fprintf(trace, "%p: R %p %d\n", ip, addr, size);

}

VOID Instruction2(INS ins, VOID *V)
{
    if(INS_IsMemoryRead(ins)) {
        RTN routine = INS_Rtn(ins);
        if(RTN_Valid(routine) && (RTN_Name(routine) == "main" || RTN_Name(routine) == "quicksort")) {
            cout << PIN_UndecorateSymbolName(RTN_Name(routine), UNDECORATION_NAME_ONLY) << "\n";

            cout << RTN_Name(routine) << "\n";


            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)RecordMemRead,
                                     IARG_INST_PTR,
                                     IARG_MEMORYREAD_EA,
                                     IARG_MEMORYREAD_SIZE,

                                     IARG_END);
        }
    }

}

// Called for every instruction
VOID Instruction(INS ins, VOID *v)
{
    if (INS_IsMemoryWrite(ins)) {
        RTN routine = INS_Rtn(ins);
        if(RTN_Valid(routine) && (RTN_Name(routine) == "main" || RTN_Name(routine) == "quicksort")) {

            // instruments writes using a predicated call, i.e. the call happens
            // iff the store is actually executed
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)RecordMemWrite,
                                     IARG_INST_PTR,
                                     IARG_MEMORYWRITE_EA,
                                     IARG_MEMORYWRITE_SIZE,
                                     IARG_END);
        }
    }
}

VOID printer(INT32 code, VOID *v)
{
    fprintf(output, "\t Data_size, \t Inst_Data_Occ, \t Block_Data_Occ,\t Func_Data_Occ,\n");
    for(int i = 1; i < 81; i++) {
        if(sizeArray[i] != 0) {
            fprintf(output, "W,\t %d, \t\t %d,\n", i, sizeArray[i]);
            printf("Bytes Written: %d  Occurences: %d \n", i, sizeArray[i]);
        }
    }
    for(int i = 1; i < 81; i++) {
        if(readArray[i] != 0) {
            fprintf(output, "R,\t %d, \t\t %d,\n", i, readArray[i]);
            printf("Bytes read: %d Occurences: %d \n", i, readArray[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    PIN_Init(argc, argv);
    PIN_InitSymbols();
    PIN_SetSyntaxATT();
    trace = fopen("try1.out", "w");
    output = fopen("results.csv", "w");
    INS_AddInstrumentFunction(Instruction, 0);
    INS_AddInstrumentFunction(Instruction2, 0);
    PIN_AddFiniFunction(printer, 0);

    PIN_StartProgram();     // Never returns
    return 0;
}

