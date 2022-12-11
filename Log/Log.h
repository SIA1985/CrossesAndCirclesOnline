#include <iostream>


//Заменить макросы ввода-вывода на функции!

#define IN_STREAM(stream, message)                                              stream << message

#define LOG(log)                                                                std::cout << "\033[31m\nОшибка: "  << log << "\033[37m\n"
#define SUCCESS(message)                                                        std::cout << "\033[32m\n" << message << "\033[37m\n"
#define CONSOLE(separateSymbol1, message, separateSymbol2)                      std::cout << separateSymbol1 << message << separateSymbol2

#define IN_STREAM_CONSOLE(stream, separateSymbol1, message, separateSymbol2)    IN_STREAM(stream, separateSymbol1); IN_STREAM(stream, message); IN_STREAM(stream, separateSymbol2) 


#define SAVE_CURSOR_POSITION()                                                  std::cout << "\033[s"
#define RESTORE_CURSORE_POSITION()                                              std::cout << "\033[u"

#define CLEAR_INPUT()                                                           std::cout << "\033[2K" << "> " 
#define CLEAR_ALL_AFTER_CURSOR()                                                std::cout << "\n\033[J"

#define CLEAR_ALL_TERMINAL()                                                    RESTORE_CURSORE_POSITION(); CLEAR_ALL_AFTER_CURSOR()