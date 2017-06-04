// ## INCLUDE #################################################################

#include <file_finder.h>
#include <file_utils.h>

#include <iostream>
#include <stdio.h>
#include <cstring>


// ## NAMESPACE ###############################################################

// ## STATIC MEMBER INSTANTIATION #############################################

std::vector<std::string> file_finder::image_search_filter_;
std::vector<std::string> file_finder::search_filter_;
std::vector<std::string> file_finder::files_found_;


// ## PUBLIC METHODS ##########################################################

std::vector<std::string> file_finder::find(std::vector<std::string> search_filter, std::string search_directory)
{
  if(!files_found_.empty())
    files_found_.clear();

  if(!dir_exists(search_directory))
  {
    std::cout << "ERROR: search directory not found.\n";
    return files_found_;
  }

  search_filter_ = search_filter;
  ftw(search_directory.c_str(), search_callback, FTW_F);

  return files_found_;
}


std::vector<std::string> file_finder::find_images(std::string search_directory)
{
  if(image_search_filter_.empty())
    image_search_filter_ = std::vector<std::string>({"*.jpg", "*.jpeg", "*.gif", "*.png"});

  return find(image_search_filter_, search_directory);
}


// ## PRIVATE METHODS #########################################################

int file_finder::search_callback(const char *fpath, const struct stat *sb, int typeflag)
{
  if(typeflag == FTW_F) {
    for(auto& filter : search_filter_) {
      if(fnmatch(filter.c_str(), fpath, FNM_CASEFOLD) == 0) {
        std::string name(fpath);
        files_found_.push_back(move(name));
        break;
      }
    }
  }

  return 0;
}