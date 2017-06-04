// ## INCLUDE #################################################################

#include <facegrep/facegrep.h>
#include <facetools/error.h>


// ## NAMESPACE ###############################################################

namespace facetools {


// ## PUBLIC METHODS ##########################################################

facegrep::facegrep(const facegrep_parameters_t& params)
{
  require_true(params.threshold > 0, "facegrep: threshold < 0");
  params_.threshold = params.threshold;

  face_detector_parameters_t detector_params;
  detector_params.detector_type = params.detector_type;

  if(!params.detector_model.empty())
    detector_params.face_detector_model_file = params.detector_model;

  if(!params.shape_model.empty())
    detector_params.shape_predictor_model_file = params.shape_model;

  detector_ = std::make_unique<face_detector>(detector_params);

  face_recogniser_parameters_t recogniser_params;
  recogniser_params.jitter_images = params.jitter;
  recogniser_params.face_difference_threshold = params.threshold;

  if(!params.recogniser_model.empty())
    recogniser_params.recogniser_model_file = params.recogniser_model;

  recogniser_ = std::make_unique<face_recogniser>(recogniser_params);

  initialised_ = false;
}

void facegrep::init(const std::string face_template_file)
{
  auto faces = detector_->extract_faces(face_template_file);
  auto faces_found = faces.size();

  if(!faces_found)
  {
    std::cout << "facegrep: No faces found in face template image.\n";
    initialised_ = false;
    return;
  }
  else if(faces_found > 1)
  {
    std::cout << "facegrep: WARNING! More than one face found in template image. Using the first face found.\n";
  }

  template_embedding_ = recogniser_->get_embedding(faces[0]);

  initialised_ = true;
}


std::vector<std::string> facegrep::search(const std::vector<std::string> image_files)
{
  std::vector<std::string> files_found;
  for(auto& image_file : image_files)
  {
    auto faces = detector_->extract_faces(image_file);
    auto embeddings = recogniser_->get_embedding(faces);
    for(auto& candidate : embeddings)
      if(face_matched_(template_embedding_, candidate))
        files_found.push_back(image_file);
  }

  return files_found;
}


std::vector<std::string> facegrep::search(const std::string face_template_file, const std::vector<std::string> image_files)
{
  std::vector<std::string> files_found;

  init(face_template_file);

  if(initialised_)
    files_found = search(image_files);

  return files_found;
}


bool facegrep::face_matched_(const embedding_t& face1, const embedding_t& face2)
{
    return (dlib::length(face1-face2) < params_.threshold);
}


} // NAMESPACE facetools