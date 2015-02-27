/*!
 * \file  sbiaBasicUtilities.cxx
 * \brief Some basic utility functions.
 *

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html.

 */
#pragma once

#include <io.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#if (_WIN32)
	#include <direct.h>
  #include <windows.h>
  #include "windows/dirent.h"
  #include <lmcons.h>
  #define GetCurrentDir _getcwd
  #define WindowsDetected = true
#else
  #include <dirent.h>
  #include <unistd.h>
  #include <libgen.h>
  #include <limits.h>
  #define GetCurrentDir getcwd
  #define WindowsDetected = false
#endif

#include "cbicaUtilities.h"

namespace cbica
{
  /**
  \brief Check if file exists

  \param fName String to check
  \return true if file exists
  */
  bool fileExists( const std::string &fName )
  {
    std::ifstream file_exists(fName.c_str());
    if(file_exists.good()) // file exists, then append
      return true;
    else
      return false;
  }
  
  /**
  \brief Check if directory exists

  \param dName String to check
  \return 1 if directory
  */
  bool directoryExists( const std::string &dName )
  {
    struct stat st;
    stat(dName.c_str(), &st);
    
    if( st.st_mode & S_IFDIR )
      return false;
    else
      return true;
  }
  
  /**
  \brief Create a temporary directory

  \param prefix Folder to be created
  \param retDir Check if prefix == retDir

  \return redir Path of created folder
  */
  int createTmpDir(const std::string &prefix,  std::string &retDir)
  {

    char *tmp;
#if (_WIN32)
    _dupenv_s( &tmp, NULL, "SBIA_TMPDIR" );
#else
    tmp = getenv ("SBIA_TMPDIR");
#endif
  
    //Build the path
    char tempPath[FILENAME_MAX];
    if (tmp==NULL)
      sprintf_s(tempPath, static_cast<size_t>(FILENAME_MAX), "/tmp");
    else
      sprintf_s(tempPath, static_cast<size_t>(FILENAME_MAX), "%s", tmp);

#if (_WIN32)
    strcat_s(tempPath,"/");
    strcat_s(tempPath,prefix.c_str());
    strcat_s(tempPath,"_XXXXXX");
#else
    strcat(tempPath,"/");
    strcat(tempPath,prefix.c_str());
    strcat(tempPath,"_XXXXXX");
#endif

    int size_tempPath = sizeof(tempPath);
    if( _mktemp_s(tempPath, size_tempPath) == 0 )
    {
      retDir = std::string(tempPath);
      return 0;
    }
    /*
    char *newDir=_mktemp(tempPath);
    if (newDir != NULL)
    {
      retDir = std::string(newDir);
      return 0;
    }
    */
    return 1;
  }

  /**
  \brief Create a directory

  \param dir_name Name of directory to be created with full path
  */
  void createDir( const std::string &dir_name )
  {
      //! Pure c++ based directory creation
    #if defined(_WIN32)
      DWORD ftyp = GetFileAttributesA(dir_name.c_str()); // check if directory exists or not
      if (ftyp == INVALID_FILE_ATTRIBUTES)
    	  _mkdir(dir_name.c_str());
    #else
      DIR *pDir;
      pDir = opendir(dir_name.c_str()) // check if directory exists or not
      if (pDir == NULL)
    	  mkdir(dir_name.c_str(), 0777);
    #endif
  }

  /**
  \brief Recursively delete a folder and contents

  \param dirname Folder to delete

  \return 0 for success
  */
  int removeDirectoryRecursively(const char *dirname)
  {
      DIR *dir;
      struct dirent *entry;
      char path[FILENAME_MAX];

      dir = opendir(dirname);
      if (dir == NULL) {
          perror("Error opendir()");
          return 1;
      }

      while ((entry = readdir(dir)) != NULL) {
          if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            sprintf_s(path, static_cast<size_t>(FILENAME_MAX), "%s/%s", dirname, entry->d_name);
              if (entry->d_type == DT_DIR) {
                  removeDirectoryRecursively(path);
              }
              //printf("(not really) Deleting: %s\n", path);
              remove(path);
          }

      }
      closedir(dir);
      // Now the directory is empty, finally delete the directory itself.
      //printf("(not really) Deleting: %s\n", dirname);
      remove(dirname);
    
