/**
\file  cbicaUtilities.cpp

\brief Some basic utility functions.

Dependecies: OpenMP

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

\author Sarthak Pati

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#if (_WIN32)
  #define NOMINMAX
	#include <direct.h>
  #include <windows.h>
  #include <conio.h>
  #include <lmcons.h>
  #include <Shlobj.h>
  #define GetCurrentDir _getcwd
  bool WindowsDetected = true;
  static const char  cSeparator  = '\\';
//  static const char* cSeparators = "\\/";
#else
  #include <dirent.h>
  #include <unistd.h>
  #include <libgen.h>
  #include <limits.h>
  #include <cstring>
  #include <cstdlib>
  #include <sys/types.h>
  #include <errno.h>
  #define GetCurrentDir getcwd
  bool WindowsDetected = false;
  static const char  cSeparator  = '/';
//  static const char* cSeparators = "/";
#endif

#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <omp.h>

#include "cbicaUtilities.h"

namespace cbica
{
  //====================================== Folder stuff ====================================//

  bool fileExists( const std::string &fName )
  {
    std::ifstream file_exists(fName.c_str());
    if( file_exists.good() )
      return true;
    else
      return false;
  }
  
  bool directoryExists( const std::string &dName )
  {
    struct stat info;
    std::string dName_Wrap = dName;

    if (dName_Wrap[dName_Wrap.length() - 1] == '/')
    {
      dName_Wrap.erase(dName_Wrap.end() - 1);
    }
    
    if (stat(dName_Wrap.c_str(), &info) != 0)
      return false;
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on windows
      return true;
    else
      return false;
  }
  
  bool isFile(const std::string &path)
  {
    return cbica::fileExists(path);
  }

  bool isDir(const std::string &path)
  {
    return cbica::directoryExists(path);
  }

  bool exists(const std::string &path)
  {
    struct stat info;
    
    if( stat( path.c_str(), &info ) != 0 )
        return false;
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
        return true;
    else
        return true;
  }

  bool createTmpDir(std::string &returnDir)
  {
    char *tmp;
    char tempPath[FILENAME_MAX];
#if defined(_WIN32)
    tmp = getenv("USERPROFILE");
    std::string temp = cbica::replaceString(tmp, "\\", "/");
    sprintf_s(tempPath, static_cast<size_t>(FILENAME_MAX), "%s", temp.c_str());
    strcat_s(tempPath,"/tmp/");
#else
      tmp = std::getenv("HOME");
      sprintf(tempPath, "%s", tmp);
      strcat(tempPath,"/tmp/");
#endif    

    returnDir = std::string(tempPath);
    tmp[0] = '\0';
    tempPath[0] = '\0';
    //std::cout << "temp_dir: " << returnDir <<std::endl;
    return createDir(returnDir);
  }
  
  bool createTemporaryDirectory( std::string &returnDir )
  {
    return createTmpDir(returnDir);
  }

  bool makeTemporaryDirectory( std::string &returnDir )
  {
    return createTmpDir(returnDir);
  }

  bool makeTempDir( std::string &returnDir )
  {
    return createTmpDir(returnDir);
  }

  bool createDir( const std::string &dir_name )
  {
      //! Pure c++ based directory creation
    #if defined(_WIN32)
      DWORD ftyp = GetFileAttributesA(dir_name.c_str()); // check if directory exists or not
      if (ftyp == INVALID_FILE_ATTRIBUTES)
    	  _mkdir(dir_name.c_str());
      return true;
    #else
      DIR *pDir;
      pDir = opendir(dir_name.c_str()); // check if directory exists or not
      if (pDir == NULL)
    	  mkdir(dir_name.c_str(), 0777);
      return true;
    #endif
    return false;
  }

  bool makeDir(const std::string &dir_name)
  {
    return createDir( dir_name );
  }

  int removeDirectoryRecursively(const std::string &dirname, bool bDeleteSubdirectories = true)
  {
    if (!directoryExists(dirname))
    {
      std::cerr << "Supplied directory name wasn't found.\n";
      exit(EXIT_FAILURE);
    }
    #if defined(_WIN32)
      bool bSubdirectory = false;       // Flag, indicating whether
                                        // subdirectories have been found
      HANDLE hFile;                     // Handle to directory
      std::string strFilePath;          // Filepath
      std::string strPattern;           // Pattern
      WIN32_FIND_DATA FileInformation;  // File information    
    
      strPattern = dirname + "/*.*";
      hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
      if(hFile != INVALID_HANDLE_VALUE)
      {
        do
        {
          if(FileInformation.cFileName[0] != '.')
          {
            strFilePath.erase();
            strFilePath = dirname + "/" + FileInformation.cFileName;
    
            if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
              if(bDeleteSubdirectories)
              {
                // Delete subdirectory
                int iRC = cbica::removeDirectoryRecursively(strFilePath, bDeleteSubdirectories);
                if(iRC)
                  return iRC;
              }
              else
                bSubdirectory = true;
            }
            else
            {
              // Set file attributes
              if(::SetFileAttributes(strFilePath.c_str(),
                                     FILE_ATTRIBUTE_NORMAL) == FALSE)
                return ::GetLastError();
    
              // Delete file
              if(::DeleteFile(strFilePath.c_str()) == FALSE)
                return ::GetLastError();
            }
          }
        } while(::FindNextFile(hFile, &FileInformation) == TRUE);
    
        // Close handle
        ::FindClose(hFile);
    
        DWORD dwError = ::GetLastError();
        if(dwError != ERROR_NO_MORE_FILES)
          return dwError;
        else
        {
          if(!bSubdirectory)
          {
            // Set directory attributes
            if(::SetFileAttributes(dirname.c_str(),
                                   FILE_ATTRIBUTE_NORMAL) == FALSE)
              return ::GetLastError();
    
            // Delete directory
            if(::RemoveDirectory(dirname.c_str()) == FALSE)
              return ::GetLastError();
          }
        }
      }
    
      return 0;
    #else   
    std::string passString = "rm -r " + dirname;
    system(passString.c_str());
    #endif
      return 0;
  }

  bool removeDir(const std::string &path)
  {
    if( removeDirectoryRecursively(path, true) != 0 )
      return false;
    return true;
  }
  
  bool deleteDir(const std::string &path)
  {
    if( removeDirectoryRecursively(path, true) != 0 )
      return false;
    return true;
    //return removeDirectoryRecursively(path);
    /*#if defined(_WIN32)
    if (_rmdir(path.c_str()) == -1)
      return false;
    #else
    std::string passString = "rm -r " + path;
    system(passString.c_str());
    #endif*/

    return true;
  }

  //======================================== OS stuff ======================================//
  
  std::string getFilenameBase(const std::string &filename, bool checkFile)
  {
    if (checkFile)
    {
      if (!fileExists(filename))
      {
        std::cerr << "Supplied file name wasn't found.\n";
        exit(EXIT_FAILURE);
      }
    }
    std::string path, base, ext;
    splitFileName(filename, path, base, ext);
    
    return base;
	}
	
  std::string getFilenameExtension(const std::string &filename, bool checkFile)
  {
    if (checkFile)
    {
      if (!fileExists(filename))
      {
        std::cerr << "Supplied file name wasn't found.\n";
        exit(EXIT_FAILURE);
      }
    }
    std::string path, base, ext;
    splitFileName(filename, path, base, ext);
    
    return ext;
  }

  std::string getFilenamePath(const std::string &filename, bool checkFile)
  {
    if (checkFile)
    {
      if (!fileExists(filename))
      {
        std::cerr << "Supplied file name wasn't found.\n";
        exit(EXIT_FAILURE);
      }
    }
    std::string path, base, ext;
    splitFileName(filename, path, base, ext);
    
    return path;
  }

  std::string getExecutableName()
  {
    #if defined(_WIN32)
    	//! Initialize pointers to file and user names
    	char filename[FILENAME_MAX];
	    GetModuleFileNameA(NULL, filename, FILENAME_MAX);
	    _splitpath(filename, NULL, NULL, filename, NULL);
      //_splitpath_s(filename, NULL, NULL, NULL, NULL, filename, NULL, NULL, NULL);
    #else
    	//! Initialize pointers to file and user names
    	char *filename, filename_2[FILENAME_MAX];    
    	::readlink("/proc/self/exe", filename_2, sizeof(filename_2)-1);
    	filename = basename(filename_2);
    #endif

    std::string return_string = std::string(filename);
    filename[0] = '\0';
    
    return return_string;
  }
  
  std::string getFullPath()
  {
    #if defined(_WIN32)
    	//! Initialize pointers to file and user names
    	char path[FILENAME_MAX];
	    GetModuleFileNameA(NULL, path, FILENAME_MAX);
      //_splitpath_s(filename, NULL, NULL, NULL, NULL, filename, NULL, NULL, NULL);
    #else
    	//! Initialize pointers to file and user names
    	char path[PATH_MAX];    
    	::readlink("/proc/self/exe", path, sizeof(path)-1);
    #endif

    std::string return_string = std::string(path);
    path[0] = '\0';
    
    return return_string;
  }
  
  std::string getUserName()
  {
    #if defined(_WIN32)
    	//! Initialize pointers to file and user names
    	char username[FILENAME_MAX];
    	DWORD username_len = FILENAME_MAX;
    	GetUserName(username, &username_len);
    #else
    	//! Initialize pointers to file and user names
    	char username[FILENAME_MAX];
    	size_t username_len = FILENAME_MAX;
    	getlogin_r(username, username_len);
    #endif
      
    std::string return_string = std::string(username);
    username[0] = '\0';
    
    return return_string;
  }

  std::string getCWD()
  {
    std::string wd = "";
    
    // use c++ convention
    char* buffer = GetCurrentDir(NULL, 0);
    
    if( buffer ) 
    {
      wd = buffer;
      buffer[0] = '\0';
    }
    wd = replaceString(wd, "\\", "/");
    if (wd[wd.length()-1] != '/')
    {
      wd.append("/");
    }

    return wd;
  }

  //! Check for separators [Internal_Function]
  inline bool issep( char c )
  {
    #if defined(_WIN32)
        return c == '/' || c == '\\';
    #else
        return c == '/';
    #endif
  }

  //! Check for absolute path [Internal_Function]
  bool isabs( const std::string& path )
  {
    size_t i = 0;
    #if defined(_WIN32)
        if (path.size() > 1 && path[1] == ':') i = 2;
    #endif
    return i < path.size() && issep(path[i]);
  }
  
  //! Joins separators based on OS [Internal_Function]
  std::string join( const std::string& base, const std::string& path )
  {
    if( base.empty() || isabs(path) )  
      return path;
    if( issep(base[base.size() - 1]) ) 
      return (base + path);

    #if defined(_WIN32)
        return base + '\\' + path;
    #else
        return base + '/' + path;
    #endif
  }

  std::string normPath( const std::string &path )
  {
    if( path.empty() ) 
      return "";
    char drive[3] = {'\0', ':', '\0'};
    size_t i = 0;
    #if defined(_WIN32)
      if( path.size() > 1 && path[1] == ':' ) 
      {
        drive[0] = path[0];
        i = 2;
      }
    #endif
    std::string norm_path = drive;
    bool abs = issep(path[i]);
    if( abs ) 
    {
      #if defined(_WIN32)
        while( i <= path.size() && issep(path[i]) ) 
        {
          norm_path += cSeparator;
          i++;
        }
      #else
        norm_path += cSeparator;
      #endif
    }
    std::string current;
    std::vector<std::string> parts;
    while( i <= path.size() ) 
    {
      if( issep(path[i]) || path[i] == '\0' )
      {
        if( current == ".." ) 
        {
          if( !abs && (parts.empty() || parts.back() == "..") ) 
          {
            parts.push_back(current);
          } 
          else if( !parts.empty() )
          {
            parts.pop_back();
          }
        } 
        else if( current != "" && current != "." ) 
        {
          parts.push_back(current);
        }
        current.clear();
      } 
      else 
      {
        current += path[i];
      }
      i++;
    }
    for( i = 0; i < parts.size(); i++ ) 
    {
      norm_path = join(norm_path, parts[i]);
    }
    return norm_path.empty() ? "." : norm_path;
  }

  std::string normalizePath( const std::string &path )
  {
    return normPath(path);
  }

  //! Absolute path [Internal_Function]
  std::string absPath(const std::string &path)
  {
    return normPath( join(getCWD(),path) );
  }

  //! Split Drive name from path [Internal_Function]
  inline void splitDrive( const std::string& path, std::string& drive, std::string& tail )
  {
    #if defined(_WIN32)
      if( path.size() > 1 && path[1] == ':' ) 
      {
          tail  = path.substr(2);
          drive = path[0]; drive += ':';
      }
      else
    #endif
      {
        tail  = path;
        drive = "";
      }
  }

  //! Split Drive name from path [Internal_Function]
  inline std::vector<std::string> splitDrive( const std::string& path )
  {
    std::vector<std::string> parts(2, "");
    splitDrive(path, parts[0], parts[1]);
    return parts;
  }

  std::string relPath( const std::string &path, const std::string &base )
  {
    // if relative path is given just return it
    if( !isabs(path) ) 
      return path;
    // normalize paths
    std::string norm_path = normPath(path);
    std::string norm_base = normPath(join(getCWD(), base));
    // check if paths are on same drive
    #if defined(_WIN32)
      std::string drive = splitDrive(norm_path)[0];
      std::string base_drive = splitDrive(norm_base)[0];
      if( drive != base_drive ) 
      {
        std::cerr << "Error: Path is on drive " << drive << ", start is on drive " << base_drive;
      }
    #endif
    // find start of first path component in which paths differ
    std::string::const_iterator b = norm_base.begin();
    std::string::const_iterator p = norm_path.begin();
    size_t pos = 0;
    size_t i   = 0;
    while( b != norm_base.end() && p != norm_path.end() ) 
    {
      if( issep(*p) ) 
      {
        if( !issep(*b) ) 
          break;
        pos = i;
      } 
      else if( *b != *p ) 
      {
        break;
      }
      b++; p++; i++;
    }
    // set pos to i (in this case, the size of one of the paths) if the end
    // of one path was reached, but the other path has a path separator
    // at this position, this is required below
    if( (b != norm_base.end() && issep(*b)) || (p != norm_path.end() && issep(*p)) ) 
        pos = i;
    // skip trailing separator of other path if end of one path reached
    if( b == norm_base.end() && p != norm_path.end() && issep(*p) ) 
      p++;
    if( p == norm_path.end() && b != norm_base.end() && issep(*b) ) 
      b++;
    // if paths are the same, just return a period (.)
    //
    // Thanks to the previous skipping of trailing separators, this condition
    // handles all of the following cases:
    //
    //    base := "/usr/bin"  path := "/usr/bin"
    //    base := "/usr/bin/" path := "/usr/bin/"
    //    base := "/usr/bin"  path := "/usr/bin/"
    //    base := "/usr/bin/" path := "/usr/bin"
    if( b == norm_base.end() && p == norm_path.end() ) 
      return ".";
    // otherwise, pos is the index of the last slash for which both paths
    // were identical; hence, everything that comes after in the original
    // path is preserved and for each following component in the base path
    // a "../" is prepended to the relative path
    std::string rel_path;
    // truncate base path with a separator as for each "*/" path component,
    // a "../" will be prepended to the relative path
    if( b != norm_base.end() && !issep(norm_base[norm_base.size() - 1]) ) 
    {
      // attention: This operation may invalidate the iterator b!
      //            Therefore, remember position of iterator and get a new one.
      size_t pos = b - norm_base.begin();
      norm_base += cSeparator;
      b = norm_base.begin() + pos;
    }
    while( b != norm_base.end() ) 
    {
      if( issep(*b) ) 
      {
        rel_path += "..";
        rel_path += cSeparator;
      }
      b++;
    }
    if( pos + 1 < norm_path.size() ) 
      rel_path += norm_path.substr(pos + 1);
    // remove trailing path separator
    if( issep(rel_path[rel_path.size() - 1]) ) 
    {
        rel_path.erase(rel_path.size() - 1);
    }
    return rel_path;
  }

  std::string relativePath(const std::string &path, const std::string &base)
  {
    return relPath(path, base);
  }

  std::string realPath( const std::string &path )
  {
    std::string curr_path = join(getCWD(), path);
    #if defined(_WIN32)
	  // nothing extra required
    #else
      char *actualPath = realpath(const_cast<char *>(curr_path.c_str()), NULL);
      curr_path = std::string(actualPath);
/*
      // use stringstream and std::getline() to split absolute path at slashes (/)
      std::stringstream ss(curr_path);
      curr_path.clear();
      std::string fname;
      std::string prev_path;
      std::string next_path;
      char slash;
      ss >> slash; // root slash
      while( getline(ss, fname, '/') ) 
      {
        // current absolute path
        curr_path += '/';
        curr_path += fname;
        // if current path is a symbolic link, follow it
        if( isLink(curr_path) ) 
        {
          // for safety reasons, restrict the depth of symbolic links followed
          for( unsigned int i = 0; i < 100; i++ ) 
          {
		    char *buffer=NULL, *newbuf=NULL;
            size_t buflen = 256;
            for(;;)
            {
              newbuf = reinterpret_cast<char*>(realloc(buffer, buflen * sizeof(char)) );
              if( !newbuf )
                break;
              buffer = newbuf;
              int n = ::newlink(path.c_str(), buffer, buflen);
              if( n<0 )
                break;
              if( static_cast<size_t>(n)<buflen )
              {
                buffer[n] = '\0';
                next_path = buffer;
                break;
              }
              buflen+=256;
            }
            free(buffer);
            //next_path = os::readlink(curr_path);
            if( next_path.empty() ) 
            {
              // if real path could not be determined because of permissions
              // or invalid path, return the original path
              break;
            } 
            else 
            {
              curr_path = join(prev_path, next_path);
              if( !isLink(next_path) ) 
                break;
            }
          }
          // if real path could not be determined with the given maximum number
          // of loop iterations (endless cycle?) or one of the symbolic links
          // could not be read, just return original path as absolute path
          if( isLink(next_path) ) 
            return absPath(path);
        }
        // memorize previous path used as base for abspath()
        prev_path = curr_path;
      }
*/
    #endif
    // normalize path after all symbolic links were resolved
    return normPath(curr_path);
  }

  bool isLink(const std::string &path)
  {
    #if defined(_WIN32)
      std::cerr << "Windows doesn't support ways to distinguish between hard and soft links.\n";
      return false;
    #else
      struct stat info;
      if (lstat(path.c_str(), &info) != 0) 
        return false;
      return S_ISLNK(info.st_mode);
    #endif
  }

  bool isSymbolicLink(const std::string &path)
  {
    return isLink(path);
  }

  bool makeSymbolicLink(const std::string &input_fileName, const std::string &ouput_fileName)
  {
    if (!fileExists(input_fileName))
    {
      std::cerr << "Supplied file name wasn't found.\n";
      exit(EXIT_FAILURE);
    }
    #if defined(_WIN32)
      if( IsUserAnAdmin() )
      {
        if( CreateSymbolicLink(input_fileName.c_str(), ouput_fileName.c_str(), 0) != 0 )
          return true;
        else
          return false;
      }
      else
      {
        std::cerr << "Windows doesn't let non-admins create soft links.\n";
        return false;
      }
    #else
    if( symlink(input_fileName.c_str(), ouput_fileName.c_str()) == 0 )
      return true;
    else 
      return false;
    #endif
  }

  bool setEnvironmentVariable(const std::string &variable_name, const std::string &variable_value)
  {
    std::string totalVariable = variable_name + "=" + variable_value;
    try
    {
      #if defined(_WIN32)
        int test = _putenv(totalVariable.c_str());
      #else
        putenv(cbica::constCharToChar(totalVariable));
      #endif
    }
    catch(const std::exception &e)
    {
      std::cerr << "Exception caught: " << e.what() << std::endl;
      return false;
    }

    return true;
  }
  
  bool deleteEnvironmentVariable(const std::string &variable_name)
  {
    return cbica::setEnvironmentVariable(variable_name, "");
  }

  std::vector< std::string > filesInDirectory( const std::string &dirName )
  {
    if (!directoryExists(dirName))
    {
      std::cerr << "Supplied directory name wasn't found.\n";
      exit(EXIT_FAILURE);
    }
    std::vector< std::string > allFiles;
    std::string dirName_wrap = cbica::replaceString(dirName, "\\", "/");
    if (dirName_wrap[dirName_wrap.length()-1] != '/')
    {
      dirName_wrap.append("/");
    }
    #if defined(_WIN32)
    {
      dirName_wrap.append("*.*");
      char* search_path = cbica::constCharToChar(dirName_wrap.c_str());
      WIN32_FIND_DATA fd; 
      HANDLE hFind = ::FindFirstFile(search_path, &fd); 
      if(hFind != INVALID_HANDLE_VALUE) 
      { 
        do 
        { 
          if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
          {
            allFiles.push_back(fd.cFileName);
          }
        } while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 
      } 
      return allFiles;

    }
    #else
    {
      DIR *dp;
      struct dirent *dirp;
      if((dp  = opendir(dirName.c_str())) == NULL) 
      {
        std::cerr << "Error(" << errno << ") occurred while opening directory '" << 
          dirName << "'\n";
      }
      
      while ((dirp = readdir(dp)) != NULL) 
      {
        allFiles.push_back(std::string(dirp->d_name));
      }
      closedir(dp);
      return allFiles;
    }
    #endif
  }
  
  std::vector<std::string> subdirectoriesInDirectory(const std::string &dirName, bool recursiveSearch)
  {
    if (!directoryExists(dirName))
    {
      std::cerr << "Supplied directory name wasn't found.\n";
      exit(EXIT_FAILURE);
    }
    std::vector< std::string > allDirectories;
    std::string dirName_wrap = cbica::replaceString(dirName, "\\", "/");
    if (dirName_wrap[dirName_wrap.length() - 1] != '/')
    {
      dirName_wrap.append("/");
    }
#if defined(_WIN32)
    dirName_wrap.append("*.*");
    char* search_path = cbica::constCharToChar(dirName_wrap.c_str());
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path, &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
      do
      {
        if ((fd.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && (fd.cFileName[0] != '.'))
        {
          allDirectories.push_back(std::string(fd.cFileName));
          if (recursiveSearch)
          {
            std::vector<std::string> tempVector = subdirectoriesInDirectory(dirName + "/" + std::string(fd.cFileName), true);
            for (size_t i = 0; i < tempVector.size(); i++)
            {
              allDirectories.push_back(std::string(fd.cFileName) + "/" + tempVector[i]);
            }
          }
        }
      } while (FindNextFile(hFind, &fd) != 0);
      ::FindClose(hFind);
    }
    return allDirectories;
#else
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dirName.c_str())) == NULL)
    {
      std::cerr << "Error(" << errno << ") occurred while opening directory '" << dirName << "'\n";
    }

    while ((dirp = readdir(dp)) != NULL)
    {
      if (recursiveSearch && (dirp->d_type == DT_DIR) && dirp->d_name[0] != '.')
      {
        std::vector<std::string> tempVector = subdirectoriesInDirectory(dirName + "/" + dirp->d_name, true);
        for (size_t i = 0; i < tempVector.size(); i++)
          allDirectories.push_back(std::string(dirp->d_name) + "/" + tempVector[i]);
      }

      if ( (strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0) )
        continue;

      allDirectories.push_back(dirp->d_name);
    }
    closedir(dp);
    return allDirectories;
