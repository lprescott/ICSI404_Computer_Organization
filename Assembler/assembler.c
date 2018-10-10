/****************************************
  * Title : Assembler 
  * Student : Luke Prescott 
  * Start : 10/10/2018 
  * Desc : Given the code below (which assembles adds only), complete the 
  * 	   assembler for the rest of the instructions as defined in the SIA 
  * 	   document.
  ***************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *ltrim(char *s) {
	while (*s == ' ' || *s == '\t') s++;
	return s;
}

char getRegister(char *text) {
	if (*text == 'r' || *text=='R') text++;
	return atoi(text);
}

int assembleLine(char *text, unsigned char* bytes) {
	text = ltrim(text);
	char *keyWord = strtok(text," ");
	if (strcmp("add",keyWord) == 0) {
		bytes[0] = 0x10;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	} 
	else if (strcmp("addimmediate",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("and",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("branchifequal",keyWord) == 0) {
		return 4;
	}
	else if (strcmp("branchifless",keyWord) == 0) {
		return 4;
	}
	else if (strcmp("divide",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("halt",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("interrupt",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("iterateover",keyWord) == 0) {
		return 4;
	}
	else if (strcmp("jump",keyWord) == 0) {
		return 4;
	}
	else if (strcmp("leftshift",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("load",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("multiply",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("or",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("rightshift",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("store",keyWord) == 0) {
		return 2;
	}
	else if (strcmp("subtract",keyWord) == 0) {
		return 2;
	} 
	else {
		//No instruction was recognized
		return 0;
	}
}

int main(int argc, char **argv) {
	FILE *src = fopen(argv[1],"r");
	FILE *dst = fopen(argv[2],"w");
	while (!feof(src)) {
		unsigned char bytes[4];
		char line[1000];
		printf ("about to read\n");
		if (NULL != fgets(line, 1000, src)) {
			printf ("read: %s\n",line);
			int byteCount = assembleLine(line,bytes);
			//If no instruction was recognized, print error, and return -1
			if (byteCount == 0) {
				printf ("unknown instrution: %s\n", ltrim(strtok(line," ")));
				return -1;
			}
			fwrite(bytes,byteCount,1,dst);
		}
	}
	fclose(src);
	fclose(dst);
	return 0;
}
