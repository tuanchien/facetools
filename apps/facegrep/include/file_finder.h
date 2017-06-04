#ifndef _FILE_FINDER_H_
#define _FILE_FINDER_H_


// ## INCLUDE #####################################################################################

#include <vector>
#include <string>
#include <ftw.h>
#include <fnmatch.h>


// ## NAMESPACE ###################################################################################

// ## CLASS DEFINITION ############################################################################

class file_finder {
public:
  /**
   * Finds all files matching the file name filters, in the search directory, including all subdirectories.
   * \param search_filter File name filters.
   * \param search_directory Search directory.
   * \return List of matching files.
   */
  static std::vector<std::string> find(std::vector<std::string> search_filter, std::string search_directory);


  /**
   * Finds all images (files with image filter suffix), in the search directory, including all subdirectories.
   * \param search_directory Search directory.
   * \return List of matching files.
   */
  static std::vector<std::string> find_images(std::string search_directory);


  /**
   * Callback for ftw API.
   * \param fpath Current matching file name.
   * \param sb Stat structure containing file info.
   * \param typeflag A file type/status flag of the current file.
   * \return State to ftw. 0 tells it to continue.
   */
  static int search_callback(const char *fpath, const struct stat *sb, int typeflag);


  /** Pre-defined list of file name matches to look for when looking for images. */
  static std::vector<std::string> image_search_filter_;


  /** User specified search filter. */
  static std::vector<std::string> search_filter_;


  /** List of files found. */
  static std::vector<std::string> files_found_;
};


#endif // _FILE_FINDER_H_