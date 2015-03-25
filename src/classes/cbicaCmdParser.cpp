/**
\file  cbicaCmdParser.cpp

\brief Implementation file for cbicaCmdParser class.

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
  CmdParser::CmdParser(const std::string &exe_name)
  {
    m_exeName = exe_name;
    m_maxLength = 0;
    checkMaxLen = false;
  }
  
  CmdParser::CmdParser()
  {
    m_exeName = PROJECT_NAME;
    m_maxLength = 0;
    checkMaxLen = false;
  }

  CmdParser::~CmdParser()
  {

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
      for( int i=0; i<m_parameters.size(); ++i )
      {
        //m_laconicParamters.push_back( std::make_tuple(std::get<0>(m_parameters[i]), i) );
        m_laconicParamters.push_back( std::get<0>(m_parameters[i]) );
        m_verboseParamters.push_back( std::make_tuple(std::get<1>(m_parameters[i]), i) );
        if( m_maxLength < std::get<4>(m_parameters[i]) )
          m_maxLength = std::get<4>(m_parameters[i]);
      }
      m_maxLength += 6;
      checkMaxLen = true;
    }
    std::cout << "Executable Name: " << PROJECT_NAME << "\n\n" << "Usage:\n\n";
    for( int i=0; i<m_parameters.size(); ++i )
    {
      std::string spaces;
      for( int n=0; n<m_maxLength-std::get<4>(m_parameters[i])-4; n++)
        spaces.append(" ");

      if( std::get<3>(m_parameters[i]) == "blank" )
        std::cout << "[" << std::get<0>(m_parameters[i]) << ", " << std::get<1>(m_parameters[i]) << 
        "]" << spaces << std::get<2>(m_parameters[i]) << NEWLINE;
      else
      {
        std::string spaces_2;
        for( int n=0; n<m_maxLength; n++)
          spaces_2.append(" ");
        std::cout << "[" << std::get<0>(m_parameters[i]) << ", " << std::get<1>(m_parameters[i]) << 
          "]" << spaces << std::get<2>(m_parameters[i]) << NEWLINE <<
          spaces_2 << std::get<3>(m_parameters[i]) << NEWLINE;
      }
    }
  }

  void CmdParser::echoVersion()
  {
    std::cout << "Executable Name: " << m_exeName << "\n\n" << "Version: " <<
      PROJECT_VERSION << NEWLINE;
  }

  bool CmdParser::compareParamter( const std::string &inputParamToCheck, 
                                   const std::string &execParamToCheck )
  {
    if(!checkMaxLen)
    {
      for( int i=0; i<m_parameters.size(); ++i )
      {
        //m_laconicParamters.push_back( std::make_tuple(std::get<0>(m_parameters[i]), i) );
        m_laconicParamters.push_back( std::get<0>(m_parameters[i]) );
        m_verboseParamters.push_back( std::make_tuple(std::get<1>(m_parameters[i]), i) );
        if( m_maxLength < std::get<4>(m_parameters[i]) )
          m_maxLength = std::get<4>(m_parameters[i]);
      }
      m_maxLength += 6;
      checkMaxLen = true;
    }
    if( inputParamToCheck == execParamToCheck )
      return true;
    else
    {
      std::string inputCheck, execCheck;
      const int minLength = std::min(inputParamToCheck.length(), inputParamToCheck.length());

      if( inputParamToCheck.length()==minLength )
        inputCheck = "-" + inputParamToCheck;
      else
        inputCheck = inputParamToCheck;

      if( execParamToCheck.length()==minLength )
        execCheck = "-" + execParamToCheck;
      else
        execCheck = execParamToCheck;
    
      return (inputCheck == execCheck);

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

  std::string CmdParser::getDescription(const std::string &parameter )
  {
    if(!checkMaxLen)
    {
      for( int i=0; i<m_parameters.size(); ++i )
      {
        //m_laconicParamters.push_back( std::make_tuple(std::get<0>(m_parameters[i]), i) );
        m_laconicParamters.push_back( std::get<0>(m_parameters[i]) );
        m_verboseParamters.push_back( std::make_tuple(std::get<1>(m_parameters[i]), i) );
        if( m_maxLength < std::get<4>(m_parameters[i]) )
          m_maxLength = std::get<4>(m_parameters[i]);
      }
      m_maxLength += 6;
      checkMaxLen = true;
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
      position = iterator - m_laconicParamters.begin();
      if( std::get<3>(m_parameters[position]) == "blank" )
        return std::get<2>(m_parameters[position]);
      else
        return (std::get<2>(m_parameters[position]) + " " + std::get<3>(m_parameters[position]) );
    }
    else
      return "Invalid Parameter.";
  }

}