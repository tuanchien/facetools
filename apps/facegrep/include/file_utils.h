#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_


// ## INCLUDE #####################################################################################

#include <string>
#include <sys/stat.h>
#include <unistd.h>


// ## INLINE FUNCTIONS ############################################################################

/**
 * Check if a directory exists in the given path.
 * \param directory Directory to check.
 * \return True if exists, false otherwise.
 */
inline bool dir_exists(const std::string& directory)
{
  struct stat sb;
  return (stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode));
}


/**
 * Check if a file exists in the given path.
 * \param file File to check.
 * \return True if exists, false otherwise.
 */
inline bool file_exists(const std::string& file)
{
  struct stat buffer;
  return (stat (file.c_str(), &buffer) == 0);
}


#endif // _FILE_UTILS_H_