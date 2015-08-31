/**
 * @file regex.h
 * @brief Represents the regular expressions in this program
 */

#ifndef REGEX_H
#define REGEX_H

/**
 * Realises the regular expressions
 *
 * @author Rune Krauss
 */
class Regex
{
public:
    /**
     * @brief Initializes the regex object
     */
    Regex();
    /**
     * @brief Gets the password
     * @return Password
     */
    static const char *getPassword();
private:
    /**
     * @brief Password of the user
     */
    static const char *password;
};

#endif // REGEX_H
