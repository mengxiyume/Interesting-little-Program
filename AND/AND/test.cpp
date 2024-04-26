#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
using namespace std;
#include <string>
#include <cassert>

#include <Windows.h>

bool isBig() {
	int n = 0x000000FF;
	return (bool)((char*)(&n));
}

void printUInt32_byte(char32_t value) {
//MSVC编译器不支持使用变量初始化数组，用def将就一下
#define u32Length_bit 32
#define byteLength_bit 8
#define byteLength_0x 2
#define u32Length_byte (u32Length_bit / byteLength_bit)
//#################################################

	char32_t Mask = 0b00000000000000000000000000000001;
	char szBuffer_bit[u32Length_bit + u32Length_byte + 1] = { 0 };
	char szBuffer_byte[(byteLength_0x + 3) * u32Length_byte + 1] = { 0 };
	unsigned char pos_0b = 0;
	const unsigned char* addr_0x = (const unsigned char*)&value;

	//当前平台小端字节序

	for (int i = 0; i < u32Length_byte; i++) {

		for (int j = 0; j < byteLength_bit; j++) {
			szBuffer_bit[pos_0b] = ((value & (Mask << (pos_0b - i))) >> (pos_0b - i)) + '0';
			pos_0b++;
		}
		szBuffer_bit[pos_0b] = ' ';
		sprintf(szBuffer_byte + ((byteLength_0x + 3) * i), "0x%02x ", *(addr_0x + i));
		pos_0b++;
	}
	printf("0b %s\n", szBuffer_bit);
	printf("0x %s\n", szBuffer_byte);

//MAVC编译器不支持使用变量初始化数组，用def将就一下
#undef u32Length_bit
#undef byteLength_bit
#undef u32Length_byte
//#################################################
}

void putstr_ch32(const char32_t* str, size_t length) {
	wchar_t* szBuffer = (wchar_t*)calloc(sizeof(*str), length);
	assert(szBuffer);

	//硬核转码
	for (int i = 0; i < length; i++) {
		szBuffer[i] = (wchar_t)*str;
		str++;
	}

	//打印与缓冲区释放
	wprintf(szBuffer);
	free(szBuffer);
	szBuffer = nullptr;
}

wstring strAnd_bit(wstring str1, wstring str2) {

	size_t len1 = str1.length();
	size_t len2 = str2.length();

	wstring ret;

	//遍历每一个字符
	int i = 0;
	for (i = 0; (i < len1) && (i < len2); i++) {
		wchar_t temp = (wchar_t)(((int)str1.at(i)) & ((int)str2.at(i)));
		ret += temp;
	}
	for (; (i < len1) || (i < len2); i++) {
		wchar_t temp = i < len1 ? str1.at(i) : str2.at(i);
		ret += temp;
	}
	
	return ret;
}

wstring strSum(wstring str1, wstring str2) {
	size_t len1 = str1.length();
	size_t len2 = str2.length();

	wstring ret;

	//遍历每一个字符
	int i = 0;
	for (i = 0; (i < len1) && (i < len2); i++) {
	//拿到区间数值，相加在取模
		//				拿到字符在范围内可以对应的值	为后增加范围初始值后不溢出方式
		wchar_t ch1 = (((int)(str1.at(i)) >= 0x4E00)) ? (((int)(str1.at(i))) - 0x4E00) : ((int)(str1.at(i)));
		wchar_t ch2 = (((int)(str2.at(i)) >= 0x4E00)) ? (((int)(str2.at(i))) - 0x4E00) : ((int)(str2.at(i)));
		wchar_t result = ((ch1 + ch2) % (0x9FFF - 0x4E00)) + 0x4E00;
	//汉字编码范围
	//0x4E00…0x9FFF
	//输出值范围为汉字编码范围
		ret += result;
	}
	for (; (i < len1) || (i < len2); i++) {
		wchar_t result = i < len1 ? str1.at(i) : str2.at(i);
		ret += result;
	}

	return ret;
}

int main() {
	wstring str1 = L"\5";
	wstring str2 = L"\5";

	//wstring str3 = strAnd_bit(str1, str2);
	wstring str3 = strSum(str1, str2);

	MessageBox(nullptr, str3.c_str(), (str1 + L" + " + str2 + L" =").c_str(), MB_OK);

	return 0;
}