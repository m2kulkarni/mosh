#include <string>
#include <vector>
#include <regex>
#include <iostream>

std::vector<std::pair<int, std::string>> parseAnsiEscapeCodes(const std::string& input) {
    std::vector<std::pair<int, std::string>> result;
    std::regex ansiPattern("\033\\[(\\d+)m([^\033]*)");
    
    auto begin = std::sregex_iterator(input.begin(), input.end(), ansiPattern);
    auto end = std::sregex_iterator();

    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        if (match.size() > 2) {
            int code = std::stoi(match[1].str());
            std::string text = match[2].str();
            result.push_back({code, text});
        }
    }

    return result;
}

int main() {
    std::string input = "\033[31mRed text\033[0m Normal \033[32mGreen text\033[0m";
    auto parsed = parseAnsiEscapeCodes(input);

    for (const auto& item : parsed) {
        std::cout << "Code: " << item.first << ", Text: \"" << item.second << "\"" << std::endl;
    }

    return 0;
}
