#pragma once 

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>

#include "testClass.h"


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkNiftiImageIO.h"
#include "itkDiffusionTensor3D.h"
#include "itkImageIOBase.h"

//#include "itkResampleWithReorientationImageFilter.h"
//#include "itkDiffusionTensorLinearInterpolateImageFunction.h"

#include "classes/cbicaUtilities.h"

using namespace sbia;

template <typename TComponentType>
void resampleImageRunner( std::string dataFile, std::string outputFile,
                          double space_x,   double space_y,   double space_z,
                          double origin_x,  double origin_y,  double origin_z,
                          int size_x, int size_y, int size_z,
                          INTERPTYPE interpType=LOG_EUCLIDEAN  )
{/*
  const unsigned int Dimension = 3;
  
  typedef itk::DiffusionTensor3D<TComponentType>          PixelType;
  typedef typename itk::Image< PixelType,  Dimension >    InputImageType;
  typedef typename itk::Image< PixelType,  Dimension >    OutputImageType;

  typedef typename itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef typename itk::ImageFileWriter< OutputImageType >  WriterType;

  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  reader->SetImageIO(itk::NiftiImageIO::New());
  writer->SetImageIO(itk::NiftiImageIO::New());
  
  reader->SetFileName( dataFile );
  writer->SetFileName( outputFile );

  typedef typename itk::ResampleWithReorientationImageFilter<InputImageType,OutputImageType> FilterType;

  typename FilterType::Pointer filter = FilterType::New();

  filter->SetDefaultPixelValue( itk::NumericTraits<PixelType>::Zero );
  
  //set the interpolator
  typedef typename itk::DiffusionTensorLinearInterpolateImageFunction< 
    InputImageType, double >  InterpolatorType;
  typename InterpolatorType::Pointer interpolator = InterpolatorType::New();

  switch ( interpType )
  {
    case LOG_EUCLIDEAN:
    {
      interpolator->SetMetricType(InterpolatorType::LOGEUCLIDEAN);
      break;
    }
    case EUCLIDEAN:
    {
      interpolator->SetMetricType(InterpolatorType::EUCLIDEAN);
      break;
    }
  }

  filter->SetInterpolator( interpolator );

  //Set up the output spacing and image size etc...
  double spacing[ Dimension ];
  spacing[0] = space_x; // pixel spacing in millimeters along X
  spacing[1] = space_y; // pixel spacing in millimeters along Y
  spacing[2] = space_z; // pixel spacing in millimeters along Z
  filter->SetOutputSpacing( spacing );

  typename InputImageType::SizeType size;
  if (size_x != -1)
  {
    size[0] = size_x;  // number of pixels along X
    size[1] = size_y;  // number of pixels along Y
    size[2] = size_z;  // number of pixels along Y
  }
  else 
  {
    typename InputImageType::SizeType oldSize;
    reader->UpdateLargestPossibleRegion();
    oldSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
    const itk::Vector<double, 3u> oldSpace = reader->GetOutput()->GetSpacing();
		size[0] = static_cast<int>(oldSize[0]*oldSpace[0]/spacing[0]);
		size[1] = static_cast<int>(oldSize[1]*oldSpace[1]/spacing[1]);
		size[2] = static_cast<int>(oldSize[2]*oldSpace[2]/spacing[2]);
  }
  filter->SetSize( size );
  
  if (origin_x != -1)
  {
    double origin[ Dimension ];
    origin[0] = origin_x;  // X space coordinate of origin
    origin[1] = origin_y;  // Y space coordinate of origin
    origin[2] = origin_z;  // Y space coordinate of origin
    filter->SetOutputOrigin( origin );
  }
  else 
  {
    const itk::Point<double, 3u> origin = reader->GetOutput()->GetOrigin();
    //for(int i=0; i<3; ++i)
 		//	cout << origin[i] << endl;
    filter->SetOutputOrigin( origin );
  }
  
  //set the output direction set to be the same as the inputs
  reader->Update();
  filter->SetOutputDirection(reader->GetOutput()->GetDirection() );

  filter->SetInput( reader->GetOutput() );
  writer->SetInput( filter->GetOutput() );
  
  
  //Run it.
  writer->Update();
}

testClass::testClass(const std::string &class_name)
{
	m_className = class_name;
	testClass::echoUsage();
	std::getline(std::cin, m_parameterString);
  std::stringstream buffer(m_parameterString);

  // Copy words to a separate vector
  std::copy(std::istream_iterator<std::string>(buffer),
            std::istream_iterator<std::string>(),
            std::back_inserter(m_inputParameters));

	int input_position = testClass::returnStringLocation(m_inputParameters, "--input", "-input", "--i", "-i");
  */
}

