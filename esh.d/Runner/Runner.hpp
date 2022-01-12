#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include "../Tools/Tools.hpp"
#include "Builtins/Builtins.hpp"


namespace esh
{
    bool wouldRun(std::string command);
    class Runner
    {
    private:
        int8_t status = 0;
        pid_t pid = 0;
    public:
        Runner(const std::vector<std::string>& args);
        Runner& wait();
        operator bool();
        operator int8_t();
    };
}