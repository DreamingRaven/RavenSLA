//
// Created by George Onoufriou on 15/02/2017.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#define LASER_ACTIVATION_THRESHOLD 0.0f


class Parser {
    // Data = list of G-code
private:
    float x = 0, y = 0, z = 0; // assumed arduino will home
    bool lsr = 0; // 0 off, 1 on
    std::vector<char> data;
    std::vector<char> dataCompressed;
protected:
    int calculateStepsXY(std::string &component, char &mtr);
    int calculateStepsZ(std::string &component);
    float interweaveRatio(int &ySteps, int &xSteps);
    void postInterweave(int &ySteps,int &xSteps,float &ratio);
    void laserModel(std::string &word, char &mtr);
    void heightModel(std::string &word, char &mtr);
    void compress(const char &first);
    void compress(const char &first, const char &second);
public:
    void parseValues(std::string &nonParsedData);
    std::vector<char>* getAddress();
    void tail(std::string &headTail);
    void compressData();


    // packet {000 0 0000} where 000 is identifier for device, 0 is operation, 0000 is the amount of times to repeat
    // Data2 = list of G's-code
};


#endif //PARSER_PARSER_H
