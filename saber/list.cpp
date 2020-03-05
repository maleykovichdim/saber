#include "pch.h"
#include "list.h"


namespace saber {

	List::~List()
	{
		ListNode * cur = head;
		for (int i = 0; i < count; i++)
		{
			ListNode * next = cur->next;
			delete cur;
			cur = next;
		}
	}


	void List::Serialize(FILE * file) {

			if (!file) return;

			std::vector<std::pair<int, std::string>> serialization_data;
			{
				std::vector<ListNode *> adresses;
				ListNode * cur = head;
				while (true)
				{
					adresses.push_back(cur);
					if (cur == tail) break;
					cur = cur->next;
				}
				for (int i = 0; i < adresses.size(); i++)
				{
					ListNode * cur = adresses[i];
					if (cur->rand == nullptr)
					{
						serialization_data.push_back(std::pair<int, std::string>(-1, cur->data));
					}
					else
					{
						int k = -1;
						for (int i = 0; i < adresses.size(); i++)
						{
							if (cur->rand == adresses[i])
							{
								k = i;
								break;
							}
						}
						assert(k >= 0);
						serialization_data.push_back(std::pair<int, std::string>(k, cur->data));
					}
				}
			}

			assert(serialization_data.size() == count);

			//buffer for disk
			size_t size_buf = (128 + 4) * count;
			char * buf = new char[size_buf];
			if (!buf)
			{
				assert(0);
				return;
			}
			char * cur_buf = buf;
			size_t pos_buf = 0;

			//start tag
			memcpy(cur_buf, &id_start_tag, sizeof(int));//save start tag
			pos_buf += sizeof(int);
			cur_buf += sizeof(int);


			pos_buf += sizeof(size_t);//reserved for data size
			cur_buf += sizeof(size_t);


			//1
			memcpy(cur_buf , &count, sizeof(int));//save count
			pos_buf += sizeof(int) ;//
			cur_buf += sizeof(int) ;

			//2
			for (int i = 0; i < serialization_data.size(); i++)
			{
				std::pair<int, std::string> _node = serialization_data[i];

				int rand = _node.first;// num of rand node
				std::string str = _node.second;
				int len_str = (int)str.size();//size of string
				const char * str_char = str.c_str();//string

				int offset = sizeof(int) + sizeof(int) + len_str;
				if (pos_buf + offset >= size_buf - sizeof(int))//realloc if it is needed
				{
					size_buf = size_buf << 1;
					char * buf_temp = new char[size_buf];
					if (!buf_temp)
					{
						assert(0);
						return;
					}
					memcpy(buf, buf_temp, size_buf >> 1);
					char * t = buf;
					buf = buf_temp;
					delete[] t;
				}

				memcpy(cur_buf, &rand, 4);
				memcpy(cur_buf + 4, &len_str, 4);
				memcpy(cur_buf + 8, str_char, len_str);

				pos_buf += offset;//
				cur_buf += offset;
			}

			pos_buf += sizeof(int);//for finish tag


			memcpy(buf + sizeof(int), &pos_buf, sizeof(size_t));//save count
			memcpy(cur_buf, &id_finish_tag, sizeof(int));
			cur_buf += sizeof(int);

			/////////////////////////////////////////////////////////

			fwrite(buf, pos_buf, 1, file);

			/////////////////////////////////////////////////////////

			delete[] buf;

		};  // сохранение в файл (файл открыт с помощью fopen(path, "wb"))


		void List::Deserialize(FILE * file) {

			if (!file) return;

			int start_code;
			size_t size;


			fread(&start_code, sizeof(int), 1, file);
			fread(&size, sizeof(size_t), 1, file);

			if (start_code != id_start_tag)
			{
				assert(0);
				return;
			}

			char * buf = new char[size];
			size -= sizeof(size_t) + sizeof(int);

			size_t rs = fread(buf, 1, size , file);
			if (rs != size)
			{
				delete[] buf;
				assert(0);///?? todo
				return;
			}
			char * cur = buf;

			int finish_code;
			memcpy(&finish_code, buf + size - 4, 4);
			if (finish_code != id_finish_tag)
			{
				assert(0);
				return;
			}
			size -= 4;

			memcpy(&count, cur, sizeof(int));
			cur += sizeof(int);
			size -= 4;

			std::vector<ListNode *> _nodes;
			{
				head = new ListNode();
				head->prev = nullptr;
				ListNode * prev = head;

				for (int i = 0; i < count - 1; i++)
				{
					ListNode * cur = new ListNode();
					prev->next = cur;
					cur->prev = prev;
					prev = cur;
				}
				prev->next = nullptr;
				tail = prev;

				ListNode * cur = head;
				for (int i = 0; i < count; i++)
				{
					_nodes.push_back(cur);
					cur = cur->next;
				}
			}



			for (int i = 0; i < count; i++)
			{

				ListNode * node = _nodes[i];
				int rand;
				int len_str;
				memcpy(&rand, cur, sizeof(int));
				memcpy(&len_str, cur + sizeof(int), sizeof(int));
				node->data = std::move(std::string(cur + sizeof(int) + sizeof(int), len_str));
				if (rand == -1)
					node->rand = nullptr;
				else
					node->rand = _nodes[rand];
				cur += sizeof(int) + sizeof(int) + len_str;
			}


			delete[] buf;
		};  // загрузка из файла (файл открыт с помощью fopen(path, "rb"))





		void List::random_Self_Filling()
		{
			count = 8 + rand() % 20;

			head = new ListNode();
			std::vector<ListNode*> _nodes;
			_nodes.push_back(head);
			head->data = std::move(std::string(rand() % 40, (char)(rand() % 255)));

			ListNode * prev = head;
			head->prev = nullptr;
			for (int i = 0; i < count - 1; i++)
			{
				ListNode * cur = new ListNode();

				cur->data = std::move(std::string(rand() % 40, (char)(rand() % 127)));

				prev->next = cur;
				cur->prev = prev;
				_nodes.push_back(cur);
				prev = cur;
			}
			tail = prev;
			tail->next = nullptr;

			for (int i = 0; i < count; i++)
			{
				ListNode * cur = _nodes[i];
				cur->rand = _nodes[rand()%count];
			}
		}


		void List::print()
		{
			ListNode * cur = head;
			printf("---- count = %d -------------------\n", count);
			int a = 0;
			while (true) {
				if (!cur) break;
				a++;
				printf("%d   string=%s ", a, cur->data.c_str());
				if (cur->rand) 
					printf("       rand=%s \n", cur->rand->data.c_str());
				else
					printf("       rand=NULL \n");
				cur = cur->next;
			}
			printf("-------------------------------\n");
		}



}



