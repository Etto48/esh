#include "Parser.hpp"

namespace esh
{
    Parser::Parser(const std::string& line)
    {
        size_t cursor = 0;
        int r_brackets = 0;
        bool qm = false;
        bool ap = false;
        std::vector<std::pair<std::string,size_t>> new_args;
        std::pair<std::string,size_t> next = {"",0};
        for(auto& c : line)
        {
            bool standard_char = false;
            bool space = false;
            switch (c)
            {
            case '\n':
            case ';':
                if((!ap && !qm && r_brackets==0 ) &&  (next.first.size() || new_args.size()))
                {//we add a new command to the line
                    if(next.first.size())
                    {
                        new_args.push_back(next);
                        next = {"",cursor+1};
                    }
                    args.push_back(new_args);
                    new_args = {};
                    space = true;
                }
                break;
            case ' ':
            case '\t':
                space = true;
                break;
            case '\"':
                if(!ap)
                    qm = !qm;
                break;
            case '\'':
                if(!qm)
                    ap = !ap;
                break;
            case '(':
                r_brackets ++;
                next.first += c;
                standard_char = true;
                break;
            case ')':
                r_brackets --;
                next.first += c;
                standard_char = true;
                break;
            default:
                next.first += c;
                standard_char = true;
                break;
            }
            if (r_brackets < 0)
            {
                error = true;
                return;
            }
            else if(((r_brackets > 0) || (qm && c!='\"') || (ap && c!='\'')) && !standard_char)
            {
                next.first += c;
            }
            else if(space && next.first.size() > 0)
            {
                new_args.push_back(next);
                next = {"",cursor+1};
            }
            else if(space)
            {
                next = {"",cursor+1};
            }
            cursor ++;
        }
        if(next.first.size() > 0)
            new_args.push_back(next);
        if(new_args.size() > 0)
            args.push_back(new_args);
    }
    const std::vector<std::vector<std::pair<std::string,size_t>>>& Parser::getArgs() 
    {
        return this->args;
    }   
    Parser::operator bool()
    {
        return !error;
    }
}