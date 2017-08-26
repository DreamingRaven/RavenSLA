//
// George Onoufriou : 15/02/2017.
//

#include "File.h"

File::File() {

}

File::~File() {

}

int File::getPtr(std::string &fPtr) {
    fPtr = fileData_m;
    return 0;
}

int File::readData(const std::string &fPath_t) {
    // get file size
    try
    {
        long fSize = getFileSize(fPath_t);
        std::cout << "  File Size = " << fSize << " bytes" << std::endl;

        // allocating buffer size to load in file fast
        std::string buffer(fSize, ' ');
        // init stream obj
        std::ifstream text(fPath_t);
        // start reading
        text.read(&buffer[0], fSize);
        fileData_m = buffer; // slowest bit of entire class, could refactor
        std::cout << "  Input Success!" << std::endl;
    }
    catch(std::exception &e)
    {
        std::cout << "  could not process file" << std::endl;
        return -1;
    }

    //std::cout << fileData_m << std::endl;
    return 0;
}

// will continue this to improve speed later, since original involves 4 passes, 3 more than it should have to do
int File::readData2(const std::string &fPath_t)
{
    try
    {
        long fSize = getFileSize(fPath_t);
        std::cout << "  File Size = " << fSize << " bytes" <<std::endl;
        std::string buffer(fSize, ' ');
        std::ifstream text(fPath_t);
        text.read(&buffer[0], fSize);
        std::cout << "  Input Success!" << std::endl;
    }
    catch(std::exception &e)
    {
        std::cout << "  could not process file" << std::endl;
        return -1;
    }
    return 0;
}

long File::getFileSize(const std::string &fPath_t) {
    struct stat st;
    if(stat(fPath_t.c_str(), &st) != 0)
    {
        return 0;
    }
    return st.st_size;
}


