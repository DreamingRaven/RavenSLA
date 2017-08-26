// main.cpp : George Onoufriou : pygmalion parser : 14/02/2017
#include <iostream>
#include "File.h"
//#include "Packman.h"
#include "Parser.h"
#include "Ptr.h"
#include "Slicer.h"
#include "Packman2.h"

//#define DEBUG true

void helpScreen();

int main(int argc, char* argv[]) {

    try // try block : 0,
    {
        // flag && store for position of argument
        int f = 0, p = 0, s = 0, u = 0;
        // cycle through arguments
        for(int i = 0; i < argc; ++i)
        {
            std::string arg(argv[i]);
            if(i == 0)                                  {;} // do nothing
            else if(arg == "-h" || arg == "--help"  )   {helpScreen();}
            else if(arg == "-f" || arg == "--file"  )   {if(!f){f = i; std::cout << "File Flagged"   << std::endl;}}
            else if(arg == "-p" || arg == "--parser")   {if(!p){p = i; std::cout << "Parser Flagged" << std::endl;}}
            else if(arg == "-s" || arg == "--slicer")   {if(!s){s = i; std::cout << "Slicer Flagged" << std::endl;}}
            else if(arg == "-u" || arg == "--udp"   )   {if(!u){u = i; std::cout << "UDP Flagged"    << std::endl;}}
            else                                        {std::cout << arg << " - not flag"<< std::endl;            }
        }

        if(f)
        {
            std::cout << "\nFile manipulation initialising "
                      "\n  looking for " << argv[f+1]
                      << std::endl;
            File file; // expected params "-f {filePath}"
            std::string fPath(argv[f+1]);
            file.readData(fPath);
            std::string ptr;
            file.getPtr(ptr);
            int counts[26] = {};

            // increasing scope of certain constructors which wont be used unless specified
            Parser parser;
            std::vector<char>* instructions = NULL; // data on stack so will be auto cleaned
            // increasing scope of udp, to maintain consistency with other similar classes
            Packman2 udp;

            if(s)
            {
                std::cout << "Slicer manipulation initialising" << std::endl;
                Slicer slicer; // expected params "-s"
            }
            if(p)
            {
                std::cout << "Parser manipulation initialising" << std::endl;
                parser.parseValues(ptr);
                parser.compressData();
                instructions = parser.getAddress();
                for(int i = 0; i < (*instructions).size(); ++i)
                {
                    switch( (*instructions)[i] )
                    {
                        case 'a': ++counts[0];
                            break;
                        case 'b': ++counts[1];
                            break;
                        case 'c': ++counts[2];
                            break;
                        case 'd': ++counts[3];
                            break;
                        case 'e': ++counts[4];
                            break;
                        case 'f': ++counts[5];
                            break;
                        case 'g': ++counts[6];
                            break;
                        case 'h': ++counts[7];
                            break;
                        case 'i': ++counts[8];
                            break;
                        case 'j': ++counts[9];
                            break;
                        case 'k': ++counts[10];
                            break;
                        case 'l': ++counts[11];
                            break;
                        case 'm': ++counts[12];
                            break;
                        case 'n': ++counts[13];
                            break;
                        case 'o': ++counts[14];
                            break;
                        case 'p': ++counts[15];
                            break;
                        case 'q': ++counts[16];
                            break;
                        case 'r': ++counts[17];
                            break;
                        case 's': ++counts[18];
                            break;
                        case 't': ++counts[19];
                            break;
                        case 'u': ++counts[20];
                            break;
                        case 'v': ++counts[21];
                            break;
                        case 'w': ++counts[22];
                            break;
                        case 'x': ++counts[23];
                            break;
                        case 'y': ++counts[24];
                            break;
                        case 'z': ++counts[25];
                            break;
                        default: std::cout << "unknown pathogen "
                                           << (*instructions)[i]
                                           << std::endl;
                            break;
                    }
                }
                for(int i = 0; i < 26; ++i)
                {
                    std::cout << i << " = " << counts[i] << std::endl;
                }
            }
            if(u) // faster check for if(u != 0)
            {
                std::cout << "UDP manipulation initialising" << std::endl;
                 // expected params "-u {ipAdd} {port} {pass?}"
                if(udp.initSocket() && instructions) // try init and check instructions is not null
                {
                    udp.sendData(*instructions);
                }
                else{
                    std::cout << "could not open port, ending processes" << std::endl;
                }
            }
            std::cout << "completion check" << std::endl;
        }
        else
        {
            std::cout << "--file OR -f, must always be defined" << std::endl;
        }

    }
    catch(std::exception &e)
    {
        std::cout << "Exception, try block: 0 - " << e.what() << std::endl;
    }

    return 0;
}

void helpScreen()
{
    std::cout << "George Onoufriou's \"Pygmalion; SLA 3D Printer\", Program Help Screen \n"
              << "  Program parameters: \n"
              << "    \'-h\' OR \'--help\' == This help screen. \n"
              << "    \'-f\' OR \'--file\' == this indicates that the next single word is the file path of manipulated "
                      "file. \n"
              << "    \'-p\' OR \'--parser\' == this indicates you want to parse file from \'-f\' as-is OR parse \'-s\'"
                      " output (if declared). \n"
              << "    \'-s\' OR \'--slicer\' == this indicates you want to slice the file from \'-f\' \n"
              << "    \'-u\' OR \'--udp\' == this indicates you want to send the file from \'-f\' as-is OR output from "
                      "\'-p\' (if declared). \n"
              << std::endl;
}