/**
\file  cbicaCmdParser.cpp

\brief Implementation file for CmdParser class.

http://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or http://www.cbica.upenn.edu/sbia/software/license.html

*/
#include <functional>
#include <cmath>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#if (_WIN32)
#include <io.h>
#else
#undef NDEBUG
#include <cassert>
#include <unistd.h>
#endif


#include "cbicaCmdParser.h"
#include "cbicaUtilities.h"

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "0.0.1"
#endif


namespace cbica
{
  CmdParser::CmdParser(int argc, char **argv, const std::string &exe_name)
  {
#ifdef PROJECT_VERSION
    m_version = PROJECT_VERSION;
#else
    m_version = 0.1.0;
#endif    
    m_exeName = exe_name;
    m_maxLength = 0;
    checkMaxLen = false;
    helpRequested = false;

    m_argc = argc;
    m_argv = argv;

    m_optionalParameters.push_back(Parameter("u", "usage", "", "", "Prints basic usage message.", "", "", "", ""));
    m_optionalParameters.push_back(Parameter("h", "help", "", "", "Prints verbose usage information.", "", "", "", ""));
    m_optionalParameters.push_back(Parameter("v", "version", "", "", "Prints information about software version.", "", "", "", ""));
  }

  CmdParser::CmdParser(int argc, char **argv)
  {
    m_exeName = argv[0];
#ifdef PROJECT_VERSION
    m_version = PROJECT_VERSION;
#else
    m_version = 0.1.0;
#endif
    m_maxLength = 0;
    checkMaxLen = false;
    helpRequested = false;

    m_argc = argc;
    m_argv = argv;

    m_optionalParameters.push_back(Parameter("u", "usage", "", "", "Prints basic usage message.", "", "", "", ""));
    m_optionalParameters.push_back(Parameter("h", "help", "", "", "Prints verbose usage information.", "", "", "", ""));
    m_optionalParameters.push_back(Parameter("v", "version", "", "", "Prints information about software version.", "", "", "", ""));
  }

  CmdParser::~CmdParser()
  {

  }

  inline void copyrightNotice()
  {
    std::cout <<
      "\n=========================================================================\n" <<
      "Contact: sbia-software@uphs.upenn.edu\n\n" <<
      "Copyright (c) 2015 University of Pennsylvania. All rights reserved.\n" <<
      "See COPYING file or http://www.cbica.upenn.edu/sbia/software/license.html" <<
      "\n=========================================================================\n";
  }

  inline void CmdParser::getMaxLength()
  {
    m_minVerboseLength = 1024;
    m_maxLaconicLength = 0;
    m_maxLength = 0; // maximum length of laconic + verbose

    // loop through optional and required parameters separately
    for (size_t i = 0; i<m_optionalParameters.size(); ++i)
    {
      m_maxLength = m_maxLength < m_optionalParameters[i].length ? m_optionalParameters[i].length : m_maxLength;
      m_minVerboseLength = m_minVerboseLength > m_optionalParameters[i].verbose.length() ? m_optionalParameters[i].verbose.length() : m_minVerboseLength;
      m_maxLaconicLength = m_maxLaconicLength < m_optionalParameters[i].laconic.length() ? m_optionalParameters[i].laconic.length() : m_maxLaconicLength;
    }

    for (size_t i = 0; i < m_requiredParameters.size(); ++i)
    {
      m_maxLength = m_maxLength < m_requiredParameters[i].length ? m_requiredParameters[i].length : m_maxLength;
      m_minVerboseLength = m_minVerboseLength > m_requiredParameters[i].verbose.length() ? m_requiredParameters[i].verbose.length() : m_minVerboseLength;
      m_maxLaconicLength = m_maxLaconicLength < m_requiredParameters[i].laconic.length() ? m_requiredParameters[i].laconic.length() : m_maxLaconicLength;
    }

    checkMaxLen = true; // trigger flag for future checks

    if (!helpRequested && (m_argc != 1))
    {
      for (size_t i = 0; i<m_requiredParameters.size(); ++i)
      {
        // check if current required parameter has been supplied in the command line (obtained from argv)
        int tempPos;
        if (!CmdParser::compareParameter(m_requiredParameters[i].laconic, tempPos))
        {
          std::cout << "The required parameter '" << m_requiredParameters[i].laconic << "' with the following description:\n\n" <<
            CmdParser::getDescription(m_requiredParameters[i].laconic, false) << "\n\nhas not been found in the supplied arguments.\n";

          assert(tempPos > 0 && "Required parameter not found");
        }
      }
    }
  }

