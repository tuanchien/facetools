/* Face structure used in the wrapper classes.
 *
 * Released into the public domain.
 * Explanation: http://creativecommons.org/licenses/publicdomain
 * If your legal jurisdiction does not recognise the public domain, then it is
 * licensed under Boost Software Licence.
 * Boost Licence: http://www.boost.org/users/license.html
 */


#ifndef _FACETOOLS_FACE_H_
#define _FACETOOLS_FACE_H_


// ## INCLUDES ################################################################

#include <dlib/image_io.h>
#include <dlib/image_processing.h>
#include <string>


// ## NAMESPACES ##############################################################

namespace facetools {


// ## CUSTOM STRUCTURES #######################################################

/** Face
 * Stores face information.
 */
struct face {
  /** Unique face identifier. Currently unused. */
  int id;

  /** Name of face. Currently unused. */
  std::string name;

  /** Facial region in the image. */
  dlib::mmod_rect bounding_box;

  /** Extracted face. */
  dlib::matrix<dlib::rgb_pixel> image;
};


// ## TYPEDEFS, USING #########################################################

/* 128 dimensional embedding for a face takes this type. */
using embedding_t = dlib::matrix<float,0,1>;


// ## END OF CONTENT ##########################################################

} // NAMESPACE facetools

#endif // _FACETOOLS_FACE_H_