      return 0;
  }
  
  /**
  \brief Splits the input file name into its constituents

  \param dataFile The full file name which is the input
  \param baseName Overwritten with file name without extension
  \param extension Overwritten with extension
  \param path Overwritten with path to file

  \return 0 if successfull
  */
  int splitFileName( const std::string &dataFile, std::string &path,
     std::string &baseName, std::string &extension )
  {
    #if defined(_WIN32)
    	//! Initialize pointers to file and user names
      char basename[MAX_PATH], ext[MAX_PATH], path_name[MAX_PATH];
      _splitpath_s(dataFile.c_str(), NULL, NULL, path_name, NULL, basename, NULL, ext, NULL);
    #else
    	//! Initialize pointers to file and user names
    	char *basename, *ext, *path_name;
      path_name = dirname(dataFile);
    	basename = basename(dataFile);
      ext = strrchr(dataFile);
    #endif
      
    path = std::string(path_name);
    baseName = std::string(basename);
    extension = std::string(ext);

    path_name[0] = '\0';
    basename[0] = '\0';
    ext[0] = '\0';
    /*
    extension = "";
    baseName = "";
    path  = "";
    std::string::size_type extPos = dataFile.rfind(".");
    if ( extPos != std::string::npos )
    {
      extension  = dataFile.substr(extPos,dataFile.length() - extPos);
      std::string tmp = dataFile.substr(0,extPos);
      std::string::size_type pathPos = dataFile.rfind("/");
      if ( pathPos != std::string::npos )
      {
        path  = tmp.substr(0,pathPos)+"/";
        baseName = tmp.substr(pathPos,tmp.length()-pathPos);
      }
      else
      {
         path = "./";
         baseName = tmp;
      }
      // Check if we have a .nii.gz extension
      extPos = baseName.rfind(".");
      if ( extPos != std::string::npos )
      {
        std::string extension2 = baseName.substr(extPos,baseName.length() - extPos);
        if (extension2.compare(".nii") == 0)
        {
          extension = extension2 + extension;
          baseName = baseName.substr(0,extPos);
        }
      }
    }
    else
    {
      std::cerr << "Can't find extension!!!!" << std::endl;  
      return EXIT_FAILURE;
    }*/
    return EXIT_SUCCESS;
  }

  /**
  \brief Get the name of the Executable which is calling the function

  \return exe name
  */
  std::string getExeName()
  {
    #if defined(_WIN32)
    	//! Initialize pointers to file and user names
    	char filename[MAX_PATH];
      _splitpath_s(filename, NULL, NULL, NULL, NULL, filename, NULL, NULL, NULL);
    #else
    	//! Initialize pointers to file and user names
    	char *filename, filename_2[PATH_MAX];    
    	::readlink("/proc/self/exe", filename_2, sizeof(filename_2)-1);
    	filename = basename(filename_2);
    #endif

    std::string return_string = std::string(filename);
    filename[0] = '\0';
    
    return return_string;
  }
  
  /**
  \brief Get the name of the user who is calling the function

  \return user name
  */
  std::string getUserName()
  {
    #if defined(_WIN32)
    	//! Initialize pointers to file and user names
    	char username[MAX_PATH];
    	DWORD username_len = MAX_PATH;
    	GetUserName(username, &username_len);
    #else
    	//! Initialize pointers to file and user names
    	char username[PATH_MAX];
    	size_t username_len = PATH_MAX;
    	getlogin_r(username, username_len);
    #endif
      
    std::string return_string = std::string(username);
    username[0] = '\0';
    
    return return_string;
  }

}