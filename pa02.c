
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void freeArr(char * sumTxtArr);
void eightBitCalculator(int numChars, int numBits, char *sumTxtArr);
void sixteenBitCalculator(int numChars, int numBits, char *sumTxtArr, int buffer);
void thirtytwoBitCalculator(int numChars, int numBits, char *sumTxtArr, int buffer);

int main(int argc, char **argv){

int numBits;
int numChars = 0;
int counter = 0;
char* sumTxtArr;
char *fname = argv[1];
FILE *sumTxt = fopen(fname, "r");

//Check if the first file is entered and if it is continue on to the program
if(sumTxt == 0)
    printf("Could not open file!\n");
    else printf("\n");

numBits = atoi(argv[2]);
    if(numBits != 8 && numBits != 16 && numBits != 32){
      fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
      return 0;
    }

fseek(sumTxt, 0, SEEK_END);
int buffer = ftell(sumTxt);
fseek(sumTxt, 0, SEEK_SET);
//Allocate memory for an array the length of the buffer size + 4
sumTxtArr = calloc(buffer, sizeof(char) + 4);

//Read in the file
for(int i = 0; i < buffer; i++){
sumTxtArr[i] = fgetc(sumTxt); 
numChars += 1;
if(counter == 80){
  printf("\n");
  counter = 0;
}
  printf("%c", sumTxtArr[i]);
  counter += 1;

}

//Depending on the input pick the right checksum
if(numBits == 8){
    eightBitCalculator(numChars, numBits, sumTxtArr);
}
else if(numBits == 16){
    sixteenBitCalculator(numChars, numBits, sumTxtArr, buffer);
}
else if(numBits == 32){
    thirtytwoBitCalculator(numChars, numBits, sumTxtArr, buffer);
}

freeArr(sumTxtArr);
}

void eightBitCalculator(int numChars, int numBits, char *sumTxtArr){

unsigned long final = 0;
//Add all numbers together 
for(int i = 0; i <numChars; i++){
    final += sumTxtArr[i];
}
printf("\n");
//Mod by 256
final = final % 256;

//Print out the results
printf("%d bit checksum is %6lx for all %4d chars\n", numBits, final, numChars);

}

void sixteenBitCalculator(int numChars, int numBits, char *sumTxtArr, int buffer){

unsigned long final;
int counter = 0, counter1 = 0;
int j = 0, k = 0;
unsigned long temp1 = 0;
unsigned long temp2 = 0;

//Determine if there needs to be padding and if it needs, add an X to the end of the array
if(numChars % 2 == 0){
    printf("\n");
}
if(numChars % 2 != 0){
    sumTxtArr[buffer] = 'X';
    printf("%c\n", sumTxtArr[buffer]);
    buffer += 1;
    numChars += 1;
}

//Add every other index together
for(int i = 0; i < buffer; i++){

    if(i%2 == 0){
    temp1 += sumTxtArr[i];
    }
    else if(i%2 != 0){
    temp2 += sumTxtArr[i];
    }

}

//Calculate the carry bit
if(temp1 >= 256 && temp2 < 256){
    temp1%=256;
}
else if(temp1 >= 256 && temp2 >= 256 && temp1 >= 512 && temp2 >= 512){
    temp1%=256;
    temp1+=1;
    temp2%=256;
    temp1+=1;
}
else if(temp1 >= 256 && temp2 >= 256){
    temp1%=256;
    temp2%=256;
    temp1+=1;
}
else if(temp2 >= 256 && temp1 < 256){
    temp2%=256;
    temp1+= 1;
}

    //Output the results
    printf("%2d bit checksum is %6lx%02lx for all %4d chars\n", numBits, temp1, temp2, numChars);

}

void thirtytwoBitCalculator(int numChars, int numBits, char *sumTxtArr, int buffer){
unsigned long temp1 = 0;
unsigned long temp2 = 0;
unsigned long temp3 = 0;
unsigned long temp4 = 0;
int padding = 4 - (numChars % 4);
int tester = 0;

//printf("%d", buffer);
//printf("%d", padding);

//Determine if there needs to be padding and if it needs, add an X to the end of the array
if(numChars % 4 != 0){
    for(int i = buffer; i < (buffer + padding); i++){
    tester = 2;
    sumTxtArr[i] = 'X';
    numChars += 1;
    printf("%c", sumTxtArr[i]);
    }
    buffer += padding;
}

//Print a new line if padding is needed to follow the formatting
if(tester == 2){
    printf("\n");
}

//Add every four index's together
for(int i = 0; i < buffer; i++){

    if(i%4 == 0){
    temp1 += sumTxtArr[i];
    }
    else if(i%4 == 1){
    temp2 += sumTxtArr[i];
    }
    else if(i%4 == 2){
    temp3 += sumTxtArr[i];
    }
    else if(i%4 == 3){
    temp4 += sumTxtArr[i];
    }

}

//Calculate the carry bit
if(temp4 >= 256 && temp3 < 256 && temp2 < 256 && temp2 < 256){
    temp4 %= 256;
    temp3 += 1;
}
if(temp3 >= 256 && temp1 < 256 && temp2 < 256 && temp3 < 256){
    temp3 %= 256;
    temp2 += 1;
}
if(temp2 >= 256 && temp1 < 256 && temp3 < 256 && temp4 < 256){
    temp2 %= 256;
    temp1 += 1;
}
if(temp4 >= 256 && temp3 >= 256 && temp2 >= 256 && temp1 >= 256){
    temp1 %= 256;
    temp2 %= 256;
    temp1 += 1;
    temp3 %= 256;
    temp2 += 1;
    temp4 %= 256;
    temp3 += 1;
}
if(temp4 >= 256 && temp3 >= 256 && temp2 < 256 && temp1 < 256){
    temp3 %= 256;
    temp2 += 1;
    temp4 %= 256;
    temp3 += 1;
}
if(temp4 < 256 && temp3 < 256 && temp2 >= 256 && temp1 >= 256){
    temp1 %= 256;
    temp2 %= 256;
    temp1 += 1;
}
if(temp1 >=256 && temp2 < 256 && temp3 < 256 && temp4 < 256){
    temp1 %= 256;
}

//Print out the result
printf("%2d bit checksum is %02lx%02lx%02lx%02lx for all %4d chars\n", numBits, temp1, temp2, temp3, temp4, numChars);

}

//Free the dynamically allocated array
void freeArr(char * sumTxtArr){
    free(sumTxtArr);
}
