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
  if(argc < 7)
  {
    std::cerr << "Required parameters: input.png SeedRow SeedCol Lower Upper output.png" << std::endl;
    return EXIT_FAILURE;
  }
  // Required parameters:
  // ./itkConnectedRegionEdgeThresholdImageFilter_Test2D input.png SeedRow SeedCol Lower Upper output.png
  // e.g.
  // ./itkConnectedRegionEdgeThresholdImageFilter_Test2D original.png 10 20 2 2 output.png
  std::string strInputFilename = argv[1];
  std::string strSeedRow = argv[2];
  std::string strSeedCol = argv[3];
  std::string strLower = argv[4];
  std::string strUpper = argv[5];
  std::string strOutputFilename = argv[6];

  // Convert the seed to integers
  itk::Index<2> seed;
  std::stringstream ssSeedRow(strSeedRow);
  ssSeedRow >> seed[1];

  std::stringstream ssSeedCol(strSeedCol);
  ssSeedCol >> seed[0];

  // Convert lower and upper to floats
  std::stringstream ssLower(strLower);
  float lower;
  ssLower >> lower;

  std::stringstream ssUpper(strUpper);
  float upper;
  ssUpper >> upper;

  std::cout << "Input: " << strInputFilename << std::endl;
  std::cout << "Seed: " << seed << std::endl;
  std::cout << "Lower threshold: " << lower << std::endl;
  std::cout << "Upper threshold: " << upper << std::endl;
  std::cout << "Output: " << strOutputFilename << std::endl;

  // Setup image types
  typedef itk::Image< float, 2>          InternalImageType;
  typedef itk::Image< unsigned char, 2>  ExternalImageType;

  // Read the input image
  typedef itk::ImageFileReader<InternalImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(strInputFilename.c_str());
  reader->Update();

  // Perform the region growing
  typedef itk::ConnectedRegionEdgeThresholdImageFilter < InternalImageType, InternalImageType>
                ConnectedFilterType;
  ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
  connectedThreshold->SetLower(lower);
  connectedThreshold->SetUpper(upper);
  connectedThreshold->SetReplaceValue(255);
  connectedThreshold->SetInput(reader->GetOutput());
  connectedThreshold->SetSeed(seed);
  connectedThreshold->Update();

  // Cast the result to the external image type
  typedef itk::CastImageFilter< InternalImageType, ExternalImageType > CastFilterType;
  CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput(connectedThreshold->GetOutput() );

  // Write the result
  typedef  itk::ImageFileWriter< ExternalImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(strOutputFilename.c_str());
  writer->SetInput(castFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
