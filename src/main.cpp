#pragma once 

// system includes
#include <iostream>
#include <iterator>
#include <stdlib.h>

#if defined(_WIN32)
#include "windows/getopt.h"
#else
#include <getopt.h>
#endif

// project includes
#include "classes/cbicaLogging.h"
#include "classes/itk/cbicaITKImageInfo.h"
#include "itkImageIOBase.h"

#include "testClass.h"


#define INDENT "\t"
#define CLASSINDENT "   "
#define NEWLINE "\n"
#define EXEC_NAME "SBIA_CPP"
#define SVN_FILE_VERSION "$Id: main.cpp 435 2015:02:17 sarthak.pati@UPHS.PENNHEALTH.PRV $"

#ifndef RELEASE_ID
#define RELEASE_ID "0.2.0"
#endif

#ifndef SVN_REV
#define SVN_REV "435"
#endif

// display version information
void echoVersion()
{
  std::cerr << std::endl << EXEC_NAME << std::endl <<
  INDENT << " Release          : " << RELEASE_ID << std::endl <<
  INDENT << " Svn Revision     : " << SVN_REV << std::endl <<
  INDENT << " Svn File versions: " << SVN_FILE_VERSION << std::endl
  << std::endl;
}

// display usage information
void echoUsage() 
{
    std::cerr << EXEC_NAME << " ---\n"
    << "A basic project written in CPP to illustrate usage of OOP concepts, ITK, VTK+Qt usage.\n\n"
    << "Usage: " << EXEC_NAME << " [options]" << NEWLINE << NEWLINE
    << "Required Options:" << NEWLINE << INDENT << "None." << NEWLINE
    << "Options:" << NEWLINE
    << "[-u --usage]&" << NEWLINE
    << "[-h --help]" << INDENT << INDENT << "Display this message." << NEWLINE
    << "[-t --testClass]" << INDENT << "test class. Further options:" << NEWLINE
    << INDENT << "[-u --usage]&" << NEWLINE
    << INDENT << "[-h --help]" << INDENT << "Display this message." << NEWLINE
    << INDENT << "[-i --input]" << INDENT << "<input_file>" << NEWLINE
    << INDENT << "[-o --output]" << INDENT << "<output_directory>" << NEWLINE
    << INDENT << "[-1 --intr]" << INDENT << "<interpolator>. can be 1 or 2" << NEWLINE << NEWLINE
    << "[-t --testClass]" << INDENT << "test class. Further options:" << NEWLINE
    << INDENT << "[-u --usage]&" << NEWLINE
    << INDENT << "[-h --help]" << INDENT << "Display this message." << NEWLINE
    << INDENT << "[-i --input]" << INDENT << "<input_file>" << NEWLINE
    << INDENT << "[-o --output]" << INDENT << "<output_directory>" << NEWLINE
    << INDENT << "[-1 --intr]" << INDENT << "<interpolator>. can be 1 or 2" << NEWLINE
    ;

    std::cin.get();
}


int main(int argc, char *argv[])
{
	try
	{
    static struct option long_options[] =
    {
      {"usage",      no_argument,        0, 'u'},
      {"help",       no_argument,        0, 'h'},
      {"Version",    no_argument,        0, 'v'},
      {"testClass",      no_argument,        0, 't'}
    };

    int c, option_index = 0;
    while( (c = getopt_long( argc, argv, "uhv:t:",
      long_options, &option_index)) != -1)
    {
      switch(c)
      {
      case 'v':
        echoVersion();
        return EXIT_SUCCESS;

      case 'u':
        echoUsage();
        return EXIT_SUCCESS;

      case 'h':
        echoUsage();
        return EXIT_SUCCESS;

      case 't':
      {
        //cbica::testClass test_1(argv[2]);
        //cbica::dtiResampleImage resample_1();
        cbica::ITKImageInfo cbica_imageInfo = cbica::ITKImageInfo(argv[2]);
        itk::SmartPointer<itk::ImageIOBase> io_base = cbica_imageInfo.getImageIOBase();
        std::vector<itk::SizeValueType> dims = cbica_imageInfo.getImageDimensions();
        std::vector<double> spacings = cbica_imageInfo.getImageSpacings();
        std::cin.get();
        break;
      }

      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
        abort ();
      }
    }
	}
	// exception handling
	catch(std::exception& e)
	{
		std::cerr << "Error: " << e.what()  << "\n";
			std::cin.get();
		return 1;
	}
	catch(...)
	{
		std::cerr << "Exception of unknown type!\n";
	}

	//! Basic check for less or more arguments
	if( (argc < 2) || (argc > 3) )
	{
		std::cerr << "The purpose of this project is to illustrate the Logging class." << std::endl;
		std::cerr << argv[0] << " logFileName. Basic log generated at 'log.txt'.\n" << std::endl;

		cbica::Logging logger("log.txt", 
			"Usage is '<project_name> <name_of_log_file> <free_text_under_double_quotes>.'");
		logger.Write();

    return EXIT_FAILURE;
	}
	
	//! Initialize Logging class with file name to whom the user has write access
	std::string free_text = std::string(argv[2]);
	cbica::Logging logger(argv[1], "test");
	logger.Write();
	
	std::cin.get();

  return EXIT_SUCCESS;
}
