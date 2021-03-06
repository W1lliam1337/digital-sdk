/***************************************************************************/
/*                                                                         */
/*  ftrender.h                                                             */
/*                                                                         */
/*    FreeType renderer modules public interface (specification).          */
/*                                                                         */
/*  Copyright 1996-2017 by                                                 */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


#ifndef FTRENDER_H_
#define FTRENDER_H_


#include <ft2build.h>
#include FT_MODULE_H
#include FT_GLYPH_H


FT_BEGIN_HEADER


/*************************************************************************/
/*                                                                       */
/* <Section>                                                             */
/*    module_management                                                  */
/*                                                                       */
/*************************************************************************/


/* create a new glyph object */
using FT_Glyph_InitFunc = FT_Error(*)(FT_Glyph glyph,
                                      FT_GlyphSlot slot);

/* destroys a given glyph object */
using FT_Glyph_DoneFunc = void(*)(FT_Glyph glyph);

using FT_Glyph_TransformFunc = void(*)(FT_Glyph glyph,
                                       const FT_Matrix* matrix,
                                       const FT_Vector* delta);

using FT_Glyph_GetBBoxFunc = void(*)(FT_Glyph glyph,
                                     FT_BBox* abbox);

using FT_Glyph_CopyFunc = FT_Error(*)(FT_Glyph source,
                                      FT_Glyph target);

using FT_Glyph_PrepareFunc = FT_Error(*)(FT_Glyph glyph,
                                         FT_GlyphSlot slot);

/* deprecated */
#define FT_Glyph_Init_Func       FT_Glyph_InitFunc
#define FT_Glyph_Done_Func       FT_Glyph_DoneFunc
#define FT_Glyph_Transform_Func  FT_Glyph_TransformFunc
#define FT_Glyph_BBox_Func       FT_Glyph_GetBBoxFunc
#define FT_Glyph_Copy_Func       FT_Glyph_CopyFunc
#define FT_Glyph_Prepare_Func    FT_Glyph_PrepareFunc


struct FT_Glyph_Class_
{
	FT_Long glyph_size;
	FT_Glyph_Format glyph_format;

	FT_Glyph_InitFunc glyph_init;
	FT_Glyph_DoneFunc glyph_done;
	FT_Glyph_CopyFunc glyph_copy;
	FT_Glyph_TransformFunc glyph_transform;
	FT_Glyph_GetBBoxFunc glyph_bbox;
	FT_Glyph_PrepareFunc glyph_prepare;
};


using FT_Renderer_RenderFunc = FT_Error(*)(FT_Renderer renderer,
                                           FT_GlyphSlot slot,
                                           FT_UInt mode,
                                           const FT_Vector* origin);

using FT_Renderer_TransformFunc = FT_Error(*)(FT_Renderer renderer,
                                              FT_GlyphSlot slot,
                                              const FT_Matrix* matrix,
                                              const FT_Vector* delta);


using FT_Renderer_GetCBoxFunc = void(*)(FT_Renderer renderer,
                                        FT_GlyphSlot slot,
                                        FT_BBox* cbox);


using FT_Renderer_SetModeFunc = FT_Error(*)(FT_Renderer renderer,
                                            FT_ULong mode_tag,
                                            FT_Pointer mode_ptr);

/* deprecated identifiers */
#define FTRenderer_render  FT_Renderer_RenderFunc
#define FTRenderer_transform  FT_Renderer_TransformFunc
#define FTRenderer_getCBox  FT_Renderer_GetCBoxFunc
#define FTRenderer_setMode  FT_Renderer_SetModeFunc


/*************************************************************************/
/*                                                                       */
/* <Struct>                                                              */
/*    FT_Renderer_Class                                                  */
/*                                                                       */
/* <Description>                                                         */
/*    The renderer module class descriptor.                              */
/*                                                                       */
/* <Fields>                                                              */
/*    root            :: The root @FT_Module_Class fields.               */
/*                                                                       */
/*    glyph_format    :: The glyph image format this renderer handles.   */
/*                                                                       */
/*    render_glyph    :: A method used to render the image that is in a  */
/*                       given glyph slot into a bitmap.                 */
/*                                                                       */
/*    transform_glyph :: A method used to transform the image that is in */
/*                       a given glyph slot.                             */
/*                                                                       */
/*    get_glyph_cbox  :: A method used to access the glyph's cbox.       */
/*                                                                       */
/*    set_mode        :: A method used to pass additional parameters.    */
/*                                                                       */
/*    raster_class    :: For @FT_GLYPH_FORMAT_OUTLINE renderers only.    */
/*                       This is a pointer to its raster's class.        */
/*                                                                       */
using FT_Renderer_Class = struct FT_Renderer_Class_
{
	FT_Module_Class root;

	FT_Glyph_Format glyph_format;

	FT_Renderer_RenderFunc render_glyph;
	FT_Renderer_TransformFunc transform_glyph;
	FT_Renderer_GetCBoxFunc get_glyph_cbox;
	FT_Renderer_SetModeFunc set_mode;

	FT_Raster_Funcs* raster_class;
};


/*************************************************************************/
/*                                                                       */
/* <Function>                                                            */
/*    FT_Get_Renderer                                                    */
/*                                                                       */
/* <Description>                                                         */
/*    Retrieve the current renderer for a given glyph format.            */
/*                                                                       */
/* <Input>                                                               */
/*    library :: A handle to the library object.                         */
/*                                                                       */
/*    format  :: The glyph format.                                       */
/*                                                                       */
/* <Return>                                                              */
/*    A renderer handle.  0~if none found.                               */
/*                                                                       */
/* <Note>                                                                */
/*    An error will be returned if a module already exists by that name, */
/*    or if the module requires a version of FreeType that is too great. */
/*                                                                       */
/*    To add a new renderer, simply use @FT_Add_Module.  To retrieve a   */
/*    renderer by its name, use @FT_Get_Module.                          */
/*                                                                       */
FT_EXPORT(FT_Renderer)
FT_Get_Renderer(FT_Library library,
                FT_Glyph_Format format);


/*************************************************************************/
/*                                                                       */
/* <Function>                                                            */
/*    FT_Set_Renderer                                                    */
/*                                                                       */
/* <Description>                                                         */
/*    Set the current renderer to use, and set additional mode.          */
/*                                                                       */
/* <InOut>                                                               */
/*    library    :: A handle to the library object.                      */
/*                                                                       */
/* <Input>                                                               */
/*    renderer   :: A handle to the renderer object.                     */
/*                                                                       */
/*    num_params :: The number of additional parameters.                 */
/*                                                                       */
/*    parameters :: Additional parameters.                               */
/*                                                                       */
/* <Return>                                                              */
/*    FreeType error code.  0~means success.                             */
/*                                                                       */
/* <Note>                                                                */
/*    In case of success, the renderer will be used to convert glyph     */
/*    images in the renderer's known format into bitmaps.                */
/*                                                                       */
/*    This doesn't change the current renderer for other formats.        */
/*                                                                       */
/*    Currently, no FreeType renderer module uses `parameters'; you      */
/*    should thus always pass NULL as the value.                         */
/*                                                                       */
FT_EXPORT(FT_Error)
FT_Set_Renderer(FT_Library library,
                FT_Renderer renderer,
                FT_UInt num_params,
                FT_Parameter* parameters);

/* */


FT_END_HEADER

#endif /* FTRENDER_H_ */


/* END */
