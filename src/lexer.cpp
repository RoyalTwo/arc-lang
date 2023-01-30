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
    std::vector<std::string> output_file_lines;

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

    std::string output_file_name = input_file_name.append(".lexed");

    std::ofstream output_file = createFile(output_file_name);

    for (auto i = input_file_lines.begin(); i != input_file_lines.end(); i++)
    {
        std::string line_tokens = tokenizeLine(*i);
        output_file << line_tokens;
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

std::string tokenizeLine(std::string line)
{
    // this isn't an error
    // if you run with c++20
    if (line.starts_with("//"))
    {
        return "";
    }

    std::vector<std::string> words;
    std::vector<std::string> tokens;
    std::string token_line;

    // not error
    // if you use c++20
    const std::vector<std::string> TYPES{"int", "string"};
    const std::vector<std::string> OPERATORS{"+", "-", "=", "*", "/", "(", ")", "{", "}"};
    const std::vector<std::string> KEY_WORDS{"let", "return", "fn"};
    const std::string NEW_LINE = "[newline]";

    // takes all but 1 word out
    while (line.find(" ") != std::string::npos)
    {
        std::string word;
        word = line.substr(0, line.find(" "));
        if (word.find("(") != std::string::npos || word.find(")") != std::string::npos)
        {
            std::vector<std::string> parentheses_stuff = separateParentheses(word);
            for (int i = 0; i < parentheses_stuff.size() - 1; i++)
            {
                if (parentheses_stuff[i].size() > 0)
                {
                    words.push_back(parentheses_stuff[i]);
                }
            }
            word = parentheses_stuff[2];
        }
        words.push_back(word);
        line.erase(0, line.find(" ") + 1);
    }

    // takes last out
    if (line.length() != 0)
    {
        std::string word = line.substr(0, line.length());
        if (word.find("(") != std::string::npos || word.find(")") != std::string::npos)
        {
            std::vector<std::string> parentheses_stuff = separateParentheses(word);
            for (int i = 0; i < parentheses_stuff.size() - 1; i++)
            {
                if (parentheses_stuff[i].size() > 0)
                {
                    words.push_back(parentheses_stuff[i]);
                }
            }
            word = parentheses_stuff[parentheses_stuff.size() - 1];
        }
        words.push_back(word);
    }

    // for each word,
    // compare against types, operators, if it's a number
    // otherwise it's an id
    for (auto i = words.begin(); i != words.end(); i++)
    {
        if ((*i).size() == 0)
            continue;
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

std::vector<std::string> separateParentheses(std::string word)
{
    if (word.find("(") != std::string::npos && word.find(")") != std::string::npos)
    {
        std::string before = word.substr(0, word.find("("));
        word.erase(0, before.length());
        std::string parenthese = word.substr(word.find("("), word.find("(") + 1);
        word.erase(0, parenthese.length());
        std::string between = word.substr(0, word.find(")"));
        word.erase(0, between.length());
        std::string last_parenthese = word;

        std::vector<std::string> output{before, parenthese, between, last_parenthese};
        return output;
    }
    std::string specific = word.find("(") != std::string::npos ? "(" : ")";
    std::string before = word.substr(0, word.find(specific));
    word.erase(0, before.length());
    std::string parenthese = word.substr(word.find(specific), word.find(specific) + 1);
    word.erase(0, parenthese.length());

    std::vector<std::string> output{before, parenthese, word};
    return output;
}