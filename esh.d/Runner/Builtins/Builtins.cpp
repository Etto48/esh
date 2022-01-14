#include "Builtins.hpp"

namespace esh::Builtins
{
    std::vector<std::pair<std::string,std::function<int8_t(const args_t&, std::ostream&)>>> functions = 
    {
        {"exit",exit},
        {"cd",cd},
        {"reload",reload},
        {"where",where},
        {"builtin",builtin},
    };
    int8_t exit(const args_t& args, std::ostream& os)
    {
        if(args.size() > 2)
            os << "exit: too many arguments" << std::endl;
        else if(args.size() == 2)
            ::exit(std::atoi(args[1].first.c_str()));
        else
            ::exit(0);
        return -1;
    }
    int8_t cd(const args_t& args, std::ostream& os)
    {
        int8_t ret = 0;
        if(args.size() > 2)
        {
            os << "cd: too many arguments" << std::endl;
            ret = -1;
        }
        else if(args.size() == 2)
        {
            if((ret = ::chdir(args[1].first.c_str())) < 0)
                perror("cd");
        }
        else
        {
            if((ret = ::chdir(getenv("HOME"))) < 0)
                perror("cd");
        }
        return ret;
    }
    int8_t reload(const args_t& args, std::ostream& os)
    {
        char* eargs[] = {nullptr};
        if(args.size() > 2)
            os << "reload: too many arguments" << std::endl;
        else if(args.size() == 2)
        {
            execv(args[1].first.c_str(),eargs);
            perror("reload");
        }
        else
        {
            execvp((std::string(getenv("HOME"))+"/.esh/esh").c_str(),eargs);
            perror("reload");
        }
        return -1;
    }
    int8_t where(const args_t& args, std::ostream& os)
    {
        if(args.size() > 2)
        {
            os << "where: too many arguments" << std::endl;
            return -1;
        }
        if(args.size() != 2)
        {
            return -1;
        }
        bool found = false;
        struct stat st;
        if (args[1].first.find('/') != std::string::npos)
        {
            if (stat(args[1].first.c_str(), &st) >= 0 && !(st.st_mode & S_IFDIR))
            {
                os << args[1].first << std::endl;
                found = true;
            }
        }
        for (auto &b : Builtins::functions)
        {
            if (args[1].first == b.first)
            {
                os << args[1].first << ": shell built-in command" << std::endl;
                found = true;
            }
        }
        auto path = Tools::strsplit(getenv("PATH"), ':');
        for (auto &p : path)
        {
            auto test = p + '/' + args[1].first;
            if (stat(test.c_str(), &st) >= 0 && !(st.st_mode & S_IFDIR))
            {
                os << test << std::endl;
                found = true;
            }
        }
        if(found)
            return 0;
        else
        {
            os << args[1].first << " not found" << std::endl;
            return -1;
        }
    }
    int8_t builtin(const args_t& args, std::ostream& os)
    {
        if (args.size()<2) return 0;
        for(auto& b : Builtins::functions)
        {
            if(args[1].first == b.first)
            {
                auto b_args = args;
                b_args.erase(b_args.begin());
                return b.second(b_args, os);
            }
        }
        os << "esh: no such builtin: " << args[1].first << std::endl;
        return -1;
    }
}