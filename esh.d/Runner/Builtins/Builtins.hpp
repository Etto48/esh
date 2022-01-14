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
    typedef std::vector<std::pair<std::string,size_t>> args_t;
     
    extern std::vector<std::pair<std::string,std::function<int8_t(const args_t&, std::ostream&)>>> functions;
    int8_t exit(const args_t& args, std::ostream& os);
    int8_t cd(const args_t& args, std::ostream& os);
    int8_t reload(const args_t& args, std::ostream& os);
    int8_t where(const args_t& args, std::ostream& os);
    int8_t builtin(const args_t& args, std::ostream& os);
}