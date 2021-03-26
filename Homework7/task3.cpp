#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <random>

void multiplication_by_two(int& i)
{
    i *= 2;
}

template <typename Iterator, class Function>
int parallel_each(Iterator first, Iterator last, Function func)
{
    const std::size_t length = std::distance(first, last);
    const std::size_t max_size = 25;
    if (length <= max_size)
    {
        std::for_each(first, last, multiplication_by_two);
    }
    else
    {
        Iterator middle = first;
        std::advance(middle, length / 2);
        std::future <int> first_half_result = std::async(parallel_each < Iterator, Function >, first, middle, func);
        int second_half_result = parallel_each(middle, last, func);
        int a = first_half_result.get() + second_half_result;
    }
    return EXIT_SUCCESS;
}

void vector_print(std::vector<int> v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << i + 1 << " element of vector: " << v[i] << std::endl;
    }
}

int main()
{
    std::vector <int> v(25);
    std::random_device rd;
    std::mt19937 mersenne(rd());
    for (int i = 0; i < v.size(); i++)
    {
        std::uniform_int_distribution<> un_distrib(0, 50);
        v[i] = un_distrib(mersenne);
    }
    vector_print(v);
    parallel_each(v.begin(), v.end(), multiplication_by_two);
    std::cout << std::endl;
    std::cout << "Vector after parallel_for_each algorithm:" << std::endl;
    std::cout << std::endl;
    vector_print(v);
    system("pause");
    return EXIT_SUCCESS;
}