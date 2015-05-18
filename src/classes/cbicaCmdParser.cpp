/**
\file  cbicaCmdParser.cpp

\brief Implementation file for CmdParser class.

http://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or http://www.cbica.upenn.edu/sbia/software/license.html

*/
#include <type_traits>
#include <functional>
#include <cmath>

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

    m_argc = argc;
    m_argv = argv;
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

    m_argc = argc;
    m_argv = argv;
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
    m_minVerboseLength = 1000;
    m_maxLaconicLength = 0;
    m_maxLength = 0;
    for( unsigned int i=0; i<m_parameters.size(); ++i )
    {
      m_laconicParamters.push_back( std::get<0>(m_parameters[i]) );
      m_verboseParamters.push_back( std::get<1>(m_parameters[i]) );
      
      if( m_maxLength < std::get<4>(m_parameters[i]) )
        m_maxLength = std::get<4>(m_parameters[i]);
      
      if (m_minVerboseLength > m_verboseParamters[i].length())
        m_minVerboseLength = m_verboseParamters[i].length();
      
      if (m_maxLaconicLength < m_laconicParamters[i].length())
        m_maxLaconicLength = m_laconicParamters[i].length();

    }
    //m_maxLength += 6;
    checkMaxLen = true;
  }
  
  void CmdParser::addParameter( const std::string &laconic, const std::string &verbose, 
                                const std::string &description_line1 )
  {
    m_parameters.push_back( std::make_tuple(laconic, verbose, description_line1, "blank",
                            laconic.length() + verbose.length()) );
    //std::string test = std::get<0>(m_parameters[0]);
    //test.append(" ");
  }

  void CmdParser::addParameter( const std::string &laconic, const std::string &verbose, 
                                const std::string &description_line1, 
                                const std::string &description_line2 )
  {
    m_parameters.push_back( std::make_tuple(laconic, verbose, description_line1, description_line2,
                            laconic.length() + verbose.length()) );
    //std::string test = std::get<0>(m_parameters[0]);
    //test.append(" ");
  }
  
  void CmdParser::echoUsage()
  {
    if(!checkMaxLen)
    {
      getMaxLength();
    }
    std::cout << "Executable Name: " << m_exeName << " v" << m_version
      << "\n\n" << "Usage:\n\n";
    for( unsigned int i=0; i<m_parameters.size(); ++i )
    {
      std::string spaces_lac, spaces_verb;

      for (size_t n = 0; n < m_maxLaconicLength - std::get<0>(m_parameters[i]).length(); n++)
      {
        spaces_lac.append(" ");
      }
      
      for (int n = 0; n < m_maxLength - std::get<4>(m_parameters[i]) - spaces_lac.length(); n++)
      {
        spaces_verb.append(" ");
      }

      std::cout << "[" << spaces_lac << std::get<0>(m_parameters[i]) << ", " << 
        std::get<1>(m_parameters[i]) << spaces_verb << "]  " << 
        std::get<2>(m_parameters[i]) << NEWLINE;

      if( std::get<3>(m_parameters[i]) != "blank" )
      {
        std::string spaces_verb_line2;
        for (int n = 0; n < m_maxLength + 6; n++)
        {
          spaces_verb_line2.append(" ");
        }
        std::cout << spaces_verb_line2 << std::get<3>(m_parameters[i]) << NEWLINE;
      }
    }

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
      auto it = std::find(m_verboseParamters.begin(), m_verboseParamters.end(), input_string);
      if (it != m_verboseParamters.end())
      {
        input_string = m_laconicParamters[it - m_verboseParamters.begin()];
      }
    }
  }

  std::pair<bool, int> CmdParser::compareParamter( const std::string &execParamToCheck )
  {
    if (!checkMaxLen)
    {
      getMaxLength();
    }

    for (int i = 1; i < m_argc; i++)
    {
      std::string execParamToCheck_wrap = execParamToCheck;
      std::string inputParamToCheck = m_argv[i];
      verbose_check(inputParamToCheck);
      verbose_check(execParamToCheck_wrap);

      if (inputParamToCheck == execParamToCheck_wrap)
      {
        return std::make_pair(true, i);
      }
      else
      {
        std::string inputCheck, execCheck;
        const unsigned int minLength = static_cast<unsigned int>( std::max(
          inputParamToCheck.length(), execParamToCheck_wrap.length()) );

        inputCheck = internal_compare(inputParamToCheck, minLength);
        execCheck = internal_compare(execParamToCheck_wrap, minLength);
        
        if( inputCheck == execCheck )
        {
          return std::make_pair(true, i);
        }

      }
    }
      
    return std::make_pair(false, -1);
  }

  std::string CmdParser::getDescription(const std::string &laconicParameter)
  {
    if (!checkMaxLen)
    {
      getMaxLength();
    }

    int position = cbica::findInVector<std::string>(m_laconicParamters, laconicParameter).second;
    if (position >= 0)
    {
      if (std::get<3>(m_parameters[position]) == "blank")
        return std::get<2>(m_parameters[position]);
      else
        return (std::get<2>(m_parameters[position]) + " " + std::get<3>(m_parameters[position]));
    }
    else
    {
      return "Invalid Parameter.";
    }
  }

}