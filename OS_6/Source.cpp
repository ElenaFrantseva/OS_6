#include <vector>
#include <list>
#include <shared_mutex>
#include <thread>

#include <iostream>
#define FULL 5
using namespace std;

void Consumer(int time);
void Producer(int time);
void PrintVector();
bool IsEmpty(unique_lock<mutex>& lock);
bool IsFull(unique_lock<mutex>& lock);

mutex mut;
vector<int> st;


int main()
{

	//thread t1([]() {Consumer(300);  });
	//thread t2([]() {Producer(600);  });

	thread t1([]() {Consumer(600);  });
	thread t2([]() {Producer(300);  });
	//thread t3([]() {Consumer(600);  });

	t1.join();
	t2.join();
	//t3.join();

	return 1;
}



void Consumer(int time)
{
	unique_lock <mutex> lock(mut, defer_lock);

	while (true)
	{
		lock.lock();
		if (!IsEmpty(lock))
		{
			st.pop_back();
			cout << "Consumer " << this_thread::get_id() << " :: ";
			PrintVector();
		}
		else
		{
			cout << "Consumer Wait " << this_thread::get_id() << endl;
		}
		lock.unlock();
		this_thread::sleep_for(chrono::milliseconds(time));
	};

}

void Producer(int time)
{
	unique_lock<mutex> lock(mut, defer_lock);
	lock.lock();
	cout << this_thread::get_id() << " PRODUCER STARTED" << endl;
	lock.unlock();


	while (true)
	{
		lock.lock();
		if (!IsFull(lock))
		{
			st.push_back(1);
			cout << "PRODUCER " << this_thread::get_id() << " :: ";
			PrintVector();
		}
		else
		{
			cout << "Producer Wait " << this_thread::get_id() << endl;
		}
		lock.unlock();
		this_thread::sleep_for(chrono::milliseconds(time));
	}

	lock.lock();
	cout << this_thread::get_id() << " Consumer END" << endl;
	lock.unlock();
};


void PrintVector()
{
	for (int i = 0; i < st.size(); i++)
	{
		cout << "\t" << st[i];
	}cout << endl;
}


bool IsEmpty(unique_lock<mutex>& lock)
{
	bool temp = st.empty();
	return temp;
};


bool IsFull(unique_lock<mutex>& lock)
{
	bool temp;
	if (st.size() == FULL)
	{
		temp = true;
	}
	else
	{
		temp = false;
	};
	return temp;
}
