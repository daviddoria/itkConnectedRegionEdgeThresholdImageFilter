/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRegionEdgeFunction.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-06 22:01:53 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRegionEdgeFunction_txx
#define __itkRegionEdgeFunction_txx

#include "itkRegionEdgeFunction.h"

namespace itk
{

template <class TInputImage, class TOutputImageType, class TCoordRep>
		RegionEdgeFunction<TInputImage,TOutputImageType, TCoordRep>
	::RegionEdgeFunction()
{
  m_Lower = NumericTraits<PixelType>::NonpositiveMin();
  m_Upper = NumericTraits<PixelType>::max();
  RegionSize = 0;
}

/**
 * Values greater than or equal to the value are inside
 */
template <class TInputImage, class TOutputImageType, class TCoordRep>
void 
		RegionEdgeFunction<TInputImage,TOutputImageType, TCoordRep>
::ThresholdAbove(PixelType thresh)
{
  if (m_Lower != thresh
      || m_Upper != NumericTraits<PixelType>::max())
    {
    m_Lower = thresh;
    m_Upper = NumericTraits<PixelType>::max();
    this->Modified();
    }
}

/**
 * The values less than or equal to the value are inside
 */
template <class TInputImage, class TOutputImageType, class TCoordRep>
void 
		RegionEdgeFunction<TInputImage,TOutputImageType, TCoordRep>
::ThresholdBelow(PixelType thresh)
{
  if (m_Lower != NumericTraits<PixelType>::NonpositiveMin()
      || m_Upper != thresh)
    {
    m_Lower = NumericTraits<PixelType>::NonpositiveMin();
    m_Upper = thresh;
    this->Modified();
    }
}

/**
 * The values less than or equal to the value are inside
 */
template <class TInputImage, class TOutputImageType, class TCoordRep>
void 
RegionEdgeFunction<TInputImage,TOutputImageType, TCoordRep>
::ThresholdBetween(PixelType lower, PixelType upper)
{
  if (m_Lower != lower
      || m_Upper != upper)
    {
    m_Lower = lower;
    m_Upper = upper;
    this->Modified();
    }
}

template <class TInputImage, class TOutputImageType, class TCoordRep>
void 
		RegionEdgeFunction<TInputImage,TOutputImageType, TCoordRep>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );

  os << indent << "Lower: " << m_Lower << std::endl;
  os << indent << "Upper: " << m_Upper << std::endl;
}

template <class TInputImage, class TOutputImageType, class TCoordRep>
bool 
RegionEdgeFunction<TInputImage,TOutputImageType, TCoordRep>
::EvaluateAtIndex( const IndexType & index ) const
{
  PixelType currentPixelValue = this->GetInputImage()->GetPixel(index);

  typedef ConstNeighborhoodIterator< InputImageType > NeighborhoodIteratorType;
  typename NeighborhoodIteratorType::RadiusType radius;
  radius.Fill(1);
  NeighborhoodIteratorType it(radius, this->GetInputImage(), this->GetInputImage()->GetRequestedRegion() );

  it.SetLocation(index);

  //std::cout << "Loop for index: " << index << std::endl;

  for (unsigned int i = 0; i < it.Size(); ++i)
    {
    if(RegionSize == 0)
      {
      //always add the first pixel to the region
      RegionSize++;
      return true;
      }
    //std::cout << "Checking " << index << " against " << it.GetIndex(i) << std::endl;

    typename TOutputImageType::IndexType OutputImagePixelIndex = it.GetIndex(i);

    // Do not check the pixel against itself
    if(OutputImagePixelIndex == index)
      {
      continue;
      }

    // Get the current neighbor value
    bool valid = true;
    PixelType CurrentNeighbor = it.GetPixel(i, valid);

    // Don't use this neighbor if it is outside of the image
    if(!valid)
      {
      continue;
      }

    // Don't compare if the current neighbor is not inside the object region
    OutputImagePixelType OutputImagePixel = OutputImage->GetPixel(OutputImagePixelIndex);
    if(OutputImagePixel == NumericTraits<OutputImagePixelType>::Zero)
      {
      continue;
      }

    // if value falls in the acceptable range
    if( (CurrentNeighbor - m_Lower) <= currentPixelValue &&
        (CurrentNeighbor + m_Upper) >= currentPixelValue )
      {
      //std::cout << "Add pixel " << index << " to region!" << std::endl << std::endl;
      return true;
      }
    }

  //std::cout << "Do NOT add pixel " << index << " to region!" << std::endl << std::endl;
  return false;
}

} // end namespace itk

#endif
