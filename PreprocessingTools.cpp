
#include "PreprocessingTools.h"
using namespace std;

map<std::string, std::tuple<int, std::string>>GatherData() {
    map<string, tuple<int, string>> returnMap;
    std::ifstream inputFile;
    inputFile.open("data.txt");
    string inputString;
    string splitTemp;
    if (inputFile.is_open()) {
        while (!inputFile.eof()) {

            for(int i = 0; i < 50 ; i += 1){
                getline(inputFile,splitTemp, ' ');
                // lambda here parses the location <alpha> here 
                returnMap.emplace(splitTemp, [&]() {
                    string splitTemp2;
                    getline(inputFile, splitTemp, ' ');
                    getline(inputFile, splitTemp2);
                    std::cout << splitTemp2 << endl;
                    char tempChar = splitTemp[0];
                    // tuple<int, string> tempTuple(atoi(&tempChar), splitTemp2);
                    return std::make_tuple(atoi(&tempChar),splitTemp2);
                });

            };

        };
    }
    return returnMap;
};