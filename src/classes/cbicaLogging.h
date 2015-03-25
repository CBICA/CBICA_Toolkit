/**
\file  cbicaLogging.h

\brief Logging class

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
	class Logging
	{
	public:
		/**
    \brief The Constructor

    \param file_name_with_path The file onto which the log file is to be written
    \param FreeText_input Free text which the user wants to be present in the log
    */
		explicit Logging(const std::string &file_name, const std::string &FreeText_input) 
		{	
      //FreeText = FreeText_input;
      file_name_with_path = file_name;
			initialize_class(file_name_with_path, log_file, exe_name, user_name); 
      writing_function(FreeText_input, log_file, timer, exe_name, user_name); 
      log_file.close();
    };
		
		/**
    \brief The Constructor without any free text input

    \param file_name_with_path The file onto which the log file is to be written
    */
		explicit Logging(const std::string &file_name) 
		{	
      //FreeText = FreeText_input;
      file_name_with_path = file_name;
			initialize_class(file_name_with_path, log_file, exe_name, user_name); 
      writing_function("", log_file, timer, exe_name, user_name); 
      log_file.close();
    };
		
		//! The Destructor
		virtual ~Logging() 
    { 
      // the closing function has been moved to the constructor and write functions instead
      /*log_file.close();*/ 
    };
	
    /**
		/brief Function to call to write to log file without any free text
    
    \param FreeText_input Free text which the user wants to be present in the log
    */
		void Write(const std::string &FreeText_input) 
		{ 
      // assumes file exists because constructor write the file once
      log_file.open(file_name_with_path.c_str(), std::ios_base::app);
      writing_function(FreeText_input, log_file, timer, exe_name, user_name); 
      log_file.close();
    };
    
    /**
		/brief Function to call to write to log file
    */
		void Write() 
		{ 
      // assumes file exists because constructor write the file once
      log_file.open(file_name_with_path.c_str(), std::ios_base::app);
      writing_function("", log_file, timer, exe_name, user_name); 
      log_file.close();
    };
    
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

		void writing_function( const std::string &FreeText_wrap, std::ofstream &log_file_wrap, 
			time_t &timer_wrap, const std::string &exe_name_wrap, const std::string &user_name_wrap );

		void initialize_class( const std::string &file_name_with_path_wrap, 
			std::ofstream &log_file_wrap, std::string &exe_name_wrap, std::string &user_name_wrap );
	};

	/**
	\brief The function used to initialize the class

	Kept private to avoid cluttering global namespace.

	\param file_name_with_path_wrap Wrap for file_name_with_path
	\param log_file_wrap Wrap for log_file
	\param exe_name_wrap Wrap for exe_name
	\param user_name_wrap Wrap for user_name
	*/
	void Logging::initialize_class(const std::string &file_name_with_path_wrap, std::ofstream &log_file_wrap, 
		std::string &exe_name_wrap, std::string &user_name_wrap )
	{
    if( cbica::fileExists(file_name_with_path_wrap) )
			log_file_wrap.open(file_name_with_path_wrap.c_str(), std::ios_base::app); // append to existing file
    else
			log_file_wrap.open(file_name_with_path_wrap.c_str());

    exe_name_wrap = cbica::getExecutableName();
    user_name_wrap = cbica::getUserName();
	}

	/**
	\brief The function used to do the actual writing onto the file
	
	Kept private to avoid cluttering global namespace.

	\param FreeText_wrap Wrap for FreeText
	\param log_file_wrap Wrap for log_file
	\param timer_wrap Wrap for timer
	\param exe_name_wrap Wrap for exe_name
	\param user_name_wrap Wrap for user_name
	*/
	void Logging::writing_function( const std::string &FreeText_wrap, std::ofstream &log_file_wrap, 
		time_t &timer_wrap, const std::string &exe_name_wrap, const std::string &user_name_wrap )
	{
		// obtain current time
		time(&timer_wrap);
		tm *time_struct = NULL;
		char buffer[200];
	
		// obtain current local date
//#if (WIN32)
//    localtime_s(time_struct, &timer_wrap);
//    sprintf_s( buffer, "%d:%02d:%02d,%02d:%02d:%02d;%s;%s", 
//    	time_struct->tm_year+1900, time_struct->tm_mon+1, time_struct->tm_mday,
//    	time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec,
//    	exe_name_wrap.c_str(), user_name_wrap.c_str() );
//#else
    time_struct = localtime(&timer_wrap);
    sprintf( buffer, "%d:%02d:%02d,%02d:%02d:%02d;%s;%s", 
    	time_struct->tm_year+1900, time_struct->tm_mon+1, time_struct->tm_mday,
    	time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec,
    	exe_name_wrap.c_str(), user_name_wrap.c_str() );
//#endif
	
    // write to the file
		log_file_wrap << buffer << ";" << FreeText_wrap.c_str() << "\n";
	}

}