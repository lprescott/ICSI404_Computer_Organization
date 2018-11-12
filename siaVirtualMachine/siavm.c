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
#include <string.h>

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

  //allocated the currentInstructions 4 possible bytes
  currentInstruction = calloc(4, sizeof(unsigned char));

  //Loop twice
  for(int x = 0; x < 2; x++){

    //Add the memory's 2 bytes to currentInstruction
    currentInstruction[x] = memory[pc+x];
  }

  //Print the hex for testing
  /*
  printf("%04d ", pc);
  for (int y = 0; y < 2; y++) {
    printf("%02x ", currentInstruction[y]);
  }
  printf("\n");
  */

  //If the current instruction is zero, return
  if(currentInstruction[0] == 0){
    free(currentInstruction);
    currentInstruction = NULL;
    return 1;
  }
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
  
  //Fetch any additional memory needed to complete the instruction.
  if(currentInstruction[0]>>4 == 0xA | currentInstruction[0]>>4 == 0xB | currentInstruction[0]>>4 == 0xC | currentInstruction[0]>>4 == 0xD){

    int temp = pc;
    //Loop twice
    for(int x = 2; x < 4; x++){

      //Add the memory's 2 bytes to currentInstruction
      currentInstruction[x] = memory[temp];
      temp+=1;
    }

    //Print the 4 byte instruction for testing
    printf("%04d ", pc);
    for (int y = 0; y < 4; y++) {
      printf("%02x ", currentInstruction[y]);
    }
    printf("\n");

    //Add 2 more bytes to program counter
    pc +=2;

  } else{

    //Print the 2 bytes instruction for testing
    printf("%04d ", pc);
    for (int y = 0; y < 2; y++) {
      printf("%02x ", currentInstruction[y]);
    }
    printf("\n");
  }

  printf("opcode: %x\n\n", currentInstruction[0]>>4);

  //Pupulate op1, and r2 if needed
  switch(currentInstruction[0]>>4) {

    case 0x0: break;
    case 0x1: //add, 3r
    case 0x2: //and, 3r
    case 0x3: //divide, 3r
    case 0x4: //multiply, 3r
    case 0x5: //subtract, 3r
    case 0x6: //or, 3r
      op1 = currentInstruction[1];
      op2 = currentInstruction[2];
      break;

    case 0x7: //left/right shift, sft
      op1 = currentInstruction[1];
      op2 = ((currentInstruction[2] & 0x1) << 5) & currentInstruction[3];
      break;

    case 0x8: //interrupt, int
      op1 = (((currentInstruction[1] << 4) & currentInstruction[2]) << 4) & currentInstruction[3];

    case 0x9: //addimmediate, ai 
      op1 = currentInstruction[1];
      op2 = (currentInstruction[2] << 4) & currentInstruction[3];
      break;

    case 0xA: //branchifequal, br
    case 0xB: //branchifless, br
      op1 = (((((((currentInstruction[3] << 4) & currentInstruction[4]) << 4) & currentInstruction[5]) << 4) & currentInstruction[6]) << 4) & currentInstruction[7];

    case 0xC: //jump, jmp
      op1 = ((((((((((currentInstruction[1] << 4) & currentInstruction[2]) << 4) & currentInstruction[3]) << 4) & currentInstruction[4]) << 4) & currentInstruction[5] << 4) & currentInstruction[6]) << 4) & currentInstruction[7];

    case 0xD: //iterateover, iter TODO
      op1 = (currentInstruction[2] << 4) & currentInstruction[3];
      op2 = (((((currentInstruction[4] << 4) & currentInstruction[5]) << 4) & currentInstruction[6]) << 4) & currentInstruction[7];
      break;

    case 0xE: //load, ls 
    case 0xF: //store, ls
      op1 = currentInstruction[3];
      break;
      
    default: 
      return -1;
  }


  //Increment the pc by 2 bytes
  pc += 2;
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

  //Allocate 10000 unsigned chars in space for memory
  memory = (unsigned char *) malloc(10000 * sizeof (unsigned char));

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

  //Check if file is loadable
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

    //Free the current instruction
    free(currentInstruction);
  }

  //free unsigned char array memory
  free(memory);
  

  //return success
  return 1;
}