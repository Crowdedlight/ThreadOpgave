#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <atomic>
using namespace std; 

vector<int> primes = {1,2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
int sze;
mutex arr_mutex;
bool notprime(int num)
{
	for (auto prim : primes)
	{
		if (prim == num)
			return false;
	}
	return true;
}

bool carryon(vector<int> arr)
{
	bool prim = false;
	arr_mutex.lock();
	for (auto i : primes)
	{
		prim = false;
		for (auto j : arr)
		{
			if (j == i)
			{
				prim = true;
				break;
			}
		}
		if (!prim)
			{
				arr_mutex.unlock();
				return true;
			}
	}
	arr_mutex.unlock();
	return false;
}


void insertNum(vector<int> &arr)
{
	int p, tal;
	while (carryon(arr))
	{
		p = 0;
		tal = 0;
		tal = rand() % 50 + 1;

		arr_mutex.lock();
		cout << tal << endl;
		arr.push_back(tal);
		arr_mutex.unlock();

		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

void removeNums(vector<int> &arr)
{
	while (carryon(arr))
	{
		arr_mutex.lock();
		for (int i = 0; i < arr.size(); i++)
		{
			if (notprime(arr[i]))
			{
				arr.erase(arr.begin() + i);
			}
		}
		arr_mutex.unlock();

		this_thread::sleep_for(chrono::milliseconds(500));
	}
}

int main()
{

	vector<int> arr;

	for (int i = 0; i < 200; i++)
		arr.push_back(0);

	thread producer{ insertNum, ref(arr) };
	thread consumer{ removeNums,ref(arr) };

	producer.join();
	consumer.join();

	for (auto i : arr)
	{
		cout << i << ",";
		if (i == 0)
		{
			sze++;
			break;
		}
		sze++;
	}

	cout << "sze: " << sze << endl;
	return 0;
}

