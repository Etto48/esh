#pragma once
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "../../Tools/Tools.hpp"

namespace esh::Builtins
{
    extern std::vector<std::pair<std::string,std::function<int8_t(const std::vector<std::pair<std::string,size_t>>&)>>> functions;
    int8_t exit(const std::vector<std::pair<std::string,size_t>>& args);
    int8_t cd(const std::vector<std::pair<std::string,size_t>>& args);
    int8_t reload(const std::vector<std::pair<std::string,size_t>>& args);
    int8_t where(const std::vector<std::pair<std::string,size_t>>& args);
    int8_t builtin(const std::vector<std::pair<std::string,size_t>>& args);
}