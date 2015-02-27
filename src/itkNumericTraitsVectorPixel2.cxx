/*!
 * \file  itkNumericTraitsVectorPixel2.cxx
 * \brief Definition of numeric traits for vector valued images.
 *
 * Copyright (c) 2011 University of Pennsylvania. All rights reserved.
 * See COPYING file or https://www.rad.upenn.edu/sbia/software/license.html.
 *
 * Contact: SBIA Group <sbia-software at uphs.upenn.edu>
 */

#include "itkNumericTraitsVectorPixel.h"

#define itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( GENERIC_ARRAY, T ) \
  itkStaticNumericTraitsGenericArrayMacro( GENERIC_ARRAY, T, 15 ); \
  itkStaticNumericTraitsGenericArrayMacro( GENERIC_ARRAY, T, 28 ); \
  itkStaticNumericTraitsGenericArrayMacro( GENERIC_ARRAY, T, 45 ); \
  itkStaticNumericTraitsGenericArrayMacro( GENERIC_ARRAY, T, 66 );

namespace itk
{

itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, char );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, unsigned char );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, signed char );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, short );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, unsigned short );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, int );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, unsigned int );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, long );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, unsigned long );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, float );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, double );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, long double );
#ifdef ITK_TYPE_USE_LONG_LONG
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, long long );
itkStaticNumericTraitsGenericArrayDimensionsMacroSBIA( Vector, unsigned long long );
#endif // ITK_TYPE_USE_LONG_LONG

} // end namespace itk
