/* Tests for the FaceTools FaceDetector class.
 *
 * Released into the public domain.
 * Explanation: http://creativecommons.org/licenses/publicdomain
 * If your legal jurisdiction does not recognise the public domain, then it is
 * licensed under Boost Software Licence.
 * Boost Licence: http://www.boost.org/users/license.html
 */


// ## INCLUDES ####################################################################################

#include <gtest/gtest.h>
#include <iostream>
#include <dlib/image_transforms.h>
#include <dlib/data_io.h>
#include <dlib/gui_widgets.h>
#include <stdexcept>

#include "face_detector.h"
#include "error.h"


// ## NAMESPACES ##################################################################################

using namespace facetools;
using namespace std;
using namespace dlib;


// ## CONSTANTS ###################################################################################

static const char FACE_DETECTOR_MODEL[] = "../models/mmod_human_face_detector.dat";
static const char SHAPE_PREDICTOR_MODEL[] = "../models/shape_predictor_68_face_landmarks.dat";
static const char BALD_GUYS[] = "../test_data/facetools_library/bald_guys.jpg";


// ## PRIVATE METHODS #############################################################################

static face_detector get_detector(face_detector_type_t detector_type = face_detector_type_t::MMOD)
{
  face_detector_parameters_t params;
  params.face_detector_model_file = FACE_DETECTOR_MODEL;
  params.shape_predictor_model_file = SHAPE_PREDICTOR_MODEL;
  params.detector_type = detector_type;
  face_detector detector(params);

  return detector;
}

static matrix<rgb_pixel> get_image()
{
  string image_file = BALD_GUYS;
  matrix<rgb_pixel> image;
  load_image(image, image_file);

  return image;
}


// ## TESTS #######################################################################################

TEST(face_detector, init)
{
  face_detector_parameters_t params;
  params.face_detector_model_file = FACE_DETECTOR_MODEL;
  params.shape_predictor_model_file = SHAPE_PREDICTOR_MODEL;
  ASSERT_NO_THROW(face_detector detector(params));
}


TEST(face_detector, detect)
{
  auto detector = get_detector();
  auto image = get_image();
  auto faces = detector.detect(image);

  EXPECT_EQ(24, faces.size());
}


TEST(face_detector, detect_downsample)
{
  auto detector = get_detector();
  auto image = get_image();
  auto resized_image = detector.downscale_image(image);
  auto faces = detector.detect(resized_image);

  EXPECT_EQ(24, faces.size());
}


TEST(face_detector, align)
{
  auto detector = get_detector();
  auto image = get_image();
  auto resized_image = detector.downscale_image(image);
  auto faces = detector.detect(resized_image);

  EXPECT_EQ(24, faces.size());

  ASSERT_NO_THROW(detector.align(faces, resized_image));

  // std::vector<image_window> win(faces.size());
  // for(int i=0; i<win.size(); ++i)
  //   win[i].set_image(faces[i].image);

  // cout << "Hit enter to exit." << endl;
  // cin.get();
}


TEST(face_detector, extract_faces1)
{
  auto detector = get_detector();
  auto image = get_image();
  auto faces = detector.extract_faces(image);
  EXPECT_EQ(24, faces.size());
}


TEST(face_detector, extract_faces2)
{
  auto detector = get_detector();
  auto faces = detector.extract_faces(BALD_GUYS);
  EXPECT_EQ(24, faces.size());
}