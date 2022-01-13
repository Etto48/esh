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
        size_t last_len = 0;
        while(!esh::input.next())
        {
            std::cout << "\033[u";
            std::cout << std::string(last_len, ' ');
            std::cout << "\033[u";
            esh::Parser tmp_parser{esh::input.getBuf()};
            std::cout << esh::Tools::color(nullptr,nullptr);
            size_t linen = 0;
            size_t charn = 0;
            for(auto& c : esh::input.getBuf())
            {
                if(tmp_parser.getArgs().size()>linen && charn == tmp_parser.getArgs()[linen][0].second)
                {
                    if(esh::wouldRun(tmp_parser.getArgs()[linen][0].first))
                    {
                        std::cout << esh::Tools::color("green",nullptr);
                    }
                    else
                    {
                        std::cout << esh::Tools::color("light red",nullptr);
                    }
                }
                else if(tmp_parser.getArgs().size()>linen && charn == tmp_parser.getArgs()[linen][0].second + tmp_parser.getArgs()[linen][0].first.size())
                {
                    std::cout << esh::Tools::color(nullptr,nullptr);
                    linen++;
                }
                std::cout << c;
                charn ++;
            }
            std::cout << esh::Tools::color(nullptr,nullptr);
            std::cout << "\033[u";
            if(esh::input.getCursor())
                std::cout << "\033[" << esh::input.getCursor() << 'C';
            std::flush(std::cout);
            last_len = esh::input.getBuf().size();
        }
        std::cout << std::endl;
        in = esh::input.getBuf();
        esh::Parser parser{in};
        
        esh::input.reset();

        for(auto& a : parser.getArgs())
        {
            esh::Runner runner{a};
            status = runner.wait();
        }
    }
    return 0;
}

std::string prompt(int8_t status)
{
    std::string ret = "";

    ret += esh::Tools::color("green", nullptr);
#ifdef POWERLINE
    ret += "";
#else
    ret += "(";
#endif
    ret += esh::Tools::color("white", "green");

    // error
    ret += esh::Tools::color("light red", "green");
    ret += status ? "x " : "";
    ret += esh::Tools::color("white", "green");

    // username
    ret += esh::Tools::getUserName();

    ret += ' ';
    ret += esh::Tools::color("green", "white");
#ifdef POWERLINE
    ret += "";
#else
    ret += ">";
#endif
    ret += ' ';

    // cwd
    char cwd[FILENAME_MAX] = {0};
    if(!getcwd(cwd, FILENAME_MAX))
        strcpy(cwd,"?");
    char *home = getenv("HOME");
    if (strncmp(cwd, home, strlen(home)) == 0)
        ret += std::string{"~"} + (cwd + strlen(home));
    else
        ret += cwd;

    // close
    ret += ' ';
    ret += esh::Tools::color(nullptr, nullptr);
    ret += esh::Tools::color("white", nullptr);
#ifdef POWERLINE
    ret += "";
#else 
    ret += ">";
#endif
    ret += ' ';
    ret += esh::Tools::color(nullptr, nullptr);
    return ret;
}