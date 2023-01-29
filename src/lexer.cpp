#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexer.h"

int main(int argc, char *argv[])
{
    std::string input_file_name;
    std::string input_file_text;
    std::vector<std::string> input_file_lines;

    if (argc > 1)
    {
        input_file_name = argv[1];
    }

    std::ifstream input_file = openFile(input_file_name);

    if (!input_file.is_open())
    {
        return 1;
    }

    while (getline(input_file, input_file_text))
    {
        input_file_lines.push_back(input_file_text);
    }
}

std::ofstream createFile(std::string file_name)
{
    std::ofstream file(file_name);
    return file;
}

std::ifstream openFile(std::string file_name)
{
    std::ifstream file(file_name);
    return file;
}