/* Error management functions.
 *
 * Released into the public domain.
 * Explanation: http://creativecommons.org/licenses/publicdomain
 * If your legal jurisdiction does not recognise the public domain, then it is
 * licensed under Boost Software Licence.
 * Boost Licence: http://www.boost.org/users/license.html
 */


// ## INCLUDES ################################################################

#include <iostream>
#include <stdexcept>

#include <facetools/error.h>


// ## NAMESPACES ##############################################################

namespace facetools {


// ## FUNCTION DEFINITIONS ####################################################

bool expect_true(bool expression, std::string message) noexcept
{
  if(!expression)
    std::cerr << "WARNING: " << message << std::endl;

  return expression;
}


void require_true(bool expression, std::string message)
{
  if(expression)
    return;

  throw std::runtime_error(message);
}


} // NAMESPACE facetools
