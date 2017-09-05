// ========================================================================
// 
//       Filename:  times.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  05.09.2017 21:50:23
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Luis Díaz Más (), i22dimal@uco.es
//        Company:  UCO
// 
// ========================================================================

#include <ctime>
#include <vector>
#include <iomanip>
#include <iostream>

using namespace std;

bool   checkTime()
{
    auto now = std::time(nullptr);
    std::vector<std::tm> tms(5, *std::localtime(&now));

    std::tm tmInit = *std::localtime(&now);
    std::tm tmEnd = *std::localtime(&now);

    tms[0].tm_hour = 8;
    tms[0].tm_min = 25;
    tms[0].tm_sec = 0;

    tms[1].tm_hour = 9;
    tms[1].tm_min = 30;
    tms[1].tm_sec = 0;

    tms[2].tm_hour = 19;
    tms[2].tm_min = 30;
    tms[2].tm_sec = 0;

    tms[3].tm_hour = 20;
    tms[3].tm_min = 31;
    tms[3].tm_sec = 0;

    tms[4].tm_hour = 2;
    tms[4].tm_min = 31;
    tms[4].tm_sec = 0;

    tmInit.tm_hour = 8;
    tmInit.tm_min = 30;
    tmInit.tm_sec = 0;

    tmEnd.tm_hour = 20;
    tmEnd.tm_min = 30;
    tmEnd.tm_sec = 0;

    std::ostringstream oss;
    std::cout << "t1: " << std::put_time(&tms[0], "%d-%m-%Y_%H-%M-%S") << endl;
    std::cout << "t2: " << std::put_time(&tms[1], "%d-%m-%Y_%H-%M-%S") << endl;
    std::cout << "t3: " << std::put_time(&tms[2], "%d-%m-%Y_%H-%M-%S") << endl;
    std::cout << "t4: " << std::put_time(&tms[3], "%d-%m-%Y_%H-%M-%S") << endl;
    std::cout << "t5: " << std::put_time(&tms[4], "%d-%m-%Y_%H-%M-%S") << endl;
    std::cout << "Init: " << std::put_time(&tmInit, "%d-%m-%Y_%H-%M-%S") << endl;
    std::cout << "End: " << std::put_time(&tmEnd, "%d-%m-%Y_%H-%M-%S") << endl;

    for (auto & t : tms)
    {
        auto seconds1 = difftime(mktime(&t), mktime(&tmInit));
        auto seconds2 = difftime(mktime(&t), mktime(&tmEnd));

        if (seconds1 > 0 && seconds2 < 0)
        {
            std::cout << "good time\n";
        }
        else
        {
            std::cout << "bad time\n";
        }
    }

}

int main()
{
    checkTime();
    return 0;
}
