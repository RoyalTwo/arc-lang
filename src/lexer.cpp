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
    std::cout << tokenizeLine(input_file_lines[0]) << tokenizeLine(input_file_lines[1]);
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

std::string tokenizeLine(std::string line)
{
    std::vector<std::string> words;
    std::vector<std::string> tokens;
    std::string token_line;

    const std::vector<std::string> TYPES{"int", "string"};
    const std::vector<std::string> OPERATORS{"+", "-", "=", "*", "/"};
    const std::vector<std::string> KEY_WORDS{"let"};
    const std::string NEW_LINE = "[newline]";

    // takes all but 1 word out
    while (line.find(" ") != std::string::npos)
    {
        std::string word;
        word = line.substr(0, line.find(" "));
        words.push_back(word);
        line.erase(0, line.find(" ") + 1);
    }

    // takes last out
    if (line.length() != 0)
    {
        std::string word = line.substr(0, line.length());
        words.push_back(word);
    }

    // for each word,
    // compare against types, operators, if it's a number
    // otherwise it's an id
    for (auto i = words.begin(); i != words.end(); i++)
    {
        std::string token;

        bool found = false;

        if (std::find(TYPES.begin(), TYPES.end(), *i) != TYPES.end())
        {
            found = true;
            token = "[type: ";
        }
        if (std::find(OPERATORS.begin(), OPERATORS.end(), *i) != OPERATORS.end())
        {
            found = true;
            token = "[op: ";
        }
        if (std::find(KEY_WORDS.begin(), KEY_WORDS.end(), *i) != KEY_WORDS.end())
        {
            found = true;
            token = "[keyword: ";
        }

        // checks if number
        char *p;
        std::string newstr = *i;
        long converted = strtol(newstr.c_str(), &p, 10);
        if (!(*p))
        {
            // is number, has no error
            found = true;
            token = "[num: ";
        }

        else if (!found)
        {
            token = "[id: ";
        }

        std::string finalstr = "'";
        finalstr.append(*i);
        finalstr.append("']");
        token.append(finalstr);

        token_line.append(token);
        token_line.append("\n");
    }
    token_line.append("[newline]\n");

    return token_line;
}