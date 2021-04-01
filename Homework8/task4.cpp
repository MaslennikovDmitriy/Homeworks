#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>
#include <boost/lockfree/stack.hpp>
#include <boost/lockfree/queue.hpp>
#include <thread>
#include <vector>
#include <functional>

template < typename T >
class Threadsafe_Stack
{
public:

	Threadsafe_Stack() = default;

	Threadsafe_Stack(const Threadsafe_Stack& other)
	{
		std::lock_guard <std::mutex> lock(other.m_mutex);
		m_data = other.m_data;
	}

	Threadsafe_Stack& operator=(const Threadsafe_Stack&) = delete;

public:

	void push(T value)
	{
		std::lock_guard < std::mutex > lock(m_mutex);
		m_data.push(value);
	}

	std::shared_ptr < T > pop()
	{
		std::lock_guard < std::mutex > lock(m_mutex);

		if (m_data.empty())
		{
			throw std::range_error("empty stack");
		}

		const auto result = std::make_shared < T >(m_data.top());
		m_data.pop();

		return result;
	}

	void pop(T& value)
	{
		std::lock_guard < std::mutex > lock(m_mutex);

		if (m_data.empty())
		{
			throw std::range_error("empty stack");
		}

		value = m_data.top();
		m_data.pop();
	}

	bool empty() const
	{
		std::lock_guard < std::mutex > lock(m_mutex);
		return m_data.empty();
	}

private:

	std::stack <T> m_data;

private:

	mutable std::mutex m_mutex;
};



template <typename T>
void Threadsafe_Stack_Adding(Threadsafe_Stack <T>& stack, const size_t M)
{
	for (int i = 0; i < M; i++)
	{
		stack.push(i);
	}
}

template <typename T>
void Threadsafe_Stack_Removing(Threadsafe_Stack <T>& stack, const size_t M)
{
	for (int i = 0; i < M; i++)
	{
		stack.pop(i);
	}
}




template <typename T>
void Boost_Stack_Adding(boost::lockfree::stack<T>& stack, const size_t M)
{
	for (int i = 0; i < M; i++)
	{
		stack.push(i);
	}
}

template <typename T>
void Boost_Stack_Removing(boost::lockfree::stack<T>& stack, const size_t M)
{
	for (int i = 0; i < M; i++)
	{
		stack.pop(i);
	}
}

template <typename T>
void Tester(Threadsafe_Stack <T>& stack1, boost::lockfree::stack<T>& stack2, const size_t N, const size_t M)
{
	std::vector<std::thread> Threads(2 * N);
	for (size_t i = 0; i < N; i++)
	{
		Threads[i] = std::thread(Threadsafe_Stack_Adding <T>, std::ref(stack1), M);
	}
	for (size_t i = N; i < 2 * N; i++)
	{
		Threads[i] = std::thread(Threadsafe_Stack_Removing <T>, std::ref(stack1), M);
	}
	auto now1 = std::chrono::system_clock::now();
	std::for_each(Threads.begin(), Threads.end(), std::mem_fn(&std::thread::join));
	auto end1 = std::chrono::system_clock::now();
	auto elapsed1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - now1);
	std::cout << "Threadsafe_Stack processing time(microseconds): " << elapsed1.count() << std::endl;


	for (size_t i = 0; i < N; i++)
	{
		Threads[i] = std::thread(Boost_Stack_Adding <T>, std::ref(stack2), M);
	}
	for (size_t i = N; i < 2 * N; i++)
	{
		Threads[i] = std::thread(Boost_Stack_Removing <T>, std::ref(stack2), M);
	}
	auto now2 = std::chrono::system_clock::now();
	std::for_each(Threads.begin(), Threads.end(), std::mem_fn(&std::thread::join));
	auto end2 = std::chrono::system_clock::now();
	auto elapsed2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - now2);
	std::cout << "Boost_Stack processing time(microseconds): " << elapsed2.count() << std::endl;
}



int main()
{
	Threadsafe_Stack <int> stack1;
	boost::lockfree::stack<int> stack2(1);
	int N = 0, M = 0;
	std::cout << "Enter the number of threads: ";
	std::cin >> N;
	std::cout << "Enter the number of elements: ";
	std::cin >> M;
	std::cout << std::endl;
	Tester(stack1, stack2, N, M);
	system("pause");
	return EXIT_SUCCESS;
}