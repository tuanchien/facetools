#ifndef _FACEGREP_H_
#define _FACEGREP_H_

// ## INCLUDE #################################################################

#include <memory>

#include <facetools/face_detector.h>
#include <facetools/face_recogniser.h>


// ## NAMESPACE ###############################################################

namespace facetools {


// ## CUSTOM STRUCTURES #######################################################

/**
 * Parameters for the facegrep class.
 */
struct facegrep_parameters_t {
  /** Type of face detector selected. */
  face_detector_type_t detector_type;

  /** Whether to apply jitter transformation to images to get a slighly more robust embedding (slower though). */
  bool jitter;

  /** Threshold for deciding closeness between faces. Values between [0,1].  dlib optimised this model for 0.6. */
  float threshold;

  /** File path of detector model. */
  std::string detector_model;

  /** File path of recogniser model. */
  std::string recogniser_model;

  /** File path of shape model. */
  std::string shape_model;

  facegrep_parameters_t()
  {
    detector_type = face_detector_type_t::DLIB_DEFAULT;
    jitter = true;
    threshold = 0.6;
    detector_model = "mmod_human_face_detector.dat";
    recogniser_model = "dlib_face_recognition_resnet_model_v1.dat";
    shape_model = "shape_predictor_68_face_landmarks.dat";
  }
};


// ## CLASS DEFINITION ########################################################

/**
 * A class for finding similar faces.
 */
class facegrep {
public:
  /**
   * Constructs a facegrep object.
   * \param params Parameters to initialise facegrep with.
   */
  facegrep(const facegrep_parameters_t& params);


  /**
   * Initialises facegrep. Required before performing a search function.
   * \param face_template_file Face we're searching for.
   */
  void init(const std::string face_template_file);


  /**
   * Tries to find the face template in the image files given.
   * \param image_files List of image file names to search through for the template face.
   * \return List of image files where matches were found.
   */
  std::vector<std::string> search(const std::vector<std::string> image_files);

  /**
   * Tries to find the face template in the image files given.
   * \param face_template_file Face we're searching for.
   * \param image_files List of image file names to search through for the template face.
   * \return List of image files where matches were found.
   */
  std::vector<std::string> search(const std::string face_template_file, const std::vector<std::string> image_files);

#ifndef _DEBUG_
private:
#endif

  /** See facegrep_parameters_t. */
  struct internal_parameters_t {
    float threshold;
  } params_;

  /** Whether the face template has been initialised for search. */
  bool initialised_;

  /** Embedding for our template face.*/
  embedding_t template_embedding_;

  /** Face detector. */
  std::unique_ptr<face_detector> detector_;

  /** Face recogniser. */
  std::unique_ptr<face_recogniser> recogniser_;


  /**
   * Determines whether two embeddings are sufficiently close. Closeness is determined by the threshold parameter.
   * \param face1 First face in comparison.
   * \param face2 Second face in the comparison.
   * \return True if close, false if not close.
   */
  bool face_matched_(const embedding_t& face1, const embedding_t& face2);
};


} // NAMESPACE facetools

#endif // _FACEGREP_H_