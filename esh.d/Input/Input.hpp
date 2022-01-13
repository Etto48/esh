#pragma once
#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <unistd.h>
#include <termios.h>

#define HISTORY_MAX 100
namespace esh
{
    class Input
    {
    private:
        std::list<std::string> history;
        std::list<std::string>::iterator history_iterator;
        std::string buf;
        size_t cursor = 0;

    public:
        Input();
        ~Input();
        void reset();
        void save();
        bool next();
        std::string getBuf();
        size_t getCursor();

        friend std::ostream &operator<<(std::ostream &os, const Input &in);
    };

    extern Input input;
}
