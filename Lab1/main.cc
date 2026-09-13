#include "Time.hpp"
#include <iostream>

int main()
{
    try {
        Time time1{"123::456"};
        Time time2{"00:00:16.11"};
        std::cout << time1 << std::endl;
        std::cout << time1.to_string(true) << std::endl;
        std::cout << time1 - time2 << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
    }

    return 0;
}