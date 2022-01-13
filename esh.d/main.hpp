#pragma once
#include <iostream>
#include <string.h>

#include "Parser/Parser.hpp"
#include "Runner/Runner.hpp"
#include "Input/Input.hpp"

int main(int argc, char **argv, char **env);
std::string prompt(int8_t status);