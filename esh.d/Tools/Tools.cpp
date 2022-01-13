#include "Tools.hpp"

namespace esh::Tools
{
    __attribute__((const)) std::vector<std::string> strsplit(const std::string &str, char split_on)
    {
        std::vector<std::string> ret;
        std::string next = "";
        for (auto &c : str)
        {
            if (c == split_on && next.size() > 0)
            {
                ret.push_back(next);
                next = "";
            }
            else
                next += c;
        }
        return ret;
    }

    std::string getUserName()
    {
        uid_t uid = geteuid();
        struct passwd *pw = getpwuid(uid);
        if (pw)
        {
            return std::string(pw->pw_name);
        }
        return {};
    }

    __attribute__((const)) int color_helper(std::string name)
    {
        bool light = false;
        if (name.starts_with("light "))
        {
            light = true;
            name.erase(0, 6);
        }
        int l = (light ? 60 : 0);
        if (name == "black")
            return l + 0;
        else if (name == "red")
            return l + 1;
        else if (name == "green")
            return l + 2;
        else if (name == "yellow")
            return l + 3;
        else if (name == "blue")
            return l + 4;
        else if (name == "purple")
            return l + 5;
        else if (name == "cyan")
            return l + 6;
        else if (name == "white")
            return l + 7;
        else
            return 0;
    }
    __attribute__((const)) std::string color(const char *fg, const char *bg)
    {

        if (fg || bg)
        {
            std::string ret = "\033[";
            if (fg && !bg)
            {
                ret += std::to_string(30 + color_helper(fg));
            }
            else if (!fg && bg)
            {
                ret += std::to_string(40 + color_helper(bg));
            }
            else
            {
                ret += std::to_string(30 + color_helper(fg)) + ";" + std::to_string(40 + color_helper(bg));
            }
            return ret + 'm';
        }
        else
            return "\033[0m";
    }
}