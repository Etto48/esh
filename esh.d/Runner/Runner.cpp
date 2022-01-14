#include "Runner.hpp"

namespace esh
{
    bool isExec(struct stat st)
    {
        return ((st.st_mode & S_IXOTH) || ((st.st_mode & S_IXUSR) && st.st_uid == getuid()) || ((st.st_mode & S_IXGRP) && st.st_gid == getgid()));
    }
    __attribute__((const)) bool wouldRun(std::string command, bool skip_exec_check)
    {
        struct stat st;
        if (command.find('/') != std::string::npos)
        {
            return stat(command.c_str(), &st) >= 0 && !(st.st_mode & S_IFDIR) && (skip_exec_check || isExec(st));
        }
        for (auto &b : Builtins::functions)
        {
            if (command == b.first)
                return true;
        }
        auto path = Tools::strsplit(getenv("PATH"), ':');
        for (auto &p : path)
        {
            auto test = p + '/' + command;
            if (stat(test.c_str(), &st) >= 0 && !(st.st_mode & S_IFDIR) && (skip_exec_check || isExec(st)))
                return true;
        }
        return false;
    }

    Runner::Runner(std::vector<std::pair<std::string, size_t>> args, bool pipe)
    {
        for (auto &b : Builtins::functions)
        {
            if (args[0].first == b.first)
            {
                status = b.second(args);
                return;
            }
        }
        std::vector<char *> cargs;
        cargs.reserve(args.size() + 1);
        for (size_t i = 0; i < args.size(); ++i)
            cargs.push_back(const_cast<char *>(args[i].first.c_str()));
        cargs.push_back(nullptr);

        if(pipe)
        {
            if(::pipe(pipes) < 0)
            {
                perror("pipe");
                return;
            }
        }

        pid = fork();
        if(pid<0)
        {
            perror("fork");
            return;
        }
        if (!pid)
        {
            if(pipe)
            {
                close(pipes[0]);//child does not read
                dup2(pipes[1],1);//set new stdout
            }
            execvp(cargs[0], &cargs[0]);
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
            std::cout << ": " << args[0].first << std::endl;
            exit(-1);
        }
        else if(pipe)
            close(pipes[1]); // parent does not write
    }

    const std::string& Runner::getOutput()
    {
        return output;
    }

    Runner &Runner::wait()
    {
        if (pid > 0)
        {
            if(pipes[0]) // we have to save the output
            {
                char reading;
                while(read(pipes[0], &reading, 1) > 0)
                {
                    output += reading;
                }
                close(pipes[0]);
            }
            int full_status;
            if (waitpid(pid, &full_status, 0) != pid)
                status = -1;
            else
                status = WIFEXITED(full_status) ? WEXITSTATUS(full_status) : -1;
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