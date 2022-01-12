#include "main.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv, [[maybe_unused]] char **env)
{
    int8_t status = 0;
    while (true)
    {
        std::string in;
        std::cout << prompt(status);
        std::cout << "\033[s";
        std::flush(std::cout);
        esh::Input input;
        size_t last_len = 0;
        while(!input.next())
        {
            std::cout << "\033[u";
            std::cout << std::string(last_len, ' ');
            std::cout << "\033[u";
            esh::Parser tmp_parser{input.getBuf()};
            std::cout << esh::Tools::color(nullptr,nullptr);
            std::cout << esh::Tools::color("green",nullptr);
            if(tmp_parser.getArgs().size() && (!tmp_parser || !esh::wouldRun(tmp_parser.getArgs()[0])))
                std::cout << esh::Tools::color("red",nullptr);
            std::cout << input.getBuf() << esh::Tools::color(nullptr,nullptr);
            std::cout << "\033[u";
            if(input.getCursor())
                std::cout << "\033[" << input.getCursor() << 'C';
            std::flush(std::cout);
            last_len = input.getBuf().size();
        }
        std::cout << std::endl;
        in = input;
        esh::Parser parser{in};

        esh::Runner runner{parser.getArgs()};
        status = runner.wait();
    }
    return 0;
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