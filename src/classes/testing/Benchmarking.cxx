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

#include "classes/cbicaCmdParser.h"
#include "classes/cbicaUtilities.h"
#include "classes/cbicaLogging.h"

int main(int argc, char** argv)
{
  cbica::CmdParser parser(argc, argv);
  parser.addRequiredParameter("d", "dataDir", cbica::Parameter::DIRECTORY, "none", "Parent directory where all data is present");
  parser.addRequiredParameter("i", "inputPatterns", cbica::Parameter::STRING, "comma separated entries", "String pattern(s) for the input files");
  parser.addRequiredParameter("o", "outputPatterns", cbica::Parameter::STRING, "comma separated entries", "String pattern(s) for the output files");
  parser.addOptionalParameter("L", "LogFile", cbica::Parameter::FILE, "text file", "File to write all logging information", "Defualts to cbica::makeTempDir()/Benchmarking.log");
  parser.exampleUsage("-d C:/here/is/my/Data/ -i fixed,moving -o output");

  std::string logFile = cbica::createTmpDir() + "Benchmarking.log", inputPatterns, outputPatterns, dataDir;

  // parser the command line
  parser.getParameterValue("d", dataDir);
  parser.getParameterValue("i", inputPatterns);
  parser.getParameterValue("o", outputPatterns);
  if (parser.isPresent("-L"))
  {
    parser.getParameterValue("-L", logFile);
  }
  cbica::Logging logger = cbica::Logging(logFile, "Starting Benchmarking");
  
  // ensure dataDir ends with a '/'
  if (dataDir[dataDir.length() - 1] != '/')
  {
    dataDir.append("/");
  }
  if (!cbica::isDir(dataDir))
  {
    logger.WriteError("dataDir='" + dataDir + "' is not detected");
    return EXIT_FAILURE;
  }

  // start making sense of the inputs from the command line
  std::vector< std::string > inputPatterns_vector  = cbica::stringSplit(inputPatterns , ",");
  std::vector< std::string > outputPatterns_vector = cbica::stringSplit(outputPatterns, ",");
  std::vector< std::string > detectedSubjects = cbica::subdirectoriesInDirectory(dataDir);

  for (size_t i = 0; i < detectedSubjects.size(); i++)
  {
    const std::string subjectUnderConsideration = dataDir + detectedSubjects[i] + "/";
    std::vector< std::string > filesPerSubject = cbica::filesInDirectory(subjectUnderConsideration);
    std::vector< std::string > inputFiles, outputFiles;
    std::vector< size_t > outputFileIndeces;

    for (size_t j = 0; j < filesPerSubject.size(); j++)
    {
      const std::string fileUnderConsideration = subjectUnderConsideration + filesPerSubject[j];
      
      // ensure file is present
      if (!cbica::fileExists(fileUnderConsideration))
      {
        logger.WriteError("The file, '" + fileUnderConsideration + "' wasn't found");
        return EXIT_FAILURE;
      }

      // check for output file patterns
      for (size_t k = 0; k < outputPatterns_vector.size(); k++)
      {
        if (fileUnderConsideration.find(outputPatterns_vector[k]) != std::string::npos)
        {
          outputFiles.push_back(fileUnderConsideration);
          outputFileIndeces.push_back(j);
        }
      }

    }

    // the reason why input and output patterns are being processes separately is to take care of the possibility of encountering a file like 'movingImage_T1_processed_output.nii.gz'
    for (size_t j = 0; j < filesPerSubject.size(); j++)
    {
      if (std::find(outputFileIndeces.begin(), outputFileIndeces.end(), j) == outputFileIndeces.end())
      {
        const std::string fileUnderConsideration = subjectUnderConsideration + filesPerSubject[j]; // check is not needed since it is already done in the previous loop
        // check for input file patterns
        for (size_t k = 0; k < inputPatterns_vector.size(); k++)
        {
          if (fileUnderConsideration.find(inputPatterns_vector[k]) != std::string::npos)
          {
            inputFiles.push_back(fileUnderConsideration);
          }
        }
      }
    }

    // run the algorithm which needs to be tested on the inputFiles and save the result(s) [preferably using one of the outputPatterns]

    // run comparisons between the different output files and generate statistics. Save those statistics using logger
  }

  logger.Write("Finished Successfully");
  return EXIT_SUCCESS;
}