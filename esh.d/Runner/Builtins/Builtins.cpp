#include "Builtins.hpp"

namespace esh::Builtins
{
    std::vector<std::pair<std::string,std::function<int8_t(const std::vector<std::string>&)>>> functions = 
    {
        {"exit",exit},
        {"cd",cd},
    };
    int8_t exit(const std::vector<std::string>& args)
    {
        if(args.size() > 2)
            std::cout << "exit: too many arguments" << std::endl;
        else if(args.size() == 2)
            ::exit(std::atoi(args[1].c_str()));
        else
            ::exit(0);
        return -1;
    }
    int8_t cd(const std::vector<std::string>& args)
    {
        int8_t ret = 0;
        if(args.size() > 2)
        {
            std::cout << "cd: too many arguments" << std::endl;
            ret = -1;
        }
        else if(args.size() == 2)
        {
            if((ret = ::chdir(args[1].c_str())) < 0)
                perror("cd");
        }
        else
        {
            if((ret = ::chdir(getenv("HOME"))) < 0)
                perror("cd");
        }
        return ret;
    }
}