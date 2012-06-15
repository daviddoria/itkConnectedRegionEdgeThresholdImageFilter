/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRegionEdgeFunction.h,v $
  Language:  C++
  Date:      $Date: 2007-02-07 14:00:14 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRegionEdgeFunction_h
#define __itkRegionEdgeFunction_h

#include "itkImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
		
namespace itk
{

/** \class RegionEdgeFunction
 * \brief Returns true if the value of an image lies within tolerance
 *        of any of its neighbors already in the region
 * This ImageFunction returns true if the pixel value lies
 * within a lower and upper threshold value. The threshold
 * range can be set with the ThresholdBelow, ThresholdBetween or
 * ThresholdAbove methods.  The input image is set via method
 * SetInputImage().
 *
 * Methods Evaluate, EvaluateAtIndex and EvaluateAtContinuousIndex
 * respectively evaluate the function at an geometric point, image index
 * and continuous image index.
 *
 * \ingroup ImageFunctions
 * 
 */
template <class TInputImage, class TOutputImageType, class TCoordRep = float>
class ITK_EXPORT RegionEdgeFunction : 
  public ImageFunction<TInputImage,bool,TCoordRep> 
{
public:
  /** Standard class typedefs. */
  typedef RegionEdgeFunction                        Self;
  typedef ImageFunction<TInputImage,bool,TCoordRep> Superclass;
  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;
  
  typedef typename TOutputImageType::Pointer OutputImagePointer;
  typedef typename TOutputImageType::PixelType OutputImagePixelType;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(RegionEdgeFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef typename Superclass::InputImageType InputImageType;
  
  /** Typedef to describe the type of pixel. */
  typedef typename TInputImage::PixelType PixelType;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);

  /** Point typedef support. */
  typedef typename Superclass::PointType PointType;

  /** Index typedef support. */
  typedef typename Superclass::IndexType IndexType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  /** Test RegionEdge criteria the image at a point position
   *
   * Returns true if the image intensity at the specified point position
   * satisfies the threshold criteria.  The point is assumed to lie within
   * the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */

  virtual bool Evaluate( const PointType& point ) const
    {
    IndexType index;
    this->ConvertPointToNearestIndex( point, index );
    return ( this->EvaluateAtIndex( index ) );
    }
    
    void SetOutputImagePointer( const OutputImagePointer output_im )
    {
	    OutputImage = output_im;
    }

  /** Test RegionEdge criteria the image at a continuous index position
   *
   * Returns true if the image intensity at the specified point position
   * satisfies the threshold criteria.  The point is assumed to lie within
   * the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual bool EvaluateAtContinuousIndex( 
    const ContinuousIndexType & index ) const
    {
    IndexType nindex;

    this->ConvertContinuousIndexToNearestIndex (index, nindex);
    return this->EvaluateAtIndex(nindex);
    }

  /** Test RegionEdge criteria the image at an index position.
   *
   * Returns true if the image intensity at the specified point position
   * satisfies the threshold criteria.  The point is assumed to lie within
   * the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual bool EvaluateAtIndex( const IndexType & index ) const;

  /** Get the lower threshold value. */
  itkGetConstReferenceMacro(Lower,PixelType);

  /** Get the upper threshold value. */
  itkGetConstReferenceMacro(Upper,PixelType);

  /** Values greater than or equal to the value are inside. */
  void ThresholdAbove(PixelType thresh);
  
  /** Values less than or equal to the value are inside. */
  void ThresholdBelow(PixelType thresh);

  /** Values that lie between lower and upper inclusive are inside. */
  void ThresholdBetween(PixelType lower, PixelType upper);

protected:
	RegionEdgeFunction();
	~RegionEdgeFunction(){};
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
	RegionEdgeFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  PixelType m_Lower;
  PixelType m_Upper;
  OutputImagePointer OutputImage;
  mutable unsigned int RegionSize;
};

} // end namespace itk


// Define instantiation macro for this template.
#define ITK_TEMPLATE_RegionEdgeFunction(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT RegionEdgeFunction< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef RegionEdgeFunction< ITK_TEMPLATE_2 x > \
                               RegionEdgeFunction##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkRegionEdgeFunction+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkRegionEdgeFunction.txx"
#endif

#endif
