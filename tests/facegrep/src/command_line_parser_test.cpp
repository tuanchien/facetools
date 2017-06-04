/* Tests for the command_line_parser class.
 */

// ## INCLUDES ################################################################

#include <gtest/gtest.h>
#include <string>
#include <cstring>

#include <facegrep/command_line_parser.h>


// ## NAMESPACES ##############################################################

using namespace facetools;


// ## CONSTANTS ###############################################################

// ## PRIVATE METHODS #########################################################

// ## TESTS ###################################################################

TEST(command_line_parser, parse_basic)
{
  const int argcount = 3;
  char *argdata[argcount];
  argdata[0] = new char[100];
  argdata[1] = new char[100];
  argdata[2] = new char[100];

  memset(argdata[0], 0, 100);
  memset(argdata[1], 0, 100);
  memset(argdata[2], 0, 100);

  std::string progname = "./program";
  std::string face_file = "../test_data/facegrep/searchdir/bruce0.jpg";
  std::string directory = "../test_data/facegrep/searchdir";

  strncpy(argdata[0], progname.c_str(), 100);
  strncpy(argdata[1], face_file.c_str(), 100);
  strncpy(argdata[2], directory.c_str(), 100);

  auto params = command_line_parser::parse(argcount, (char**)argdata);
  EXPECT_EQ(params.search_directory, directory);
  EXPECT_EQ(params.face_file, face_file);

  delete argdata[0];
  delete argdata[1];
  delete argdata[2];
}


TEST(command_line_parser, parse_allopt)
{
  std::string progname = "./program";
  std::string face_file = "../test_data/facegrep/searchdir/bruce0.jpg";
  std::string directory = "../test_data/facegrep/searchdir";
  std::string mmod = "-m";
  std::string threshold = "-t 5";

  const int argcount = 5;
  char *argdata[argcount];

  argdata[0] = new char[100];
  argdata[1] = new char[100];
  argdata[2] = new char[100];
  argdata[3] = new char[100];
  argdata[4] = new char[100];

  memset(argdata[0], 0, 100);
  memset(argdata[1], 0, 100);
  memset(argdata[2], 0, 100);
  memset(argdata[3], 0, 100);
  memset(argdata[4], 0, 100);

  strncpy(argdata[0], progname.c_str(), 100);
  strncpy(argdata[1], mmod.c_str(), 100);
  strncpy(argdata[2], threshold.c_str(), 100);
  strncpy(argdata[3], face_file.c_str(), 100);
  strncpy(argdata[4], directory.c_str(), 100);

  auto params = command_line_parser::parse(argcount, argdata);
  EXPECT_EQ(params.search_directory, directory);
  EXPECT_EQ(params.face_file, face_file);
  EXPECT_TRUE(params.mmod);
  EXPECT_EQ(params.threshold, 5);

  delete argdata[0];
  delete argdata[1];
  delete argdata[2];
  delete argdata[3];
  delete argdata[4];
}