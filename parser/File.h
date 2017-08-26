//
// George Onoufriou : 15/02/2017.
//
#include <iostream>
#include <sys/stat.h>
#include <fstream>

#ifndef PARSER_FILE_H
#define PARSER_FILE_H

// needs to be able to handle 400MB of data at least!
class File {

private:
    std::string fileData_m;
    std::string fileArr_m[];
protected:
    long getFileSize(const std::string &fPath_t);
public:
    File(); // empty constructor
    int getPtr(std::string &fPtr);
    int readData(const std::string &fPath_t);
    int readData2(const std::string &path_t);
    ~File();
};


#endif //PARSER_FILE_H