/**
\brief The constructor
*/
testClass::testClass(const std::string &class_name)
{
  m_className = class_name;
  testClass::echoUsage();
  // get input from user. this can be easily changed to get input directly from the class construction phase
  std::getline(std::cin, m_parameterString);
  std::istringstream i_string_stream(m_parameterString);
  std::copy( std::istream_iterator<std::string>(i_string_stream),
    std::istream_iterator<std::string>(),
    std::back_inserter(m_inputParameters) );
    //std::ostream_iterator<std::string>(std::cout, "\n") );

  testClass::runAlgorithm();
}

/**
runAlgorithm
*/
int testClass::runAlgorithm()
{
  if( (m_inputParameters[0] == "-u") || (m_inputParameters[0] == "-h") || 
    (m_inputParameters[0] == "--usage") || (m_inputParameters[0] == "--help") 
    )
  {
    testClass::echoUsage();

  }

  int p = 0;
  if( cbica::findInVector<std::string>(m_inputParameters, "-i", p) ||
    cbica::findInVector<std::string>(m_inputParameters, "--input", p)
    )
    m_input = m_inputParameters[p];

  if( cbica::findInVector<std::string>(m_inputParameters, "-o", p) ||
    cbica::findInVector<std::string>(m_inputParameters, "--output", p)
    )
    m_output = m_inputParameters[p];
  
  std::string outputDir="";
  std::string dataFile,prefix;
  int outputDir_flag = 0;
  int prefix_flag = 0;
  int verbose = 0;

  INTERPTYPE interpType = EUCLIDEAN; //LOG_EUCLIDEAN;
  std::vector<std::string> pixdimsStrVec, originStrVec, dimsStrVec;

  static struct option long_options[] =
  {
    {"usage",       no_argument,            0, 'u'},
    {"help",        no_argument,            0, 'h'},
    {"Version",     no_argument,            0, 'V'},
    {"verbose",     no_argument,            0, 'v'},
    {"output",      required_argument,      0, 'o'},
    {"dataFile",    required_argument,      0, 'd'},
    {"prefix",      required_argument,      0, 'p'},
    {"dims",        required_argument,      0, 'D'},
    {"pixdims",     required_argument,      0, 'P'},
    {"origin",      required_argument,      0, 'O'},
    {"interp",      required_argument,      0, 'i'}
  };
  
  int c, option_index = 0;
  int reqParams = 0;

  std::vector<char*> temp_vec;
  for(int i=0; i<m_parameterString.length(); ++i)
    temp_vec.push_back( reinterpret_cast<char*>(m_parameterString[i]) );

  char* const* argv = &temp_vec[0];
  const std::string temp_const_string = m_parameterString;

  while ( (c = getopt_long (m_inputParameters.size()+1, argv, "uhVvno:d:p:t:s:c:g:i:D:P:O:",
              long_options,&option_index)) != -1)
  {
    switch (c)
    {
      case 'v':
        verbose++;
        break;
      
      case 'V':
        testClass::echoVersion();
        return EXIT_SUCCESS;
      
      case 'u':
        echoUsage();
        return EXIT_SUCCESS;
      
      case 'h':
        echoUsage();
        return EXIT_SUCCESS;
      
      case 'd':
        dataFile = optarg;
        ++reqParams;
        break;
      
      case 'o':
        outputDir = optarg;
        outputDir_flag = 1;
        outputDir += "/";
        break;
      
      case 'p':
        prefix = optarg;
        prefix_flag = 1;
        ++reqParams;
        break;
      
      case 'D':
        cbica::stringSplit(optarg, ":", &dimsStrVec);
        //++reqParams;
        break;
      
      case 'P':
        cbica::stringSplit(optarg, ":", &pixdimsStrVec);
        ++reqParams;
        break;
      
      case 'O':
        cbica::stringSplit(optarg, ":", &originStrVec);
        //++reqParams;
        break;
      
      case 'i':
        if ( (static_cast<std::string>(optarg)).compare("LOGEUC") == 0 )
        {
          interpType = LOG_EUCLIDEAN;
        }
        else if ( (static_cast<std::string>(optarg)).compare("EUCLIDEAN") == 0 )
        {
          interpType = EUCLIDEAN;
        }
      
        break;
      
      case '?':
        // getopt_long already printed an error message. 
        break;
      
      default:
        abort ();
    }
  }


  if (reqParams != 3)
  {
    echoUsage();
    std::cerr << "*** Please specify all required inputs!\n\n";
    return 1;
  }
  
  //Check datafile exists
  if (! cbica::fileExists(dataFile) )
  {
    std::cerr << "*** Can't find specified datafile\n\t"<< dataFile << std::endl;
    return EXIT_FAILURE;
  }
  
  // split filename to base, extension, path
  std::string baseName;
  std::string extension;
  std::string path;
  cbica::splitFileName(dataFile,baseName,extension,path);
  if (extension == ".img")
  {
    std::cerr << "Please supply a header as input, not an .img !" << std::endl;
    return EXIT_FAILURE;
  }
  
  std::string outputBasename = "";
  if (outputDir_flag)
    outputBasename += outputDir;
  else
    outputBasename += path;
    
  if (prefix_flag)
    outputBasename += prefix;
  else
    outputBasename += baseName;
  
  std::string outputFile = outputBasename+extension;
  
  if (verbose){
    std::cerr << "data file   : " << dataFile << std::endl;
    std::cerr << "output file : " << outputFile << std::endl;
    if ( interpType == LOG_EUCLIDEAN )
    {
      std::cerr << "interp      : LOG_EUCLIDEAN" << std::endl;
    }
    else if ( interpType == EUCLIDEAN )
    {
      std::cerr << "interp      : EUCLIDEAN" << std::endl;
    }
    std::cerr << "interp      : " << interpType << std::endl;
  }
  
  double spacing[ 3 ];
  cbica::convertString<double>(pixdimsStrVec[0].c_str(), spacing[0]);
  cbica::convertString<double>(pixdimsStrVec[1].c_str(), spacing[1]);
  cbica::convertString<double>(pixdimsStrVec[2].c_str(), spacing[2]);
  
  double origin[ 3 ];
  if (! originStrVec.empty())
  {
    cbica::convertString<double>(originStrVec[0].c_str(), origin[0]); // X space coordinate of origin
    cbica::convertString<double>(originStrVec[1].c_str(), origin[1]); // Y space coordinate of origin
    cbica::convertString<double>(originStrVec[2].c_str(), origin[2]); // Z space coordinate of origin
  }
  else 
  {
    origin[0] = -1;
    origin[1] = -1;
    origin[2] = -1;
  }
  
  unsigned int size[3];
  if (! dimsStrVec.empty())
  {
    cbica::convertString<double>(dimsStrVec[0].c_str(), size[0]); // number of pixels along X
    cbica::convertString<double>(dimsStrVec[1].c_str(), size[1]); // number of pixels along Y
    cbica::convertString<double>(dimsStrVec[2].c_str(), size[2]); // number of pixels along Z
  }
  else 
  {
    size[0] = -1;
    size[1] = -1;
    size[2] = -1;
  }
  
  /////////////////////////////////////////////////////////////////////
  // Get the image pixeltype and dimension
  itk::ImageIOBase::Pointer imageIO;
  try
  {
    imageIO = itk::ImageIOFactory::CreateImageIO(
         dataFile.c_str() , itk::ImageIOFactory::ReadMode);
    if ( imageIO )
    {
      imageIO->SetFileName(dataFile.c_str());
      imageIO->ReadImageInformation();
    }
    else
    {
      std::cout << "Could not read the input image information." <<
        std::endl;
      exit( EXIT_FAILURE );
    }
  }
  catch( itk::ExceptionObject& err )
  {
    std::cout << "Could not read the input image information." << std::endl;
    std::cout << err << std::endl;
    exit( EXIT_FAILURE );
  }
  
  //We only know how to deal with scalar pixel types.
  //so die if its not scalar...
  if ( imageIO->GetPixelType() != itk::ImageIOBase::DIFFUSIONTENSOR3D 
    && imageIO->GetPixelType() != itk::ImageIOBase::SYMMETRICSECONDRANKTENSOR)
  {
      std::cout << "Unsupported pixel Type" << std::endl;
      std::cout << "We can only work with Tensor images" << std::endl;
      std::cout << imageIO->GetPixelType() << std::endl;
      return EXIT_FAILURE;
  }
  
  switch ( imageIO->GetComponentType() )
  {
    case itk::ImageIOBase::FLOAT:
    {
      resampleImageRunner<float>(dataFile,outputFile,
        spacing[0],spacing[1],spacing[2],
        origin[0],origin[1],origin[2],
        size[0],size[1],size[2], interpType );
      break;
    }
    case itk::ImageIOBase::DOUBLE:
    {
      resampleImageRunner<double>(dataFile,outputFile,
        spacing[0],spacing[1],spacing[2],
        origin[0],origin[1],origin[2],
        size[0],size[1],size[2], interpType );
      break;
    }
    
    default:
    {
      std::cout << "Unsupported component Type : " <<
        imageIO->GetComponentType()<< std::endl;
  
      return EXIT_FAILURE;
    }
  }

  return 0;
}
/**
\brief Returns the location of string in vector (non-verbose)

This function checks for non-verbose options of input parameters (which are developer-defined).

\param input_string_vector vector to search
\param input_1 1st string to search in input_string_vector
\param input_2 2nd string to search in input_string_vector

\return location of either input_1 or input_2 in input_string_vector
*/
int testClass::returnStringLocation(std::vector<std::string> &input_string_vector,
									const std::string &input_1, const std::string &input_2)
{
	std::vector<std::string>::iterator it;
	it = std::find(input_string_vector.begin(), input_string_vector.end(), input_1);
	if(it != input_string_vector.end())
		return static_cast<int>(it - input_string_vector.begin());		
	else 
	{
		it = std::find(input_string_vector.begin(), input_string_vector.end(), input_2);
	if(it != input_string_vector.end())
		return static_cast<int>(it - input_string_vector.begin());
	else
		return -1;
	}
}

