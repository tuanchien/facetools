/* Face recognition wrapper for the recognition method in the dlib examples.
 * Uses a neural network to extract a 128 dimensional embedding of a face. It
 * then uses a metric based connectivity criterion to use for the Chinese
 * Whispers clustering algorithm.
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
 * Model can be downloaded from:
 * http://dlib.net/files/dlib_face_recognition_resnet_model_v1.dat.bz2
 */


// ## INCLUDES ################################################################

#include "face_recogniser.h"
#include "error.h"


// ## NAMESPACES ##############################################################

namespace facetools {


// ## PUBLIC METHODS ##########################################################

face_recogniser::face_recogniser(const face_recogniser_parameters_t& params)
{
  require_true(params.face_difference_threshold > 0, "recogniser: face difference threshold must be > 0");

  params_.face_difference_threshold = params.face_difference_threshold;
  params_.jitter_images = params.jitter_images;
  dlib::deserialize(params.recogniser_model_file) >> recogniser_;
}


embedding_t face_recogniser::get_embedding(const face& input_face)
{
  std::vector<dlib::matrix<dlib::rgb_pixel>> face;
  face.push_back(input_face.image);
  auto embeddings = recogniser_(face);

  return embeddings[0];
}


std::vector<embedding_t> face_recogniser::get_embedding(const std::vector<face>& input_faces)
{
  unsigned int input_faces_size = input_faces.size();
  std::vector<dlib::matrix<dlib::rgb_pixel>> faces(input_faces_size);

  std::vector<embedding_t> embeddings;

  if(params_.jitter_images)
  {
    for(int i=0; i<input_faces_size; i++)
    {
      auto jitter_stack = jitter_image_(input_faces[i].image);
      dlib::matrix<float,0,1> face_descriptor = dlib::mean(dlib::mat(recogniser_(jitter_stack)));
      embeddings.push_back(std::move(face_descriptor));
    }
  }
  else
  {
    for(int i=0; i<input_faces_size; i++)
      faces[i] = input_faces[i].image;

    embeddings = recogniser_(faces);
  }

  return embeddings;
}


bool face_recogniser::get_jitter() const noexcept
{
  return params_.jitter_images;
}


std::vector<facelist_t> face_recogniser::get_people(const std::vector<embedding_t>& embeddings)
{
  return get_chinese_whispers_clusters_(embeddings);
}


void face_recogniser::set_jitter(bool state) noexcept
{
  params_.jitter_images = true;
}

// ## PRIVATE METHODS #########################################################

size_t face_recogniser::chinese_whispers_get_unvisited_faces_(std::vector<bool>& visited_face, const size_t labels_size)
{
  for(size_t i=0; i<labels_size; ++i)
    visited_face[i] = true;

  size_t unvisited_count = 0;
  for(auto visited : visited_face)
    if(!visited)
      ++unvisited_count;

  return unvisited_count;
}


void face_recogniser::chinese_whispers_add_remaining_faces_(std::vector<facelist_t>& people, const std::vector<bool>& visited_face,
  const size_t faces_size, const size_t num_clusters)
{
  size_t write_position = num_clusters;
  for(size_t i=0; i<faces_size; ++i)
  {
    if(!visited_face[i])
    {
      people[write_position].push_back(i);
      ++write_position;
    }
  }
}


std::vector<facelist_t> face_recogniser::get_chinese_whispers_clusters_(const std::vector<embedding_t>& embeddings)
{
  size_t faces_size = embeddings.size();

  std::vector<dlib::sample_pair> edges;
  for (size_t i = 0; i < faces_size; ++i)
    for (size_t j = i+1; j < faces_size; ++j)
      if (dlib::length(embeddings[i]-embeddings[j]) < params_.face_difference_threshold)
        edges.push_back(dlib::sample_pair(i,j));

  std::vector<unsigned long> labels;
  const auto num_clusters = dlib::chinese_whispers(edges, labels);
  auto labels_size = labels.size();

  std::vector<bool> visited_face(faces_size);
  auto unvisited_count = chinese_whispers_get_unvisited_faces_(visited_face, labels_size);

  std::vector<facelist_t> people(num_clusters + unvisited_count);

  for(size_t i=0; i<labels_size; ++i)
    people[labels[i]].push_back(i);

  chinese_whispers_add_remaining_faces_(people, visited_face, faces_size, num_clusters);

  return people;
}


std::vector<dlib::matrix<dlib::rgb_pixel>> face_recogniser::jitter_image_(const dlib::matrix<dlib::rgb_pixel>& img)
{
  thread_local dlib::random_cropper cropper;
  cropper.set_chip_dims(150,150);
  cropper.set_randomly_flip(true);
  cropper.set_max_object_height(0.99999);
  cropper.set_background_crops_fraction(0);
  cropper.set_min_object_height(0.97);
  cropper.set_translate_amount(0.02);
  cropper.set_max_rotation_degrees(3);

  std::vector<dlib::mmod_rect> raw_boxes(1), ignored_crop_boxes;
  raw_boxes[0] = dlib::shrink_rect(dlib::get_rect(img),3);
  std::vector<dlib::matrix<dlib::rgb_pixel>> crops;

  dlib::matrix<dlib::rgb_pixel> temp;
  for (int i = 0; i < 100; ++i)
  {
    cropper(img, raw_boxes, temp, ignored_crop_boxes);
    crops.push_back(std::move(temp));
  }

  return crops;
}


} // NAMESPACE facetools
