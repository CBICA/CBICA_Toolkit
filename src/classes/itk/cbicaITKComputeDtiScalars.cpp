/**
\file  cbicaITKComputeDtiScalars.cpp

\brief Implementation of the ComputeDtiScalars class

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/

#include <algorithm>

#include "cbicaITKComputeDtiScalars.h"

#include "cbicaUtilities.h"
#include "cbicaITKImageInfo.h"

namespace cbica
{
  ComputeDtiScalars::ComputeDtiScalars()
  {
    m_verbose = false;
  }
  
  ComputeDtiScalars::ComputeDtiScalars( const std::string &inputFileName, 
                                        const std::string &output ) : 
                                        CommonHolder(inputFileName, output)
  {
    std::cout << "All parameters enabled.\n";
    m_parameters = "ALL";
    ComputeDtiScalars::runAlgorithm();
  }
  
  ComputeDtiScalars::ComputeDtiScalars( const std::string &inputFileName, const std::string &output, 
                                        const std::string &parameters ) : 
                                        CommonHolder(inputFileName, output)
  {
    m_parameters = parameters;
    ComputeDtiScalars::runAlgorithm();
  }
  
  ComputeDtiScalars::ComputeDtiScalars( const std::string &inputFileName, const std::string &output, 
                                        const std::string &parameters, const std::string &prefix ) : 
                                        CommonHolder(inputFileName, output, prefix)
  {
    m_parameters = parameters;
    ComputeDtiScalars::runAlgorithm();
  }

  ComputeDtiScalars::~ComputeDtiScalars()
  {

  }
  
  void ComputeDtiScalars::SetParameters( const std::vector<std::string> &inputFileNames, 
                                         const std::string &output, const std::string &prefix )
  { 
    CommonHolder(inputFileNames[0], output, prefix);
    ComputeDtiScalars::runAlgorithm();
  }

  void ComputeDtiScalars::SetParameters( const std::string &inputFileName, const std::string &output, 
                                         const std::string &parameters, const std::string &prefix )
  { 
    std::vector<std::string> inputFilesWrap;
    inputFilesWrap.push_back(inputFileName);
    m_parameters = parameters;
    SetParameters(inputFilesWrap, output, prefix);
    ComputeDtiScalars::runAlgorithm();
  }

  template< typename ScalarImageType, typename TensorImageType >
  inline void allocateScalarIm( typename ScalarImageType::Pointer scIm,
                                typename TensorImageType::Pointer tenIm )
  {
    scIm->SetOrigin(tenIm->GetOrigin());
    scIm->SetSpacing(tenIm->GetSpacing());
    scIm->SetDirection(tenIm->GetDirection());
    scIm->SetLargestPossibleRegion(tenIm->GetLargestPossibleRegion());
    scIm->SetRequestedRegion(tenIm->GetRequestedRegion());
    scIm->SetBufferedRegion(tenIm->GetBufferedRegion());
    scIm->Allocate();
  }
  
  template< typename VectorImageType, typename TensorImageType >
  inline void allocateVectorIm( typename VectorImageType::Pointer scIm,
                                typename TensorImageType::Pointer tenIm )
  {
    scIm->SetOrigin(tenIm->GetOrigin());
    scIm->SetSpacing(tenIm->GetSpacing());
    scIm->SetDirection(tenIm->GetDirection());
    scIm->SetLargestPossibleRegion(tenIm->GetLargestPossibleRegion());
    scIm->SetRequestedRegion(tenIm->GetRequestedRegion());
    scIm->SetBufferedRegion(tenIm->GetBufferedRegion());
    scIm->Allocate();
  }


  
  template <class TTensorComponentType, class ScalarPixelType, unsigned int ImageDimension>
  inline bool runner( const std::string &dataFile, const std::string &outputFile, 
                      const std::string &ext, const std::string &parameters )
  {
    bool writeFA = false, writeTR = false, writeEign = false, writeGeo = false, writeGordR = false, 
      writeGordK = false, writeRadAx = false, writeSkew = false, writeKurt = false, verbose = true;

    std::string params = parameters, default_ext = ext;
    std::transform(params.begin(), params.end(), params.begin(), ::tolower);

    // defaults to all parameters if nothing else is specified
    if (params == "all")
    {
      writeFA = true;
      writeEign = true;
      writeGeo = true;
      writeSkew = true;
      writeKurt = true;
      writeRadAx = true;
      writeGordR = true;
      writeGordK = true;
    }
    else
    {
      if (params.find("fa") != std::string::npos)
        writeFA = true;
      
      else if (params.find("eig") != std::string::npos)
        writeEign = true;
      
      else if (params.find("geo") != std::string::npos)
        writeGeo = true;
      
      else if (params.find("skew") != std::string::npos)
        writeSkew = true;
      
      else if (params.find("kurt") != std::string::npos)
        writeKurt = true;
      
      else if (params.find("radax") != std::string::npos)
        writeRadAx = true;
      
      else if (params.find("gordr") != std::string::npos)
        writeGordR = true;
      
      else if (params.find("gordk") != std::string::npos)
        writeGordK = true;
          
      else
        std::cerr << "Invalid Parameter string.\n";
    }

    typedef itk::DiffusionTensor3D< TTensorComponentType > TensorPixelType;
    typedef itk::Image< ScalarPixelType,ImageDimension > ScalarImageType;
    typedef itk::VectorImage< ScalarPixelType,ImageDimension > VectorImageType;
    typedef itk::Image< TensorPixelType,ImageDimension > TensorImageType;
    typedef itk::ImageFileReader< TensorImageType > ReaderType;
    typedef itk::ImageRegionConstIteratorWithIndex < TensorImageType > ConstIterType;
    
    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(dataFile);

    try
    {
      typename TensorImageType::Pointer tensorIm = TensorImageType::New();
      reader->Update();
      tensorIm = reader->GetOutput();
    
      // Allocate each image
      //FA
      typename ScalarImageType::Pointer faIm = ScalarImageType::New();
      //TR
      typename ScalarImageType::Pointer trIm = ScalarImageType::New();
      
      //Eigensys
      typename ScalarImageType::Pointer l1Im = ScalarImageType::New();
      typename ScalarImageType::Pointer l2Im = ScalarImageType::New();
      typename ScalarImageType::Pointer l3Im = ScalarImageType::New();
      typename VectorImageType::Pointer v1Im = VectorImageType::New();
      typename VectorImageType::Pointer v2Im = VectorImageType::New();
      typename VectorImageType::Pointer v3Im = VectorImageType::New();
    	
    	//Skewness & Kurtosis
    	typename ScalarImageType::Pointer skIm = ScalarImageType::New();
    	typename ScalarImageType::Pointer kuIm = ScalarImageType::New();
      
      //Geometric Features
      typename ScalarImageType::Pointer clIm = ScalarImageType::New();
      typename ScalarImageType::Pointer cpIm = ScalarImageType::New();
      typename ScalarImageType::Pointer csIm = ScalarImageType::New();
      
      //Radial Axial
      typename ScalarImageType::Pointer rdIm = ScalarImageType::New();
      typename ScalarImageType::Pointer adIm = ScalarImageType::New();
          
      //Gordons R features
      typename ScalarImageType::Pointer r1Im = ScalarImageType::New();
      typename ScalarImageType::Pointer r2Im = ScalarImageType::New();
      typename ScalarImageType::Pointer r3Im = ScalarImageType::New();
    
      //Gordons K features
      typename ScalarImageType::Pointer k1Im = ScalarImageType::New();
      typename ScalarImageType::Pointer k2Im = ScalarImageType::New();
      typename ScalarImageType::Pointer k3Im = ScalarImageType::New();
      
      //Allocate all the images...
      if (writeFA)
        allocateScalarIm<ScalarImageType,TensorImageType>(faIm,tensorIm);
      
      if (writeTR)
        allocateScalarIm<ScalarImageType,TensorImageType>(trIm,tensorIm);
      
      if (writeEign)
      {
        allocateScalarIm<ScalarImageType,TensorImageType>(l1Im,tensorIm);
        allocateScalarIm<ScalarImageType,TensorImageType>(l2Im,tensorIm);
        allocateScalarIm<ScalarImageType,TensorImageType>(l3Im,tensorIm);
        v1Im->SetVectorLength(3);
        v2Im->SetVectorLength(3);
        v3Im->SetVectorLength(3);
        allocateVectorIm<VectorImageType,TensorImageType>(v1Im,tensorIm);
        allocateVectorIm<VectorImageType,TensorImageType>(v2Im,tensorIm);
        allocateVectorIm<VectorImageType,TensorImageType>(v3Im,tensorIm);
        
      }
    	
    	if (writeSkew)
    	{
    		allocateScalarIm<ScalarImageType,TensorImageType>(skIm,tensorIm);
    	}
    	
    	if (writeKurt)
    	{
    		allocateScalarIm<ScalarImageType,TensorImageType>(kuIm,tensorIm);
    	}
    	
      if (writeGeo)
      {
        allocateScalarIm<ScalarImageType,TensorImageType>(clIm,tensorIm);
        allocateScalarIm<ScalarImageType,TensorImageType>(cpIm,tensorIm);
        allocateScalarIm<ScalarImageType,TensorImageType>(csIm,tensorIm);
      }
      if (writeRadAx)
      {
        allocateScalarIm<ScalarImageType,TensorImageType>(rdIm,tensorIm);
        allocateScalarIm<ScalarImageType,TensorImageType>(adIm,tensorIm);
      }
      if (writeGordR)
      {
        allocateScalarIm<ScalarImageType,TensorImageType>(r1Im,tensorIm);
        allocateScalarIm<ScalarImageType,TensorImageType>(r2Im,tensorIm);
        allocateScalarIm<ScalarImageType,TensorImageType>(r3Im,tensorIm);
      }
      if (writeGordK)
      {
        allocateScalarIm<ScalarImageType,TensorImageType>(k1Im,tensorIm);
        allocateScalarIm<ScalarImageType,TensorImageType>(k2Im,tensorIm);
        allocateScalarIm<ScalarImageType,TensorImageType>(k3Im,tensorIm);
      }
      
      //Loop though all the voxels and if compute the needed measures!
      ConstIterType iter( tensorIm, tensorIm->GetLargestPossibleRegion() );
      for ( iter.GoToBegin(); !iter.IsAtEnd(); ++iter)
      {
        TensorPixelType tmp = iter.Get();
        typename TensorPixelType::EigenValuesArrayType     lambda;
        typename TensorPixelType::EigenVectorsMatrixType   vMat;
        tmp.ComputeEigenAnalysis(lambda, vMat);
        
        typename ScalarImageType::IndexType index = iter.GetIndex();
        
        if (writeTR)
          trIm->SetPixel(index, lambda[0]+lambda[1]+lambda[2] );
          
        if (writeFA)
        {
          faIm->SetPixel(index, tmp.GetFractionalAnisotropy());
        }
        
        if (writeEign)
        {
          l1Im->SetPixel(index,lambda[2]);
          l2Im->SetPixel(index,lambda[1]);
          l3Im->SetPixel(index,lambda[0]);
    
          typename VectorImageType::PixelType vec1(3);
          typename VectorImageType::PixelType vec2(3);
          typename VectorImageType::PixelType vec3(3);
    
          vec1[0] = vMat[2][0];
          vec1[1] = vMat[2][1];
          vec1[2] = vMat[2][2];
          
          vec2[0] = vMat[1][0];
          vec2[1] = vMat[1][1];
          vec2[2] = vMat[1][2];
          
          vec3[0] = vMat[0][0];
          vec3[1] = vMat[0][1];
          vec3[2] = vMat[0][2];
    
          v1Im->SetPixel(index,vec1);
          v2Im->SetPixel(index,vec2);
          v3Im->SetPixel(index,vec3);
          
        }
    		
    		if (writeSkew)
    		{
    			ScalarPixelType m1,m3,l1,l2,l3;
    			l1 = abs(lambda[0]);
    			l2 = abs(lambda[1]);
    			l3 = abs(lambda[2]);
    			m1 = (l1 + l2 + l3) / 3.0;
    			
    			if (m1 > 0)
    			{
    				m3 = ( vcl_pow(l1-m1,3) + vcl_pow(l2-m1,3) + vcl_pow(l3-m1,3) ) / ( vcl_pow(l1,3) + vcl_pow(l2,3) + vcl_pow(l3,3) );
    				if (m3 > 0)
    				{
    					skIm->SetPixel(index,vcl_pow(m3,static_cast<ScalarPixelType>(1.0/3.0)));
    				}
    				else
    				{
    					skIm->SetPixel(index,-1*vcl_pow((-1*m3),static_cast<ScalarPixelType>(1.0/3.0)));
    				}
    			}
    			else
    			{
    				skIm->SetPixel(index,static_cast<ScalarPixelType>(0));
    			}
    		}
    		
    		if (writeKurt)
    		{
    			ScalarPixelType m1,m4,l1,l2,l3;
    			l1 = abs(lambda[0]);
    			l2 = abs(lambda[1]);
    			l3 = abs(lambda[2]);
    			m1 = (l1 + l2 + l3) / 3.0;
    			if (m1 > 0)
    			{
    				m4 = (  vcl_pow(l1-m1,4) + vcl_pow(l2-m1,4) + vcl_pow(l3-m1,4) ) / ( vcl_pow(l1,4) + vcl_pow(l2,4) + vcl_pow(l3,4) );
    				kuIm->SetPixel(index,vcl_pow(m4,static_cast<ScalarPixelType>(1.0/4.0)));
    			}
    			else
    			{
    			  kuIm->SetPixel(index,static_cast<ScalarPixelType>(0));
    			}
    		}
    
        if (writeGeo)
        {
          //These seem kinda arbitrary to me?
          //ScalarPixelType tr = lambda[0]+lambda[1]+lambda[2];
          //if (tr > 0 )
          if (lambda[2] > 0 )
          {
            //backwards
            //clIm->SetPixel(index,     (lambda[0]-lambda[1]) / tr);
            //cpIm->SetPixel(index, 2 * (lambda[1]-lambda[2]) / tr);
            //csIm->SetPixel(index, 3 * lambda[2] / tr);
            clIm->SetPixel(index, (lambda[2]-lambda[1]) / lambda[2]);
            cpIm->SetPixel(index, (lambda[1]-lambda[0]) / lambda[2]);
            csIm->SetPixel(index,  lambda[0] / lambda[2]);
          }
          else
          {
            clIm->SetPixel(index, 0);
            cpIm->SetPixel(index, 0);
            csIm->SetPixel(index, 0);
          }
        }
        
        if (writeRadAx)
        {
          rdIm->SetPixel(index, (lambda[1]+lambda[0]) / 2);
          adIm->SetPixel(index, lambda[2]);
        }
        
        if (writeGordR)
        {
          //Compute the moments...
          ScalarPixelType m1, m2, m3;
          ScalarPixelType r1, r2, r3;
          m1 = (lambda[0] + lambda[1] + lambda[2] ) / 3.0;
          m2 = (  vcl_pow(lambda[0]-m1,2) + vcl_pow(lambda[1]-m1,2)
                + vcl_pow(lambda[2]-m1,2) ) / 3.0;
          m3 = (  vcl_pow(lambda[0]-m1,3) + vcl_pow(lambda[1]-m1,3)
                + vcl_pow(lambda[2]-m1,3) ) / 3.0;
          
          r1 = sqrt(3 * (vcl_pow(m1,2) + m2) );
          r2 = sqrt( 3 * m2 / 2 / (vcl_pow(m1,2)+m2) );
            r3 = sqrt(2) * m3 / vcl_pow(static_cast<double>(m2), static_cast<double>(1.5));
          r3 = (lambda[0] * lambda[1] * lambda[2]) / vcl_pow(static_cast<double>(sqrt( 3 * m2 )), 3);
    
          r1Im->SetPixel(index, r1);
          r2Im->SetPixel(index, r2);
          r3Im->SetPixel(index, r3);
        }
    
        if (writeGordK)
        {
          //Compute the moments...
          ScalarPixelType m1, m2, m3;
          ScalarPixelType k1, k2, k3;
          
          m1 = (lambda[0] + lambda[1] + lambda[2] ) / 3.0;
          m2 = (  vcl_pow(lambda[0]-m1,2) + vcl_pow(lambda[1]-m1,2)
                + vcl_pow(lambda[2]-m1,2) ) / 3.0;
          m3 = (  vcl_pow(lambda[0]-m1,3) + vcl_pow(lambda[1]-m1,3)
                + vcl_pow(lambda[2]-m1,3) ) / 3.0;
          
          k1 = 3 * m1;
          k2 = sqrt( 3 * m2 );
            k3 = sqrt(2) * m3 / vcl_pow(static_cast<double>(m2), static_cast<double>(1.5));
    			k3 = (lambda[0] * lambda[1] * lambda[2]) / vcl_pow(static_cast<double>(sqrt( 3 * m2 )), 3);
    			
    
          k1Im->SetPixel(index, k1);
          k2Im->SetPixel(index, k2);
          k3Im->SetPixel(index, k3);
        }
      }
      
      //Done Computation
      if (verbose)
        std::cout << "Done Computing Scalars.\n";
    
      typedef itk::ImageFileWriter< ScalarImageType >  ScalarWriterType;
      typedef itk::ImageFileWriter< VectorImageType >  VectorWriterType;
      
      if (writeFA)
      {
        typename ScalarWriterType::Pointer writer = ScalarWriterType::New();
        writer->SetInput( faIm );
        itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_FA"+default_ext);
    
        if (verbose)
          std::cout << "Writing FA image: " << outputFile << "_FA" << default_ext << "\n";
        
        writer->Update();
      }    
      
      if (writeTR)
      {
        typename ScalarWriterType::Pointer writer = ScalarWriterType::New();
        writer->SetInput( trIm );
        itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_TR"+default_ext);
    
        if (verbose)
          std::cout << "Writing TR image: " << outputFile << "_TR" << default_ext+"\n";
        
        writer->Update();
      }    
    
      if (writeEign)
      {
        { //scope just for coding ease.
          typename ScalarWriterType::Pointer writer = ScalarWriterType::New();
          writer->SetInput( l1Im );
          itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
          writer->SetImageIO(imageIO);
          writer->SetFileName( outputFile+"_L1"+default_ext);
          if (verbose)
            std::cout << "Writing L1 image: " << outputFile << "_L1" << default_ext+"\n";
          writer->Update();
    
          writer = ScalarWriterType::New();
          writer->SetInput( l2Im );
          imageIO = itk::NiftiImageIO::New();
          writer->SetImageIO(imageIO);
          writer->SetFileName( outputFile+"_L2"+default_ext);
          if (verbose)
            std::cout << "Writing L2 image: " << outputFile << "_L2" << default_ext << "\n";
          writer->Update();
    
          writer = ScalarWriterType::New();
          writer->SetInput( l3Im );
          imageIO = itk::NiftiImageIO::New();
          writer->SetImageIO(imageIO);
          writer->SetFileName( outputFile+"_L3"+default_ext);
          if (verbose)
            std::cout << "Writing L3 image: " << outputFile << "_L3" << default_ext << "\n";
          writer->Update();
        }
        { //scope just for coding ease.
          typename VectorWriterType::Pointer writer = VectorWriterType::New();
          writer->SetInput( v1Im );
          writer->SetImageIO(itk::NiftiImageIO::New());
          writer->SetFileName( outputFile+"_V1"+default_ext);
          if (verbose)
            std::cout << "Writing V1 image: " << outputFile << "_V1" << default_ext << "\n";
          writer->Update();
    
          writer = VectorWriterType::New();
          writer->SetInput( v2Im );
          writer->SetImageIO(itk::NiftiImageIO::New());
          writer->SetFileName( outputFile+"_V2"+default_ext);
          if (verbose)
            std::cout << "Writing V2 image: " << outputFile << "_V2" << default_ext << "\n";
          writer->Update();
    
          writer = VectorWriterType::New();
          writer->SetInput( v3Im );
          writer->SetImageIO(itk::NiftiImageIO::New());
          writer->SetFileName( outputFile+"_V3"+default_ext);
          if (verbose)
            std::cout << "Writing V3 image: " << outputFile << "_V3" << default_ext << "\n";
          writer->Update();
        }
      }    
    	
    	if (writeSkew)
      {
        typename ScalarWriterType::Pointer writer = ScalarWriterType::New();
        writer->SetInput( skIm );
        itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_SK"+default_ext);
    
        if (verbose)
          std::cout << "Writing SK image: " << outputFile << "_SK" << default_ext << "\n";
        
        writer->Update();
      } 
    	
    	if (writeKurt)
      {
        typename ScalarWriterType::Pointer writer = ScalarWriterType::New();
        writer->SetInput( kuIm );
        itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_KU"+default_ext);
    
        if (verbose)
          std::cout << "Writing FA image: " << outputFile << "_KU" << default_ext << "\n";
        
        writer->Update();
      } 
    
      if (writeGeo)
      {
        typename ScalarWriterType::Pointer writer = ScalarWriterType::New();
        writer->SetInput( clIm );
        itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_CL"+default_ext);
        if (verbose)
          std::cout << "Writing CL image: " << outputFile << "_CL" << default_ext << "\n";
        writer->Update();
    
        writer = ScalarWriterType::New();
        writer->SetInput( cpIm );
        imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_CP"+default_ext);
        if (verbose)
          std::cout << "Writing CP image: " << outputFile << "_CP" << default_ext << "\n";
        writer->Update();
    
        writer = ScalarWriterType::New();
        writer->SetInput( csIm );
        imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_CS"+default_ext);
        if (verbose)
          std::cout << "Writing CS image: " << outputFile << "_CS" << default_ext << "\n";
        writer->Update();
      }    
    
      if (writeRadAx)
      {
        typename ScalarWriterType::Pointer writer = ScalarWriterType::New();
        writer->SetInput( rdIm );
        itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_RAD"+default_ext);
        if (verbose)
          std::cout << "Writing Radial image: " << outputFile << "_RAD" << default_ext << "\n";
        writer->Update();
    
        writer = ScalarWriterType::New();
        writer->SetInput( adIm );
        imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_AX"+default_ext);
        if (verbose)
          std::cout << "Writing Axial image: " << outputFile << "_AX" << default_ext << "\n";
        writer->Update();
      }
      
      if (writeGordR)
      {
        typename ScalarWriterType::Pointer writer = ScalarWriterType::New();
        writer->SetInput( r1Im );
        itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_R1"+default_ext);
        if (verbose)
          std::cout << "Writing R1 image: " << outputFile << "_R1" << default_ext << "\n";
        writer->Update();
    
        writer = ScalarWriterType::New();
        writer->SetInput( r2Im );
        imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_R2"+default_ext);
        if (verbose)
          std::cout << "Writing R2 image: " << outputFile << "_R2" << default_ext << "\n";
        writer->Update();
    
        writer = ScalarWriterType::New();
        writer->SetInput( r3Im );
        imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_R3"+default_ext);
        if (verbose)
          std::cout << "Writing R3 image: " << outputFile << "_R3" << default_ext << "\n";
        writer->Update();
    
      }    
    
      if (writeGordK)
      {
        typename ScalarWriterType::Pointer writer = ScalarWriterType::New();
        writer->SetInput( k1Im );
        itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_K1"+default_ext);
        if (verbose)
          std::cout << "Writing K1 image: " << outputFile << "_K1" << default_ext << "\n";
        writer->Update();
    
        writer = ScalarWriterType::New();
        writer->SetInput( k2Im );
        imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_K2"+default_ext);
        if (verbose)
          std::cout << "Writing K2 image: " << outputFile << "_K2" << default_ext << "\n";
        writer->Update();
    
        writer = ScalarWriterType::New();
        writer->SetInput( k3Im );
        imageIO = itk::NiftiImageIO::New();
        writer->SetImageIO(imageIO);
        writer->SetFileName( outputFile+"_K3"+default_ext);
        if (verbose)
          std::cout << "Writing K3 image: " << outputFile << "_K3" << default_ext << "\n";
        writer->Update();
    
      }    
    }
    catch (itk::ExceptionObject &ex)
    {
      std::cerr << "Exception Caught!!\n" << ex << std::endl;
      return false;
    }
    
    return true;

  }

  inline void ComputeDtiScalars::runAlgorithm()
  {
    switch(m_component)
    {
    case itk::ImageIOBase::FLOAT:
      {
        std::cout << "Writing image(s) in directory: " << m_outputDir << "\n";
        if( !runner<float, float, 3>(m_inputFiles[0], m_outputBaseNames[0], m_extension, m_parameters) )
        {
          std::cerr << "Runner failed.\n";
          exit(EXIT_FAILURE);
        }
      }
    case itk::ImageIOBase::DOUBLE:
      {
        std::cout << "Writing image(s) in directory: " << m_outputDir << "\n";
        if( !runner<double, float, 3>(m_inputFiles[0], m_outputBaseNames[0], m_extension, m_parameters) )
        {
          std::cerr << "Runner failed.\n";
          exit(EXIT_FAILURE);
        }
      }
    default:
      {
        std::cerr << "Unsupported pixel component type: " << m_component_asString << "\n";
      }
    }
  }

}