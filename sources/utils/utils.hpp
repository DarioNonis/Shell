#ifndef UTILS_HPP
#define UTILS_HPP

#include "utils/libraries.hpp"

/**
 * @brief Convert an std::vector of std::string to a char* array (it's a copy that must be free)
 *
 * @param string_array the std::vector of std::string to convert
 * @return the result char* array
 */
char*const* to_char_array(const std::vector<std::string>& string_array);

/**
 * @brief Show the error message (exit if child, throw exception otherwise)
 *
 * @param message the error messange
 * @param child true if it's the child process
 */
void show_error(const std::string& message, bool child);

#endif
