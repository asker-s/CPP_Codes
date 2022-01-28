#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <list>

namespace {

std::map<char, long long int> romans = {{'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}};
std::map<std::string, bool> validation;

std::string
readSequence()
{
    std::string line;
    if (!std::getline(std::cin, line) || std::cin.eof() || line.empty()) {
        throw std::runtime_error("No sequence");
    }

    std::regex re("[IVXLCDM]+");
    if (!std::regex_match(line, re)) {
        throw std::runtime_error("Invalid sequence");
    }

    return line;
}

bool
readCommand(std::string& command)
{
    std::string line;
    if ((!std::getline(std::cin, line) || line.empty()) && !std::cin.eof()) {
        throw std::runtime_error("No command");
    }
    
    if (!std::cin.eof()) {
        std::regex re("(list|count)\\s+[0-9]+$");
        if (!std::regex_match(line, re)) {
            throw std::runtime_error("Invalid command");
        }
    }

    command = line;
    return !line.empty();
}

bool
invalidRoman(const std::string& value)
{
    auto it = validation.find(value);
    if (it != validation.end()) {
        return it->second;
    }

    std::regex re("^M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$");
    bool result = !std::regex_match(value, re);
    validation[value] = result;
    return result;
}

long long int
romanNumber(const std::string& value)
{
    if (invalidRoman(value)) {
        return 0;
    }

    long long int result = 0;
    for (size_t i = 0; i < value.size(); ++i) {
        long long int first = romans[value[i]];

        if (i + 1 < value.size()) {
            long long int second = romans[value[i + 1]];
  
            if (first >= second) {
                result = result + first;
            } else { 
                result = result + second - first;
                ++i;
            } 
        } else { 
            result = result + first;

            ++i;
        } 
    } 

    return result; 
}

void
printList(const std::string& sequence, const std::string& current, long long int argument, long long int value, long long int& count, std::list<std::string>& romanList)
{
    if (current.empty() && value == argument) {
        std::cout << "> " << argument << " = ";

        int i = 0;
        for (const auto& number : romanList) {
            if (!number.empty()) {
                if (i != 0) {
                    std::cout << "+";
                }

                std::cout << number;
                ++i;
            }
        }

        std::cout << std::endl;

        ++count;
        return;
    }

    long long int decimal = 0;
    size_t length = 1;
    while (length <= current.size()) { 
        std::string currentValue = current.substr(0, length);
        if ((decimal = romanNumber(current.substr(0, length))) == 0) {
            break;
        }

        romanList.push_back(currentValue);
        std::string newValue = current.substr(length, current.size() - length);
        printList(sequence, newValue, argument, value + decimal, count, romanList);
        romanList.pop_back();
        ++length;
    }
}

void
searchCount(const std::string& sequence, long long int argument, long long int& count, long long int value)
{
    if (sequence.empty() && value == argument) {
        ++count;
        return;
    }

    long long int decimal = 0;
    size_t length = 1;
    while (length <= sequence.size() && (decimal = romanNumber(sequence.substr(0, length))) != 0) {
        searchCount(sequence.substr(length, sequence.size() - length), argument, count, value + decimal);
        ++length;
    }
}

}

int
main()
{
    std::cout << "Sequence:" << std::endl;

    try {
        std::string sequence = readSequence();
        std::cout << "Operations:" << std::endl;

        std::string command;
        while (readCommand(command)) {
            std::smatch match;
            std::regex re("(list|count)\\s+([0-9]+)$");
            if (std::regex_search(command, match, re)) {
                long long int argument = atoll(match[2].str().c_str());
                if (match[1] == "list") {
                    long long int count = 0;
                    std::list<std::string> romanList;
                    printList(sequence, sequence, argument, 0, count, romanList);
                    std::cout << argument << ": " << count << std::endl;
                } else if (match[1] == "count") {
                    long long int count = 0;
                    searchCount(sequence, argument, count, 0);
                    std::cout << argument << ": " << count << std::endl;
                }
            }
        }
    } catch (const std::exception&) {
        std::cout << "Invalid input." << std::endl;
    }
}
