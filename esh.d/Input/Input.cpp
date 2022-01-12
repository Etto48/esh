#include "Input.hpp"
#define ARROW_U 'A'
#define ARROW_D 'B'
#define ARROW_R 'C'
#define ARROW_L 'D'
#define BKSP 127
#define DEL 126
#define ESC 27

namespace esh
{
    struct termios orig_termios;
    void enableRawMode()
    {
        struct termios raw;
        tcgetattr(STDIN_FILENO, &raw);
        orig_termios = raw;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    void disableRawMode()
    {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }

    Input::Input()
    {
        enableRawMode();
    }
    Input::~Input()
    {
        disableRawMode();
    }
    void Input::reset()
    {
        buf = "";
        cursor = 0;
    }
    bool Input::next()
    {
        char c;
        //static int dbg = 0;
        if (read(STDIN_FILENO, &c, 1) == 1)
        {
            //std::cout << "\033[1;" << dbg << "H" << '(' << cursor << ':' << int(c) << ')';
            //dbg+= 10;
            switch (c)
            {
            case '\n':
            case '\0':
                return true;
                break;
            case BKSP:
                if (buf.size() && cursor > 0)
                {
                    cursor--;
                    buf.erase(cursor, 1);
                }
                break;
            case DEL:
                if (buf.size() && cursor < buf.size())
                {
                    buf.erase(cursor, 1);
                }
                break;
            case char(0x80 | ARROW_L):
                break;
            case ESC:
            {
                char esc_type;
                if (read(STDIN_FILENO, &esc_type, 1) == 1 && esc_type == '[')
                {
                    char ch[10];
                    if (read(STDIN_FILENO, ch + 0, 1) == 1)
                    {
                        switch (ch[0])
                        {
                        case ARROW_L:
                            if (cursor > 0)
                                cursor--;
                            break;
                        case ARROW_R:
                            if (cursor < buf.size())
                                cursor++;
                            break;
                        case ARROW_U:
                            break;
                        case ARROW_D:
                            break;
                        }
                    }
                }
            }
            break;
            default:
                buf.insert(buf.begin() + cursor, c);
                cursor++;
                break;
            }
            return false;
        }
        else
            return true;
    }
    Input::operator std::string()
    {
        return buf;
    }
    const std::string &Input::getBuf()
    {
        return buf;
    }
    const size_t &Input::getCursor()
    {
        return cursor;
    }

    std::ostream &operator<<(std::ostream &os, const Input &in)
    {
        os << in.buf;
        return os;
    }
}