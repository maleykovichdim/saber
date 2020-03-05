#include "pch.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <stdio.h>


#include "list.h" //third task

using namespace saber;

// first task
void print_int_to_binary(int inp)
{
	unsigned int a = (unsigned int) inp;
	std::cout << inp <<" = ";
	for (int i= 31; i >= 0; i--)
		std::cout << ((a & (1<<i))? "1":"0");
	std::cout <<  std::endl;
}

//second task
void RemoveDups(char* str)
{
	if (!str) return;

	int len = (int)strlen(str);
	char * temp = new char[len + 1];

	int pos = 0;

	temp[pos++] = str[0];//first symbol

	for (int i = 1; i < len; i++)
	{
		if (str[i] != str[i - 1]) 
			temp[pos++] = str[i];
	}

	memset(str, '\0', len);
	memcpy(str, temp, pos);


	delete[] temp;
}






int main()
{
	//3
	{
		List a;
		List b;
		a.random_Self_Filling();
		printf("before Serialization:\n\n");
		a.print();
		FILE *stream, *stream2;
		errno_t err = fopen_s(&stream, "c:\\work\\serialize.bin", "wb");
		a.Serialize(stream);
		fclose(stream);
		err = fopen_s(&stream2, "c:\\work\\serialize.bin", "rb");
		b.Deserialize(stream2);
		printf("\n\nafter DeSerialization:\n\n");
		b.print();
		fclose(stream2);
	}

	//1
	{
		printf("\n\nprint_int_to_binary\n\n");
		print_int_to_binary(-45);
	}

	{

		printf("\n\RemoveDups\n\n");

		// пример использования
		char data[] = "AAA BBB AAA";
		RemoveDups(data);
		printf("%s\n", data); // “A B A”

	}


}

