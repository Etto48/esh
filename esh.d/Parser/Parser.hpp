#pragma once
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

namespace esh
{
    class Parser
    {
    private:
        std::vector<std::string> args;
        bool error = false;
    public:
        Parser(const std::string& line);
        const std::vector<std::string>& getArgs();
        operator bool();
    };
}