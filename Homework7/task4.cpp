#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <ppl.h>

using namespace concurrency;

void multiplication_by_two(int& i)
{
    i *= 2;
}

int main()
{
    std::vector <int> v(50);
    std::vector <int> v2(50);
    std::random_device rd;
    std::mt19937 mersenne(rd());
    for (int i = 0; i < v.size(); i++)
    {
        std::uniform_int_distribution<> un_distrib(0, 50);
        v[i] = un_distrib(mersenne);
    }
    std::cout << "Comparing transform & parallel_transform: " << std::endl;
    auto now1 = std::chrono::system_clock::now();
    std::transform(v.begin(), v.end(), v.begin(), [](int n) {return n * 2; });
    auto end1 = std::chrono::system_clock::now();
    auto elapsed1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - now1);
    std::cout << "transform algorithm working time (microseconds): " << elapsed1.count() << std::endl;

    auto now2 = std::chrono::system_clock::now();
    parallel_transform(v.begin(), v.end(), v.begin(), [](int n) {return n * 2; });
    auto end2 = std::chrono::system_clock::now();
    auto elapsed2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - now2);
    std::cout << "parallel_transform algorithm working time (microseconds): " << elapsed2.count() << std::endl;

    std::cout << std::endl;

    std::cout << "Comparing for_each & parallel_for_each: " << std::endl;
    auto now3 = std::chrono::system_clock::now();
    std::for_each(v.begin(), v.end(), multiplication_by_two);
    auto end3 = std::chrono::system_clock::now();
    auto elapsed3 = std::chrono::duration_cast<std::chrono::microseconds>(end3 - now3);
    std::cout << "for_each algorithm working time (microseconds): " << elapsed3.count() << std::endl;

    auto now4 = std::chrono::system_clock::now();
    parallel_for_each(v.begin(), v.end(), multiplication_by_two);
    auto end4 = std::chrono::system_clock::now();
    auto elapsed4 = std::chrono::duration_cast<std::chrono::microseconds>(end4 - now4);
    std::cout << "parallel_for_each algorithm working time (microseconds): " << elapsed4.count() << std::endl;

    std::cout << std::endl;

    for (int i = 0; i < v.size(); i++)
    {
        v2[i] = v[i];
    }

    std::cout << "Comparing sort & parallel_sort: " << std::endl;
    auto now5 = std::chrono::system_clock::now();
    std::sort(v.begin(), v.end());
    auto end5 = std::chrono::system_clock::now();
    auto elapsed5 = std::chrono::duration_cast<std::chrono::microseconds>(end5 - now5);
    std::cout << "sort algorithm working time (microseconds): " << elapsed5.count() << std::endl;

    auto now6 = std::chrono::system_clock::now();
    parallel_sort(v2.begin(), v2.end());
    auto end6 = std::chrono::system_clock::now();
    auto elapsed6 = std::chrono::duration_cast<std::chrono::microseconds>(end6 - now6);
    std::cout << "parallel_sort algorithm working time (microseconds): " << elapsed6.count() << std::endl;
    system("pause");
    return EXIT_SUCCESS;
}