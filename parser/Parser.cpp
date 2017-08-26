//
// George Onoufriou : 15/02/2017.
//

#include "Parser.h"

/// sole handler function to limit interface with processing preventing fk ups
void Parser::parseValues(std::string &nonParsedData) {

    data = std::vector<char>();
    // split std::string into its components

    std::istringstream lineString(nonParsedData);
    std::string line;
    std::string word;

    while (getline(lineString, line)) {
        std::cout << line << std::endl;
        std::istringstream wordString(line);
        getline(wordString, word, ' ');
        if(word == "G1")
        {
            // counters, so later interleaving is possible of components
            int xSteps = 0;
            int ySteps = 0;
            float ratio = 0;
            // do calculation on every relevant subsequent component
            while( getline(wordString, word, ' ') )
            {
                // word = X222.333, word[0] = X||Y||Z, component
                char mtr = word[0];
                if(mtr == 'X' || mtr == 'Y')
                {
                    std::string component = word;
                    tail(component);
                    if(mtr == 'X')
                    {
                        xSteps = calculateStepsXY(component, mtr);
                        std::cout << "   " << mtr << ": " << xSteps << std::endl;
                        if(ySteps != 0) {ratio = interweaveRatio(ySteps, xSteps);}
                    }
                    else
                    {
                        ySteps = calculateStepsXY(component, mtr);
                        std::cout << "   " << mtr << ": " << ySteps << std::endl;
                        if(xSteps != 0) {ratio = interweaveRatio(ySteps, xSteps);}
                    }
                }
                else if (mtr == 'E')
                {
                    // this is a difficult one since laser is either on or off
                    laserModel(word, mtr);
                }
                else if(mtr == 'Z') // Extruder or in this case laser
                {
                    // calculates steps with no distortion + post movement directly
                    heightModel(word, mtr);
                }

            }
            // after all words completed, if ratio is != 0, start posting interleaved XY movements
            if(ratio != 0) {postInterweave(ySteps, xSteps, ratio);}
            else if(xSteps)
            {
                if(xSteps > 0)
                {   // inserting data using a block of data rather than push back
                    std::vector<char> tempY( abs(xSteps), 'x');
                    data.insert(data.end(), tempY.begin(), tempY.end());
                }
                else{
                    std::vector<char> tempY( abs(xSteps), 'a');
                    data.insert(data.end(), tempY.begin(), tempY.end());
                }
            }
            else if(ySteps)
            {
                if(ySteps > 0) /// THIS LITTLE MO FO
                {   // inserting data using a block of data rather than push back
                    std::vector<char> tempY( abs(ySteps), 'y');
                    data.insert(data.end(), tempY.begin(), tempY.end());
                }
                else{
                    std::vector<char> tempY( abs(ySteps), 'b');
                    data.insert(data.end(), tempY.begin(), tempY.end());
                }
            }
            // else post any spare axis
        }
    }
    std::cout << "Total instruction count: " << data.size() << std::endl;
}

/// X Y calculations
int Parser::calculateStepsXY(std::string &component, char &mtr) {
    float difference; // assigned depending on mtr in question
    float standardLength = 0.5;
    float distortion = 1;
    float nextPos = std::stof(component.c_str());

    // find difference to next position
    if(mtr == 'X') {difference = nextPos - x;}
    else/*mtr = Y*/{difference = nextPos - y;}

    // data loss is intentional,unnecessary accuracy
    float remainder = fmod(difference , (distortion * standardLength));

    // set x / y position to the maximum movable amount within the boundary
    if(mtr == 'X') {x = nextPos - remainder;}
    else/*mtr = Y*/{y = nextPos - remainder;}

    // data loss is intentional,unnecessary accuracy
    return (int)(difference/(distortion * standardLength)); // return DIV of two floats as int by truncation
}

/// Z axis calculations
int Parser::calculateStepsZ(std::string &component)
{
    float nextPos = std::stof(component.c_str());
    float difference = nextPos - z; // this is previous/ current Z not next Z from below
    float standardLength = 0.02;
    float distortion = 1; // Z axis has no distortion as its pureley linear
    // data loss is intentional,unnecessary accuracy
    float remainder = fmod(difference , (distortion * standardLength)); // slight data loss
    z = nextPos - remainder; // rounding could be added if remainder > threshold change nextPos
    // data loss is intentional,unnecessary accuracy
    return (int)(difference/(distortion * standardLength)); // return DIV of two floats as int by truncation
}

/// models extruder usage into laser actions i.e on/off instead of rate if >0/<=0 respectiveley
void Parser::laserModel(std::string &word, char &mtr)
{
    std::string component = word;
    tail(component);
    float compF = std::stof(component.c_str()); // compF (component as float)
    if(compF > LASER_ACTIVATION_THRESHOLD && lsr == 0) // must check previous state to prevent duplicate on commands
    {
        lsr = 1;
        data.push_back('w');
        std::cout << "   " << mtr << ": " << "on" << std::endl;
    }
    else if(compF <= LASER_ACTIVATION_THRESHOLD && lsr == 1) // check previous state to prevent duplicate off commands
    {
        lsr = 0;
        data.push_back('v');
        std::cout << "   " << mtr << ": " << "off" << std::endl;
    }
}