/**
\brief Returns the location of string in vector (verbose)

This function checks for verbose options of input parameters (which are developer-defined).

\param input_string_vector vector to search
\param input_1 1st string to search in input_string_vector
\param input_2 2nd string to search in input_string_vector
\param input_3 3rd string to search in input_string_vector
\param input_4 4th string to search in input_string_vector

\return location of either input_1 or input_2 or input_3 or input_4 in input_string_vector
*/
int testClass::returnStringLocation(std::vector<std::string> &input_string_vector,
									const std::string &input_1, const std::string &input_2, 
									const std::string &input_3, const std::string &input_4)
{
	std::vector<std::string>::iterator it;
	it = std::find(input_string_vector.begin(), input_string_vector.end(), input_1);
	if(it != input_string_vector.end())
		return static_cast<int>(it - input_string_vector.begin());		
	else 
	{
		it = std::find(input_string_vector.begin(), input_string_vector.end(), input_2);
	if(it != input_string_vector.end())
		return static_cast<int>(it - input_string_vector.begin());
	else
		{
			it = std::find(input_string_vector.begin(), input_string_vector.end(), input_3);
		if(it != input_string_vector.end())
			return static_cast<int>(it - input_string_vector.begin());
		else 
			{
				it = std::find(input_string_vector.begin(), input_string_vector.end(), input_4);
			if(it != input_string_vector.end())
				return static_cast<int>(it - input_string_vector.begin());
			else
				return -1;
			}
		}
	}
	
}


testClass::~testClass()
{

}

void testClass::echoUsage()
{
  std::cerr << CLASS_NAME << " ---\n"
	<< "The usage for testClass is:\n"
  << "[-u --usage]&\n"
  << "[-h --help]" << INDENT << "Display class usage." << NEWLINE
	<< "[-i --input]" << INDENT << "<input_file>" << NEWLINE
	<< "[-o --output]" << INDENT << "<output_directory>" << NEWLINE
	<< "[-1 --intr]" << INDENT << "<interpolator>. can be 1 or 2" << NEWLINE
	<< "Input your parameters and press ENTER." << NEWLINE
  ;
}

void testClass::echoVersion()
{

	std::cout << "1.0.0\n";
}
