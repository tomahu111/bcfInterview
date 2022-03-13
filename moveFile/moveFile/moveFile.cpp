//Program written for windows, c++ 17 standard
//todo: check whehter file with given name exists in a directory
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstring>

//structure for storing paths information about moving process
//fileArgumentID - short int, stores number of file argument, -1 by default
//destinationArgumentID - short int, stores number of destination argument, -1 by default
//hash - bool, stores whether to use hashing to check if moving process was successfull or not, true by default
struct MovingParams
{
    short fileArgumentID=-1;
    short destinationArgumentID=-1;
    bool hash = true;
};

//prints programs help
void help();

//-1 - wrong arguments, prints advice to use help
// 0 - succesfully passed arguments for execution, 
//1 - help called
int arguments(MovingParams& movingParameters, int argc, char* argv[]);

//returns files hash, needs file path, uses constants from djb and murmurhash2
unsigned long HashFile(char file[]);

//moves the file, needs path and destination
void move(char file[], char destination[], bool hash=true);



int main(int argc, char* argv[]) {
    MovingParams movingParameters;
    
    int arg_result = arguments(movingParameters, argc, argv);
    if (arg_result == -1 || movingParameters.fileArgumentID == -1 || movingParameters.destinationArgumentID == -1)
    {
        std::cout << "Wrong arguments given, try using --help" << std::endl;
    }
    else if (arg_result == 0)
    {
        move(argv[movingParameters.fileArgumentID], argv[movingParameters.destinationArgumentID], movingParameters.hash);
    }

    return 0;
}



void help()
{
    std::cout << "moveFile is a program for file transfering" << std::endl <<
        "Options:" << std::endl <<
        "--help        Print this help" << std::endl <<
        "-file     file path and name, first argument by default" << std::endl <<
        "-destination     file destination, second argument by default, must contain file name at the end" << std::endl <<
        "-Hash    checks files hash to verify movement process, prints error if some issue occured, true by default, true or false, will show hash at the end of program"
        "Example arguments for program: " << " file.txt C:\\Users\\Admin\\Documents -Hash false" << std::endl <<
        "would move file.txt to C:\\Users\\Admin\\Documents without checking files hash" << std::endl;
}


int arguments(MovingParams& movingParameters, int argc, char* argv[])
{
    if (argc == 1) return -1;
    for (int arg = 1; arg < argc; arg++)
    {
        if (strcmp(argv[arg], "--help") == 0) 
        {
            help();
            return 1;
        }
        else if (strcmp(argv[arg], "-file") == 0)
        {
            arg++;
            if (movingParameters.fileArgumentID == -1 && arg<argc)
            {
                movingParameters.fileArgumentID = arg;
            }
            else
            {
                return -1;
            }
        }
        else if (strcmp(argv[arg], "-destination") == 0)
        {
            arg++;

            if (movingParameters.destinationArgumentID == -1 && arg < argc)
            {
                movingParameters.destinationArgumentID = arg;
            }
            else
            {
                return -1;
            }
        }
        else if (strcmp(argv[arg], "-Hash") == 0)
        {
            arg++;
            if (arg < argc)
            {
                if (strcmp(argv[arg], "true") == 0)
                {
                    movingParameters.hash = true;
                }
                else if (strcmp(argv[arg], "false") == 0)
                {
                    movingParameters.hash = false;
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                return -1;
            }
        }
        else if (movingParameters.fileArgumentID == -1)
        {
            movingParameters.fileArgumentID = arg;
        }
        else if (movingParameters.destinationArgumentID == -1)
        {
            movingParameters.destinationArgumentID = arg;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

unsigned long HashFile(char file[])
{
    unsigned char buffer;
    unsigned long hashResult = 5381;
    const unsigned long m = 0x5bd1e995;


    std::ifstream ReadFile;
    ReadFile.open(file);
    if (!ReadFile) {
        std::cout << "file couldnt be opened" << std::endl;
        return 0;
    }
    ReadFile >> buffer;
    if (!ReadFile.eof())
    {
        hashResult = ((hashResult << 33) + buffer) ^ m;
        ReadFile >> buffer;
    }
    
    while (!ReadFile.eof())
    {

        hashResult = ((hashResult << 33) + buffer) ^ m;
        ReadFile >> buffer;
    }
    ReadFile.close();
    return hashResult;
}

void move(char file[], char destination[], bool hash)
{
    try {
        if (hash == true)
        {
            unsigned long originalHash = HashFile(file);
            std::cout << "original files hash is: " << originalHash << std::endl;
        }
        std::filesystem::rename(file, destination);
        if (hash == true)
        {
            unsigned long movedHash = HashFile(destination);
            std::cout << "moved files hash is: " << movedHash << std::endl;
        }
    }
    catch (std::filesystem::filesystem_error& e) {
        std::cout << e.what() << '\n';
        return;
    }
}