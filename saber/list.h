#pragma once
#include "pch.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <stdio.h>


// структуру ListNode модифицровать нельзя
struct ListNode {
	ListNode *      prev;
	ListNode *      next;
	ListNode *      rand; // указатель на произвольный элемент данного списка, либо NULL
	std::string     data;
};

namespace saber {



	class List {
	public:

		~List();

		void Serialize(FILE * file);  // сохранение в файл (файл открыт с помощью fopen(path, "wb")) // !!!!! I prefer use stream !!!!!
		void Deserialize(FILE * file); // загрузка из файла (файл открыт с помощью fopen(path, "rb"))

		void random_Self_Filling();//random filling for List
		void print(); //printing list

	protected:

		ListNode *      head;
		ListNode *      tail;
		int       count;

		static const int id_start_tag = 0xF0F0F0F;
		static const int id_finish_tag = 0x8F8F8F8F;
	};

}