/// allows for quick matrix operation removeHead
void Parser::tail(std::string &headTail)
{
    std::reverse(headTail.begin(), headTail.end());
    headTail.pop_back();
    std::reverse(headTail.begin(), headTail.end());
}

/// parses Z component into actionable movements + pushes to data
void Parser::heightModel(std::string &word, char &mtr)
{
    std::string component = word;
    tail(component);
    int steps = 0;
    steps = calculateStepsZ(component);
    std::cout << "   " << mtr << ": " << steps << std::endl;
    if(steps > 0)
    {
        for(int i = 0; i < steps; ++i)
        {
            //std::cout << "      u - " << i << std::endl;
            data.push_back('u');
        }
    }
    else if(steps < 0)
    {
        steps *= -1;
        for(int i = 0; i < steps; ++i)
        {
            //std::cout << "      d - " << i << std::endl;
            data.push_back('d');
        }
    }
}

/// dy/dx - rate of change calculator
float Parser::interweaveRatio(int &ySteps, int &xSteps) {
    return (float)ySteps / (float)xSteps;
}

/// ratio based iterative data weaver
void Parser::postInterweave(int &ySteps, int &xSteps, float &ratio) {

//    if(ySteps > 0)
//    {
//        for(int i = 0; i < ySteps; ++i)
//        {
//            data.push_back('y');
//        }
//    }
//    else if(ySteps < 0)
//    {
//        for(int i = 0; i > ySteps; --i)
//        {
//            data.push_back('b');
//        }
//    }
//
//    if(xSteps > 0)
//    {
//        for(int i = 0; i < ySteps; ++i)
//        {
//            data.push_back('x');
//        }
//    }
//    else if(xSteps < 0)
//    {
//        for(int i = 0; i > ySteps; --i)
//        {
//            data.push_back('a');
//        }
//    }

    std::cout << "   R: " << ratio << std::endl;
    float sum = 0;
    for(int i = 0; i < abs(xSteps); ++i)
    {
        //data.push_back('c');
        if(xSteps > 0){data.push_back('x');}
        else          {data.push_back('a');}
        sum += fabs(ratio);
        if(sum >= 1)
        {
            if( ((int)sum) > 1 )
            {
                if(ySteps > 0)
                {   std::vector<char> tempY( ((int)sum), 'y');
                    data.insert(data.end(), tempY.begin(), tempY.end());
                }
                else
                {   std::vector<char> tempY( ((int)sum), 'b');
                    data.insert(data.end(), tempY.begin(), tempY.end());
                }
            }
            else{
                // if positive
                if(ySteps > 0) {data.push_back('y');}
                else           {data.push_back('b');}
            }
            sum -= (int)sum; // a clever trick to subtract all now pushed changes to data by truncation
        }
    }
    // this checks for rounding errors, and corrects data set accordingly
    if(sum != 0) // so if not exactly nice round 0
    {
        if(roundf(sum) == 1) // check if round of this error is significant
        {   // if significant add it to list according to its direction
            if(ySteps > 0){data.push_back('y');} // clockwise  Y
            else          {data.push_back('b');} // anti-clock Y
        }
    }
    std::cout << "sum: " << sum << std::endl;
    std::cout << "sumRound: " << roundf(sum) << std::endl;
}

/// passing movement data out
std::vector<char>* Parser::getAddress() {
    return &data;
}

void Parser::compressData() {
    bool previousCharUsed = 1; // cannot use first char
    for(int i = 0; i < data.size(); ++i)
    {
        //std::cout << i << std::endl;
        if(i+1 == data.size() && previousCharUsed) // if at end and no spare chars
        {
            compress(data[i]);
            previousCharUsed = 1;
        }
        else if(!previousCharUsed) // if previous char is NOT used
        {
            compress(data[i-1], data[i]);
            previousCharUsed = 1;
        }
        else
        {
            // if previous char HAS been used and is NOT at END, skip and change flag
            previousCharUsed = 0; // this should happen almost every other value
        }
    }
    std::cout << "***********************************iz ere" << std::endl;
    data = dataCompressed;
}

void Parser::compress(const char &first) {
    switch(first)
    {   // converts intermediate SINGLE commands to actual values
        case 'x': dataCompressed.push_back('o');
            break;
        case 'y': dataCompressed.push_back('q');
            break;
        case 'a': dataCompressed.push_back('p');
            break;
        case 'b': dataCompressed.push_back('r');
            break;
        case 'u': dataCompressed.push_back('t');
            break;
        case 'd': dataCompressed.push_back('s');
            break;
        case 'w': dataCompressed.push_back('w');
            break;
        case 'v': dataCompressed.push_back('v');
            break;
        default:
        std::cout << "could not compress: " << first << std::endl;
    }
}

