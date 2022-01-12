#include "Parser.hpp"

namespace esh
{
    Parser::Parser(const std::string& line)
    {
        int r_brackets = 0;
        bool qm = false;
        bool ap = false;
        std::string next = "";
        for(auto& c : line)
        {
            bool standard_char = false;
            bool space = false;
            switch (c)
            {
            case ' ':
            case '\t':
            case '\n':
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
                next += c;
                standard_char = true;
                break;
            case ')':
                r_brackets --;
                next += c;
                standard_char = true;
                break;
            default:
                next += c;
                standard_char = true;
                break;
            }
            if (r_brackets < 0)
            {
                error = true;
                return;
            }
            else if(((r_brackets > 0) || (qm && c!='\"') || (ap && c!='\'')) && !standard_char)
                next += c;
            else if(space && next.size() > 0)
            {
                args.push_back(next);
                next = "";
            }
        }
        if(next.size() > 0)
            args.push_back(next);
    }
    const std::vector<std::string>& Parser::getArgs() 
    {
        return this->args;
    }   
    Parser::operator bool()
    {
        return !error;
    }
}