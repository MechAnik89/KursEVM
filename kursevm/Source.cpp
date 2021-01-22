#include <iostream>
#include <time.h>
#include<fstream>
#include <string>
using namespace std;
const int N = 5;
const int K = 10;
int M;
int Pn;
int Kr;
enum { DENIED, ALLOWED, ACCESSED };
enum { REQUEST, FREEING };
int ACCESS = DENIED;
int programm[N][K];
int memory[N] = { ALLOWED, ALLOWED, ALLOWED };
int tacts[N] = { 0,0,0,0,0 };
void programm_set(string str);
void programm_get();
int mem_req(int request, int num);
int mem_req(int request);
void commutator(string str);
int calculate();
void pci(string str);
void process(int m);
int main() {
	setlocale(LC_ALL, "Rus");
	process(2);
	process(5);
	process(10);
	cout << "Конец работы,результаты записаны" << endl << endl;
}
void programm_set()
{
	for (int id = 1; id <= N; id++)
	{
		srand(time(NULL) - 10 * id);
		int tmp = K - K * Kr / 100;
		int Kp = tmp * Pn / 100;
		int memory = 0;
		for (int i = 0; i < K; i++) {
			if (tmp && rand() % ((K - i) / tmp) == 0)
			{
				if (tmp - Kp != 0 && rand() % (tmp / (tmp - Kp)) == 0)
				{
					while (true)
					{
						memory = rand() % N + 1;
						if (memory != id)
							break;
					}
				}
				else
				{
					memory = id;
					Kp--;
				}
				programm[id - 1][i] = memory;
				tmp--;
			}
			else {
				programm[id - 1][i] = 0;
			}
		}
	}
}
void programm_get(string str)
{
	std::ofstream out(str, std::ios::trunc);
	out.open("ID.txt");
	out << "id\ id1 id2\tid3 \tid4 \tid5" << endl;
	for (int i = 0; i < K; i++)
	{
		out << i + 1 << '\t' << programm[0][i] << '\t' << programm[1][i] << '\t' << programm[2][i] << '\t' << programm[3][i] << '\t' << programm[4][i] << '\t' << endl;
		if (i == K - 1)
		{
			ACCESS = ALLOWED;
			out << endl << endl;
		}
	}
	out.close();
}
int mem_req(int request, int num)
{
	ACCESS = memory[num - 1];
	if (ACCESS == ALLOWED && request == REQUEST)
	{
		ACCESS = DENIED;
		memory[num - 1] = DENIED;
		return ACCESSED;
	}
	else if (ACCESS == DENIED && request == REQUEST)
	{
		return DENIED;
	}
	else if (ACCESS == DENIED && request == FREEING) {
		ACCESS = ALLOWED;
	}
	memory[num - 1] = ALLOWED;
	return ACCESS;
}
int mem_req(int request)
{
	if (ACCESS == ALLOWED && request == REQUEST)
	{
		ACCESS = DENIED;
		return ACCESSED;
	}
	else if (ACCESS == DENIED && request == FREEING) {
		ACCESS = ALLOWED;
	}
	return ACCESS;
}
void commutator(string str)
{
	std::ofstream out(str, std::ios::trunc);
	out.open("Коммутатор.txt");
	int time = 0;
	int access;
	int flag[N] = { 1,1,1,1,1 };
	int id = 0;
	for (int i = 0; i < K; i++)
	{
		for (id = 1; id <= N; id++) {
			if (programm[id - 1][i])
			{
				out << "\nrequest " << programm[id - 1][i] << "\t\tid " << id;
				while (true)
				{
					access = mem_req(REQUEST, programm[id - 1][i]);
					if (access == ACCESSED)
					{
						out << "\naccess " <<
							programm[id - 1][i] << "\t\tid " << id;
						tacts[id - 1] += M * 50;
						break;
					}
					else
					{
						out << "\nWaiting " << programm[id - 1][i] << "\t\tid " << id;
						tacts[id - 1] += 200 * flag[id - 1];
						flag[id - 1]++;
						mem_req(FREEING, programm[id - 1][i]);
					}
				}
			}
			else {
				tacts[id - 1] += 100;
				out << "\nNo request " << "\t\tid " << id;
			}
		}
		for (int i = 1; i <= N; i++)
		{
			flag[i - 1] = 1;
			memory[i] = ALLOWED;
		}
	}
	int tacts = calculate();
	out << "\nNumber of tacts: " << tacts / 50 << endl << endl <<
		endl;
	out.close();
}
void pci(string str)
{
	std::ofstream out(str, std::ios::trunc);
	out.open("Шина.txt");
	int flag = 1;
	int access;
	ACCESS = ALLOWED;
	for (int i = 0; i < K; i++)
	{
		for (int id = 1; id <= N; id++) {
			if (programm[id - 1][i])
			{
				out << "\nRequest " << programm[id - 1][i]
					<< "\t\tid " << id;
				while (true)
				{
					access = mem_req(REQUEST);
					if (access == ACCESSED)
					{
						out << "\nAccess " <<
							programm[id - 1][i] << "\t\tid " << id;
						tacts[id - 1] += M * 50;
						break;
					}
					else
					{
						tacts[id - 1] += 200 * flag;
						flag++;
						out << "\nWaiting " <<
							programm[id - 1][i] << "\t\tid " << id;
						mem_req(FREEING);
					}
				}
			}
			else {
				tacts[id - 1] += 100;
				out << "\nNo request " << "\t\tid " << id;
			}
		}
		ACCESS = ALLOWED;
		flag = 1;
	}
	int tacts = calculate();
	out << "\nNumber of tacts: " << tacts / 50 << endl << endl;
	out.close();
}
int calculate() {
	int time = 0;
	for (int i = 0; i < 10; ++i) {
		if (tacts[i] > time) {
			time = tacts[i];
		}
		tacts[i] = 0;
	}
	return time;
}
void process(int m)
{
	int M = m;
	string str1, str2, str3;
	for (Pn = 60; Pn <= 90; Pn += 30)
	{
		str1 = str2 = str3 = "";
		for (Kr = 60; Kr <= 80; Kr += 10)
		{
			if (Kr == 80)
			{
				Kr += 10;
			}
			str1 = "results/" + to_string(M) + "/Kr=" + to_string(Kr) + "/Pn = " + to_string(Pn) + "/id.txt";
			str2 = "results/" + to_string(M) + "/Kr=" + to_string(Kr) + "/Pn = " + to_string(Pn) + "/Шина.txt";
			str3 = "results/" + to_string(M) + "/Kr=" + to_string(Kr) + "/Pn = " + to_string(Pn) + "/Коммутатор.txt";
			programm_set();
			programm_get(str1);
			pci(str2);
			commutator(str3);
		}
	}
}