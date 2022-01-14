#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include "../Tools/Tools.hpp"
#include "Builtins/Builtins.hpp"

namespace esh
{
    __attribute__((const)) bool wouldRun(std::string command, bool skip_exec_check = true);
    class Runner
    {
    private:
        int8_t status = 0;
        pid_t pid = 0;
        std::string output;
        int pipes[2] = {0,0};
    public:
        Runner(std::vector<std::pair<std::string, size_t>> args, bool pipe = false);
        Runner &wait();
        const std::string& getOutput();
        operator bool();
        operator int8_t();
    };
}