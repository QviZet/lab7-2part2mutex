#include "header.h"

std::mutex mut, mut3;
std::condition_variable con3, conMain;
bool flag = false;
int intCon;

void sortPart(std::string name, std::string* pName, double* arr, int len) {
	double min = 1001;
	int minNum = 0;
	auto beginSort = std::chrono::steady_clock::now();
	for (int i = 0; i < len; i++)
	{
		for (int j = i; j < len; j++) {
			if (arr[j] < min) {
				min = arr[j];
				minNum = j;
			}
		}
		min = arr[i];
		arr[i] = arr[minNum];
		arr[minNum] = min;
		min = 1001;
	}
	for (int i = 0; i < len; i++)
	{
		mut.lock();
		std::cout << name << "\t";
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << arr[i] << std::endl;
		mut.unlock();
	}
	while (*pName != "");
	mut.lock();
	*pName = name;
	intCon++;
	mut.unlock();
	con3.notify_one();
}

void sortArr(std::string name, std::string* pName, double* arr, double* frstPart, double* scndPart, int len) {
	std::unique_lock<std::mutex> lock(mut3);
	while (intCon != 3) {
		if (intCon == 1) {
			mut.lock();
			std::cout << "\n" << *pName << "\tcompleted\n\n";
			intCon = 2;
			*pName = "";
			mut.unlock();
		}
		con3.wait(lock);
	}

	std::cout << "\n" << *pName << "\tcompleted\n\n";

	int frst = 0, scnd = 0, k = 0;
	while ((frst != len / 2) && (scnd != len / 2)) {
		if (frstPart[frst] < scndPart[scnd]) {
			arr[k] = frstPart[frst];
			k++;
			frst++;
		}
		else {
			arr[k] = scndPart[scnd];
			k++;
			scnd++;
		}
	}
	while (frst != len / 2) {
		arr[k] = frstPart[frst];
		k++;
		frst++;
	}
	while (scnd != len / 2) {
		arr[k] = scndPart[scnd];
		k++;
		scnd++;
	}
	for (int i = 0; i < len; i++)
	{
		std::cout << name << "\t";
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << arr[i] << std::endl;
	}
	conMain.notify_one();
}