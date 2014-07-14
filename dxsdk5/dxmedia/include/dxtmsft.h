/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.02.88 */
/* at Wed Sep 02 08:16:12 1998
 */
/* Compiler settings for dxtmsft.idl:
    Oicf (OptLev=i2), W0, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"
#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __dxtmsft_h__
#define __dxtmsft_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IDXLUTBuilder_FWD_DEFINED__
#define __IDXLUTBuilder_FWD_DEFINED__
typedef interface IDXLUTBuilder IDXLUTBuilder;
#endif 	/* __IDXLUTBuilder_FWD_DEFINED__ */


#ifndef __IDXDLUTBuilder_FWD_DEFINED__
#define __IDXDLUTBuilder_FWD_DEFINED__
typedef interface IDXDLUTBuilder IDXDLUTBuilder;
#endif 	/* __IDXDLUTBuilder_FWD_DEFINED__ */


#ifndef __IDXTResampler_FWD_DEFINED__
#define __IDXTResampler_FWD_DEFINED__
typedef interface IDXTResampler IDXTResampler;
#endif 	/* __IDXTResampler_FWD_DEFINED__ */


#ifndef __IDXTResamplerD_FWD_DEFINED__
#define __IDXTResamplerD_FWD_DEFINED__
typedef interface IDXTResamplerD IDXTResamplerD;
#endif 	/* __IDXTResamplerD_FWD_DEFINED__ */


#ifndef __IDXTGradientD_FWD_DEFINED__
#define __IDXTGradientD_FWD_DEFINED__
typedef interface IDXTGradientD IDXTGradientD;
#endif 	/* __IDXTGradientD_FWD_DEFINED__ */


#ifndef __IDXTConvolution_FWD_DEFINED__
#define __IDXTConvolution_FWD_DEFINED__
typedef interface IDXTConvolution IDXTConvolution;
#endif 	/* __IDXTConvolution_FWD_DEFINED__ */


#ifndef __IDXMapper_FWD_DEFINED__
#define __IDXMapper_FWD_DEFINED__
typedef interface IDXMapper IDXMapper;
#endif 	/* __IDXMapper_FWD_DEFINED__ */


#ifndef __IDXDMapper_FWD_DEFINED__
#define __IDXDMapper_FWD_DEFINED__
typedef interface IDXDMapper IDXDMapper;
#endif 	/* __IDXDMapper_FWD_DEFINED__ */


#ifndef __IDXTComposite_FWD_DEFINED__
#define __IDXTComposite_FWD_DEFINED__
typedef interface IDXTComposite IDXTComposite;
#endif 	/* __IDXTComposite_FWD_DEFINED__ */


#ifndef __IExplode_FWD_DEFINED__
#define __IExplode_FWD_DEFINED__
typedef interface IExplode IExplode;
#endif 	/* __IExplode_FWD_DEFINED__ */


#ifndef __IRuffle_FWD_DEFINED__
#define __IRuffle_FWD_DEFINED__
typedef interface IRuffle IRuffle;
#endif 	/* __IRuffle_FWD_DEFINED__ */


#ifndef __IRipple_FWD_DEFINED__
#define __IRipple_FWD_DEFINED__
typedef interface IRipple IRipple;
#endif 	/* __IRipple_FWD_DEFINED__ */


#ifndef __IDissolve_FWD_DEFINED__
#define __IDissolve_FWD_DEFINED__
typedef interface IDissolve IDissolve;
#endif 	/* __IDissolve_FWD_DEFINED__ */


#ifndef __IFlip_FWD_DEFINED__
#define __IFlip_FWD_DEFINED__
typedef interface IFlip IFlip;
#endif 	/* __IFlip_FWD_DEFINED__ */


#ifndef __IHeightField_FWD_DEFINED__
#define __IHeightField_FWD_DEFINED__
typedef interface IHeightField IHeightField;
#endif 	/* __IHeightField_FWD_DEFINED__ */


#ifndef __IDXTWipe_FWD_DEFINED__
#define __IDXTWipe_FWD_DEFINED__
typedef interface IDXTWipe IDXTWipe;
#endif 	/* __IDXTWipe_FWD_DEFINED__ */


#ifndef __ICrBlur_FWD_DEFINED__
#define __ICrBlur_FWD_DEFINED__
typedef interface ICrBlur ICrBlur;
#endif 	/* __ICrBlur_FWD_DEFINED__ */


#ifndef __ICrEngrave_FWD_DEFINED__
#define __ICrEngrave_FWD_DEFINED__
typedef interface ICrEngrave ICrEngrave;
#endif 	/* __ICrEngrave_FWD_DEFINED__ */


#ifndef __ICrEmboss_FWD_DEFINED__
#define __ICrEmboss_FWD_DEFINED__
typedef interface ICrEmboss ICrEmboss;
#endif 	/* __ICrEmboss_FWD_DEFINED__ */


#ifndef __IDXTFade_FWD_DEFINED__
#define __IDXTFade_FWD_DEFINED__
typedef interface IDXTFade IDXTFade;
#endif 	/* __IDXTFade_FWD_DEFINED__ */


#ifndef __IDXBasicImage_FWD_DEFINED__
#define __IDXBasicImage_FWD_DEFINED__
typedef interface IDXBasicImage IDXBasicImage;
#endif 	/* __IDXBasicImage_FWD_DEFINED__ */


#ifndef __IDXPixelate_FWD_DEFINED__
#define __IDXPixelate_FWD_DEFINED__
typedef interface IDXPixelate IDXPixelate;
#endif 	/* __IDXPixelate_FWD_DEFINED__ */


#ifndef __Explode_FWD_DEFINED__
#define __Explode_FWD_DEFINED__

#ifdef __cplusplus
typedef class Explode Explode;
#else
typedef struct Explode Explode;
#endif /* __cplusplus */

#endif 	/* __Explode_FWD_DEFINED__ */


#ifndef __ExplodeProp_FWD_DEFINED__
#define __ExplodeProp_FWD_DEFINED__

#ifdef __cplusplus
typedef class ExplodeProp ExplodeProp;
#else
typedef struct ExplodeProp ExplodeProp;
#endif /* __cplusplus */

#endif 	/* __ExplodeProp_FWD_DEFINED__ */


#ifndef __Ruffle_FWD_DEFINED__
#define __Ruffle_FWD_DEFINED__

#ifdef __cplusplus
typedef class Ruffle Ruffle;
#else
typedef struct Ruffle Ruffle;
#endif /* __cplusplus */

#endif 	/* __Ruffle_FWD_DEFINED__ */


#ifndef __RuffleProp_FWD_DEFINED__
#define __RuffleProp_FWD_DEFINED__

#ifdef __cplusplus
typedef class RuffleProp RuffleProp;
#else
typedef struct RuffleProp RuffleProp;
#endif /* __cplusplus */

#endif 	/* __RuffleProp_FWD_DEFINED__ */


#ifndef __Ripple_FWD_DEFINED__
#define __Ripple_FWD_DEFINED__

#ifdef __cplusplus
typedef class Ripple Ripple;
#else
typedef struct Ripple Ripple;
#endif /* __cplusplus */

#endif 	/* __Ripple_FWD_DEFINED__ */


#ifndef __RipProp_FWD_DEFINED__
#define __RipProp_FWD_DEFINED__

#ifdef __cplusplus
typedef class RipProp RipProp;
#else
typedef struct RipProp RipProp;
#endif /* __cplusplus */

#endif 	/* __RipProp_FWD_DEFINED__ */


#ifndef __Dissolve_FWD_DEFINED__
#define __Dissolve_FWD_DEFINED__

#ifdef __cplusplus
typedef class Dissolve Dissolve;
#else
typedef struct Dissolve Dissolve;
#endif /* __cplusplus */

#endif 	/* __Dissolve_FWD_DEFINED__ */


#ifndef __Flip_FWD_DEFINED__
#define __Flip_FWD_DEFINED__

#ifdef __cplusplus
typedef class Flip Flip;
#else
typedef struct Flip Flip;
#endif /* __cplusplus */

#endif 	/* __Flip_FWD_DEFINED__ */


#ifndef __HeightField_FWD_DEFINED__
#define __HeightField_FWD_DEFINED__

#ifdef __cplusplus
typedef class HeightField HeightField;
#else
typedef struct HeightField HeightField;
#endif /* __cplusplus */

#endif 	/* __HeightField_FWD_DEFINED__ */


#ifndef __DXTComposite_FWD_DEFINED__
#define __DXTComposite_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXTComposite DXTComposite;
#else
typedef struct DXTComposite DXTComposite;
#endif /* __cplusplus */

#endif 	/* __DXTComposite_FWD_DEFINED__ */


#ifndef __DXTResampler_FWD_DEFINED__
#define __DXTResampler_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXTResampler DXTResampler;
#else
typedef struct DXTResampler DXTResampler;
#endif /* __cplusplus */

#endif 	/* __DXTResampler_FWD_DEFINED__ */


#ifndef __DXBufferCache_FWD_DEFINED__
#define __DXBufferCache_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXBufferCache DXBufferCache;
#else
typedef struct DXBufferCache DXBufferCache;
#endif /* __cplusplus */

#endif 	/* __DXBufferCache_FWD_DEFINED__ */


#ifndef __DXLUTBuilder_FWD_DEFINED__
#define __DXLUTBuilder_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXLUTBuilder DXLUTBuilder;
#else
typedef struct DXLUTBuilder DXLUTBuilder;
#endif /* __cplusplus */

#endif 	/* __DXLUTBuilder_FWD_DEFINED__ */


#ifndef __DXTGradientD_FWD_DEFINED__
#define __DXTGradientD_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXTGradientD DXTGradientD;
#else
typedef struct DXTGradientD DXTGradientD;
#endif /* __cplusplus */

#endif 	/* __DXTGradientD_FWD_DEFINED__ */


#ifndef __DXTWipe_FWD_DEFINED__
#define __DXTWipe_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXTWipe DXTWipe;
#else
typedef struct DXTWipe DXTWipe;
#endif /* __cplusplus */

#endif 	/* __DXTWipe_FWD_DEFINED__ */


#ifndef __DXTConvolution_FWD_DEFINED__
#define __DXTConvolution_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXTConvolution DXTConvolution;
#else
typedef struct DXTConvolution DXTConvolution;
#endif /* __cplusplus */

#endif 	/* __DXTConvolution_FWD_DEFINED__ */


#ifndef __CrBlur_FWD_DEFINED__
#define __CrBlur_FWD_DEFINED__

#ifdef __cplusplus
typedef class CrBlur CrBlur;
#else
typedef struct CrBlur CrBlur;
#endif /* __cplusplus */

#endif 	/* __CrBlur_FWD_DEFINED__ */


#ifndef __CrEmboss_FWD_DEFINED__
#define __CrEmboss_FWD_DEFINED__

#ifdef __cplusplus
typedef class CrEmboss CrEmboss;
#else
typedef struct CrEmboss CrEmboss;
#endif /* __cplusplus */

#endif 	/* __CrEmboss_FWD_DEFINED__ */


#ifndef __CrEngrave_FWD_DEFINED__
#define __CrEngrave_FWD_DEFINED__

#ifdef __cplusplus
typedef class CrEngrave CrEngrave;
#else
typedef struct CrEngrave CrEngrave;
#endif /* __cplusplus */

#endif 	/* __CrEngrave_FWD_DEFINED__ */


#ifndef __DXFade_FWD_DEFINED__
#define __DXFade_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXFade DXFade;
#else
typedef struct DXFade DXFade;
#endif /* __cplusplus */

#endif 	/* __DXFade_FWD_DEFINED__ */


#ifndef __FadePP_FWD_DEFINED__
#define __FadePP_FWD_DEFINED__

#ifdef __cplusplus
typedef class FadePP FadePP;
#else
typedef struct FadePP FadePP;
#endif /* __cplusplus */

#endif 	/* __FadePP_FWD_DEFINED__ */


#ifndef __BasicImageEffects_FWD_DEFINED__
#define __BasicImageEffects_FWD_DEFINED__

#ifdef __cplusplus
typedef class BasicImageEffects BasicImageEffects;
#else
typedef struct BasicImageEffects BasicImageEffects;
#endif /* __cplusplus */

#endif 	/* __BasicImageEffects_FWD_DEFINED__ */


#ifndef __BasicImageEffectsPP_FWD_DEFINED__
#define __BasicImageEffectsPP_FWD_DEFINED__

#ifdef __cplusplus
typedef class BasicImageEffectsPP BasicImageEffectsPP;
#else
typedef struct BasicImageEffectsPP BasicImageEffectsPP;
#endif /* __cplusplus */

#endif 	/* __BasicImageEffectsPP_FWD_DEFINED__ */


#ifndef __Pixelate_FWD_DEFINED__
#define __Pixelate_FWD_DEFINED__

#ifdef __cplusplus
typedef class Pixelate Pixelate;
#else
typedef struct Pixelate Pixelate;
#endif /* __cplusplus */

#endif 	/* __Pixelate_FWD_DEFINED__ */


#ifndef __PixelatePP_FWD_DEFINED__
#define __PixelatePP_FWD_DEFINED__

#ifdef __cplusplus
typedef class PixelatePP PixelatePP;
#else
typedef struct PixelatePP PixelatePP;
#endif /* __cplusplus */

#endif 	/* __PixelatePP_FWD_DEFINED__ */


#ifndef __DXTWipePP_FWD_DEFINED__
#define __DXTWipePP_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXTWipePP DXTWipePP;
#else
typedef struct DXTWipePP DXTWipePP;
#endif /* __cplusplus */

#endif 	/* __DXTWipePP_FWD_DEFINED__ */


#ifndef __ResamplePP_FWD_DEFINED__
#define __ResamplePP_FWD_DEFINED__

#ifdef __cplusplus
typedef class ResamplePP ResamplePP;
#else
typedef struct ResamplePP ResamplePP;
#endif /* __cplusplus */

#endif 	/* __ResamplePP_FWD_DEFINED__ */


#ifndef __CrBlurPP_FWD_DEFINED__
#define __CrBlurPP_FWD_DEFINED__

#ifdef __cplusplus
typedef class CrBlurPP CrBlurPP;
#else
typedef struct CrBlurPP CrBlurPP;
#endif /* __cplusplus */

#endif 	/* __CrBlurPP_FWD_DEFINED__ */


#ifndef __GradientPP_FWD_DEFINED__
#define __GradientPP_FWD_DEFINED__

#ifdef __cplusplus
typedef class GradientPP GradientPP;
#else
typedef struct GradientPP GradientPP;
#endif /* __cplusplus */

#endif 	/* __GradientPP_FWD_DEFINED__ */


#ifndef __CompositePP_FWD_DEFINED__
#define __CompositePP_FWD_DEFINED__

#ifdef __cplusplus
typedef class CompositePP CompositePP;
#else
typedef struct CompositePP CompositePP;
#endif /* __cplusplus */

#endif 	/* __CompositePP_FWD_DEFINED__ */


#ifndef __ConvolvePP_FWD_DEFINED__
#define __ConvolvePP_FWD_DEFINED__

#ifdef __cplusplus
typedef class ConvolvePP ConvolvePP;
#else
typedef struct ConvolvePP ConvolvePP;
#endif /* __cplusplus */

#endif 	/* __ConvolvePP_FWD_DEFINED__ */


#ifndef __LUTBuilderPP_FWD_DEFINED__
#define __LUTBuilderPP_FWD_DEFINED__

