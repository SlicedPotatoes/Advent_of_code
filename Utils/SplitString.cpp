#include <vector>

std::vector<std::string> splitString(const std::string &input, const std::string &delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;

    while ((end = input.find(delimiter, start)) != std::string::npos)
    {
        if (end != start)
        {
            tokens.push_back(input.substr(start, end - start));
        }
        start = end + delimiter.length();
    }

    if (start < input.length())
    {
        tokens.push_back(input.substr(start));
    }

    return tokens;
}