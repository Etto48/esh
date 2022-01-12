#include "Runner.hpp"

namespace esh
{
    Runner::Runner(const std::vector<std::string>& args)
    {   
        std::vector<char*> cargs;
        cargs.reserve(args.size()+1);

        for(size_t i = 0; i < args.size(); ++i)
            cargs.push_back(const_cast<char*>(args[i].c_str()));
        cargs.push_back(nullptr);

        pid = fork();
        if(!pid)
        {
            execvp(cargs[0],&cargs[0]);
            std::cout << "esh: ";
            switch (errno)
            {
            case EACCES:
                std::cout << "permission denied";
                break;
            case ENOENT:
                std::cout << "command not found";
                break;
            default:
                std::cout << "an error occurred";
                break;
            }
            std::cout << ": " << args[0] << std::endl;
            exit(-1);
            
        }
    }

    Runner& Runner::wait()
    {
        if(pid>0)
        {
            int full_status;
            if(waitpid(pid,&full_status,0)!=pid)
                status = -1;
            else
                status = WIFEXITED(full_status)? WEXITSTATUS(full_status) : -1;
            pid = 0;
        }
        return *this;
    }

    Runner::operator bool()
    {
        return status == 0;
    }

    Runner::operator int8_t()
    {
        return status;
    }
}