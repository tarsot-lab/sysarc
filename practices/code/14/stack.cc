//-----------------------------------------------------------------------------
// stack.cc: memory management with new/delete is not lockfree...
//-----------------------------------------------------------------------------

#include <algorithm>
#include <atomic>
#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <stack>
#include <thread>
#include "demangle.h"

//-----------------------------------------------------------------------------

const int REP = 5;             // test repetitions
const int THREADS = 16;        // maximun number of threads
const unsigned WORKTIME = 111; // milliseconds
const int W = 15;              // column width

//-----------------------------------------------------------------------------

namespace spinlock
{
	template<class T> class stack
	{
	public:
		struct empty: std::exception {};

		void push(const T& x)
		{
		}

		T pop()
		{
		}
	};
}

//-----------------------------------------------------------------------------

namespace lock
{
	template<class T> class stack
	{
	public:
		struct empty: std::exception {};

		void push(const T& x)
		{
		}

		T pop()
		{
		}
	};
}

//-----------------------------------------------------------------------------

namespace lockfree // bug: suffers ABA problem
{
	template<class T> class stack
	{
	public:
		struct empty: std::exception {};

		void push(const T& x)
		{
		}

		T pop()
		{
		}
	};
}

//-----------------------------------------------------------------------------

namespace stm // bug: STM insecure implementation of std::stack
{
	template<class T> class stack
	{
	public:
		struct empty: std::exception {};

		void push(const T& x)
		{
		}

		T pop()
		{
		}
	};
}

//-----------------------------------------------------------------------------

std::atomic<bool> start(false), stop(false);
std::atomic<int> ops(0);

//-----------------------------------------------------------------------------

template<class S> void worker(S& s)
{
	std::random_device rd;
	std::mt19937_64 eng(rd());
	std::bernoulli_distribution dist;
	auto rng = std::bind(dist, eng);

	int o = 0;

	while (!start);
	while (!stop)
	{
		try
		{
			if (rng())
				s.push(1);
			else
				s.pop();
		}
		catch(...)
		{
		}
		++o;
	}

	ops += o;
}

//-----------------------------------------------------------------------------

template<class S> void check(S& s)
{
	const long N = 1000;
	long sum = 0;

	for (int i = 0; i < N; ++i)
		s.push(i);

	for (int i = 0; i < N; ++i)
		sum += s.pop();

	if (sum != N * (0 + N - 1) / 2)
		throw std::logic_error("check failed for class " + ec::demangle(s));
}

//-----------------------------------------------------------------------------

template<class S> void test(int t)
{
	S s;

	check(s);

	std::thread workers[t];

	unsigned rep[REP];

	for (auto& r: rep)
	{
		start = false;
		stop = false;
		ops = 0;

		for (auto& w: workers)
			w = std::thread(worker<S>, std::ref(s));

		start = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(WORKTIME));
		stop = true;

		for (auto& w: workers)
			w.join();

		r = ops;
	}

	std::nth_element(rep, rep + REP / 2, rep + REP);
	std::cout << std::setw(W) << rep[REP / 2];
}

//-----------------------------------------------------------------------------

int main()
{
	std::cout.imbue(std::locale("es_ES"));
	std::cout << std::setw(W) << "hebras" 
	          << std::setw(W) << "spinlock"
	          << std::setw(W) << "lock"
	          << std::setw(W) << "lockfree"
	          << std::setw(W) << "stm" << std::endl;

	for (int i = 1; i <= THREADS; ++i)
	{
		std::cout << std::setw(W) << i;
		test<spinlock::stack<int>>(i);
		test<    lock::stack<int>>(i);
		test<lockfree::stack<int>>(i);
		test<     stm::stack<int>>(i);
		std::cout << std::endl;
	}
}

//-----------------------------------------------------------------------------

