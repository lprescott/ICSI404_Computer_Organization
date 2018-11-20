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

/*
  Initialize virtual machine’s memory. 
*/
unsigned char * memory; 

/*
  Initialize virtual machine’s current instruction.
*/
unsigned char * currentInstruction;


/*
  The internal register: pc is a unsigned int, as it cannot be negative.
  The internal registers: op1, op2, and result are all signed ints.
*/
unsigned int pc = 0;
int op1;
int op2;
int result;

/*
  Other registers: unsigned chars as to hold the maximum values of 4 bits.
  There are 16 of them, stored in an array for ease of access.
*/
int registers[16];

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

    unsigned int temp = pc;
    //Loop twice
    for(int x = 2; x < 4; x++){

      //Add the memory's 2 bytes to currentInstruction
      currentInstruction[x] = memory[temp];
      temp+=1;
    }

    //Print the 4 byte instruction for testing
    printf("%04u ", pc);
    for (int y = 0; y < 4; y++) {
      printf("%02x ", currentInstruction[y]);
    }
    printf("\n");

    //Add 2 more bytes to program counter
    pc +=2;

  } else{

    //Print the 2 bytes instruction for testing
    printf("%04u ", pc);
    for (int y = 0; y < 2; y++) {
      printf("%02x ", currentInstruction[y]);
    }
    printf("\n");
  }

  printf("opcode: %01x\n\n", currentInstruction[0]>>4);

  //Populate op1, and op2 if needed
  switch(currentInstruction[0]>>4) {

    case 0x7: //left/right shift, sft
      op1 = (currentInstruction[1] & 0x20) >> 5; //set for rightshift, clear for left shift
      op2 = currentInstruction[1] & 0x1F; //shift amount
      break;

    case 0x8: //interrupt, int
      op1 = ((currentInstruction[0] & 0x0F) << 8) | currentInstruction[1];
      break;

    case 0x9: //addimmediate, ai
      op1 = currentInstruction[1];
      break;

    case 0xA: //branchifequal, br
    case 0xB: //branchifless, br
      op1 = ((((currentInstruction[1] & 0x0F) << 8) | currentInstruction[2]) << 8) | currentInstruction[3];
      break;

    case 0xC: //jump, jmp
      op1 = ((((((currentInstruction[0] & 0x0F) << 8) | currentInstruction[1]) << 8) | currentInstruction[2]) << 8) | currentInstruction[3];
      break;

    case 0xD: //iterateover, iter
      op1 = currentInstruction[1];
      op2 = (currentInstruction[2] << 8) | currentInstruction[3];
      break;

    case 0xE: //load, ls 
    case 0xF: //store, ls
      op1 = currentInstruction[1] & 0x0F; //offset
      break;
      
    default: 
      break;
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
    
  switch(currentInstruction[0]>>4) {
    case 0x0: break;
    case 0x1: //add, 3r
      result = registers[currentInstruction[1]] + registers[currentInstruction[2]];
      break;
    case 0x2: //and, 3r
      result = registers[currentInstruction[1]] & registers[currentInstruction[2]];
      break;
    case 0x3: //divide, 3r
      result = registers[currentInstruction[1]] / registers[currentInstruction[2]];
      break;
    case 0x4: //multiply, 3r
      result = registers[currentInstruction[1]] * registers[currentInstruction[2]];
      break;
    case 0x5: //subtract, 3r
      result = registers[currentInstruction[1]] - registers[currentInstruction[2]];
      break;
    case 0x6: //or, 3r
      result = registers[currentInstruction[1]] | registers[currentInstruction[2]];
      break;

    case 0x7: //left/right shift, sft
      if(op1 == 1){
        //rightshift
        result = registers[currentInstruction[1]] >> op2;
      } else if(op1 == 0){
        //leftshift
        result = registers[currentInstruction[1]] << op2;
      } else{
        return -1;
      }
      break;

    case 0x8: //interrupt, int
      if(op1 == 0){
        //print all the registers
        printf("Op1: \"%d\"", op1);
        int x;
        for(x = 0; x < 16; x++){
          printf("r%d: %d\n", x, registers[x]);
        }
      } else if(op1 == 1){
        //print the memory up to 100
        int x;
        for(x = 0; x < 100; x++){
          if(x%10 == 0) printf("\n%08d ", x);
          if(x%2 == 0) printf("%02x", memory[x]);
          else printf("%02x ", memory[x]);
        }
      } 
      
      break;

    case 0x9: //addimmediate, ai 
      result = registers[currentInstruction[1]] + op1;
      break;

    case 0xA: //branchifequal, br
      if(registers[currentInstruction[1]] == registers[currentInstruction[2]]){
        pc += op1;
        break;
      } else{
        break;
      }
    case 0xB: //branchifless, br
      if(registers[currentInstruction[1]] < registers[currentInstruction[2]]){
        pc += op1;
        break;
      } else{
        break;
      }

    case 0xC: //jump, jmp
      pc = op1;
      break;

    case 0xD: //iterateover, iter
      if(memory[registers[currentInstruction[1]] + op1] != 0){
        result = memory[registers[currentInstruction[1]] + op1];
        pc -= op2;
      }
      break;

    case 0xE: //load, ls 
      result = memory[registers[currentInstruction[2]] + op1];
      break;
    case 0xF: //store, ls
      break;

    default: 
      return -1;
  }
}

/*
  Write result into final register or memory address.
*/
int store(){

  //These variables are used to keep track of the store instruction's value and starting address
  int value; //the value to store
  int start; //the starting address to store to

  //Check if the current instruction is empty, halt
  if(currentInstruction == NULL){
    return 1;
  }

  switch(currentInstruction[0]>>4) {
    case 0x0: break;
    case 0x1: //add, 3r
    case 0x2: //and, 3r
    case 0x3: //divide, 3r
    case 0x4: //multiply, 3r
    case 0x5: //subtract, 3r
    case 0x6: //or, 3r
      registers[currentInstruction[3]] = result;
      break;

    case 0x7: //left/right shift, sft
      registers[currentInstruction[1]] = result;
      break;

    case 0x8: //interrupt, int
      break;

    case 0x9: //addimmediate, ai 
      registers[currentInstruction[1]] = result;
      break;

    case 0xA: //branchifequal, br
    case 0xB: //branchifless, br
      break;

    case 0xC: //jump, jmp
      break;

    case 0xD: //iterateover, iter
      registers[currentInstruction[1]] = result;
      break;

    case 0xE: //load, ls 
      registers[currentInstruction[1]] = result;
      break;
    case 0xF: //store, ls

      value = registers[currentInstruction[1]];
      start = registers[currentInstruction[2]] + op1;

      memory[start] = value >> 24;
      memory[start+1] = (value >> 16) & 0x0F;
      memory[start+2] = (value >> 8) & 0x00F;
      memory[start+3] = value & 0x000F;
      break;

    default: 
      return -1;
  }
}

int main(int argc, char **argv) {

  //Allocate 10000 unsigned chars in space for memory
  memory = (unsigned char *) malloc(10000 * sizeof (unsigned char));

  memset(registers, 0, sizeof(registers)); //sets the registers explicitly to 0

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