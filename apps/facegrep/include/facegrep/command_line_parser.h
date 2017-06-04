#ifndef _FACEGREP_COMMAND_LINE_PARSER_H_
#define _FACEGREP_COMMAND_LINE_PARSER_H_

// ## INCLUDE #################################################################

#include <getopt.h>
#include <string>

#include <facegrep/facegrep.h>


// ## NAMESPACE ###############################################################

namespace facetools {


// ## CUSTOM STRUCTURES #######################################################

/**
 * Command line parameters.
 */
struct facegrep_commandline_parameters_t {
  /** Name of face file we're searching for. */
  std::string face_file;

  /** Whether to jitter images. */
  bool jitter;

  /** Whether to use mmod for face detection. */
  bool mmod;

  /** Directory to search for images. */
  std::string search_directory;

  /** Similarity threshold. Default is 0.6. */
  float threshold;

facegrep_commandline_parameters_t() {
  search_directory = ".";
  jitter = true;
  mmod = false;
  threshold = 0.6;
  }
};


/** Options for getopt. */
static struct option command_line_options[] = {
  {"jitter", no_argument, 0, 'j'},
  {"mmod", no_argument, 0, 'm'},
  {"threshold", required_argument, 0, 't'},
  {0, 0, 0, 0}
};


// ## CLASS DEFINITION ########################################################

/**
 * A command line parsing class based on getopt.
 */
class command_line_parser {
public:
  /**
   * Parses the command line for recognised options.
   * \param argc Number of arguments passed.
   * \param argv An array of arrays, containing the list of arguments.
   * \return Facegrep command line parameters.
   */
  static facegrep_commandline_parameters_t parse(int argc, char** argv);


  /**
   * Print usage to standard output if there was an error in parsing.
   * \param argv The list of command line arguments. Only the program name will be used for helping the user decide
   * how to pass arguments to the program.
   */
  static void print_usage(char** argv);
};

} // NAMESPACE facetools

#endif // _FACEGREP_COMMAND_LINE_PARSER_H_