```c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <inttypes.h>

#include <string.h>

//#include "RAMus.h"

#define getBit(n,k) (n & ( 1 << k )) >> k
enum { VR_SBOX_SIZE = 256 };
enum { NUM_OF_BYTES = 8 };
enum { ROUNDS = 2 };

const char vr_sbox[];
void sr_layer(unsigned char* iState);
void transpose(unsigned char* iState);
void sc_layer(unsigned char* iState);
void encrypt(unsigned char* plaintext);
void testVectors();
unsigned char cplaintext[8];
const char vr_sbox[] = { 51, 36, 78, 193, 141, 154, 231, 21, 240, 127, 89, 171, 2, 104, 188, 214, 66, 85, 63, 176, 252, 235, 150, 100, 129, 14, 40, 218, 115, 25, 205, 167, 228, 243, 153, 22, 90, 77, 48, 194, 39, 168, 142, 124, 213, 191, 107, 1, 28, 11, 97, 238, 162, 181, 200, 58, 223, 80, 118, 132, 45, 71, 147, 249, 216, 207, 165, 42, 102, 113, 12, 254, 27, 148, 178, 64, 233, 131, 87, 61, 169, 190, 212, 91, 23, 0, 125, 143, 106, 229, 195, 49, 152, 242, 38, 76, 126, 105, 3, 140, 192, 215, 170, 88, 189, 50, 20, 230, 79, 37, 241, 155, 81, 70, 44, 163, 239, 248, 133, 119, 146, 29, 59, 201, 96, 10, 222, 180, 15, 24, 114, 253, 177, 166, 219, 41, 204, 67, 101, 151, 62, 84, 128, 234, 247, 224, 138, 5, 73, 94, 35, 209, 52, 187, 157, 111, 198, 172, 120, 18, 149, 130, 232, 103, 43, 60, 65, 179, 86, 217, 255, 13, 164, 206, 26, 112, 186, 173, 199, 72, 4, 19, 110, 156, 121, 246, 208, 34, 139, 225, 53, 95, 32, 55, 93, 210, 158, 137, 244, 6, 227, 108, 74, 184, 17, 123, 175, 197, 134, 145, 251, 116, 56, 47, 82, 160, 69, 202, 236, 30, 183, 221, 9, 99, 203, 220, 182, 57, 117, 98, 31, 237, 8, 135, 161, 83, 250, 144, 68, 46, 109, 122, 16, 159, 211, 196, 185, 75, 174, 33, 7, 245, 92, 54, 226, 136 };
const char present_sbox[] = { 12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2 };
const char transp[] = { 63,55,47,39,31,23,15,7,
					   62,54,46,38,30,22,14,6,
					   61,53,45,37,29,21,13,5,
					   60,52,44,36,28,20,12,4,
					   59,51,43,35,27,19,11,3,
					   58,50,42,34,26,18,10,2,
					   57,49,41,33,25,17,9,1,
					   56,48,40,32,24,16,8,0 };

void sr_layer(unsigned char* iState)
{
	int i = 0;
	for (i = 0; i < 8; i++) {
		iState[i] = vr_sbox[iState[i]];
	}
}


void transpose(unsigned char* iState)
{
	unsigned char c[8];
	unsigned char* tmp = c;
	size_t i = 0;
	size_t j = 0;
	for (i = 0; i < 8; i++)
	{
		tmp[i] = 0;

		for (j = 0; j < 8; j++)
		{
			tmp[i] ^= (((iState[j] >> (7 - i)) & 1) << (7 - j));
		}

	}
	for (i = 0; i < 8; i++)
	{
		iState[i] = tmp[i];
	}

}

void sc_layer(unsigned char* iState) {
	transpose(iState);
	sr_layer(iState);
	transpose(iState);
}

void encrypt(unsigned char* iState)
{
	size_t i = 0;
	for (i = 0; i < ROUNDS; i++)
	{
		sr_layer(iState);
		sc_layer(iState);
		//sr_layer(iState);
	}
	sr_layer(iState);
}

void testVectors(unsigned char* plaintext)
{
	unsigned char i = 0;
	encrypt(plaintext);
	for (i = 0; i < 8; i++)
	{
		cplaintext[i] ^= plaintext[i];
	}
}

void resetPlain(unsigned char *plain) {
	int i;
    for ( i = 0; i < 8; i++) {
		plain[i] = 0x00; 
    }
   
}
int main(){
	unsigned long i,k,l,j;
	unsigned char plain[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char* p = plain;
	time_t T;
	struct tm *t;
	for(i = 0; i < 256 ; i++){
		T = time(NULL);
		t = localtime(&T);
		printf("|%d  %s",i,asctime(t));
			for(k = 0; k < 256 ; k++){
				for(l = 0; l < 256 ; l++){
					resetPlain(p);
					plain[0] = 0x55;
    				plain[2] = 0x55;
					p[1] = i;
					p[3] = k;
					p[7] = l;
					testVectors(p);
				}
			}
	}
	for (i = 0; i < 8; i++){
		printf("%02x  ",cplaintext[i]);
	}
	return 0;
}
```

