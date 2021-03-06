/***************************************************************************/
/*                                                                         */
/*  svpfr.h                                                                */
/*                                                                         */
/*    Internal PFR service functions (specification).                      */
/*                                                                         */
/*  Copyright 2003-2017 by                                                 */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


#ifndef SVPFR_H_
#define SVPFR_H_

#include FT_PFR_H
#include FT_INTERNAL_SERVICE_H


FT_BEGIN_HEADER


#define FT_SERVICE_ID_PFR_METRICS  "pfr-metrics"


	typedef FT_Error
		        (*FT_PFR_GetMetricsFunc)( FT_Face    face
,
FT_UInt *aoutline,
                            FT_UInt   *ametrics,
                            FT_Fixed  *ax_scale,
                            FT_Fixed  *ay_scale
);

using FT_PFR_GetKerningFunc = FT_Error(*)(FT_Face face,
                                          FT_UInt left,
                                          FT_UInt right,
                                          FT_Vector* avector);

using FT_PFR_GetAdvanceFunc = FT_Error(*)(FT_Face face,
                                          FT_UInt gindex,
                                          FT_Pos* aadvance);


FT_DEFINE_SERVICE ( PfrMetrics )
{
	FT_PFR_GetMetricsFunc get_metrics;
	FT_PFR_GetKerningFunc get_kerning;
	FT_PFR_GetAdvanceFunc get_advance;
};

/* */

FT_END_HEADER

#endif /* SVPFR_H_ */


/* END */
