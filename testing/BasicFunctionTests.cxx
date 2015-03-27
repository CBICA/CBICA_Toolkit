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
  
    if( (std::string( "-cmdParser").compare(argv[2]) == 0) || 
        (std::string("--cmdParser").compare(argv[2]) == 0) )
    {
      cbica::CmdParser parser;
      parser.setExeName("BasicFunctionTests");
      parser.addParameter("1p", "firstParam", "first parameter");
      parser.addParameter("2p", "secondParam", "second parameter", "more description");

      if( !parser.compareParamter("1p", "-1p") )
        return EXIT_FAILURE;
    
      if( !parser.compareParamter("2p", "-2p") )
        return EXIT_FAILURE;

      if( parser.getDescription("1p") == "" )
        return EXIT_FAILURE;
    
      if( parser.getDescription("2p") == "" )
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
      if( !cbica::setEnvironmentVariable(random_variable, val) )
        return EXIT_FAILURE;
      value = getenv(random_variable.c_str());
      if( value != random_val )
        return EXIT_FAILURE;
      cbica::deleteEnvironmentVariable(random_variable);
    }

  }
  catch( std::exception &e )
  {
    std::cout << "An exception occured. Number: " << e.what() << NEWLINE;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