#ifdef __cplusplus
typedef class LUTBuilderPP LUTBuilderPP;
#else
typedef struct LUTBuilderPP LUTBuilderPP;
#endif /* __cplusplus */

#endif 	/* __LUTBuilderPP_FWD_DEFINED__ */


#ifndef __FlipProp_FWD_DEFINED__
#define __FlipProp_FWD_DEFINED__

#ifdef __cplusplus
typedef class FlipProp FlipProp;
#else
typedef struct FlipProp FlipProp;
#endif /* __cplusplus */

#endif 	/* __FlipProp_FWD_DEFINED__ */


#ifndef __HtFieldProp_FWD_DEFINED__
#define __HtFieldProp_FWD_DEFINED__

#ifdef __cplusplus
typedef class HtFieldProp HtFieldProp;
#else
typedef struct HtFieldProp HtFieldProp;
#endif /* __cplusplus */

#endif 	/* __HtFieldProp_FWD_DEFINED__ */


#ifndef __Disslvpp_FWD_DEFINED__
#define __Disslvpp_FWD_DEFINED__

#ifdef __cplusplus
typedef class Disslvpp Disslvpp;
#else
typedef struct Disslvpp Disslvpp;
#endif /* __cplusplus */

#endif 	/* __Disslvpp_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "dxtrans.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0000
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 









typedef 
enum OPIDDXLUTBUILDER
    {	OPID_DXLUTBUILDER_Gamma	= 0,
	OPID_DXLUTBUILDER_Opacity	= OPID_DXLUTBUILDER_Gamma + 1,
	OPID_DXLUTBUILDER_Brightness	= OPID_DXLUTBUILDER_Opacity + 1,
	OPID_DXLUTBUILDER_Contrast	= OPID_DXLUTBUILDER_Brightness + 1,
	OPID_DXLUTBUILDER_ColorBalance	= OPID_DXLUTBUILDER_Contrast + 1,
	OPID_DXLUTBUILDER_Posterize	= OPID_DXLUTBUILDER_ColorBalance + 1,
	OPID_DXLUTBUILDER_Invert	= OPID_DXLUTBUILDER_Posterize + 1,
	OPID_DXLUTBUILDER_Threshold	= OPID_DXLUTBUILDER_Invert + 1,
	OPID_DXLUTBUILDER_NUM_OPS	= OPID_DXLUTBUILDER_Threshold + 1
    }	OPIDDXLUTBUILDER;

typedef 
enum DXLUTCOLOR
    {	DXLUTCOLOR_RED	= 0,
	DXLUTCOLOR_GREEN	= DXLUTCOLOR_RED + 1,
	DXLUTCOLOR_BLUE	= DXLUTCOLOR_GREEN + 1
    }	DXLUTCOLOR;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0000_v0_0_s_ifspec;

#ifndef __IDXLUTBuilder_INTERFACE_DEFINED__
#define __IDXLUTBuilder_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXLUTBuilder
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [object][unique][helpstring][uuid] */ 



EXTERN_C const IID IID_IDXLUTBuilder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("F4370FC1-CADB-11D0-B52C-00A0C9054373")
    IDXLUTBuilder : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetNumBuildSteps( 
            /* [out] */ ULONG __RPC_FAR *pulNumSteps) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBuildOrder( 
            /* [size_is][out] */ OPIDDXLUTBUILDER __RPC_FAR OpOrder[  ],
            /* [in] */ ULONG ulSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetBuildOrder( 
            /* [size_is][in] */ const OPIDDXLUTBUILDER __RPC_FAR OpOrder[  ],
            /* [in] */ ULONG ulNumSteps) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetGamma( 
            /* [in] */ float newVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetGamma( 
            /* [out] */ float __RPC_FAR *pVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOpacity( 
            /* [out] */ float __RPC_FAR *pVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetOpacity( 
            /* [in] */ float newVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBrightness( 
            /* [out][in] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][out] */ float __RPC_FAR Weights[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetBrightness( 
            /* [in] */ ULONG ulCount,
            /* [size_is][in] */ const float __RPC_FAR Weights[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContrast( 
            /* [out][in] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][out] */ float __RPC_FAR Weights[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetContrast( 
            /* [in] */ ULONG ulCount,
            /* [size_is][in] */ const float __RPC_FAR Weights[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetColorBalance( 
            /* [in] */ DXLUTCOLOR Color,
            /* [out][in] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][out] */ float __RPC_FAR Weights[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetColorBalance( 
            /* [in] */ DXLUTCOLOR Color,
            /* [in] */ ULONG ulCount,
            /* [size_is][in] */ const float __RPC_FAR Weights[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLevelsPerChannel( 
            /* [out] */ ULONG __RPC_FAR *pVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLevelsPerChannel( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInvert( 
            /* [out] */ float __RPC_FAR *pThreshold) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetInvert( 
            /* [in] */ float Threshold) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetThreshold( 
            /* [out] */ float __RPC_FAR *pVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetThreshold( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXLUTBuilderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXLUTBuilder __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXLUTBuilder __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNumBuildSteps )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulNumSteps);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBuildOrder )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [size_is][out] */ OPIDDXLUTBUILDER __RPC_FAR OpOrder[  ],
            /* [in] */ ULONG ulSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBuildOrder )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [size_is][in] */ const OPIDDXLUTBUILDER __RPC_FAR OpOrder[  ],
            /* [in] */ ULONG ulNumSteps);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetGamma )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ float newVal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetGamma )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *pVal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOpacity )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *pVal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOpacity )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ float newVal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBrightness )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [out][in] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][out] */ float __RPC_FAR Weights[  ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBrightness )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ ULONG ulCount,
            /* [size_is][in] */ const float __RPC_FAR Weights[  ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetContrast )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [out][in] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][out] */ float __RPC_FAR Weights[  ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetContrast )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ ULONG ulCount,
            /* [size_is][in] */ const float __RPC_FAR Weights[  ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetColorBalance )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ DXLUTCOLOR Color,
            /* [out][in] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][out] */ float __RPC_FAR Weights[  ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetColorBalance )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ DXLUTCOLOR Color,
            /* [in] */ ULONG ulCount,
            /* [size_is][in] */ const float __RPC_FAR Weights[  ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLevelsPerChannel )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pVal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLevelsPerChannel )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ ULONG newVal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInvert )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *pThreshold);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetInvert )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ float Threshold);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetThreshold )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *pVal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetThreshold )( 
            IDXLUTBuilder __RPC_FAR * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } IDXLUTBuilderVtbl;

    interface IDXLUTBuilder
    {
        CONST_VTBL struct IDXLUTBuilderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXLUTBuilder_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXLUTBuilder_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXLUTBuilder_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXLUTBuilder_GetNumBuildSteps(This,pulNumSteps)	\
    (This)->lpVtbl -> GetNumBuildSteps(This,pulNumSteps)

#define IDXLUTBuilder_GetBuildOrder(This,OpOrder,ulSize)	\
    (This)->lpVtbl -> GetBuildOrder(This,OpOrder,ulSize)

#define IDXLUTBuilder_SetBuildOrder(This,OpOrder,ulNumSteps)	\
    (This)->lpVtbl -> SetBuildOrder(This,OpOrder,ulNumSteps)

#define IDXLUTBuilder_SetGamma(This,newVal)	\
    (This)->lpVtbl -> SetGamma(This,newVal)

#define IDXLUTBuilder_GetGamma(This,pVal)	\
    (This)->lpVtbl -> GetGamma(This,pVal)

#define IDXLUTBuilder_GetOpacity(This,pVal)	\
    (This)->lpVtbl -> GetOpacity(This,pVal)

#define IDXLUTBuilder_SetOpacity(This,newVal)	\
    (This)->lpVtbl -> SetOpacity(This,newVal)

#define IDXLUTBuilder_GetBrightness(This,pulCount,Weights)	\
    (This)->lpVtbl -> GetBrightness(This,pulCount,Weights)

#define IDXLUTBuilder_SetBrightness(This,ulCount,Weights)	\
    (This)->lpVtbl -> SetBrightness(This,ulCount,Weights)

#define IDXLUTBuilder_GetContrast(This,pulCount,Weights)	\
    (This)->lpVtbl -> GetContrast(This,pulCount,Weights)

#define IDXLUTBuilder_SetContrast(This,ulCount,Weights)	\
    (This)->lpVtbl -> SetContrast(This,ulCount,Weights)

#define IDXLUTBuilder_GetColorBalance(This,Color,pulCount,Weights)	\
    (This)->lpVtbl -> GetColorBalance(This,Color,pulCount,Weights)

#define IDXLUTBuilder_SetColorBalance(This,Color,ulCount,Weights)	\
    (This)->lpVtbl -> SetColorBalance(This,Color,ulCount,Weights)

#define IDXLUTBuilder_GetLevelsPerChannel(This,pVal)	\
    (This)->lpVtbl -> GetLevelsPerChannel(This,pVal)

#define IDXLUTBuilder_SetLevelsPerChannel(This,newVal)	\
    (This)->lpVtbl -> SetLevelsPerChannel(This,newVal)

#define IDXLUTBuilder_GetInvert(This,pThreshold)	\
    (This)->lpVtbl -> GetInvert(This,pThreshold)

#define IDXLUTBuilder_SetInvert(This,Threshold)	\
    (This)->lpVtbl -> SetInvert(This,Threshold)

#define IDXLUTBuilder_GetThreshold(This,pVal)	\
    (This)->lpVtbl -> GetThreshold(This,pVal)

#define IDXLUTBuilder_SetThreshold(This,newVal)	\
    (This)->lpVtbl -> SetThreshold(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetNumBuildSteps_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulNumSteps);


void __RPC_STUB IDXLUTBuilder_GetNumBuildSteps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetBuildOrder_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [size_is][out] */ OPIDDXLUTBUILDER __RPC_FAR OpOrder[  ],
    /* [in] */ ULONG ulSize);


void __RPC_STUB IDXLUTBuilder_GetBuildOrder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_SetBuildOrder_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [size_is][in] */ const OPIDDXLUTBUILDER __RPC_FAR OpOrder[  ],
    /* [in] */ ULONG ulNumSteps);


void __RPC_STUB IDXLUTBuilder_SetBuildOrder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_SetGamma_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDXLUTBuilder_SetGamma_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetGamma_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDXLUTBuilder_GetGamma_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetOpacity_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDXLUTBuilder_GetOpacity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_SetOpacity_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDXLUTBuilder_SetOpacity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetBrightness_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [out][in] */ ULONG __RPC_FAR *pulCount,
    /* [size_is][out] */ float __RPC_FAR Weights[  ]);


void __RPC_STUB IDXLUTBuilder_GetBrightness_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_SetBrightness_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [in] */ ULONG ulCount,
    /* [size_is][in] */ const float __RPC_FAR Weights[  ]);


void __RPC_STUB IDXLUTBuilder_SetBrightness_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetContrast_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [out][in] */ ULONG __RPC_FAR *pulCount,
    /* [size_is][out] */ float __RPC_FAR Weights[  ]);


void __RPC_STUB IDXLUTBuilder_GetContrast_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_SetContrast_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [in] */ ULONG ulCount,
    /* [size_is][in] */ const float __RPC_FAR Weights[  ]);


void __RPC_STUB IDXLUTBuilder_SetContrast_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetColorBalance_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [in] */ DXLUTCOLOR Color,
    /* [out][in] */ ULONG __RPC_FAR *pulCount,
    /* [size_is][out] */ float __RPC_FAR Weights[  ]);


void __RPC_STUB IDXLUTBuilder_GetColorBalance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_SetColorBalance_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [in] */ DXLUTCOLOR Color,
    /* [in] */ ULONG ulCount,
    /* [size_is][in] */ const float __RPC_FAR Weights[  ]);


void __RPC_STUB IDXLUTBuilder_SetColorBalance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetLevelsPerChannel_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pVal);


void __RPC_STUB IDXLUTBuilder_GetLevelsPerChannel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_SetLevelsPerChannel_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [in] */ ULONG newVal);


void __RPC_STUB IDXLUTBuilder_SetLevelsPerChannel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetInvert_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *pThreshold);


void __RPC_STUB IDXLUTBuilder_GetInvert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_SetInvert_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [in] */ float Threshold);


void __RPC_STUB IDXLUTBuilder_SetInvert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_GetThreshold_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDXLUTBuilder_GetThreshold_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXLUTBuilder_SetThreshold_Proxy( 
    IDXLUTBuilder __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDXLUTBuilder_SetThreshold_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXLUTBuilder_INTERFACE_DEFINED__ */


#ifndef __IDXDLUTBuilder_INTERFACE_DEFINED__
#define __IDXDLUTBuilder_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXDLUTBuilder
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDXDLUTBuilder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("73068231-35EE-11d1-81A1-0000F87557DB")
    IDXDLUTBuilder : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_NumBuildSteps( 
            /* [retval][out] */ long __RPC_FAR *pNumSteps) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_BuildOrder( 
            /* [retval][out] */ VARIANT __RPC_FAR *pOpOrder) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_BuildOrder( 
            /* [in] */ VARIANT __RPC_FAR *pOpOrder) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Gamma( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Gamma( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Opacity( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Opacity( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Brightness( 
            /* [retval][out] */ VARIANT __RPC_FAR *pWeights) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Brightness( 
            /* [in] */ VARIANT __RPC_FAR *pWeights) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Contrast( 
            /* [retval][out] */ VARIANT __RPC_FAR *pWeights) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Contrast( 
            /* [in] */ VARIANT __RPC_FAR *pWeights) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ColorBalance( 
            /* [in] */ DXLUTCOLOR Color,
            /* [retval][out] */ VARIANT __RPC_FAR *pWeights) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ColorBalance( 
            /* [in] */ DXLUTCOLOR Color,
            /* [in] */ VARIANT __RPC_FAR *pWeights) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LevelsPerChannel( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_LevelsPerChannel( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Invert( 
            /* [retval][out] */ float __RPC_FAR *pThreshold) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Invert( 
            /* [in] */ float Threshold) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Threshold( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Threshold( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXDLUTBuilderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXDLUTBuilder __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXDLUTBuilder __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumBuildSteps )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pNumSteps);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BuildOrder )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pOpOrder);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BuildOrder )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *pOpOrder);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Gamma )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Gamma )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Opacity )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Opacity )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Brightness )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pWeights);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Brightness )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *pWeights);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Contrast )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pWeights);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Contrast )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *pWeights);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ColorBalance )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ DXLUTCOLOR Color,
            /* [retval][out] */ VARIANT __RPC_FAR *pWeights);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ColorBalance )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ DXLUTCOLOR Color,
            /* [in] */ VARIANT __RPC_FAR *pWeights);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LevelsPerChannel )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LevelsPerChannel )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Invert )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pThreshold);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Invert )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ float Threshold);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Threshold )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Threshold )( 
            IDXDLUTBuilder __RPC_FAR * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } IDXDLUTBuilderVtbl;

    interface IDXDLUTBuilder
    {
        CONST_VTBL struct IDXDLUTBuilderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXDLUTBuilder_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXDLUTBuilder_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXDLUTBuilder_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXDLUTBuilder_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDXDLUTBuilder_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDXDLUTBuilder_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDXDLUTBuilder_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDXDLUTBuilder_get_NumBuildSteps(This,pNumSteps)	\
    (This)->lpVtbl -> get_NumBuildSteps(This,pNumSteps)

#define IDXDLUTBuilder_get_BuildOrder(This,pOpOrder)	\
    (This)->lpVtbl -> get_BuildOrder(This,pOpOrder)

#define IDXDLUTBuilder_put_BuildOrder(This,pOpOrder)	\
    (This)->lpVtbl -> put_BuildOrder(This,pOpOrder)

#define IDXDLUTBuilder_get_Gamma(This,pVal)	\
    (This)->lpVtbl -> get_Gamma(This,pVal)

#define IDXDLUTBuilder_put_Gamma(This,newVal)	\
    (This)->lpVtbl -> put_Gamma(This,newVal)

#define IDXDLUTBuilder_get_Opacity(This,pVal)	\
    (This)->lpVtbl -> get_Opacity(This,pVal)

#define IDXDLUTBuilder_put_Opacity(This,newVal)	\
    (This)->lpVtbl -> put_Opacity(This,newVal)

#define IDXDLUTBuilder_get_Brightness(This,pWeights)	\
    (This)->lpVtbl -> get_Brightness(This,pWeights)

#define IDXDLUTBuilder_put_Brightness(This,pWeights)	\
    (This)->lpVtbl -> put_Brightness(This,pWeights)

#define IDXDLUTBuilder_get_Contrast(This,pWeights)	\
    (This)->lpVtbl -> get_Contrast(This,pWeights)

#define IDXDLUTBuilder_put_Contrast(This,pWeights)	\
    (This)->lpVtbl -> put_Contrast(This,pWeights)

#define IDXDLUTBuilder_get_ColorBalance(This,Color,pWeights)	\
    (This)->lpVtbl -> get_ColorBalance(This,Color,pWeights)

#define IDXDLUTBuilder_put_ColorBalance(This,Color,pWeights)	\
    (This)->lpVtbl -> put_ColorBalance(This,Color,pWeights)

#define IDXDLUTBuilder_get_LevelsPerChannel(This,pVal)	\
    (This)->lpVtbl -> get_LevelsPerChannel(This,pVal)

#define IDXDLUTBuilder_put_LevelsPerChannel(This,newVal)	\
    (This)->lpVtbl -> put_LevelsPerChannel(This,newVal)

#define IDXDLUTBuilder_get_Invert(This,pThreshold)	\
    (This)->lpVtbl -> get_Invert(This,pThreshold)

#define IDXDLUTBuilder_put_Invert(This,Threshold)	\
    (This)->lpVtbl -> put_Invert(This,Threshold)

#define IDXDLUTBuilder_get_Threshold(This,pVal)	\
    (This)->lpVtbl -> get_Threshold(This,pVal)

#define IDXDLUTBuilder_put_Threshold(This,newVal)	\
    (This)->lpVtbl -> put_Threshold(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_NumBuildSteps_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pNumSteps);


void __RPC_STUB IDXDLUTBuilder_get_NumBuildSteps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_BuildOrder_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pOpOrder);


