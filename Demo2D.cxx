#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"

#include "itkConnectedRegionEdgeThresholdImageFilter.h"

#include "itkCastImageFilter.h"

#include <iostream>

typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;

static void CreateImage(UnsignedCharImageType* const image);

int main( int argc, char *argv[])
{
  itk::Index<2> seed = {{50,50}};
  float lower = 80;
  float upper = 110;
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  CreateImage(image);

  // Perform the region growing
  typedef itk::ConnectedRegionEdgeThresholdImageFilter < UnsignedCharImageType, UnsignedCharImageType>
                ConnectedFilterType;
  ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
  connectedThreshold->SetLower(lower);
  connectedThreshold->SetUpper(upper);
  connectedThreshold->SetReplaceValue(255);
  connectedThreshold->SetInput(image);
  connectedThreshold->SetSeed(seed);
  connectedThreshold->Update();

  // Write the result
  typedef  itk::ImageFileWriter< UnsignedCharImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("demo2D.png");
  writer->SetInput(connectedThreshold->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

void CreateImage(UnsignedCharImageType* const image)
{
  UnsignedCharImageType::IndexType corner = {{0,0}};

  UnsignedCharImageType::SizeType size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  UnsignedCharImageType::RegionType region(corner, size);

  image->SetRegions(region);
  image->Allocate();

  // Make a square
  for(unsigned int r = 40; r < 100; r++)
    {
    for(unsigned int c = 40; c < 100; c++)
      {
      UnsignedCharImageType::IndexType pixelIndex = {{r,c}};

      image->SetPixel(pixelIndex, 100);
      }
    }
}
