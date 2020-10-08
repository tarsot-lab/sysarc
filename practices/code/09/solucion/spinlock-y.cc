//----------------------------------------------------
// spinlock-y.cc
//----------------------------------------------------

#include <unistd.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

//----------------------------------------------------

using namespace std;

//----------------------------------------------------

const int N = 16;

//----------------------------------------------------

class spinlock
{
public:
	spinlock(): m_locked(ATOMIC_FLAG_INIT) {}

	void adquirir()
	{
		while (m_locked.test_and_set(memory_order_acquire))
			this_thread::yield();
	}

	void liberar()
	{
		m_locked.clear(memory_order_release);
	}

private:
	atomic_flag m_locked;
} c;
//----------------------------------------------------

void seccion_critica()
{
	cout << "[" << this_thread::get_id() << "]: ";
	for(int i = 0; i < 10; ++i)
		cout << i;
	cout << endl;
}

//----------------------------------------------------

void hebra()
{
	while(true)
	{
		c.adquirir();
		seccion_critica();
		c.liberar();
	}
}

//----------------------------------------------------

int main()
{
	alarm(1);

	thread t[N];
	
	for(auto& i: t)
		i = thread(hebra);

	for(auto& i: t)
		i.join();
}

//----------------------------------------------------
