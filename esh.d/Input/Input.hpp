#pragma once
#include <string>
#include <iostream>
#include <unistd.h>
#include <termios.h>

namespace esh
{
    class Input
    {
    private:
        std::string buf;
        size_t cursor = 0;
    public:
        Input();
        ~Input();
        void reset();
        bool next();
        operator std::string();
        const std::string& getBuf();
        const size_t& getCursor();

        friend std::ostream& operator <<(std::ostream& os, const Input& in);
    };
}

