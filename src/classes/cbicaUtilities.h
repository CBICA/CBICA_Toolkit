/*!
 * \file  sbiaUtilities.h
 * \brief Some basic utility functions.
 *

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html.

 */
#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iterator>

/**

*/
class BadConversion : public std::runtime_error 
{
public:
  BadConversion(const std::string& s)
    : std::runtime_error(s)
    { }
};

namespace cbica
{

  bool fileExists(const std::string &fName);
  bool directoryExists(const std::string &dName);
  int createTmpDir(const std::string &prefix,  std::string &retDir);
  void createDir(const std::string &dir_name);
  int removeDirectoryRecursively(const char *dirname);
  int splitFileName(const std::string &dataFile, std::string &path,
     std::string &baseName, std::string &extension);

  /**
  \brief Searches for an element in a vector and returns true/false and position
  */
  template<typename TContainerType>
  bool findInVector( std::vector<TContainerType> vector_to_search_in, 
    TContainerType element_to_search_for, int position )
  {
    position = -1;
    //std::vector<int>::const_iterator
    typename std::vector<TContainerType>::const_iterator iterator = 
      std::find(vector_to_search_in.begin(), vector_to_search_in.end(), element_to_search_for) ;
    if( iterator !=  vector_to_search_in.end() )
    {
      position = iterator - vector_to_search_in.begin();
      return true;
    }
    else
      return false;
  }

  std::string getExeName();
  std::string getUserName();

  /**
  \brief Convert character to integer, double, unsigned int, etc.

  \param input_string Input character to be convereted
  \return Templated to the type of return required
  */
  template<typename TConvertType>
  void convertString( const std::string &input_string, TConvertType output )
  {
    bool failIfLeftoverChars = true;
    std::istringstream i(input_string);
    char c;
    if (!(i >> output) || (failIfLeftoverChars && i.get(c)))
      throw BadConversion("conversionError(\"" + input_string + "\")");
  }
   
  /**
  \brief Splits the string
  */
  inline void
  stringSplit( std::string str, const std::string delim, std::vector<std::string> *results )
  {
    int cutAt;
    while( (cutAt = static_cast<int>(str.find_first_of(delim))) != static_cast<int>(str.npos) )
    {
      if(cutAt > 0)
      {
        (*results).push_back(str.substr(0,cutAt));
      }
      str = str.substr(cutAt+1);
    }
    if(str.length() > 0)
    {
      (*results).push_back(str);
    }
  }

};
