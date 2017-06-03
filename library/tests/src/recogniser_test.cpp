/* Tests for the FaceTools FaceRecogniser class.
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
#include <vector>

#include "face_detector.h"
#include "face_recogniser.h"
#include "error.h"


// ## NAMESPACES ##################################################################################

using namespace facetools;
using namespace std;
using namespace dlib;


// ## CONSTANTS ###################################################################################

static const char FACE_DETECTOR_MODEL[] = "../models/mmod_human_face_detector.dat";
static const char FACE_RECOGNITION_MODEL[] = "../models/dlib_face_recognition_resnet_model_v1.dat";
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


static matrix<rgb_pixel> get_image(face_detector& detector, const string image_file)
{
  matrix<rgb_pixel> image;
  load_image(image, image_file);
  auto resized_image = detector.downscale_image(image);
  return resized_image;
}


static std::vector<face> detect_and_align(const string image_file, face_detector_type_t detector_type = face_detector_type_t::MMOD)
{
  auto detector = get_detector(detector_type);
  auto image = get_image(detector, image_file);
  auto faces = detector.detect(image);
  detector.align(faces, image);

  return faces;
}


static face_recogniser get_recogniser(float face_threshold = 0.4, bool jitter_images = false)
{
  face_recogniser_parameters_t params;
  params.recogniser_model_file = FACE_RECOGNITION_MODEL;
  params.face_difference_threshold = face_threshold;
  params.jitter_images = jitter_images;
  face_recogniser recogniser(params);

  return recogniser;
}


static void print_embedding(embedding_t embedding)
{
  cout << "Vector: (";
  for(auto it = embedding.begin(); it != embedding.end(); ++it)
  {
    if(it != embedding.begin())
      cout << ", ";

    cout << *it;
  }

  cout << ")\n";
}

// ## TESTS #######################################################################################

TEST(face_recogniser, init) {
  auto faces = detect_and_align(BALD_GUYS);
  auto recogniser = get_recogniser();
}


TEST(face_recogniser, get_embeddings_mmod) {
  auto faces = detect_and_align(BALD_GUYS);
  auto recogniser = get_recogniser();

  auto embeddings = recogniser.get_embedding(faces);

  EXPECT_EQ(24, embeddings.size());

  for(auto embedding : embeddings) {
    EXPECT_EQ(embedding.nr() * embedding.nc(), 128);
    // print_embedding(embedding);
  }
}

TEST(face_recogniser, get_people_mmod) {
  auto faces = detect_and_align(BALD_GUYS);
  auto recogniser = get_recogniser(0.6);

  auto embeddings = recogniser.get_embedding(faces);

  EXPECT_EQ(24, embeddings.size());

  auto people = recogniser.get_people(embeddings);
  size_t people_size = people.size();
  EXPECT_EQ(4, people_size);

  // std::vector<image_window> win_clusters(people_size);
  // for(size_t i=0; i<people_size; ++i) {
  //   std::vector<matrix<rgb_pixel>> temp;

  //   for (auto face_index : people[i])
  //       temp.push_back(faces[face_index].image);

  //   win_clusters[i].set_title("face cluster " + cast_to_string(i));
  //   win_clusters[i].set_image(tile_images(temp));
  // }

  // cout << "Press key to exit.\n";
  // cin.get();
}


TEST(face_recogniser, get_embeddings_dlib_default) {
  auto faces = detect_and_align(BALD_GUYS, face_detector_type_t::DLIB_DEFAULT);
  auto recogniser = get_recogniser();

  auto embeddings = recogniser.get_embedding(faces);

  EXPECT_EQ(24, embeddings.size());

  for(auto embedding : embeddings) {
    EXPECT_EQ(embedding.nr() * embedding.nc(), 128);
    // print_embedding(embedding);
  }
}

TEST(face_recogniser, get_people_dlib_default) {
  auto faces = detect_and_align(BALD_GUYS, face_detector_type_t::DLIB_DEFAULT);
  auto recogniser = get_recogniser(0.6);

  auto embeddings = recogniser.get_embedding(faces);

  EXPECT_EQ(24, embeddings.size());

  auto people = recogniser.get_people(embeddings);
  size_t people_size = people.size();
  EXPECT_EQ(4, people_size);

  // std::vector<image_window> win_clusters(people_size);
  // for(size_t i=0; i<people_size; ++i) {
  //   std::vector<matrix<rgb_pixel>> temp;

  //   for (auto face_index : people[i])
  //       temp.push_back(faces[face_index].image);

  //   win_clusters[i].set_title("face cluster " + cast_to_string(i));
  //   win_clusters[i].set_image(tile_images(temp));
  // }

  // cout << "Press key to exit.\n";
  // cin.get();
}


// TEST(face_recogniser, custom_people) {
//   auto faces = detect_and_align("../custom.jpg", face_detector_type_t::MMOD);
//   auto recogniser = get_recogniser(0.4);

//   auto embeddings = recogniser.get_embedding(faces);

//   // EXPECT_EQ(3, embeddings.size());

//   // for(auto embedding : embeddings)
//   //   print_embedding(embedding);

//   auto people = recogniser.get_people(embeddings);
//   size_t people_size = people.size();
//   // EXPECT_EQ(3, people_size);

//   std::vector<image_window> win_clusters(people_size);
//   for(size_t i=0; i<people_size; ++i) {
//     std::vector<matrix<rgb_pixel>> temp;

//     for (auto face_index : people[i])
//         temp.push_back(faces[face_index].image);

//     EXPECT_GT(temp.size(), 0);
//     win_clusters[i].set_title("face cluster " + cast_to_string(i));
//     win_clusters[i].set_image(tile_images(temp));
//   }

//   cout << "Press key to exit.\n";
//   cin.get();
// }