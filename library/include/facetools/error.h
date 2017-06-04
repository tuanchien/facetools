/* Error management functions.
 *
 * Released into the public domain.
 * Explanation: http://creativecommons.org/licenses/publicdomain
 * If your legal jurisdiction does not recognise the public domain, then it is
 * licensed under Boost Software Licence.
 * Boost Licence: http://www.boost.org/users/license.html
 */


#ifndef _FACETOOLS_ERROR_H_
#define _FACETOOLS_ERROR_H_

// ## INCLUDES ################################################################

#include <string>


// ## NAMESPACES ##############################################################

namespace facetools {


// ## FUNCTION DECLARATIONS ###################################################

/**
 * Prints a warning if the expression is not true.
 * \param expression Expression to evaluate.
 * \param error_message Error message.
 * \return Truth of the expression.
 */
bool expect_true(bool expression, std::string error_message) noexcept;


/**
 * Prints an error message if the expression is not true, and throws a runtime exception.
 * \param expression Expression.
 * \param error_message Error message.
 */
void require_true(bool expression, std::string error_message);


} // NAMESPACE facetools

#endif // _FACETOOLS_ERROR_H_