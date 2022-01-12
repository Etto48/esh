#include "main.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv, [[maybe_unused]] char **env)
{
    int8_t status = 0;
    while (true)
    {
        std::string in;
        std::cout << prompt(status);
        std::getline(std::cin, in);
        esh::Parser parser{in};

        if (parser.getArgs()[0] == "exit" && parser.getArgs().size() == 1)
            break;
        else if (parser.getArgs()[0] == "cd" && parser.getArgs().size() == 2)
        {
            if(chdir(parser.getArgs()[1].c_str())<0)
                perror("esh");
        }
        else
        {
            esh::Runner runner{parser.getArgs()};
            status = runner.wait();
        }
    }
}

std::string prompt(int8_t status)
{
    std::string ret = "";

    ret += esh::Tools::color("green", nullptr);
    ret += "";
    ret += esh::Tools::color("white", "green");

    // error
    ret += esh::Tools::color("red", "green");
    ret += status ? "x " : "";
    ret += esh::Tools::color("white", "green");

    // username
    ret += esh::Tools::getUserName();

    ret += ' ';
    ret += esh::Tools::color("green", "white");
    ret += " ";

    // cwd
    char cwd[FILENAME_MAX] = {0};
    getcwd(cwd, FILENAME_MAX);
    char *home = getenv("HOME");
    if (strncmp(cwd, home, strlen(home)) == 0)
        ret += std::string{"~"} + (cwd + strlen(home));
    else
        ret += cwd;

    // close
    ret += ' ';
    ret += esh::Tools::color(nullptr, nullptr);
    ret += esh::Tools::color("white", nullptr);
    ret += " ";
    ret += esh::Tools::color(nullptr, nullptr);
    return ret;
}