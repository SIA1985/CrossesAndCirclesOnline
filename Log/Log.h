#include <iostream>
#include <stdio.h>

#define LOG(log)                                                std::cerr << "\n\033[31m"  << log << "\033[37m\n"
#define SUCCESS(message)                                        std::cout << "\n\033[32m" << message << "\033[37m\n"
#define CONSOLE(separateSymbol1, message, separateSymbol2)      std::cout << separateSymbol1 << message << separateSymbol2