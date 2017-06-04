// ## INCLUDES ################################################################

#include <facetools/face_detector.h>
#include <facetools/face_recogniser.h>
#include <facegrep/command_line_parser.h>
#include <file_utils.h>
#include <file_finder.h>

#include <iostream>
#include <algorithm>


// ## NAMESPACES ##############################################################

using namespace facetools;


// ## DEFINES #################################################################

#define GLOBAL "/usr/local/share/facegrep/models/"
#define LOCAL "./"
#define FACE_DETECTOR_MODEL "mmod_human_face_detector.dat"
#define FACE_RECOGNITION_MODEL "dlib_face_recognition_resnet_model_v1.dat"
#define SHAPE_PREDICTOR_MODEL "shape_predictor_68_face_landmarks.dat"


// ## INLINE FUNCTIONS ########################################################

/**
 * Print the file list to standard output.
 * \param file_list File list to print.
 */
inline void print_file_list(std::vector<std::string> file_list)
{
  for(auto& entry: file_list)
    std::cout << entry << std::endl;
}

/**
 * Checks for the existence of local and system wide versions of a model file and assigns it to model parameter. Local
 * files take precedence. Print an error if no files found.
 * \param model Model parameter to set.
 * \param local_candidate The local candidate location.
 * \param global_candidate  The global candidate location.
 * \param error_message Error message to print if the file is not found in either location.
 */
inline void assign_model(std::string& model, const std::string& local_candidate, const std::string& global_candidate,
  const std::string& error_message)
{
  if(file_exists(local_candidate))
    model = local_candidate;
  else if(file_exists(global_candidate))
    model = global_candidate;
  else {
    std::cout << error_message << std::endl;
    exit(1);
  }
}


// ## PRIVATE FUNCTION DECLARATIONS ###########################################

/**
 * Initialise the facegrep parameter structure using the command line arguments.
 * \param cmd_params Command line parameters.
 * \param params Facegrep parameters to initialise.
 */
static void init_parameters(facegrep_commandline_parameters_t& cmd_params, facegrep_parameters_t& params);


// ## MAIN FUNCTION ###########################################################

/**
 * Facegrep main program execution entry point.
 * \param argc Command line argument count.
 * \param argv List of command line arguments.
 * \return 0 on success. Early exits return 1 to the shell.
 */
int main(int argc, char** argv)
{
  auto command_line_args = command_line_parser::parse(argc, argv);

  facegrep_parameters_t params;
  init_parameters(command_line_args, params);
  facegrep fg(params);
  fg.init(command_line_args.face_file);

  auto image_files = file_finder::find_images(command_line_args.search_directory);
  auto results = fg.search(image_files);
  std::sort(results.begin(), results.end());
  print_file_list(results);

  return 0;
}


// ## PRIVATE FUNCTION DEFINITIONS ############################################

static void init_parameters(facegrep_commandline_parameters_t& cmd_params, facegrep_parameters_t& params)
{
  params.jitter = cmd_params.jitter;
  params.threshold = cmd_params.threshold;

  if(cmd_params.mmod)
    params.detector_type = face_detector_type_t::MMOD;
  else
    params.detector_type = face_detector_type_t::DLIB_DEFAULT;

  const std::string local_detector_model = LOCAL FACE_DETECTOR_MODEL;
  const std::string global_detector_model = GLOBAL FACE_DETECTOR_MODEL;
  const std::string local_recogniser_model = LOCAL FACE_RECOGNITION_MODEL;
  const std::string global_recogniser_model = GLOBAL FACE_RECOGNITION_MODEL;
  const std::string local_shape_model = LOCAL SHAPE_PREDICTOR_MODEL;
  const std::string global_shape_model = GLOBAL SHAPE_PREDICTOR_MODEL;

  assign_model(params.detector_model, local_detector_model, global_detector_model,
    "No face detector model file found.");

  assign_model(params.recogniser_model, local_recogniser_model, global_recogniser_model,
    "No face recogniser model file found.");

  assign_model(params.shape_model, local_shape_model, global_shape_model, "No shape model file found.");
}