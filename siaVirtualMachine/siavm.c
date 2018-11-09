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
  The program counter internal register, starting at 0, using a float as to 
  replicate the 4 byte registers of the emulated cpu.
*/
float pc = 0;

int load(char * filename){
  //Initialize and open the file
  FILE *fptr;
  fptr = fopen(filename, "rb");

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

  //Print the hex
  int x;
  for(x = 0; x < fileSize; x++){       
      printf("%02x", memory[x]);
  }

  //Close the file
  fclose(fptr);

  //return success
  return 1;
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
  }

  //free unsigned char array memory
  free(memory);

  //return success
  return 1;

}

