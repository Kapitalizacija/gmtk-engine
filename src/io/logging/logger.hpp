#include <iostream>

#define LOG(msg) \
    std::cout << "\033[0m(LOG) " << __PRETTY_FUNCTION__ << ": " << msg << "\n"

#define WARN(msg) \
    std::cout << "\033[33m(WARN) \033[0m" << __PRETTY_FUNCTION__ << ": " << msg << "\n"

#define ERROR(msg) \
    std::cout << "\033[31m(ERROR) \033[0m" << __PRETTY_FUNCTION__ << ": " << msg << "\n"

namespace GMTKEngine {
    
}