void __RPC_STUB IDXDLUTBuilder_get_BuildOrder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_put_BuildOrder_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *pOpOrder);


void __RPC_STUB IDXDLUTBuilder_put_BuildOrder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_Gamma_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDXDLUTBuilder_get_Gamma_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_put_Gamma_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDXDLUTBuilder_put_Gamma_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_Opacity_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDXDLUTBuilder_get_Opacity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_put_Opacity_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDXDLUTBuilder_put_Opacity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_Brightness_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pWeights);


void __RPC_STUB IDXDLUTBuilder_get_Brightness_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_put_Brightness_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *pWeights);


void __RPC_STUB IDXDLUTBuilder_put_Brightness_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_Contrast_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pWeights);


void __RPC_STUB IDXDLUTBuilder_get_Contrast_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_put_Contrast_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *pWeights);


void __RPC_STUB IDXDLUTBuilder_put_Contrast_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_ColorBalance_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ DXLUTCOLOR Color,
    /* [retval][out] */ VARIANT __RPC_FAR *pWeights);


void __RPC_STUB IDXDLUTBuilder_get_ColorBalance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_put_ColorBalance_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ DXLUTCOLOR Color,
    /* [in] */ VARIANT __RPC_FAR *pWeights);


void __RPC_STUB IDXDLUTBuilder_put_ColorBalance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_LevelsPerChannel_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IDXDLUTBuilder_get_LevelsPerChannel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_put_LevelsPerChannel_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IDXDLUTBuilder_put_LevelsPerChannel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_Invert_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pThreshold);


void __RPC_STUB IDXDLUTBuilder_get_Invert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_put_Invert_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ float Threshold);


void __RPC_STUB IDXDLUTBuilder_put_Invert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_get_Threshold_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDXDLUTBuilder_get_Threshold_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXDLUTBuilder_put_Threshold_Proxy( 
    IDXDLUTBuilder __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDXDLUTBuilder_put_Threshold_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXDLUTBuilder_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0179
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum DXFILTERTYPE
    {	DXFILTER_NEAREST	= 0,
	DXFILTER_BILINEAR	= DXFILTER_NEAREST + 1,
	DXFILTER_CUBIC	= DXFILTER_BILINEAR + 1,
	DXFILTER_BSPLINE	= DXFILTER_CUBIC + 1,
	DXFILTER_NUM_FILTERS	= DXFILTER_BSPLINE + 1
    }	DXFILTERTYPE;

typedef 
enum DXRESAMPFLAGS
    {	DXRESAMP_X	= 1L << 0,
	DXRESAMP_Y	= 1L << 1,
	DXRESAMP_Z	= 1L << 2,
	DXRESAMP_T	= 1L << 3,
	DXRESAMP_ABSOLUTE	= 1L << 4,
	DXRESAMP_ALL	= DXRESAMP_X | DXRESAMP_Y | DXRESAMP_Z | DXRESAMP_T
    }	DXRESAMPFLAGS;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0179_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0179_v0_0_s_ifspec;

#ifndef __IDXTResampler_INTERFACE_DEFINED__
#define __IDXTResampler_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXTResampler
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [object][unique][helpstring][uuid] */ 



EXTERN_C const IID IID_IDXTResampler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("015E2938-C341-11d1-81D1-0000F87557DB")
    IDXTResampler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetFilterType( 
            /* [in] */ DXFILTERTYPE eType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFilterType( 
            /* [out] */ DXFILTERTYPE __RPC_FAR *peType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetMapping( 
            /* [in] */ const DX2DXFORM __RPC_FAR *pXForm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMapping( 
            /* [out] */ DX2DXFORM __RPC_FAR *pXForm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetMappingObject( 
            /* [in] */ IUnknown __RPC_FAR *pMapper) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMappingObject( 
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppMapper) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Rotate( 
            /* [in] */ float Rotation,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Scale( 
            /* [in] */ float __RPC_FAR Scales[ 2 ],
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ScaleFitToSize( 
            /* [in] */ DXBNDS __RPC_FAR *pClipBounds,
            /* [in] */ SIZE FitToSize,
            /* [in] */ BOOL bMaintainAspect) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXTResamplerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXTResampler __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXTResampler __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXTResampler __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFilterType )( 
            IDXTResampler __RPC_FAR * This,
            /* [in] */ DXFILTERTYPE eType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFilterType )( 
            IDXTResampler __RPC_FAR * This,
            /* [out] */ DXFILTERTYPE __RPC_FAR *peType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMapping )( 
            IDXTResampler __RPC_FAR * This,
            /* [in] */ const DX2DXFORM __RPC_FAR *pXForm);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMapping )( 
            IDXTResampler __RPC_FAR * This,
            /* [out] */ DX2DXFORM __RPC_FAR *pXForm);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMappingObject )( 
            IDXTResampler __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pMapper);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMappingObject )( 
            IDXTResampler __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppMapper);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Rotate )( 
            IDXTResampler __RPC_FAR * This,
            /* [in] */ float Rotation,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Scale )( 
            IDXTResampler __RPC_FAR * This,
            /* [in] */ float __RPC_FAR Scales[ 2 ],
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ScaleFitToSize )( 
            IDXTResampler __RPC_FAR * This,
            /* [in] */ DXBNDS __RPC_FAR *pClipBounds,
            /* [in] */ SIZE FitToSize,
            /* [in] */ BOOL bMaintainAspect);
        
        END_INTERFACE
    } IDXTResamplerVtbl;

    interface IDXTResampler
    {
        CONST_VTBL struct IDXTResamplerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXTResampler_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXTResampler_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXTResampler_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXTResampler_SetFilterType(This,eType)	\
    (This)->lpVtbl -> SetFilterType(This,eType)

#define IDXTResampler_GetFilterType(This,peType)	\
    (This)->lpVtbl -> GetFilterType(This,peType)

#define IDXTResampler_SetMapping(This,pXForm)	\
    (This)->lpVtbl -> SetMapping(This,pXForm)

#define IDXTResampler_GetMapping(This,pXForm)	\
    (This)->lpVtbl -> GetMapping(This,pXForm)

#define IDXTResampler_SetMappingObject(This,pMapper)	\
    (This)->lpVtbl -> SetMappingObject(This,pMapper)

#define IDXTResampler_GetMappingObject(This,ppMapper)	\
    (This)->lpVtbl -> GetMappingObject(This,ppMapper)

#define IDXTResampler_Rotate(This,Rotation,dwFlags)	\
    (This)->lpVtbl -> Rotate(This,Rotation,dwFlags)

#define IDXTResampler_Scale(This,Scales,dwFlags)	\
    (This)->lpVtbl -> Scale(This,Scales,dwFlags)

#define IDXTResampler_ScaleFitToSize(This,pClipBounds,FitToSize,bMaintainAspect)	\
    (This)->lpVtbl -> ScaleFitToSize(This,pClipBounds,FitToSize,bMaintainAspect)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDXTResampler_SetFilterType_Proxy( 
    IDXTResampler __RPC_FAR * This,
    /* [in] */ DXFILTERTYPE eType);


void __RPC_STUB IDXTResampler_SetFilterType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTResampler_GetFilterType_Proxy( 
    IDXTResampler __RPC_FAR * This,
    /* [out] */ DXFILTERTYPE __RPC_FAR *peType);


void __RPC_STUB IDXTResampler_GetFilterType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTResampler_SetMapping_Proxy( 
    IDXTResampler __RPC_FAR * This,
    /* [in] */ const DX2DXFORM __RPC_FAR *pXForm);


void __RPC_STUB IDXTResampler_SetMapping_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTResampler_GetMapping_Proxy( 
    IDXTResampler __RPC_FAR * This,
    /* [out] */ DX2DXFORM __RPC_FAR *pXForm);


void __RPC_STUB IDXTResampler_GetMapping_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTResampler_SetMappingObject_Proxy( 
    IDXTResampler __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pMapper);


void __RPC_STUB IDXTResampler_SetMappingObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTResampler_GetMappingObject_Proxy( 
    IDXTResampler __RPC_FAR * This,
    /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppMapper);


void __RPC_STUB IDXTResampler_GetMappingObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTResampler_Rotate_Proxy( 
    IDXTResampler __RPC_FAR * This,
    /* [in] */ float Rotation,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IDXTResampler_Rotate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTResampler_Scale_Proxy( 
    IDXTResampler __RPC_FAR * This,
    /* [in] */ float __RPC_FAR Scales[ 2 ],
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IDXTResampler_Scale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTResampler_ScaleFitToSize_Proxy( 
    IDXTResampler __RPC_FAR * This,
    /* [in] */ DXBNDS __RPC_FAR *pClipBounds,
    /* [in] */ SIZE FitToSize,
    /* [in] */ BOOL bMaintainAspect);


void __RPC_STUB IDXTResampler_ScaleFitToSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXTResampler_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0180
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum DXRESAMPDISPID
    {	DISPID_MappingMatrix	= 1,
	DISPID_MappingObject	= DISPID_MappingMatrix + 1,
	DISPID_FilterType	= DISPID_MappingObject + 1,
	DISPID_Rotation	= DISPID_FilterType + 1,
	DISPID_ScaleX	= DISPID_Rotation + 1,
	DISPID_ScaleY	= DISPID_ScaleX + 1,
	DISPID_ScaleFitToSize	= DISPID_ScaleY + 1
    }	DXRESAMPDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0180_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0180_v0_0_s_ifspec;

#ifndef __IDXTResamplerD_INTERFACE_DEFINED__
#define __IDXTResamplerD_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXTResamplerD
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDXTResamplerD;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("015E2937-C341-11d1-81D1-0000F87557DB")
    IDXTResamplerD : public IDispatch
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MappingMatrix( 
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pFilter) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MappingMatrix( 
            /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pFilter) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MappingObject( 
            /* [in] */ IUnknown __RPC_FAR *pMapper) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MappingObject( 
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppMapper) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_FilterType( 
            /* [in] */ DXFILTERTYPE eType) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FilterType( 
            /* [retval][out] */ DXFILTERTYPE __RPC_FAR *peType) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Rotation( 
            /* [in] */ float Rotation) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Rotation( 
            /* [retval][out] */ float __RPC_FAR *Rotation) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ScaleX( 
            /* [in] */ float X) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ScaleX( 
            /* [retval][out] */ float __RPC_FAR *pX) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ScaleY( 
            /* [in] */ float Y) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ScaleY( 
            /* [retval][out] */ float __RPC_FAR *pY) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ScaleFitToSize( 
            /* [in] */ long FitWid,
            /* [in] */ long FitHgt,
            /* [in] */ BOOL bMaintainAspect) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXTResamplerDVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXTResamplerD __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXTResamplerD __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MappingMatrix )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pFilter);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MappingMatrix )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pFilter);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MappingObject )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pMapper);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MappingObject )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppMapper);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FilterType )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ DXFILTERTYPE eType);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FilterType )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [retval][out] */ DXFILTERTYPE __RPC_FAR *peType);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Rotation )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ float Rotation);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Rotation )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *Rotation);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScaleX )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ float X);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScaleX )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pX);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScaleY )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ float Y);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScaleY )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pY);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ScaleFitToSize )( 
            IDXTResamplerD __RPC_FAR * This,
            /* [in] */ long FitWid,
            /* [in] */ long FitHgt,
            /* [in] */ BOOL bMaintainAspect);
        
        END_INTERFACE
    } IDXTResamplerDVtbl;

    interface IDXTResamplerD
    {
        CONST_VTBL struct IDXTResamplerDVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXTResamplerD_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXTResamplerD_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXTResamplerD_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXTResamplerD_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDXTResamplerD_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDXTResamplerD_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDXTResamplerD_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDXTResamplerD_put_MappingMatrix(This,pFilter)	\
    (This)->lpVtbl -> put_MappingMatrix(This,pFilter)

#define IDXTResamplerD_get_MappingMatrix(This,pFilter)	\
    (This)->lpVtbl -> get_MappingMatrix(This,pFilter)

#define IDXTResamplerD_put_MappingObject(This,pMapper)	\
    (This)->lpVtbl -> put_MappingObject(This,pMapper)

#define IDXTResamplerD_get_MappingObject(This,ppMapper)	\
    (This)->lpVtbl -> get_MappingObject(This,ppMapper)

#define IDXTResamplerD_put_FilterType(This,eType)	\
    (This)->lpVtbl -> put_FilterType(This,eType)

#define IDXTResamplerD_get_FilterType(This,peType)	\
    (This)->lpVtbl -> get_FilterType(This,peType)

#define IDXTResamplerD_put_Rotation(This,Rotation)	\
    (This)->lpVtbl -> put_Rotation(This,Rotation)

#define IDXTResamplerD_get_Rotation(This,Rotation)	\
    (This)->lpVtbl -> get_Rotation(This,Rotation)

#define IDXTResamplerD_put_ScaleX(This,X)	\
    (This)->lpVtbl -> put_ScaleX(This,X)

#define IDXTResamplerD_get_ScaleX(This,pX)	\
    (This)->lpVtbl -> get_ScaleX(This,pX)

#define IDXTResamplerD_put_ScaleY(This,Y)	\
    (This)->lpVtbl -> put_ScaleY(This,Y)

#define IDXTResamplerD_get_ScaleY(This,pY)	\
    (This)->lpVtbl -> get_ScaleY(This,pY)

#define IDXTResamplerD_ScaleFitToSize(This,FitWid,FitHgt,bMaintainAspect)	\
    (This)->lpVtbl -> ScaleFitToSize(This,FitWid,FitHgt,bMaintainAspect)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_put_MappingMatrix_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pFilter);


