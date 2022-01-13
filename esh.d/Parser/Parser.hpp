#pragma once
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

namespace esh
{
    class Parser
    {
    private:
        std::vector<std::vector<std::pair<std::string, size_t>>> args;
        bool error = false;

    public:
        Parser(const std::string &line);
        const std::vector<std::vector<std::pair<std::string, size_t>>> &getArgs();
        operator bool();
    };
}