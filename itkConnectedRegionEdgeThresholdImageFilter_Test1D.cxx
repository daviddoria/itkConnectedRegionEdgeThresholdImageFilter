#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"

#include "itkConnectedRegionEdgeThresholdImageFilter.h"

#include "itkCastImageFilter.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

int main( int argc, char *argv[])
{
  // Required parameters:
  // ./itkConnectedRegionEdgeThresholdImageFilter_Test1D original.png Seed Lower Upper output.png
  // e.g.
  // itkConnectedRegionEdgeThresholdImageFilter_Test1D input.png 120 5 5 output.png
  std::string strInputFilename = argv[1];
  std::string strSeed = argv[2];
  std::string strLower = argv[3];
  std::string strUpper = argv[4];
  std::string strOutputFilename = argv[5];
  
  std::cout << "Input: " << strInputFilename << std::endl;
  std::cout << "Output: " << strOutputFilename << std::endl;
  
  // The processing should be performed using float pixels so that
  // there are no problems when the result of a subtraction is negative
  // or outside the range of the pixel type, etc.
  typedef itk::Image< float, 1 >          InternalImageType;
  typedef itk::Image< unsigned char, 1 >  ExternalImageType;
  
  // Read the input image
  typedef itk::ImageFileReader<InternalImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(strInputFilename.c_str());
  reader->Update();
  
  // Prepare the seed
  std::stringstream ssSeed(strSeed);
  itk::Index<1> seed;
  ssSeed >> seed[0];
  std::cout << "Seed: " << seed[0] << std::endl;;

  // Convert lower and upper input strings to floats
  std::stringstream ssLower(strLower);
  float lower;
  ssLower >> lower;
  
  std::stringstream ssUpper(strUpper);
  float upper;
  ssUpper >> upper;
  
  std::cout << "Lower: " << lower << std::endl << "Upper: " << upper << std::endl;
  
  // Perform the region growing
  typedef itk::ConnectedRegionEdgeThresholdImageFilter < InternalImageType, InternalImageType> ConnectedFilterType;
  ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
  connectedThreshold->SetLower(lower);
  connectedThreshold->SetUpper(upper);
  connectedThreshold->SetReplaceValue(255);
  connectedThreshold->SetSeed(seed);
  connectedThreshold->SetInput(reader->GetOutput());
  connectedThreshold->Update();
  
  typedef itk::CastImageFilter< InternalImageType, ExternalImageType > CastFilterType;
  CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput(connectedThreshold->GetOutput() );
  castFilter->Update();
  
  // Write the output
  typedef  itk::ImageFileWriter< ExternalImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(strOutputFilename.c_str());
  writer->SetInput(castFilter->GetOutput());
  writer->Update();
  
  return EXIT_SUCCESS;
}