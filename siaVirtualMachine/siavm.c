/****************************************
  * Title : SIA Virtual Machine 
  * Student : Luke Prescott 
  * Start : 11/08/2018 
  * Desc : Create a command line application in C called siavm that takes as a 
  *     parameter a filename (argv[1]). 
  ***************************************/

//standard c libraries
#include <stdio.h> 
#include <stdlib.h>

// included external header files containing prototypes
#include "siavmHeaders.h"

//Global variables:

/*
  Initialize virtual machine’s memory. 
*/
unsigned char * memory; 

/*
  Initialize virtual machine’s current instruction.
*/
unsigned char * currentInstruction;


/*
  The program counter internal register, starting at 0, using a unsigned char
  as to replicate the 4 bit registers of the emulated cpu.

  The op1, and op2 registers, are long unsigned ints as to be able to store 
  the largest possible operands in the sia instruction set.

  result is an unsigned int, as it must be stored in a register for multiple 
  instructions.
*/
unsigned char pc = 0;
long unsigned int op1;
long unsigned int op2;
unsigned char result;

/*
  Other registers: unsigned chars as to hold the maximum values of 4 bits.
*/
unsigned char r0;
unsigned char r1;
unsigned char r2;
unsigned char r3;
unsigned char r4;
unsigned char r5;
unsigned char r6;
unsigned char r7;
unsigned char r8;
unsigned char r9;
unsigned char r10;
unsigned char r11;
unsigned char r12;
unsigned char r13;
unsigned char r14;
unsigned char r15;

//The load function reads the assembled sia code, and stores it into memory.
int load(char * filename){
  //Initialize and open the file
  FILE *fptr;
  fptr = fopen(filename, "rb");

  //Check if the file is non-existant 
  if(fptr == NULL){
    fclose(fptr);
    return -1;
  }

  //Seek to the end of the file,
  fseek(fptr, 0, SEEK_END);
  //Store the length of the file
  size_t fileSize = ftell(fptr);
  //Seek back to the beginning
  fseek(fptr, 0, SEEK_SET);  

  //Read the whole file into memory (the array called "memory")
  if (fread(memory, sizeof *memory , fileSize, fptr) != fileSize){
    fclose(fptr);
    return -1;
  }

  /*
  //Print the hex for testing
  int x;
  for(x = 0; x < fileSize; x++){       
    printf("%02x", memory[x]);
  }
  */

  //Close the file
  fclose(fptr);

  //return success
  return 1;
}

/*
  Read 2 bytes from memory. 
  Store them in an internal array of bytes, currentInstruction.
*/
int fetch(){

  //allocated the currentInstructions 2 bytes
  currentInstruction = calloc(2, sizeof(unsigned char));
  
  //Assign temp to the pc
  int temp = pc;

  //Loop twice
  for(int x = 0; x < 2; x++){

    //Add the memory's 2 bytes to currentInstruction
    currentInstruction[x] = memory[pc+x];

    //If the current instruction is zero, return
    if(currentInstruction[x] == 0x00){
      free(currentInstruction);
      currentInstruction = NULL;
      return 1;
    }
  }

  //Print the hex for testing
  printf("%04d ", pc);
  for (int y = 0; y < 2; y++) {
    printf("%02x ", currentInstruction[y]);
  }
  printf("\n");

  //Increment the pc by 2 bytes
  pc += 2;

  //Free the current instruction
  free(currentInstruction);
}

/*
  Read only from the array of bytes from fetch. 
  Populate the OP1 and OP2 registers if appropriate. 
  Fetch any additional memory needed to complete the instruction.
*/
int dispatch(){
  //Check if the current instruction is empty, halt
  if(currentInstruction == NULL){
    return 1;
  }
}

/*
  Switch statement that “does the work” and stores the work into Result
*/
int execute(){
  //Check if the current instruction is empty, halt
  if(currentInstruction == NULL){
    return 1;
  }
}

/*
  Write result into final register or memory address.
*/
int store(){
  //Check if the current instruction is empty, halt
  if(currentInstruction == NULL){
    return 1;
  }
}

int main(int argc, char **argv) {

  memory = (unsigned char *) malloc(10000);

  //Check if the correct number of arguments were supplied
  if(argc < 2){
    puts("ERROR: siavm requires one additional argument");
    //free unsigned char array memory
    free(memory);
    return -1;
  } else if(argc > 2){
    puts("ERROR: siavm requires only one additional argument");
    //free unsigned char array memory
    free(memory);
    return -1;
  }

  //A string to hold the filename
  char * filename = argv[1];

  if (load(filename) == -1){
    puts("ERROR: siavm failed loading file");
    free(memory);
    return -1;
  }

  //Run flag
  int halt = 0;
  //Run loop
  while(halt != 1){
    //Fetch: read 2 bytes to current instruction
    if(fetch() == -1){
      puts("ERROR: siavm could not fetch instruction");
      free(memory);
      return -1;
    }
    //Dispatch: read current instruction, population registers, fetch more
    if(dispatch() == -1){
      puts("ERROR: siavm could not dispatch instruction");
      free(memory);
      return -1;
    }
    //Execute: Store result into result register
    if(execute() == -1){
      puts("ERROR: siavm could not execute instruction");
      free(memory);
      return -1;
    }
    //Store: Store result register into mem, or register
    if(store() == -1){
      puts("ERROR: siavm could not store instruction");
      free(memory);
      return -1;
    }

    //Check if there are no more instructions
    if(currentInstruction == NULL){
      halt = 1;
    }
  }

  //free unsigned char array memory
  free(memory);

  //return success
  return 1;
}