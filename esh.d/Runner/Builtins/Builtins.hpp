#pragma once
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

namespace esh::Builtins
{
    extern std::vector<std::pair<std::string,std::function<int8_t(const std::vector<std::string>&)>>> functions;
    int8_t exit(const std::vector<std::string>& args);
    int8_t cd(const std::vector<std::string>& args);
}