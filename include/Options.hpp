//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>

namespace eddic {

/*!
 * Indicates if the given option has been defined. 
 * \param option_name The name of the option to test. 
 * \return true if the option has been defined, otherwise false. 
 */
bool option_defined(const std::string& option_name);

/*!
 * Return the value of the defined option. 
 * \param option_name The name of the option to search. 
 * \return the value of the given option. 
 */
std::string option_value(const std::string& option_name);

int option_int_value(const std::string& option_name);

/*!
 * \brief Parse the options of the command line filling the options. 
 * \param argc The number of argument
 * \param argv The arguments
 * \return A boolean indicating if the options are valid (\c true) or not. 
 */
bool parseOptions(int argc, const char* argv[]);

/*!
 * \brief Print the help.
 */
void print_help();

/*!
 * \brief Print the usage.
 */
void print_version();

} //end of eddic

#endif
