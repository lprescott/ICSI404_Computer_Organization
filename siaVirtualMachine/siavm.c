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
  Allocate space for your virtual machine’s memory. 10k or so as unsigned byte 
  should be enough. 
*/
unsigned char * memory; 

/*
  The program counter internal register, starting at 0, using a unsigned char
  as to replicate the 4 bit registers of the emulated cpu.

  The op1, and op2 registers, are long unsigned ints as to be able to store 
  the largest possible operands in the sia instruction set.

  result is an unsigned int, as it must be stored in a register for multiple 
  instructions.
*/
unsigned char PC = 0;
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

  //Print the hex for testing
  int x;
  for(x = 0; x < fileSize; x++){       
      printf("%02x", memory[x]);
  }

  //Close the file
  fclose(fptr);

  //return success
  return 1;
}

int fetch(){

}

int dispatch(){

}

int store(){

}


int main(int argc, char **argv) {

  memory = (unsigned char *) malloc(10000);
  unsigned char * currentInstruction;

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
  }

  //Run flag
  int halt = 0;
  //Run loop
  while(halt != 1){
    //Fetch: read 2 bytes to current instruction
    //Dispatch: read current instruction, population registers, fetch more
    //Execute: Store result into result register
    //Store: Store result register into mem, or register
  }


  //free unsigned char array memory
  free(memory);

  //return success
  return 1;
}