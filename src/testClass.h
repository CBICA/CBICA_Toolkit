#pragma once

#if defined(_WIN32)
#include "windows/getopt.h" // windows implementation of getopt
#else
#include <getopt.h>
#endif

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#define CLASS_NAME "testClass"
#define INDENT "\t"
#define NEWLINE "\n"

#include <sys/stat.h> 

#include <string>
#include <vector>

enum INTERPTYPE 
{
  LOG_EUCLIDEAN,
  EUCLIDEAN
};

namespace sbia
{
	class testClass
	{
	public:
		explicit testClass(const std::string &class_name);
		virtual ~testClass();

	private:
		std::string m_className, m_parameterString, m_input, m_output;
		std::vector<std::string> m_inputParameters;

		void echoUsage();
    void echoVersion();
    int runAlgorithm();

		int returnStringLocation(std::vector<std::string> &input_string_vector,
									const std::string &input_1, const std::string &input_2 );
		int returnStringLocation(std::vector<std::string> &input_string_vector,
									const std::string &input_1, const std::string &input_2, 
									const std::string &input_3, const std::string &input_4 );
	};
}