void __RPC_STUB IDXTResamplerD_put_MappingMatrix_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_get_MappingMatrix_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pFilter);


void __RPC_STUB IDXTResamplerD_get_MappingMatrix_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_put_MappingObject_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pMapper);


void __RPC_STUB IDXTResamplerD_put_MappingObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_get_MappingObject_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppMapper);


void __RPC_STUB IDXTResamplerD_get_MappingObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_put_FilterType_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [in] */ DXFILTERTYPE eType);


void __RPC_STUB IDXTResamplerD_put_FilterType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_get_FilterType_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [retval][out] */ DXFILTERTYPE __RPC_FAR *peType);


void __RPC_STUB IDXTResamplerD_get_FilterType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_put_Rotation_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [in] */ float Rotation);


void __RPC_STUB IDXTResamplerD_put_Rotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_get_Rotation_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *Rotation);


void __RPC_STUB IDXTResamplerD_get_Rotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_put_ScaleX_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [in] */ float X);


void __RPC_STUB IDXTResamplerD_put_ScaleX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_get_ScaleX_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pX);


void __RPC_STUB IDXTResamplerD_get_ScaleX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_put_ScaleY_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [in] */ float Y);


void __RPC_STUB IDXTResamplerD_put_ScaleY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_get_ScaleY_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pY);


void __RPC_STUB IDXTResamplerD_get_ScaleY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IDXTResamplerD_ScaleFitToSize_Proxy( 
    IDXTResamplerD __RPC_FAR * This,
    /* [in] */ long FitWid,
    /* [in] */ long FitHgt,
    /* [in] */ BOOL bMaintainAspect);


void __RPC_STUB IDXTResamplerD_ScaleFitToSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXTResamplerD_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0181
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum DXGRADIENTTYPE
    {	DXGRADIENT_VERTICAL	= 0,
	DXGRADIENT_HORIZONTAL	= DXGRADIENT_VERTICAL + 1,
	DXGRADIENT_NUM_GRADIENTS	= DXGRADIENT_HORIZONTAL + 1
    }	DXGRADIENTTYPE;

typedef 
enum DXGRADDISPID
    {	DISPID_GradientType	= 1,
	DISPID_StartColor	= DISPID_GradientType + 1,
	DISPID_EndColor	= DISPID_StartColor + 1,
	DISPID_GradientWidth	= DISPID_EndColor + 1,
	DISPID_GradientHeight	= DISPID_GradientWidth + 1,
	DISPID_GradientAspect	= DISPID_GradientHeight + 1,
	DISPID_StartColorStr	= DISPID_GradientAspect + 1,
	DISPID_EndColorStr	= DISPID_StartColorStr + 1
    }	DXGRADDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0181_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0181_v0_0_s_ifspec;

#ifndef __IDXTGradientD_INTERFACE_DEFINED__
#define __IDXTGradientD_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXTGradientD
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDXTGradientD;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("623E2881-FC0E-11d1-9A77-0000F8756A10")
    IDXTGradientD : public IDispatch
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_GradientType( 
            /* [in] */ DXGRADIENTTYPE eType) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GradientType( 
            /* [retval][out] */ DXGRADIENTTYPE __RPC_FAR *peType) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_StartColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_StartColor( 
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_EndColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_EndColor( 
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GradientWidth( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GradientWidth( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GradientHeight( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GradientHeight( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_KeepAspectRatio( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_KeepAspectRatio( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_StartColorStr( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_EndColorStr( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXTGradientDVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXTGradientD __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXTGradientD __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDXTGradientD __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_GradientType )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ DXGRADIENTTYPE eType);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_GradientType )( 
            IDXTGradientD __RPC_FAR * This,
            /* [retval][out] */ DXGRADIENTTYPE __RPC_FAR *peType);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StartColor )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StartColor )( 
            IDXTGradientD __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EndColor )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EndColor )( 
            IDXTGradientD __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_GradientWidth )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_GradientWidth )( 
            IDXTGradientD __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_GradientHeight )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_GradientHeight )( 
            IDXTGradientD __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_KeepAspectRatio )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_KeepAspectRatio )( 
            IDXTGradientD __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StartColorStr )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EndColorStr )( 
            IDXTGradientD __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IDXTGradientDVtbl;

    interface IDXTGradientD
    {
        CONST_VTBL struct IDXTGradientDVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXTGradientD_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXTGradientD_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXTGradientD_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXTGradientD_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDXTGradientD_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDXTGradientD_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDXTGradientD_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDXTGradientD_put_GradientType(This,eType)	\
    (This)->lpVtbl -> put_GradientType(This,eType)

#define IDXTGradientD_get_GradientType(This,peType)	\
    (This)->lpVtbl -> get_GradientType(This,peType)

#define IDXTGradientD_put_StartColor(This,newVal)	\
    (This)->lpVtbl -> put_StartColor(This,newVal)

#define IDXTGradientD_get_StartColor(This,pVal)	\
    (This)->lpVtbl -> get_StartColor(This,pVal)

#define IDXTGradientD_put_EndColor(This,newVal)	\
    (This)->lpVtbl -> put_EndColor(This,newVal)

#define IDXTGradientD_get_EndColor(This,pVal)	\
    (This)->lpVtbl -> get_EndColor(This,pVal)

#define IDXTGradientD_put_GradientWidth(This,newVal)	\
    (This)->lpVtbl -> put_GradientWidth(This,newVal)

#define IDXTGradientD_get_GradientWidth(This,pVal)	\
    (This)->lpVtbl -> get_GradientWidth(This,pVal)

#define IDXTGradientD_put_GradientHeight(This,newVal)	\
    (This)->lpVtbl -> put_GradientHeight(This,newVal)

#define IDXTGradientD_get_GradientHeight(This,pVal)	\
    (This)->lpVtbl -> get_GradientHeight(This,pVal)

#define IDXTGradientD_put_KeepAspectRatio(This,newVal)	\
    (This)->lpVtbl -> put_KeepAspectRatio(This,newVal)

#define IDXTGradientD_get_KeepAspectRatio(This,pVal)	\
    (This)->lpVtbl -> get_KeepAspectRatio(This,pVal)

#define IDXTGradientD_put_StartColorStr(This,newVal)	\
    (This)->lpVtbl -> put_StartColorStr(This,newVal)

#define IDXTGradientD_put_EndColorStr(This,newVal)	\
    (This)->lpVtbl -> put_EndColorStr(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_put_GradientType_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [in] */ DXGRADIENTTYPE eType);


void __RPC_STUB IDXTGradientD_put_GradientType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_get_GradientType_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [retval][out] */ DXGRADIENTTYPE __RPC_FAR *peType);


void __RPC_STUB IDXTGradientD_get_GradientType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_put_StartColor_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [in] */ OLE_COLOR newVal);


void __RPC_STUB IDXTGradientD_put_StartColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_get_StartColor_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pVal);


void __RPC_STUB IDXTGradientD_get_StartColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_put_EndColor_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [in] */ OLE_COLOR newVal);


void __RPC_STUB IDXTGradientD_put_EndColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_get_EndColor_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pVal);


