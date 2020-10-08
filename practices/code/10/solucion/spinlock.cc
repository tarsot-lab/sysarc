//---------------------------------------------------------
// spinlock.cc
//---------------------------------------------------------

#include <unistd.h>  // alarm
#include <algorithm> // for_each
#include <atomic>    // atomic
#include <iostream>  // cout endl
#include <sstream>   // stringstream
#include <thread>    // thread

//---------------------------------------------------------

const int N = 32;

//---------------------------------------------------------

class barrera_t
{
public:
	explicit barrera_t(unsigned count): 
		m_count(count), m_generation(0), m_count_reset_value(count) {}

	void esperar()
	{
		unsigned gen = m_generation;
		
		if (--m_count != 0)
		{
			while ((gen == m_generation) && (m_count != 0))
				std::this_thread::yield();
		}
		else
		{
			if (m_generation.compare_exchange_weak(gen, gen + 1))
			{
				m_count = m_count_reset_value;
			}
		}
	}

private:
	std::atomic<unsigned> m_count;
	std::atomic<unsigned> m_generation;
	unsigned m_count_reset_value;
} barrera(N);

//---------------------------------------------------------

void hebra()
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	std::string antes = ss.str() + ": antes\n", despues = ss.str() + ": después\n";
	
	while(true)
	{
		std::cout << antes;
		barrera.esperar();
		std::cout << despues;
	}
}
//---------------------------------------------------------

int main()
{
	alarm(1);
	std::thread t[N];
	for (auto& i: t) i = std::thread(hebra);
	for (auto& i: t) i.join();
}

//---------------------------------------------------------
