#pragma once
#include <vector>
#include <string>
#include <unistd.h>
#include <pwd.h>

namespace esh::Tools
{
    std::vector<std::string> strsplit(const std::string& str, char split_on);
    std::string getUserName();
    std::string color(const char *fg, const char* bg);
}