void Parser::compress(const char &first,const char &second) {
    switch(first)
    {   // converts intermediate pairs of commands commands to actual values
        case 'x': switch(second)
            {
                case 'x': dataCompressed.push_back('x');
                    break;
                case 'y': dataCompressed.push_back('h');
                    break;
                case 'a': dataCompressed.push_back('c');
                    break;
                case 'b': dataCompressed.push_back('i');
                    break;
                case 'u':
                case 'd':
                case 'w':
                case 'v': compress(first); compress(second);
                    break;
                default:
                    std::cout << "could not compress: " << first << " WITH " << second << std::endl;
            }
            break;
        case 'y': switch(second)
            {
                case 'x': dataCompressed.push_back('k');
                    break;
                case 'y': dataCompressed.push_back('y');
                    break;
                case 'a': dataCompressed.push_back('l');
                    break;
                case 'b': dataCompressed.push_back('f');
                    break;
                case 'u':
                case 'd':
                case 'w':
                case 'v': compress(first); compress(second);
                    break;
                default:
                    std::cout << "could not compress: " << first << " WITH " << second << std::endl;
            }
            break;
        case 'a': switch(second)
            {
                case 'x': dataCompressed.push_back('e');
                    break;
                case 'y': dataCompressed.push_back('j');
                    break;
                case 'a': dataCompressed.push_back('a');
                    break;
                case 'b': dataCompressed.push_back('z'); // this one is missing
                    break;
                case 'u':
                case 'd':
                case 'w':
                case 'v': compress(first); compress(second);
                    break;
                default:
                    std::cout << "could not compress: " << first << " WITH " << second << std::endl;
            }
            break;
        case 'b': switch(second)
            {
                case 'x': dataCompressed.push_back('m');
                    break;
                case 'y': dataCompressed.push_back('g');
                    break;
                case 'a': dataCompressed.push_back('n');
                    break;
                case 'b': dataCompressed.push_back('b');
                    break;
                case 'u':
                case 'd':
                case 'w':
                case 'v': compress(first); compress(second);
                    break;
                default:
                    std::cout << "could not compress: " << first << " WITH " << second << std::endl;
            }
            break;
        case 'u': switch(second)
            {
                case 'x':
                case 'y':
                case 'a':
                case 'b': compress(first); compress(second);
                    break;
                case 'u': dataCompressed.push_back('u');
                    break;
                case 'd':
                case 'w':
                case 'v': compress(first); compress(second);
                    break;
                default:
                    std::cout << "could not compress: " << first << " WITH " << second << std::endl;
            }
            break;
        case 'd': switch(second)
            {
                case 'x':
                case 'y':
                case 'a':
                case 'b':
                case 'u': compress(first); compress(second);
                    break;
                case 'd': dataCompressed.push_back('d');
                    break;
                case 'w':
                case 'v': compress(first); compress(second);
                    break;
                default:
                    std::cout << "could not compress: " << first << " WITH " << second << std::endl;
            }
            break;
        case 'w': switch(second)
            {
                case 'x':
                case 'y':
                case 'a':
                case 'b':
                case 'u':
                case 'd':
                case 'w':
                case 'v': compress(first); compress(second);
                    break;
                default:
                    std::cout << "could not compress: " << first << " WITH " << second << std::endl;
            }
            break;
        case 'v': switch(second)
            {
                case 'x':
                case 'y':
                case 'a':
                case 'b':
                case 'u':
                case 'd':
                case 'w':
                case 'v': compress(first); compress(second);
                    break;
                default:
                    std::cout << "could not compress: " << first << " WITH " << second << std::endl;
            }
            break;
        default:
            std::cout << "could not compress: " << first << " not in table."<< std::endl;
    }
}

/// Encoding
// 1  //data.push_back("zzzz"); lsr off halt
// 2  //data.push_back("uuuu"); 8 up commands
// 3  //data.push_back("dddd"); 8 down commands
// 4  //data.push_back("xxxx"); 8 x clockwise
// 5  //data.push_back("aaaa"); 8 x anti-clockwise
// 6  //data.push_back("yyyy"); 8 y clockwise
// 7  //data.push_back("bbbb"); 8 y anti-clockwise
// 8  //data.push_back("cccc"); 4 x clock x antic
// 9  //data.push_back("eeee"); 4 x antic x clock
// 10 //data.push_back("ffff"); 4 y clock y antic
// 11 //data.push_back("gggg"); 4 y antic y clock
// 12 //data.push_back("hhhh"); 4 x clock y clock
// 13 //data.push_back("iiii"); 4 x clock y antic
// 14 //data.push_back("jjjj"); 4 x antic y clock
// 15 //data.push_back("kkkk"); 4 y clock x clock
// 16 //data.push_back("llll"); 4 y clock x antic
// 17 //data.push_back("mmmm"); 4 y antic x clock
// 18 //data.push_back("nnnn"); 4 y antic x antic
// 19 //data.push_back("oooo"); 4 x clock   empty
// 20 //data.push_back("pppp"); 4 x antic   empty
// 21 //data.push_back("qqqq"); 4 y clock   empty
// 22 //data.push_back("rrrr"); 4 y antic   empty
// 23 //data.push_back("ssss"); 4   down    empty
// 24 //data.push_back("tttt"); 4   up      empty
// 25 //data.push_back("wwww"); 4   lasr on
// 26 //data.push_back("vvvv"); 4   lsr off