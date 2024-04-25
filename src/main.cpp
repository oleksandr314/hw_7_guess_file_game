#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include<random>

class RandomGenerator {
public:
    explicit RandomGenerator(unsigned int seed = std::random_device{}())
        : engine(seed) {}

    int getRandomNumber(int min = 0, int max = 1000) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }

private:
    std::mt19937 engine;
};

std::vector<std::string> getFileNames(const std::filesystem::path& directory) {
    std::vector<std::string> fileNames;
    for (const auto& item : std::filesystem::directory_iterator(directory)) {
        if (item.is_regular_file()) {
            fileNames.push_back(item.path().filename().string());
        }
    }
    return fileNames;
}

std::string chooseRandomFile(const std::filesystem::path& directory) {
    std::vector<std::string> fileNames;
    for (const auto& item : std::filesystem::directory_iterator(directory)) {
        if (item.is_regular_file()) {
            fileNames.push_back(item.path().filename().string());
        }
    }

    if (fileNames.empty()) {
        throw std::invalid_argument("No files found in the directory!");
    }
    RandomGenerator rg;
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    return fileNames[rg.getRandomNumber() % fileNames.size()];
}

void playGame(const std::vector<std::string>& fileNames, const std::string& randomFile) {
    if (fileNames.empty()) {
        std::cout << "No files found in the directory!" << std::endl;
        return;
    }

    std::cout << "Welcome to the File Guessing Game!" << std::endl;
    std::cout << "You have 3 attempts to guess the name of a file." << std::endl;
    std::cout << "Good luck!" << std::endl;

    for (int attempt = 1; attempt <= 3; ++attempt) {
        std::string guess;
        std::cout << "Attempt " << attempt << ": ";
        std::cin >> guess;

        if (guess == randomFile) {
            std::cout << "You win! The file '" << randomFile << "' exists." << std::endl;
            return;
        } else {
            std::cout << "Incorrect guess. Try again!" << std::endl;
        }
    }

    std::cout << "Sorry, you've run out of attempts. The file was: " << randomFile << std::endl;
}

int main(int argc, char* argv[]) {
    std::filesystem::path defaultDirectory = std::filesystem::current_path(); // Set default path to executable directory
    std::filesystem::path directoryPath = defaultDirectory;
    
    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-help") {
            std::cout << "Usage: program_name [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -help                 Print this help message" << std::endl;
            std::cout << "  -set-folder-path <path to folder>   Set the folder path" << std::endl;
            std::cout << "  -current-folder       Use the current folder as the default path" << std::endl;
            return 0;
        } else if (arg == "-set-folder-path") {
            if (i + 1 < argc) {
                directoryPath = argv[i + 1];
                ++i; // Skip the next argument as it's the folder path
            } else {
                std::cerr << "Error: -set-folder-path option requires an argument." << std::endl;
                return 1;
            }
        } else if (arg == "-current-folder") {
            // No need to do anything, defaultDirectory is already set to the current folder
        } else {
            std::cerr << "Error: Unknown option '" << arg << "'." << std::endl;
            return 1;
        }
    }
    std::string randomFile = chooseRandomFile(directoryPath);
    std::vector<std::string> fileNames = getFileNames(directoryPath);
    playGame(fileNames, randomFile);

    return 0;
}
