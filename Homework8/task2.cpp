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
    for (size_t i = 0; i < 15; i++)
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

int main()
{
    std::string main_string;
    Rand_String_Generator(main_string);
    std::cout << main_string;
    system("pause");
    return EXIT_SUCCESS;
}