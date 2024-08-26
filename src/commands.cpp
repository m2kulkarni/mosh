#include <array>
#include <cstdio>
#include <memory>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <iostream>

#include <sys/wait.h>

std::string execCommands(const std::string& command)
{
    std::array<char, 128> buffer;
    std::string result;
    std::string fish_command = "fish -c \"" + command + "\" | cat -v";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(fish_command.c_str(), "r"), pclose);
    if(!pipe)
        throw std::runtime_error("popen() failed");

    while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}
