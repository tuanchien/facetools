/* Face detection wrapper to the MMOD and frontal face detection HOG model
 * found in dlib.
 *
 * Adapted from code in dlib examples. Thanks David King for the framework and
 * the pre-trained models, and of course the collective researchers who came up
 * with the architectures.
 * See: dnn_face_recognition_ex.cpp, dnn_mmod_face_detection_ex.cpp in dlib.
 *
 * Released into the public domain.
 * Explanation: http://creativecommons.org/licenses/publicdomain
 * If your legal jurisdiction does not recognise the public domain, then it is
 * licensed under Boost Software Licence.
 * Boost Licence: http://www.boost.org/users/license.html
 *
 * Models can be downloaded from:
 * http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
 * http://dlib.net/files/mmod_human_face_detector.dat.bz2
 */


#ifndef _FACETOOLS_FACE_DETECTOR_H_
#define _FACETOOLS_FACE_DETECTOR_H_


// ## INCLUDES ################################################################

#include <iostream>
#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <vector>
#include <string>

#include "face.h"


// ## NAMESPACES ##############################################################

namespace facetools {


// ## CONSTANTS ###############################################################

// ## TYPE DEFINITIONS ########################################################

/* Sets up the templates for the Max Margin Object Detection neural network. */

template <long num_filters, typename SUBNET>
using con5d = dlib::con<num_filters,5,5,2,2,SUBNET>;

template <long num_filters, typename SUBNET>
using con5  = dlib::con<num_filters,5,5,1,1,SUBNET>;

template <typename SUBNET>
using downsampler  = dlib::relu<dlib::affine<con5d<32, dlib::relu<dlib::affine<con5d<32,dlib::relu<dlib::affine<con5d<16,SUBNET>>>>>>>>>;

template <typename SUBNET>
using rcon5  = dlib::relu<dlib::affine<con5<45,SUBNET>>>;

using mmod_facenet = dlib::loss_mmod<dlib::con<1,9,9,1,1,rcon5<rcon5<rcon5<downsampler<dlib::input_rgb_image_pyramid<dlib::pyramid_down<6>>>>>>>>;


// ## CUSTOM STRUCTURES #######################################################

/**
 * Supported face detection methods.
 * The MMOD method is extremely memory and calculation intensive. Only select if you have a lot of ram or preferrably, a GPU with a lot of ram.
 */
enum class face_detector_type_t {
  /** Frontal_face_detector method in dlib. Based off 5 HOG classifiers. */
  DLIB_DEFAULT = 0,

  /** Max margin object detection. */
  MMOD
};


/**
 * Parameters to initialise the FaceDetector class.
 */
struct face_detector_parameters_t {
  /** Detector we want to use. */
  face_detector_type_t detector_type;

  /** Path to face detector model file (mmod). */
  std::string face_detector_model_file;

  /** Maximum number of times the image can be doubled in size during search. */
  unsigned int max_scaling_times;

  /** Maximum size of width or height allowed before an image is upscaled. */
  unsigned int max_scaling_length;

  /** Path to shape predictor model file (68 point predictor). */
  std::string shape_predictor_model_file;

  face_detector_parameters_t()
  {
    detector_type = face_detector_type_t::MMOD;
    face_detector_model_file = "mmod_human_face_detector.dat";
    max_scaling_times = 5;
    max_scaling_length = 1800;
    shape_predictor_model_file = "shape_predictor_68_face_landmarks.dat";
  }
};


// ## CLASS DEFINITION ########################################################

class face_detector {
public:
  /**
   * \param params Structure containing the parameters.
   */
  face_detector(const face_detector_parameters_t& params);


  /**
   * Applies an affine transformation to the identified face region to 'straighten' it. Saves it in the face structure.
   * \param face Face structure to store results and to get facial region information. Returned from a detect call.
   * \param image Image where the face came from.
   */
  void align(face& face, const dlib::matrix<dlib::rgb_pixel>& image);


  /**
   * Applies an affine transformation to the identified face region to 'straighten' it. Saves it in the face structure.
   * \param faces List of face structure to store results and to get facial region information. Returned from a detect call.
   * \param image Image where the face came from.
   */
  void align(std::vector<face>& faces, const dlib::matrix<dlib::rgb_pixel>& image);


  /**
   * Detects all faces in an image.
   * \param image Image as a matrix.
   * \return List of faces found.
   */
  std::vector<face> detect(dlib::matrix<dlib::rgb_pixel>& image);


  /**
   * Detects all faces in an image.
   * \param image_file Image file name (string).
   * \return List of faces found.
   */
  std::vector<face> detect(const std::string image_file);


  /**
   * Downscales the image to have max dimensions (width or height) less than max_scaling_length (parameter).
   * \param input_image Image to downscale.
   * \return Downscaled image.
   */
  dlib::matrix<dlib::rgb_pixel> downscale_image(const dlib::matrix<dlib::rgb_pixel>& input_image);


  /**
   * Detect and align faces.
   * \param image_file Image file where the face came from.
   * \return List of faces found (including the aligned face images).
   */
  std::vector<face> extract_faces(const std::string image_file);


  /**
   * Detect and align faces.
   * \param image The image as a matrix.
   * \return List of faces found (including the aligned face images).
   */
  std::vector<face> extract_faces(dlib::matrix<dlib::rgb_pixel>& image);


#ifndef _DEBUG_
private:
#endif

  /**
   * Similar to face_detector_parameters_t.
   */
  struct internal_parameters_t {
    face_detector_type_t detector_type;
    unsigned int max_scaling_times;
    unsigned int max_scaling_length;
  } params_;


  /** DLIB_DEFAULT detector. */
  dlib::frontal_face_detector frontal_face_detector_;

  /** Max margin object detector. */
  mmod_facenet mmod_face_detector_;

  /** Shape predictor. */
  dlib::shape_predictor shape_predictor_;


  /**
   * Uses the frontal_face_detector to do a face detection.
   * \param image Image to search.
   * \return List of faces found.
   */
  std::vector<face> frontal_face_detection_(const dlib::matrix<dlib::rgb_pixel>& image);


  /**
   * Uses the max margin object detector to do a face detection.
   * \param image Image to search.
   * \return List of faces found.
   */
  std::vector<face> mmod_detection_(const dlib::matrix<dlib::rgb_pixel>& image);

};


} // NAMESPACE facetools

#endif // _FACETOOLS_FACE_DETECTOR_H_