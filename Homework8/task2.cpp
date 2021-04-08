#include <iostream>
#include <random>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <string>

std::mutex _mutex;

void Rand_String_Generator(std::string& main_string)
{
    std::random_device rd;
    std::mt19937 mersenne(rd());
    for (size_t i = 0; i < 100; i++)
    {
        std::uniform_int_distribution<> un_distrib(0, 99);
        if (un_distrib(mersenne) < 25)
        {
            main_string.push_back('A');
        }
        else if (un_distrib(mersenne) < 50)
        {
            main_string.push_back('G');
        }
        else if (un_distrib(mersenne) < 75)
        {
            main_string.push_back('T');
        }
        else if (un_distrib(mersenne) < 100)
        {
            main_string.push_back('C');
        }
    }
}

int Checker(char a, char b)
{
    if (a == b)
    {
        return 1;
    }
    else return 0;
}

void Substring_Searcher(std::string main_string, std::string substring, std::vector<size_t>& Iterators, size_t left, size_t right)
{
    _mutex.lock();
    int count = 0;
    for (size_t i = left; i < right; i++)
    {
        for (size_t j = 0; j < substring.size(); j++)
        {
            if (i + j < main_string.size())
            {
                count += Checker(main_string[i + j], substring[j]);
            }
        }
        if (count == substring.size())
        {
            Iterators.push_back(i);
        }
        count = 0;
    }
    _mutex.unlock();
}

void parallel_algorithm(std::string main_string, std::string substring, std::vector<size_t>& Iterators)
{
    const std::size_t num_core = std::thread::hardware_concurrency();
    int hardware_threads = num_core != 0 ? num_core : 12;
    int thread_x_size = main_string.size() / hardware_threads;
    int residue = main_string.size() % hardware_threads;
    std::vector<std::thread> threads(hardware_threads);
    double left = 0, right = thread_x_size;
    for (size_t i = 0; i < threads.size(); i++)
    {
        threads[i] = std::thread(Substring_Searcher, main_string, substring, std::ref(Iterators), left, right);
        left += thread_x_size;
        right += thread_x_size;
    }

    if (residue != 0)
    {
        threads.push_back(std::thread(Substring_Searcher, main_string, substring, std::ref(Iterators), left, main_string.size()));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int main()
{
    std::string main_string, substring;
    Rand_String_Generator(main_string);
    std::cout << main_string;
    std::cout << std::endl;
    std::cout << "Insert the fragment you are looking for: ";
    std::cin >> substring;
    std::vector<size_t> Iterators;
    parallel_algorithm(main_string, substring, Iterators);
    for (size_t i = 0; i < Iterators.size(); i++)
    {
        std::cout << "The beginning of " << i + 1 << " substring: " << Iterators[i] << std::endl;
    }
    if (Iterators.size() == 0)
    {
        std::cout << "No matches" << std::endl;
    }
    system("pause");
    return EXIT_SUCCESS;
}