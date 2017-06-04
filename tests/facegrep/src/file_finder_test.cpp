/* Tests for the file_finder class.
 */

// ## INCLUDES ################################################################

#include <gtest/gtest.h>
#include <string>

#include "file_finder.h"


// ## NAMESPACES ##############################################################

// ## CONSTANTS ###############################################################
static const char SEARCH_DIR[] = "../test_data/facegrep/searchdir";

// ## PRIVATE METHODS #########################################################

// ## TESTS ###################################################################

TEST(file_finder, find)
{
  std::vector<std::string> filter;
  std::string filter_string = "*.txt";
  filter.push_back(filter_string);
  auto found = file_finder::find(filter, "../test_data/facegrep/file_finder");
  EXPECT_EQ(found.size(), 1);
}


TEST(file_finder, find_images)
{
  auto found = file_finder::find_images("../test_data/facegrep/file_finder");
  EXPECT_EQ(found.size(), 2);
}


TEST(file_finder, find_images_searchdir)
{
  auto found = file_finder::find_images(SEARCH_DIR);
  EXPECT_EQ(found.size(), 8);
}