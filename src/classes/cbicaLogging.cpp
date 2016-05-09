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
    consoleLogging = false; // console output is disabled if file is provided
    file_name_with_path = file_name;
    initialize_class(file_name_with_path, log_file, exe_name, user_name);
    writing_function(";" + FreeText_input, log_file, exe_name, user_name);
    log_file.close();
  }
  
  Logging::Logging(const std::string file_name) 
  {
    consoleLogging = false; // console output is disabled if file is provided
    file_name_with_path = file_name;
  	initialize_class(file_name_with_path, log_file, exe_name, user_name); 
    writing_function("", log_file, exe_name, user_name);
    log_file.close();
  }
  
  Logging::Logging() 
  {	
    file_name_with_path = "";
    consoleLogging = true; // no file path translates to console output
    initialize_class(file_name_with_path, log_file, exe_name, user_name); 
    //writing_function("", log_file, exe_name, user_name); 
  }

  void Logging::UseNewFile(const std::string &newLogFile)
  {
    consoleLogging = false;
    if (newLogFile == "")
    {
      file_name_with_path = cbica::createTmpDir();
      file_name_with_path += cbica::getExecutableName() + "-log.txt";
    }
    else
    {
      file_name_with_path = newLogFile;
    }
    initialize_class(file_name_with_path, log_file, exe_name, user_name);
    //writing_function("", log_file, timer, exe_name, user_name);
    log_file.close();
  }

  Logging::Logging(const Logging &origin)
  {
    //file_name_with_path = origin.file_name_with_path;
  }

  Logging::~Logging() 
  { 
    /*log_file.close();*/ 
  }
  void Logging::EnableTextLogging(const std::string &newLogFile)
  {
    UseNewFile(newLogFile);
  }

  void Logging::WriteError(const std::string FreeText_input)
  {
    // assumes file exists because constructor writes the file once
    log_file.open(file_name_with_path.c_str(), std::ios_base::app);
    writing_function(";" + FreeText_input, log_file, exe_name, user_name, true);
    log_file.close();
  }

  void Logging::Write(const std::string FreeText_input) 
  { 
    // assumes file exists because constructor writes the file once
    log_file.open(file_name_with_path.c_str(), std::ios_base::app);
    writing_function(";" + FreeText_input, log_file, exe_name, user_name);
    log_file.close();
  }
  
  void Logging::Write() 
  { 
    // assumes file exists because constructor writes the file once
    log_file.open(file_name_with_path.c_str(), std::ios_base::app);
    writing_function("", log_file, exe_name, user_name);
    log_file.close();
  }

	inline void Logging::initialize_class(std::string &file_name_with_path_wrap, std::ofstream &log_file_wrap, 
		std::string &exe_name_wrap, std::string &user_name_wrap )
	{
    if (!consoleLogging)
    {
      if (file_name_with_path_wrap == "")
      {
        file_name_with_path_wrap = cbica::createTmpDir() + "temp.log";
        std::cout << "Blank file name provided. A new file has been created with path: "
          << file_name_with_path_wrap << std::endl;
      }
      if (cbica::fileExists(file_name_with_path_wrap))
      {
        //std::cout << "File name '" << file_name_with_path_wrap << "' has been found. Appending.\n";
        log_file_wrap.open(file_name_with_path_wrap.c_str(), std::ios_base::app); // append to existing file
      }
      else
      {
        log_file_wrap.open(file_name_with_path_wrap.c_str());
      }
    }

    if (exe_name_wrap == "")
    {
      exe_name_wrap = cbica::getExecutableName();
    }
    if (user_name_wrap == "")
    {
      user_name_wrap = cbica::getUserName();
    }
	}

	inline void Logging::writing_function( const std::string &FreeText_wrap, std::ofstream &log_file_wrap, 
		const std::string &exe_name_wrap, const std::string &user_name_wrap, bool isError )
  {
    // obtain current time and add exeName, userName along with proper delineation
    std::string timeExeUser = cbica::getCurrentLocalDateAndTime() + ";" + exe_name_wrap + ";" + user_name_wrap + ";";

    if (consoleLogging)
    {
      if (isError)
      {
        std::cerr << timeExeUser + FreeText_wrap << "\n";
      }
      else
      {
        std::cout << timeExeUser + FreeText_wrap << "\n";
      }
    }
    else
    {
      log_file_wrap << timeExeUser.c_str() << FreeText_wrap.c_str() << "\n";
    }
	}

} // end namespace cbica