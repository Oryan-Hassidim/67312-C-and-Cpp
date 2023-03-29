#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    std::ifstream fileIn("myFile.txt");
    int val;
    fileIn >> val;
    while (fileIn.good())
    {
        std::cout << val << " ";
        fileIn >> val;
    }
    return 0;
}