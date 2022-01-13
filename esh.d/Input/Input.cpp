#include "Input.hpp"
#define ARROW_U 'A'
#define ARROW_D 'B'
#define ARROW_R 'C'
#define ARROW_L 'D'
#define HOME 'H'
#define END 'F'
#define BKSP 127
#define DEL 126
#define ESC 27
#define CTRL_C 3

namespace esh
{
    Input input;

    struct termios orig_termios;
    void enableRawMode()
    {
        struct termios raw;
        tcgetattr(STDIN_FILENO, &raw);
        orig_termios = raw;
        raw.c_lflag &= ~(ECHO | ICANON | ISIG | IXON | IEXTEN | ICRNL);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    void disableRawMode()
    {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }

    Input::Input()
    {
        enableRawMode();
        reset();
        std::ifstream reader{std::string(getenv("HOME")) + "/.esh/.history"};
        std::string line;
        while (std::getline(reader, line))
        {
            history.push_back(line);
        }
    }
    Input::~Input()
    {
        disableRawMode();
        save();
    }
    void Input::reset()
    {
        if (buf.size())
        {
            if (history_iterator == history.end() && history.back() != buf)
                history.push_back(buf);
            else if (history_iterator != history.end() && history.back() != buf)
            {
                history.erase(history_iterator);
                history.push_back(buf);
            }
            while (history.size() > HISTORY_MAX)
            {
                history.pop_front();
            }
        }
        history_iterator = history.end();
        buf = "";
        cursor = 0;
    }
    void Input::save()
    {
        std::ofstream writer{std::string(getenv("HOME")) + "/.esh/.history"};
        for (auto &h : history)
        {
            writer << h << std::endl;
        }
    }
    bool Input::next()
    {
        char c;
        // static int dbg = 0;
        if (read(STDIN_FILENO, &c, 1) == 1)
        {
            // std::cout << "\033[1;" << dbg << "H" << '(' << cursor << ':' << int(c) << ')';
            // dbg+= 8;
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
                    history_iterator = history.end();
                }
                break;
            case DEL:
                if (buf.size() && cursor < buf.size())
                {
                    buf.erase(cursor, 1);
                    history_iterator = history.end();
                }
                break;
            case char(0x80 | ARROW_L):
                break;
            case ESC:
            {
                char esc_type;
                if (read(STDIN_FILENO, &esc_type, 1) == 1)
                {
                    // std::cout << "\033[1;" << dbg << "H" << '(' << cursor << ':' << int(esc_type) << ')';
                    // dbg+= 8;
                    switch (esc_type)
                    {
                    case '[':
                    {
                        char cs;
                        if (read(STDIN_FILENO, &cs, 1) == 1)
                        {
                            // std::cout << "\033[1;" << dbg << "H" << '(' << cursor << ':' << int(cs) << ')';
                            // dbg+= 8;
                            switch (cs)
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
                                if (history_iterator != history.begin())
                                {
                                    auto old_iterator = history_iterator--;
                                    if (old_iterator == history.end() && buf.size())
                                        history.push_back(buf);
                                    buf = *history_iterator;
                                    cursor = buf.size();
                                }
                                break;
                            case ARROW_D:
                                if (history_iterator != history.end())
                                {
                                    history_iterator++;
                                    if (history_iterator != history.end())
                                    {
                                        buf = *history_iterator;
                                        cursor = buf.size();
                                    }
                                    else
                                        history_iterator--;
                                }
                                break;
                            case END:
                                cursor = buf.size();
                                break;
                            case HOME:
                                cursor = 0;
                                break;
                            }
                        }
                    }
                    break;
                    }
                }
            }
            break;
            case CTRL_C:
                history_iterator = history.end();
                buf = "";
                cursor = 0;
                break;
            default:
                buf.insert(buf.begin() + cursor, c);
                history_iterator = history.end();
                cursor++;
                break;
            }
            return false;
        }
        else
            return true;
    }
    std::string Input::getBuf()
    {
        return buf;
    }
    size_t Input::getCursor()
    {
        return cursor;
    }

    std::ostream &operator<<(std::ostream &os, const Input &in)
    {
        os << in.buf;
        return os;
    }
}