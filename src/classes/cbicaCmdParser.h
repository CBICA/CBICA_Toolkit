/**
\file  cbicaCmdParser.h

\brief Declaration of the CmdParser class

http://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or http://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <string>
#include <stdio.h>
#include <vector>
#include <tuple>
#include <iostream>
#include <iterator>
#include <algorithm>

#define NEWLINE "\n"

namespace cbica
{
  /**
  \class CmdParser

  \brief Simple command line parsing

  This is a pure c++ implementation. Executable name and project version are picked up automatically
  from the main CMakeLists file. Only the executable name can be modified in this class.
  
  An example of usage is shown below:
  
  \verbatim
  cbica::CmdParser parser = cbica::CmdParser(argc, argv); // OR,  
  //cbica::CmdParser parser = cbica::CmdParser(argc, argv, "exe_name"); // if a different exe_name is desired
  
  // add paramters to the variable
  parser.addParameter("u","usage", "prints usage", "second line"); // defaults to optional parameter
  parser.addRequiredParameter("h","help", "gets help from Justice League"); 
  parser.addOptionalParameter("m","marvel", "nope, I prefer The Avengers"); 
  
  /// checks for required parameters are done internally.
  
  // optional parameters 
  int m_position = parser.compareParamter("m").second;
  if (m_position > 0) 
  { 
    doSomethingWithFile( m_position+1 );
  } 
  \endverbatim
  */
  class CmdParser
  {
  public:
    /**
    \brief The Constructor

    \param argc The "argc" from executable
    \param argv The "argv" from executable
    */
    explicit CmdParser(const int argc, char **argv);
    
    /**
    \brief The Constructor

    \param argc The "argc" from executable
    \param argv The "argv" from executable

    \param exe_name Name of the executable
    */
    explicit CmdParser(const int argc, char **argv, const std::string &exe_name);

    /**
    \brief The Destructor
    */
    virtual ~CmdParser();

    /**
    \brief Set a custom executable name
    */
    void setExeName(const std::string exeName){m_exeName = exeName;};

    /**
    \brief Adding parameters: defaults to optional parameters

    \param laconic The laconic variant
    \param verbose The verbose variant
    \param description_line1 The single line of description for parameters
    */
    void addParameter( const std::string &laconic, const std::string &verbose, 
                       const std::string &description_line1 );
    
    /**
    \brief Adding parameters: defaults to optional parameters

    \param laconic The laconic variant
    \param verbose The verbose variant
    \param description_line1 The first line of description for parameters
    \param description_line2 The second line of description for parameters
    */
    void addParameter( const std::string &laconic, const std::string &verbose, 
                       const std::string &description_line1, const std::string &description_line2);

    /**
    \brief Adding Optional parameters

    \param laconic The laconic variant
    \param verbose The verbose variant
    \param description_line1 The single line of description for parameters
    */
    void addOptionalParameter( const std::string &laconic, const std::string &verbose,
                               const std::string &description_line1);

    /**
    \brief Adding Optional parameters

    \param laconic The laconic variant
    \param verbose The verbose variant
    \param description_line1 The first line of description for parameters
    \param description_line2 The second line of description for parameters
    */
    void addOptionalParameter( const std::string &laconic, const std::string &verbose,
                               const std::string &description_line1, const std::string &description_line2);

    /**
    \brief Adding Required parameters

    \param laconic The laconic variant
    \param verbose The verbose variant
    \param description_line1 The single line of description for parameters
    */
    void addRequiredParameter( const std::string &laconic, const std::string &verbose,
                               const std::string &description_line1);

    /**
    \brief Adding Required parameters

    \param laconic The laconic variant
    \param verbose The verbose variant
    \param description_line1 The first line of description for parameters
    \param description_line2 The second line of description for parameters
    */
    void addRequiredParameter( const std::string &laconic, const std::string &verbose,
                               const std::string &description_line1, const std::string &description_line2);

    /**
    \brief Display the usage
    */
    void echoUsage();
    
    /**
    \brief Display the version details
    */
    void echoVersion();

    /**
    \brief Check paramters WITHOUT hyphens

    Checks for both laconic and verbose variants of the specified parameter.

    \param execParamToCheck Which parameter to check
    \return True if parameter found
    \return Position of parameter in argv else -1
    */
    std::pair<bool, int> compareParamter(const std::string &execParamToCheck);

    /**
    \brief Get the description analogous with the paramter

    Searches only using the laconic parameter; restriction placed for performance reasons.

    \param paramter Parameter whose description is requested
    \return Description of parameter
    */
    std::string getDescription(const std::string &laconicParameter );

  private:
    //! Executable name
    std::string m_exeName;
    //! Version
    std::string m_version;

    //! CMD variable
    int m_argc;
    //! CMD variable
    char ** m_argv;

    //! The parameters are stored as tuples. description_line2 is empty if not initialized
    std::vector< std::tuple< std::string,std::string,std::string,std::string, int > > m_parameters;

    ////! Store laconic paramters along with their position in m_paramters
    //std::vector< std::tuple< std::string, int > > m_laconicParamters;
    
    //! Store laconic paramters along with their position in m_paramters
    std::vector< std::string > m_laconicParamters;
    
    //! Store laconic paramters along with their position in m_paramters
    std::vector< std::string > m_verboseParamters;

    std::vector< std::string > m_optionalParameters, m_requiredParameters;

    //! Max length of parameters for echoUsage()
    int m_maxLength;

    //! Get max length
    bool checkMaxLen;
    inline void getMaxLength();
    inline void verbose_check(std::string &input_string);

    size_t m_maxLaconicLength, m_minVerboseLength;
  };
}