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


// ## INCLUDES ################################################################

#include <facetools/face_detector.h>
#include <facetools/error.h>

#include <algorithm>


// ## NAMESPACES ##############################################################

namespace facetools {


// ## PUBLIC METHODS ##########################################################

face_detector::face_detector(const face_detector_parameters_t& params)
{
  params_.max_scaling_length = params.max_scaling_length;
  params_.max_scaling_times = params.max_scaling_times;
  params_.detector_type = params.detector_type;

  if(params_.detector_type == face_detector_type_t::MMOD)
    dlib::deserialize(params.face_detector_model_file) >> mmod_face_detector_;
  else
    frontal_face_detector_ = dlib::get_frontal_face_detector();

  dlib::deserialize(params.shape_predictor_model_file) >> shape_predictor_;
}


void face_detector::align(face& face, const dlib::matrix<dlib::rgb_pixel>& image)
{
  auto shape = shape_predictor_(image, face.bounding_box);
  dlib::matrix<dlib::rgb_pixel> face_chip;
  dlib::extract_image_chip(image, dlib::get_face_chip_details(shape,150,0.25), face_chip);
  face.image = face_chip;
}


void face_detector::align(std::vector<face>& faces, const dlib::matrix<dlib::rgb_pixel>& image)
{
  for(auto& face : faces)
    align(face, image);
}


std::vector<face> face_detector::detect(dlib::matrix<dlib::rgb_pixel>& image)
{
  int max_size = std::max(image.nr(), image.nc());
  int times_scaled = 0;

  while(max_size < params_.max_scaling_length && times_scaled < params_.max_scaling_times) {
    dlib::pyramid_up(image);
    max_size = std::max(image.nr(), image.nc());
    ++times_scaled;
  }

  if(params_.detector_type == face_detector_type_t::MMOD)
    return mmod_detection_(image);
  else
    return frontal_face_detection_(image);
}


std::vector<face> face_detector::detect(const std::string image_file)
{
  dlib::matrix<dlib::rgb_pixel> image;
  dlib::load_image(image, image_file);

  return detect(image);
}


dlib::matrix<dlib::rgb_pixel> face_detector::downscale_image(const dlib::matrix<dlib::rgb_pixel>& input_image)
{
  int max_dimension = std::max(input_image.nr(), input_image.nc());

  dlib::matrix<dlib::rgb_pixel> image;

  if(max_dimension > params_.max_scaling_length) {
    float scaling = 1.0 * params_.max_scaling_length / (1.0 * max_dimension);
    int resized_width = scaling * input_image.nc();
    int resized_height = scaling * input_image.nr();
    image = dlib::matrix<dlib::rgb_pixel>(resized_height, resized_width);
    dlib::resize_image(input_image, image);
  }
  else {
    image = input_image;
  }

  return image;
}


std::vector<face> face_detector::extract_faces(const std::string image_file)
{
    dlib::matrix<dlib::rgb_pixel> image;
    dlib::load_image(image, image_file);

    return extract_faces(image);
}


std::vector<face> face_detector::extract_faces(dlib::matrix<dlib::rgb_pixel>& image)
{
    auto resized_image = downscale_image(image);
    auto faces = detect(resized_image);
    align(faces, resized_image);

    return faces;
}


// ## PRIVATE METHODS #########################################################

std::vector<face> face_detector::frontal_face_detection_(const dlib::matrix<dlib::rgb_pixel>& image)
{
  std::vector<dlib::rectangle> detections = frontal_face_detector_(image);
  auto detections_size = detections.size();
  std::vector<face> faces(detections_size);

  for(size_t i = 0; i < detections_size; ++i) {
    face face;
    face.bounding_box = std::move(detections[i]);
    faces[i] = face;
  }

  return faces;
}


std::vector<face> face_detector::mmod_detection_(const dlib::matrix<dlib::rgb_pixel>& image)
{
  std::vector<dlib::mmod_rect> detections = mmod_face_detector_(image);
  auto detections_size = detections.size();
  std::vector<face> faces(detections_size);

  for(size_t i = 0; i < detections_size; ++i) {
    face face;
    face.bounding_box = std::move(detections[i]);
    faces[i] = face;
  }

  return faces;
}

} // NAMESPACE facetools
