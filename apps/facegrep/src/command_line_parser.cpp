// ## INCLUDE #################################################################

#include <stdlib.h>
#include <iostream>
#include <string.h>

#include <facegrep/command_line_parser.h>
#include <file_utils.h>


// ## NAMESPACE ###############################################################

namespace facetools {


// ## PUBLIC METHODS ##########################################################

facegrep_commandline_parameters_t command_line_parser::parse(int argc, char** argv)
{
  facegrep_commandline_parameters_t params;

  while(true) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "jmt:", command_line_options, &option_index);

    if(c == -1)
      break;

    switch(c)
    {
      case 'j':
        params.jitter = true;
        break;
      case 'm':
        params.mmod = true;
        break;
      case 't':
        params.threshold = std::stof(optarg);
        break;
      default:
        print_usage(argv);
    }
  }

  if(optind < argc-1)
  {
    params.face_file = argv[optind];
    params.search_directory = argv[optind+1];
  }
  else
    print_usage(argv);

  if(!file_exists(params.face_file) || !dir_exists(params.search_directory))
    print_usage(argv);

  return params;
}


void command_line_parser::print_usage(char** argv)
{
  std::string program_name = argv[0];
  std::cout << "Usage: " << program_name + " [options] <face file> <search directory>\n" <<
    "Options:\n"
    "  -j or --jitter\t Apply jitter averaging. Makes it more robust.\n"
    "  -m or --mmod\t\t Uses the max marginal object face detection method.\n"
    "              \t\t Slow but more accurate [Recommended if you have powerful GPU].\n"
    "  -t or --threshold\t Distance threshold to use for determining face similarity. Default: 0.6.\n"
  ;

  exit(1);
}


} // NAMESPACE facetools