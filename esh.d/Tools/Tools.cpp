#include "Tools.hpp"

namespace esh::Tools
{
    std::vector<std::string> strsplit(const std::string& str, char split_on)
    {
        std::vector<std::string> ret;
        std::string next = "";
        for(auto& c : str)
        {
            if(c==split_on && next.size()>0)
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

    int color_helper(const std::string& name)
    {
        if (name == "black")
            return 0;
        else if (name == "red")
            return 1;
        else if (name == "green")
            return 2;
        else if (name == "yellow")
            return 3;
        else if (name == "blue")
            return 4;
        else if (name == "purple")
            return 5;
        else if (name == "cyan")
            return 6;
        else if (name == "white")
            return 7;
        else return 0;
    }
    std::string color(const char *fg, const char* bg)
    {
        
        if(fg || bg)
        {
            std::string ret = "\033[";
            if(fg && !bg)
            {
                ret += std::to_string(30+color_helper(fg));
            }
            else if( !fg && bg)
            {
                ret += std::to_string(40+color_helper(bg));
            }
            else
            {
                ret += std::to_string(30+color_helper(fg)) + ";" + std::to_string(40+color_helper(bg));
            }
            return ret + 'm';
        }
        else return "\033[0m";
    }
}