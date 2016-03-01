/**
\file  BasicFunctionTests.cxx

\brief File for testing the basic classes.

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html.

*/
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <stdio.h>
#if(WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#define TEST_DATA "/nifit1/"

#include "classes/cbicaLogging.h"
#include "classes/cbicaUtilities.h"
#include "classes/cbicaCmdParser.h"

int main(int argc, char** argv)
{
  try
  {
    if( (std::string( "-u").compare(argv[1]) == 0) || (std::string( "-h").compare(argv[1]) == 0) || 
        (std::string("--u").compare(argv[1]) == 0) || (std::string("--h").compare(argv[1]) == 0) ||
        (std::string( "-usage").compare(argv[1]) == 0) || (std::string( "-help").compare(argv[1]) == 0) ||
        (std::string("--usage").compare(argv[1]) == 0) || (std::string("--help").compare(argv[1]) == 0)  )
    {
      char buff[100];
      sprintf( buff, "This is a testing scenario.\n");
      buff[0] = '\0';
    }
  
    if( (std::string( "-cmdParser").compare(argv[1]) == 0) || 
        (std::string("--cmdParser").compare(argv[1]) == 0) )
    {
      int i = 0;
      argv[++i] = "-1p";
      argv[++i] = "1";
      argv[++i] = "--secondParam";
      argv[++i] = "2";
      
      cbica::CmdParser parser = cbica::CmdParser(i+1, argv);
      parser.setExeName("BasicFunctionTests");
      parser.addParameter("1p", "firstParam", "int", "1 to 10", "first parameter"); // defaults to optional parameters 
      parser.addRequiredParameter("2p", "secondParam", "int", "1 to 10", "second parameter", "description line 2", "description line 3", "description line 4", "description line 5");
      parser.addOptionalParameter("3p", "thirdParam", "string", "max length = 1024", "third parameter which is optional");

      int tempPosition;

      if (!parser.compareParameter("1p", tempPosition))
        return EXIT_FAILURE;

      if (!parser.compareParameter("2p", tempPosition))
        return EXIT_FAILURE;
    
      if( parser.getDescription("1p", false) == "" )
        return EXIT_FAILURE;
    
      if (parser.getDescription("2p", false) == "")
        return EXIT_FAILURE;
    }

    else if( (std::string( "-noFolder").compare(argv[1]) == 0) || 
             (std::string("--noFolder").compare(argv[1]) == 0) )
    {
      if( cbica::directoryExists(argv[2]) )
        return EXIT_FAILURE;
      else if( cbica::exists(argv[2]) )
        return EXIT_FAILURE;
    }
  
    else if( (std::string( "-noFile").compare(argv[1]) == 0) || 
             (std::string("--noFile").compare(argv[1]) == 0) )
    {
      if( cbica::fileExists(argv[2]) )
        return EXIT_FAILURE;
      else if( cbica::exists(argv[2]) )
        return EXIT_FAILURE;
    }

    else if( (std::string( "-createFolder").compare(argv[1]) == 0) || 
             (std::string("--createFolder").compare(argv[1]) == 0) )
    {
      cbica::createDir(argv[2]);
      if( !cbica::directoryExists(argv[2]) )
        return EXIT_FAILURE;
      if( !cbica::exists(argv[2]) )
        return EXIT_FAILURE;
    
      cbica::deleteDir(argv[2]);
    }
  
    else if( (std::string( "-createFile").compare(argv[1]) == 0) || 
             (std::string("--createFile").compare(argv[1]) == 0) )
    {
      std::ofstream file;
      file.open(argv[2]);
      file << "test";
      file.close();
      if( !cbica::fileExists(argv[2]) )
        return EXIT_FAILURE;
      if( !cbica::exists(argv[2]) )
        return EXIT_FAILURE;
    
      std::remove(argv[2]);
    }
  
    else if( (std::string( "-deleteFolder").compare(argv[1]) == 0) || 
             (std::string("--deleteFolder").compare(argv[1]) == 0) )
    {
      cbica::createDir(argv[2]);
      if( !cbica::deleteDir(argv[2]) )
        return EXIT_FAILURE;
    }

    else if( (std::string( "-deleteFile").compare(argv[1]) == 0) || 
             (std::string("--deleteFile").compare(argv[1]) == 0) )
    {
      std::ofstream file;
      file.open(argv[2]);
      file << "test";
      file.close();
      if( std::remove(argv[2])!=0 )
        return EXIT_FAILURE;
    }
    
    else if( (std::string( "-makeTempDir").compare(argv[1]) == 0) || 
             (std::string("--makeTempDir").compare(argv[1]) == 0) )
    {
      std::string return_dir;
      if( !cbica::createTmpDir(return_dir) )
        return EXIT_FAILURE;
      cbica::deleteDir(return_dir);
    }

    else if( (std::string( "-logging").compare(argv[1]) == 0) ||
             (std::string("--logging").compare(argv[1]) == 0) )
    {
      std::string dirName;
      cbica::createTmpDir(dirName);
      std::string fName = dirName + std::string("log.txt");
    
      cbica::Logging logger( fName, argv[2] );

      if (!cbica::fileExists(fName))
      {
        return EXIT_FAILURE;
      }

      std::string fileName, extName, baseName, path;
      if (cbica::splitFileName(fName, path, baseName, extName))
      {
        if (extName == ".txt")
        {
          //int blah = 1;      
        }
        else
          return EXIT_FAILURE;
      }
      else
        return EXIT_FAILURE;

      std::ifstream logFile;
      logFile.open(fName);
      std::string content;
      if( logFile.is_open() )
        while( !logFile.eof() )
          logFile >> content;
      logFile.close();
      cbica::deleteDir(dirName);
    
      #if defined(_WIN32)
        std::string userName = getenv("USERNAME");
      #else
        std::string userName = std::getenv("USER");
      #endif
      if( content.find(userName) != std::string::npos )
        return EXIT_SUCCESS;    
    }

    else if( (std::string( "-symbolic").compare(argv[1]) == 0) ||
             (std::string("--symbolic").compare(argv[1]) == 0) )
    {
      std::string dirName;
      cbica::createTmpDir(dirName);
      std::string fName = dirName + std::string("log.txt");
      std::string fName_sym = dirName + std::string("log_sym.txt");
      cbica::Logging logger( fName, argv[2] );

      #if defined(_WIN32)
       // do nothing as Windows doesn't support symbolic linkage creatation for non-admins
      #else
        cbica::makeSymbolicLink(fName, fName_sym);
        if( !cbica::isLink(fName_sym) )
          return EXIT_FAILURE;
      #endif // defined(WIN_32)
      
      cbica::deleteDir(dirName);
    }

    else if( (std::string( "-envName").compare(argv[1]) == 0) ||
             (std::string("--envName").compare(argv[1]) == 0) )
    {
      std::string random_variable = "RANDOM_VAR", random_val = "VAL";
      int val = 1;

      if( !cbica::setEnvironmentVariable(random_variable, random_val) )
        return EXIT_FAILURE;
      std::string value = getenv(random_variable.c_str());
      if( value != random_val )
        return EXIT_FAILURE;
      cbica::deleteEnvironmentVariable(random_variable);
      if( !cbica::setEnvironmentVariable(random_variable, std::to_string(val)) )
        return EXIT_FAILURE;
      value = getenv(random_variable.c_str());
      if( value != std::to_string(val) )
        return EXIT_FAILURE;
      cbica::deleteEnvironmentVariable(random_variable);
    }

    else if ( (std::string( "-subDir").compare(argv[1]) == 0) ||
              (std::string("--subDir").compare(argv[1]) == 0) )
    {
      std::string return_dir;
      if (!cbica::createTmpDir(return_dir))
        return EXIT_FAILURE;

      if (mkdir(std::string(return_dir + "/random1/").c_str()
#if(WIN32)
       
#else
        , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
#endif
        ) != 0)
        return EXIT_FAILURE;

      if (mkdir(std::string(return_dir + "/random2/").c_str()
#if(WIN32)

#else
        , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
#endif
        ) != 0)
        return EXIT_FAILURE;

      if (mkdir(std::string(return_dir + "/random3/").c_str()
#if(WIN32)

#else
        , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
#endif
        ) != 0)
        return EXIT_FAILURE;

      if (mkdir(std::string(return_dir + "random1/random11/").c_str()
#if(WIN32)

#else
        , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
#endif
        ) != 0)
        return EXIT_FAILURE;

      if (cbica::subdirectoriesInDirectory(return_dir, true).size() != 4)
        return EXIT_FAILURE;

      if (!cbica::deleteDir(return_dir))
        return EXIT_FAILURE;
    }
    
    else if( (std::string( "-csvParser").compare(argv[1]) == 0) ||
             (std::string("--csvParser").compare(argv[1]) == 0) )
    {
      std::string csvFileName = argv[2];
      
      std::vector< CSVDict > csv_test = cbica::parseCSVFile(argv[2], "T2,FL,T1", "AGE", ",");

      if (csv_test.size() != 8)
      {
        return EXIT_FAILURE;      
      }
      if (csv_test[0].inputImages.size() != 3)
      {
        return EXIT_FAILURE;      
      }
      if (csv_test[0].inputLabels.size() != 1)
      {
        return EXIT_FAILURE;      
      }
    }

  }
  catch( std::exception &e )
  {
    std::cout << "An exception occured. Number: " << e.what() << NEWLINE;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
