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
enum { ROUNDS = 1 };

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
}

unsigned char bitadd_p(unsigned char* p){
	unsigned char a = ((p[2] >> 6) & 0x01) ^ ((p[2] >> 5) & 0x01) ^ ((p[2] >> 4) & 0x01) ^ ((p[2] >> 1) & 0x01) ^ (p[2] & 0x01);
	return a;
}

unsigned char bitadd_c(unsigned char* p){
	unsigned char b = ((p[2] >> 3) & 0x01) ^ ((p[2] >> 2) & 0x01) ^ ((p[2] >> 1) & 0x01) ^ ((p[6] >> 3) & 0x01) ^ ((p[6] >> 2) & 0x01) ^ ((p[6] >> 1) & 0x01) ^ (p[6] & 0x01) ^ ((p[7] >> 3) & 0x01) ^ ((p[7] >> 2) & 0x01) ^ ((p[7] >> 1) & 0x01) ^ (p[7] & 0x01);
	return b;
}  

void resetPlain(unsigned char *plain) {
	int i;
    for ( i = 0; i < 8; i++) {
		plain[i] = 0x00; 
    }
   
}

unsigned long long generateRandom64BitBinary() {
    unsigned long long result = 0;
    int i = 0;
    // 设置种子，通常使用当前时间
    srand((unsigned int)time(NULL));

    // 生成每一位
    for (i = 0; i < 64; i++) {
        // 通过左移位和或运算生成随机二进制位
        result = (result << 1) | (rand() % 2);
    }

    return result;
}

// 洗牌算法，用于生成不重复的随机数序列
void shuffleArray(int* array, int size) {
	int i = 0; 
    for (i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        // 交换位置
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// 将64位随机数分割成八个8位的数，并以16进制形式存储在数组中
void splitAndStore64BitNumber(unsigned long long randomNumber, unsigned char* resultArray) {
	int i = 0;
    for (i = 0; i < 8; i++) {
        resultArray[i] = (randomNumber >> (i * 8)) & 0xFF;
    }
}

int main() {
	unsigned char A,B;
	unsigned char plain[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char* p = plain;
	int M = 0;
	int i = 0;
    // 生成2^26个随机数
    int size = 1 << 26;
    int* randomNumbers = (int*)malloc(size * sizeof(int));

    // 初始化数组
    for (i = 0; i < size; i++) {
        randomNumbers[i] = generateRandom64BitBinary();
    }

    // 设置种子，通常使用当前时间
    srand((unsigned int)time(NULL));

    // 使用洗牌算法打乱数组
    //shuffleArray(randomNumbers, size);
	// 分割并存储随机数
    
    for(i = 0; i < size; i++){
    	splitAndStore64BitNumber(randomNumbers[i], plain);
		A = bitadd_p(p);
		testVectors(p);
		B = bitadd_c(p);
		if(A == B){
			M++;
		}
		resetPlain(p);
	}
	printf("%d\n",M);
    // 释放内存
    free(randomNumbers);

    return 0;
}
