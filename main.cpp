#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

int main() {




    // Open the input file
    std::ifstream inputFile2("input.txt");
    if (!inputFile2.is_open()) {
        std::cerr << "Error opening input file" << std::endl;
        return 1;
    }

    // Open the output file
    std::ofstream outputFile2("output.txt");
    if (!outputFile2.is_open()) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    // Read the content of the file line by line
    std::string line2;
    while (std::getline(inputFile2 >> std::ws, line2)) {
        // Remove the semicolon at the end of the line, if present
        if (!line2.empty() && line2.back() == ';') {
            line2.pop_back();
        }

        // Skip empty lines or lines containing only whitespace
        if (!std::all_of(line2.begin(), line2.end(), [](unsigned char c) { return std::isspace(c); })) {
            // Write the modified line to the output file
            outputFile2 << line2 << std::endl;
        }
    }

    // Close the files
    inputFile2.close();
    outputFile2.close();

    std::cout << "Semicolons, leading and trailing spaces, and empty lines removed successfully." << std::endl;







       //////////////////////////////////////////////////
    //////////////////////Separation////////////////////
    ////////////////////////////////////////////////////





    std::ifstream inputFile("output.txt");
    std::ofstream outputFile("transformed_output.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return 1;
    }

    std::string line;

    while (std::getline(inputFile, line)) {

        if (line.find("#region") != std::string::npos || line.find("#endregion") != std::string::npos) {
            // Write the full line to the output file
            outputFile << line << std::endl;
            continue; // Skip the rest of the loop for this line
        }
        // Find the return type, function name, and parameter
        size_t returnTypeEnd = line.find(" ");
        size_t functionNameStart = line.find(" ", returnTypeEnd );
        size_t functionNameEnd = line.find("(");
        size_t parameterStart = functionNameEnd;
        size_t parameterEnd = line.find(")");

        if (returnTypeEnd != std::string::npos &&
            functionNameStart != std::string::npos && functionNameEnd != std::string::npos &&
            parameterStart != std::string::npos && parameterEnd != std::string::npos) {

            std::string returnType = line.substr(0, returnTypeEnd);
            std::string functionName = line.substr(functionNameStart + 1, functionNameEnd - functionNameStart - 1);
            std::string parameters = line.substr(parameterStart + 1, parameterEnd - parameterStart - 1);

            // Transform the line
            std::ostringstream transformedLine;
            transformedLine << "public async " << returnType << " " << functionName << "(" << parameters << ")" << "{" << std::endl;
            transformedLine << "\treturn await _repository." << functionName << "(";
            std::vector<std::string> parameterArray;
    std::istringstream iss(parameters);
    std::string parameter;
    while (std::getline(iss, parameter, ',')) {
        // Trim leading and trailing whitespaces from parameter
        size_t start = parameter.find_first_not_of(" ");
        size_t end = parameter.find_last_not_of(" ");
        parameterArray.push_back(parameter.substr(start, end - start + 1));
    }
    //std::cout << "Parameter names without type information:" << std::endl;
bool firstParameter = true;  // Flag to track the first parameter

for (const std::string& param : parameterArray) {
    // Extract only the parameter name
    size_t spaceIndex = param.find_last_of(" ");
    if (spaceIndex != std::string::npos) {
        if (!firstParameter) {
            transformedLine << ", ";
        }
        transformedLine << param.substr(spaceIndex + 1);
    } else {
        if (!firstParameter) {
            transformedLine << ", ";
        }
        transformedLine << param;
    }

    // After the first iteration, set the flag to false
    firstParameter = false;
}

            transformedLine << ");" << std::endl;
            transformedLine << "}" << std::endl;

            // Write the transformed line to the output file
            outputFile << transformedLine.str() << std::endl;
        }
    }

    std::cout << "Transformation complete. Check transformed_output.txt." << std::endl;

    inputFile.close();
    outputFile.close();

    return 0;
}
