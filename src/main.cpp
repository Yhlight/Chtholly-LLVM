#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;
    }

    std::cout << "Chtholly Compiler" << std::endl;
    std::cout << "Source file: " << argv[1] << std::endl;

    return 0;
}