void __RPC_STUB IDXTGradientD_get_EndColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_put_GradientWidth_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IDXTGradientD_put_GradientWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_get_GradientWidth_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IDXTGradientD_get_GradientWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_put_GradientHeight_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IDXTGradientD_put_GradientHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_get_GradientHeight_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IDXTGradientD_get_GradientHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_put_KeepAspectRatio_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IDXTGradientD_put_KeepAspectRatio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_get_KeepAspectRatio_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IDXTGradientD_get_KeepAspectRatio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_put_StartColorStr_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDXTGradientD_put_StartColorStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXTGradientD_put_EndColorStr_Proxy( 
    IDXTGradientD __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDXTGradientD_put_EndColorStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXTGradientD_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0182
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum DXCONVFILTERTYPE
    {	DXCFILTER_SRCCOPY	= 0,
	DXCFILTER_BOX7X7	= DXCFILTER_SRCCOPY + 1,
	DXCFILTER_BLUR3X3	= DXCFILTER_BOX7X7 + 1,
	DXCFILTER_SHARPEN	= DXCFILTER_BLUR3X3 + 1,
	DXCFILTER_EMBOSS	= DXCFILTER_SHARPEN + 1,
	DXCFILTER_ENGRAVE	= DXCFILTER_EMBOSS + 1,
	DXCFILTER_NUM_FILTERS	= DXCFILTER_ENGRAVE + 1,
	DXCFILTER_CUSTOM	= DXCFILTER_NUM_FILTERS + 1
    }	DXCONVFILTERTYPE;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0182_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0182_v0_0_s_ifspec;

#ifndef __IDXTConvolution_INTERFACE_DEFINED__
#define __IDXTConvolution_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXTConvolution
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [object][unique][helpstring][uuid] */ 



EXTERN_C const IID IID_IDXTConvolution;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("7BA7F8AF-E5EA-11d1-81DD-0000F87557DB")
    IDXTConvolution : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetFilterType( 
            /* [in] */ DXCONVFILTERTYPE eType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFilterType( 
            /* [out] */ DXCONVFILTERTYPE __RPC_FAR *peType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCustomFilter( 
            /* [in] */ float __RPC_FAR *pFilter,
            /* [in] */ SIZE Size) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetConvertToGray( 
            /* [in] */ BOOL bConvertToGray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetConvertToGray( 
            /* [out] */ BOOL __RPC_FAR *pbConvertToGray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetBias( 
            /* [in] */ float Bias) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBias( 
            /* [out] */ float __RPC_FAR *pBias) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetExcludeAlpha( 
            /* [in] */ BOOL bExcludeAlpha) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetExcludeAlpha( 
            /* [out] */ BOOL __RPC_FAR *pbExcludeAlpha) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXTConvolutionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXTConvolution __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXTConvolution __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXTConvolution __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFilterType )( 
            IDXTConvolution __RPC_FAR * This,
            /* [in] */ DXCONVFILTERTYPE eType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFilterType )( 
            IDXTConvolution __RPC_FAR * This,
            /* [out] */ DXCONVFILTERTYPE __RPC_FAR *peType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCustomFilter )( 
            IDXTConvolution __RPC_FAR * This,
            /* [in] */ float __RPC_FAR *pFilter,
            /* [in] */ SIZE Size);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetConvertToGray )( 
            IDXTConvolution __RPC_FAR * This,
            /* [in] */ BOOL bConvertToGray);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetConvertToGray )( 
            IDXTConvolution __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pbConvertToGray);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBias )( 
            IDXTConvolution __RPC_FAR * This,
            /* [in] */ float Bias);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBias )( 
            IDXTConvolution __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *pBias);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetExcludeAlpha )( 
            IDXTConvolution __RPC_FAR * This,
            /* [in] */ BOOL bExcludeAlpha);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetExcludeAlpha )( 
            IDXTConvolution __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pbExcludeAlpha);
        
        END_INTERFACE
    } IDXTConvolutionVtbl;

    interface IDXTConvolution
    {
        CONST_VTBL struct IDXTConvolutionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXTConvolution_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXTConvolution_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXTConvolution_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXTConvolution_SetFilterType(This,eType)	\
    (This)->lpVtbl -> SetFilterType(This,eType)

#define IDXTConvolution_GetFilterType(This,peType)	\
    (This)->lpVtbl -> GetFilterType(This,peType)

#define IDXTConvolution_SetCustomFilter(This,pFilter,Size)	\
    (This)->lpVtbl -> SetCustomFilter(This,pFilter,Size)

#define IDXTConvolution_SetConvertToGray(This,bConvertToGray)	\
    (This)->lpVtbl -> SetConvertToGray(This,bConvertToGray)

#define IDXTConvolution_GetConvertToGray(This,pbConvertToGray)	\
    (This)->lpVtbl -> GetConvertToGray(This,pbConvertToGray)

#define IDXTConvolution_SetBias(This,Bias)	\
    (This)->lpVtbl -> SetBias(This,Bias)

#define IDXTConvolution_GetBias(This,pBias)	\
    (This)->lpVtbl -> GetBias(This,pBias)

#define IDXTConvolution_SetExcludeAlpha(This,bExcludeAlpha)	\
    (This)->lpVtbl -> SetExcludeAlpha(This,bExcludeAlpha)

#define IDXTConvolution_GetExcludeAlpha(This,pbExcludeAlpha)	\
    (This)->lpVtbl -> GetExcludeAlpha(This,pbExcludeAlpha)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDXTConvolution_SetFilterType_Proxy( 
    IDXTConvolution __RPC_FAR * This,
    /* [in] */ DXCONVFILTERTYPE eType);


void __RPC_STUB IDXTConvolution_SetFilterType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTConvolution_GetFilterType_Proxy( 
    IDXTConvolution __RPC_FAR * This,
    /* [out] */ DXCONVFILTERTYPE __RPC_FAR *peType);


void __RPC_STUB IDXTConvolution_GetFilterType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTConvolution_SetCustomFilter_Proxy( 
    IDXTConvolution __RPC_FAR * This,
    /* [in] */ float __RPC_FAR *pFilter,
    /* [in] */ SIZE Size);


void __RPC_STUB IDXTConvolution_SetCustomFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTConvolution_SetConvertToGray_Proxy( 
    IDXTConvolution __RPC_FAR * This,
    /* [in] */ BOOL bConvertToGray);


void __RPC_STUB IDXTConvolution_SetConvertToGray_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTConvolution_GetConvertToGray_Proxy( 
    IDXTConvolution __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pbConvertToGray);


void __RPC_STUB IDXTConvolution_GetConvertToGray_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTConvolution_SetBias_Proxy( 
    IDXTConvolution __RPC_FAR * This,
    /* [in] */ float Bias);


void __RPC_STUB IDXTConvolution_SetBias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTConvolution_GetBias_Proxy( 
    IDXTConvolution __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *pBias);


void __RPC_STUB IDXTConvolution_GetBias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTConvolution_SetExcludeAlpha_Proxy( 
    IDXTConvolution __RPC_FAR * This,
    /* [in] */ BOOL bExcludeAlpha);


void __RPC_STUB IDXTConvolution_SetExcludeAlpha_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXTConvolution_GetExcludeAlpha_Proxy( 
    IDXTConvolution __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pbExcludeAlpha);


void __RPC_STUB IDXTConvolution_GetExcludeAlpha_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXTConvolution_INTERFACE_DEFINED__ */


#ifndef __IDXMapper_INTERFACE_DEFINED__
#define __IDXMapper_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXMapper
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [object][unique][helpstring][uuid] */ 



EXTERN_C const IID IID_IDXMapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("555278E5-05DB-11D1-883A-3C8B00C10000")
    IDXMapper : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE MapIn2Out( 
            /* [in] */ DXVEC __RPC_FAR *pInPt,
            /* [out] */ DXVEC __RPC_FAR *pOutPt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapOut2In( 
            /* [in] */ DXVEC __RPC_FAR *pOutPt,
            /* [out] */ DXVEC __RPC_FAR *pInPt) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXMapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXMapper __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXMapper __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXMapper __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MapIn2Out )( 
            IDXMapper __RPC_FAR * This,
            /* [in] */ DXVEC __RPC_FAR *pInPt,
            /* [out] */ DXVEC __RPC_FAR *pOutPt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MapOut2In )( 
            IDXMapper __RPC_FAR * This,
            /* [in] */ DXVEC __RPC_FAR *pOutPt,
            /* [out] */ DXVEC __RPC_FAR *pInPt);
        
        END_INTERFACE
    } IDXMapperVtbl;

    interface IDXMapper
    {
        CONST_VTBL struct IDXMapperVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXMapper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXMapper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXMapper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXMapper_MapIn2Out(This,pInPt,pOutPt)	\
    (This)->lpVtbl -> MapIn2Out(This,pInPt,pOutPt)

#define IDXMapper_MapOut2In(This,pOutPt,pInPt)	\
    (This)->lpVtbl -> MapOut2In(This,pOutPt,pInPt)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDXMapper_MapIn2Out_Proxy( 
    IDXMapper __RPC_FAR * This,
    /* [in] */ DXVEC __RPC_FAR *pInPt,
    /* [out] */ DXVEC __RPC_FAR *pOutPt);


void __RPC_STUB IDXMapper_MapIn2Out_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXMapper_MapOut2In_Proxy( 
    IDXMapper __RPC_FAR * This,
    /* [in] */ DXVEC __RPC_FAR *pOutPt,
    /* [out] */ DXVEC __RPC_FAR *pInPt);


void __RPC_STUB IDXMapper_MapOut2In_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXMapper_INTERFACE_DEFINED__ */


#ifndef __IDXDMapper_INTERFACE_DEFINED__
#define __IDXDMapper_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXDMapper
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDXDMapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("7FD9088B-35ED-11d1-81A1-0000F87557DB")
    IDXDMapper : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE MapIn2Out( 
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pInPt,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pOutPt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapOut2In( 
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pOutPt,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pInPt) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXDMapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXDMapper __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXDMapper __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXDMapper __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDXDMapper __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDXDMapper __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDXDMapper __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDXDMapper __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MapIn2Out )( 
            IDXDMapper __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pInPt,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pOutPt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MapOut2In )( 
            IDXDMapper __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pOutPt,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pInPt);
        
        END_INTERFACE
    } IDXDMapperVtbl;

    interface IDXDMapper
    {
        CONST_VTBL struct IDXDMapperVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXDMapper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXDMapper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXDMapper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXDMapper_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDXDMapper_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDXDMapper_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDXDMapper_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDXDMapper_MapIn2Out(This,pInPt,pOutPt)	\
    (This)->lpVtbl -> MapIn2Out(This,pInPt,pOutPt)

#define IDXDMapper_MapOut2In(This,pOutPt,pInPt)	\
    (This)->lpVtbl -> MapOut2In(This,pOutPt,pInPt)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDXDMapper_MapIn2Out_Proxy( 
    IDXDMapper __RPC_FAR * This,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pInPt,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pOutPt);


void __RPC_STUB IDXDMapper_MapIn2Out_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDXDMapper_MapOut2In_Proxy( 
    IDXDMapper __RPC_FAR * This,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pOutPt,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pInPt);


void __RPC_STUB IDXDMapper_MapOut2In_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXDMapper_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0185
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum DXCOMPFUNC
    {	DXCOMPFUNC_SWAP_AB	= 0x10,
	DXCOMPFUNC_FUNCMASK	= 0xf,
	DXCOMPFUNC_CLEAR	= 0,
	DXCOMPFUNC_MIN	= DXCOMPFUNC_CLEAR + 1,
	DXCOMPFUNC_MAX	= DXCOMPFUNC_MIN + 1,
	DXCOMPFUNC_A	= DXCOMPFUNC_MAX + 1,
	DXCOMPFUNC_A_OVER_B	= DXCOMPFUNC_A + 1,
	DXCOMPFUNC_A_IN_B	= DXCOMPFUNC_A_OVER_B + 1,
	DXCOMPFUNC_A_OUT_B	= DXCOMPFUNC_A_IN_B + 1,
	DXCOMPFUNC_A_ATOP_B	= DXCOMPFUNC_A_OUT_B + 1,
	DXCOMPFUNC_A_SUBTRACT_B	= DXCOMPFUNC_A_ATOP_B + 1,
	DXCOMPFUNC_A_ADD_B	= DXCOMPFUNC_A_SUBTRACT_B + 1,
	DXCOMPFUNC_A_XOR_B	= DXCOMPFUNC_A_ADD_B + 1,
	DXCOMPFUNC_B	= DXCOMPFUNC_A | DXCOMPFUNC_SWAP_AB,
	DXCOMPFUNC_B_OVER_A	= DXCOMPFUNC_A_OVER_B | DXCOMPFUNC_SWAP_AB,
	DXCOMPFUNC_B_IN_A	= DXCOMPFUNC_A_IN_B | DXCOMPFUNC_SWAP_AB,
	DXCOMPFUNC_B_OUT_A	= DXCOMPFUNC_A_OUT_B | DXCOMPFUNC_SWAP_AB,
	DXCOMPFUNC_B_ATOP_A	= DXCOMPFUNC_A_ATOP_B | DXCOMPFUNC_SWAP_AB,
	DXCOMPFUNC_B_SUBTRACT_A	= DXCOMPFUNC_A_SUBTRACT_B | DXCOMPFUNC_SWAP_AB,
	DXCOMPFUNC_B_ADD_A	= DXCOMPFUNC_A_ADD_B | DXCOMPFUNC_SWAP_AB,
	DXCOMPFUNC_NUMFUNCS	= DXCOMPFUNC_B_ADD_A + 1
    }	DXCOMPFUNC;

typedef 
enum DXCOMPOSITEDISPID
    {	DISPID_DXCOMPOSITE_Function	= 1
    }	DXCOMPOSITEDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0185_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0185_v0_0_s_ifspec;

#ifndef __IDXTComposite_INTERFACE_DEFINED__
#define __IDXTComposite_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXTComposite
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDXTComposite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("9A43A843-0831-11D1-817F-0000F87557DB")
    IDXTComposite : public IDispatch
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Function( 
            /* [in] */ DXCOMPFUNC eFunc) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Function( 
            /* [retval][out] */ DXCOMPFUNC __RPC_FAR *peFunc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXTCompositeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXTComposite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXTComposite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXTComposite __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDXTComposite __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDXTComposite __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDXTComposite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDXTComposite __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Function )( 
            IDXTComposite __RPC_FAR * This,
            /* [in] */ DXCOMPFUNC eFunc);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Function )( 
            IDXTComposite __RPC_FAR * This,
            /* [retval][out] */ DXCOMPFUNC __RPC_FAR *peFunc);
        
        END_INTERFACE
    } IDXTCompositeVtbl;

    interface IDXTComposite
    {
        CONST_VTBL struct IDXTCompositeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXTComposite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXTComposite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXTComposite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXTComposite_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDXTComposite_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDXTComposite_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDXTComposite_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDXTComposite_put_Function(This,eFunc)	\
    (This)->lpVtbl -> put_Function(This,eFunc)

#define IDXTComposite_get_Function(This,peFunc)	\
    (This)->lpVtbl -> get_Function(This,peFunc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTComposite_put_Function_Proxy( 
    IDXTComposite __RPC_FAR * This,
    /* [in] */ DXCOMPFUNC eFunc);


void __RPC_STUB IDXTComposite_put_Function_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTComposite_get_Function_Proxy( 
    IDXTComposite __RPC_FAR * This,
    /* [retval][out] */ DXCOMPFUNC __RPC_FAR *peFunc);


void __RPC_STUB IDXTComposite_get_Function_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXTComposite_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0186
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum EXPLODEDISPID
    {	DISPID_Explode_Tumble	= DISPID_DXE_NEXT_ID,
	DISPID_Explode_MaxRotations	= DISPID_Explode_Tumble + 1,
	DISPID_Explode_FinalVelocity	= DISPID_Explode_MaxRotations + 1,
	DISPID_Explode_PositionJump	= DISPID_Explode_FinalVelocity + 1,
	DISPID_Explode_DecayTime	= DISPID_Explode_PositionJump + 1
    }	EXPLODEDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0186_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0186_v0_0_s_ifspec;

#ifndef __IExplode_INTERFACE_DEFINED__
#define __IExplode_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IExplode
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IExplode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("141DBAF0-55FB-11D1-B83E-00A0C933BE86")
    IExplode : public IDXEffect
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Tumble( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Tumble( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxRotations( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxRotations( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_FinalVelocity( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FinalVelocity( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PositionJump( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PositionJump( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DecayTime( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DecayTime( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IExplodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IExplode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IExplode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IExplode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IExplode __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IExplode __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IExplode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IExplode __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Capabilities )( 
            IExplode __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Progress )( 
            IExplode __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Progress )( 
            IExplode __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StepResolution )( 
            IExplode __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Duration )( 
            IExplode __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Duration )( 
            IExplode __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Tumble )( 
            IExplode __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Tumble )( 
            IExplode __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaxRotations )( 
            IExplode __RPC_FAR * This,
            /* [in] */ LONG newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaxRotations )( 
            IExplode __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FinalVelocity )( 
            IExplode __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FinalVelocity )( 
            IExplode __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PositionJump )( 
            IExplode __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PositionJump )( 
            IExplode __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DecayTime )( 
            IExplode __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DecayTime )( 
            IExplode __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        END_INTERFACE
    } IExplodeVtbl;

    interface IExplode
    {
        CONST_VTBL struct IExplodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IExplode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IExplode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IExplode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IExplode_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IExplode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IExplode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IExplode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IExplode_get_Capabilities(This,pVal)	\
    (This)->lpVtbl -> get_Capabilities(This,pVal)

#define IExplode_get_Progress(This,pVal)	\
    (This)->lpVtbl -> get_Progress(This,pVal)

#define IExplode_put_Progress(This,newVal)	\
    (This)->lpVtbl -> put_Progress(This,newVal)

#define IExplode_get_StepResolution(This,pVal)	\
    (This)->lpVtbl -> get_StepResolution(This,pVal)

#define IExplode_get_Duration(This,pVal)	\
    (This)->lpVtbl -> get_Duration(This,pVal)

#define IExplode_put_Duration(This,newVal)	\
    (This)->lpVtbl -> put_Duration(This,newVal)


#define IExplode_put_Tumble(This,newVal)	\
    (This)->lpVtbl -> put_Tumble(This,newVal)

#define IExplode_get_Tumble(This,pVal)	\
    (This)->lpVtbl -> get_Tumble(This,pVal)

#define IExplode_put_MaxRotations(This,newVal)	\
    (This)->lpVtbl -> put_MaxRotations(This,newVal)

#define IExplode_get_MaxRotations(This,pVal)	\
    (This)->lpVtbl -> get_MaxRotations(This,pVal)

#define IExplode_put_FinalVelocity(This,newVal)	\
    (This)->lpVtbl -> put_FinalVelocity(This,newVal)

#define IExplode_get_FinalVelocity(This,pVal)	\
    (This)->lpVtbl -> get_FinalVelocity(This,pVal)

#define IExplode_put_PositionJump(This,newVal)	\
    (This)->lpVtbl -> put_PositionJump(This,newVal)

#define IExplode_get_PositionJump(This,pVal)	\
    (This)->lpVtbl -> get_PositionJump(This,pVal)

#define IExplode_put_DecayTime(This,newVal)	\
    (This)->lpVtbl -> put_DecayTime(This,newVal)

#define IExplode_get_DecayTime(This,pVal)	\
    (This)->lpVtbl -> get_DecayTime(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE IExplode_put_Tumble_Proxy( 
    IExplode __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IExplode_put_Tumble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IExplode_get_Tumble_Proxy( 
    IExplode __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IExplode_get_Tumble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IExplode_put_MaxRotations_Proxy( 
    IExplode __RPC_FAR * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IExplode_put_MaxRotations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IExplode_get_MaxRotations_Proxy( 
    IExplode __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB IExplode_get_MaxRotations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IExplode_put_FinalVelocity_Proxy( 
    IExplode __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IExplode_put_FinalVelocity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IExplode_get_FinalVelocity_Proxy( 
    IExplode __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IExplode_get_FinalVelocity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IExplode_put_PositionJump_Proxy( 
    IExplode __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IExplode_put_PositionJump_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IExplode_get_PositionJump_Proxy( 
    IExplode __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IExplode_get_PositionJump_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IExplode_put_DecayTime_Proxy( 
    IExplode __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IExplode_put_DecayTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IExplode_get_DecayTime_Proxy( 
    IExplode __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IExplode_get_DecayTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IExplode_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0187
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum RUFFLEDISPID
    {	DISPID_Ruffle_MaxRotations	= DISPID_DXE_NEXT_ID
    }	RUFFLEDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0187_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0187_v0_0_s_ifspec;

#ifndef __IRuffle_INTERFACE_DEFINED__
#define __IRuffle_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IRuffle
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IRuffle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("69EE49C4-6268-11D1-B83E-00A0C933BE86")
    IRuffle : public IDXEffect
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxRotations( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxRotations( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRuffleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRuffle __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRuffle __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRuffle __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRuffle __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRuffle __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRuffle __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRuffle __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Capabilities )( 
            IRuffle __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Progress )( 
            IRuffle __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Progress )( 
            IRuffle __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StepResolution )( 
            IRuffle __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Duration )( 
            IRuffle __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Duration )( 
            IRuffle __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaxRotations )( 
            IRuffle __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaxRotations )( 
            IRuffle __RPC_FAR * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } IRuffleVtbl;

    interface IRuffle
    {
        CONST_VTBL struct IRuffleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRuffle_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRuffle_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRuffle_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRuffle_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRuffle_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRuffle_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRuffle_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRuffle_get_Capabilities(This,pVal)	\
    (This)->lpVtbl -> get_Capabilities(This,pVal)

#define IRuffle_get_Progress(This,pVal)	\
    (This)->lpVtbl -> get_Progress(This,pVal)

#define IRuffle_put_Progress(This,newVal)	\
    (This)->lpVtbl -> put_Progress(This,newVal)

#define IRuffle_get_StepResolution(This,pVal)	\
    (This)->lpVtbl -> get_StepResolution(This,pVal)

#define IRuffle_get_Duration(This,pVal)	\
    (This)->lpVtbl -> get_Duration(This,pVal)

#define IRuffle_put_Duration(This,newVal)	\
    (This)->lpVtbl -> put_Duration(This,newVal)


#define IRuffle_get_MaxRotations(This,pVal)	\
    (This)->lpVtbl -> get_MaxRotations(This,pVal)

#define IRuffle_put_MaxRotations(This,newVal)	\
    (This)->lpVtbl -> put_MaxRotations(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IRuffle_get_MaxRotations_Proxy( 
    IRuffle __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IRuffle_get_MaxRotations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IRuffle_put_MaxRotations_Proxy( 
    IRuffle __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IRuffle_put_MaxRotations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRuffle_INTERFACE_DEFINED__ */


#ifndef __IRipple_INTERFACE_DEFINED__
#define __IRipple_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IRipple
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IRipple;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("6DA4A05E-8E9E-11D1-904E-00C04FD9189D")
    IRipple : public IDXEffect
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XOrigin( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_XOrigin( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_YOrigin( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_YOrigin( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Wavelength( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Wavelength( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Amplitude( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Amplitude( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumberOfWaves( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NumberOfWaves( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MinSteps( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MinSteps( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxSteps( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxSteps( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRippleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRipple __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRipple __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRipple __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRipple __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRipple __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRipple __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRipple __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Capabilities )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Progress )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Progress )( 
            IRipple __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StepResolution )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Duration )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Duration )( 
            IRipple __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_XOrigin )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_XOrigin )( 
            IRipple __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_YOrigin )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_YOrigin )( 
            IRipple __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Wavelength )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Wavelength )( 
            IRipple __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Amplitude )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Amplitude )( 
            IRipple __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumberOfWaves )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_NumberOfWaves )( 
            IRipple __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MinSteps )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MinSteps )( 
            IRipple __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaxSteps )( 
            IRipple __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaxSteps )( 
            IRipple __RPC_FAR * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } IRippleVtbl;

    interface IRipple
    {
        CONST_VTBL struct IRippleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRipple_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRipple_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRipple_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRipple_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRipple_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRipple_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRipple_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRipple_get_Capabilities(This,pVal)	\
    (This)->lpVtbl -> get_Capabilities(This,pVal)

#define IRipple_get_Progress(This,pVal)	\
    (This)->lpVtbl -> get_Progress(This,pVal)

#define IRipple_put_Progress(This,newVal)	\
    (This)->lpVtbl -> put_Progress(This,newVal)

#define IRipple_get_StepResolution(This,pVal)	\
    (This)->lpVtbl -> get_StepResolution(This,pVal)

#define IRipple_get_Duration(This,pVal)	\
    (This)->lpVtbl -> get_Duration(This,pVal)

#define IRipple_put_Duration(This,newVal)	\
    (This)->lpVtbl -> put_Duration(This,newVal)


#define IRipple_get_XOrigin(This,pVal)	\
    (This)->lpVtbl -> get_XOrigin(This,pVal)

#define IRipple_put_XOrigin(This,newVal)	\
    (This)->lpVtbl -> put_XOrigin(This,newVal)

#define IRipple_get_YOrigin(This,pVal)	\
    (This)->lpVtbl -> get_YOrigin(This,pVal)

#define IRipple_put_YOrigin(This,newVal)	\
    (This)->lpVtbl -> put_YOrigin(This,newVal)

#define IRipple_get_Wavelength(This,pVal)	\
    (This)->lpVtbl -> get_Wavelength(This,pVal)

#define IRipple_put_Wavelength(This,newVal)	\
    (This)->lpVtbl -> put_Wavelength(This,newVal)

#define IRipple_get_Amplitude(This,pVal)	\
    (This)->lpVtbl -> get_Amplitude(This,pVal)

#define IRipple_put_Amplitude(This,newVal)	\
    (This)->lpVtbl -> put_Amplitude(This,newVal)

#define IRipple_get_NumberOfWaves(This,pVal)	\
    (This)->lpVtbl -> get_NumberOfWaves(This,pVal)

#define IRipple_put_NumberOfWaves(This,newVal)	\
    (This)->lpVtbl -> put_NumberOfWaves(This,newVal)

#define IRipple_get_MinSteps(This,pVal)	\
    (This)->lpVtbl -> get_MinSteps(This,pVal)

#define IRipple_put_MinSteps(This,newVal)	\
    (This)->lpVtbl -> put_MinSteps(This,newVal)

#define IRipple_get_MaxSteps(This,pVal)	\
    (This)->lpVtbl -> get_MaxSteps(This,pVal)

#define IRipple_put_MaxSteps(This,newVal)	\
    (This)->lpVtbl -> put_MaxSteps(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRipple_get_XOrigin_Proxy( 
    IRipple __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IRipple_get_XOrigin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRipple_put_XOrigin_Proxy( 
    IRipple __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IRipple_put_XOrigin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRipple_get_YOrigin_Proxy( 
    IRipple __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IRipple_get_YOrigin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRipple_put_YOrigin_Proxy( 
    IRipple __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IRipple_put_YOrigin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRipple_get_Wavelength_Proxy( 
    IRipple __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IRipple_get_Wavelength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRipple_put_Wavelength_Proxy( 
    IRipple __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IRipple_put_Wavelength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRipple_get_Amplitude_Proxy( 
    IRipple __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IRipple_get_Amplitude_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRipple_put_Amplitude_Proxy( 
    IRipple __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IRipple_put_Amplitude_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRipple_get_NumberOfWaves_Proxy( 
    IRipple __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IRipple_get_NumberOfWaves_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRipple_put_NumberOfWaves_Proxy( 
    IRipple __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IRipple_put_NumberOfWaves_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRipple_get_MinSteps_Proxy( 
    IRipple __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IRipple_get_MinSteps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRipple_put_MinSteps_Proxy( 
    IRipple __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IRipple_put_MinSteps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRipple_get_MaxSteps_Proxy( 
    IRipple __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IRipple_get_MaxSteps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRipple_put_MaxSteps_Proxy( 
    IRipple __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IRipple_put_MaxSteps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRipple_INTERFACE_DEFINED__ */


#ifndef __IDissolve_INTERFACE_DEFINED__
#define __IDissolve_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDissolve
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDissolve;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("90b5e2e0-5050-11d1-b83e-00a0c933be86")
    IDissolve : public IDXEffect
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt0( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt0( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt1( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt1( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt2( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt2( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt3( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt3( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt4( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt4( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt5( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt5( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt6( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt6( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt7( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt7( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt8( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt8( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedPt9( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedPt9( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDissolveVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDissolve __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDissolve __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDissolve __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Capabilities )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Progress )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Progress )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StepResolution )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Duration )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Duration )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt0 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt0 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt1 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt1 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt2 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt2 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt3 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt3 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt4 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt4 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt5 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt5 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt6 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt6 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt7 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt7 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt8 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt8 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpeedPt9 )( 
            IDissolve __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SpeedPt9 )( 
            IDissolve __RPC_FAR * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } IDissolveVtbl;

    interface IDissolve
    {
        CONST_VTBL struct IDissolveVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDissolve_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDissolve_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDissolve_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDissolve_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDissolve_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDissolve_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDissolve_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDissolve_get_Capabilities(This,pVal)	\
    (This)->lpVtbl -> get_Capabilities(This,pVal)

#define IDissolve_get_Progress(This,pVal)	\
    (This)->lpVtbl -> get_Progress(This,pVal)

#define IDissolve_put_Progress(This,newVal)	\
    (This)->lpVtbl -> put_Progress(This,newVal)

#define IDissolve_get_StepResolution(This,pVal)	\
    (This)->lpVtbl -> get_StepResolution(This,pVal)

#define IDissolve_get_Duration(This,pVal)	\
    (This)->lpVtbl -> get_Duration(This,pVal)

#define IDissolve_put_Duration(This,newVal)	\
    (This)->lpVtbl -> put_Duration(This,newVal)


#define IDissolve_get_SpeedPt0(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt0(This,pVal)

#define IDissolve_put_SpeedPt0(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt0(This,newVal)

#define IDissolve_get_SpeedPt1(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt1(This,pVal)

#define IDissolve_put_SpeedPt1(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt1(This,newVal)

#define IDissolve_get_SpeedPt2(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt2(This,pVal)

#define IDissolve_put_SpeedPt2(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt2(This,newVal)

#define IDissolve_get_SpeedPt3(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt3(This,pVal)

#define IDissolve_put_SpeedPt3(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt3(This,newVal)

#define IDissolve_get_SpeedPt4(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt4(This,pVal)

#define IDissolve_put_SpeedPt4(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt4(This,newVal)

#define IDissolve_get_SpeedPt5(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt5(This,pVal)

#define IDissolve_put_SpeedPt5(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt5(This,newVal)

#define IDissolve_get_SpeedPt6(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt6(This,pVal)

#define IDissolve_put_SpeedPt6(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt6(This,newVal)

#define IDissolve_get_SpeedPt7(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt7(This,pVal)

#define IDissolve_put_SpeedPt7(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt7(This,newVal)

#define IDissolve_get_SpeedPt8(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt8(This,pVal)

#define IDissolve_put_SpeedPt8(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt8(This,newVal)

#define IDissolve_get_SpeedPt9(This,pVal)	\
    (This)->lpVtbl -> get_SpeedPt9(This,pVal)

#define IDissolve_put_SpeedPt9(This,newVal)	\
    (This)->lpVtbl -> put_SpeedPt9(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt0_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt0_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt0_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt0_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt1_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt1_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt2_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt2_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt3_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt3_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt4_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt4_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt4_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt4_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt5_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt5_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt5_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt5_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt6_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt6_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt6_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt6_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt7_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt7_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt7_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt7_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt8_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt8_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt8_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt8_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDissolve_get_SpeedPt9_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDissolve_get_SpeedPt9_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDissolve_put_SpeedPt9_Proxy( 
    IDissolve __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDissolve_put_SpeedPt9_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDissolve_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0190
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum FLIPDISPID
    {	DISPID_Flip_Width	= DISPID_DXE_NEXT_ID,
	DISPID_Flip_Height	= DISPID_Flip_Width + 1,
	DISPID_Flip_Depth	= DISPID_Flip_Height + 1
    }	FLIPDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0190_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0190_v0_0_s_ifspec;

#ifndef __IFlip_INTERFACE_DEFINED__
#define __IFlip_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IFlip
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IFlip;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("945F5841-3A8D-11D1-9037-00C04FD9189D")
    IFlip : public IDXEffect
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Depth( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Depth( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFlipVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFlip __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFlip __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFlip __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFlip __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFlip __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFlip __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFlip __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Capabilities )( 
            IFlip __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Progress )( 
            IFlip __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Progress )( 
            IFlip __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StepResolution )( 
            IFlip __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Duration )( 
            IFlip __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Duration )( 
            IFlip __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Width )( 
            IFlip __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Width )( 
            IFlip __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Height )( 
            IFlip __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Height )( 
            IFlip __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Depth )( 
            IFlip __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Depth )( 
            IFlip __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        END_INTERFACE
    } IFlipVtbl;

    interface IFlip
    {
        CONST_VTBL struct IFlipVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFlip_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFlip_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFlip_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFlip_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFlip_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFlip_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFlip_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IFlip_get_Capabilities(This,pVal)	\
    (This)->lpVtbl -> get_Capabilities(This,pVal)

#define IFlip_get_Progress(This,pVal)	\
    (This)->lpVtbl -> get_Progress(This,pVal)

#define IFlip_put_Progress(This,newVal)	\
    (This)->lpVtbl -> put_Progress(This,newVal)

#define IFlip_get_StepResolution(This,pVal)	\
    (This)->lpVtbl -> get_StepResolution(This,pVal)

#define IFlip_get_Duration(This,pVal)	\
    (This)->lpVtbl -> get_Duration(This,pVal)

#define IFlip_put_Duration(This,newVal)	\
    (This)->lpVtbl -> put_Duration(This,newVal)


#define IFlip_get_Width(This,pVal)	\
    (This)->lpVtbl -> get_Width(This,pVal)

#define IFlip_put_Width(This,newVal)	\
    (This)->lpVtbl -> put_Width(This,newVal)

#define IFlip_get_Height(This,pVal)	\
    (This)->lpVtbl -> get_Height(This,pVal)

#define IFlip_put_Height(This,newVal)	\
    (This)->lpVtbl -> put_Height(This,newVal)

#define IFlip_put_Depth(This,newVal)	\
    (This)->lpVtbl -> put_Depth(This,newVal)

#define IFlip_get_Depth(This,pVal)	\
    (This)->lpVtbl -> get_Depth(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFlip_get_Width_Proxy( 
    IFlip __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IFlip_get_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFlip_put_Width_Proxy( 
    IFlip __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IFlip_put_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFlip_get_Height_Proxy( 
    IFlip __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IFlip_get_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFlip_put_Height_Proxy( 
    IFlip __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IFlip_put_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFlip_put_Depth_Proxy( 
    IFlip __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IFlip_put_Depth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFlip_get_Depth_Proxy( 
    IFlip __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IFlip_get_Depth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFlip_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0191
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum HeightFieldDISPID
    {	DISPID_HeightField_Width	= DISPID_DXE_NEXT_ID,
	DISPID_HeightField_Height	= DISPID_HeightField_Width + 1,
	DISPID_HeightField_Depth	= DISPID_HeightField_Height + 1,
	DISPID_HeightField_Samples	= DISPID_HeightField_Depth + 1
    }	HeightFieldDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0191_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0191_v0_0_s_ifspec;

#ifndef __IHeightField_INTERFACE_DEFINED__
#define __IHeightField_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IHeightField
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IHeightField;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("0492170A-B159-11d1-9207-0000F8758E66")
    IHeightField : public IDXEffect
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Depth( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Depth( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Samples( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Samples( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHeightFieldVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHeightField __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHeightField __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IHeightField __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Capabilities )( 
            IHeightField __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Progress )( 
            IHeightField __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Progress )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StepResolution )( 
            IHeightField __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Duration )( 
            IHeightField __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Duration )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Width )( 
            IHeightField __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Width )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Height )( 
            IHeightField __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Height )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Depth )( 
            IHeightField __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Depth )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Samples )( 
            IHeightField __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Samples )( 
            IHeightField __RPC_FAR * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } IHeightFieldVtbl;

    interface IHeightField
    {
        CONST_VTBL struct IHeightFieldVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHeightField_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHeightField_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHeightField_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHeightField_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHeightField_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHeightField_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHeightField_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHeightField_get_Capabilities(This,pVal)	\
    (This)->lpVtbl -> get_Capabilities(This,pVal)

#define IHeightField_get_Progress(This,pVal)	\
    (This)->lpVtbl -> get_Progress(This,pVal)

#define IHeightField_put_Progress(This,newVal)	\
    (This)->lpVtbl -> put_Progress(This,newVal)

#define IHeightField_get_StepResolution(This,pVal)	\
    (This)->lpVtbl -> get_StepResolution(This,pVal)

#define IHeightField_get_Duration(This,pVal)	\
    (This)->lpVtbl -> get_Duration(This,pVal)

#define IHeightField_put_Duration(This,newVal)	\
    (This)->lpVtbl -> put_Duration(This,newVal)


#define IHeightField_get_Width(This,pVal)	\
    (This)->lpVtbl -> get_Width(This,pVal)

#define IHeightField_put_Width(This,newVal)	\
    (This)->lpVtbl -> put_Width(This,newVal)

#define IHeightField_get_Height(This,pVal)	\
    (This)->lpVtbl -> get_Height(This,pVal)

#define IHeightField_put_Height(This,newVal)	\
    (This)->lpVtbl -> put_Height(This,newVal)

#define IHeightField_get_Depth(This,pVal)	\
    (This)->lpVtbl -> get_Depth(This,pVal)

#define IHeightField_put_Depth(This,newVal)	\
    (This)->lpVtbl -> put_Depth(This,newVal)

#define IHeightField_get_Samples(This,pVal)	\
    (This)->lpVtbl -> get_Samples(This,pVal)

#define IHeightField_put_Samples(This,newVal)	\
    (This)->lpVtbl -> put_Samples(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IHeightField_get_Width_Proxy( 
    IHeightField __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IHeightField_get_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IHeightField_put_Width_Proxy( 
    IHeightField __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IHeightField_put_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IHeightField_get_Height_Proxy( 
    IHeightField __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IHeightField_get_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IHeightField_put_Height_Proxy( 
    IHeightField __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IHeightField_put_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IHeightField_get_Depth_Proxy( 
    IHeightField __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IHeightField_get_Depth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IHeightField_put_Depth_Proxy( 
    IHeightField __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IHeightField_put_Depth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IHeightField_get_Samples_Proxy( 
    IHeightField __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IHeightField_get_Samples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IHeightField_put_Samples_Proxy( 
    IHeightField __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IHeightField_put_Samples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHeightField_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0192
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum DXWIPEDIRECTION
    {	DXWD_HORIZONTAL	= 0,
	DXWD_VERTICAL	= DXWD_HORIZONTAL + 1
    }	DXWIPEDIRECTION;

typedef 
enum DXWIPEDISPID
    {	DISPID_DXW_GradientSize	= DISPID_DXE_NEXT_ID,
	DISPID_DXW_WipeStyle	= DISPID_DXW_GradientSize + 1
    }	DXWIPEDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0192_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0192_v0_0_s_ifspec;

#ifndef __IDXTWipe_INTERFACE_DEFINED__
#define __IDXTWipe_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXTWipe
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDXTWipe;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("AF279B2F-86EB-11D1-81BF-0000F87557DB")
    IDXTWipe : public IDXEffect
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GradientSize( 
            /* [retval][out] */ float __RPC_FAR *pPercentSize) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_GradientSize( 
            /* [in] */ float PercentSize) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_WipeStyle( 
            /* [retval][out] */ DXWIPEDIRECTION __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_WipeStyle( 
            /* [in] */ DXWIPEDIRECTION newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXTWipeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXTWipe __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXTWipe __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXTWipe __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDXTWipe __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDXTWipe __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDXTWipe __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDXTWipe __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Capabilities )( 
            IDXTWipe __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Progress )( 
            IDXTWipe __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Progress )( 
            IDXTWipe __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StepResolution )( 
            IDXTWipe __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Duration )( 
            IDXTWipe __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Duration )( 
            IDXTWipe __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_GradientSize )( 
            IDXTWipe __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pPercentSize);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_GradientSize )( 
            IDXTWipe __RPC_FAR * This,
            /* [in] */ float PercentSize);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WipeStyle )( 
            IDXTWipe __RPC_FAR * This,
            /* [retval][out] */ DXWIPEDIRECTION __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_WipeStyle )( 
            IDXTWipe __RPC_FAR * This,
            /* [in] */ DXWIPEDIRECTION newVal);
        
        END_INTERFACE
    } IDXTWipeVtbl;

    interface IDXTWipe
    {
        CONST_VTBL struct IDXTWipeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXTWipe_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXTWipe_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXTWipe_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXTWipe_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDXTWipe_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDXTWipe_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDXTWipe_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDXTWipe_get_Capabilities(This,pVal)	\
    (This)->lpVtbl -> get_Capabilities(This,pVal)

#define IDXTWipe_get_Progress(This,pVal)	\
    (This)->lpVtbl -> get_Progress(This,pVal)

#define IDXTWipe_put_Progress(This,newVal)	\
    (This)->lpVtbl -> put_Progress(This,newVal)

#define IDXTWipe_get_StepResolution(This,pVal)	\
    (This)->lpVtbl -> get_StepResolution(This,pVal)

#define IDXTWipe_get_Duration(This,pVal)	\
    (This)->lpVtbl -> get_Duration(This,pVal)

#define IDXTWipe_put_Duration(This,newVal)	\
    (This)->lpVtbl -> put_Duration(This,newVal)


#define IDXTWipe_get_GradientSize(This,pPercentSize)	\
    (This)->lpVtbl -> get_GradientSize(This,pPercentSize)

#define IDXTWipe_put_GradientSize(This,PercentSize)	\
    (This)->lpVtbl -> put_GradientSize(This,PercentSize)

#define IDXTWipe_get_WipeStyle(This,pVal)	\
    (This)->lpVtbl -> get_WipeStyle(This,pVal)

#define IDXTWipe_put_WipeStyle(This,newVal)	\
    (This)->lpVtbl -> put_WipeStyle(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTWipe_get_GradientSize_Proxy( 
    IDXTWipe __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pPercentSize);


void __RPC_STUB IDXTWipe_get_GradientSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTWipe_put_GradientSize_Proxy( 
    IDXTWipe __RPC_FAR * This,
    /* [in] */ float PercentSize);


void __RPC_STUB IDXTWipe_put_GradientSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDXTWipe_get_WipeStyle_Proxy( 
    IDXTWipe __RPC_FAR * This,
    /* [retval][out] */ DXWIPEDIRECTION __RPC_FAR *pVal);


void __RPC_STUB IDXTWipe_get_WipeStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IDXTWipe_put_WipeStyle_Proxy( 
    IDXTWipe __RPC_FAR * This,
    /* [in] */ DXWIPEDIRECTION newVal);


void __RPC_STUB IDXTWipe_put_WipeStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXTWipe_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0193
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum CRBLURDISPID
    {	DISPID_CRB_MakeShadow	= 1,
	DISPID_CRB_ShadowOpacity	= DISPID_CRB_MakeShadow + 1,
	DISPID_CRB_PixelRadius	= DISPID_CRB_ShadowOpacity + 1
    }	CRBLURDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0193_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0193_v0_0_s_ifspec;

#ifndef __ICrBlur_INTERFACE_DEFINED__
#define __ICrBlur_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: ICrBlur
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_ICrBlur;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("9F7C7827-E87A-11d1-81E0-0000F87557DB")
    ICrBlur : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MakeShadow( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MakeShadow( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ShadowOpacity( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ShadowOpacity( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PixelRadius( 
            /* [retval][out] */ float __RPC_FAR *pPixelRadius) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PixelRadius( 
            /* [in] */ float PixelRadius) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICrBlurVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICrBlur __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICrBlur __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICrBlur __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICrBlur __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICrBlur __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICrBlur __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICrBlur __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MakeShadow )( 
            ICrBlur __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MakeShadow )( 
            ICrBlur __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ShadowOpacity )( 
            ICrBlur __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ShadowOpacity )( 
            ICrBlur __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PixelRadius )( 
            ICrBlur __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pPixelRadius);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PixelRadius )( 
            ICrBlur __RPC_FAR * This,
            /* [in] */ float PixelRadius);
        
        END_INTERFACE
    } ICrBlurVtbl;

    interface ICrBlur
    {
        CONST_VTBL struct ICrBlurVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICrBlur_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICrBlur_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICrBlur_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICrBlur_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICrBlur_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICrBlur_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICrBlur_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICrBlur_get_MakeShadow(This,pVal)	\
    (This)->lpVtbl -> get_MakeShadow(This,pVal)

#define ICrBlur_put_MakeShadow(This,newVal)	\
    (This)->lpVtbl -> put_MakeShadow(This,newVal)

#define ICrBlur_get_ShadowOpacity(This,pVal)	\
    (This)->lpVtbl -> get_ShadowOpacity(This,pVal)

#define ICrBlur_put_ShadowOpacity(This,newVal)	\
    (This)->lpVtbl -> put_ShadowOpacity(This,newVal)

#define ICrBlur_get_PixelRadius(This,pPixelRadius)	\
    (This)->lpVtbl -> get_PixelRadius(This,pPixelRadius)

#define ICrBlur_put_PixelRadius(This,PixelRadius)	\
    (This)->lpVtbl -> put_PixelRadius(This,PixelRadius)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ICrBlur_get_MakeShadow_Proxy( 
    ICrBlur __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ICrBlur_get_MakeShadow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ICrBlur_put_MakeShadow_Proxy( 
    ICrBlur __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ICrBlur_put_MakeShadow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ICrBlur_get_ShadowOpacity_Proxy( 
    ICrBlur __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB ICrBlur_get_ShadowOpacity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ICrBlur_put_ShadowOpacity_Proxy( 
    ICrBlur __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB ICrBlur_put_ShadowOpacity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ICrBlur_get_PixelRadius_Proxy( 
    ICrBlur __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pPixelRadius);


void __RPC_STUB ICrBlur_get_PixelRadius_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ICrBlur_put_PixelRadius_Proxy( 
    ICrBlur __RPC_FAR * This,
    /* [in] */ float PixelRadius);


void __RPC_STUB ICrBlur_put_PixelRadius_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICrBlur_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0194
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum CRENGRAVEDISPID
    {	DISPID_CREN_Bias	= 1
    }	CRENGRAVEDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0194_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0194_v0_0_s_ifspec;

#ifndef __ICrEngrave_INTERFACE_DEFINED__
#define __ICrEngrave_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: ICrEngrave
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_ICrEngrave;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("E4ACFB7F-053E-11d2-81EA-0000F87557DB")
    ICrEngrave : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Bias( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Bias( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICrEngraveVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICrEngrave __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICrEngrave __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICrEngrave __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICrEngrave __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICrEngrave __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICrEngrave __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICrEngrave __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Bias )( 
            ICrEngrave __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Bias )( 
            ICrEngrave __RPC_FAR * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } ICrEngraveVtbl;

    interface ICrEngrave
    {
        CONST_VTBL struct ICrEngraveVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICrEngrave_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICrEngrave_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICrEngrave_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICrEngrave_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICrEngrave_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICrEngrave_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICrEngrave_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICrEngrave_get_Bias(This,pVal)	\
    (This)->lpVtbl -> get_Bias(This,pVal)

#define ICrEngrave_put_Bias(This,newVal)	\
    (This)->lpVtbl -> put_Bias(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ICrEngrave_get_Bias_Proxy( 
    ICrEngrave __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB ICrEngrave_get_Bias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ICrEngrave_put_Bias_Proxy( 
    ICrEngrave __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB ICrEngrave_put_Bias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICrEngrave_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_dxtmsft_0195
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [local] */ 


typedef 
enum CREMBOSSDISPID
    {	DISPID_CREM_Bias	= 1
    }	CREMBOSSDISPID;



extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0195_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dxtmsft_0195_v0_0_s_ifspec;

#ifndef __ICrEmboss_INTERFACE_DEFINED__
#define __ICrEmboss_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: ICrEmboss
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_ICrEmboss;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("E4ACFB80-053E-11d2-81EA-0000F87557DB")
    ICrEmboss : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Bias( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Bias( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICrEmbossVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICrEmboss __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICrEmboss __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICrEmboss __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICrEmboss __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICrEmboss __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICrEmboss __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICrEmboss __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Bias )( 
            ICrEmboss __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Bias )( 
            ICrEmboss __RPC_FAR * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } ICrEmbossVtbl;

    interface ICrEmboss
    {
        CONST_VTBL struct ICrEmbossVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICrEmboss_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICrEmboss_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICrEmboss_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICrEmboss_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICrEmboss_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICrEmboss_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICrEmboss_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICrEmboss_get_Bias(This,pVal)	\
    (This)->lpVtbl -> get_Bias(This,pVal)

#define ICrEmboss_put_Bias(This,newVal)	\
    (This)->lpVtbl -> put_Bias(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ICrEmboss_get_Bias_Proxy( 
    ICrEmboss __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB ICrEmboss_get_Bias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ICrEmboss_put_Bias_Proxy( 
    ICrEmboss __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB ICrEmboss_put_Bias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICrEmboss_INTERFACE_DEFINED__ */


#ifndef __IDXTFade_INTERFACE_DEFINED__
#define __IDXTFade_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXTFade
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDXTFade;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("16B280C4-EE70-11D1-9066-00C04FD9189D")
    IDXTFade : public IDXEffect
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Overlap( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Overlap( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Center( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Center( 
            /* [in] */ BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXTFadeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXTFade __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXTFade __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXTFade __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDXTFade __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDXTFade __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDXTFade __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDXTFade __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Capabilities )( 
            IDXTFade __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Progress )( 
            IDXTFade __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Progress )( 
            IDXTFade __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StepResolution )( 
            IDXTFade __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Duration )( 
            IDXTFade __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Duration )( 
            IDXTFade __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Overlap )( 
            IDXTFade __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Overlap )( 
            IDXTFade __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Center )( 
            IDXTFade __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Center )( 
            IDXTFade __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        END_INTERFACE
    } IDXTFadeVtbl;

    interface IDXTFade
    {
        CONST_VTBL struct IDXTFadeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXTFade_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXTFade_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXTFade_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXTFade_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDXTFade_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDXTFade_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDXTFade_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDXTFade_get_Capabilities(This,pVal)	\
    (This)->lpVtbl -> get_Capabilities(This,pVal)

#define IDXTFade_get_Progress(This,pVal)	\
    (This)->lpVtbl -> get_Progress(This,pVal)

#define IDXTFade_put_Progress(This,newVal)	\
    (This)->lpVtbl -> put_Progress(This,newVal)

#define IDXTFade_get_StepResolution(This,pVal)	\
    (This)->lpVtbl -> get_StepResolution(This,pVal)

#define IDXTFade_get_Duration(This,pVal)	\
    (This)->lpVtbl -> get_Duration(This,pVal)

#define IDXTFade_put_Duration(This,newVal)	\
    (This)->lpVtbl -> put_Duration(This,newVal)


#define IDXTFade_get_Overlap(This,pVal)	\
    (This)->lpVtbl -> get_Overlap(This,pVal)

#define IDXTFade_put_Overlap(This,newVal)	\
    (This)->lpVtbl -> put_Overlap(This,newVal)

#define IDXTFade_get_Center(This,pVal)	\
    (This)->lpVtbl -> get_Center(This,pVal)

#define IDXTFade_put_Center(This,newVal)	\
    (This)->lpVtbl -> put_Center(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXTFade_get_Overlap_Proxy( 
    IDXTFade __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDXTFade_get_Overlap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXTFade_put_Overlap_Proxy( 
    IDXTFade __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDXTFade_put_Overlap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXTFade_get_Center_Proxy( 
    IDXTFade __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IDXTFade_get_Center_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXTFade_put_Center_Proxy( 
    IDXTFade __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IDXTFade_put_Center_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXTFade_INTERFACE_DEFINED__ */


#ifndef __IDXBasicImage_INTERFACE_DEFINED__
#define __IDXBasicImage_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXBasicImage
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDXBasicImage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("16B280C7-EE70-11D1-9066-00C04FD9189D")
    IDXBasicImage : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Rotation( 
            /* [retval][out] */ int __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Rotation( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mirror( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Mirror( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GrayScale( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GrayScale( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Opacity( 
            /* [retval][out] */ float __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Opacity( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Invert( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Invert( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XRay( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_XRay( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mask( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Mask( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaskColor( 
            /* [retval][out] */ int __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaskColor( 
            /* [in] */ int newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXBasicImageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXBasicImage __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXBasicImage __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDXBasicImage __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Rotation )( 
            IDXBasicImage __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Rotation )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Mirror )( 
            IDXBasicImage __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Mirror )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_GrayScale )( 
            IDXBasicImage __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_GrayScale )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Opacity )( 
            IDXBasicImage __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Opacity )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Invert )( 
            IDXBasicImage __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Invert )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_XRay )( 
            IDXBasicImage __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_XRay )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Mask )( 
            IDXBasicImage __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Mask )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaskColor )( 
            IDXBasicImage __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaskColor )( 
            IDXBasicImage __RPC_FAR * This,
            /* [in] */ int newVal);
        
        END_INTERFACE
    } IDXBasicImageVtbl;

    interface IDXBasicImage
    {
        CONST_VTBL struct IDXBasicImageVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXBasicImage_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXBasicImage_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXBasicImage_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXBasicImage_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDXBasicImage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDXBasicImage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDXBasicImage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDXBasicImage_get_Rotation(This,pVal)	\
    (This)->lpVtbl -> get_Rotation(This,pVal)

#define IDXBasicImage_put_Rotation(This,newVal)	\
    (This)->lpVtbl -> put_Rotation(This,newVal)

#define IDXBasicImage_get_Mirror(This,pVal)	\
    (This)->lpVtbl -> get_Mirror(This,pVal)

#define IDXBasicImage_put_Mirror(This,newVal)	\
    (This)->lpVtbl -> put_Mirror(This,newVal)

#define IDXBasicImage_get_GrayScale(This,pVal)	\
    (This)->lpVtbl -> get_GrayScale(This,pVal)

#define IDXBasicImage_put_GrayScale(This,newVal)	\
    (This)->lpVtbl -> put_GrayScale(This,newVal)

#define IDXBasicImage_get_Opacity(This,pVal)	\
    (This)->lpVtbl -> get_Opacity(This,pVal)

#define IDXBasicImage_put_Opacity(This,newVal)	\
    (This)->lpVtbl -> put_Opacity(This,newVal)

#define IDXBasicImage_get_Invert(This,pVal)	\
    (This)->lpVtbl -> get_Invert(This,pVal)

#define IDXBasicImage_put_Invert(This,newVal)	\
    (This)->lpVtbl -> put_Invert(This,newVal)

#define IDXBasicImage_get_XRay(This,pVal)	\
    (This)->lpVtbl -> get_XRay(This,pVal)

#define IDXBasicImage_put_XRay(This,newVal)	\
    (This)->lpVtbl -> put_XRay(This,newVal)

#define IDXBasicImage_get_Mask(This,pVal)	\
    (This)->lpVtbl -> get_Mask(This,pVal)

#define IDXBasicImage_put_Mask(This,newVal)	\
    (This)->lpVtbl -> put_Mask(This,newVal)

#define IDXBasicImage_get_MaskColor(This,pVal)	\
    (This)->lpVtbl -> get_MaskColor(This,pVal)

#define IDXBasicImage_put_MaskColor(This,newVal)	\
    (This)->lpVtbl -> put_MaskColor(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_get_Rotation_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *pVal);


void __RPC_STUB IDXBasicImage_get_Rotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_put_Rotation_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [in] */ int newVal);


void __RPC_STUB IDXBasicImage_put_Rotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_get_Mirror_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IDXBasicImage_get_Mirror_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_put_Mirror_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IDXBasicImage_put_Mirror_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_get_GrayScale_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IDXBasicImage_get_GrayScale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_put_GrayScale_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IDXBasicImage_put_GrayScale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_get_Opacity_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *pVal);


void __RPC_STUB IDXBasicImage_get_Opacity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_put_Opacity_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [in] */ float newVal);


void __RPC_STUB IDXBasicImage_put_Opacity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_get_Invert_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IDXBasicImage_get_Invert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_put_Invert_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IDXBasicImage_put_Invert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_get_XRay_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IDXBasicImage_get_XRay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_put_XRay_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IDXBasicImage_put_XRay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_get_Mask_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IDXBasicImage_get_Mask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_put_Mask_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IDXBasicImage_put_Mask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_get_MaskColor_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *pVal);


void __RPC_STUB IDXBasicImage_get_MaskColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXBasicImage_put_MaskColor_Proxy( 
    IDXBasicImage __RPC_FAR * This,
    /* [in] */ int newVal);


void __RPC_STUB IDXBasicImage_put_MaskColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXBasicImage_INTERFACE_DEFINED__ */


#ifndef __IDXPixelate_INTERFACE_DEFINED__
#define __IDXPixelate_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDXPixelate
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_IDXPixelate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("D33E180F-FBE9-11d1-906A-00C04FD9189D")
    IDXPixelate : public IDXEffect
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxSquare( 
            /* [retval][out] */ int __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxSquare( 
            /* [in] */ int newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDXPixelateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDXPixelate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDXPixelate __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDXPixelate __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDXPixelate __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDXPixelate __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDXPixelate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDXPixelate __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Capabilities )( 
            IDXPixelate __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Progress )( 
            IDXPixelate __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Progress )( 
            IDXPixelate __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StepResolution )( 
            IDXPixelate __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Duration )( 
            IDXPixelate __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Duration )( 
            IDXPixelate __RPC_FAR * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaxSquare )( 
            IDXPixelate __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaxSquare )( 
            IDXPixelate __RPC_FAR * This,
            /* [in] */ int newVal);
        
        END_INTERFACE
    } IDXPixelateVtbl;

    interface IDXPixelate
    {
        CONST_VTBL struct IDXPixelateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXPixelate_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDXPixelate_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDXPixelate_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDXPixelate_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDXPixelate_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDXPixelate_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDXPixelate_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDXPixelate_get_Capabilities(This,pVal)	\
    (This)->lpVtbl -> get_Capabilities(This,pVal)

#define IDXPixelate_get_Progress(This,pVal)	\
    (This)->lpVtbl -> get_Progress(This,pVal)

#define IDXPixelate_put_Progress(This,newVal)	\
    (This)->lpVtbl -> put_Progress(This,newVal)

#define IDXPixelate_get_StepResolution(This,pVal)	\
    (This)->lpVtbl -> get_StepResolution(This,pVal)

#define IDXPixelate_get_Duration(This,pVal)	\
    (This)->lpVtbl -> get_Duration(This,pVal)

#define IDXPixelate_put_Duration(This,newVal)	\
    (This)->lpVtbl -> put_Duration(This,newVal)


#define IDXPixelate_get_MaxSquare(This,pVal)	\
    (This)->lpVtbl -> get_MaxSquare(This,pVal)

#define IDXPixelate_put_MaxSquare(This,newVal)	\
    (This)->lpVtbl -> put_MaxSquare(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDXPixelate_get_MaxSquare_Proxy( 
    IDXPixelate __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *pVal);


void __RPC_STUB IDXPixelate_get_MaxSquare_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDXPixelate_put_MaxSquare_Proxy( 
    IDXPixelate __RPC_FAR * This,
    /* [in] */ int newVal);


void __RPC_STUB IDXPixelate_put_MaxSquare_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDXPixelate_INTERFACE_DEFINED__ */



#ifndef __DXTMSFTLib_LIBRARY_DEFINED__
#define __DXTMSFTLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: DXTMSFTLib
 * at Wed Sep 02 08:16:12 1998
 * using MIDL 3.02.88
 ****************************************/
/* [helpstring][version][uuid] */ 



EXTERN_C const IID LIBID_DXTMSFTLib;

EXTERN_C const CLSID CLSID_Explode;

#ifdef __cplusplus

class DECLSPEC_UUID("141DBAF1-55FB-11D1-B83E-00A0C933BE86")
Explode;
#endif

EXTERN_C const CLSID CLSID_ExplodeProp;

#ifdef __cplusplus

class DECLSPEC_UUID("C53059E1-E6E3-11d1-BA12-00C04FB6BD36")
ExplodeProp;
#endif

EXTERN_C const CLSID CLSID_Ruffle;

#ifdef __cplusplus

class DECLSPEC_UUID("69EE49C5-6268-11D1-B83E-00A0C933BE86")
Ruffle;
#endif

EXTERN_C const CLSID CLSID_RuffleProp;

#ifdef __cplusplus

class DECLSPEC_UUID("A21CA3E1-E6DA-11d1-BA12-00C04FB6BD36")
RuffleProp;
#endif

EXTERN_C const CLSID CLSID_Ripple;

#ifdef __cplusplus

class DECLSPEC_UUID("945F5842-3A8D-11D1-9037-00C04FD9189D")
Ripple;
#endif

EXTERN_C const CLSID CLSID_RipProp;

#ifdef __cplusplus

class DECLSPEC_UUID("945F5843-3A8D-11D1-9037-00C04FD9189D")
RipProp;
#endif

EXTERN_C const CLSID CLSID_Dissolve;

#ifdef __cplusplus

class DECLSPEC_UUID("90b5e2e1-5050-11d1-b83e-00a0c933be86")
Dissolve;
#endif

EXTERN_C const CLSID CLSID_Flip;

#ifdef __cplusplus

class DECLSPEC_UUID("3A5EA1BA-3C28-11D1-9039-00C04FD9189D")
Flip;
#endif

EXTERN_C const CLSID CLSID_HeightField;

#ifdef __cplusplus

class DECLSPEC_UUID("04921709-B159-11d1-9207-0000F8758E66")
HeightField;
#endif

EXTERN_C const CLSID CLSID_DXTComposite;

#ifdef __cplusplus

class DECLSPEC_UUID("9A43A844-0831-11D1-817F-0000F87557DB")
DXTComposite;
#endif

EXTERN_C const CLSID CLSID_DXTResampler;

#ifdef __cplusplus

class DECLSPEC_UUID("015E2939-C341-11d1-81D1-0000F87557DB")
DXTResampler;
#endif

EXTERN_C const CLSID CLSID_DXBufferCache;

#ifdef __cplusplus

class DECLSPEC_UUID("3E669F1A-9C23-11d1-9053-00C04FD9189D")
DXBufferCache;
#endif

EXTERN_C const CLSID CLSID_DXLUTBuilder;

#ifdef __cplusplus

class DECLSPEC_UUID("1E54333B-2A00-11d1-8198-0000F87557DB")
DXLUTBuilder;
#endif

EXTERN_C const CLSID CLSID_DXTGradientD;

#ifdef __cplusplus

class DECLSPEC_UUID("623E2882-FC0E-11d1-9A77-0000F8756A10")
DXTGradientD;
#endif

EXTERN_C const CLSID CLSID_DXTWipe;

#ifdef __cplusplus

class DECLSPEC_UUID("AF279B30-86EB-11D1-81BF-0000F87557DB")
DXTWipe;
#endif

EXTERN_C const CLSID CLSID_DXTConvolution;

#ifdef __cplusplus

class DECLSPEC_UUID("2BC0EF29-E6BA-11d1-81DD-0000F87557DB")
DXTConvolution;
#endif

EXTERN_C const CLSID CLSID_CrBlur;

#ifdef __cplusplus

class DECLSPEC_UUID("7312498D-E87A-11d1-81E0-0000F87557DB")
CrBlur;
#endif

EXTERN_C const CLSID CLSID_CrEmboss;

#ifdef __cplusplus

class DECLSPEC_UUID("F515306D-0156-11d2-81EA-0000F87557DB")
CrEmboss;
#endif

EXTERN_C const CLSID CLSID_CrEngrave;

#ifdef __cplusplus

class DECLSPEC_UUID("F515306E-0156-11d2-81EA-0000F87557DB")
CrEngrave;
#endif

EXTERN_C const CLSID CLSID_DXFade;

#ifdef __cplusplus

class DECLSPEC_UUID("16B280C5-EE70-11D1-9066-00C04FD9189D")
DXFade;
#endif

EXTERN_C const CLSID CLSID_FadePP;

#ifdef __cplusplus

class DECLSPEC_UUID("16B280C6-EE70-11D1-9066-00C04FD9189D")
FadePP;
#endif

EXTERN_C const CLSID CLSID_BasicImageEffects;

#ifdef __cplusplus

class DECLSPEC_UUID("16B280C8-EE70-11D1-9066-00C04FD9189D")
BasicImageEffects;
#endif

EXTERN_C const CLSID CLSID_BasicImageEffectsPP;

#ifdef __cplusplus

class DECLSPEC_UUID("16B280C9-EE70-11D1-9066-00C04FD9189D")
BasicImageEffectsPP;
#endif

EXTERN_C const CLSID CLSID_Pixelate;

#ifdef __cplusplus

class DECLSPEC_UUID("4CCEA634-FBE0-11d1-906A-00C04FD9189D")
Pixelate;
#endif

EXTERN_C const CLSID CLSID_PixelatePP;

#ifdef __cplusplus

class DECLSPEC_UUID("4CCEA635-FBE0-11d1-906A-00C04FD9189D")
PixelatePP;
#endif

EXTERN_C const CLSID CLSID_DXTWipePP;

#ifdef __cplusplus

class DECLSPEC_UUID("7FFE4D08-FBFD-11d1-9A77-0000F8756A10")
DXTWipePP;
#endif

EXTERN_C const CLSID CLSID_ResamplePP;

#ifdef __cplusplus

class DECLSPEC_UUID("623E287C-FC0E-11d1-9A77-0000F8756A10")
ResamplePP;
#endif

EXTERN_C const CLSID CLSID_CrBlurPP;

#ifdef __cplusplus

class DECLSPEC_UUID("623E287E-FC0E-11d1-9A77-0000F8756A10")
CrBlurPP;
#endif

EXTERN_C const CLSID CLSID_GradientPP;

#ifdef __cplusplus

class DECLSPEC_UUID("623E2880-FC0E-11d1-9A77-0000F8756A10")
GradientPP;
#endif

EXTERN_C const CLSID CLSID_CompositePP;

#ifdef __cplusplus

class DECLSPEC_UUID("25B33660-FD83-11d1-8ADE-444553540001")
CompositePP;
#endif

EXTERN_C const CLSID CLSID_ConvolvePP;

#ifdef __cplusplus

class DECLSPEC_UUID("25B33661-FD83-11d1-8ADE-444553540001")
ConvolvePP;
#endif

EXTERN_C const CLSID CLSID_LUTBuilderPP;

#ifdef __cplusplus

class DECLSPEC_UUID("25B33662-FD83-11d1-8ADE-444553540001")
LUTBuilderPP;
#endif

EXTERN_C const CLSID CLSID_FlipProp;

#ifdef __cplusplus

class DECLSPEC_UUID("7A8402E0-FBD6-11D1-B5E0-00AA003B6061")
FlipProp;
#endif

EXTERN_C const CLSID CLSID_HtFieldProp;

#ifdef __cplusplus

class DECLSPEC_UUID("7A8402E3-FBD6-11D1-B5E0-00AA003B6061")
HtFieldProp;
#endif

EXTERN_C const CLSID CLSID_Disslvpp;

#ifdef __cplusplus

class DECLSPEC_UUID("9A8A1CC2-FEEA-11D1-B5E0-00AA003B6061")
Disslvpp;
#endif
#endif /* __DXTMSFTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long __RPC_FAR *, unsigned long            , LPSAFEARRAY __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