#endif
  }
  
  size_t numberOfRowsInFile(const std::string &csvFileName, const std::string &delim)
  {
    std::ifstream inFile(csvFileName.c_str());

    // new lines will be skipped     
    inFile.unsetf(std::ios_base::skipws);
    
    // count the newlines with an algorithm specialized for counting
    return std::count(std::istream_iterator<char>(inFile), std::istream_iterator<char>(), *constCharToChar(delim));
  }

  size_t numberOfColsInFile(const std::string &csvFileName, const std::string & delim)
  {
    std::vector< std::string > rowVec;
    //std::ifstream inFile(csvFileName.c_str());
    //std::string line;
    //std::getline(inFile, line, '\n');
    //line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
    //
    //// read a single row
    //rowVec = stringSplit(line, delim);
    
    return rowVec.size();
  }

  std::vector< CSVDict > parseCSVFile( const std::string &csvFileName, const std::string &inputColumns, const std::string &inputLabels, 
    bool checkFile, const std::string &rowsDelimiter, const std::string &colsDelimiter, const std::string &optionsDelimiter)
  {
    return parseCSVFile("", csvFileName, inputColumns, inputLabels, checkFile, rowsDelimiter, colsDelimiter, optionsDelimiter);
  }

  std::vector< CSVDict > parseCSVFile(const std::string &dataDir, const std::string &csvFileName, 
    const std::string &inputColumns, const std::string &inputLabels, bool checkFile,
    const std::string &rowsDelimiter, const std::string &colsDelimiter, const std::string &optionsDelimiter)
  {
    // if CSV file doesn't exist, exit with meaningful message
    if (!fileExists(csvFileName))
    {
      std::cerr << "Supplied file name wasn't found.\n";
      exit(EXIT_FAILURE);
    }

    // store number of rows in the file - this is used to make the program parallelize-able 
    const size_t numberOfRows = numberOfRowsInFile(csvFileName);
    
    std::vector< std::string > inputColumnsVec = stringSplit(inputColumns, optionsDelimiter), // columns to consider as images
      inputLabelsVec = stringSplit(inputLabels, optionsDelimiter); // columns to consider as labels
    
    // initialize return dictionary
    std::vector< CSVDict > return_CSVDict;
    return_CSVDict.resize(numberOfRows - 1);

    std::vector< size_t > inputColumnIndeces, // indeces in the CSV file which correspond to images
      inputLabelIndeces; // indeces in the CSV file which correspond to labels

    inputColumnIndeces.resize(inputColumnsVec.size());
    inputLabelIndeces.resize(inputLabelsVec.size());

    std::vector< std::vector < std::string > > allRows; // store the entire data of the CSV file as a vector of colums and rows (vector< rows <cols> >)

    std::ifstream inFile(csvFileName.c_str());

    for (size_t i = 0; i < numberOfRows; i++)
    {
      std::string line;
      std::getline(inFile, line, *constCharToChar(rowsDelimiter));
      line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
      allRows.push_back(stringSplit(line, colsDelimiter));
    }

    inFile.close(); // at this point, the entire data from the CSV file has been read and stored in allRows

    // populate information about which indeces to store for data and labels from first row (rowCounter = 0)
    for (size_t i = 0; i < allRows[0].size(); i++)
    {
      for (size_t j = 0; j < inputColumnsVec.size(); j++)
      {
        inputColumnIndeces[j] = std::find(allRows[0].begin(), allRows[0].end(), inputColumnsVec[j]) - allRows[0].begin();
      }
      for (size_t j = 0; j < inputLabelsVec.size(); j++)
      {
        inputLabelIndeces[j] = std::find(allRows[0].begin(), allRows[0].end(), inputLabelsVec[j]) - allRows[0].begin();
      }
    }

    // organize the data
    int threads = omp_get_max_threads(); // obtain maximum number of threads available on machine  
    // if the total number of rows in CSV file are less than the available number of threads on machine (happens for testing),
    // use only the number of rows where meaningful data is present - this avoids extra thread overhead
    threads > static_cast<int>(numberOfRows) ? threads = static_cast<int>(numberOfRows - 1) : threads = threads; 
#pragma omp parallel for num_threads(threads)
    for (int rowCounter = 1; rowCounter < static_cast<int>(allRows.size()); rowCounter++)
    {
      return_CSVDict[rowCounter - 1].inputImages.resize(inputColumnIndeces.size()); // pre-initialize size to ensure thread-safety
      for (size_t i = 0; i < inputColumnIndeces.size(); i++)
      {
        if (!checkFile) // this case should only be used for testing purposes
        {
          return_CSVDict[rowCounter - 1].inputImages[i] = dataDir + allRows[rowCounter][inputColumnIndeces[i]];
        }
        else
        {
          if (fileExists(dataDir + allRows[rowCounter][inputColumnIndeces[i]]))
          {
            return_CSVDict[rowCounter - 1].inputImages[i] = dataDir + allRows[rowCounter][inputColumnIndeces[i]];
          }
          else
          {
            std::cerr << "File name in list does not exist. Location: row = " << rowCounter << ", col = " << inputColumnIndeces[i] << "\n";
            exit(EXIT_FAILURE);
          }
        }
      }
      
      return_CSVDict[rowCounter - 1].inputLabels.resize(inputLabelIndeces.size()); // pre-initialize size to ensure thread-safety
      for (size_t i = 0; i < inputLabelIndeces.size(); i++)
      {
        return_CSVDict[rowCounter - 1].inputLabels[i] = std::atof(allRows[rowCounter][inputLabelIndeces[i]].c_str());
      }
    }

    return return_CSVDict;
  }

  //====================================== String stuff ====================================//

  bool splitFileName( const std::string &dataFile, std::string &path,
     std::string &baseName, std::string &extension )
  {
    std::string dataFile_wrap = dataFile;
    if (dataFile_wrap.find(".nii.gz") != std::string::npos)
    {
      dataFile_wrap = cbica::replaceString(dataFile_wrap, ".nii.gz", "");
      extension = ".nii.gz";
      std::string temp;
      cbica::splitFileName(dataFile_wrap, path, baseName, temp);
    }
    else
    {
      //! Initialize pointers to file and user names
#if (_MSC_VER >= 1700)
      char basename_var[FILENAME_MAX], ext[FILENAME_MAX], path_name[FILENAME_MAX], drive_letter[FILENAME_MAX];
      //_splitpath(dataFile_wrap.c_str(), NULL, path_name, basename_var, ext);
      _splitpath_s(dataFile.c_str(), drive_letter, FILENAME_MAX, path_name, FILENAME_MAX, basename_var, FILENAME_MAX, ext, FILENAME_MAX);
      path = std::string(drive_letter) + std::string(path_name);
      path = cbica::replaceString(path, "\\", "/"); // normalize path for Windows
#else
      char *basename_var, *ext, *path_name;
      path_name = dirname(cbica::constCharToChar(dataFile_wrap.c_str()));
      basename_var = basename(cbica::constCharToChar(dataFile_wrap.c_str()));
      ext = strrchr(cbica::constCharToChar(dataFile_wrap.c_str()), '.');
      path = std::string(path_name);
#endif

      baseName = std::string(basename_var);
      extension = std::string(ext);
    
#if (_MSC_VER >= 1700)
    path_name[0] = NULL;
    basename_var[0] = NULL;
    ext[0] = NULL;
    drive_letter[0] = NULL;
#endif
    if (path[path.length() - 1] != '/')
    {
      path += "/";
    }
    }
    if (baseName == "")
      return false;
    else
      return true;
  }

  std::vector<std::string> stringSplit( const std::string &str, const std::string &delim )
  {
    std::vector<std::string> results;

    for (size_t i = 0; i < str.length(); i++)
    {
      std::string tempString = "";
      while ((str[i] != *delim.c_str()) && (i < str.length()))
      {
        tempString += str[i];
        i++;
      }
      results.push_back(tempString);
    }

    return results;
  }

  std::string replaceString( const std::string &entireString, 
                                    const std::string &toReplace, 
                                    const std::string &replaceWith )
  {
    std::string return_string = entireString;
    for( size_t pos = 0; ; pos += replaceWith.length() ) 
    {
    pos = return_string.find( toReplace, pos );
      if( pos == std::string::npos ) 
        break;
      
      return_string.erase( pos, toReplace.length() );
      return_string.insert( pos, replaceWith );
    }
    return return_string;
    /*
    if( entireString.length() < toReplace.length() )
      std::cerr << "Length of string to search < length of string to replace. Please check.\n";

    return(return_string.replace(entireString.find(toReplace), toReplace.length(), replaceWith));
    */
  }
  
  char* constCharToChar(const std::string &input)
  {
    char *s = new char[input.size()+1];
    std::strcpy(s, input.c_str());
    return s;
  }

  char* constCharToChar( const char *input )
  {
    return cbica::constCharToChar(std::string(input));
  }


}
