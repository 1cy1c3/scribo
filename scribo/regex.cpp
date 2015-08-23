/** @file regex.cpp
 * @brief Represents the regular expressions in this program.
 */

#include "regex.h"

/**
 * Password of the user
 */
const char *Regex::password = "^(?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{8,}$";

/**
 * Initializes the regex object.
 */
Regex::Regex()
{
}

/**
 * Gets the password.
 * @return Password
 */
const char *Regex::getPassword() {
    return password;
}
