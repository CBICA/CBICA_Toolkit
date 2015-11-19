/**
\file  cbicaLogging.cpp

\brief Implementation of the Logging class

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#include "cbicaLogging.h"

namespace cbica
{ 
  Logging::Logging(const std::string file_name, const std::string FreeText_input) 
  {	
    file_name_with_path = file_name;
  	initialize_class(file_name_with_path, log_file, exe_name, user_name); 
    writing_function(FreeText_input, log_file, timer, exe_name, user_name); 
    log_file.close();
    localLogging = false;
  }
  
  Logging::Logging(const std::string file_name) 
  {	
    file_name_with_path = file_name;
  	initialize_class(file_name_with_path, log_file, exe_name, user_name); 
    writing_function("", log_file, timer, exe_name, user_name); 
    log_file.close();
    localLogging = false;
  }
  
  Logging::Logging() 
  {	
    file_name_with_path = "";
  	//initialize_class(file_name_with_path, log_file, exe_name, user_name); 
    //writing_function("", log_file, timer, exe_name, user_name); 
    //log_file.close();
    localLogging = false;
  }

  void Logging::UseNewFile(const std::string &newLogFile)
  {
    file_name_with_path = newLogFile;
    initialize_class(file_name_with_path, log_file, exe_name, user_name);
    writing_function("", log_file, timer, exe_name, user_name);
    log_file.close();
    localLogging = false;
  }

  Logging::Logging(const Logging &origin)
  {
    file_name_with_path = origin.file_name_with_path;
  }

  Logging::~Logging() 
  { 
    /*log_file.close();*/ 
  }
  void Logging::enableLocalLogging()
  {
    localLogging = true;
  }

  void Logging::Write(const std::string FreeText_input) 
  { 
    // assumes file exists because constructor writes the file once
    log_file.open(file_name_with_path.c_str(), std::ios_base::app);
    writing_function(FreeText_input, log_file, timer, exe_name, user_name); 
    log_file.close();
  }
  
  void Logging::Write() 
  { 
    // assumes file exists because constructor writes the file once
    log_file.open(file_name_with_path.c_str(), std::ios_base::app);
    writing_function("", log_file, timer, exe_name, user_name); 
    log_file.close();
  }

	inline void Logging::initialize_class(std::string &file_name_with_path_wrap, std::ofstream &log_file_wrap, 
		std::string &exe_name_wrap, std::string &user_name_wrap )
	{
    if( file_name_with_path_wrap == "" )
    {
      cbica::createTmpDir(file_name_with_path_wrap);
      file_name_with_path_wrap += "temp.log";
      std::cout << "Blank file name provided. A new file has been created with path: "
        << file_name_with_path_wrap << std::endl;
    }
    if( cbica::fileExists(file_name_with_path_wrap) )
    {
      std::cout << "File name '" << file_name_with_path_wrap << "' has been found. Appending.\n";
			log_file_wrap.open(file_name_with_path_wrap.c_str(), std::ios_base::app); // append to existing file
    }
    else
			log_file_wrap.open(file_name_with_path_wrap.c_str());

    exe_name_wrap = cbica::getExecutableName();
    user_name_wrap = cbica::getUserName();
	}

	inline void Logging::writing_function( const std::string &FreeText_wrap, std::ofstream &log_file_wrap, 
		time_t &timer_wrap, const std::string &exe_name_wrap, const std::string &user_name_wrap )
	{
		// obtain current time
		time(&timer_wrap);
		tm *time_struct = NULL;
		char buffer[200];
	
		// obtain current local date
#ifdef _WIN32
    localtime_s(time_struct, &timer_wrap);
#else
    time_struct = localtime(&timer_wrap);
#endif

    if (!localLogging)
    {
#ifdef _WIN32
      sprintf_s(buffer, "%d:%02d:%02d,%02d:%02d:%02d;%s;%s",
        time_struct->tm_year + 1900, time_struct->tm_mon + 1, time_struct->tm_mday,
        time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec,
        exe_name_wrap.c_str(), user_name_wrap.c_str());
#else
      sprintf(buffer, "%d:%02d:%02d,%02d:%02d:%02d;%s;%s",
        time_struct->tm_year + 1900, time_struct->tm_mon + 1, time_struct->tm_mday,
        time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec,
        exe_name_wrap.c_str(), user_name_wrap.c_str());
#endif
    }
    else
    {
#ifdef _WIN32
      sprintf_s(buffer, "%d:%02d:%02d,%02d:%02d:%02d",
        time_struct->tm_year + 1900, time_struct->tm_mon + 1, time_struct->tm_mday,
        time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec);
#else
      sprintf(buffer, "%d:%02d:%02d,%02d:%02d:%02d",
        time_struct->tm_year + 1900, time_struct->tm_mon + 1, time_struct->tm_mday,
        time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec);
#endif
    }
	
    // write to the file
    if (FreeText_wrap != "")
    {
      log_file_wrap << buffer << ";" << FreeText_wrap.c_str() << "\n";
    }
    else
    {
      log_file_wrap << buffer << "\n";
    }
	}

}