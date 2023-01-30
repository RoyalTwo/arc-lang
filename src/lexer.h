#include <iostream>
#include <fstream>
#pragma once

std::ofstream createFile(std::string file_name);
std::ifstream openFile(std::string file_name);
std::string tokenizeLine(std::string line);
std::vector<std::string> separateParentheses(std::string word);