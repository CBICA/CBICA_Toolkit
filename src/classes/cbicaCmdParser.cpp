/**
\file  cbicaCmdParser.cpp

\brief Implementation file for CmdParser class.

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#include <type_traits>
#include <functional>

#include "cbicaCmdParser.h"

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
    m_exeName = PROJECT_NAME;
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

  inline void CmdParser::getMaxLength()
  {
    for( int i=0; i<m_parameters.size(); ++i )
    {
      //m_laconicParamters.push_back( std::make_tuple(std::get<0>(m_parameters[i]), i) );
      m_laconicParamters.push_back( std::get<0>(m_parameters[i]) );
      m_verboseParamters.push_back( std::make_tuple(std::get<1>(m_parameters[i]), i) );
      if( m_maxLength < std::get<4>(m_parameters[i]) )
        m_maxLength = std::get<4>(m_parameters[i]);
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
    for( int i=0; i<m_parameters.size(); ++i )
    {
      std::string spaces;
      for( int n=0; n<m_maxLength-std::get<4>(m_parameters[i])/*-4*/; n++)
        spaces.append(" ");

      std::cout << "[" << std::get<0>(m_parameters[i]) << ", " << std::get<1>(m_parameters[i]) << spaces <<
      "]  " << std::get<2>(m_parameters[i]) << NEWLINE;

      if( std::get<3>(m_parameters[i]) != "blank" )
      {
        std::string spaces_2;
        for( int n=0; n<m_maxLength+6; n++)
          spaces_2.append(" ");
        std::cout << spaces_2 << std::get<3>(m_parameters[i]) << NEWLINE;
      }
    }
  }

  void CmdParser::echoVersion()
  {
    std::cout << "Executable Name: " << m_exeName << "\n\n" << "Version: " <<
      m_version << NEWLINE;
  }

  std::pair<bool, int> CmdParser::compareParamter( /*const std::string &inputParamToCheck,*/ 
                                   const std::string &execParamToCheck
                                   )
  {
    for (int i = 1; i < m_argc; i++)
    {
      std::string inputParamToCheck = m_argv[i];
    if(!checkMaxLen)
    {
      getMaxLength();
    }
    if( inputParamToCheck == execParamToCheck )
    {
      //argv_position = i;
      return std::make_pair(true, i);
    }
    else
    {
      std::string inputCheck, execCheck;
      const int minLength = static_cast<int>( std::min(inputParamToCheck.length(), inputParamToCheck.length()) );

      if( inputParamToCheck.length()<minLength )
        inputCheck = "-" + inputParamToCheck;
      else
        inputCheck = inputParamToCheck;

      if( execParamToCheck.length()<minLength )
        execCheck = "-" + execParamToCheck;
      else
        execCheck = execParamToCheck;
    
      if( inputCheck == execCheck )
      {
        //argv_position = i;
      return std::make_pair(true, i);
      }

      /*// implement if developer plans to add verbose paramters to compare
      else if( paramToCheck.length()==2 )
        toCheck = paramToCheck;
      else
      {
        m_verboseParamters
      }
      */
    }
    }
      return std::make_pair(false, -1);
  }

  std::string CmdParser::getDescription(const std::string &parameter )
  {
    if(!checkMaxLen)
    {
      getMaxLength();
    }
    std::string inputCheck;
    
    if( parameter.length()==1 )
      inputCheck = "-" + parameter;
    else
      inputCheck = parameter;
    /// use cbica::findInVector() when using inside CBICA_Toolkit
    /*
    int position;
    std::string description;
    if( cbica::findInVector(m_laconicParamters, inputCheck, position)
    {
      if( std::get<3>(m_parameters[position]) == "blank" )
        return std::get<2>(m_parameters[position]);
      else
        return (std::get<2>(m_parameters[position]) + " " + std::get<3>(m_parameters[position]) );
    }
    else
      return "Invalid Parameter.";
    */
    int position = -1;
    std::vector<std::string>::const_iterator iterator = 
      std::find(m_laconicParamters.begin(), m_laconicParamters.end(), inputCheck) ;
    if( iterator !=  m_laconicParamters.end() )
    {
      position = static_cast<int>(iterator - m_laconicParamters.begin());
      if( std::get<3>(m_parameters[position]) == "blank" )
        return std::get<2>(m_parameters[position]);
      else
        return (std::get<2>(m_parameters[position]) + " " + std::get<3>(m_parameters[position]) );
    }
    else
      return "Invalid Parameter.";
  }

}