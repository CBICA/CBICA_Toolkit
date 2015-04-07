/**
\file  cbicaLogging.h

\brief Declaration of the Logging class

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#include <iostream>
#include <fstream>
#include <memory>
#include <time.h>
#include <stdio.h>

#include "cbicaUtilities.h"

namespace cbica
{
  /**
  \class Logging

  \brief The logging class.

  This automatically generates a machine-parseable log specified by the file name. The user also has the option 
  of submitting free text to be put along with the log. The generated log is in the format show below:

  <CODE><4 digit year>:<2 digit month>:<2 digit date>,<2 digit 24 hour>:<2 digit minute>:<2 digit second>;<exe name>;<user name>;<free text></CODE>
  */
	class Logging
	{
	public:
		/**
    \brief The Constructor

    \param file_name_with_path The file onto which the log file is to be written
    \param FreeText_input Free text which the user wants to be present in the log
    */
		explicit Logging(const std::string file_name, const std::string FreeText_input);
		
		/**
    \brief The Constructor without any free text input

    \param file_name_with_path The file onto which the log file is to be written
    */
		explicit Logging(const std::string file_name);
		
		/**
    \brief The Constructor without any free text input or file name

    Just used to keep a track of the user name and executable run at a particular time.
    */
		explicit Logging();

		//! The Destructor
		virtual ~Logging();
	
    /**
		/brief Function to call to write to log file without any free text
    
    \param FreeText_input Free text which the user wants to be present in the log
    */
		void Write(const std::string FreeText_input);
    
    /**
		/brief Function to call to write to log file
    */
		void Write();
    
  protected:    
    /**
    \brief The function used to initialize the class
    
    Kept private to avoid cluttering global namespace.
    
    \param file_name_with_path_wrap Wrap for file_name_with_path
    \param log_file_wrap Wrap for log_file
    \param exe_name_wrap Wrap for exe_name
    \param user_name_wrap Wrap for user_name
    */
		inline void initialize_class( std::string &file_name_with_path_wrap, 
			std::ofstream &log_file_wrap, std::string &exe_name_wrap, std::string &user_name_wrap );
    
    /**
	  \brief The function used to do the actual writing onto the file
	
	  Kept private to avoid cluttering global namespace.

	  \param FreeText_wrap Wrap for FreeText
	  \param log_file_wrap Wrap for log_file
	  \param timer_wrap Wrap for timer
	  \param exe_name_wrap Wrap for exe_name
	  \param user_name_wrap Wrap for user_name
	  */
	  inline void writing_function( const std::string &FreeText_wrap, std::ofstream &log_file_wrap, 
			time_t &timer_wrap, const std::string &exe_name_wrap, const std::string &user_name_wrap );


	private:
    //! The file handler class
		std::ofstream log_file;
		//! The time class
    time_t timer; 
    //! The name of the executable calling the class
		std::string exe_name; 
    //! The current active user name
		std::string user_name;
    //! The free text to be written the log file. It is taken as input from user_name
		//std::string FreeText; 
    //! File path
    std::string file_name_with_path;

	};
}