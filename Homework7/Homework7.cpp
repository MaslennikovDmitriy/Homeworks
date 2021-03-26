#include <iostream>
#include <random>
#include <vector>
#include <thread>

struct Point
{
    int x, y;
};

size_t Circle_Checker(int R, std::vector<Point> Points)
{
    unsigned int count = 0;
    for (size_t i = 0; i < Points.size(); i++)
    {
        if (Points[i].x * Points[i].x + Points[i].y * Points[i].y <= R * R)
        {
            count++;
        }
    }
    return count;
}

void thread_Circle_Checker(int left, int right, int R, std::vector<Point> Points, int& count)
{
    for (size_t i = 0; i < Points.size(); i++)
    {
        if ((Points[i].x >= left) && (Points[i].x < right))
        {
            if (Points[i].x * Points[i].x + Points[i].y * Points[i].y <= R * R)
            {
                count++;
            }
        }
    }
}

void parallel_counter(std::vector<Point> Points, int& circle_points_quantity_2)
{
    std::thread thr1(thread_Circle_Checker, 0, 200, 1000, Points, std::ref(circle_points_quantity_2));
    std::thread thr2(thread_Circle_Checker, 200, 400, 1000, Points, std::ref(circle_points_quantity_2));
    std::thread thr3(thread_Circle_Checker, 400, 600, 1000, Points, std::ref(circle_points_quantity_2));
    std::thread thr4(thread_Circle_Checker, 600, 800, 1000, Points, std::ref(circle_points_quantity_2));
    std::thread thr5(thread_Circle_Checker, 800, 1000, 1000, Points, std::ref(circle_points_quantity_2));
    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
}


int main()
{
    unsigned int points_quantity = 0;
    unsigned int circle_points_quantity_1 = 0;
    std::cout << "Insert quantity of points: " << std::endl;
    std::cin >> points_quantity;
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::vector<Point> Points(points_quantity);
    for (int i = 0; i < points_quantity; i++)
    {
        std::uniform_int_distribution<> un_distrib(0, 1000);
        Points[i].x = un_distrib(mersenne);
        Points[i].y = un_distrib(mersenne);
    }
    circle_points_quantity_1 = Circle_Checker(1000, Points);
    std::cout << "Approximate value of the number pi: " << 4 * circle_points_quantity_1 / (double)points_quantity << std::endl;


    int circle_points_quantity_2 = 0;
    parallel_counter(Points, circle_points_quantity_2);
    std::cout << "Approximate value of the number pi (parallel algorithm): " << 4 * circle_points_quantity_2 / (double)points_quantity << std::endl;

    system("pause");
    return EXIT_SUCCESS;
}