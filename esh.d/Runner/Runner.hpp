#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "../Tools/Tools.hpp"

namespace esh
{
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