
// C program to extract k bits from a given 
// position. 
#include <stdio.h> 
  
// Function to extract k bits from p position 
// and returns the extracted value as integer 
int bitExtracted(int number, int k, int p) 
{ 
    return (((1 << k) - 1) & (number >> (p - 1))); 
} 
  
// Driver code 
int main() 
{ 
    int number = 0x2B9A0000, k = 12, p = 1; 
    printf("The extracted number is %d \n", ((number >> 28) & 0x0F)); 
    return 0; 
} 
