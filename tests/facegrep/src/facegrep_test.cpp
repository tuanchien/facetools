/* Tests for the facegrep class.
 */

// ## INCLUDES ################################################################

#include <gtest/gtest.h>
#include <string>
#include <unordered_set>

#include <facegrep/facegrep.h>
#include <file_finder.h>


// ## NAMESPACES ##############################################################

using namespace facetools;


// ## CONSTANTS ###############################################################

static const char FACE_DETECTOR_MODEL[] = "../models/mmod_human_face_detector.dat";
static const char FACE_RECOGNITION_MODEL[] = "../models/dlib_face_recognition_resnet_model_v1.dat";
static const char SHAPE_PREDICTOR_MODEL[] = "../models/shape_predictor_68_face_landmarks.dat";
static const char BRUCE_TEMPLATE[] = "../test_data/facegrep/searchdir/bruce0.jpg";
static const char ROCK_TEMPLATE[] = "../test_data/facegrep/searchdir/rock0.jpg";
static const char SEARCH_DIR[] = "../test_data/facegrep/searchdir";


// ## PRIVATE METHODS #########################################################

static facegrep get_facegrep()
{
  facegrep_parameters_t params;
  params.jitter = false;
  params.threshold = 0.6;
  params.detector_type = face_detector_type_t::DLIB_DEFAULT;
  params.detector_model = FACE_DETECTOR_MODEL;
  params.recogniser_model = FACE_RECOGNITION_MODEL;
  params.shape_model = SHAPE_PREDICTOR_MODEL;
  facegrep fg(params);

  return fg;
}


// ## TESTS ###################################################################

TEST(facegrep, constructor)
{
  facegrep_parameters_t params;
  params.jitter = false;
  params.threshold = 5.0;
  params.detector_type = face_detector_type_t::MMOD;
  params.detector_model = FACE_DETECTOR_MODEL;
  params.recogniser_model = FACE_RECOGNITION_MODEL;
  params.shape_model = SHAPE_PREDICTOR_MODEL;
  facegrep fg(params);

  EXPECT_EQ(fg.params_.threshold, 5.0);
  EXPECT_EQ(fg.detector_->params_.detector_type, face_detector_type_t::MMOD);
  EXPECT_FALSE(fg.recogniser_->params_.jitter_images);
  EXPECT_EQ(fg.recogniser_->params_.face_difference_threshold, 5.0);
}


TEST(facegrep, init)
{
  auto fg = get_facegrep();

  EXPECT_FALSE(fg.initialised_);

  fg.init(BRUCE_TEMPLATE);

  EXPECT_TRUE(fg.initialised_);
}


TEST(facegrep, face_matched_)
{
  auto fg = get_facegrep();
  fg.init(BRUCE_TEMPLATE);

  auto rock_face = fg.detector_->extract_faces(ROCK_TEMPLATE);
  auto rock_template = fg.recogniser_->get_embedding(rock_face[0]);
  EXPECT_TRUE(fg.face_matched_(fg.template_embedding_, fg.template_embedding_));
  EXPECT_FALSE(fg.face_matched_(fg.template_embedding_, rock_template));
}


TEST(facegrep, search1)
{
  auto fg = get_facegrep();
  fg.init(BRUCE_TEMPLATE);
  auto images = file_finder::find_images(SEARCH_DIR);
  EXPECT_EQ(images.size(), 8);
  auto results = fg.search(images);
  EXPECT_EQ(results.size(), 4);

  std::unordered_set<std::string> bruce_set;
  bruce_set.insert("../test_data/facegrep/searchdir/bruce0.jpg");
  bruce_set.insert("../test_data/facegrep/searchdir/bruce/bruce1.jpg");
  bruce_set.insert("../test_data/facegrep/searchdir/bruce/bruce2.jpg");
  bruce_set.insert("../test_data/facegrep/searchdir/bruce/bruce3.jpg");

  for(auto& file : results)
    EXPECT_EQ(bruce_set.count(file), 1);
}


TEST(facegrep, search2)
{
  auto fg = get_facegrep();
  auto images = file_finder::find_images(SEARCH_DIR);
  EXPECT_EQ(images.size(), 8);
  auto results = fg.search(ROCK_TEMPLATE, images);
  EXPECT_EQ(results.size(), 4);

  std::unordered_set<std::string> rock_set;
  rock_set.insert("../test_data/facegrep/searchdir/rock0.jpg");
  rock_set.insert("../test_data/facegrep/searchdir/rock/rock1.jpg");
  rock_set.insert("../test_data/facegrep/searchdir/rock/rock2.jpg");
  rock_set.insert("../test_data/facegrep/searchdir/rock/rock3.jpg");

  for(auto& file : results)
    EXPECT_EQ(rock_set.count(file), 1);
}