  void CmdParser::addOptionalParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1)
  {
    CmdParser::addOptionalParameter(laconic, verbose, expectedDataType, dataRange, description_line1, "", "", "", "");
  }

  void CmdParser::addOptionalParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2)
  {
    CmdParser::addOptionalParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, "", "", "");
  }

  void CmdParser::addOptionalParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2,
    const std::string &description_line3)
  {
    CmdParser::addOptionalParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, "", "");
  }

  void CmdParser::addOptionalParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2,
    const std::string &description_line3,
    const std::string &description_line4)
  {
    CmdParser::addOptionalParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, description_line4, "");
  }

  void CmdParser::addOptionalParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2,
    const std::string &description_line3,
    const std::string &description_line4,
    const std::string &description_line5)
  {
    if ((laconic == "u") || (laconic == "h") || (laconic == "v"))
    {
      return;
    }
    assert(laconic != "" && "Laconic parameter cannot be empty");
    assert(laconic != "" && "Verbose parameter cannot be empty");
    assert(description_line1 != "" && "Failure to initialize an empty string as description line 1");

    m_parameters.push_back(Parameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, description_line4, description_line5));
    m_optionalParameters.push_back(Parameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, description_line4, description_line5));
  }

  void CmdParser::addRequiredParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1)
  {
    CmdParser::addRequiredParameter(laconic, verbose, expectedDataType, dataRange, description_line1, "", "", "", "");
  }

  void CmdParser::addRequiredParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2)
  {
    CmdParser::addRequiredParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, "", "", "");
  }

  void CmdParser::addRequiredParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2,
    const std::string &description_line3)
  {
    CmdParser::addRequiredParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, "", "");
  }

  void CmdParser::addRequiredParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2,
    const std::string &description_line3,
    const std::string &description_line4)
  {
    CmdParser::addRequiredParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, description_line4, "");
  }

  void CmdParser::addRequiredParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2,
    const std::string &description_line3,
    const std::string &description_line4,
    const std::string &description_line5)
  {
    if ((laconic == "u") || (laconic == "h") || (laconic == "v"))
    {
      return;
    }
    assert(laconic != "" && "Laconic parameter cannot be empty");
    assert(laconic != "" && "Verbose parameter cannot be empty");
    assert(description_line1 != "" && "Failure to initialize an empty string as description line 1");

    m_parameters.push_back(Parameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, description_line4, description_line5));
    m_requiredParameters.push_back(Parameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, description_line4, description_line5));
  }

  void CmdParser::addParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1)
  {
    CmdParser::addOptionalParameter(laconic, verbose, expectedDataType, dataRange, description_line1);
  }

  void CmdParser::addParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2)
  {
    CmdParser::addOptionalParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2);
  }

  void CmdParser::addParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2,
    const std::string &description_line3)
  {
    CmdParser::addOptionalParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3);
  }

  void CmdParser::addParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2,
    const std::string &description_line3,
    const std::string &description_line4)
  {
    CmdParser::addOptionalParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, description_line4);
  }

  void CmdParser::addParameter(const std::string &laconic, const std::string &verbose, const std::string &expectedDataType, const std::string &dataRange,
    const std::string &description_line1,
    const std::string &description_line2,
    const std::string &description_line3,
    const std::string &description_line4,
    const std::string &description_line5)
  {
    CmdParser::addOptionalParameter(laconic, verbose, expectedDataType, dataRange, description_line1, description_line2, description_line3, description_line4, description_line5);
  }

  inline void CmdParser::writeParameters(const std::vector< Parameter > &inputParameters, bool verbose)
  {
    std::string spaces_verb_line2;
    for (size_t n = 0; n < m_maxLength + 6; n++)
    {
      spaces_verb_line2.append(" ");
    }
    for (size_t i = 0; i < inputParameters.size(); i++)
    {
      std::string spaces_lac, spaces_verb;

      for (size_t n = 0; n < m_maxLaconicLength - inputParameters[i].laconic.length(); n++)
      {
        spaces_lac.append(" ");
      }

      for (size_t n = 0; n < m_maxLength - inputParameters[i].length - spaces_lac.length(); n++)
      {
        spaces_verb.append(" ");
      }

      std::cout << "[" << spaces_lac << inputParameters[i].laconic << ", " <<
        inputParameters[i].verbose << spaces_verb << "]  " <<
        inputParameters[i].descriptionLine1 << NEWLINE;

      if (inputParameters[i].descriptionLine2 != "")
      {
        std::cout << spaces_verb_line2 << inputParameters[i].descriptionLine2 << NEWLINE;
        if (inputParameters[i].descriptionLine3 != "")
        {
          std::cout << spaces_verb_line2 << inputParameters[i].descriptionLine3 << NEWLINE;
          if (inputParameters[i].descriptionLine4 != "")
          {
            std::cout << spaces_verb_line2 << inputParameters[i].descriptionLine4 << NEWLINE;
            if (inputParameters[i].descriptionLine5 != "")
            {
              std::cout << spaces_verb_line2 << inputParameters[i].descriptionLine5 << NEWLINE;
            }
          }
        }
      }

      if (verbose && (inputParameters[i].laconic != "u") && (inputParameters[i].laconic != "h") && (inputParameters[i].laconic != "v"))
      {
        std::cout << spaces_verb_line2 << "Expected Type  :: " << inputParameters[i].expectedDataType << NEWLINE <<
          spaces_verb_line2 << "Expected Range :: " << inputParameters[i].dataRange << NEWLINE;
      }

      std::cout << NEWLINE; // an extra to keep coherence
    }
  }

  void CmdParser::echoUsage()
  {
    if (!checkMaxLen)
    {
      getMaxLength();
    }
    std::cout << "Executable Name: " << m_exeName << " v" << m_version
      << "\n\n" << "Usage:\n\n";

    writeParameters(m_requiredParameters, false);
    writeParameters(m_optionalParameters, false);

    copyrightNotice();
  }

  void CmdParser::echoHelp()
  {
    if (!checkMaxLen)
    {
      getMaxLength();
    }
    std::cout << "Executable Name: " << m_exeName << " v" << m_version
      << "\n\n" << "Usage:\n\n";

    std::cout << ":::Required parameters:::\n\n";
    writeParameters(m_requiredParameters, true);
    std::cout << ":::Optional parameters:::\n\n";
    writeParameters(m_optionalParameters, true);

    copyrightNotice();
  }

  void CmdParser::echoVersion()
  {
    std::cout << "Executable Name: " << m_exeName << "\n" << "        Version: " <<
      m_version << NEWLINE;

    copyrightNotice();
  }

  inline std::string internal_compare(const std::string &check_string, const int check_length)
  {
    switch (std::abs(static_cast<int>(check_string.length() - check_length)))
    {
    case 1:
      return ("-" + check_string);
      break;
    case 2:
      return ("--" + check_string);
      break;
    default:
      return (check_string);
      break;
    }
  }

  inline void CmdParser::verbose_check(std::string &input_string)
  {
    if (input_string.length() > m_maxLaconicLength)
    {
      input_string = cbica::replaceString(input_string, "--", "");
      input_string = cbica::replaceString(input_string, "-", "");

      for (size_t i = 0; i < m_requiredParameters.size(); i++)
      {
        input_string = m_requiredParameters[i].verbose == input_string ? m_requiredParameters[i].laconic : input_string;
      }

      for (size_t i = 0; i < m_optionalParameters.size(); i++)
      {
        input_string = m_optionalParameters[i].verbose == input_string ? m_optionalParameters[i].laconic : input_string;
      }

      return;
    }
  }

  bool CmdParser::compareParameter(const std::string &execParamToCheck, int &position)
  {
    bool found = false;
    position = -1;

    for (int i = 1; i < m_argc; i++)
    {
      std::string execParamToCheck_wrap = execParamToCheck;
      std::string inputParamToCheck = m_argv[i];
      if ((inputParamToCheck == "usage") || (inputParamToCheck == "-usage") || (inputParamToCheck == "--usage")
        || (inputParamToCheck == "u") || (inputParamToCheck == "-u") || (inputParamToCheck == "--u") ||
        (inputParamToCheck == "help") || (inputParamToCheck == "-help") || (inputParamToCheck == "--help")
        || (inputParamToCheck == "h") || (inputParamToCheck == "-h") || (inputParamToCheck == "--h") ||
        (inputParamToCheck == "version") || (inputParamToCheck == "-version") || (inputParamToCheck == "--version")
        || (inputParamToCheck == "v") || (inputParamToCheck == "-v") || (inputParamToCheck == "--v"))
      {
        helpRequested = true;
      }
      if (!checkMaxLen)
      {
        getMaxLength();
      }
      verbose_check(inputParamToCheck);
      verbose_check(execParamToCheck_wrap);

      if (inputParamToCheck == execParamToCheck_wrap)
      {
        position = i;
        found = true;
        return found;
      }
      else
      {
        std::string inputCheck, execCheck;
        const unsigned int minLength = static_cast<unsigned int>(std::max(
          inputParamToCheck.length(), execParamToCheck_wrap.length()));

        inputCheck = internal_compare(inputParamToCheck, minLength);
        execCheck = internal_compare(execParamToCheck_wrap, minLength);

        if (inputCheck == execCheck)
        {
          position = i;
          found = true;
          return found;
        }
      }
    }

    return found;
  }

  std::string CmdParser::getDescription(const std::string &laconicParameter, bool newLine)
  {
    if (!checkMaxLen)
    {
      getMaxLength();
    }

    for (size_t i = 0; i < m_requiredParameters.size(); i++)
    {
      if (m_requiredParameters[i].laconic == laconicParameter)
      {
        if (newLine)
        {
          return m_requiredParameters[i].descriptionLine1 + "\n" + m_requiredParameters[i].descriptionLine2 + "\n" +
            m_requiredParameters[i].descriptionLine3 + "\n" + m_requiredParameters[i].descriptionLine4 + "\n" +
            m_requiredParameters[i].descriptionLine5;
        }
        else
        {
          return m_requiredParameters[i].descriptionLine1 + " " + m_requiredParameters[i].descriptionLine2 + " " +
            m_requiredParameters[i].descriptionLine3 + " " + m_requiredParameters[i].descriptionLine4 + " " +
            m_requiredParameters[i].descriptionLine5;
        }
      }
    }

    for (size_t i = 0; i < m_optionalParameters.size(); i++)
    {
      if (m_optionalParameters[i].laconic == laconicParameter)
      {
        if (newLine)
        {
          return m_optionalParameters[i].descriptionLine1 + "\n" + m_optionalParameters[i].descriptionLine2 + "\n" +
            m_optionalParameters[i].descriptionLine3 + "\n" + m_optionalParameters[i].descriptionLine4 + "\n" +
            m_optionalParameters[i].descriptionLine5;
        }
        else
        {
          return m_optionalParameters[i].descriptionLine1 + " " + m_optionalParameters[i].descriptionLine2 + " " +
            m_optionalParameters[i].descriptionLine3 + " " + m_optionalParameters[i].descriptionLine4 + " " +
            m_optionalParameters[i].descriptionLine5;
        }
      }
    }

    assert(laconicParameter == "" && "Parameter not found in the list of initialized parameters. Please try again.");

    return "";
  }

  void CmdParser::writeConfigFile(const std::string &dirName)
  {
    if (!checkMaxLen)
    {
      getMaxLength();
    }

    std::string dirName_wrap;
    if (!cbica::directoryExists(dirName) || (dirName == ""))
    {
      cbica::makeTempDir(dirName_wrap);
    }
    dirName_wrap = cbica::replaceString(dirName, "\\", "/");
    if (dirName_wrap.substr(dirName_wrap.length() - 1) != "/")
    {
      dirName_wrap += "/";
    }

    std::string fileName = dirName_wrap + m_exeName + ".txt";

#if (_WIN32)
    assert(_access(fileName.c_str(), 6) != -1 && "No write permission for the specified config file");
#else
    assert(access(fileName.c_str(), R_OK && W_OK) != 0 && "No write permission for the specified config file");
#endif

    std::ofstream file;
    file.open(fileName.c_str());

    if (file.is_open())
    {
      for (size_t i = 0; i < m_requiredParameters.size(); i++)
      {
        file << ":" << m_requiredParameters[i].verbose << ": [" << m_requiredParameters[i].expectedDataType << "] <" <<
          m_requiredParameters[i].dataRange << "> " <<
          m_requiredParameters[i].descriptionLine1 + " " + m_requiredParameters[i].descriptionLine2 + " " +
          m_requiredParameters[i].descriptionLine3 + " " + m_requiredParameters[i].descriptionLine4 + " " +
          m_requiredParameters[i].descriptionLine5 << NEWLINE;
      }

      for (size_t i = 0; i < m_optionalParameters.size(); i++)
      {
        file << ":" << m_optionalParameters[i].verbose << ": [" << m_optionalParameters[i].expectedDataType << "] <" <<
          m_optionalParameters[i].dataRange << "> " <<
          m_optionalParameters[i].descriptionLine1 + " " + m_optionalParameters[i].descriptionLine2 + " " +
          m_optionalParameters[i].descriptionLine3 + " " + m_optionalParameters[i].descriptionLine4 + " " +
          m_optionalParameters[i].descriptionLine5 << NEWLINE;
      }
    }

    file.close();

    std::cout << "Config file written with path: '" << fileName << "'\n";
    return;
  }

  void CmdParser::writeConfigFile()
  {
    std::string dirName_wrap;
    cbica::makeTempDir(dirName_wrap);
    CmdParser::writeConfigFile(dirName_wrap);
    return;
  }

}