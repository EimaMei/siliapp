/*
siligl.h - a cross-platform OpenGL headers package, all in one file.
===========================================================================
	- YOU MUST DEFINE 'SI_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SI_IMPLEMENTATION
		#include "sili.h"

	- All other files should just include the library without the #define macro.

	- If you want to disable certain features, you can do:
		- #define SI_NO_ALLOCATOR
		- #define SI_NO_PAIR
	before the SI_IMPLEMENTATION macro, as well as before any other include of
	`sili.h`.

	- NOTE: Disabling certain features may or may not create 'undefined'
	compiler errors at certain spots. Use it at your own risk.

===========================================================================
DOCUMENTATION
	- All functions, constant variables and macros contain a comment with a description
	of what they do above them, as well what they return (if anything). Macros
	specifically use a consistent format because of their lack of typing.
	That being:
		/ argumentName - type | otherArgumentName - KEYWORD | ...VALUES - TYPE*
		description of the macro. /
		#define smth(argumentName, otherArgumentName, .../ VALUES/)

	- More often than not a macro's argument will not be a specific type and instead
	some kind of 'text'. Such arguments are noted if their type denotation is
	A FULLY CAPITALIZED KEYWORD. A general list of the keywords, what they mean
	and examples what to input:
		- TYPE - argument is just the type name (siString, usize, rawptr).
		- TYPE* - same as TYPE except it's a pointer (siString*, usize*, rawptr*).
		- INT - argument can be any integer (50, -250LL, ISIZE_MAX).
		- UINT - argument can be any UNSIGNED integer (50, 250LL, USIZE_MAX).
		- EXPRESSION - argument is just some kind of value (60, "hello", SI_RGB(255, 255, 255)).
		- NAME - argument has to be regular text with _no_ enquotes (test, var, len).
		- ANYTHING - argument can be literally anything.

===========================================================================
MACROS
	- For any of the macros to work, you must _always_ define it before including
	the library. Example:
	```c
		#define SI_IMPLEMENTATION
		#define SI_RELEASE_MODE
		#include "sili.h"
	```

	- SI_NO_WINDOWS_H - disables the inclusion of windows.h inside sili.h. Might
	cause 'undefined' compiler errors.

===========================================================================
CREDITS
	- Ginger Bill's 'gb.h' (https://github.com//gingerBill/gb) - inspired me to
	make the Sili Toolchain, as well as certain features were taken from the
	library and implemented here.

LICENSE:
	- This software is licensed under the zlib license (see the LICENSE in the
	bottom of the file).

WARNING
	- This library is _slightly_ experimental and features may not work as
	expected.
	- This also means that functions may not be documented.

*/

#ifndef SIGL_INCLUDE_SIGL_H
#define SIGL_INCLUDE_SIGL_H

#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(SIGL_SYSTEM_IS_WINDOWS)
	#define SIGL_SYSTEM_IS_WINDOWS (_WIN32 == 1 || _WIN64 == 1 || (__CYGWIN__ == 1))
#endif

#if !defined(SIGL_SYSTEM_IS_UNIX)
	#define SIGL_SYSTEM_IS_UNIX (__unix__ == 1 && __APPLE__ == 0 && __MACH__ == 0)
#endif

#if !defined(SIGL_SYSTEM_IS_64BIT)
	#define SIGL_SYSTEM_IS_64BIT (__SIZEOF_LONG__ == 8 || _WIN64 == 1 || __amd64__ == 1 || __amd64 == 1 || __x86_64__ == 1 || __x86_64 == 1 || _M_X64 == 1 || _M_AMD64 == 1 || __aarch64__ == 1 || __powerpc64__ == 1 || __ppc64__ == 1 || __PPC64__ == 1 || _ARCH_PPC64 == 1)
#endif

#if !defined(SIGL_INT64)
	#define SIGL_INT64 long long
#endif

#if !defined(SIGL_UINT64)
	#define SIGL_UINT64 unsigned long long
#endif

#if !defined(SIGL_INTPTR)
	#if SIGL_SYSTEM_IS_64BIT
		#define SIGL_INTPTR SIGL_INT64
	#else
		#define SIGL_INTPTR int
	#endif
#endif

#if defined(SIGL_NO_OPENGL_TYPES)
	#define SIGL_NO_GL_1_1_H_TYPES
	#define SIGL_NO_GL_1_2_H_TYPES
	#define SIGL_NO_GL_1_5_H_TYPES
	#define SIGL_NO_GL_2_0_H_TYPES
	#define SIGL_NO_GL_3_0_H_TYPES
#endif /* SIGL_NO_OPENGL_TYPES */

#if defined(__APPLE__)
	#define SIGL_NO_GL_4_2_H_FUNCS
	#define SIGL_NO_GL_4_3_H_FUNCS
	#define SIGL_NO_GL_4_4_H_FUNCS
	#define SIGL_NO_GL_4_5_H_FUNCS
	#define SIGL_NO_GL_4_6_H_FUNCS
#endif

#if SIGL_SYSTEM_IS_WINDOWS && !defined(SIGL_NO_WINDOWS_H)
	#define NOMINMAX            1
	#define WIN32_LEAN_AND_MEAN 1
	#define WIN32_MEAN_AND_LEAN 1
	#define VC_EXTRALEAN        1
	#include <windows.h>
	#undef NOMINMAX
	#undef WIN32_LEAN_AND_MEAN
	#undef WIN32_MEAN_AND_LEAN
	#undef VC_EXTRALEAN
    typedef PROC siglOsFuncType;
#elif SIGL_SYSTEM_IS_UNIX && !defined(SIGL_NO_GLX_H)
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    typedef void (*siglOsFuncType)(void);
#endif

#if SIGL_SYSTEM_IS_WINDOWS
	#define GLAPI DECLSPEC_IMPORT
	#define GLAPIENTRY __stdcall
#else
    #define GLAPI extern
    #define GLAPIENTRY
#endif

#if !defined(SIGL_NO_GL_1_1_H_TYPES) && !defined(__gl_h_) && !defined(__GL_H_)
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
#endif
#if !defined(SIGL_NO_GL_1_5_H_TYPES) && !defined(GL_VERSION_1_5)
typedef SIGL_INTPTR GLintptr;
typedef SIGL_INTPTR GLsizeiptr;
#endif
#if !defined(SIGL_NO_GL_2_0_H_TYPES) && !defined(GL_VERSION_2_0)
typedef char GLchar;
#endif
#if !defined(SIGL_NO_GL_3_0_H_TYPES) && !defined(GL_VERSION_3_0)
typedef unsigned short GLhalf;
#endif
#if !defined(SIGL_NO_GL_3_2_H_TYPES) && !defined(GL_VERSION_3_2)
typedef struct __GLsync *GLsync;
typedef SIGL_UINT64 GLuint64;
typedef SIGL_INT64 GLint64;
#endif
#if SIGL_SYSTEM_IS_WINDOWS && !defined(SIGL_NO_WGL_H_TYPES) && !defined(__wgl_wgl_h_)
#if defined(WGL_ARB_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
typedef void* HPBUFFERARB;
#endif /* WGL_ARB_pbuffer */
#if defined(WGL_EXT_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
typedef void* HPBUFFEREXT;
#endif /* WGL_EXT_pbuffer */
#if defined(WGL_NV_gpu_affinity) || defined(SIGL_INCLUDE_WGL_EXT)
typedef void* HGPUNV;
struct _GPU_DEVICE {
	unsigned long cb;
	char   DeviceName[32];
	char   DeviceString[128];
	unsigned long  Flags;
#if !defined(_WINDOWS_)
	struct { long left; long top; long right; long bottom; } rcVirtualScreen;
#else
	RECT rcVirtualScreen;
#endif
};
typedef struct _GPU_DEVICE *PGPU_DEVICE;
#endif /* WGL_NV_gpu_affinity */
#if defined(WGL_NV_present_video) || defined(SIGL_INCLUDE_WGL_EXT)
typedef void* HVIDEOOUTPUTDEVICENV;
#endif /* WGL_NV_present_video */
#if defined(WGL_NV_video_capture) || defined(SIGL_INCLUDE_WGL_EXT)
typedef void* HVIDEOINPUTDEVICENV;
#endif /* WGL_NV_video_capture */
#if defined(WGL_NV_video_output) || defined(SIGL_INCLUDE_WGL_EXT)
typedef void* HPVIDEODEV;
#endif /* WGL_NV_video_output */
#endif
#if SIGL_SYSTEM_IS_UNIX && !defined(SIGL_NO_GLX_H_TYPES) && !defined(GLX_H)
typedef struct __GLXcontextRec *GLXContext;
typedef XID GLXPixmap;
typedef XID GLXDrawable;
/* GLX 1.3 and later */
typedef struct __GLXFBConfigRec *GLXFBConfig;
typedef XID GLXFBConfigID;
typedef XID GLXContextID;
typedef XID GLXWindow;
typedef XID GLXPbuffer;

typedef void ( *__GLXextFuncPtr)(void);

/*
** GLX Events
*/
typedef struct {
    int event_type;		/* GLX_DAMAGED or GLX_SAVED */
    int draw_type;		/* GLX_WINDOW or GLX_PBUFFER */
    unsigned long serial;	/* # of last request processed by server */
    Bool send_event;		/* true if this came for SendEvent request */
    Display *display;		/* display the event was read from */
    GLXDrawable drawable;	/* XID of Drawable */
    unsigned int buffer_mask;	/* mask indicating which buffers are affected */
    unsigned int aux_buffer;	/* which aux buffer was affected */
    int x, y;
    int width, height;
    int count;			/* if nonzero, at least this many more */
} GLXPbufferClobberEvent;

typedef struct {
    int type;
    unsigned long serial;	/* # of last request processed by server */
    Bool send_event;		/* true if this came from a SendEvent request */
    Display *display;		/* Display the event was read from */
    Drawable drawable;	/* drawable on which event was requested in event mask */
    int event_type;
    int64_t ust;
    int64_t msc;
    int64_t sbc;
} GLXBufferSwapComplete;

typedef union __GLXEvent {
    GLXPbufferClobberEvent glxpbufferclobber;
    GLXBufferSwapComplete glxbufferswapcomplete;
    long pad[24];
} GLXEvent;
#endif

#if !defined(SIGL_NO_GL_1_1_H_MACROS)
#ifndef GL_VERSION_1_1
#define GL_VERSION_1_1 1

#define GL_ACCUM 0x0100
#define GL_LOAD 0x0101
#define GL_RETURN 0x0102
#define GL_MULT 0x0103
#define GL_ADD 0x0104

#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207

#define GL_CURRENT_BIT 0x00000001
#define GL_POINT_BIT 0x00000002
#define GL_LINE_BIT 0x00000004
#define GL_POLYGON_BIT 0x00000008
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#define GL_PIXEL_MODE_BIT 0x00000020
#define GL_LIGHTING_BIT 0x00000040
#define GL_FOG_BIT 0x00000080
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_ACCUM_BUFFER_BIT 0x00000200
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_VIEWPORT_BIT 0x00000800
#define GL_TRANSFORM_BIT 0x00001000
#define GL_ENABLE_BIT 0x00002000
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_HINT_BIT 0x00008000
#define GL_EVAL_BIT 0x00010000
#define GL_LIST_BIT 0x00020000
#define GL_TEXTURE_BIT 0x00040000
#define GL_SCISSOR_BIT 0x00080000
#define GL_ALL_ATTRIB_BITS 0x000fffff

#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON 0x0009

#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305

#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308

#define GL_TRUE 1
#define GL_FALSE 0

#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_PLANE1 0x3001
#define GL_CLIP_PLANE2 0x3002
#define GL_CLIP_PLANE3 0x3003
#define GL_CLIP_PLANE4 0x3004
#define GL_CLIP_PLANE5 0x3005

#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_2_BYTES 0x1407
#define GL_3_BYTES 0x1408
#define GL_4_BYTES 0x1409
#define GL_DOUBLE 0x140A

#define GL_NONE 0
#define GL_FRONT_LEFT 0x0400
#define GL_FRONT_RIGHT 0x0401
#define GL_BACK_LEFT 0x0402
#define GL_BACK_RIGHT 0x0403
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_AND_BACK 0x0408
#define GL_AUX0 0x0409
#define GL_AUX1 0x040A
#define GL_AUX2 0x040B
#define GL_AUX3 0x040C

#define GL_NO_ERROR 0
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_OUT_OF_MEMORY 0x0505

#define GL_2D 0x0600
#define GL_3D 0x0601
#define GL_3D_COLOR 0x0602
#define GL_3D_COLOR_TEXTURE 0x0603
#define GL_4D_COLOR_TEXTURE 0x0604

#define GL_PASS_THROUGH_TOKEN 0x0700
#define GL_POINT_TOKEN 0x0701
#define GL_LINE_TOKEN 0x0702
#define GL_POLYGON_TOKEN 0x0703
#define GL_BITMAP_TOKEN 0x0704
#define GL_DRAW_PIXEL_TOKEN 0x0705
#define GL_COPY_PIXEL_TOKEN 0x0706
#define GL_LINE_RESET_TOKEN 0x0707

#define GL_EXP 0x0800
#define GL_EXP2 0x0801

#define GL_CW 0x0900
#define GL_CCW 0x0901

#define GL_COEFF 0x0A00
#define GL_ORDER 0x0A01
#define GL_DOMAIN 0x0A02

#define GL_CURRENT_COLOR 0x0B00
#define GL_CURRENT_INDEX 0x0B01
#define GL_CURRENT_NORMAL 0x0B02
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#define GL_CURRENT_RASTER_COLOR 0x0B04
#define GL_CURRENT_RASTER_INDEX 0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#define GL_CURRENT_RASTER_POSITION 0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID 0x0B08
#define GL_CURRENT_RASTER_DISTANCE 0x0B09
#define GL_POINT_SMOOTH 0x0B10
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_RANGE 0x0B12
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_WIDTH 0x0B21
#define GL_LINE_WIDTH_RANGE 0x0B22
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_LINE_STIPPLE 0x0B24
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#define GL_LIST_MODE 0x0B30
#define GL_MAX_LIST_NESTING 0x0B31
#define GL_LIST_BASE 0x0B32
#define GL_LIST_INDEX 0x0B33
#define GL_POLYGON_MODE 0x0B40
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_POLYGON_STIPPLE 0x0B42
#define GL_EDGE_FLAG 0x0B43
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_LIGHTING 0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER 0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE 0x0B52
#define GL_LIGHT_MODEL_AMBIENT 0x0B53
#define GL_SHADE_MODEL 0x0B54
#define GL_COLOR_MATERIAL_FACE 0x0B55
#define GL_COLOR_MATERIAL_PARAMETER 0x0B56
#define GL_COLOR_MATERIAL 0x0B57
#define GL_FOG 0x0B60
#define GL_FOG_INDEX 0x0B61
#define GL_FOG_DENSITY 0x0B62
#define GL_FOG_START 0x0B63
#define GL_FOG_END 0x0B64
#define GL_FOG_MODE 0x0B65
#define GL_FOG_COLOR 0x0B66
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC 0x0B74
#define GL_ACCUM_CLEAR_VALUE 0x0B80
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_MATRIX_MODE 0x0BA0
#define GL_NORMALIZE 0x0BA1
#define GL_VIEWPORT 0x0BA2
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#define GL_MODELVIEW_MATRIX 0x0BA6
#define GL_PROJECTION_MATRIX 0x0BA7
#define GL_TEXTURE_MATRIX 0x0BA8
#define GL_ATTRIB_STACK_DEPTH 0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#define GL_ALPHA_TEST 0x0BC0
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_DITHER 0x0BD0
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND 0x0BE2
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_INDEX_LOGIC_OP 0x0BF1
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_AUX_BUFFERS 0x0C00
#define GL_DRAW_BUFFER 0x0C01
#define GL_READ_BUFFER 0x0C02
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_INDEX_CLEAR_VALUE 0x0C20
#define GL_INDEX_WRITEMASK 0x0C21
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_INDEX_MODE 0x0C30
#define GL_RGBA_MODE 0x0C31
#define GL_DOUBLEBUFFER 0x0C32
#define GL_STEREO 0x0C33
#define GL_RENDER_MODE 0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_POINT_SMOOTH_HINT 0x0C51
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#define GL_FOG_HINT 0x0C54
#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_Q 0x0C63
#define GL_PIXEL_MAP_I_TO_I 0x0C70
#define GL_PIXEL_MAP_S_TO_S 0x0C71
#define GL_PIXEL_MAP_I_TO_R 0x0C72
#define GL_PIXEL_MAP_I_TO_G 0x0C73
#define GL_PIXEL_MAP_I_TO_B 0x0C74
#define GL_PIXEL_MAP_I_TO_A 0x0C75
#define GL_PIXEL_MAP_R_TO_R 0x0C76
#define GL_PIXEL_MAP_G_TO_G 0x0C77
#define GL_PIXEL_MAP_B_TO_B 0x0C78
#define GL_PIXEL_MAP_A_TO_A 0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE 0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE 0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE 0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE 0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE 0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE 0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE 0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE 0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE 0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE 0x0CB9
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#define GL_UNPACK_LSB_FIRST 0x0CF1
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_SWAP_BYTES 0x0D00
#define GL_PACK_LSB_FIRST 0x0D01
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_MAP_COLOR 0x0D10
#define GL_MAP_STENCIL 0x0D11
#define GL_INDEX_SHIFT 0x0D12
#define GL_INDEX_OFFSET 0x0D13
#define GL_RED_SCALE 0x0D14
#define GL_RED_BIAS 0x0D15
#define GL_ZOOM_X 0x0D16
#define GL_ZOOM_Y 0x0D17
#define GL_GREEN_SCALE 0x0D18
#define GL_GREEN_BIAS 0x0D19
#define GL_BLUE_SCALE 0x0D1A
#define GL_BLUE_BIAS 0x0D1B
#define GL_ALPHA_SCALE 0x0D1C
#define GL_ALPHA_BIAS 0x0D1D
#define GL_DEPTH_SCALE 0x0D1E
#define GL_DEPTH_BIAS 0x0D1F
#define GL_MAX_EVAL_ORDER 0x0D30
#define GL_MAX_LIGHTS 0x0D31
#define GL_MAX_CLIP_PLANES 0x0D32
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_PIXEL_MAP_TABLE 0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define GL_MAX_NAME_STACK_DEPTH 0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_INDEX_BITS 0x0D51
#define GL_RED_BITS 0x0D52
#define GL_GREEN_BITS 0x0D53
#define GL_BLUE_BITS 0x0D54
#define GL_ALPHA_BITS 0x0D55
#define GL_DEPTH_BITS 0x0D56
#define GL_STENCIL_BITS 0x0D57
#define GL_ACCUM_RED_BITS 0x0D58
#define GL_ACCUM_GREEN_BITS 0x0D59
#define GL_ACCUM_BLUE_BITS 0x0D5A
#define GL_ACCUM_ALPHA_BITS 0x0D5B
#define GL_NAME_STACK_DEPTH 0x0D70
#define GL_AUTO_NORMAL 0x0D80
#define GL_MAP1_COLOR_4 0x0D90
#define GL_MAP1_INDEX 0x0D91
#define GL_MAP1_NORMAL 0x0D92
#define GL_MAP1_TEXTURE_COORD_1 0x0D93
#define GL_MAP1_TEXTURE_COORD_2 0x0D94
#define GL_MAP1_TEXTURE_COORD_3 0x0D95
#define GL_MAP1_TEXTURE_COORD_4 0x0D96
#define GL_MAP1_VERTEX_3 0x0D97
#define GL_MAP1_VERTEX_4 0x0D98
#define GL_MAP2_COLOR_4 0x0DB0
#define GL_MAP2_INDEX 0x0DB1
#define GL_MAP2_NORMAL 0x0DB2
#define GL_MAP2_TEXTURE_COORD_1 0x0DB3
#define GL_MAP2_TEXTURE_COORD_2 0x0DB4
#define GL_MAP2_TEXTURE_COORD_3 0x0DB5
#define GL_MAP2_TEXTURE_COORD_4 0x0DB6
#define GL_MAP2_VERTEX_3 0x0DB7
#define GL_MAP2_VERTEX_4 0x0DB8
#define GL_MAP1_GRID_DOMAIN 0x0DD0
#define GL_MAP1_GRID_SEGMENTS 0x0DD1
#define GL_MAP2_GRID_DOMAIN 0x0DD2
#define GL_MAP2_GRID_SEGMENTS 0x0DD3
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER 0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE 0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE 0x0DF2
#define GL_SELECTION_BUFFER_POINTER 0x0DF3
#define GL_SELECTION_BUFFER_SIZE 0x0DF4

#define GL_TEXTURE_WIDTH 0x1000
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_TEXTURE_BORDER 0x1005

#define GL_DONT_CARE 0x1100
#define GL_FASTEST 0x1101
#define GL_NICEST 0x1102

#define GL_LIGHT0 0x4000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#define GL_LIGHT3 0x4003
#define GL_LIGHT4 0x4004
#define GL_LIGHT5 0x4005
#define GL_LIGHT6 0x4006
#define GL_LIGHT7 0x4007

#define GL_AMBIENT 0x1200
#define GL_DIFFUSE 0x1201
#define GL_SPECULAR 0x1202
#define GL_POSITION 0x1203
#define GL_SPOT_DIRECTION 0x1204
#define GL_SPOT_EXPONENT 0x1205
#define GL_SPOT_CUTOFF 0x1206
#define GL_CONSTANT_ATTENUATION 0x1207
#define GL_LINEAR_ATTENUATION 0x1208
#define GL_QUADRATIC_ATTENUATION 0x1209

#define GL_COMPILE 0x1300
#define GL_COMPILE_AND_EXECUTE 0x1301

#define GL_CLEAR 0x1500
#define GL_AND 0x1501
#define GL_AND_REVERSE 0x1502
#define GL_COPY 0x1503
#define GL_AND_INVERTED 0x1504
#define GL_NOOP 0x1505
#define GL_XOR 0x1506
#define GL_OR 0x1507
#define GL_NOR 0x1508
#define GL_EQUIV 0x1509
#define GL_INVERT 0x150A
#define GL_OR_REVERSE 0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED 0x150D
#define GL_NAND 0x150E
#define GL_SET 0x150F

#define GL_EMISSION 0x1600
#define GL_SHININESS 0x1601
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#define GL_COLOR_INDEXES 0x1603

#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_TEXTURE 0x1702

#define GL_COLOR 0x1800
#define GL_DEPTH 0x1801
#define GL_STENCIL 0x1802

#define GL_COLOR_INDEX 0x1900
#define GL_STENCIL_INDEX 0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A

#define GL_BITMAP 0x1A00

#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02

#define GL_RENDER 0x1C00
#define GL_FEEDBACK 0x1C01
#define GL_SELECT 0x1C02

#define GL_FLAT 0x1D00
#define GL_SMOOTH 0x1D01

#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03

#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03

#define GL_S 0x2000
#define GL_T 0x2001
#define GL_R 0x2002
#define GL_Q 0x2003

#define GL_MODULATE 0x2100
#define GL_DECAL 0x2101

#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_TEXTURE_ENV_COLOR 0x2201

#define GL_TEXTURE_ENV 0x2300

#define GL_EYE_LINEAR 0x2400
#define GL_OBJECT_LINEAR 0x2401
#define GL_SPHERE_MAP 0x2402

#define GL_TEXTURE_GEN_MODE 0x2500
#define GL_OBJECT_PLANE 0x2501
#define GL_EYE_PLANE 0x2502

#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601

#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703

#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803

#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901

#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#define GL_CLIENT_ALL_ATTRIB_BITS 0xffffffff

#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_POLYGON_OFFSET_FILL 0x8037

#define GL_ALPHA4 0x803B
#define GL_ALPHA8 0x803C
#define GL_ALPHA12 0x803D
#define GL_ALPHA16 0x803E
#define GL_LUMINANCE4 0x803F
#define GL_LUMINANCE8 0x8040
#define GL_LUMINANCE12 0x8041
#define GL_LUMINANCE16 0x8042
#define GL_LUMINANCE4_ALPHA4 0x8043
#define GL_LUMINANCE6_ALPHA2 0x8044
#define GL_LUMINANCE8_ALPHA8 0x8045
#define GL_LUMINANCE12_ALPHA4 0x8046
#define GL_LUMINANCE12_ALPHA12 0x8047
#define GL_LUMINANCE16_ALPHA16 0x8048
#define GL_INTENSITY 0x8049
#define GL_INTENSITY4 0x804A
#define GL_INTENSITY8 0x804B
#define GL_INTENSITY12 0x804C
#define GL_INTENSITY16 0x804D
#define GL_R3_G3_B2 0x2A10
#define GL_RGB4 0x804F
#define GL_RGB5 0x8050
#define GL_RGB8 0x8051
#define GL_RGB10 0x8052
#define GL_RGB12 0x8053
#define GL_RGB16 0x8054
#define GL_RGBA2 0x8055
#define GL_RGBA4 0x8056
#define GL_RGB5_A1 0x8057
#define GL_RGBA8 0x8058
#define GL_RGB10_A2 0x8059
#define GL_RGBA12 0x805A
#define GL_RGBA16 0x805B
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_TEXTURE_LUMINANCE_SIZE 0x8060
#define GL_TEXTURE_INTENSITY_SIZE 0x8061
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064

#define GL_TEXTURE_PRIORITY 0x8066
#define GL_TEXTURE_RESIDENT 0x8067
#define GL_TEXTURE_BINDING_1D 0x8068
#define GL_TEXTURE_BINDING_2D 0x8069

#define GL_VERTEX_ARRAY 0x8074
#define GL_NORMAL_ARRAY 0x8075
#define GL_COLOR_ARRAY 0x8076
#define GL_INDEX_ARRAY 0x8077
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_EDGE_FLAG_ARRAY 0x8079
#define GL_VERTEX_ARRAY_SIZE 0x807A
#define GL_VERTEX_ARRAY_TYPE 0x807B
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#define GL_NORMAL_ARRAY_TYPE 0x807E
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#define GL_COLOR_ARRAY_SIZE 0x8081
#define GL_COLOR_ARRAY_TYPE 0x8082
#define GL_COLOR_ARRAY_STRIDE 0x8083
#define GL_INDEX_ARRAY_TYPE 0x8085
#define GL_INDEX_ARRAY_STRIDE 0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#define GL_VERTEX_ARRAY_POINTER 0x808E
#define GL_NORMAL_ARRAY_POINTER 0x808F
#define GL_COLOR_ARRAY_POINTER 0x8090
#define GL_INDEX_ARRAY_POINTER 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER 0x8093
#define GL_V2F 0x2A20
#define GL_V3F 0x2A21
#define GL_C4UB_V2F 0x2A22
#define GL_C4UB_V3F 0x2A23
#define GL_C3F_V3F 0x2A24
#define GL_N3F_V3F 0x2A25
#define GL_C4F_N3F_V3F 0x2A26
#define GL_T2F_V3F 0x2A27
#define GL_T4F_V4F 0x2A28
#define GL_T2F_C4UB_V3F 0x2A29
#define GL_T2F_C3F_V3F 0x2A2A
#define GL_T2F_N3F_V3F 0x2A2B
#define GL_T2F_C4F_N3F_V3F 0x2A2C
#define GL_T4F_C4F_N3F_V4F 0x2A2D

#define GL_EXT_vertex_array 1
#define GL_EXT_bgra 1
#define GL_EXT_paletted_texture 1
#define GL_WIN_swap_hint 1
#define GL_WIN_draw_range_elements 1

#define GL_VERTEX_ARRAY_EXT 0x8074
#define GL_NORMAL_ARRAY_EXT 0x8075
#define GL_COLOR_ARRAY_EXT 0x8076
#define GL_INDEX_ARRAY_EXT 0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT 0x8078
#define GL_EDGE_FLAG_ARRAY_EXT 0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT 0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT 0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT 0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT 0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT 0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT 0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT 0x8080
#define GL_COLOR_ARRAY_SIZE_EXT 0x8081
#define GL_COLOR_ARRAY_TYPE_EXT 0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT 0x8083
#define GL_COLOR_ARRAY_COUNT_EXT 0x8084
#define GL_INDEX_ARRAY_TYPE_EXT 0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT 0x8086
#define GL_INDEX_ARRAY_COUNT_EXT 0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT 0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT 0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT 0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT 0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT 0x808F
#define GL_COLOR_ARRAY_POINTER_EXT 0x8090
#define GL_INDEX_ARRAY_POINTER_EXT 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT 0x8093
#define GL_DOUBLE_EXT GL_DOUBLE

#define GL_BGR_EXT 0x80E0
#define GL_BGRA_EXT 0x80E1

#define GL_COLOR_TABLE_FORMAT_EXT 0x80D8
#define GL_COLOR_TABLE_WIDTH_EXT 0x80D9
#define GL_COLOR_TABLE_RED_SIZE_EXT 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_EXT 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_EXT 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_EXT 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_EXT 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_EXT 0x80DF

#define GL_COLOR_INDEX1_EXT 0x80E2
#define GL_COLOR_INDEX2_EXT 0x80E3
#define GL_COLOR_INDEX4_EXT 0x80E4
#define GL_COLOR_INDEX8_EXT 0x80E5
#define GL_COLOR_INDEX12_EXT 0x80E6
#define GL_COLOR_INDEX16_EXT 0x80E7

#define GL_MAX_ELEMENTS_VERTICES_WIN 0x80E8
#define GL_MAX_ELEMENTS_INDICES_WIN 0x80E9

#define GL_PHONG_WIN 0x80EA
#define GL_PHONG_HINT_WIN 0x80EB

#define GL_FOG_SPECULAR_TEXTURE_WIN 0x80EC

#define GL_LOGIC_OP GL_INDEX_LOGIC_OP
#define GL_TEXTURE_COMPONENTS GL_TEXTURE_INTERNAL_FORMAT
#endif /* !GL_VERSION_1_1 */
#endif /* !SIGL_NO_GL_1_1_H_MACROS */
#if !defined(SIGL_NO_GL_1_2_H_MACROS)
#ifndef GL_VERSION_1_2
#define GL_VERSION_1_2 1
#define GL_UNSIGNED_BYTE_3_3_2            0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_INT_8_8_8_8           0x8035
#define GL_UNSIGNED_INT_10_10_10_2        0x8036
#define GL_TEXTURE_BINDING_3D             0x806A
#define GL_PACK_SKIP_IMAGES               0x806B
#define GL_PACK_IMAGE_HEIGHT              0x806C
#define GL_UNPACK_SKIP_IMAGES             0x806D
#define GL_UNPACK_IMAGE_HEIGHT            0x806E
#define GL_TEXTURE_3D                     0x806F
#define GL_PROXY_TEXTURE_3D               0x8070
#define GL_TEXTURE_DEPTH                  0x8071
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY  0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY  0x0B23
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E
#define GL_RESCALE_NORMAL                 0x803A
#define GL_LIGHT_MODEL_COLOR_CONTROL      0x81F8
#define GL_SINGLE_COLOR                   0x81F9
#define GL_SEPARATE_SPECULAR_COLOR        0x81FA
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#endif /* !GL_VERSION_1_2 */
#endif /* !SIGL_NO_GL_1_2_H_MACROS */
#if !defined(SIGL_NO_GL_1_3_H_MACROS)
#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3 1
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_MULTISAMPLE                    0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE            0x809F
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP         0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_COMPRESSED_RGB                 0x84ED
#define GL_COMPRESSED_RGBA                0x84EE
#define GL_TEXTURE_COMPRESSION_HINT       0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE  0x86A0
#define GL_TEXTURE_COMPRESSED             0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_CLIENT_ACTIVE_TEXTURE          0x84E1
#define GL_MAX_TEXTURE_UNITS              0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX     0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX    0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX       0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX         0x84E6
#define GL_MULTISAMPLE_BIT                0x20000000
#define GL_NORMAL_MAP                     0x8511
#define GL_REFLECTION_MAP                 0x8512
#define GL_COMPRESSED_ALPHA               0x84E9
#define GL_COMPRESSED_LUMINANCE           0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA     0x84EB
#define GL_COMPRESSED_INTENSITY           0x84EC
#define GL_COMBINE                        0x8570
#define GL_COMBINE_RGB                    0x8571
#define GL_COMBINE_ALPHA                  0x8572
#define GL_SOURCE0_RGB                    0x8580
#define GL_SOURCE1_RGB                    0x8581
#define GL_SOURCE2_RGB                    0x8582
#define GL_SOURCE0_ALPHA                  0x8588
#define GL_SOURCE1_ALPHA                  0x8589
#define GL_SOURCE2_ALPHA                  0x858A
#define GL_OPERAND0_RGB                   0x8590
#define GL_OPERAND1_RGB                   0x8591
#define GL_OPERAND2_RGB                   0x8592
#define GL_OPERAND0_ALPHA                 0x8598
#define GL_OPERAND1_ALPHA                 0x8599
#define GL_OPERAND2_ALPHA                 0x859A
#define GL_RGB_SCALE                      0x8573
#define GL_ADD_SIGNED                     0x8574
#define GL_INTERPOLATE                    0x8575
#define GL_SUBTRACT                       0x84E7
#define GL_CONSTANT                       0x8576
#define GL_PRIMARY_COLOR                  0x8577
#define GL_PREVIOUS                       0x8578
#define GL_DOT3_RGB                       0x86AE
#define GL_DOT3_RGBA                      0x86AF
#endif
#endif /* !SIGL_NO_GL_1_3_H_MACROS */
#if !defined(SIGL_NO_GL_1_4_H_MACROS)
#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4 1
#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_POINT_FADE_THRESHOLD_SIZE      0x8128
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32              0x81A7
#define GL_MIRRORED_REPEAT                0x8370
#define GL_MAX_TEXTURE_LOD_BIAS           0x84FD
#define GL_TEXTURE_LOD_BIAS               0x8501
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_TEXTURE_DEPTH_SIZE             0x884A
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_POINT_SIZE_MIN                 0x8126
#define GL_POINT_SIZE_MAX                 0x8127
#define GL_POINT_DISTANCE_ATTENUATION     0x8129
#define GL_GENERATE_MIPMAP                0x8191
#define GL_GENERATE_MIPMAP_HINT           0x8192
#define GL_FOG_COORDINATE_SOURCE          0x8450
#define GL_FOG_COORDINATE                 0x8451
#define GL_FRAGMENT_DEPTH                 0x8452
#define GL_CURRENT_FOG_COORDINATE         0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE      0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE    0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER   0x8456
#define GL_FOG_COORDINATE_ARRAY           0x8457
#define GL_COLOR_SUM                      0x8458
#define GL_CURRENT_SECONDARY_COLOR        0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE     0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE     0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE   0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER  0x845D
#define GL_SECONDARY_COLOR_ARRAY          0x845E
#define GL_TEXTURE_FILTER_CONTROL         0x8500
#define GL_DEPTH_TEXTURE_MODE             0x884B
#define GL_COMPARE_R_TO_TEXTURE           0x884E
#define GL_BLEND_COLOR                    0x8005
#define GL_BLEND_EQUATION                 0x8009
#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_FUNC_ADD                       0x8006
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008
#endif
#endif /* !SIGL_NO_GL_1_4_H_MACROS */
#if !defined(SIGL_NO_GL_1_5_H_MACROS)
#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5 1
#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_QUERY_COUNTER_BITS             0x8864
#define GL_CURRENT_QUERY                  0x8865
#define GL_QUERY_RESULT                   0x8866
#define GL_QUERY_RESULT_AVAILABLE         0x8867
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY                      0x88B8
#define GL_WRITE_ONLY                     0x88B9
#define GL_READ_WRITE                     0x88BA
#define GL_BUFFER_ACCESS                  0x88BB
#define GL_BUFFER_MAPPED                  0x88BC
#define GL_BUFFER_MAP_POINTER             0x88BD
#define GL_STREAM_DRAW                    0x88E0
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#define GL_SAMPLES_PASSED                 0x8914
#define GL_SRC1_ALPHA                     0x8589
#define GL_VERTEX_ARRAY_BUFFER_BINDING    0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING    0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING     0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING     0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING    0x889E
#define GL_FOG_COORD_SRC                  0x8450
#define GL_FOG_COORD                      0x8451
#define GL_CURRENT_FOG_COORD              0x8453
#define GL_FOG_COORD_ARRAY_TYPE           0x8454
#define GL_FOG_COORD_ARRAY_STRIDE         0x8455
#define GL_FOG_COORD_ARRAY_POINTER        0x8456
#define GL_FOG_COORD_ARRAY                0x8457
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING 0x889D
#define GL_SRC0_RGB                       0x8580
#define GL_SRC1_RGB                       0x8581
#define GL_SRC2_RGB                       0x8582
#define GL_SRC0_ALPHA                     0x8588
#define GL_SRC2_ALPHA                     0x858A
#endif
#endif
#if !defined(SIGL_NO_GL_2_0_H_MACROS)
#ifndef GL_VERSION_2_0
#define GL_BLEND_EQUATION_RGB             0x8009
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_CURRENT_VERTEX_ATTRIB          0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_DRAW_BUFFER0                   0x8825
#define GL_DRAW_BUFFER1                   0x8826
#define GL_DRAW_BUFFER2                   0x8827
#define GL_DRAW_BUFFER3                   0x8828
#define GL_DRAW_BUFFER4                   0x8829
#define GL_DRAW_BUFFER5                   0x882A
#define GL_DRAW_BUFFER6                   0x882B
#define GL_DRAW_BUFFER7                   0x882C
#define GL_DRAW_BUFFER8                   0x882D
#define GL_DRAW_BUFFER9                   0x882E
#define GL_DRAW_BUFFER10                  0x882F
#define GL_DRAW_BUFFER11                  0x8830
#define GL_DRAW_BUFFER12                  0x8831
#define GL_DRAW_BUFFER13                  0x8832
#define GL_DRAW_BUFFER14                  0x8833
#define GL_DRAW_BUFFER15                  0x8834
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
#define GL_MAX_VARYING_FLOATS             0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE                    0x8B4F
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_1D                     0x8B5D
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_SAMPLER_1D_SHADOW              0x8B61
#define GL_SAMPLER_2D_SHADOW              0x8B62
#define GL_DELETE_STATUS                  0x8B80
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN      0x8CA0
#define GL_LOWER_LEFT                     0x8CA1
#define GL_UPPER_LEFT                     0x8CA2
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5
#define GL_VERTEX_PROGRAM_TWO_SIDE        0x8643
#define GL_POINT_SPRITE                   0x8861
#define GL_COORD_REPLACE                  0x8862
#define GL_MAX_TEXTURE_COORDS             0x8871
#endif /* GL_VERSION_2_0 */
#endif /* SIGL_NO_GL_2_0_H_MACROS */
#if !defined(SIGL_NO_GL_2_1_H_MACROS)
#ifndef GL_VERSION_2_1
#define GL_VERSION_2_1 1
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING      0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING    0x88EF
#define GL_FLOAT_MAT2x3                   0x8B65
#define GL_FLOAT_MAT2x4                   0x8B66
#define GL_FLOAT_MAT3x2                   0x8B67
#define GL_FLOAT_MAT3x4                   0x8B68
#define GL_FLOAT_MAT4x2                   0x8B69
#define GL_FLOAT_MAT4x3                   0x8B6A
#define GL_SRGB                           0x8C40
#define GL_SRGB8                          0x8C41
#define GL_SRGB_ALPHA                     0x8C42
#define GL_SRGB8_ALPHA8                   0x8C43
#define GL_COMPRESSED_SRGB                0x8C48
#define GL_COMPRESSED_SRGB_ALPHA          0x8C49
#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#define GL_SLUMINANCE_ALPHA               0x8C44
#define GL_SLUMINANCE8_ALPHA8             0x8C45
#define GL_SLUMINANCE                     0x8C46
#define GL_SLUMINANCE8                    0x8C47
#define GL_COMPRESSED_SLUMINANCE          0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA    0x8C4B
#endif /* GL_VERSION_2_1 */
#endif /* SIGL_NO_GL_2_1_H_MACROS */
#if !defined(SIGL_NO_GL_3_0_H_MACROS)
#ifndef GL_VERSION_3_0
#define GL_VERSION_3_0 1
#define GL_COMPARE_REF_TO_TEXTURE         0x884E
#define GL_CLIP_DISTANCE0                 0x3000
#define GL_CLIP_DISTANCE1                 0x3001
#define GL_CLIP_DISTANCE2                 0x3002
#define GL_CLIP_DISTANCE3                 0x3003
#define GL_CLIP_DISTANCE4                 0x3004
#define GL_CLIP_DISTANCE5                 0x3005
#define GL_CLIP_DISTANCE6                 0x3006
#define GL_CLIP_DISTANCE7                 0x3007
#define GL_MAX_CLIP_DISTANCES             0x0D32
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_CONTEXT_FLAGS                  0x821E
#define GL_COMPRESSED_RED                 0x8225
#define GL_COMPRESSED_RG                  0x8226
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x00000001
#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGBA16F                        0x881A
#define GL_RGB16F                         0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER    0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS       0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET       0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET       0x8905
#define GL_CLAMP_READ_COLOR               0x891C
#define GL_FIXED_ONLY                     0x891D
#define GL_MAX_VARYING_COMPONENTS         0x8B4B
#define GL_TEXTURE_1D_ARRAY               0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY         0x8C19
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY         0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY       0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY       0x8C1D
#define GL_R11F_G11F_B10F                 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV   0x8C3B
#define GL_RGB9_E5                        0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV       0x8C3E
#define GL_TEXTURE_SHARED_SIZE            0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS    0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED           0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD             0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS            0x8C8C
#define GL_SEPARATE_ATTRIBS               0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER      0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI                       0x8D70
#define GL_RGB32UI                        0x8D71
#define GL_RGBA16UI                       0x8D76
#define GL_RGB16UI                        0x8D77
#define GL_RGBA8UI                        0x8D7C
#define GL_RGB8UI                         0x8D7D
#define GL_RGBA32I                        0x8D82
#define GL_RGB32I                         0x8D83
#define GL_RGBA16I                        0x8D88
#define GL_RGB16I                         0x8D89
#define GL_RGBA8I                         0x8D8E
#define GL_RGB8I                          0x8D8F
#define GL_RED_INTEGER                    0x8D94
#define GL_GREEN_INTEGER                  0x8D95
#define GL_BLUE_INTEGER                   0x8D96
#define GL_RGB_INTEGER                    0x8D98
#define GL_RGBA_INTEGER                   0x8D99
#define GL_BGR_INTEGER                    0x8D9A
#define GL_BGRA_INTEGER                   0x8D9B
#define GL_SAMPLER_1D_ARRAY               0x8DC0
#define GL_SAMPLER_2D_ARRAY               0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW        0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW        0x8DC4
#define GL_SAMPLER_CUBE_SHADOW            0x8DC5
#define GL_UNSIGNED_INT_VEC2              0x8DC6
#define GL_UNSIGNED_INT_VEC3              0x8DC7
#define GL_UNSIGNED_INT_VEC4              0x8DC8
#define GL_INT_SAMPLER_1D                 0x8DC9
#define GL_INT_SAMPLER_2D                 0x8DCA
#define GL_INT_SAMPLER_3D                 0x8DCB
#define GL_INT_SAMPLER_CUBE               0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY           0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY           0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D        0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D        0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D        0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE      0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY  0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY  0x8DD7
#define GL_QUERY_WAIT                     0x8E13
#define GL_QUERY_NO_WAIT                  0x8E14
#define GL_QUERY_BY_REGION_WAIT           0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT        0x8E16
#define GL_BUFFER_ACCESS_FLAGS            0x911F
#define GL_BUFFER_MAP_LENGTH              0x9120
#define GL_BUFFER_MAP_OFFSET              0x9121
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_DEPTH32F_STENCIL8              0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT            0x8218
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_TEXTURE_STENCIL_SIZE           0x88F1
#define GL_TEXTURE_RED_TYPE               0x8C10
#define GL_TEXTURE_GREEN_TYPE             0x8C11
#define GL_TEXTURE_BLUE_TYPE              0x8C12
#define GL_TEXTURE_ALPHA_TYPE             0x8C13
#define GL_TEXTURE_DEPTH_TYPE             0x8C16
#define GL_UNSIGNED_NORMALIZED            0x8C17
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING       0x8CA6
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       0x8CAA
#define GL_RENDERBUFFER_SAMPLES           0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_COLOR_ATTACHMENT16             0x8CF0
#define GL_COLOR_ATTACHMENT17             0x8CF1
#define GL_COLOR_ATTACHMENT18             0x8CF2
#define GL_COLOR_ATTACHMENT19             0x8CF3
#define GL_COLOR_ATTACHMENT20             0x8CF4
#define GL_COLOR_ATTACHMENT21             0x8CF5
#define GL_COLOR_ATTACHMENT22             0x8CF6
#define GL_COLOR_ATTACHMENT23             0x8CF7
#define GL_COLOR_ATTACHMENT24             0x8CF8
#define GL_COLOR_ATTACHMENT25             0x8CF9
#define GL_COLOR_ATTACHMENT26             0x8CFA
#define GL_COLOR_ATTACHMENT27             0x8CFB
#define GL_COLOR_ATTACHMENT28             0x8CFC
#define GL_COLOR_ATTACHMENT29             0x8CFD
#define GL_COLOR_ATTACHMENT30             0x8CFE
#define GL_COLOR_ATTACHMENT31             0x8CFF
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_STENCIL_INDEX1                 0x8D46
#define GL_STENCIL_INDEX4                 0x8D47
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_STENCIL_INDEX16                0x8D49
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES                    0x8D57
#define GL_INDEX                          0x8222
#define GL_TEXTURE_LUMINANCE_TYPE         0x8C14
#define GL_TEXTURE_INTENSITY_TYPE         0x8C15
#define GL_FRAMEBUFFER_SRGB               0x8DB9
#define GL_HALF_FLOAT                     0x140B
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT       0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT      0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT         0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT         0x0020
#define GL_COMPRESSED_RED_RGTC1           0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1    0x8DBC
#define GL_COMPRESSED_RG_RGTC2            0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2     0x8DBE
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_R16                            0x822A
#define GL_RG8                            0x822B
#define GL_RG16                           0x822C
#define GL_R16F                           0x822D
#define GL_R32F                           0x822E
#define GL_RG16F                          0x822F
#define GL_RG32F                          0x8230
#define GL_R8I                            0x8231
#define GL_R8UI                           0x8232
#define GL_R16I                           0x8233
#define GL_R16UI                          0x8234
#define GL_R32I                           0x8235
#define GL_R32UI                          0x8236
#define GL_RG8I                           0x8237
#define GL_RG8UI                          0x8238
#define GL_RG16I                          0x8239
#define GL_RG16UI                         0x823A
#define GL_RG32I                          0x823B
#define GL_RG32UI                         0x823C
#define GL_VERTEX_ARRAY_BINDING           0x85B5
#define GL_CLAMP_VERTEX_COLOR             0x891A
#define GL_CLAMP_FRAGMENT_COLOR           0x891B
#define GL_ALPHA_INTEGER                  0x8D97
#endif /* GL_VERSION_3_0 */
#endif /* SIGL_NO_GL_3_0_H_MACROS */
#if !defined(SIGL_NO_GL_3_1_H_MACROS)
#ifndef GL_VERSION_3_1
#define GL_VERSION_3_1 1
#define GL_SAMPLER_2D_RECT                0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW         0x8B64
#define GL_SAMPLER_BUFFER                 0x8DC2
#define GL_INT_SAMPLER_2D_RECT            0x8DCD
#define GL_INT_SAMPLER_BUFFER             0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT   0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER    0x8DD8
#define GL_TEXTURE_BUFFER                 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE        0x8C2B
#define GL_TEXTURE_BINDING_BUFFER         0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_RECTANGLE              0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE      0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE        0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE     0x84F8
#define GL_R8_SNORM                       0x8F94
#define GL_RG8_SNORM                      0x8F95
#define GL_RGB8_SNORM                     0x8F96
#define GL_RGBA8_SNORM                    0x8F97
#define GL_R16_SNORM                      0x8F98
#define GL_RG16_SNORM                     0x8F99
#define GL_RGB16_SNORM                    0x8F9A
#define GL_RGBA16_SNORM                   0x8F9B
#define GL_SIGNED_NORMALIZED              0x8F9C
#define GL_PRIMITIVE_RESTART              0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX        0x8F9E
#define GL_COPY_READ_BUFFER               0x8F36
#define GL_COPY_WRITE_BUFFER              0x8F37
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_UNIFORM_BUFFER_BINDING         0x8A28
#define GL_UNIFORM_BUFFER_START           0x8A29
#define GL_UNIFORM_BUFFER_SIZE            0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS      0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS    0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS    0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS    0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS    0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE         0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS          0x8A36
#define GL_UNIFORM_TYPE                   0x8A37
#define GL_UNIFORM_SIZE                   0x8A38
#define GL_UNIFORM_NAME_LENGTH            0x8A39
#define GL_UNIFORM_BLOCK_INDEX            0x8A3A
#define GL_UNIFORM_OFFSET                 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE           0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE          0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR           0x8A3E
#define GL_UNIFORM_BLOCK_BINDING          0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE        0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH      0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS  0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX                  0xFFFFFFFFu
#endif /* GL_VERSION_3_1 */
#endif /* SIGL_NO_GL_3_1_H_MACROS */
#if !defined(SIGL_NO_GL_3_2_H_MACROS)
#ifndef GL_VERSION_3_2
#define GL_VERSION_3_2 1
#define GL_CONTEXT_CORE_PROFILE_BIT       0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY                0x000A
#define GL_LINE_STRIP_ADJACENCY           0x000B
#define GL_TRIANGLES_ADJACENCY            0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY       0x000D
#define GL_PROGRAM_POINT_SIZE             0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_GEOMETRY_VERTICES_OUT          0x8916
#define GL_GEOMETRY_INPUT_TYPE            0x8917
#define GL_GEOMETRY_OUTPUT_TYPE           0x8918
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES   0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS   0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS  0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS  0x9125
#define GL_CONTEXT_PROFILE_MASK           0x9126
#define GL_DEPTH_CLAMP                    0x864F
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION        0x8E4D
#define GL_LAST_VERTEX_CONVENTION         0x8E4E
#define GL_PROVOKING_VERTEX               0x8E4F
#define GL_TEXTURE_CUBE_MAP_SEAMLESS      0x884F
#define GL_MAX_SERVER_WAIT_TIMEOUT        0x9111
#define GL_OBJECT_TYPE                    0x9112
#define GL_SYNC_CONDITION                 0x9113
#define GL_SYNC_STATUS                    0x9114
#define GL_SYNC_FLAGS                     0x9115
#define GL_SYNC_FENCE                     0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE     0x9117
#define GL_UNSIGNALED                     0x9118
#define GL_SIGNALED                       0x9119
#define GL_ALREADY_SIGNALED               0x911A
#define GL_TIMEOUT_EXPIRED                0x911B
#define GL_CONDITION_SATISFIED            0x911C
#define GL_WAIT_FAILED                    0x911D
#define GL_TIMEOUT_IGNORED                0xFFFFFFFFFFFFFFFFull
#define GL_SYNC_FLUSH_COMMANDS_BIT        0x00000001
#define GL_SAMPLE_POSITION                0x8E50
#define GL_SAMPLE_MASK                    0x8E51
#define GL_SAMPLE_MASK_VALUE              0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS          0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE         0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE   0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY   0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_TEXTURE_SAMPLES                0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_SAMPLER_2D_MULTISAMPLE         0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE     0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY   0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES      0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES      0x910F
#define GL_MAX_INTEGER_SAMPLES            0x9110
#endif /* GL_VERSION_3_2 */
#endif /* SIGL_NO_GL_3_2_H_MACROS */
#if !defined(SIGL_NO_GL_3_3_H_MACROS)
#ifndef GL_VERSION_3_3
#define GL_VERSION_3_3 1
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR    0x88FE
#define GL_SRC1_COLOR                     0x88F9
#define GL_ONE_MINUS_SRC1_COLOR           0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA           0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS   0x88FC
#define GL_ANY_SAMPLES_PASSED             0x8C2F
#define GL_SAMPLER_BINDING                0x8919
#define GL_RGB10_A2UI                     0x906F
#define GL_TEXTURE_SWIZZLE_R              0x8E42
#define GL_TEXTURE_SWIZZLE_G              0x8E43
#define GL_TEXTURE_SWIZZLE_B              0x8E44
#define GL_TEXTURE_SWIZZLE_A              0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA           0x8E46
#define GL_TIME_ELAPSED                   0x88BF
#define GL_TIMESTAMP                      0x8E28
#define GL_INT_2_10_10_10_REV             0x8D9F
#endif /* GL_VERSION_3_3 */
#endif /* SIGL_NO_GL_3_3_H_MACROS */
#if !defined(SIGL_NO_GL_4_0_H_MACROS)
#ifndef GL_VERSION_4_0
#define GL_VERSION_4_0 1
#define GL_SAMPLE_SHADING                 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE       0x8C37
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_TEXTURE_CUBE_MAP_ARRAY         0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY   0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY         0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW  0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY     0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F
#define GL_DRAW_INDIRECT_BUFFER           0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING   0x8F43
#define GL_GEOMETRY_SHADER_INVOCATIONS    0x887F
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_MAX_VERTEX_STREAMS             0x8E71
#define GL_DOUBLE_VEC2                    0x8FFC
#define GL_DOUBLE_VEC3                    0x8FFD
#define GL_DOUBLE_VEC4                    0x8FFE
#define GL_DOUBLE_MAT2                    0x8F46
#define GL_DOUBLE_MAT3                    0x8F47
#define GL_DOUBLE_MAT4                    0x8F48
#define GL_DOUBLE_MAT2x3                  0x8F49
#define GL_DOUBLE_MAT2x4                  0x8F4A
#define GL_DOUBLE_MAT3x2                  0x8F4B
#define GL_DOUBLE_MAT3x4                  0x8F4C
#define GL_DOUBLE_MAT4x2                  0x8F4D
#define GL_DOUBLE_MAT4x3                  0x8F4E
#define GL_ACTIVE_SUBROUTINES             0x8DE5
#define GL_ACTIVE_SUBROUTINE_UNIFORMS     0x8DE6
#define GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS 0x8E47
#define GL_ACTIVE_SUBROUTINE_MAX_LENGTH   0x8E48
#define GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH 0x8E49
#define GL_MAX_SUBROUTINES                0x8DE7
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS 0x8DE8
#define GL_NUM_COMPATIBLE_SUBROUTINES     0x8E4A
#define GL_COMPATIBLE_SUBROUTINES         0x8E4B
#define GL_PATCHES                        0x000E
#define GL_PATCH_VERTICES                 0x8E72
#define GL_PATCH_DEFAULT_INNER_LEVEL      0x8E73
#define GL_PATCH_DEFAULT_OUTER_LEVEL      0x8E74
#define GL_TESS_CONTROL_OUTPUT_VERTICES   0x8E75
#define GL_TESS_GEN_MODE                  0x8E76
#define GL_TESS_GEN_SPACING               0x8E77
#define GL_TESS_GEN_VERTEX_ORDER          0x8E78
#define GL_TESS_GEN_POINT_MODE            0x8E79
#define GL_ISOLINES                       0x8E7A
#define GL_FRACTIONAL_ODD                 0x8E7B
#define GL_FRACTIONAL_EVEN                0x8E7C
#define GL_MAX_PATCH_VERTICES             0x8E7D
#define GL_MAX_TESS_GEN_LEVEL             0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS      0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS 0x8E86
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS 0x8E8A
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E1F
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER 0x84F0
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER 0x84F1
#define GL_TESS_EVALUATION_SHADER         0x8E87
#define GL_TESS_CONTROL_SHADER            0x8E88
#define GL_TRANSFORM_FEEDBACK             0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING     0x8E25
#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS 0x8E70
#endif /* GL_VERSION_4_0 */
#endif /* SIGL_NO_GL_4_0_H_MACROS */
#if !defined(SIGL_NO_GL_4_1_H_MACROS)
#ifndef GL_VERSION_4_1
#define GL_VERSION_4_1 1
#define GL_FIXED                          0x140C
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_LOW_FLOAT                      0x8DF0
#define GL_MEDIUM_FLOAT                   0x8DF1
#define GL_HIGH_FLOAT                     0x8DF2
#define GL_LOW_INT                        0x8DF3
#define GL_MEDIUM_INT                     0x8DF4
#define GL_HIGH_INT                       0x8DF5
#define GL_SHADER_COMPILER                0x8DFA
#define GL_SHADER_BINARY_FORMATS          0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS      0x8DF9
#define GL_MAX_VERTEX_UNIFORM_VECTORS     0x8DFB
#define GL_MAX_VARYING_VECTORS            0x8DFC
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS   0x8DFD
#define GL_RGB565                         0x8D62
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH          0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS     0x87FE
#define GL_PROGRAM_BINARY_FORMATS         0x87FF
#define GL_VERTEX_SHADER_BIT              0x00000001
#define GL_FRAGMENT_SHADER_BIT            0x00000002
#define GL_GEOMETRY_SHADER_BIT            0x00000004
#define GL_TESS_CONTROL_SHADER_BIT        0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT     0x00000010
#define GL_ALL_SHADER_BITS                0xFFFFFFFF
#define GL_PROGRAM_SEPARABLE              0x8258
#define GL_ACTIVE_PROGRAM                 0x8259
#define GL_PROGRAM_PIPELINE_BINDING       0x825A
#define GL_MAX_VIEWPORTS                  0x825B
#define GL_VIEWPORT_SUBPIXEL_BITS         0x825C
#define GL_VIEWPORT_BOUNDS_RANGE          0x825D
#define GL_LAYER_PROVOKING_VERTEX         0x825E
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX 0x825F
#define GL_UNDEFINED_VERTEX               0x8260
#endif /* GL_VERSION_4_1 */
#endif /* SIGL_NO_GL_4_1_H_MACROS */
#if !defined(SIGL_NO_GL_4_2_H_MACROS)
#ifndef GL_VERSION_4_2
#define GL_VERSION_4_2 1
#define GL_COPY_READ_BUFFER_BINDING       0x8F36
#define GL_COPY_WRITE_BUFFER_BINDING      0x8F37
#define GL_TRANSFORM_FEEDBACK_ACTIVE      0x8E24
#define GL_TRANSFORM_FEEDBACK_PAUSED      0x8E23
#define GL_UNPACK_COMPRESSED_BLOCK_WIDTH  0x9127
#define GL_UNPACK_COMPRESSED_BLOCK_HEIGHT 0x9128
#define GL_UNPACK_COMPRESSED_BLOCK_DEPTH  0x9129
#define GL_UNPACK_COMPRESSED_BLOCK_SIZE   0x912A
#define GL_PACK_COMPRESSED_BLOCK_WIDTH    0x912B
#define GL_PACK_COMPRESSED_BLOCK_HEIGHT   0x912C
#define GL_PACK_COMPRESSED_BLOCK_DEPTH    0x912D
#define GL_PACK_COMPRESSED_BLOCK_SIZE     0x912E
#define GL_NUM_SAMPLE_COUNTS              0x9380
#define GL_MIN_MAP_BUFFER_ALIGNMENT       0x90BC
#define GL_ATOMIC_COUNTER_BUFFER          0x92C0
#define GL_ATOMIC_COUNTER_BUFFER_BINDING  0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START    0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE     0x92C3
#define GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE 0x92C4
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS 0x92C5
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES 0x92C6
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER 0x92C7
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER 0x92C8
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER 0x92C9
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER 0x92CA
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER 0x92CB
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS 0x92CE
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS 0x92CF
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS     0x92D2
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS   0x92D5
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS   0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS   0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS  0x92D9
#define GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX 0x92DA
#define GL_UNSIGNED_INT_ATOMIC_COUNTER    0x92DB
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT      0x00000002
#define GL_UNIFORM_BARRIER_BIT            0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT      0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_COMMAND_BARRIER_BIT            0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT       0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT     0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT      0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT        0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT     0x00001000
#define GL_ALL_BARRIER_BITS               0xFFFFFFFF
#define GL_MAX_IMAGE_UNITS                0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS 0x8F39
#define GL_IMAGE_BINDING_NAME             0x8F3A
#define GL_IMAGE_BINDING_LEVEL            0x8F3B
#define GL_IMAGE_BINDING_LAYERED          0x8F3C
#define GL_IMAGE_BINDING_LAYER            0x8F3D
#define GL_IMAGE_BINDING_ACCESS           0x8F3E
#define GL_IMAGE_1D                       0x904C
#define GL_IMAGE_2D                       0x904D
#define GL_IMAGE_3D                       0x904E
#define GL_IMAGE_2D_RECT                  0x904F
#define GL_IMAGE_CUBE                     0x9050
#define GL_IMAGE_BUFFER                   0x9051
#define GL_IMAGE_1D_ARRAY                 0x9052
#define GL_IMAGE_2D_ARRAY                 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY           0x9054
#define GL_IMAGE_2D_MULTISAMPLE           0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY     0x9056
#define GL_INT_IMAGE_1D                   0x9057
#define GL_INT_IMAGE_2D                   0x9058
#define GL_INT_IMAGE_3D                   0x9059
#define GL_INT_IMAGE_2D_RECT              0x905A
#define GL_INT_IMAGE_CUBE                 0x905B
#define GL_INT_IMAGE_BUFFER               0x905C
#define GL_INT_IMAGE_1D_ARRAY             0x905D
#define GL_INT_IMAGE_2D_ARRAY             0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY       0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE       0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D          0x9062
#define GL_UNSIGNED_INT_IMAGE_2D          0x9063
#define GL_UNSIGNED_INT_IMAGE_3D          0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT     0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE        0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER      0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY    0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY    0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x906C
#define GL_MAX_IMAGE_SAMPLES              0x906D
#define GL_IMAGE_BINDING_FORMAT           0x906E
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_MAX_VERTEX_IMAGE_UNIFORMS      0x90CA
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS 0x90CC
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS    0x90CD
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS    0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS    0x90CF
#define GL_COMPRESSED_RGBA_BPTC_UNORM     0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT 0x8E8F
#define GL_TEXTURE_IMMUTABLE_FORMAT       0x912F
#endif /* GL_VERSION_4_2 */
#endif /* SIGL_NO_GL_4_2_H_MACROS */
#if !defined(SIGL_NO_GL_4_3_H_MACROS)
#ifndef GL_VERSION_4_3
#define GL_VERSION_4_3 1
typedef void (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
#define GL_NUM_SHADING_LANGUAGE_VERSIONS  0x82E9
#define GL_VERTEX_ATTRIB_ARRAY_LONG       0x874E
#define GL_COMPRESSED_RGB8_ETC2           0x9274
#define GL_COMPRESSED_SRGB8_ETC2          0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC      0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279
#define GL_COMPRESSED_R11_EAC             0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC      0x9271
#define GL_COMPRESSED_RG11_EAC            0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC     0x9273
#define GL_PRIMITIVE_RESTART_FIXED_INDEX  0x8D69
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_MAX_ELEMENT_INDEX              0x8D6B
#define GL_COMPUTE_SHADER                 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS     0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS     0x91BD
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS    0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT   0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE    0x91BF
#define GL_COMPUTE_WORK_GROUP_SIZE        0x8267
#define GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER 0x90EC
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER 0x90ED
#define GL_DISPATCH_INDIRECT_BUFFER       0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_COMPUTE_SHADER_BIT             0x00000020
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION        0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM      0x8245
#define GL_DEBUG_SOURCE_API               0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM     0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER   0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY       0x8249
#define GL_DEBUG_SOURCE_APPLICATION       0x824A
#define GL_DEBUG_SOURCE_OTHER             0x824B
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_OTHER               0x8251
#define GL_MAX_DEBUG_MESSAGE_LENGTH       0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES      0x9144
#define GL_DEBUG_LOGGED_MESSAGES          0x9145
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_TYPE_MARKER              0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP          0x8269
#define GL_DEBUG_TYPE_POP_GROUP           0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH    0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH        0x826D
#define GL_BUFFER                         0x82E0
#define GL_SHADER                         0x82E1
#define GL_PROGRAM                        0x82E2
#define GL_QUERY                          0x82E3
#define GL_PROGRAM_PIPELINE               0x82E4
#define GL_SAMPLER                        0x82E6
#define GL_MAX_LABEL_LENGTH               0x82E8
#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_CONTEXT_FLAG_DEBUG_BIT         0x00000002
#define GL_MAX_UNIFORM_LOCATIONS          0x826E
#define GL_FRAMEBUFFER_DEFAULT_WIDTH      0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT     0x9311
#define GL_FRAMEBUFFER_DEFAULT_LAYERS     0x9312
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES    0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH          0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT         0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS         0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES        0x9318
#define GL_INTERNALFORMAT_SUPPORTED       0x826F
#define GL_INTERNALFORMAT_PREFERRED       0x8270
#define GL_INTERNALFORMAT_RED_SIZE        0x8271
#define GL_INTERNALFORMAT_GREEN_SIZE      0x8272
#define GL_INTERNALFORMAT_BLUE_SIZE       0x8273
#define GL_INTERNALFORMAT_ALPHA_SIZE      0x8274
#define GL_INTERNALFORMAT_DEPTH_SIZE      0x8275
#define GL_INTERNALFORMAT_STENCIL_SIZE    0x8276
#define GL_INTERNALFORMAT_SHARED_SIZE     0x8277
#define GL_INTERNALFORMAT_RED_TYPE        0x8278
#define GL_INTERNALFORMAT_GREEN_TYPE      0x8279
#define GL_INTERNALFORMAT_BLUE_TYPE       0x827A
#define GL_INTERNALFORMAT_ALPHA_TYPE      0x827B
#define GL_INTERNALFORMAT_DEPTH_TYPE      0x827C
#define GL_INTERNALFORMAT_STENCIL_TYPE    0x827D
#define GL_MAX_WIDTH                      0x827E
#define GL_MAX_HEIGHT                     0x827F
#define GL_MAX_DEPTH                      0x8280
#define GL_MAX_LAYERS                     0x8281
#define GL_MAX_COMBINED_DIMENSIONS        0x8282
#define GL_COLOR_COMPONENTS               0x8283
#define GL_DEPTH_COMPONENTS               0x8284
#define GL_STENCIL_COMPONENTS             0x8285
#define GL_COLOR_RENDERABLE               0x8286
#define GL_DEPTH_RENDERABLE               0x8287
#define GL_STENCIL_RENDERABLE             0x8288
#define GL_FRAMEBUFFER_RENDERABLE         0x8289
#define GL_FRAMEBUFFER_RENDERABLE_LAYERED 0x828A
#define GL_FRAMEBUFFER_BLEND              0x828B
#define GL_READ_PIXELS                    0x828C
#define GL_READ_PIXELS_FORMAT             0x828D
#define GL_READ_PIXELS_TYPE               0x828E
#define GL_TEXTURE_IMAGE_FORMAT           0x828F
#define GL_TEXTURE_IMAGE_TYPE             0x8290
#define GL_GET_TEXTURE_IMAGE_FORMAT       0x8291
#define GL_GET_TEXTURE_IMAGE_TYPE         0x8292
#define GL_MIPMAP                         0x8293
#define GL_MANUAL_GENERATE_MIPMAP         0x8294
#define GL_AUTO_GENERATE_MIPMAP           0x8295
#define GL_COLOR_ENCODING                 0x8296
#define GL_SRGB_READ                      0x8297
#define GL_SRGB_WRITE                     0x8298
#define GL_FILTER                         0x829A
#define GL_VERTEX_TEXTURE                 0x829B
#define GL_TESS_CONTROL_TEXTURE           0x829C
#define GL_TESS_EVALUATION_TEXTURE        0x829D
#define GL_GEOMETRY_TEXTURE               0x829E
#define GL_FRAGMENT_TEXTURE               0x829F
#define GL_COMPUTE_TEXTURE                0x82A0
#define GL_TEXTURE_SHADOW                 0x82A1
#define GL_TEXTURE_GATHER                 0x82A2
#define GL_TEXTURE_GATHER_SHADOW          0x82A3
#define GL_SHADER_IMAGE_LOAD              0x82A4
#define GL_SHADER_IMAGE_STORE             0x82A5
#define GL_SHADER_IMAGE_ATOMIC            0x82A6
#define GL_IMAGE_TEXEL_SIZE               0x82A7
#define GL_IMAGE_COMPATIBILITY_CLASS      0x82A8
#define GL_IMAGE_PIXEL_FORMAT             0x82A9
#define GL_IMAGE_PIXEL_TYPE               0x82AA
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST 0x82AC
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST 0x82AD
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE 0x82AE
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE 0x82AF
#define GL_TEXTURE_COMPRESSED_BLOCK_WIDTH 0x82B1
#define GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT 0x82B2
#define GL_TEXTURE_COMPRESSED_BLOCK_SIZE  0x82B3
#define GL_CLEAR_BUFFER                   0x82B4
#define GL_TEXTURE_VIEW                   0x82B5
#define GL_VIEW_COMPATIBILITY_CLASS       0x82B6
#define GL_FULL_SUPPORT                   0x82B7
#define GL_CAVEAT_SUPPORT                 0x82B8
#define GL_IMAGE_CLASS_4_X_32             0x82B9
#define GL_IMAGE_CLASS_2_X_32             0x82BA
#define GL_IMAGE_CLASS_1_X_32             0x82BB
#define GL_IMAGE_CLASS_4_X_16             0x82BC
#define GL_IMAGE_CLASS_2_X_16             0x82BD
#define GL_IMAGE_CLASS_1_X_16             0x82BE
#define GL_IMAGE_CLASS_4_X_8              0x82BF
#define GL_IMAGE_CLASS_2_X_8              0x82C0
#define GL_IMAGE_CLASS_1_X_8              0x82C1
#define GL_IMAGE_CLASS_11_11_10           0x82C2
#define GL_IMAGE_CLASS_10_10_10_2         0x82C3
#define GL_VIEW_CLASS_128_BITS            0x82C4
#define GL_VIEW_CLASS_96_BITS             0x82C5
#define GL_VIEW_CLASS_64_BITS             0x82C6
#define GL_VIEW_CLASS_48_BITS             0x82C7
#define GL_VIEW_CLASS_32_BITS             0x82C8
#define GL_VIEW_CLASS_24_BITS             0x82C9
#define GL_VIEW_CLASS_16_BITS             0x82CA
#define GL_VIEW_CLASS_8_BITS              0x82CB
#define GL_VIEW_CLASS_S3TC_DXT1_RGB       0x82CC
#define GL_VIEW_CLASS_S3TC_DXT1_RGBA      0x82CD
#define GL_VIEW_CLASS_S3TC_DXT3_RGBA      0x82CE
#define GL_VIEW_CLASS_S3TC_DXT5_RGBA      0x82CF
#define GL_VIEW_CLASS_RGTC1_RED           0x82D0
#define GL_VIEW_CLASS_RGTC2_RG            0x82D1
#define GL_VIEW_CLASS_BPTC_UNORM          0x82D2
#define GL_VIEW_CLASS_BPTC_FLOAT          0x82D3
#define GL_UNIFORM                        0x92E1
#define GL_UNIFORM_BLOCK                  0x92E2
#define GL_PROGRAM_INPUT                  0x92E3
#define GL_PROGRAM_OUTPUT                 0x92E4
#define GL_BUFFER_VARIABLE                0x92E5
#define GL_SHADER_STORAGE_BLOCK           0x92E6
#define GL_VERTEX_SUBROUTINE              0x92E8
#define GL_TESS_CONTROL_SUBROUTINE        0x92E9
#define GL_TESS_EVALUATION_SUBROUTINE     0x92EA
#define GL_GEOMETRY_SUBROUTINE            0x92EB
#define GL_FRAGMENT_SUBROUTINE            0x92EC
#define GL_COMPUTE_SUBROUTINE             0x92ED
#define GL_VERTEX_SUBROUTINE_UNIFORM      0x92EE
#define GL_TESS_CONTROL_SUBROUTINE_UNIFORM 0x92EF
#define GL_TESS_EVALUATION_SUBROUTINE_UNIFORM 0x92F0
#define GL_GEOMETRY_SUBROUTINE_UNIFORM    0x92F1
#define GL_FRAGMENT_SUBROUTINE_UNIFORM    0x92F2
#define GL_COMPUTE_SUBROUTINE_UNIFORM     0x92F3
#define GL_TRANSFORM_FEEDBACK_VARYING     0x92F4
#define GL_ACTIVE_RESOURCES               0x92F5
#define GL_MAX_NAME_LENGTH                0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES       0x92F7
#define GL_MAX_NUM_COMPATIBLE_SUBROUTINES 0x92F8
#define GL_NAME_LENGTH                    0x92F9
#define GL_TYPE                           0x92FA
#define GL_ARRAY_SIZE                     0x92FB
#define GL_OFFSET                         0x92FC
#define GL_BLOCK_INDEX                    0x92FD
#define GL_ARRAY_STRIDE                   0x92FE
#define GL_MATRIX_STRIDE                  0x92FF
#define GL_IS_ROW_MAJOR                   0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX    0x9301
#define GL_BUFFER_BINDING                 0x9302
#define GL_BUFFER_DATA_SIZE               0x9303
#define GL_NUM_ACTIVE_VARIABLES           0x9304
#define GL_ACTIVE_VARIABLES               0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER    0x9306
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER 0x9308
#define GL_REFERENCED_BY_GEOMETRY_SHADER  0x9309
#define GL_REFERENCED_BY_FRAGMENT_SHADER  0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER   0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE           0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE         0x930D
#define GL_LOCATION                       0x930E
#define GL_LOCATION_INDEX                 0x930F
#define GL_IS_PER_PATCH                   0x92E7
#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING  0x90D3
#define GL_SHADER_STORAGE_BUFFER_START    0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE     0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE  0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF
#define GL_SHADER_STORAGE_BARRIER_BIT     0x00002000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_DEPTH_STENCIL_TEXTURE_MODE     0x90EA
#define GL_TEXTURE_BUFFER_OFFSET          0x919D
#define GL_TEXTURE_BUFFER_SIZE            0x919E
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT 0x919F
#define GL_TEXTURE_VIEW_MIN_LEVEL         0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS        0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER         0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS        0x82DE
#define GL_TEXTURE_IMMUTABLE_LEVELS       0x82DF
#define GL_VERTEX_ATTRIB_BINDING          0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET  0x82D5
#define GL_VERTEX_BINDING_DIVISOR         0x82D6
#define GL_VERTEX_BINDING_OFFSET          0x82D7
#define GL_VERTEX_BINDING_STRIDE          0x82D8
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS     0x82DA
#define GL_VERTEX_BINDING_BUFFER          0x8F4F
#define GL_DISPLAY_LIST                   0x82E7
#endif /* GL_VERSION_4_3 */
#endif /* SIGL_NO_GL_4_3_H_MACROS */
#if !defined(SIGL_NO_GL_4_4_H_MACROS)
#ifndef GL_VERSION_4_4
#define GL_VERSION_4_4 1
#define GL_MAX_VERTEX_ATTRIB_STRIDE       0x82E5
#define GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED 0x8221
#define GL_TEXTURE_BUFFER_BINDING         0x8C2A
#define GL_MAP_PERSISTENT_BIT             0x0040
#define GL_MAP_COHERENT_BIT               0x0080
#define GL_DYNAMIC_STORAGE_BIT            0x0100
#define GL_CLIENT_STORAGE_BIT             0x0200
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT 0x00004000
#define GL_BUFFER_IMMUTABLE_STORAGE       0x821F
#define GL_BUFFER_STORAGE_FLAGS           0x8220
#define GL_CLEAR_TEXTURE                  0x9365
#define GL_LOCATION_COMPONENT             0x934A
#define GL_TRANSFORM_FEEDBACK_BUFFER_INDEX 0x934B
#define GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE 0x934C
#define GL_QUERY_BUFFER                   0x9192
#define GL_QUERY_BUFFER_BARRIER_BIT       0x00008000
#define GL_QUERY_BUFFER_BINDING           0x9193
#define GL_QUERY_RESULT_NO_WAIT           0x9194
#define GL_MIRROR_CLAMP_TO_EDGE           0x8743
#endif /* GL_VERSION_4_4 */
#endif /* SIGL_NO_GL_4_4_H_MACROS */
#if !defined(SIGL_NO_GL_4_5_H_MACROS)
#ifndef GL_VERSION_4_5
#define GL_VERSION_4_5 1
#define GL_CONTEXT_LOST                   0x0507
#define GL_NEGATIVE_ONE_TO_ONE            0x935E
#define GL_ZERO_TO_ONE                    0x935F
#define GL_CLIP_ORIGIN                    0x935C
#define GL_CLIP_DEPTH_MODE                0x935D
#define GL_QUERY_WAIT_INVERTED            0x8E17
#define GL_QUERY_NO_WAIT_INVERTED         0x8E18
#define GL_QUERY_BY_REGION_WAIT_INVERTED  0x8E19
#define GL_QUERY_BY_REGION_NO_WAIT_INVERTED 0x8E1A
#define GL_MAX_CULL_DISTANCES             0x82F9
#define GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES 0x82FA
#define GL_TEXTURE_TARGET                 0x1006
#define GL_QUERY_TARGET                   0x82EA
#define GL_GUILTY_CONTEXT_RESET           0x8253
#define GL_INNOCENT_CONTEXT_RESET         0x8254
#define GL_UNKNOWN_CONTEXT_RESET          0x8255
#define GL_RESET_NOTIFICATION_STRATEGY    0x8256
#define GL_LOSE_CONTEXT_ON_RESET          0x8252
#define GL_NO_RESET_NOTIFICATION          0x8261
#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT 0x00000004
#define GL_CONTEXT_RELEASE_BEHAVIOR       0x82FB
#define GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH 0x82FC
#endif /* GL_VERSION_4_5 */
#endif /* SIGL_NO_GL_4_5_H_MACROS */
#if !defined(SIGL_NO_GL_4_6_H_MACROS)
#ifndef GL_VERSION_4_6
#define GL_VERSION_4_6 1
#define GL_SHADER_BINARY_FORMAT_SPIR_V    0x9551
#define GL_SPIR_V_BINARY                  0x9552
#define GL_PARAMETER_BUFFER               0x80EE
#define GL_PARAMETER_BUFFER_BINDING       0x80EF
#define GL_CONTEXT_FLAG_NO_ERROR_BIT      0x00000008
#define GL_VERTICES_SUBMITTED             0x82EE
#define GL_PRIMITIVES_SUBMITTED           0x82EF
#define GL_VERTEX_SHADER_INVOCATIONS      0x82F0
#define GL_TESS_CONTROL_SHADER_PATCHES    0x82F1
#define GL_TESS_EVALUATION_SHADER_INVOCATIONS 0x82F2
#define GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED 0x82F3
#define GL_FRAGMENT_SHADER_INVOCATIONS    0x82F4
#define GL_COMPUTE_SHADER_INVOCATIONS     0x82F5
#define GL_CLIPPING_INPUT_PRIMITIVES      0x82F6
#define GL_CLIPPING_OUTPUT_PRIMITIVES     0x82F7
#define GL_POLYGON_OFFSET_CLAMP           0x8E1B
#define GL_SPIR_V_EXTENSIONS              0x9553
#define GL_NUM_SPIR_V_EXTENSIONS          0x9554
#define GL_TEXTURE_MAX_ANISOTROPY         0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY     0x84FF
#define GL_TRANSFORM_FEEDBACK_OVERFLOW    0x82EC
#define GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW 0x82ED
#endif /* GL_VERSION_4_6 */
#endif /* SIGL_NO_GL_4_6_H_MACROS */
#if SIGL_SYSTEM_IS_WINDOWS && !defined(SIGL_NO_WGL_H_MACROS) && !defined(__wgl_wgl_h_)
#if defined(SIGL_NO_WINDOWS_H) && (defined(WGL_VERSION_1_0) || defined(SIGL_INCLUDE_WGL_EXT))
#undef WGL_VERSION_1_0
#define WGL_VERSION_1_0 1
#define WGL_FONT_LINES                    0
#define WGL_FONT_POLYGONS                 1
#define WGL_SWAP_MAIN_PLANE               0x00000001
#define WGL_SWAP_OVERLAY1                 0x00000002
#define WGL_SWAP_OVERLAY2                 0x00000004
#define WGL_SWAP_OVERLAY3                 0x00000008
#define WGL_SWAP_OVERLAY4                 0x00000010
#define WGL_SWAP_OVERLAY5                 0x00000020
#define WGL_SWAP_OVERLAY6                 0x00000040
#define WGL_SWAP_OVERLAY7                 0x00000080
#define WGL_SWAP_OVERLAY8                 0x00000100
#define WGL_SWAP_OVERLAY9                 0x00000200
#define WGL_SWAP_OVERLAY10                0x00000400
#define WGL_SWAP_OVERLAY11                0x00000800
#define WGL_SWAP_OVERLAY12                0x00001000
#define WGL_SWAP_OVERLAY13                0x00002000
#define WGL_SWAP_OVERLAY14                0x00004000
#define WGL_SWAP_OVERLAY15                0x00008000
#define WGL_SWAP_UNDERLAY1                0x00010000
#define WGL_SWAP_UNDERLAY2                0x00020000
#define WGL_SWAP_UNDERLAY3                0x00040000
#define WGL_SWAP_UNDERLAY4                0x00080000
#define WGL_SWAP_UNDERLAY5                0x00100000
#define WGL_SWAP_UNDERLAY6                0x00200000
#define WGL_SWAP_UNDERLAY7                0x00400000
#define WGL_SWAP_UNDERLAY8                0x00800000
#define WGL_SWAP_UNDERLAY9                0x01000000
#define WGL_SWAP_UNDERLAY10               0x02000000
#define WGL_SWAP_UNDERLAY11               0x04000000
#define WGL_SWAP_UNDERLAY12               0x08000000
#define WGL_SWAP_UNDERLAY13               0x10000000
#define WGL_SWAP_UNDERLAY14               0x20000000
#define WGL_SWAP_UNDERLAY15               0x40000000
#endif /* WGL_VERSION_1_0 */
#if defined(WGL_ARB_buffer_region) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_buffer_region
#define WGL_ARB_buffer_region 1
#define WGL_FRONT_COLOR_BUFFER_BIT_ARB    0x00000001
#define WGL_BACK_COLOR_BUFFER_BIT_ARB     0x00000002
#define WGL_DEPTH_BUFFER_BIT_ARB          0x00000004
#define WGL_STENCIL_BUFFER_BIT_ARB        0x00000008
#endif /* WGL_ARB_buffer_region */
#if defined(WGL_ARB_context_flush_control) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_context_flush_control
#define WGL_ARB_context_flush_control 1
#define WGL_CONTEXT_RELEASE_BEHAVIOR_ARB  0x2097
#define WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB 0
#define WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB 0x2098
#endif /* WGL_ARB_context_flush_control */
#if defined(WGL_ARB_create_context) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_create_context
#define WGL_ARB_create_context 1
#define WGL_CONTEXT_DEBUG_BIT_ARB         0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB       0x2093
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define ERROR_INVALID_VERSION_ARB         0x2095
#endif /* WGL_ARB_create_context */
#if defined(WGL_ARB_create_context_no_error) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_create_context_no_error
#define WGL_ARB_create_context_no_error 1
#define WGL_CONTEXT_OPENGL_NO_ERROR_ARB   0x31B3
#endif /* WGL_ARB_create_context_no_error */
#if defined(WGL_ARB_create_context_profile) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_create_context_profile
#define WGL_ARB_create_context_profile 1
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define ERROR_INVALID_PROFILE_ARB         0x2096
#endif /* WGL_ARB_create_context_profile */
#if defined(WGL_ARB_create_context_robustness) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_create_context_robustness
#define WGL_ARB_create_context_robustness 1
#define WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB 0x00000004
#define WGL_LOSE_CONTEXT_ON_RESET_ARB     0x8252
#define WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define WGL_NO_RESET_NOTIFICATION_ARB     0x8261
#endif /* WGL_ARB_create_context_robustness */
#if defined(WGL_ARB_extensions_string) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_extensions_string
#define WGL_ARB_extensions_string 1
#endif /* WGL_ARB_extensions_string */
#if defined(WGL_ARB_framebuffer_sRGB) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_framebuffer_sRGB
#define WGL_ARB_framebuffer_sRGB 1
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB  0x20A9
#endif /* WGL_ARB_framebuffer_sRGB */
#if defined(WGL_ARB_make_current_read) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_make_current_read
#define WGL_ARB_make_current_read 1
#define ERROR_INVALID_PIXEL_TYPE_ARB      0x2043
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB 0x2054
#endif /* WGL_ARB_make_current_read */
#if defined(WGL_ARB_multisample) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_multisample
#define WGL_ARB_multisample 1
#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042
#endif /* WGL_ARB_multisample */
#if defined(WGL_ARB_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_pbuffer
#define WGL_ARB_pbuffer 1
#define WGL_DRAW_TO_PBUFFER_ARB           0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB        0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB         0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB        0x2030
#define WGL_PBUFFER_LARGEST_ARB           0x2033
#define WGL_PBUFFER_WIDTH_ARB             0x2034
#define WGL_PBUFFER_HEIGHT_ARB            0x2035
#define WGL_PBUFFER_LOST_ARB              0x2036
#endif /* WGL_ARB_pbuffer */
#if defined(WGL_ARB_pixel_format) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_pixel_format
#define WGL_ARB_pixel_format 1
#define WGL_NUMBER_PIXEL_FORMATS_ARB      0x2000
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_DRAW_TO_BITMAP_ARB            0x2002
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_NEED_PALETTE_ARB              0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB       0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB        0x2006
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_NUMBER_OVERLAYS_ARB           0x2008
#define WGL_NUMBER_UNDERLAYS_ARB          0x2009
#define WGL_TRANSPARENT_ARB               0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB     0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB   0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB    0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB   0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB   0x203B
#define WGL_SHARE_DEPTH_ARB               0x200C
#define WGL_SHARE_STENCIL_ARB             0x200D
#define WGL_SHARE_ACCUM_ARB               0x200E
#define WGL_SUPPORT_GDI_ARB               0x200F
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_STEREO_ARB                    0x2012
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_RED_BITS_ARB                  0x2015
#define WGL_RED_SHIFT_ARB                 0x2016
#define WGL_GREEN_BITS_ARB                0x2017
#define WGL_GREEN_SHIFT_ARB               0x2018
#define WGL_BLUE_BITS_ARB                 0x2019
#define WGL_BLUE_SHIFT_ARB                0x201A
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_ALPHA_SHIFT_ARB               0x201C
#define WGL_ACCUM_BITS_ARB                0x201D
#define WGL_ACCUM_RED_BITS_ARB            0x201E
#define WGL_ACCUM_GREEN_BITS_ARB          0x201F
#define WGL_ACCUM_BLUE_BITS_ARB           0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB          0x2021
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_AUX_BUFFERS_ARB               0x2024
#define WGL_NO_ACCELERATION_ARB           0x2025
#define WGL_GENERIC_ACCELERATION_ARB      0x2026
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_SWAP_COPY_ARB                 0x2029
#define WGL_SWAP_UNDEFINED_ARB            0x202A
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_TYPE_COLORINDEX_ARB           0x202C
#endif /* WGL_ARB_pixel_format */
#if defined(WGL_ARB_pixel_format_float) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_pixel_format_float
#define WGL_ARB_pixel_format_float 1
#define WGL_TYPE_RGBA_FLOAT_ARB           0x21A0
#endif /* WGL_ARB_pixel_format_float */
#if defined(WGL_ARB_render_texture) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_render_texture
#define WGL_ARB_render_texture 1
#define WGL_BIND_TO_TEXTURE_RGB_ARB       0x2070
#define WGL_BIND_TO_TEXTURE_RGBA_ARB      0x2071
#define WGL_TEXTURE_FORMAT_ARB            0x2072
#define WGL_TEXTURE_TARGET_ARB            0x2073
#define WGL_MIPMAP_TEXTURE_ARB            0x2074
#define WGL_TEXTURE_RGB_ARB               0x2075
#define WGL_TEXTURE_RGBA_ARB              0x2076
#define WGL_NO_TEXTURE_ARB                0x2077
#define WGL_TEXTURE_CUBE_MAP_ARB          0x2078
#define WGL_TEXTURE_1D_ARB                0x2079
#define WGL_TEXTURE_2D_ARB                0x207A
#define WGL_MIPMAP_LEVEL_ARB              0x207B
#define WGL_CUBE_MAP_FACE_ARB             0x207C
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x207D
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x207E
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x207F
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x2080
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x2081
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x2082
#define WGL_FRONT_LEFT_ARB                0x2083
#define WGL_FRONT_RIGHT_ARB               0x2084
#define WGL_BACK_LEFT_ARB                 0x2085
#define WGL_BACK_RIGHT_ARB                0x2086
#define WGL_AUX0_ARB                      0x2087
#define WGL_AUX1_ARB                      0x2088
#define WGL_AUX2_ARB                      0x2089
#define WGL_AUX3_ARB                      0x208A
#define WGL_AUX4_ARB                      0x208B
#define WGL_AUX5_ARB                      0x208C
#define WGL_AUX6_ARB                      0x208D
#define WGL_AUX7_ARB                      0x208E
#define WGL_AUX8_ARB                      0x208F
#define WGL_AUX9_ARB                      0x2090
#endif /* WGL_ARB_render_texture */
#if defined(WGL_ARB_robustness_application_isolation) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_robustness_application_isolation
#define WGL_ARB_robustness_application_isolation 1
#define WGL_CONTEXT_RESET_ISOLATION_BIT_ARB 0x00000008
#endif /* WGL_ARB_robustness_application_isolation */
#if defined(WGL_ARB_robustness_share_group_isolation) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ARB_robustness_share_group_isolation
#define WGL_ARB_robustness_share_group_isolation 1
#endif /* WGL_ARB_robustness_share_group_isolation */
#if defined(WGL_3DFX_multisample) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_3DFX_multisample
#define WGL_3DFX_multisample 1
#define WGL_SAMPLE_BUFFERS_3DFX           0x2060
#define WGL_SAMPLES_3DFX                  0x2061
#endif /* WGL_3DFX_multisample */
#if defined(WGL_3DL_stereo_control) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_3DL_stereo_control
#define WGL_3DL_stereo_control 1
#define WGL_STEREO_EMITTER_ENABLE_3DL     0x2055
#define WGL_STEREO_EMITTER_DISABLE_3DL    0x2056
#define WGL_STEREO_POLARITY_NORMAL_3DL    0x2057
#define WGL_STEREO_POLARITY_INVERT_3DL    0x2058
#endif /* WGL_3DL_stereo_control */
#if defined(WGL_AMD_gpu_association) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_AMD_gpu_association
#define WGL_AMD_gpu_association 1
#define WGL_GPU_VENDOR_AMD                0x1F00
#define WGL_GPU_RENDERER_STRING_AMD       0x1F01
#define WGL_GPU_OPENGL_VERSION_STRING_AMD 0x1F02
#define WGL_GPU_FASTEST_TARGET_GPUS_AMD   0x21A2
#define WGL_GPU_RAM_AMD                   0x21A3
#define WGL_GPU_CLOCK_AMD                 0x21A4
#define WGL_GPU_NUM_PIPES_AMD             0x21A5
#define WGL_GPU_NUM_SIMD_AMD              0x21A6
#define WGL_GPU_NUM_RB_AMD                0x21A7
#define WGL_GPU_NUM_SPI_AMD               0x21A8
#endif /* WGL_AMD_gpu_association */
#if defined(WGL_ATI_pixel_format_float) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ATI_pixel_format_float
#define WGL_ATI_pixel_format_float 1
#define WGL_TYPE_RGBA_FLOAT_ATI           0x21A0
#endif /* WGL_ATI_pixel_format_float */
#if defined(WGL_ATI_render_texture_rectangle) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_ATI_render_texture_rectangle
#define WGL_ATI_render_texture_rectangle 1
#define WGL_TEXTURE_RECTANGLE_ATI         0x21A5
#endif /* WGL_ATI_render_texture_rectangle */
#if defined(WGL_EXT_colorspace) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_colorspace
#define WGL_EXT_colorspace 1
#define WGL_COLORSPACE_EXT                0x309D
#define WGL_COLORSPACE_SRGB_EXT           0x3089
#define WGL_COLORSPACE_LINEAR_EXT         0x308A
#endif /* WGL_EXT_colorspace */
#if defined(WGL_EXT_create_context_es2_profile) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_create_context_es2_profile
#define WGL_EXT_create_context_es2_profile 1
#define WGL_CONTEXT_ES2_PROFILE_BIT_EXT   0x00000004
#endif /* WGL_EXT_create_context_es2_profile */
#if defined(WGL_EXT_create_context_es_profile) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_create_context_es_profile
#define WGL_EXT_create_context_es_profile 1
#define WGL_CONTEXT_ES_PROFILE_BIT_EXT    0x00000004
#endif /* WGL_EXT_create_context_es_profile */
#if defined(WGL_EXT_depth_float) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_depth_float
#define WGL_EXT_depth_float 1
#define WGL_DEPTH_FLOAT_EXT               0x2040
#endif /* WGL_EXT_depth_float */
#if defined(WGL_EXT_display_color_table) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_display_color_table
#define WGL_EXT_display_color_table 1
#endif /* WGL_EXT_display_color_table */
#if defined(WGL_EXT_extensions_string) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_extensions_string
#define WGL_EXT_extensions_string 1
#endif /* WGL_EXT_extensions_string */
#if defined(WGL_EXT_framebuffer_sRGB) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_framebuffer_sRGB
#define WGL_EXT_framebuffer_sRGB 1
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT  0x20A9
#endif /* WGL_EXT_framebuffer_sRGB */
#if defined(WGL_EXT_make_current_read) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_make_current_read
#define WGL_EXT_make_current_read 1
#define ERROR_INVALID_PIXEL_TYPE_EXT      0x2043
#endif /* WGL_EXT_make_current_read */
#if defined(WGL_EXT_multisample) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_multisample
#define WGL_EXT_multisample 1
#define WGL_SAMPLE_BUFFERS_EXT            0x2041
#define WGL_SAMPLES_EXT                   0x2042
#endif /* WGL_EXT_multisample */
#if defined(WGL_EXT_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_pbuffer
#define WGL_EXT_pbuffer 1
#define WGL_DRAW_TO_PBUFFER_EXT           0x202D
#define WGL_MAX_PBUFFER_PIXELS_EXT        0x202E
#define WGL_MAX_PBUFFER_WIDTH_EXT         0x202F
#define WGL_MAX_PBUFFER_HEIGHT_EXT        0x2030
#define WGL_OPTIMAL_PBUFFER_WIDTH_EXT     0x2031
#define WGL_OPTIMAL_PBUFFER_HEIGHT_EXT    0x2032
#define WGL_PBUFFER_LARGEST_EXT           0x2033
#define WGL_PBUFFER_WIDTH_EXT             0x2034
#define WGL_PBUFFER_HEIGHT_EXT            0x2035
#endif /* WGL_EXT_pbuffer */
#if defined(WGL_EXT_pixel_format) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_pixel_format
#define WGL_EXT_pixel_format 1
#define WGL_NUMBER_PIXEL_FORMATS_EXT      0x2000
#define WGL_DRAW_TO_WINDOW_EXT            0x2001
#define WGL_DRAW_TO_BITMAP_EXT            0x2002
#define WGL_ACCELERATION_EXT              0x2003
#define WGL_NEED_PALETTE_EXT              0x2004
#define WGL_NEED_SYSTEM_PALETTE_EXT       0x2005
#define WGL_SWAP_LAYER_BUFFERS_EXT        0x2006
#define WGL_SWAP_METHOD_EXT               0x2007
#define WGL_NUMBER_OVERLAYS_EXT           0x2008
#define WGL_NUMBER_UNDERLAYS_EXT          0x2009
#define WGL_TRANSPARENT_EXT               0x200A
#define WGL_TRANSPARENT_VALUE_EXT         0x200B
#define WGL_SHARE_DEPTH_EXT               0x200C
#define WGL_SHARE_STENCIL_EXT             0x200D
#define WGL_SHARE_ACCUM_EXT               0x200E
#define WGL_SUPPORT_GDI_EXT               0x200F
#define WGL_SUPPORT_OPENGL_EXT            0x2010
#define WGL_DOUBLE_BUFFER_EXT             0x2011
#define WGL_STEREO_EXT                    0x2012
#define WGL_PIXEL_TYPE_EXT                0x2013
#define WGL_COLOR_BITS_EXT                0x2014
#define WGL_RED_BITS_EXT                  0x2015
#define WGL_RED_SHIFT_EXT                 0x2016
#define WGL_GREEN_BITS_EXT                0x2017
#define WGL_GREEN_SHIFT_EXT               0x2018
#define WGL_BLUE_BITS_EXT                 0x2019
#define WGL_BLUE_SHIFT_EXT                0x201A
#define WGL_ALPHA_BITS_EXT                0x201B
#define WGL_ALPHA_SHIFT_EXT               0x201C
#define WGL_ACCUM_BITS_EXT                0x201D
#define WGL_ACCUM_RED_BITS_EXT            0x201E
#define WGL_ACCUM_GREEN_BITS_EXT          0x201F
#define WGL_ACCUM_BLUE_BITS_EXT           0x2020
#define WGL_ACCUM_ALPHA_BITS_EXT          0x2021
#define WGL_DEPTH_BITS_EXT                0x2022
#define WGL_STENCIL_BITS_EXT              0x2023
#define WGL_AUX_BUFFERS_EXT               0x2024
#define WGL_NO_ACCELERATION_EXT           0x2025
#define WGL_GENERIC_ACCELERATION_EXT      0x2026
#define WGL_FULL_ACCELERATION_EXT         0x2027
#define WGL_SWAP_EXCHANGE_EXT             0x2028
#define WGL_SWAP_COPY_EXT                 0x2029
#define WGL_SWAP_UNDEFINED_EXT            0x202A
#define WGL_TYPE_RGBA_EXT                 0x202B
#define WGL_TYPE_COLORINDEX_EXT           0x202C
#endif /* WGL_EXT_pixel_format */
#if defined(WGL_EXT_pixel_format_packed_float) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_pixel_format_packed_float
#define WGL_EXT_pixel_format_packed_float 1
#define WGL_TYPE_RGBA_UNSIGNED_FLOAT_EXT  0x20A8
#endif /* WGL_EXT_pixel_format_packed_float */
#if defined(WGL_EXT_swap_control) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_swap_control
#define WGL_EXT_swap_control 1
#endif /* WGL_EXT_swap_control */
#if defined(WGL_EXT_swap_control_tear) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_EXT_swap_control_tear
#define WGL_EXT_swap_control_tear 1
#endif /* WGL_EXT_swap_control_tear */
#if defined(WGL_I3D_digital_video_control) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_I3D_digital_video_control
#define WGL_I3D_digital_video_control 1
#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_FRAMEBUFFER_I3D 0x2050
#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_VALUE_I3D 0x2051
#define WGL_DIGITAL_VIDEO_CURSOR_INCLUDED_I3D 0x2052
#define WGL_DIGITAL_VIDEO_GAMMA_CORRECTED_I3D 0x2053
#endif /* WGL_I3D_digital_video_control */
#if defined(WGL_I3D_gamma) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_I3D_gamma
#define WGL_I3D_gamma 1
#define WGL_GAMMA_TABLE_SIZE_I3D          0x204E
#define WGL_GAMMA_EXCLUDE_DESKTOP_I3D     0x204F
#endif /* WGL_I3D_gamma */
#if defined(WGL_I3D_genlock) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_I3D_genlock
#define WGL_I3D_genlock 1
#define WGL_GENLOCK_SOURCE_MULTIVIEW_I3D  0x2044
#define WGL_GENLOCK_SOURCE_EXTERNAL_SYNC_I3D 0x2045
#define WGL_GENLOCK_SOURCE_EXTERNAL_FIELD_I3D 0x2046
#define WGL_GENLOCK_SOURCE_EXTERNAL_TTL_I3D 0x2047
#define WGL_GENLOCK_SOURCE_DIGITAL_SYNC_I3D 0x2048
#define WGL_GENLOCK_SOURCE_DIGITAL_FIELD_I3D 0x2049
#define WGL_GENLOCK_SOURCE_EDGE_FALLING_I3D 0x204A
#define WGL_GENLOCK_SOURCE_EDGE_RISING_I3D 0x204B
#define WGL_GENLOCK_SOURCE_EDGE_BOTH_I3D  0x204C
#endif /* WGL_I3D_genlock */
#if defined(WGL_I3D_image_buffer) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_I3D_image_buffer
#define WGL_I3D_image_buffer 1
#define WGL_IMAGE_BUFFER_MIN_ACCESS_I3D   0x00000001
#define WGL_IMAGE_BUFFER_LOCK_I3D         0x00000002
#endif /* WGL_I3D_image_buffer */
#if defined(WGL_I3D_swap_frame_lock) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_I3D_swap_frame_lock
#define WGL_I3D_swap_frame_lock 1
#endif /* WGL_I3D_swap_frame_lock */
#if defined(WGL_I3D_swap_frame_usage) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_I3D_swap_frame_usage
#define WGL_I3D_swap_frame_usage 1
#endif /* WGL_I3D_swap_frame_usage */
#if defined(WGL_NV_DX_interop) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_DX_interop
#define WGL_NV_DX_interop 1
#define WGL_ACCESS_READ_ONLY_NV           0x00000000
#define WGL_ACCESS_READ_WRITE_NV          0x00000001
#define WGL_ACCESS_WRITE_DISCARD_NV       0x00000002
#endif /* WGL_NV_DX_interop */
#if defined(WGL_NV_DX_interop2) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_DX_interop2
#define WGL_NV_DX_interop2 1
#endif /* WGL_NV_DX_interop2 */
#if defined(WGL_NV_copy_image) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_copy_image
#define WGL_NV_copy_image 1
#endif /* WGL_NV_copy_image */
#if defined(WGL_NV_delay_before_swap) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_delay_before_swap
#define WGL_NV_delay_before_swap 1
#endif /* WGL_NV_delay_before_swap */
#if defined(WGL_NV_float_buffer) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_float_buffer
#define WGL_NV_float_buffer 1
#define WGL_FLOAT_COMPONENTS_NV           0x20B0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_R_NV 0x20B1
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RG_NV 0x20B2
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV 0x20B3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV 0x20B4
#define WGL_TEXTURE_FLOAT_R_NV            0x20B5
#define WGL_TEXTURE_FLOAT_RG_NV           0x20B6
#define WGL_TEXTURE_FLOAT_RGB_NV          0x20B7
#define WGL_TEXTURE_FLOAT_RGBA_NV         0x20B8
#endif /* WGL_NV_float_buffer */
#if defined(WGL_NV_gpu_affinity) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_gpu_affinity
#define WGL_NV_gpu_affinity 1
#define ERROR_INCOMPATIBLE_AFFINITY_MASKS_NV 0x20D0
#define ERROR_MISSING_AFFINITY_MASK_NV    0x20D1
#endif /* WGL_NV_gpu_affinity */
#if defined(WGL_NV_multisample_coverage) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_multisample_coverage
#define WGL_NV_multisample_coverage 1
#define WGL_COVERAGE_SAMPLES_NV           0x2042
#define WGL_COLOR_SAMPLES_NV              0x20B9
#endif /* WGL_NV_multisample_coverage */
#if defined(WGL_NV_present_video) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_present_video
#define WGL_NV_present_video 1
#define WGL_NUM_VIDEO_SLOTS_NV            0x20F0
#endif /* WGL_NV_present_video */
#if defined(WGL_NV_render_depth_texture) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_render_depth_texture
#define WGL_NV_render_depth_texture 1
#define WGL_BIND_TO_TEXTURE_DEPTH_NV      0x20A3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_DEPTH_NV 0x20A4
#define WGL_DEPTH_TEXTURE_FORMAT_NV       0x20A5
#define WGL_TEXTURE_DEPTH_COMPONENT_NV    0x20A6
#define WGL_DEPTH_COMPONENT_NV            0x20A7
#endif /* WGL_NV_render_depth_texture */
#if defined(WGL_NV_render_texture_rectangle) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_render_texture_rectangle
#define WGL_NV_render_texture_rectangle 1
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGB_NV 0x20A0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV 0x20A1
#define WGL_TEXTURE_RECTANGLE_NV          0x20A2
#endif /* WGL_NV_render_texture_rectangle */
#if defined(WGL_NV_swap_group) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_swap_group
#define WGL_NV_swap_group 1
#endif /* WGL_NV_swap_group */
#if defined(WGL_NV_vertex_array_range) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_vertex_array_range
#define WGL_NV_vertex_array_range 1
#endif /* WGL_NV_vertex_array_range */
#if defined(WGL_NV_video_capture) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_video_capture
#define WGL_NV_video_capture 1
#define WGL_UNIQUE_ID_NV                  0x20CE
#define WGL_NUM_VIDEO_CAPTURE_SLOTS_NV    0x20CF
#endif /* WGL_NV_video_capture */
#if defined(WGL_NV_video_output) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_NV_video_output
#define WGL_NV_video_output 1
#define WGL_BIND_TO_VIDEO_RGB_NV          0x20C0
#define WGL_BIND_TO_VIDEO_RGBA_NV         0x20C1
#define WGL_BIND_TO_VIDEO_RGB_AND_DEPTH_NV 0x20C2
#define WGL_VIDEO_OUT_COLOR_NV            0x20C3
#define WGL_VIDEO_OUT_ALPHA_NV            0x20C4
#define WGL_VIDEO_OUT_DEPTH_NV            0x20C5
#define WGL_VIDEO_OUT_COLOR_AND_ALPHA_NV  0x20C6
#define WGL_VIDEO_OUT_COLOR_AND_DEPTH_NV  0x20C7
#define WGL_VIDEO_OUT_FRAME               0x20C8
#define WGL_VIDEO_OUT_FIELD_1             0x20C9
#define WGL_VIDEO_OUT_FIELD_2             0x20CA
#define WGL_VIDEO_OUT_STACKED_FIELDS_1_2  0x20CB
#define WGL_VIDEO_OUT_STACKED_FIELDS_2_1  0x20CC
#endif /* WGL_NV_video_output */
#if defined(WGL_OML_sync_control) || defined(SIGL_INCLUDE_WGL_EXT)
#undef WGL_OML_sync_control
#define WGL_OML_sync_control 1
#endif /* WGL_OML_sync_control */
#endif
#if SIGL_SYSTEM_IS_UNIX && !defined(SIGL_NO_GLX_H_MACROS) && !defined(GLX_H)
#define GLX_H

/*
 * Tokens for glXChooseVisual and glXGetConfig:
 */
#define GLX_USE_GL		1
#define GLX_BUFFER_SIZE		2
#define GLX_LEVEL		3
#define GLX_RGBA		4
#define GLX_DOUBLEBUFFER	5
#define GLX_STEREO		6
#define GLX_AUX_BUFFERS		7
#define GLX_RED_SIZE		8
#define GLX_GREEN_SIZE		9
#define GLX_BLUE_SIZE		10
#define GLX_ALPHA_SIZE		11
#define GLX_DEPTH_SIZE		12
#define GLX_STENCIL_SIZE	13
#define GLX_ACCUM_RED_SIZE	14
#define GLX_ACCUM_GREEN_SIZE	15
#define GLX_ACCUM_BLUE_SIZE	16
#define GLX_ACCUM_ALPHA_SIZE	17

#define GLX_BAD_SCREEN		1
#define GLX_BAD_ATTRIBUTE	2
#define GLX_NO_EXTENSION	3
#define GLX_BAD_VISUAL		4
#define GLX_BAD_CONTEXT		5
#define GLX_BAD_VALUE       	6
#define GLX_BAD_ENUM		7


#ifndef GLX_VERSION_1_1
#define GLX_VERSION_1_1		1
#define GLX_VENDOR		1
#define GLX_VERSION		2
#define GLX_EXTENSIONS 		3
#endif

#ifndef GLX_VERSION_1_3
#define GLX_VERSION_1_3		1
#define GLX_CONFIG_CAVEAT		0x20
#define GLX_DONT_CARE			0xFFFFFFFF
#define GLX_X_VISUAL_TYPE		0x22
#define GLX_TRANSPARENT_TYPE		0x23
#define GLX_TRANSPARENT_INDEX_VALUE	0x24
#define GLX_TRANSPARENT_RED_VALUE	0x25
#define GLX_TRANSPARENT_GREEN_VALUE	0x26
#define GLX_TRANSPARENT_BLUE_VALUE	0x27
#define GLX_TRANSPARENT_ALPHA_VALUE	0x28
#define GLX_WINDOW_BIT			0x00000001
#define GLX_PIXMAP_BIT			0x00000002
#define GLX_PBUFFER_BIT			0x00000004
#define GLX_AUX_BUFFERS_BIT		0x00000010
#define GLX_FRONT_LEFT_BUFFER_BIT	0x00000001
#define GLX_FRONT_RIGHT_BUFFER_BIT	0x00000002
#define GLX_BACK_LEFT_BUFFER_BIT	0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT	0x00000008
#define GLX_DEPTH_BUFFER_BIT		0x00000020
#define GLX_STENCIL_BUFFER_BIT		0x00000040
#define GLX_ACCUM_BUFFER_BIT		0x00000080
#define GLX_NONE			0x8000
#define GLX_SLOW_CONFIG			0x8001
#define GLX_TRUE_COLOR			0x8002
#define GLX_DIRECT_COLOR		0x8003
#define GLX_PSEUDO_COLOR		0x8004
#define GLX_STATIC_COLOR		0x8005
#define GLX_GRAY_SCALE			0x8006
#define GLX_STATIC_GRAY			0x8007
#define GLX_TRANSPARENT_RGB		0x8008
#define GLX_TRANSPARENT_INDEX		0x8009
#define GLX_VISUAL_ID			0x800B
#define GLX_SCREEN			0x800C
#define GLX_NON_CONFORMANT_CONFIG	0x800D
#define GLX_DRAWABLE_TYPE		0x8010
#define GLX_RENDER_TYPE			0x8011
#define GLX_X_RENDERABLE		0x8012
#define GLX_FBCONFIG_ID			0x8013
#define GLX_RGBA_TYPE			0x8014
#define GLX_COLOR_INDEX_TYPE		0x8015
#define GLX_MAX_PBUFFER_WIDTH		0x8016
#define GLX_MAX_PBUFFER_HEIGHT		0x8017
#define GLX_MAX_PBUFFER_PIXELS		0x8018
#define GLX_PRESERVED_CONTENTS		0x801B
#define GLX_LARGEST_PBUFFER		0x801C
#define GLX_WIDTH			0x801D
#define GLX_HEIGHT			0x801E
#define GLX_EVENT_MASK			0x801F
#define GLX_DAMAGED			0x8020
#define GLX_SAVED			0x8021
#define GLX_WINDOW			0x8022
#define GLX_PBUFFER			0x8023
#define GLX_PBUFFER_HEIGHT              0x8040
#define GLX_PBUFFER_WIDTH               0x8041
#define GLX_RGBA_BIT			0x00000001
#define GLX_COLOR_INDEX_BIT		0x00000002
#define GLX_PBUFFER_CLOBBER_MASK	0x08000000

#define GLX_WINDOW_BIT                    0x00000001
#define GLX_PIXMAP_BIT                    0x00000002
#define GLX_PBUFFER_BIT                   0x00000004
#define GLX_RGBA_BIT                      0x00000001
#define GLX_COLOR_INDEX_BIT               0x00000002
#define GLX_PBUFFER_CLOBBER_MASK          0x08000000
#define GLX_FRONT_LEFT_BUFFER_BIT         0x00000001
#define GLX_FRONT_RIGHT_BUFFER_BIT        0x00000002
#define GLX_BACK_LEFT_BUFFER_BIT          0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT         0x00000008
#define GLX_AUX_BUFFERS_BIT               0x00000010
#define GLX_DEPTH_BUFFER_BIT              0x00000020
#define GLX_STENCIL_BUFFER_BIT            0x00000040
#define GLX_ACCUM_BUFFER_BIT              0x00000080
#define GLX_CONFIG_CAVEAT                 0x20
#define GLX_X_VISUAL_TYPE                 0x22
#define GLX_TRANSPARENT_TYPE              0x23
#define GLX_TRANSPARENT_INDEX_VALUE       0x24
#define GLX_TRANSPARENT_RED_VALUE         0x25
#define GLX_TRANSPARENT_GREEN_VALUE       0x26
#define GLX_TRANSPARENT_BLUE_VALUE        0x27
#define GLX_TRANSPARENT_ALPHA_VALUE       0x28
#define GLX_DONT_CARE                     0xFFFFFFFF
#define GLX_NONE                          0x8000
#define GLX_SLOW_CONFIG                   0x8001
#define GLX_TRUE_COLOR                    0x8002
#define GLX_DIRECT_COLOR                  0x8003
#define GLX_PSEUDO_COLOR                  0x8004
#define GLX_STATIC_COLOR                  0x8005
#define GLX_GRAY_SCALE                    0x8006
#define GLX_STATIC_GRAY                   0x8007
#define GLX_TRANSPARENT_RGB               0x8008
#define GLX_TRANSPARENT_INDEX             0x8009
#define GLX_VISUAL_ID                     0x800B
#define GLX_SCREEN                        0x800C
#define GLX_NON_CONFORMANT_CONFIG         0x800D
#define GLX_DRAWABLE_TYPE                 0x8010
#define GLX_RENDER_TYPE                   0x8011
#define GLX_X_RENDERABLE                  0x8012
#define GLX_FBCONFIG_ID                   0x8013
#define GLX_RGBA_TYPE                     0x8014
#define GLX_COLOR_INDEX_TYPE              0x8015
#define GLX_MAX_PBUFFER_WIDTH             0x8016
#define GLX_MAX_PBUFFER_HEIGHT            0x8017
#define GLX_MAX_PBUFFER_PIXELS            0x8018
#define GLX_PRESERVED_CONTENTS            0x801B
#define GLX_LARGEST_PBUFFER               0x801C
#define GLX_WIDTH                         0x801D
#define GLX_HEIGHT                        0x801E
#define GLX_EVENT_MASK                    0x801F
#define GLX_DAMAGED                       0x8020
#define GLX_SAVED                         0x8021
#define GLX_WINDOW                        0x8022
#define GLX_PBUFFER                       0x8023
#define GLX_PBUFFER_HEIGHT                0x8040
#define GLX_PBUFFER_WIDTH                 0x8041
#endif

#ifndef GLX_VERSION_1_4
#define GLX_VERSION_1_4
#define GLX_SAMPLE_BUFFERS              0x186a0 /*100000*/
#define GLX_SAMPLES                     0x186a1 /*100001*/
#endif


#ifndef GLX_ARB_context_flush_control
#define GLX_ARB_context_flush_control 1
#define GLX_CONTEXT_RELEASE_BEHAVIOR_ARB  0x2097
#define GLX_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB 0
#define GLX_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB 0x2098
#endif /* GLX_ARB_context_flush_control */

#ifndef GLX_ARB_create_context
#define GLX_ARB_create_context 1
#define GLX_CONTEXT_DEBUG_BIT_ARB         0x00000001
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define GLX_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB     0x2092
#define GLX_CONTEXT_FLAGS_ARB             0x2094
#endif /* GLX_ARB_create_context */

#ifndef GLX_ARB_create_context_no_error
#define GLX_ARB_create_context_no_error 1
#define GLX_CONTEXT_OPENGL_NO_ERROR_ARB   0x31B3
#endif /* GLX_ARB_create_context_no_error */

#ifndef GLX_ARB_create_context_profile
#define GLX_ARB_create_context_profile 1
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define GLX_CONTEXT_PROFILE_MASK_ARB      0x9126
#endif /* GLX_ARB_create_context_profile */

#endif

/* Loads every possible OpenGL function from 1.2 up to 4.6, including any platform
 * specific ones. */
void sigl_loadOpenGLAll(void);
/* */
void sigl_loadOpenGLAllVer(int major, int minor, int osFuncs);
/* Loads every OpenGL 1.2 function. */
void sigl_loadOpenGL_1_2(void);
/* Loads every OpenGL 1.3 function. */
void sigl_loadOpenGL_1_3(void);
/* Loads every OpenGL 1.4 function. */
void sigl_loadOpenGL_1_4(void);
/* Loads every OpenGL 1.5 function. */
void sigl_loadOpenGL_1_5(void);
/* Loads every OpenGL 2.0 function. */
void sigl_loadOpenGL_2_0(void);
/* Loads every OpenGL 2.1 function. */
void sigl_loadOpenGL_2_1(void);
/* Loads every OpenGL 3.0 function. */
void sigl_loadOpenGL_3_0(void);
/* Loads every OpenGL 3.1 function. */
void sigl_loadOpenGL_3_1(void);
/* Loads every OpenGL 3.2 function. */
void sigl_loadOpenGL_3_2(void);
/* Loads every OpenGL 3.3 function. */
void sigl_loadOpenGL_3_3(void);
/* Loads every OpenGL 4.0 function. */
void sigl_loadOpenGL_4_0(void);
/* Loads every OpenGL 4.1 function. */
void sigl_loadOpenGL_4_1(void);
/* Loads every OpenGL 4.2 function. */
void sigl_loadOpenGL_4_2(void);
/* Loads every OpenGL 4.3 function. */
void sigl_loadOpenGL_4_3(void);
/* Loads every OpenGL 4.4 function. */
void sigl_loadOpenGL_4_4(void);
/* Loads every OpenGL 4.5 function. */
void sigl_loadOpenGL_4_5(void);
/* Loads every OpenGL 4.6 function. */
void sigl_loadOpenGL_4_6(void);

/* Loads every OS extension function available for the system. */
void sigl_loadOpenGLOS(void);


#if !defined(__gl_h_) && !defined(__GL_H__) && !defined(SIGL_NO_GL_1_1_H_FUNCS)

#define SIGL_FUNC_DECLARE(retType, name, ...) GLAPI retType GLAPIENTRY name(__VA_ARGS__)

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glAccum, GLenum op, GLfloat value);
SIGL_FUNC_DECLARE(void, glAlphaFunc, GLenum func, GLclampf ref);
SIGL_FUNC_DECLARE(GLboolean, glAreTexturesResident, GLsizei n, const GLuint* textures, GLboolean* residences);
SIGL_FUNC_DECLARE(void, glArrayElement, GLint i);
SIGL_FUNC_DECLARE(void, glBegin, GLenum mode);
SIGL_FUNC_DECLARE(void, glBitmap, GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte* bitmap);
SIGL_FUNC_DECLARE(void, glCallList, GLuint list);
SIGL_FUNC_DECLARE(void, glCallLists, GLsizei n, GLenum type, const GLvoid* lists);
#endif

SIGL_FUNC_DECLARE(void, glBlendFunc, GLenum sfactor, GLenum dfactor);
SIGL_FUNC_DECLARE(void, glBindTexture, GLenum target, GLuint texture);
SIGL_FUNC_DECLARE(void, glClear, GLbitfield mask);
SIGL_FUNC_DECLARE(void, glClearColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glClearAccum, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
SIGL_FUNC_DECLARE(void, glClearDepth, GLclampd depth);
SIGL_FUNC_DECLARE(void, glClearIndex, GLfloat c);
SIGL_FUNC_DECLARE(void, glClearStencil, GLint s);
SIGL_FUNC_DECLARE(void, glClipPlane, GLenum plane, const GLdouble* equation);
SIGL_FUNC_DECLARE(void, glColor3b, GLbyte red, GLbyte green, GLbyte blue);
SIGL_FUNC_DECLARE(void, glColor3bv, const GLbyte* v);
SIGL_FUNC_DECLARE(void, glColor3d, GLdouble red, GLdouble green, GLdouble blue);
SIGL_FUNC_DECLARE(void, glColor3dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glColor3f, GLfloat red, GLfloat green, GLfloat blue);
SIGL_FUNC_DECLARE(void, glColor3fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glColor3i, GLint red, GLint green, GLint blue);
SIGL_FUNC_DECLARE(void, glColor3iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glColor3s, GLshort red, GLshort green, GLshort blue);
SIGL_FUNC_DECLARE(void, glColor3sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glColor3ub, GLubyte red, GLubyte green, GLubyte blue);
SIGL_FUNC_DECLARE(void, glColor3ubv, const GLubyte* v);
SIGL_FUNC_DECLARE(void, glColor3ui, GLuint red, GLuint green, GLuint blue);
SIGL_FUNC_DECLARE(void, glColor3uiv, const GLuint* v);
SIGL_FUNC_DECLARE(void, glColor3us, GLushort red, GLushort green, GLushort blue);
SIGL_FUNC_DECLARE(void, glColor3usv, const GLushort* v);
SIGL_FUNC_DECLARE(void, glColor4b, GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
SIGL_FUNC_DECLARE(void, glColor4bv, const GLbyte* v);
SIGL_FUNC_DECLARE(void, glColor4d, GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
SIGL_FUNC_DECLARE(void, glColor4dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glColor4f, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
SIGL_FUNC_DECLARE(void, glColor4fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glColor4i, GLint red, GLint green, GLint blue, GLint alpha);
SIGL_FUNC_DECLARE(void, glColor4iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glColor4s, GLshort red, GLshort green, GLshort blue, GLshort alpha);
SIGL_FUNC_DECLARE(void, glColor4sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glColor4ub, GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
SIGL_FUNC_DECLARE(void, glColor4ubv, const GLubyte* v);
SIGL_FUNC_DECLARE(void, glColor4ui, GLuint red, GLuint green, GLuint blue, GLuint alpha);
SIGL_FUNC_DECLARE(void, glColor4uiv, const GLuint* v);
SIGL_FUNC_DECLARE(void, glColor4us, GLushort red, GLushort green, GLushort blue, GLushort alpha);
SIGL_FUNC_DECLARE(void, glColor4usv, const GLushort* v);
SIGL_FUNC_DECLARE(void, glColorMask, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
SIGL_FUNC_DECLARE(void, glColorMaterial, GLenum face, GLenum mode);
SIGL_FUNC_DECLARE(void, glColorPointer, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
SIGL_FUNC_DECLARE(void, glCopyPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
#endif

SIGL_FUNC_DECLARE(void, glCopyTexImage1D, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
SIGL_FUNC_DECLARE(void, glCopyTexImage2D, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
SIGL_FUNC_DECLARE(void, glCopyTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
SIGL_FUNC_DECLARE(void, glCopyTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glCullFace, GLenum mode);
SIGL_FUNC_DECLARE(void, glDeleteLists, GLuint list, GLsizei range);
#endif

SIGL_FUNC_DECLARE(void, glDeleteTextures, GLsizei n, const GLuint* textures);
SIGL_FUNC_DECLARE(void, glDepthFunc, GLenum func);
SIGL_FUNC_DECLARE(void, glDepthMask, GLboolean flag);
SIGL_FUNC_DECLARE(void, glDepthRange, GLclampd zNear, GLclampd zFar);
SIGL_FUNC_DECLARE(void, glDisable, GLenum cap);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glDisableClientState, GLenum array);
#endif

SIGL_FUNC_DECLARE(void, glDrawArrays, GLenum mode, GLint first, GLsizei count);
SIGL_FUNC_DECLARE(void, glDrawBuffer, GLenum mode);
SIGL_FUNC_DECLARE(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glDrawPixels, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
SIGL_FUNC_DECLARE(void, glEdgeFlag, GLboolean flag);
SIGL_FUNC_DECLARE(void, glEdgeFlagPointer, GLsizei stride, const GLvoid* pointer);
SIGL_FUNC_DECLARE(void, glEdgeFlagv, const GLboolean* flag);
#endif

SIGL_FUNC_DECLARE(void, glEnable, GLenum cap);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glEnableClientState, GLenum array);
SIGL_FUNC_DECLARE(void, glEnd, void);
SIGL_FUNC_DECLARE(void, glEndList, void);
SIGL_FUNC_DECLARE(void, glEvalCoord1d, GLdouble u);
SIGL_FUNC_DECLARE(void, glEvalCoord1dv, const GLdouble* u);
SIGL_FUNC_DECLARE(void, glEvalCoord1f, GLfloat u);
SIGL_FUNC_DECLARE(void, glEvalCoord1fv, const GLfloat* u);
SIGL_FUNC_DECLARE(void, glEvalCoord2d, GLdouble u, GLdouble v);
SIGL_FUNC_DECLARE(void, glEvalCoord2dv, const GLdouble* u);
SIGL_FUNC_DECLARE(void, glEvalCoord2f, GLfloat u, GLfloat v);
SIGL_FUNC_DECLARE(void, glEvalCoord2fv, const GLfloat* u);
SIGL_FUNC_DECLARE(void, glEvalMesh1, GLenum mode, GLint i1, GLint i2);
SIGL_FUNC_DECLARE(void, glEvalMesh2, GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
SIGL_FUNC_DECLARE(void, glEvalPoint1, GLint i);
SIGL_FUNC_DECLARE(void, glEvalPoint2, GLint i, GLint j);
SIGL_FUNC_DECLARE(void, glFeedbackBuffer, GLsizei size, GLenum type, GLfloat* buffer);
#endif

SIGL_FUNC_DECLARE(void, glFinish, void);
SIGL_FUNC_DECLARE(void, glFlush, void);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glFogf, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glFogfv, GLenum pname, const GLfloat* params);
SIGL_FUNC_DECLARE(void, glFogi, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glFogiv, GLenum pname, const GLint* params);
#endif

SIGL_FUNC_DECLARE(void, glFrontFace, GLenum mode);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glFrustum, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
SIGL_FUNC_DECLARE(GLuint, glGenLists, GLsizei range);
#endif

SIGL_FUNC_DECLARE(void, glGenTextures, GLsizei n, GLuint* textures);
SIGL_FUNC_DECLARE(void, glGetBooleanv, GLenum pname, GLboolean* params);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glGetClipPlane, GLenum plane, GLdouble* equation);
SIGL_FUNC_DECLARE(void, glGetColorTable, GLenum target, GLenum format, GLenum type, GLvoid* data);
SIGL_FUNC_DECLARE(void, glGetColorTableParameterfv, GLenum target, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetColorTableParameteriv, GLenum target, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetConvolutionFilter, GLenum target, GLenum format, GLenum type, GLvoid* image);
SIGL_FUNC_DECLARE(void, glGetConvolutionParameterfv, GLenum target, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetConvolutionParameteriv, GLenum target, GLenum pname, GLint* params);
#endif

SIGL_FUNC_DECLARE(void, glGetDoublev, GLenum pname, GLdouble* params);
SIGL_FUNC_DECLARE(GLenum, glGetError, void);
SIGL_FUNC_DECLARE(void, glGetFloatv, GLenum pname, GLfloat* params);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glGetHistogram, GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid* values);
SIGL_FUNC_DECLARE(void, glGetHistogramParameterfv, GLenum target, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetHistogramParameteriv, GLenum target, GLenum pname, GLint* params);
#endif

SIGL_FUNC_DECLARE(void, glGetIntegerv, GLenum pname, GLint* params);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glGetLightfv, GLenum light, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetLightiv, GLenum light, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetMapdv, GLenum target, GLenum query, GLdouble* v);
SIGL_FUNC_DECLARE(void, glGetMapfv, GLenum target, GLenum query, GLfloat* v);
SIGL_FUNC_DECLARE(void, glGetMapiv, GLenum target, GLenum query, GLint* v);
SIGL_FUNC_DECLARE(void, glGetMaterialfv, GLenum face, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetMaterialiv, GLenum face, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetMinmax, GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid* values);
SIGL_FUNC_DECLARE(void, glGetMinmaxParameterfv, GLenum target, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetMinmaxParameteriv, GLenum target, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetPixelMapfv, GLenum map, GLfloat* values);
SIGL_FUNC_DECLARE(void, glGetPixelMapuiv, GLuint* values);
SIGL_FUNC_DECLARE(void, glGetPixelMapusv, GLushort* values);
SIGL_FUNC_DECLARE(void, glGetPointerv, GLenum pname, GLvoid** params);
SIGL_FUNC_DECLARE(void, glGetPolygonStipple, GLubyte* mask);
SIGL_FUNC_DECLARE(void, glGetSeparableFilter, GLenum target, GLenum format, GLenum type, GLvoid* row, GLvoid* column, GLvoid* span);
#endif

SIGL_FUNC_DECLARE(const GLubyte*, glGetString, GLenum name);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glGetTexEnvfv, GLenum target, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetTexEnviv, GLenum target, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetTexGendv, GLenum coord, GLenum pname, GLdouble* params);
SIGL_FUNC_DECLARE(void, glGetTexGenfv, GLenum coord, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetTexGeniv, GLenum coord, GLenum pname, GLint* params);
#endif

SIGL_FUNC_DECLARE(void, glGetTexImage, GLenum target, GLint level, GLenum format, GLenum type, GLvoid* pixels);
SIGL_FUNC_DECLARE(void, glGetTexLevelParameterfv, GLenum target, GLint level, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetTexLevelParameteriv, GLenum target, GLint level, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetTexParameterfv, GLenum target, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetTexParameteriv, GLenum target, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glHint, GLenum target, GLenum mode);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glHistogram, GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
SIGL_FUNC_DECLARE(void, glIndexMask, GLuint mask);
SIGL_FUNC_DECLARE(void, glIndexPointer, GLenum type, GLsizei stride, const GLvoid* pointer);
SIGL_FUNC_DECLARE(void, glIndexd, GLdouble c);
SIGL_FUNC_DECLARE(void, glIndexdv, const GLdouble* c);
SIGL_FUNC_DECLARE(void, glIndexf, GLfloat c);
SIGL_FUNC_DECLARE(void, glIndexfv, const GLfloat* c);
SIGL_FUNC_DECLARE(void, glIndexi, GLint c);
SIGL_FUNC_DECLARE(void, glIndexiv, const GLint* c);
SIGL_FUNC_DECLARE(void, glIndexs, GLshort c);
SIGL_FUNC_DECLARE(void, glIndexsv, const GLshort* c);
SIGL_FUNC_DECLARE(void, glIndexub, GLubyte c);
SIGL_FUNC_DECLARE(void, glIndexubv, const GLubyte* c);
SIGL_FUNC_DECLARE(void, glInitNames, void);
SIGL_FUNC_DECLARE(void, glInterleavedArrays, GLenum format, GLsizei stride, const GLvoid* pointer);
#endif

SIGL_FUNC_DECLARE(GLboolean, glIsEnabled, GLenum cap);
SIGL_FUNC_DECLARE(GLboolean, glIsTexture, GLuint texture);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(GLboolean, glIsList, GLuint list);
SIGL_FUNC_DECLARE(void, glLightModelf, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glLightModelfv, GLenum pname, const GLfloat* params);
SIGL_FUNC_DECLARE(void, glLightModeli, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glLightModeliv, GLenum pname, const GLint* params);
SIGL_FUNC_DECLARE(void, glLightf, GLenum light, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glLightfv, GLenum light, GLenum pname, const GLfloat* params);
SIGL_FUNC_DECLARE(void, glLighti, GLenum light, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glLightiv, GLenum light, GLenum pname, const GLint* params);
SIGL_FUNC_DECLARE(void, glLineStipple, GLint factor, GLushort pattern);
#endif

SIGL_FUNC_DECLARE(void, glLineWidth, GLfloat width);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glListBase, GLuint base);
SIGL_FUNC_DECLARE(void, glLoadIdentity, void);
SIGL_FUNC_DECLARE(void, glLoadMatrixd, const GLdouble* m);
SIGL_FUNC_DECLARE(void, glLoadMatrixf, const GLfloat* m);
SIGL_FUNC_DECLARE(void, glLoadName, GLuint name);
#endif

SIGL_FUNC_DECLARE(void, glLogicOp, GLenum opcode);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glMap1d, GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble* points);
SIGL_FUNC_DECLARE(void, glMap1f, GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat* points);
SIGL_FUNC_DECLARE(void, glMap2d, GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble* points);
SIGL_FUNC_DECLARE(void, glMap2f, GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat* points);
SIGL_FUNC_DECLARE(void, glMapGrid1d, GLint un, GLdouble u1, GLdouble u2);
SIGL_FUNC_DECLARE(void, glMapGrid1f, GLint un, GLfloat u1, GLfloat u2);
SIGL_FUNC_DECLARE(void, glMapGrid2d, GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
SIGL_FUNC_DECLARE(void, glMapGrid2f, GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
SIGL_FUNC_DECLARE(void, glMaterialf, GLenum face, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glMaterialfv, GLenum face, GLenum pname, const GLfloat* params);
SIGL_FUNC_DECLARE(void, glMateriali, GLenum face, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glMaterialiv, GLenum face, GLenum pname, const GLint* params);
SIGL_FUNC_DECLARE(void, glMatrixMode, GLenum mode);
SIGL_FUNC_DECLARE(void, glMultMatrixd, const GLdouble* m);
SIGL_FUNC_DECLARE(void, glMultMatrixf, const GLfloat* m);
SIGL_FUNC_DECLARE(void, glNewList, GLuint list, GLenum mode);
SIGL_FUNC_DECLARE(void, glNormal3b, GLbyte nx, GLbyte ny, GLbyte nz);
SIGL_FUNC_DECLARE(void, glNormal3bv, const GLbyte* v);
SIGL_FUNC_DECLARE(void, glNormal3d, GLdouble nx, GLdouble ny, GLdouble nz);
SIGL_FUNC_DECLARE(void, glNormal3dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glNormal3f, GLfloat nx, GLfloat ny, GLfloat nz);
SIGL_FUNC_DECLARE(void, glNormal3fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glNormal3i, GLint nx, GLint ny, GLint nz);
SIGL_FUNC_DECLARE(void, glNormal3iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glNormal3s, GLshort nx, GLshort ny, GLshort nz);
SIGL_FUNC_DECLARE(void, glNormal3sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glNormalPointer, GLenum type, GLsizei stride, const GLvoid* pointer);
SIGL_FUNC_DECLARE(void, glOrtho, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
SIGL_FUNC_DECLARE(void, glPassThrough, GLfloat token);
SIGL_FUNC_DECLARE(void, glPixelMapfv, GLenum map, GLsizei mapsize, const GLfloat* values);
SIGL_FUNC_DECLARE(void, glPixelMapuiv, GLenum map, GLsizei mapsize, const GLuint* values);
SIGL_FUNC_DECLARE(void, glPixelMapusv, GLenum map, GLsizei mapsize, const GLushort* values);
#endif

SIGL_FUNC_DECLARE(void, glPixelStoref, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glPixelStorei, GLenum pname, GLint param);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glPixelTransferf, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glPixelTransferi, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glPixelZoom, GLfloat xfactor, GLfloat yfactor);
#endif

SIGL_FUNC_DECLARE(void, glPointSize, GLfloat size);
SIGL_FUNC_DECLARE(void, glPolygonMode, GLenum face, GLenum mode);
SIGL_FUNC_DECLARE(void, glPolygonOffset, GLfloat factor, GLfloat units);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glPolygonStipple, const GLubyte* mask);
SIGL_FUNC_DECLARE(void, glPopAttrib, void);
SIGL_FUNC_DECLARE(void, glPopClientAttrib, void);
SIGL_FUNC_DECLARE(void, glPopMatrix, void);
SIGL_FUNC_DECLARE(void, glPopName, void);
SIGL_FUNC_DECLARE(void, glPrioritizeTextures, GLsizei n, const GLuint* textures, const GLclampf* priorities);
SIGL_FUNC_DECLARE(void, glPushAttrib, GLbitfield mask);
SIGL_FUNC_DECLARE(void, glPushClientAttrib, GLbitfield mask);
SIGL_FUNC_DECLARE(void, glPushMatrix, void);
SIGL_FUNC_DECLARE(void, glPushName, GLuint name);
SIGL_FUNC_DECLARE(void, glRasterPos2d, GLdouble x, GLdouble y);
SIGL_FUNC_DECLARE(void, glRasterPos2dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glRasterPos2f, GLfloat x, GLfloat y);
SIGL_FUNC_DECLARE(void, glRasterPos2fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glRasterPos2i, GLint x, GLint y);
SIGL_FUNC_DECLARE(void, glRasterPos2iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glRasterPos2s, GLshort x, GLshort y);
SIGL_FUNC_DECLARE(void, glRasterPos2sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glRasterPos3d, GLdouble x, GLdouble y, GLdouble z);
SIGL_FUNC_DECLARE(void, glRasterPos3dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glRasterPos3f, GLfloat x, GLfloat y, GLfloat z);
SIGL_FUNC_DECLARE(void, glRasterPos3fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glRasterPos3i, GLint x, GLint y, GLint z);
SIGL_FUNC_DECLARE(void, glRasterPos3iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glRasterPos3s, GLshort x, GLshort y, GLshort z);
SIGL_FUNC_DECLARE(void, glRasterPos3sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glRasterPos4d, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
SIGL_FUNC_DECLARE(void, glRasterPos4dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glRasterPos4f, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
SIGL_FUNC_DECLARE(void, glRasterPos4fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glRasterPos4i, GLint x, GLint y, GLint z, GLint w);
SIGL_FUNC_DECLARE(void, glRasterPos4iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glRasterPos4s, GLshort x, GLshort y, GLshort z, GLshort w);
SIGL_FUNC_DECLARE(void, glRasterPos4sv, const GLshort* v);
#endif

SIGL_FUNC_DECLARE(void, glReadBuffer, GLenum mode);
SIGL_FUNC_DECLARE(void, glReadPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glRectd, GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
SIGL_FUNC_DECLARE(void, glRectdv, const GLdouble* v1, const GLdouble* v2);
SIGL_FUNC_DECLARE(void, glRectf, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
SIGL_FUNC_DECLARE(void, glRectfv, const GLfloat* v1, const GLfloat* v2);
SIGL_FUNC_DECLARE(void, glRecti, GLint x1, GLint y1, GLint x2, GLint y2);
SIGL_FUNC_DECLARE(void, glRectiv, const GLint* v1, const GLint* v2);
SIGL_FUNC_DECLARE(void, glRects, GLshort x1, GLshort y1, GLshort x2, GLshort y2);
SIGL_FUNC_DECLARE(void, glRectsv, const GLshort* v1, const GLshort* v2);
SIGL_FUNC_DECLARE(GLint, glRenderMode, GLenum mode);
SIGL_FUNC_DECLARE(void, glRotated, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
SIGL_FUNC_DECLARE(void, glRotatef, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
SIGL_FUNC_DECLARE(void, glScaled, GLdouble x, GLdouble y, GLdouble z);
SIGL_FUNC_DECLARE(void, glScalef, GLfloat x, GLfloat y, GLfloat z);
#endif

SIGL_FUNC_DECLARE(void, glScissor, GLint x, GLint y, GLsizei width, GLsizei height);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glSelectBuffer, GLsizei size, GLuint* buffer);
SIGL_FUNC_DECLARE(void, glShadeModel, GLenum mode);
#endif

SIGL_FUNC_DECLARE(void, glStencilFunc, GLenum func, GLint ref, GLuint mask);
SIGL_FUNC_DECLARE(void, glStencilMask, GLuint mask);
SIGL_FUNC_DECLARE(void, glStencilOp, GLenum fail, GLenum zfail, GLenum zpass);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glTexCoord1d, GLdouble s);
SIGL_FUNC_DECLARE(void, glTexCoord1dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glTexCoord1f, GLfloat s);
SIGL_FUNC_DECLARE(void, glTexCoord1fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glTexCoord1i, GLint s);
SIGL_FUNC_DECLARE(void, glTexCoord1iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glTexCoord1s, GLshort s);
SIGL_FUNC_DECLARE(void, glTexCoord1sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glTexCoord2d, GLdouble s, GLdouble t);
SIGL_FUNC_DECLARE(void, glTexCoord2dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glTexCoord2f, GLfloat s, GLfloat t);
SIGL_FUNC_DECLARE(void, glTexCoord2fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glTexCoord2i, GLint s, GLint t);
SIGL_FUNC_DECLARE(void, glTexCoord2iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glTexCoord2s, GLshort s, GLshort t);
SIGL_FUNC_DECLARE(void, glTexCoord2sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glTexCoord3d, GLdouble s, GLdouble t, GLdouble r);
SIGL_FUNC_DECLARE(void, glTexCoord3dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glTexCoord3f, GLfloat s, GLfloat t, GLfloat r);
SIGL_FUNC_DECLARE(void, glTexCoord3fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glTexCoord3i, GLint s, GLint t, GLint r);
SIGL_FUNC_DECLARE(void, glTexCoord3iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glTexCoord3s, GLshort s, GLshort t, GLshort r);
SIGL_FUNC_DECLARE(void, glTexCoord3sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glTexCoord4d, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
SIGL_FUNC_DECLARE(void, glTexCoord4dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glTexCoord4f, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
SIGL_FUNC_DECLARE(void, glTexCoord4fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glTexCoord4i, GLint s, GLint t, GLint r, GLint q);
SIGL_FUNC_DECLARE(void, glTexCoord4iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glTexCoord4s, GLshort s, GLshort t, GLshort r, GLshort q);
SIGL_FUNC_DECLARE(void, glTexCoord4sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glTexCoordPointer, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
SIGL_FUNC_DECLARE(void, glTexEnvf, GLenum target, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glTexEnvfv, GLenum target, GLenum pname, const GLfloat* params);
SIGL_FUNC_DECLARE(void, glTexEnvi, GLenum target, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glTexEnviv, GLenum target, GLenum pname, const GLint* params);
SIGL_FUNC_DECLARE(void, glTexGend, GLenum coord, GLenum pname, GLdouble param);
SIGL_FUNC_DECLARE(void, glTexGendv, GLenum coord, GLenum pname, const GLdouble* params);
SIGL_FUNC_DECLARE(void, glTexGenf, GLenum coord, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glTexGenfv, GLenum coord, GLenum pname, const GLfloat* params);
SIGL_FUNC_DECLARE(void, glTexGeni, GLenum coord, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glTexGeniv, GLenum coord, GLenum pname, const GLint* params);
#endif

SIGL_FUNC_DECLARE(void, glTexImage1D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
SIGL_FUNC_DECLARE(void, glTexImage2D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
SIGL_FUNC_DECLARE(void, glTexParameterf, GLenum target, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glTexParameterfv, GLenum target, GLenum pname, const GLfloat* params);
SIGL_FUNC_DECLARE(void, glTexParameteri, GLenum target, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glTexParameteriv, GLenum target, GLenum pname, const GLint* params);
SIGL_FUNC_DECLARE(void, glTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels);
SIGL_FUNC_DECLARE(void, glTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);

#if defined(SIGL_INCLUDE_DEPRECATED_GL_1_1_FUNCTIONS)
SIGL_FUNC_DECLARE(void, glVertex2d, GLdouble x, GLdouble y);
SIGL_FUNC_DECLARE(void, glVertex2dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertex2f, GLfloat x, GLfloat y);
SIGL_FUNC_DECLARE(void, glVertex2fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glVertex2i, GLint x, GLint y);
SIGL_FUNC_DECLARE(void, glVertex2iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glVertex2s, GLshort x, GLshort y);
SIGL_FUNC_DECLARE(void, glVertex2sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glVertex3d, GLdouble x, GLdouble y, GLdouble z);
SIGL_FUNC_DECLARE(void, glVertex3dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertex3f, GLfloat x, GLfloat y, GLfloat z);
SIGL_FUNC_DECLARE(void, glVertex3fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glVertex3i, GLint x, GLint y, GLint z);
SIGL_FUNC_DECLARE(void, glVertex3iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glVertex3s, GLshort x, GLshort y, GLshort z);
SIGL_FUNC_DECLARE(void, glVertex3sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glVertex4d, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
SIGL_FUNC_DECLARE(void, glVertex4dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertex4f, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
SIGL_FUNC_DECLARE(void, glVertex4fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glVertex4i, GLint x, GLint y, GLint z, GLint w);
SIGL_FUNC_DECLARE(void, glVertex4iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glVertex4s, GLshort x, GLshort y, GLshort z, GLshort w);
SIGL_FUNC_DECLARE(void, glVertex4sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glVertexPointer, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
#endif

SIGL_FUNC_DECLARE(void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height);
#undef SIGL_FUNC_DECLARE
#endif

#if SIGL_SYSTEM_IS_WINDOWS || SIGL_SYSTEM_IS_UNIX
	#define SIGL_FUNC_DECLARE(retType, name, ...) typedef retType(GLAPIENTRY* name##SIPROC) (__VA_ARGS__); extern name##SIPROC name
#else
	#define SIGL_FUNC_DECLARE(retType, name, ...) extern retType name(__VA_ARGS__)
#endif

#if !defined(SIGL_NO_GL_1_2_H_FUNCS)
SIGL_FUNC_DECLARE(void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices);
SIGL_FUNC_DECLARE(void, glTexImage3D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
SIGL_FUNC_DECLARE(void, glTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
SIGL_FUNC_DECLARE(void, glCopyTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
#endif /* SIGL_NO_GL_1_2_H_FUNCS */
#if !defined(SIGL_NO_GL_1_3_H_FUNCS)
SIGL_FUNC_DECLARE(void, glActiveTexture, GLenum texture);
SIGL_FUNC_DECLARE(void, glSampleCoverage, GLfloat value, GLboolean invert);
SIGL_FUNC_DECLARE(void, glCompressedTexImage3D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
SIGL_FUNC_DECLARE(void, glCompressedTexImage2D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
SIGL_FUNC_DECLARE(void, glCompressedTexImage1D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
SIGL_FUNC_DECLARE(void, glCompressedTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
SIGL_FUNC_DECLARE(void, glCompressedTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
SIGL_FUNC_DECLARE(void, glCompressedTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
SIGL_FUNC_DECLARE(void, glGetCompressedTexImage, GLenum target, GLint level, void* img);
SIGL_FUNC_DECLARE(void, glClientActiveTexture, GLenum texture);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1d, GLenum target, GLdouble s);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1dv, GLenum target, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1f, GLenum target, GLfloat s);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1fv, GLenum target, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1i, GLenum target, GLint s);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1iv, GLenum target, const GLint* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1s, GLenum target, GLshort s);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1sv, GLenum target, const GLshort* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2d, GLenum target, GLdouble s, GLdouble t);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2dv, GLenum target, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2f, GLenum target, GLfloat s, GLfloat t);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2fv, GLenum target, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2i, GLenum target, GLint s, GLint t);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2iv, GLenum target, const GLint* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2s, GLenum target, GLshort s, GLshort t);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2sv, GLenum target, const GLshort* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3d, GLenum target, GLdouble s, GLdouble t, GLdouble r);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3dv, GLenum target, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3f, GLenum target, GLfloat s, GLfloat t, GLfloat r);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3fv, GLenum target, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3i, GLenum target, GLint s, GLint t, GLint r);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3iv, GLenum target, const GLint* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3s, GLenum target, GLshort s, GLshort t, GLshort r);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3sv, GLenum target, const GLshort* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4d, GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4dv, GLenum target, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4f, GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4fv, GLenum target, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4i, GLenum target, GLint s, GLint t, GLint r, GLint q);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4iv, GLenum target, const GLint* v);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4s, GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4sv, GLenum target, const GLshort* v);
SIGL_FUNC_DECLARE(void, glLoadTransposeMatrixf, const GLfloat* m);
SIGL_FUNC_DECLARE(void, glLoadTransposeMatrixd, const GLdouble* m);
SIGL_FUNC_DECLARE(void, glMultTransposeMatrixf, const GLfloat* m);
SIGL_FUNC_DECLARE(void, glMultTransposeMatrixd, const GLdouble* m);
#endif /* SIGL_NO_GL_1_3_H_FUNCS */
#if !defined(SIGL_NO_GL_1_4_H_FUNCS)
SIGL_FUNC_DECLARE(void, glBlendFuncSeparate, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
SIGL_FUNC_DECLARE(void, glMultiDrawArrays, GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawcount);
SIGL_FUNC_DECLARE(void, glMultiDrawElements, GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount);
SIGL_FUNC_DECLARE(void, glPointParameterf, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glPointParameterfv, GLenum pname, const GLfloat* params);
SIGL_FUNC_DECLARE(void, glPointParameteri, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glPointParameteriv, GLenum pname, const GLint* params);
SIGL_FUNC_DECLARE(void, glFogCoordf, GLfloat coord);
SIGL_FUNC_DECLARE(void, glFogCoordfv, const GLfloat* coord);
SIGL_FUNC_DECLARE(void, glFogCoordd, GLdouble coord);
SIGL_FUNC_DECLARE(void, glFogCoorddv, const GLdouble* coord);
SIGL_FUNC_DECLARE(void, glFogCoordPointer, GLenum type, GLsizei stride, const void* pointer);
SIGL_FUNC_DECLARE(void, glSecondaryColor3b, GLbyte red, GLbyte green, GLbyte blue);
SIGL_FUNC_DECLARE(void, glSecondaryColor3bv, const GLbyte* v);
SIGL_FUNC_DECLARE(void, glSecondaryColor3d, GLdouble red, GLdouble green, GLdouble blue);
SIGL_FUNC_DECLARE(void, glSecondaryColor3dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glSecondaryColor3f, GLfloat red, GLfloat green, GLfloat blue);
SIGL_FUNC_DECLARE(void, glSecondaryColor3fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glSecondaryColor3i, GLint red, GLint green, GLint blue);
SIGL_FUNC_DECLARE(void, glSecondaryColor3iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glSecondaryColor3s, GLshort red, GLshort green, GLshort blue);
SIGL_FUNC_DECLARE(void, glSecondaryColor3sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glSecondaryColor3ub, GLubyte red, GLubyte green, GLubyte blue);
SIGL_FUNC_DECLARE(void, glSecondaryColor3ubv, const GLubyte* v);
SIGL_FUNC_DECLARE(void, glSecondaryColor3ui, GLuint red, GLuint green, GLuint blue);
SIGL_FUNC_DECLARE(void, glSecondaryColor3uiv, const GLuint* v);
SIGL_FUNC_DECLARE(void, glSecondaryColor3us, GLushort red, GLushort green, GLushort blue);
SIGL_FUNC_DECLARE(void, glSecondaryColor3usv, const GLushort* v);
SIGL_FUNC_DECLARE(void, glSecondaryColorPointer, GLint size, GLenum type, GLsizei stride, const void* pointer);
SIGL_FUNC_DECLARE(void, glWindowPos2d, GLdouble x, GLdouble y);
SIGL_FUNC_DECLARE(void, glWindowPos2dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glWindowPos2f, GLfloat x, GLfloat y);
SIGL_FUNC_DECLARE(void, glWindowPos2fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glWindowPos2i, GLint x, GLint y);
SIGL_FUNC_DECLARE(void, glWindowPos2iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glWindowPos2s, GLshort x, GLshort y);
SIGL_FUNC_DECLARE(void, glWindowPos2sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glWindowPos3d, GLdouble x, GLdouble y, GLdouble z);
SIGL_FUNC_DECLARE(void, glWindowPos3dv, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glWindowPos3f, GLfloat x, GLfloat y, GLfloat z);
SIGL_FUNC_DECLARE(void, glWindowPos3fv, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glWindowPos3i, GLint x, GLint y, GLint z);
SIGL_FUNC_DECLARE(void, glWindowPos3iv, const GLint* v);
SIGL_FUNC_DECLARE(void, glWindowPos3s, GLshort x, GLshort y, GLshort z);
SIGL_FUNC_DECLARE(void, glWindowPos3sv, const GLshort* v);
SIGL_FUNC_DECLARE(void, glBlendColor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
SIGL_FUNC_DECLARE(void, glBlendEquation, GLenum mode);
#endif
#if !defined(SIGL_NO_GL_1_5_H_FUNCS)
SIGL_FUNC_DECLARE(void, glGenQueries, GLsizei n, GLuint* ids);
SIGL_FUNC_DECLARE(void, glDeleteQueries, GLsizei n, const GLuint* ids);
SIGL_FUNC_DECLARE(GLboolean, glIsQuery, GLuint id);
SIGL_FUNC_DECLARE(void, glBeginQuery, GLenum target, GLuint id);
SIGL_FUNC_DECLARE(void, glEndQuery, GLenum target);
SIGL_FUNC_DECLARE(void, glGetQueryiv, GLenum target, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetQueryObjectiv, GLuint id, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint* params);
SIGL_FUNC_DECLARE(void, glBindBuffer, GLenum target, GLuint buffer);
SIGL_FUNC_DECLARE(void, glDeleteBuffers, GLsizei n, const GLuint* buffers);
SIGL_FUNC_DECLARE(void, glGenBuffers, GLsizei n, GLuint* buffers);
SIGL_FUNC_DECLARE(GLboolean, glIsBuffer, GLuint buffer);
SIGL_FUNC_DECLARE(void, glBufferData, GLenum target, GLsizeiptr size, const void* data, GLenum usage);
SIGL_FUNC_DECLARE(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
SIGL_FUNC_DECLARE(void, glGetBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, void* data);
SIGL_FUNC_DECLARE(void*, glMapBuffer, GLenum target, GLenum access);
SIGL_FUNC_DECLARE(GLboolean, glUnmapBuffer, GLenum target);
SIGL_FUNC_DECLARE(void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetBufferPointerv, GLenum target, GLenum pname, void** params);
#endif /* !SIGL_NO_GL_1_5_H_FUNCS */
#if !defined(SIGL_NO_GL_2_0_H_FUNCS)
SIGL_FUNC_DECLARE(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha);
SIGL_FUNC_DECLARE(void, glDrawBuffers, GLsizei n, const GLenum* bufs);
SIGL_FUNC_DECLARE(void, glStencilOpSeparate, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
SIGL_FUNC_DECLARE(void, glStencilFuncSeparate, GLenum face, GLenum func, GLint ref, GLuint mask);
SIGL_FUNC_DECLARE(void, glStencilMaskSeparate, GLenum face, GLuint mask);
SIGL_FUNC_DECLARE(void, glAttachShader, GLuint program, GLuint shader);
SIGL_FUNC_DECLARE(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar* name);
SIGL_FUNC_DECLARE(void, glCompileShader, GLuint shader);
SIGL_FUNC_DECLARE(GLuint, glCreateProgram, void);
SIGL_FUNC_DECLARE(GLuint, glCreateShader, GLenum type);
SIGL_FUNC_DECLARE(void, glDeleteProgram, GLuint program);
SIGL_FUNC_DECLARE(void, glDeleteShader, GLuint shader);
SIGL_FUNC_DECLARE(void, glDetachShader, GLuint program, GLuint shader);
SIGL_FUNC_DECLARE(void, glDisableVertexAttribArray, GLuint index);
SIGL_FUNC_DECLARE(void, glEnableVertexAttribArray, GLuint index);
SIGL_FUNC_DECLARE(void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
SIGL_FUNC_DECLARE(void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
SIGL_FUNC_DECLARE(void, glGetAttachedShaders, GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
SIGL_FUNC_DECLARE(GLint, glGetAttribLocation, GLuint program, const GLchar* name);
SIGL_FUNC_DECLARE(void, glGetProgramiv, GLuint program, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
SIGL_FUNC_DECLARE(void, glGetShaderiv, GLuint shader, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
SIGL_FUNC_DECLARE(void, glGetShaderSource, GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source);
SIGL_FUNC_DECLARE(GLint, glGetUniformLocation, GLuint program, const GLchar* name);
SIGL_FUNC_DECLARE(void, glGetUniformfv, GLuint program, GLint location, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetUniformiv, GLuint program, GLint location, GLint* params);
SIGL_FUNC_DECLARE(void, glGetVertexAttribdv, GLuint index, GLenum pname, GLdouble* params);
SIGL_FUNC_DECLARE(void, glGetVertexAttribfv, GLuint index, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetVertexAttribiv, GLuint index, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetVertexAttribPointerv, GLuint index, GLenum pname, void** pointer);
SIGL_FUNC_DECLARE(GLboolean, glIsProgram, GLuint program);
SIGL_FUNC_DECLARE(GLboolean, glIsShader, GLuint shader);
SIGL_FUNC_DECLARE(void, glLinkProgram, GLuint program);
SIGL_FUNC_DECLARE(void, glShaderSource, GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
SIGL_FUNC_DECLARE(void, glUseProgram, GLuint program);
SIGL_FUNC_DECLARE(void, glUniform1f, GLint location, GLfloat v0);
SIGL_FUNC_DECLARE(void, glUniform2f, GLint location, GLfloat v0, GLfloat v1);
SIGL_FUNC_DECLARE(void, glUniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
SIGL_FUNC_DECLARE(void, glUniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
SIGL_FUNC_DECLARE(void, glUniform1i, GLint location, GLint v0);
SIGL_FUNC_DECLARE(void, glUniform2i, GLint location, GLint v0, GLint v1);
SIGL_FUNC_DECLARE(void, glUniform3i, GLint location, GLint v0, GLint v1, GLint v2);
SIGL_FUNC_DECLARE(void, glUniform4i, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
SIGL_FUNC_DECLARE(void, glUniform1fv, GLint location, GLsizei count, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniform2fv, GLint location, GLsizei count, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniform3fv, GLint location, GLsizei count, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniform4fv, GLint location, GLsizei count, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniform1iv, GLint location, GLsizei count, const GLint* value);
SIGL_FUNC_DECLARE(void, glUniform2iv, GLint location, GLsizei count, const GLint* value);
SIGL_FUNC_DECLARE(void, glUniform3iv, GLint location, GLsizei count, const GLint* value);
SIGL_FUNC_DECLARE(void, glUniform4iv, GLint location, GLsizei count, const GLint* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glValidateProgram, GLuint program);
SIGL_FUNC_DECLARE(void, glVertexAttrib1d, GLuint index, GLdouble x);
SIGL_FUNC_DECLARE(void, glVertexAttrib1dv, GLuint index, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib1f, GLuint index, GLfloat x);
SIGL_FUNC_DECLARE(void, glVertexAttrib1fv, GLuint index, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib1s, GLuint index, GLshort x);
SIGL_FUNC_DECLARE(void, glVertexAttrib1sv, GLuint index, const GLshort* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib2d, GLuint index, GLdouble x, GLdouble y);
SIGL_FUNC_DECLARE(void, glVertexAttrib2dv, GLuint index, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib2f, GLuint index, GLfloat x, GLfloat y);
SIGL_FUNC_DECLARE(void, glVertexAttrib2fv, GLuint index, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib2s, GLuint index, GLshort x, GLshort y);
SIGL_FUNC_DECLARE(void, glVertexAttrib2sv, GLuint index, const GLshort* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib3d, GLuint index, GLdouble x, GLdouble y, GLdouble z);
SIGL_FUNC_DECLARE(void, glVertexAttrib3dv, GLuint index, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib3f, GLuint index, GLfloat x, GLfloat y, GLfloat z);
SIGL_FUNC_DECLARE(void, glVertexAttrib3fv, GLuint index, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib3s, GLuint index, GLshort x, GLshort y, GLshort z);
SIGL_FUNC_DECLARE(void, glVertexAttrib3sv, GLuint index, const GLshort* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nbv, GLuint index, const GLbyte* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Niv, GLuint index, const GLint* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nsv, GLuint index, const GLshort* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nub, GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nubv, GLuint index, const GLubyte* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nuiv, GLuint index, const GLuint* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nusv, GLuint index, const GLushort* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4bv, GLuint index, const GLbyte* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
SIGL_FUNC_DECLARE(void, glVertexAttrib4dv, GLuint index, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4f, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
SIGL_FUNC_DECLARE(void, glVertexAttrib4fv, GLuint index, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4iv, GLuint index, const GLint* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4s, GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
SIGL_FUNC_DECLARE(void, glVertexAttrib4sv, GLuint index, const GLshort* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4ubv, GLuint index, const GLubyte* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4uiv, GLuint index, const GLuint* v);
SIGL_FUNC_DECLARE(void, glVertexAttrib4usv, GLuint index, const GLushort* v);
SIGL_FUNC_DECLARE(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
#endif /* !SIGL_NO_GL_2_0_H_FUNCS */
#if !defined(SIGL_NO_GL_2_1_H_FUNCS)
SIGL_FUNC_DECLARE(void, glUniformMatrix2x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix3x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix2x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix4x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix3x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix4x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif /* !SIGL_NO_GL_2_1_H_FUNCS */
#if !defined(SIGL_NO_GL_3_0_H_FUNCS)
SIGL_FUNC_DECLARE(void, glColorMaski, GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
SIGL_FUNC_DECLARE(void, glGetBooleani_v, GLenum target, GLuint index, GLboolean* data);
SIGL_FUNC_DECLARE(void, glGetIntegeri_v, GLenum target, GLuint index, GLint* data);
SIGL_FUNC_DECLARE(void, glEnablei, GLenum target, GLuint index);
SIGL_FUNC_DECLARE(void, glDisablei, GLenum target, GLuint index);
SIGL_FUNC_DECLARE(GLboolean, glIsEnabledi, GLenum target, GLuint index);
SIGL_FUNC_DECLARE(void, glBeginTransformFeedback, GLenum primitiveMode);
SIGL_FUNC_DECLARE(void, glEndTransformFeedback, void);
SIGL_FUNC_DECLARE(void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
SIGL_FUNC_DECLARE(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer);
SIGL_FUNC_DECLARE(void, glTransformFeedbackVaryings, GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
SIGL_FUNC_DECLARE(void, glGetTransformFeedbackVarying, GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
SIGL_FUNC_DECLARE(void, glClampColor, GLenum target, GLenum clamp);
SIGL_FUNC_DECLARE(void, glBeginConditionalRender, GLuint id, GLenum mode);
SIGL_FUNC_DECLARE(void, glEndConditionalRender, void);
SIGL_FUNC_DECLARE(void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
SIGL_FUNC_DECLARE(void, glGetVertexAttribIiv, GLuint index, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetVertexAttribIuiv, GLuint index, GLenum pname, GLuint* params);
SIGL_FUNC_DECLARE(void, glVertexAttribI1i, GLuint index, GLint x);
SIGL_FUNC_DECLARE(void, glVertexAttribI2i, GLuint index, GLint x, GLint y);
SIGL_FUNC_DECLARE(void, glVertexAttribI3i, GLuint index, GLint x, GLint y, GLint z);
SIGL_FUNC_DECLARE(void, glVertexAttribI4i, GLuint index, GLint x, GLint y, GLint z, GLint w);
SIGL_FUNC_DECLARE(void, glVertexAttribI1ui, GLuint index, GLuint x);
SIGL_FUNC_DECLARE(void, glVertexAttribI2ui, GLuint index, GLuint x, GLuint y);
SIGL_FUNC_DECLARE(void, glVertexAttribI3ui, GLuint index, GLuint x, GLuint y, GLuint z);
SIGL_FUNC_DECLARE(void, glVertexAttribI4ui, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
SIGL_FUNC_DECLARE(void, glVertexAttribI1iv, GLuint index, const GLint* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI2iv, GLuint index, const GLint* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI3iv, GLuint index, const GLint* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI4iv, GLuint index, const GLint* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI1uiv, GLuint index, const GLuint* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI2uiv, GLuint index, const GLuint* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI3uiv, GLuint index, const GLuint* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI4uiv, GLuint index, const GLuint* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI4bv, GLuint index, const GLbyte* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI4sv, GLuint index, const GLshort* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI4ubv, GLuint index, const GLubyte* v);
SIGL_FUNC_DECLARE(void, glVertexAttribI4usv, GLuint index, const GLushort* v);
SIGL_FUNC_DECLARE(void, glGetUniformuiv, GLuint program, GLint location, GLuint* params);
SIGL_FUNC_DECLARE(void, glBindFragDataLocation, GLuint program, GLuint color, const GLchar* name);
SIGL_FUNC_DECLARE(GLint, glGetFragDataLocation, GLuint program, const GLchar* name);
SIGL_FUNC_DECLARE(void, glUniform1ui, GLint location, GLuint v0);
SIGL_FUNC_DECLARE(void, glUniform2ui, GLint location, GLuint v0, GLuint v1);
SIGL_FUNC_DECLARE(void, glUniform3ui, GLint location, GLuint v0, GLuint v1, GLuint v2);
SIGL_FUNC_DECLARE(void, glUniform4ui, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
SIGL_FUNC_DECLARE(void, glUniform1uiv, GLint location, GLsizei count, const GLuint* value);
SIGL_FUNC_DECLARE(void, glUniform2uiv, GLint location, GLsizei count, const GLuint* value);
SIGL_FUNC_DECLARE(void, glUniform3uiv, GLint location, GLsizei count, const GLuint* value);
SIGL_FUNC_DECLARE(void, glUniform4uiv, GLint location, GLsizei count, const GLuint* value);
SIGL_FUNC_DECLARE(void, glTexParameterIiv, GLenum target, GLenum pname, const GLint* params);
SIGL_FUNC_DECLARE(void, glTexParameterIuiv, GLenum target, GLenum pname, const GLuint* params);
SIGL_FUNC_DECLARE(void, glGetTexParameterIiv, GLenum target, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetTexParameterIuiv, GLenum target, GLenum pname, GLuint* params);
SIGL_FUNC_DECLARE(void, glClearBufferiv, GLenum buffer, GLint drawbuffer, const GLint* value);
SIGL_FUNC_DECLARE(void, glClearBufferuiv, GLenum buffer, GLint drawbuffer, const GLuint* value);
SIGL_FUNC_DECLARE(void, glClearBufferfv, GLenum buffer, GLint drawbuffer, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glClearBufferfi, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
SIGL_FUNC_DECLARE(const GLubyte*, glGetStringi, GLenum name, GLuint index);
SIGL_FUNC_DECLARE(GLboolean, glIsRenderbuffer, GLuint renderbuffer);
SIGL_FUNC_DECLARE(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer);
SIGL_FUNC_DECLARE(void, glDeleteRenderbuffers, GLsizei n, const GLuint* renderbuffers);
SIGL_FUNC_DECLARE(void, glGenRenderbuffers, GLsizei n, GLuint* renderbuffers);
SIGL_FUNC_DECLARE(void, glRenderbufferStorage, GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glGetRenderbufferParameteriv, GLenum target, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(GLboolean, glIsFramebuffer, GLuint framebuffer);
SIGL_FUNC_DECLARE(void, glBindFramebuffer, GLenum target, GLuint framebuffer);
SIGL_FUNC_DECLARE(void, glDeleteFramebuffers, GLsizei n, const GLuint* framebuffers);
SIGL_FUNC_DECLARE(void, glGenFramebuffers, GLsizei n, GLuint* framebuffers);
SIGL_FUNC_DECLARE(GLenum, glCheckFramebufferStatus, GLenum target);
SIGL_FUNC_DECLARE(void, glFramebufferTexture1D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
SIGL_FUNC_DECLARE(void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
SIGL_FUNC_DECLARE(void, glFramebufferTexture3D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
SIGL_FUNC_DECLARE(void, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
SIGL_FUNC_DECLARE(void, glGetFramebufferAttachmentParameteriv, GLenum target, GLenum attachment, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGenerateMipmap, GLenum target);
SIGL_FUNC_DECLARE(void, glBlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
SIGL_FUNC_DECLARE(void, glRenderbufferStorageMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glFramebufferTextureLayer, GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
SIGL_FUNC_DECLARE(void*, glMapBufferRange, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
SIGL_FUNC_DECLARE(void, glFlushMappedBufferRange, GLenum target, GLintptr offset, GLsizeiptr length);
SIGL_FUNC_DECLARE(void, glBindVertexArray, GLuint array);
SIGL_FUNC_DECLARE(void, glDeleteVertexArrays, GLsizei n, const GLuint* arrays);
SIGL_FUNC_DECLARE(void, glGenVertexArrays, GLsizei n, GLuint* arrays);
SIGL_FUNC_DECLARE(GLboolean, glIsVertexArray, GLuint array);
#endif /* !SIGL_NO_GL_3_0_H_FUNCS */
#if !defined(SIGL_NO_GL_3_1_H_FUNCS)
SIGL_FUNC_DECLARE(void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
SIGL_FUNC_DECLARE(void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
SIGL_FUNC_DECLARE(void, glTexBuffer, GLenum target, GLenum internalformat, GLuint buffer);
SIGL_FUNC_DECLARE(void, glPrimitiveRestartIndex, GLuint index);
SIGL_FUNC_DECLARE(void, glCopyBufferSubData, GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
SIGL_FUNC_DECLARE(void, glGetUniformIndices, GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices);
SIGL_FUNC_DECLARE(void, glGetActiveUniformsiv, GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetActiveUniformName, GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
SIGL_FUNC_DECLARE(GLuint, glGetUniformBlockIndex, GLuint program, const GLchar* uniformBlockName);
SIGL_FUNC_DECLARE(void, glGetActiveUniformBlockiv, GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetActiveUniformBlockName, GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
SIGL_FUNC_DECLARE(void, glUniformBlockBinding, GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
#endif /* !SIGL_NO_GL_3_1_H_FUNCS */
#if !defined(SIGL_NO_GL_3_2_H_FUNCS)
SIGL_FUNC_DECLARE(void, glDrawElementsBaseVertex, GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex);
SIGL_FUNC_DECLARE(void, glDrawRangeElementsBaseVertex, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices, GLint basevertex);
SIGL_FUNC_DECLARE(void, glDrawElementsInstancedBaseVertex, GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex);
SIGL_FUNC_DECLARE(void, glMultiDrawElementsBaseVertex, GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount, const GLint* basevertex);
SIGL_FUNC_DECLARE(void, glProvokingVertex, GLenum mode);
SIGL_FUNC_DECLARE(GLsync, glFenceSync, GLenum condition, GLbitfield flags);
SIGL_FUNC_DECLARE(GLboolean, glIsSync, GLsync sync);
SIGL_FUNC_DECLARE(void, glDeleteSync, GLsync sync);
SIGL_FUNC_DECLARE(GLenum, glClientWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout);
SIGL_FUNC_DECLARE(void, glWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout);
SIGL_FUNC_DECLARE(void, glGetInteger64v, GLenum pname, GLint64* params);
SIGL_FUNC_DECLARE(void, glGetSynciv, GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values);
SIGL_FUNC_DECLARE(void, glGetInteger64i_v, GLenum target, GLuint index, GLint64* data);
SIGL_FUNC_DECLARE(void, glGetBufferParameteri64v, GLenum target, GLenum pname, GLint64* params);
SIGL_FUNC_DECLARE(void, glFramebufferTexture, GLenum target, GLenum attachment, GLuint texture, GLint level);
SIGL_FUNC_DECLARE(void, glTexImage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
SIGL_FUNC_DECLARE(void, glTexImage3DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
SIGL_FUNC_DECLARE(void, glGetMultisamplefv, GLenum pname, GLuint index, GLfloat* val);
SIGL_FUNC_DECLARE(void, glSampleMaski, GLuint maskNumber, GLbitfield mask);
#endif /* !SIGL_NO_GL_3_2_H_FUNCS */
#if !defined(SIGL_NO_GL_3_3_H_FUNCS)
SIGL_FUNC_DECLARE(void, glBindFragDataLocationIndexed, GLuint program, GLuint colorNumber, GLuint index, const GLchar* name);
SIGL_FUNC_DECLARE(GLint, glGetFragDataIndex, GLuint program, const GLchar* name);
SIGL_FUNC_DECLARE(void, glGenSamplers, GLsizei count, GLuint* samplers);
SIGL_FUNC_DECLARE(void, glDeleteSamplers, GLsizei count, const GLuint* samplers);
SIGL_FUNC_DECLARE(GLboolean, glIsSampler, GLuint sampler);
SIGL_FUNC_DECLARE(void, glBindSampler, GLuint unit, GLuint sampler);
SIGL_FUNC_DECLARE(void, glSamplerParameteri, GLuint sampler, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glSamplerParameteriv, GLuint sampler, GLenum pname, const GLint* param);
SIGL_FUNC_DECLARE(void, glSamplerParameterf, GLuint sampler, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glSamplerParameterfv, GLuint sampler, GLenum pname, const GLfloat* param);
SIGL_FUNC_DECLARE(void, glSamplerParameterIiv, GLuint sampler, GLenum pname, const GLint* param);
SIGL_FUNC_DECLARE(void, glSamplerParameterIuiv, GLuint sampler, GLenum pname, const GLuint* param);
SIGL_FUNC_DECLARE(void, glGetSamplerParameteriv, GLuint sampler, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetSamplerParameterIiv, GLuint sampler, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetSamplerParameterfv, GLuint sampler, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetSamplerParameterIuiv, GLuint sampler, GLenum pname, GLuint* params);
SIGL_FUNC_DECLARE(void, glQueryCounter, GLuint id, GLenum target);
SIGL_FUNC_DECLARE(void, glGetQueryObjecti64v, GLuint id, GLenum pname, GLint64* params);
SIGL_FUNC_DECLARE(void, glGetQueryObjectui64v, GLuint id, GLenum pname, GLuint64* params);
SIGL_FUNC_DECLARE(void, glVertexAttribDivisor, GLuint index, GLuint divisor);
SIGL_FUNC_DECLARE(void, glVertexAttribP1ui, GLuint index, GLenum type, GLboolean normalized, GLuint value);
SIGL_FUNC_DECLARE(void, glVertexAttribP1uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
SIGL_FUNC_DECLARE(void, glVertexAttribP2ui, GLuint index, GLenum type, GLboolean normalized, GLuint value);
SIGL_FUNC_DECLARE(void, glVertexAttribP2uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
SIGL_FUNC_DECLARE(void, glVertexAttribP3ui, GLuint index, GLenum type, GLboolean normalized, GLuint value);
SIGL_FUNC_DECLARE(void, glVertexAttribP3uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
SIGL_FUNC_DECLARE(void, glVertexAttribP4ui, GLuint index, GLenum type, GLboolean normalized, GLuint value);
SIGL_FUNC_DECLARE(void, glVertexAttribP4uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
SIGL_FUNC_DECLARE(void, glVertexP2ui, GLenum type, GLuint value);
SIGL_FUNC_DECLARE(void, glVertexP2uiv, GLenum type, const GLuint* value);
SIGL_FUNC_DECLARE(void, glVertexP3ui, GLenum type, GLuint value);
SIGL_FUNC_DECLARE(void, glVertexP3uiv, GLenum type, const GLuint* value);
SIGL_FUNC_DECLARE(void, glVertexP4ui, GLenum type, GLuint value);
SIGL_FUNC_DECLARE(void, glVertexP4uiv, GLenum type, const GLuint* value);
SIGL_FUNC_DECLARE(void, glTexCoordP1ui, GLenum type, GLuint coords);
SIGL_FUNC_DECLARE(void, glTexCoordP1uiv, GLenum type, const GLuint* coords);
SIGL_FUNC_DECLARE(void, glTexCoordP2ui, GLenum type, GLuint coords);
SIGL_FUNC_DECLARE(void, glTexCoordP2uiv, GLenum type, const GLuint* coords);
SIGL_FUNC_DECLARE(void, glTexCoordP3ui, GLenum type, GLuint coords);
SIGL_FUNC_DECLARE(void, glTexCoordP3uiv, GLenum type, const GLuint* coords);
SIGL_FUNC_DECLARE(void, glTexCoordP4ui, GLenum type, GLuint coords);
SIGL_FUNC_DECLARE(void, glTexCoordP4uiv, GLenum type, const GLuint* coords);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP1ui, GLenum target, GLenum type, GLuint coords);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP1uiv, GLenum target, GLenum type, const GLuint* coords);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP2ui, GLenum target, GLenum type, GLuint coords);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP2uiv, GLenum target, GLenum type, const GLuint* coords);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP3ui, GLenum target, GLenum type, GLuint coords);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP3uiv, GLenum target, GLenum type, const GLuint* coords);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP4ui, GLenum target, GLenum type, GLuint coords);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP4uiv, GLenum target, GLenum type, const GLuint* coords);
SIGL_FUNC_DECLARE(void, glNormalP3ui, GLenum type, GLuint coords);
SIGL_FUNC_DECLARE(void, glNormalP3uiv, GLenum type, const GLuint* coords);
SIGL_FUNC_DECLARE(void, glColorP3ui, GLenum type, GLuint color);
SIGL_FUNC_DECLARE(void, glColorP3uiv, GLenum type, const GLuint* color);
SIGL_FUNC_DECLARE(void, glColorP4ui, GLenum type, GLuint color);
SIGL_FUNC_DECLARE(void, glColorP4uiv, GLenum type, const GLuint* color);
SIGL_FUNC_DECLARE(void, glSecondaryColorP3ui, GLenum type, GLuint color);
SIGL_FUNC_DECLARE(void, glSecondaryColorP3uiv, GLenum type, const GLuint* color);
#endif /* !SIGL_NO_GL_3_3_H_FUNCS */
#if !defined(SIGL_NO_GL_4_0_H_FUNCS)
SIGL_FUNC_DECLARE(void, glMinSampleShading, GLfloat value);
SIGL_FUNC_DECLARE(void, glBlendEquationi, GLuint buf, GLenum mode);
SIGL_FUNC_DECLARE(void, glBlendEquationSeparatei, GLuint buf, GLenum modeRGB, GLenum modeAlpha);
SIGL_FUNC_DECLARE(void, glBlendFunci, GLuint buf, GLenum src, GLenum dst);
SIGL_FUNC_DECLARE(void, glBlendFuncSeparatei, GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
SIGL_FUNC_DECLARE(void, glDrawArraysIndirect, GLenum mode, const void* indirect);
SIGL_FUNC_DECLARE(void, glDrawElementsIndirect, GLenum mode, GLenum type, const void* indirect);
SIGL_FUNC_DECLARE(void, glUniform1d, GLint location, GLdouble x);
SIGL_FUNC_DECLARE(void, glUniform2d, GLint location, GLdouble x, GLdouble y);
SIGL_FUNC_DECLARE(void, glUniform3d, GLint location, GLdouble x, GLdouble y, GLdouble z);
SIGL_FUNC_DECLARE(void, glUniform4d, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
SIGL_FUNC_DECLARE(void, glUniform1dv, GLint location, GLsizei count, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniform2dv, GLint location, GLsizei count, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniform3dv, GLint location, GLsizei count, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniform4dv, GLint location, GLsizei count, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix2x3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix2x4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix3x2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix3x4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix4x2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glUniformMatrix4x3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glGetUniformdv, GLuint program, GLint location, GLdouble* params);
SIGL_FUNC_DECLARE(GLint, glGetSubroutineUniformLocation, GLuint program, GLenum shadertype, const GLchar* name);
SIGL_FUNC_DECLARE(GLuint, glGetSubroutineIndex, GLuint program, GLenum shadertype, const GLchar* name);
SIGL_FUNC_DECLARE(void, glGetActiveSubroutineUniformiv, GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values);
SIGL_FUNC_DECLARE(void, glGetActiveSubroutineUniformName, GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar* name);
SIGL_FUNC_DECLARE(void, glGetActiveSubroutineName, GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar* name);
SIGL_FUNC_DECLARE(void, glUniformSubroutinesuiv, GLenum shadertype, GLsizei count, const GLuint* indices);
SIGL_FUNC_DECLARE(void, glGetUniformSubroutineuiv, GLenum shadertype, GLint location, GLuint* params);
SIGL_FUNC_DECLARE(void, glGetProgramStageiv, GLuint program, GLenum shadertype, GLenum pname, GLint* values);
SIGL_FUNC_DECLARE(void, glPatchParameteri, GLenum pname, GLint value);
SIGL_FUNC_DECLARE(void, glPatchParameterfv, GLenum pname, const GLfloat* values);
SIGL_FUNC_DECLARE(void, glBindTransformFeedback, GLenum target, GLuint id);
SIGL_FUNC_DECLARE(void, glDeleteTransformFeedbacks, GLsizei n, const GLuint* ids);
SIGL_FUNC_DECLARE(void, glGenTransformFeedbacks, GLsizei n, GLuint* ids);
SIGL_FUNC_DECLARE(GLboolean, glIsTransformFeedback, GLuint id);
SIGL_FUNC_DECLARE(void, glPauseTransformFeedback, void);
SIGL_FUNC_DECLARE(void, glResumeTransformFeedback, void);
SIGL_FUNC_DECLARE(void, glDrawTransformFeedback, GLenum mode, GLuint id);
SIGL_FUNC_DECLARE(void, glDrawTransformFeedbackStream, GLenum mode, GLuint id, GLuint stream);
SIGL_FUNC_DECLARE(void, glBeginQueryIndexed, GLenum target, GLuint index, GLuint id);
SIGL_FUNC_DECLARE(void, glEndQueryIndexed, GLenum target, GLuint index);
SIGL_FUNC_DECLARE(void, glGetQueryIndexediv, GLenum target, GLuint index, GLenum pname, GLint* params);
#endif /* !SIGL_NO_GL_4_0_H_FUNCS */
#if !defined(SIGL_NO_GL_4_1_H_FUNCS)
SIGL_FUNC_DECLARE(void, glReleaseShaderCompiler, void);
SIGL_FUNC_DECLARE(void, glShaderBinary, GLsizei count, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length);
SIGL_FUNC_DECLARE(void, glGetShaderPrecisionFormat, GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
SIGL_FUNC_DECLARE(void, glDepthRangef, GLfloat n, GLfloat f);
SIGL_FUNC_DECLARE(void, glClearDepthf, GLfloat d);
SIGL_FUNC_DECLARE(void, glGetProgramBinary, GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void* binary);
SIGL_FUNC_DECLARE(void, glProgramBinary, GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
SIGL_FUNC_DECLARE(void, glProgramParameteri, GLuint program, GLenum pname, GLint value);
SIGL_FUNC_DECLARE(void, glUseProgramStages, GLuint pipeline, GLbitfield stages, GLuint program);
SIGL_FUNC_DECLARE(void, glActiveShaderProgram, GLuint pipeline, GLuint program);
SIGL_FUNC_DECLARE(GLuint, glCreateShaderProgramv, GLenum type, GLsizei count, const GLchar* const* strings);
SIGL_FUNC_DECLARE(void, glBindProgramPipeline, GLuint pipeline);
SIGL_FUNC_DECLARE(void, glDeleteProgramPipelines, GLsizei n, const GLuint* pipelines);
SIGL_FUNC_DECLARE(void, glGenProgramPipelines, GLsizei n, GLuint* pipelines);
SIGL_FUNC_DECLARE(GLboolean, glIsProgramPipeline, GLuint pipeline);
SIGL_FUNC_DECLARE(void, glGetProgramPipelineiv, GLuint pipeline, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glProgramUniform1i, GLuint program, GLint location, GLint v0);
SIGL_FUNC_DECLARE(void, glProgramUniform1iv, GLuint program, GLint location, GLsizei count, const GLint* value);
SIGL_FUNC_DECLARE(void, glProgramUniform1f, GLuint program, GLint location, GLfloat v0);
SIGL_FUNC_DECLARE(void, glProgramUniform1fv, GLuint program, GLint location, GLsizei count, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniform1d, GLuint program, GLint location, GLdouble v0);
SIGL_FUNC_DECLARE(void, glProgramUniform1dv, GLuint program, GLint location, GLsizei count, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniform1ui, GLuint program, GLint location, GLuint v0);
SIGL_FUNC_DECLARE(void, glProgramUniform1uiv, GLuint program, GLint location, GLsizei count, const GLuint* value);
SIGL_FUNC_DECLARE(void, glProgramUniform2i, GLuint program, GLint location, GLint v0, GLint v1);
SIGL_FUNC_DECLARE(void, glProgramUniform2iv, GLuint program, GLint location, GLsizei count, const GLint* value);
SIGL_FUNC_DECLARE(void, glProgramUniform2f, GLuint program, GLint location, GLfloat v0, GLfloat v1);
SIGL_FUNC_DECLARE(void, glProgramUniform2fv, GLuint program, GLint location, GLsizei count, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniform2d, GLuint program, GLint location, GLdouble v0, GLdouble v1);
SIGL_FUNC_DECLARE(void, glProgramUniform2dv, GLuint program, GLint location, GLsizei count, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniform2ui, GLuint program, GLint location, GLuint v0, GLuint v1);
SIGL_FUNC_DECLARE(void, glProgramUniform2uiv, GLuint program, GLint location, GLsizei count, const GLuint* value);
SIGL_FUNC_DECLARE(void, glProgramUniform3i, GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
SIGL_FUNC_DECLARE(void, glProgramUniform3iv, GLuint program, GLint location, GLsizei count, const GLint* value);
SIGL_FUNC_DECLARE(void, glProgramUniform3f, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
SIGL_FUNC_DECLARE(void, glProgramUniform3fv, GLuint program, GLint location, GLsizei count, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniform3d, GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
SIGL_FUNC_DECLARE(void, glProgramUniform3dv, GLuint program, GLint location, GLsizei count, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniform3ui, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
SIGL_FUNC_DECLARE(void, glProgramUniform3uiv, GLuint program, GLint location, GLsizei count, const GLuint* value);
SIGL_FUNC_DECLARE(void, glProgramUniform4i, GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
SIGL_FUNC_DECLARE(void, glProgramUniform4iv, GLuint program, GLint location, GLsizei count, const GLint* value);
SIGL_FUNC_DECLARE(void, glProgramUniform4f, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
SIGL_FUNC_DECLARE(void, glProgramUniform4fv, GLuint program, GLint location, GLsizei count, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniform4d, GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
SIGL_FUNC_DECLARE(void, glProgramUniform4dv, GLuint program, GLint location, GLsizei count, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniform4ui, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
SIGL_FUNC_DECLARE(void, glProgramUniform4uiv, GLuint program, GLint location, GLsizei count, const GLuint* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2x3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3x2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2x4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4x2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3x4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4x3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2x3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3x2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2x4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4x2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3x4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4x3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
SIGL_FUNC_DECLARE(void, glValidateProgramPipeline, GLuint pipeline);
SIGL_FUNC_DECLARE(void, glGetProgramPipelineInfoLog, GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
SIGL_FUNC_DECLARE(void, glVertexAttribL1d, GLuint index, GLdouble x);
SIGL_FUNC_DECLARE(void, glVertexAttribL2d, GLuint index, GLdouble x, GLdouble y);
SIGL_FUNC_DECLARE(void, glVertexAttribL3d, GLuint index, GLdouble x, GLdouble y, GLdouble z);
SIGL_FUNC_DECLARE(void, glVertexAttribL4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
SIGL_FUNC_DECLARE(void, glVertexAttribL1dv, GLuint index, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertexAttribL2dv, GLuint index, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertexAttribL3dv, GLuint index, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertexAttribL4dv, GLuint index, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glVertexAttribLPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
SIGL_FUNC_DECLARE(void, glGetVertexAttribLdv, GLuint index, GLenum pname, GLdouble* params);
SIGL_FUNC_DECLARE(void, glViewportArrayv, GLuint first, GLsizei count, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glViewportIndexedf, GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
SIGL_FUNC_DECLARE(void, glViewportIndexedfv, GLuint index, const GLfloat* v);
SIGL_FUNC_DECLARE(void, glScissorArrayv, GLuint first, GLsizei count, const GLint* v);
SIGL_FUNC_DECLARE(void, glScissorIndexed, GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glScissorIndexedv, GLuint index, const GLint* v);
SIGL_FUNC_DECLARE(void, glDepthRangeArrayv, GLuint first, GLsizei count, const GLdouble* v);
SIGL_FUNC_DECLARE(void, glDepthRangeIndexed, GLuint index, GLdouble n, GLdouble f);
SIGL_FUNC_DECLARE(void, glGetFloati_v, GLenum target, GLuint index, GLfloat* data);
SIGL_FUNC_DECLARE(void, glGetDoublei_v, GLenum target, GLuint index, GLdouble* data);
#endif /* !SIGL_NO_GL_4_1_H_FUNCS */
#if !defined(SIGL_NO_GL_4_2_H_FUNCS)
SIGL_FUNC_DECLARE(void, glDrawArraysInstancedBaseInstance, GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
SIGL_FUNC_DECLARE(void, glDrawElementsInstancedBaseInstance, GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLuint baseinstance);
SIGL_FUNC_DECLARE(void, glDrawElementsInstancedBaseVertexBaseInstance, GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
SIGL_FUNC_DECLARE(void, glGetInternalformativ, GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint* params);
SIGL_FUNC_DECLARE(void, glGetActiveAtomicCounterBufferiv, GLuint program, GLuint bufferIndex, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glBindImageTexture, GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
SIGL_FUNC_DECLARE(void, glMemoryBarrier, GLbitfield barriers);
SIGL_FUNC_DECLARE(void, glTexStorage1D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
SIGL_FUNC_DECLARE(void, glTexStorage2D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glTexStorage3D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
SIGL_FUNC_DECLARE(void, glDrawTransformFeedbackInstanced, GLenum mode, GLuint id, GLsizei instancecount);
SIGL_FUNC_DECLARE(void, glDrawTransformFeedbackStreamInstanced, GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);
#endif /* !SIGL_NO_GL_4_2_H_FUNCS */
#if !defined(SIGL_NO_GL_4_3_H_FUNCS)
SIGL_FUNC_DECLARE(void, glClearBufferData, GLenum target, GLenum internalformat, GLenum format, GLenum type, const void* data);
SIGL_FUNC_DECLARE(void, glClearBufferSubData, GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void* data);
SIGL_FUNC_DECLARE(void, glDispatchCompute, GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
SIGL_FUNC_DECLARE(void, glDispatchComputeIndirect, GLintptr indirect);
SIGL_FUNC_DECLARE(void, glCopyImageSubData, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
SIGL_FUNC_DECLARE(void, glFramebufferParameteri, GLenum target, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glGetFramebufferParameteriv, GLenum target, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetInternalformati64v, GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint64* params);
SIGL_FUNC_DECLARE(void, glInvalidateTexSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
SIGL_FUNC_DECLARE(void, glInvalidateTexImage, GLuint texture, GLint level);
SIGL_FUNC_DECLARE(void, glInvalidateBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr length);
SIGL_FUNC_DECLARE(void, glInvalidateBufferData, GLuint buffer);
SIGL_FUNC_DECLARE(void, glInvalidateFramebuffer, GLenum target, GLsizei numAttachments, const GLenum* attachments);
SIGL_FUNC_DECLARE(void, glInvalidateSubFramebuffer, GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glMultiDrawArraysIndirect, GLenum mode, const void* indirect, GLsizei drawcount, GLsizei stride);
SIGL_FUNC_DECLARE(void, glMultiDrawElementsIndirect, GLenum mode, GLenum type, const void* indirect, GLsizei drawcount, GLsizei stride);
SIGL_FUNC_DECLARE(void, glGetProgramInterfaceiv, GLuint program, GLenum programInterface, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(GLuint, glGetProgramResourceIndex, GLuint program, GLenum programInterface, const GLchar* name);
SIGL_FUNC_DECLARE(void, glGetProgramResourceName, GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length, GLchar* name);
SIGL_FUNC_DECLARE(void, glGetProgramResourceiv, GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props, GLsizei bufSize, GLsizei* length, GLint* params);
SIGL_FUNC_DECLARE(GLint, glGetProgramResourceLocation, GLuint program, GLenum programInterface, const GLchar* name);
SIGL_FUNC_DECLARE(GLint, glGetProgramResourceLocationIndex, GLuint program, GLenum programInterface, const GLchar* name);
SIGL_FUNC_DECLARE(void, glShaderStorageBlockBinding, GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
SIGL_FUNC_DECLARE(void, glTexBufferRange, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
SIGL_FUNC_DECLARE(void, glTexStorage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
SIGL_FUNC_DECLARE(void, glTexStorage3DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
SIGL_FUNC_DECLARE(void, glTextureView, GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
SIGL_FUNC_DECLARE(void, glBindVertexBuffer, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
SIGL_FUNC_DECLARE(void, glVertexAttribFormat, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
SIGL_FUNC_DECLARE(void, glVertexAttribIFormat, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
SIGL_FUNC_DECLARE(void, glVertexAttribLFormat, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
SIGL_FUNC_DECLARE(void, glVertexAttribBinding, GLuint attribindex, GLuint bindingindex);
SIGL_FUNC_DECLARE(void, glVertexBindingDivisor, GLuint bindingindex, GLuint divisor);
SIGL_FUNC_DECLARE(void, glDebugMessageControl, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
SIGL_FUNC_DECLARE(void, glDebugMessageInsert, GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
SIGL_FUNC_DECLARE(void, glDebugMessageCallback, GLDEBUGPROC callback, const void* userParam);
SIGL_FUNC_DECLARE(GLuint, glGetDebugMessageLog, GLuint count, GLsizei bufSize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);
SIGL_FUNC_DECLARE(void, glPushDebugGroup, GLenum source, GLuint id, GLsizei length, const GLchar* message);
SIGL_FUNC_DECLARE(void, glPopDebugGroup, void);
SIGL_FUNC_DECLARE(void, glObjectLabel, GLenum identifier, GLuint name, GLsizei length, const GLchar* label);
SIGL_FUNC_DECLARE(void, glGetObjectLabel, GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, GLchar* label);
SIGL_FUNC_DECLARE(void, glObjectPtrLabel, const void* ptr, GLsizei length, const GLchar* label);
SIGL_FUNC_DECLARE(void, glGetObjectPtrLabel, const void* ptr, GLsizei bufSize, GLsizei* length, GLchar* label);
#endif /* !SIGL_NO_GL_4_3_H_FUNCS */
#if !defined(SIGL_NO_GL_4_4_H_FUNCS)
SIGL_FUNC_DECLARE(void, glBufferStorage, GLenum target, GLsizeiptr size, const void* data, GLbitfield flags);
SIGL_FUNC_DECLARE(void, glClearTexImage, GLuint texture, GLint level, GLenum format, GLenum type, const void* data);
SIGL_FUNC_DECLARE(void, glClearTexSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* data);
SIGL_FUNC_DECLARE(void, glBindBuffersBase, GLenum target, GLuint first, GLsizei count, const GLuint* buffers);
SIGL_FUNC_DECLARE(void, glBindBuffersRange, GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizeiptr* sizes);
SIGL_FUNC_DECLARE(void, glBindTextures, GLuint first, GLsizei count, const GLuint* textures);
SIGL_FUNC_DECLARE(void, glBindSamplers, GLuint first, GLsizei count, const GLuint* samplers);
SIGL_FUNC_DECLARE(void, glBindImageTextures, GLuint first, GLsizei count, const GLuint* textures);
SIGL_FUNC_DECLARE(void, glBindVertexBuffers, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizei* strides);
#endif /* !SIGL_NO_GL_4_4_H_FUNCS */
#if !defined(SIGL_NO_GL_4_5_H_FUNCS)
SIGL_FUNC_DECLARE(void, glClipControl, GLenum, GLenum);
SIGL_FUNC_DECLARE(void, glCreateTransformFeedbacks, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glTransformFeedbackBufferBase, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glTransformFeedbackBufferRange, GLuint, GLuint, GLuint, GLintptr, GLsizeiptr);
SIGL_FUNC_DECLARE(void, glGetTransformFeedbackiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetTransformFeedbacki_v, GLuint, GLenum, GLuint, GLint*);
SIGL_FUNC_DECLARE(void, glGetTransformFeedbacki64_v, GLuint, GLenum, GLuint, GLint64*);
SIGL_FUNC_DECLARE(void, glCreateBuffers, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glNamedBufferStorage, GLuint, GLsizeiptr, const void*, GLbitfield);
SIGL_FUNC_DECLARE(void, glNamedBufferData, GLuint, GLsizeiptr, const void*, GLenum);
SIGL_FUNC_DECLARE(void, glNamedBufferSubData, GLuint, GLintptr, GLsizeiptr, const void*);
SIGL_FUNC_DECLARE(void, glCopyNamedBufferSubData, GLuint, GLuint, GLintptr, GLintptr, GLsizeiptr);
SIGL_FUNC_DECLARE(void, glClearNamedBufferData, GLuint, GLenum, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glClearNamedBufferSubData, GLuint, GLenum, GLintptr, GLsizeiptr, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void*, glMapNamedBuffer, GLuint, GLenum);
SIGL_FUNC_DECLARE(void*, glMapNamedBufferRange, GLuint, GLintptr, GLsizeiptr, GLbitfield);
SIGL_FUNC_DECLARE(GLboolean, glUnmapNamedBuffer, GLuint);
SIGL_FUNC_DECLARE(void, glFlushMappedNamedBufferRange, GLuint, GLintptr, GLsizeiptr);
SIGL_FUNC_DECLARE(void, glGetNamedBufferParameteriv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetNamedBufferParameteri64v, GLuint, GLenum, GLint64*);
SIGL_FUNC_DECLARE(void, glGetNamedBufferPointerv, GLuint, GLenum, void**);
SIGL_FUNC_DECLARE(void, glGetNamedBufferSubData, GLuint, GLintptr, GLsizeiptr, void*);
SIGL_FUNC_DECLARE(void, glCreateFramebuffers, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glNamedFramebufferRenderbuffer, GLuint, GLenum, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glNamedFramebufferParameteri, GLuint, GLenum, GLint);
SIGL_FUNC_DECLARE(void, glNamedFramebufferTexture, GLuint, GLenum, GLuint, GLint);
SIGL_FUNC_DECLARE(void, glNamedFramebufferTextureLayer, GLuint, GLenum, GLuint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glNamedFramebufferDrawBuffer, GLuint, GLenum);
SIGL_FUNC_DECLARE(void, glNamedFramebufferDrawBuffers, GLuint, GLsizei, const GLenum*);
SIGL_FUNC_DECLARE(void, glNamedFramebufferReadBuffer, GLuint, GLenum);
SIGL_FUNC_DECLARE(void, glInvalidateNamedFramebufferData, GLuint, GLsizei, const GLenum*);
SIGL_FUNC_DECLARE(void, glInvalidateNamedFramebufferSubData, GLuint, GLsizei, const GLenum*, GLint, GLint, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glClearNamedFramebufferiv, GLuint, GLenum, GLint, const GLint*);
SIGL_FUNC_DECLARE(void, glClearNamedFramebufferuiv, GLuint, GLenum, GLint, const GLuint*);
SIGL_FUNC_DECLARE(void, glClearNamedFramebufferfv, GLuint, GLenum, GLint, const GLfloat*);
SIGL_FUNC_DECLARE(void, glClearNamedFramebufferfi, GLuint, GLenum, GLint, GLfloat, GLint);
SIGL_FUNC_DECLARE(void, glBlitNamedFramebuffer, GLuint, GLuint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
SIGL_FUNC_DECLARE(GLenum, glCheckNamedFramebufferStatus, GLuint, GLenum);
SIGL_FUNC_DECLARE(void, glGetNamedFramebufferParameteriv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetNamedFramebufferAttachmentParameteriv, GLuint, GLenum, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glCreateRenderbuffers, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glNamedRenderbufferStorage, GLuint, GLenum, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glNamedRenderbufferStorageMultisample, GLuint, GLsizei, GLenum, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glGetNamedRenderbufferParameteriv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glCreateTextures, GLenum, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glTextureBuffer, GLuint, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glTextureBufferRange, GLuint, GLenum, GLuint, GLintptr, GLsizeiptr);
SIGL_FUNC_DECLARE(void, glTextureStorage1D, GLuint, GLsizei, GLenum, GLsizei);
SIGL_FUNC_DECLARE(void, glTextureStorage2D, GLuint, GLsizei, GLenum, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glTextureStorage3D, GLuint, GLsizei, GLenum, GLsizei, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glTextureStorage2DMultisample, GLuint, GLsizei, GLenum, GLsizei, GLsizei, GLboolean);
SIGL_FUNC_DECLARE(void, glTextureStorage3DMultisample, GLuint, GLsizei, GLenum, GLsizei, GLsizei, GLsizei, GLboolean);
SIGL_FUNC_DECLARE(void, glTextureSubImage1D, GLuint, GLint, GLint, GLsizei, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glTextureSubImage2D, GLuint, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glTextureSubImage3D, GLuint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glCompressedTextureSubImage1D, GLuint, GLint, GLint, GLsizei, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glCompressedTextureSubImage2D, GLuint, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glCompressedTextureSubImage3D, GLuint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glCopyTextureSubImage1D, GLuint, GLint, GLint, GLint, GLint, GLsizei);
SIGL_FUNC_DECLARE(void, glCopyTextureSubImage2D, GLuint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glCopyTextureSubImage3D, GLuint, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glTextureParameterf, GLuint, GLenum, GLfloat);
SIGL_FUNC_DECLARE(void, glTextureParameterfv, GLuint, GLenum, const GLfloat*);
SIGL_FUNC_DECLARE(void, glTextureParameteri, GLuint, GLenum, GLint);
SIGL_FUNC_DECLARE(void, glTextureParameterIiv, GLuint, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glTextureParameterIuiv, GLuint, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glTextureParameteriv, GLuint, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glGenerateTextureMipmap, GLuint);
SIGL_FUNC_DECLARE(void, glBindTextureUnit, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glGetTextureImage, GLuint, GLint, GLenum, GLenum, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glGetCompressedTextureImage, GLuint, GLint, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glGetTextureLevelParameterfv, GLuint, GLint, GLenum, GLfloat*);
SIGL_FUNC_DECLARE(void, glGetTextureLevelParameteriv, GLuint, GLint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetTextureParameterfv, GLuint, GLenum, GLfloat*);
SIGL_FUNC_DECLARE(void, glGetTextureParameterIiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetTextureParameterIuiv, GLuint, GLenum, GLuint*);
SIGL_FUNC_DECLARE(void, glGetTextureParameteriv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glCreateVertexArrays, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glDisableVertexArrayAttrib, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glEnableVertexArrayAttrib, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glVertexArrayElementBuffer, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glVertexArrayVertexBuffer, GLuint, GLuint, GLuint, GLintptr, GLsizei);
SIGL_FUNC_DECLARE(void, glVertexArrayVertexBuffers, GLuint, GLuint, GLsizei, const GLuint*, const GLintptr*, const GLsizei*);
SIGL_FUNC_DECLARE(void, glVertexArrayAttribBinding, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glVertexArrayAttribFormat, GLuint, GLuint, GLint, GLenum, GLboolean, GLuint);
SIGL_FUNC_DECLARE(void, glVertexArrayAttribIFormat, GLuint, GLuint, GLint, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glVertexArrayAttribLFormat, GLuint, GLuint, GLint, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glVertexArrayBindingDivisor, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glGetVertexArrayiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetVertexArrayIndexediv, GLuint, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetVertexArrayIndexed64iv, GLuint, GLuint, GLenum, GLint64*);
SIGL_FUNC_DECLARE(void, glCreateSamplers, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glCreateProgramPipelines, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glCreateQueries, GLenum, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glGetQueryBufferObjecti64v, GLuint, GLuint, GLenum, GLintptr);
SIGL_FUNC_DECLARE(void, glGetQueryBufferObjectiv, GLuint, GLuint, GLenum, GLintptr);
SIGL_FUNC_DECLARE(void, glGetQueryBufferObjectui64v, GLuint, GLuint, GLenum, GLintptr);
SIGL_FUNC_DECLARE(void, glGetQueryBufferObjectuiv, GLuint, GLuint, GLenum, GLintptr);
SIGL_FUNC_DECLARE(void, glMemoryBarrierByRegion, GLbitfield);
SIGL_FUNC_DECLARE(void, glGetTextureSubImage, GLuint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glGetCompressedTextureSubImage, GLuint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLsizei, void*);
SIGL_FUNC_DECLARE(GLenum, glGetGraphicsResetStatus, void);
SIGL_FUNC_DECLARE(void, glGetnCompressedTexImage, GLenum, GLint, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glGetnTexImage, GLenum, GLint, GLenum, GLenum, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glGetnUniformdv, GLuint, GLint, GLsizei, GLdouble*);
SIGL_FUNC_DECLARE(void, glGetnUniformfv, GLuint, GLint, GLsizei, GLfloat*);
SIGL_FUNC_DECLARE(void, glGetnUniformiv, GLuint, GLint, GLsizei, GLint*);
SIGL_FUNC_DECLARE(void, glGetnUniformuiv, GLuint, GLint, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glReadnPixels, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glGetnMapdv, GLenum, GLenum, GLsizei, GLdouble*);
SIGL_FUNC_DECLARE(void, glGetnMapfv, GLenum, GLenum, GLsizei, GLfloat*);
SIGL_FUNC_DECLARE(void, glGetnMapiv, GLenum, GLenum, GLsizei, GLint*);
SIGL_FUNC_DECLARE(void, glGetnPixelMapfv, GLenum, GLsizei, GLfloat*);
SIGL_FUNC_DECLARE(void, glGetnPixelMapuiv, GLenum, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glGetnPixelMapusv, GLenum, GLsizei, GLushort*);
SIGL_FUNC_DECLARE(void, glGetnPolygonStipple, GLsizei, GLubyte*);
SIGL_FUNC_DECLARE(void, glGetnColorTable, GLenum, GLenum, GLenum, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glGetnConvolutionFilter, GLenum, GLenum, GLenum, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glGetnSeparableFilter, GLenum, GLenum, GLenum, GLsizei, void*, GLsizei, void*, void*);
SIGL_FUNC_DECLARE(void, glGetnHistogram, GLenum, GLboolean, GLenum, GLenum, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glGetnMinmax, GLenum, GLboolean, GLenum, GLenum, GLsizei, void*);
SIGL_FUNC_DECLARE(void, glTextureBarrier, void);
#endif /* !SIGL_NO_GL_4_5_H_FUNCS */
#if !defined(SIGL_NO_GL_4_6_H_FUNCS)
SIGL_FUNC_DECLARE(void, glSpecializeShader, GLuint shader, const GLchar* pEntryPoint, GLuint numSpecializationConstants, const GLuint* pConstantIndex, const GLuint* pConstantValue);
SIGL_FUNC_DECLARE(void, glMultiDrawArraysIndirectCount, GLenum mode, const void* indirect, GLintptr drawCountOffset, GLsizei maxDrawCount, GLsizei stride);
SIGL_FUNC_DECLARE(void, glMultiDrawElementsIndirectCount, GLenum mode, GLenum type, const void* indirect, GLintptr drawCountOffset, GLsizei maxDrawCount, GLsizei stride);
SIGL_FUNC_DECLARE(void, glPolygonOffsetClamp, GLfloat factor, GLfloat units, GLfloat clamp);
#endif /* !SIGL_NO_GL_4_6_H_FUNCS */
#if SIGL_SYSTEM_IS_WINDOWS && !defined(SIGL_NO_WGL_H_FUNCS)
#if defined(WGL_VERSION_1_0) && defined(SIGL_NO_WINDOWS_H)
SIGL_FUNC_DECLARE(int, ChoosePixelFormat, HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd);
SIGL_FUNC_DECLARE(int, DescribePixelFormat, HDC hdc, int iPixelFormat, UINT nBytes, PIXELFORMATDESCRIPTOR* ppfd);
SIGL_FUNC_DECLARE(UINT, GetEnhMetaFilePixelFormat, HENHMETAFILE hemf, const PIXELFORMATDESCRIPTOR* ppfd);
SIGL_FUNC_DECLARE(int, GetPixelFormat, HDC hdc);
SIGL_FUNC_DECLARE(BOOL, SetPixelFormat, HDC hdc, int iPixelFormat, const PIXELFORMATDESCRIPTOR* ppfd);
SIGL_FUNC_DECLARE(BOOL, SwapBuffers, HDC hdc);
SIGL_FUNC_DECLARE(BOOL, wglCopyContext, HGLRC hglrcSrc, HGLRC hglrcDst, UINT mask);
SIGL_FUNC_DECLARE(HGLRC, wglCreateContext, HDC hdc);
SIGL_FUNC_DECLARE(HGLRC, wglCreateLayerContext, HDC hdc, int iLayerPlane);
SIGL_FUNC_DECLARE(BOOL, wglDeleteContext, HGLRC hglrc);
SIGL_FUNC_DECLARE(BOOL, wglDescribeLayerPlane, HDC hdc, int iPixelFormat, int iLayerPlane, UINT nBytes, LAYERPLANEDESCRIPTOR* plpd);
SIGL_FUNC_DECLARE(HGLRC, wglGetCurrentContext, void);
SIGL_FUNC_DECLARE(HDC, wglGetCurrentDC, void);
SIGL_FUNC_DECLARE(int, wglGetLayerPaletteEntries, HDC hdc, int iLayerPlane, int iStart, int cEntries, const COLORREF* pcr);
SIGL_FUNC_DECLARE(PROC, wglGetProcAddress, LPCSTR lpszProc);
SIGL_FUNC_DECLARE(BOOL, wglMakeCurrent, HDC hdc, HGLRC hglrc);
SIGL_FUNC_DECLARE(BOOL, wglRealizeLayerPalette, HDC hdc, int iLayerPlane, BOOL bRealize);
SIGL_FUNC_DECLARE(int, wglSetLayerPaletteEntries, HDC hdc, int iLayerPlane, int iStart, int cEntries, const COLORREF* pcr);
SIGL_FUNC_DECLARE(BOOL, wglShareLists, HGLRC hglrc1, HGLRC hglrc2);
SIGL_FUNC_DECLARE(BOOL, wglSwapLayerBuffers, HDC hdc, UINT fuFlags);
SIGL_FUNC_DECLARE(BOOL, wglUseFontBitmaps, HDC hdc, DWORD first, DWORD count, DWORD listBase);
SIGL_FUNC_DECLARE(BOOL, wglUseFontBitmapsA, HDC hdc, DWORD first, DWORD count, DWORD listBase);
SIGL_FUNC_DECLARE(BOOL, wglUseFontBitmapsW, HDC hdc, DWORD first, DWORD count, DWORD listBase);
SIGL_FUNC_DECLARE(BOOL, wglUseFontOutlines, HDC hdc, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf);
SIGL_FUNC_DECLARE(BOOL, wglUseFontOutlinesA, HDC hdc, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf);
SIGL_FUNC_DECLARE(BOOL, wglUseFontOutlinesW, HDC hdc, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf);
#endif /* WGL_VERSION_1_0 */
#if defined(WGL_ARB_buffer_region) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(HANDLE, wglCreateBufferRegionARB, HDC hDC, int layerPlane, UINT type);
SIGL_FUNC_DECLARE(VOID, wglDeleteBufferRegionARB, HANDLE hRegion);
SIGL_FUNC_DECLARE(BOOL, wglSaveBufferRegionARB, HANDLE hRegion, int x, int y, int width, int height);
SIGL_FUNC_DECLARE(BOOL, wglRestoreBufferRegionARB, HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);
#endif /* WGL_ARB_buffer_region */
#if defined(WGL_ARB_create_context) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(HGLRC, wglCreateContextAttribsARB, HDC hDC, HGLRC hShareContext, const int* attribList);
#endif
#if defined(WGL_ARB_extensions_string) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(const char*, wglGetExtensionsStringARB, HDC hDC);
#endif
#if defined(WGL_ARB_make_current_read) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglMakeContextCurrentARB, HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
SIGL_FUNC_DECLARE(HDC, wglGetCurrentReadDCARB, void);
#endif
#if defined(WGL_ARB_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(HPBUFFERARB, wglCreatePbufferARB, HDC hDC, int pixelFormat, int width, int height, const int* attribList);
SIGL_FUNC_DECLARE(HDC, wglGetPbufferDCARB, HPBUFFERARB hPbuffer);
SIGL_FUNC_DECLARE(int, wglReleasePbufferDCARB, HPBUFFERARB hPbuffer, HDC hDC);
SIGL_FUNC_DECLARE(BOOL, wglDestroyPbufferARB, HPBUFFERARB hPbuffer);
SIGL_FUNC_DECLARE(BOOL, wglQueryPbufferARB, HPBUFFERARB hPbuffer, int iAttribute, int* piValue);
#endif
#if defined(WGL_ARB_pixel_format) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetPixelFormatAttribivARB, HDC hDC, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, int* piValues);
SIGL_FUNC_DECLARE(BOOL, wglGetPixelFormatAttribfvARB, HDC hDC, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, FLOAT* pfValues);
SIGL_FUNC_DECLARE(BOOL, wglChoosePixelFormatARB, HDC hDC, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
#endif
#if defined(WGL_ARB_render_texture) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglBindTexImageARB, HPBUFFERARB hPbuffer, int iBuffer);
SIGL_FUNC_DECLARE(BOOL, wglReleaseTexImageARB, HPBUFFERARB hPbuffer, int iBuffer);
SIGL_FUNC_DECLARE(BOOL, wglSetPbufferAttribARB, HPBUFFERARB hPbuffer, const int* piAttribList);
#endif
#if defined(WGL_3DL_stereo_control) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglSetStereoEmitterState3DL, HDC hDC, UINT uState);
#endif
#if defined(WGL_AMD_gpu_association) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(UINT, wglGetGPUIDsAMD, UINT maxCount, UINT* ids);
SIGL_FUNC_DECLARE(INT, wglGetGPUInfoAMD, UINT id, INT property, GLenum dataType, UINT size, void* data);
SIGL_FUNC_DECLARE(UINT, wglGetContextGPUIDAMD, HGLRC hglrc);
SIGL_FUNC_DECLARE(HGLRC, wglCreateAssociatedContextAMD, UINT id);
SIGL_FUNC_DECLARE(HGLRC, wglCreateAssociatedContextAttribsAMD, UINT id, HGLRC hShareContext, const int* attribList);
SIGL_FUNC_DECLARE(BOOL, wglDeleteAssociatedContextAMD, HGLRC hglrc);
SIGL_FUNC_DECLARE(BOOL, wglMakeAssociatedContextCurrentAMD, HGLRC hglrc);
SIGL_FUNC_DECLARE(HGLRC, wglGetCurrentAssociatedContextAMD, void);
SIGL_FUNC_DECLARE(VOID, wglBlitContextFramebufferAMD, HGLRC hSrcRC, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
#endif
#if defined(WGL_EXT_display_color_table) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(GLboolean, wglCreateDisplayColorTableEXT, GLushort id);
SIGL_FUNC_DECLARE(GLboolean, wglLoadDisplayColorTableEXT, const GLushort* table, GLuint length);
SIGL_FUNC_DECLARE(GLboolean, wglBindDisplayColorTableEXT, GLushort id);
SIGL_FUNC_DECLARE(VOID, wglDestroyDisplayColorTableEXT, GLushort id);
#endif
#if defined(WGL_EXT_extensions_string) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(const char*, wglGetExtensionsStringEXT, void);
#endif
#if defined(WGL_EXT_make_current_read) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglMakeContextCurrentEXT, HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
SIGL_FUNC_DECLARE(HDC, wglGetCurrentReadDCEXT, void);
#endif
#if defined(WGL_EXT_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(HPBUFFEREXT, wglCreatePbufferEXT, HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int* piAttribList);
SIGL_FUNC_DECLARE(HDC, wglGetPbufferDCEXT, HPBUFFEREXT hPbuffer);
SIGL_FUNC_DECLARE(int, wglReleasePbufferDCEXT, HPBUFFEREXT hPbuffer, HDC hDC);
SIGL_FUNC_DECLARE(BOOL, wglDestroyPbufferEXT, HPBUFFEREXT hPbuffer);
SIGL_FUNC_DECLARE(BOOL, wglQueryPbufferEXT, HPBUFFEREXT hPbuffer, int iAttribute, int* piValue);
#endif
#if defined(WGL_EXT_pixel_format) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetPixelFormatAttribivEXT, HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int* piAttributes, int* piValues);
SIGL_FUNC_DECLARE(BOOL, wglGetPixelFormatAttribfvEXT, HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int* piAttributes, FLOAT* pfValues);
SIGL_FUNC_DECLARE(BOOL, wglChoosePixelFormatEXT, HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
#endif
#if defined(WGL_EXT_swap_control) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglSwapIntervalEXT, int interval);
SIGL_FUNC_DECLARE(int, wglGetSwapIntervalEXT, void);
#endif
#if defined(WGL_I3D_digital_video_control) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetDigitalVideoParametersI3D, HDC hDC, int iAttribute, int* piValue);
SIGL_FUNC_DECLARE(BOOL, wglSetDigitalVideoParametersI3D, HDC hDC, int iAttribute, const int* piValue);
#endif
#if defined(WGL_I3D_gamma) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetGammaTableParametersI3D, HDC hDC, int iAttribute, int* piValue);
SIGL_FUNC_DECLARE(BOOL, wglSetGammaTableParametersI3D, HDC hDC, int iAttribute, const int* piValue);
SIGL_FUNC_DECLARE(BOOL, wglGetGammaTableI3D, HDC hDC, int iEntries, USHORT* puRed, USHORT* puGreen, USHORT* puBlue);
SIGL_FUNC_DECLARE(BOOL, wglSetGammaTableI3D, HDC hDC, int iEntries, const USHORT* puRed, const USHORT* puGreen, const USHORT* puBlue);
#endif
#if defined(WGL_I3D_genlock) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglEnableGenlockI3D, HDC hDC);
SIGL_FUNC_DECLARE(BOOL, wglDisableGenlockI3D, HDC hDC);
SIGL_FUNC_DECLARE(BOOL, wglIsEnabledGenlockI3D, HDC hDC, BOOL* pFlag);
SIGL_FUNC_DECLARE(BOOL, wglGenlockSourceI3D, HDC hDC, UINT uSource);
SIGL_FUNC_DECLARE(BOOL, wglGetGenlockSourceI3D, HDC hDC, UINT* uSource);
SIGL_FUNC_DECLARE(BOOL, wglGenlockSourceEdgeI3D, HDC hDC, UINT uEdge);
SIGL_FUNC_DECLARE(BOOL, wglGetGenlockSourceEdgeI3D, HDC hDC, UINT* uEdge);
SIGL_FUNC_DECLARE(BOOL, wglGenlockSampleRateI3D, HDC hDC, UINT uRate);
SIGL_FUNC_DECLARE(BOOL, wglGetGenlockSampleRateI3D, HDC hDC, UINT* uRate);
SIGL_FUNC_DECLARE(BOOL, wglGenlockSourceDelayI3D, HDC hDC, UINT uDelay);
SIGL_FUNC_DECLARE(BOOL, wglGetGenlockSourceDelayI3D, HDC hDC, UINT* uDelay);
SIGL_FUNC_DECLARE(BOOL, wglQueryGenlockMaxSourceDelayI3D, HDC hDC, UINT* uMaxLineDelay, UINT* uMaxPixelDelay);
#endif
#if defined(WGL_I3D_image_buffer) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(LPVOID, wglCreateImageBufferI3D, HDC hDC, DWORD dwSize, UINT uFlags);
SIGL_FUNC_DECLARE(BOOL, wglDestroyImageBufferI3D, HDC hDC, LPVOID pAddress);
SIGL_FUNC_DECLARE(BOOL, wglAssociateImageBufferEventsI3D, HDC hDC, const HANDLE* pEvent, const LPVOID* pAddress, const DWORD* pSize, UINT count);
SIGL_FUNC_DECLARE(BOOL, wglReleaseImageBufferEventsI3D, HDC hDC, const LPVOID* pAddress, UINT count);
#endif
#if defined(WGL_I3D_swap_frame_lock) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglEnableFrameLockI3D, void);
SIGL_FUNC_DECLARE(BOOL, wglDisableFrameLockI3D, void);
SIGL_FUNC_DECLARE(BOOL, wglIsEnabledFrameLockI3D, BOOL* pFlag);
SIGL_FUNC_DECLARE(BOOL, wglQueryFrameLockMasterI3D, BOOL* pFlag);
#endif
#if defined(WGL_I3D_swap_frame_usage) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetFrameUsageI3D, float* pUsage);
SIGL_FUNC_DECLARE(BOOL, wglBeginFrameTrackingI3D, void);
SIGL_FUNC_DECLARE(BOOL, wglEndFrameTrackingI3D, void);
SIGL_FUNC_DECLARE(BOOL, wglQueryFrameTrackingI3D, DWORD* pFrameCount, DWORD* pMissedFrames, float* pLastMissedUsage);
#endif
#if defined(WGL_NV_DX_interop) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglDXSetResourceShareHandleNV, void* dxObject, HANDLE shareHandle);
SIGL_FUNC_DECLARE(HANDLE, wglDXOpenDeviceNV, void* dxDevice);
SIGL_FUNC_DECLARE(BOOL, wglDXCloseDeviceNV, HANDLE hDevice);
SIGL_FUNC_DECLARE(HANDLE, wglDXRegisterObjectNV, HANDLE hDevice, void* dxObject, GLuint name, GLenum type, GLenum access);
SIGL_FUNC_DECLARE(BOOL, wglDXUnregisterObjectNV, HANDLE hDevice, HANDLE hObject);
SIGL_FUNC_DECLARE(BOOL, wglDXObjectAccessNV, HANDLE hObject, GLenum access);
SIGL_FUNC_DECLARE(BOOL, wglDXLockObjectsNV, HANDLE hDevice, GLint count, HANDLE* hObjects);
SIGL_FUNC_DECLARE(BOOL, wglDXUnlockObjectsNV, HANDLE hDevice, GLint count, HANDLE* hObjects);
#endif
#if defined(WGL_NV_copy_image) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglCopyImageSubDataNV, HGLRC hSrcRC, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, HGLRC hDstRC, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
#endif
#if defined(WGL_NV_gpu_affinity) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglEnumGpusNV, UINT iGpuIndex, HGPUNV* phGpu);
SIGL_FUNC_DECLARE(BOOL, wglEnumGpuDevicesNV, HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE pGpuDevice);
SIGL_FUNC_DECLARE(HDC, wglCreateAffinityDCNV, const HGPUNV* phGpuList);
SIGL_FUNC_DECLARE(BOOL, wglEnumGpusFromAffinityDCNV, HDC hAffinityDC, UINT iGpuIndex, HGPUNV* phGpu);
SIGL_FUNC_DECLARE(BOOL, wglDeleteDCNV, HDC hdc);
#endif
#if defined(WGL_NV_present_video) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(int, wglEnumerateVideoDevicesNV, HDC hDC, HVIDEOOUTPUTDEVICENV* pDeviceList);
SIGL_FUNC_DECLARE(BOOL, wglBindVideoDeviceNV, HDC hDC, unsigned int uVideoSlot, HVIDEOOUTPUTDEVICENV hVideoDevice, const int* piAttribList);
SIGL_FUNC_DECLARE(BOOL, wglQueryCurrentContextNV, int iAttribute, int* piValue);
#endif
#if defined(WGL_NV_swap_group) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglJoinSwapGroupNV, HDC hDC, GLuint group);
SIGL_FUNC_DECLARE(BOOL, wglBindSwapBarrierNV, GLuint group, GLuint barrier);
SIGL_FUNC_DECLARE(BOOL, wglQuerySwapGroupNV, HDC hDC, GLuint* group, GLuint* barrier);
SIGL_FUNC_DECLARE(BOOL, wglQueryMaxSwapGroupsNV, HDC hDC, GLuint* maxGroups, GLuint* maxBarriers);
SIGL_FUNC_DECLARE(BOOL, wglQueryFrameCountNV, HDC hDC, GLuint* count);
SIGL_FUNC_DECLARE(BOOL, wglResetFrameCountNV, HDC hDC);
#endif
#if defined(WGL_NV_vertex_array_range) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(void*, wglAllocateMemoryNV, GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority);
SIGL_FUNC_DECLARE(void, wglFreeMemoryNV, void* pointer);
#endif
#if defined(WGL_NV_video_capture) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglBindVideoCaptureDeviceNV, UINT uVideoSlot, HVIDEOINPUTDEVICENV hDevice);
SIGL_FUNC_DECLARE(UINT, wglEnumerateVideoCaptureDevicesNV, HDC hDC, HVIDEOINPUTDEVICENV* phDeviceList);
SIGL_FUNC_DECLARE(BOOL, wglLockVideoCaptureDeviceNV, HDC hDC, HVIDEOINPUTDEVICENV hDevice);
SIGL_FUNC_DECLARE(BOOL, wglQueryVideoCaptureDeviceNV, HDC hDC, HVIDEOINPUTDEVICENV hDevice, int iAttribute, int* piValue);
SIGL_FUNC_DECLARE(BOOL, wglReleaseVideoCaptureDeviceNV, HDC hDC, HVIDEOINPUTDEVICENV hDevice);
#endif
#if defined(WGL_NV_video_output) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetVideoDeviceNV, HDC hDC, int numDevices, HPVIDEODEV* hVideoDevice);
SIGL_FUNC_DECLARE(BOOL, wglReleaseVideoDeviceNV, HPVIDEODEV hVideoDevice);
SIGL_FUNC_DECLARE(BOOL, wglBindVideoImageNV, HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer);
SIGL_FUNC_DECLARE(BOOL, wglReleaseVideoImageNV, HPBUFFERARB hPbuffer, int iVideoBuffer);
SIGL_FUNC_DECLARE(BOOL, wglSendPbufferToVideoNV, HPBUFFERARB hPbuffer, int iBufferType, unsigned long* pulCounterPbuffer, BOOL bBlock);
SIGL_FUNC_DECLARE(BOOL, wglGetVideoInfoNV, HPVIDEODEV hpVideoDevice, unsigned long* pulCounterOutputPbuffer, unsigned long* pulCounterOutputVideo);
SIGL_FUNC_DECLARE(BOOL, wglGetSyncValuesOML, HDC hdc, INT64* ust, INT64* msc, INT64* sbc);
SIGL_FUNC_DECLARE(BOOL, wglGetMscRateOML, HDC hdc, INT32* numerator, INT32* denominator);
SIGL_FUNC_DECLARE(INT64, wglSwapBuffersMscOML, HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder);
SIGL_FUNC_DECLARE(INT64, wglSwapLayerBuffersMscOML, HDC hdc, INT fuPlanes, INT64 target_msc, INT64 divisor, INT64 remainder);
SIGL_FUNC_DECLARE(BOOL, wglWaitForMscOML, HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder, INT64* ust, INT64* msc, INT64* sbc);
SIGL_FUNC_DECLARE(BOOL, wglWaitForSbcOML, HDC hdc, INT64 target_sbc, INT64* ust, INT64* msc, INT64* sbc);
#endif

#endif

#if SIGL_SYSTEM_IS_UNIX && !defined(SIGL_NO_GLX_H_FUNCS)
extern XVisualInfo* glXChooseVisual( Display *dpy, int screen,
				     int *attribList );

extern GLXContext glXCreateContext( Display *dpy, XVisualInfo *vis,
				    GLXContext shareList, Bool direct );

extern void glXDestroyContext( Display *dpy, GLXContext ctx );

extern Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable,
			    GLXContext ctx);

extern void glXCopyContext( Display *dpy, GLXContext src, GLXContext dst,
			    unsigned long mask );

extern void glXSwapBuffers( Display *dpy, GLXDrawable drawable );

extern GLXPixmap glXCreateGLXPixmap( Display *dpy, XVisualInfo *visual,
				     Pixmap pixmap );

extern void glXDestroyGLXPixmap( Display *dpy, GLXPixmap pixmap );

extern Bool glXQueryExtension( Display *dpy, int *errorb, int *event );

extern Bool glXQueryVersion( Display *dpy, int *maj, int *min );

extern Bool glXIsDirect( Display *dpy, GLXContext ctx );

extern int glXGetConfig( Display *dpy, XVisualInfo *visual,
			 int attrib, int *value );

extern GLXContext glXGetCurrentContext( void );

extern GLXDrawable glXGetCurrentDrawable( void );

extern void glXWaitGL( void );

extern void glXWaitX( void );

extern void glXUseXFont( Font font, int first, int count, int list );



/* GLX 1.1 and later */
extern const char *glXQueryExtensionsString( Display *dpy, int screen );

extern const char *glXQueryServerString( Display *dpy, int screen, int name );

extern const char *glXGetClientString( Display *dpy, int name );


/* GLX 1.2 and later */
extern Display *glXGetCurrentDisplay( void );


/* GLX 1.3 and later */
extern GLXFBConfig *glXChooseFBConfig( Display *dpy, int screen,
                                       const int *attribList, int *nitems );

extern int glXGetFBConfigAttrib( Display *dpy, GLXFBConfig config,
                                 int attribute, int *value );

extern GLXFBConfig *glXGetFBConfigs( Display *dpy, int screen,
                                     int *nelements );

extern XVisualInfo *glXGetVisualFromFBConfig( Display *dpy,
                                              GLXFBConfig config );

extern GLXWindow glXCreateWindow( Display *dpy, GLXFBConfig config,
                                  Window win, const int *attribList );

extern void glXDestroyWindow( Display *dpy, GLXWindow window );

extern GLXPixmap glXCreatePixmap( Display *dpy, GLXFBConfig config,
                                  Pixmap pixmap, const int *attribList );

extern void glXDestroyPixmap( Display *dpy, GLXPixmap pixmap );

extern GLXPbuffer glXCreatePbuffer( Display *dpy, GLXFBConfig config,
                                    const int *attribList );

extern void glXDestroyPbuffer( Display *dpy, GLXPbuffer pbuf );

extern void glXQueryDrawable( Display *dpy, GLXDrawable draw, int attribute,
                              unsigned int *value );

extern GLXContext glXCreateNewContext( Display *dpy, GLXFBConfig config,
                                       int renderType, GLXContext shareList,
                                       Bool direct );

extern Bool glXMakeContextCurrent( Display *dpy, GLXDrawable draw,
                                   GLXDrawable read, GLXContext ctx );

extern GLXDrawable glXGetCurrentReadDrawable( void );

extern int glXQueryContext( Display *dpy, GLXContext ctx, int attribute,
                            int *value );

extern void glXSelectEvent( Display *dpy, GLXDrawable drawable,
                            unsigned long mask );

extern void glXGetSelectedEvent( Display *dpy, GLXDrawable drawable,
                                 unsigned long *mask );

/* GLX 1.4 and later */
extern __GLXextFuncPtr glXGetProcAddress(const GLubyte *procname);

#ifndef GLX_EXT_swap_control
SIGL_FUNC_DECLARE(void, glXSwapIntervalEXT, Display *dpy, GLXDrawable drawable, int interval);
#endif
#endif

#undef SIGL_FUNC_DECLARE


#if defined(SIGL_IMPLEMENTATION)

#if SIGL_SYSTEM_IS_WINDOWS || SIGL_SYSTEM_IS_UNIX
	#define SIGL_FUNC_DECLARE(retType, name, ...) name##SIPROC name
#else
	#define SIGL_FUNC_DECLARE(retType, name, ...) typedef int name ## sili
#endif

#if !defined(SIGL_NO_GL_1_2_H_FUNCS)
SIGL_FUNC_DECLARE(void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices);
SIGL_FUNC_DECLARE(void, glTexImage3D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
SIGL_FUNC_DECLARE(void, glTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
SIGL_FUNC_DECLARE(void, glCopyTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
#endif /* SIGL_NO_GL_1_2_H_FUNCS */
#if !defined(SIGL_NO_GL_1_3_H_FUNCS)
SIGL_FUNC_DECLARE(void, glActiveTexture, GLenum);
SIGL_FUNC_DECLARE(void, glSampleCoverage, GLfloat, GLboolean);
SIGL_FUNC_DECLARE(void, glCompressedTexImage3D, GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glCompressedTexImage2D, GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glCompressedTexImage1D, GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glCompressedTexSubImage3D, GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glCompressedTexSubImage2D, GLenum, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glCompressedTexSubImage1D, GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glGetCompressedTexImage, GLenum, GLint, void*);
SIGL_FUNC_DECLARE(void, glClientActiveTexture, GLenum);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1d, GLenum, GLdouble);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1dv, GLenum, const GLdouble*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1f, GLenum, GLfloat);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1fv, GLenum, const GLfloat*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1i, GLenum, GLint);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1iv, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1s, GLenum, GLshort);
SIGL_FUNC_DECLARE(void, glMultiTexCoord1sv, GLenum, const GLshort*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2d, GLenum, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2dv, GLenum, const GLdouble*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2f, GLenum, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2fv, GLenum, const GLfloat*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2i, GLenum, GLint, GLint);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2iv, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2s, GLenum, GLshort, GLshort);
SIGL_FUNC_DECLARE(void, glMultiTexCoord2sv, GLenum, const GLshort*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3d, GLenum, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3dv, GLenum, const GLdouble*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3f, GLenum, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3fv, GLenum, const GLfloat*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3i, GLenum, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3iv, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3s, GLenum, GLshort, GLshort, GLshort);
SIGL_FUNC_DECLARE(void, glMultiTexCoord3sv, GLenum, const GLshort*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4d, GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4dv, GLenum, const GLdouble*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4f, GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4fv, GLenum, const GLfloat*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4i, GLenum, GLint, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4iv, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4s, GLenum, GLshort, GLshort, GLshort, GLshort);
SIGL_FUNC_DECLARE(void, glMultiTexCoord4sv, GLenum, const GLshort*);
SIGL_FUNC_DECLARE(void, glLoadTransposeMatrixf, const GLfloat*);
SIGL_FUNC_DECLARE(void, glLoadTransposeMatrixd, const GLdouble*);
SIGL_FUNC_DECLARE(void, glMultTransposeMatrixf, const GLfloat*);
SIGL_FUNC_DECLARE(void, glMultTransposeMatrixd, const GLdouble*);
#endif /* SIGL_NO_GL_1_3_H_FUNCS */
#if !defined(SIGL_NO_GL_1_4_H_FUNCS)
SIGL_FUNC_DECLARE(void, glBlendFuncSeparate, GLenum, GLenum, GLenum, GLenum);
SIGL_FUNC_DECLARE(void, glMultiDrawArrays, GLenum, const GLint*, const GLsizei*, GLsizei);
SIGL_FUNC_DECLARE(void, glMultiDrawElements, GLenum, const GLsizei*, GLenum, const void* const*, GLsizei);
SIGL_FUNC_DECLARE(void, glPointParameterf, GLenum, GLfloat);
SIGL_FUNC_DECLARE(void, glPointParameterfv, GLenum, const GLfloat*);
SIGL_FUNC_DECLARE(void, glPointParameteri, GLenum, GLint);
SIGL_FUNC_DECLARE(void, glPointParameteriv, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glFogCoordf, GLfloat);
SIGL_FUNC_DECLARE(void, glFogCoordfv, const GLfloat*);
SIGL_FUNC_DECLARE(void, glFogCoordd, GLdouble);
SIGL_FUNC_DECLARE(void, glFogCoorddv, const GLdouble*);
SIGL_FUNC_DECLARE(void, glFogCoordPointer, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glSecondaryColor3b, GLbyte, GLbyte, GLbyte);
SIGL_FUNC_DECLARE(void, glSecondaryColor3bv, const GLbyte*);
SIGL_FUNC_DECLARE(void, glSecondaryColor3d, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glSecondaryColor3dv, const GLdouble*);
SIGL_FUNC_DECLARE(void, glSecondaryColor3f, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glSecondaryColor3fv, const GLfloat*);
SIGL_FUNC_DECLARE(void, glSecondaryColor3i, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glSecondaryColor3iv, const GLint*);
SIGL_FUNC_DECLARE(void, glSecondaryColor3s, GLshort, GLshort, GLshort);
SIGL_FUNC_DECLARE(void, glSecondaryColor3sv, const GLshort*);
SIGL_FUNC_DECLARE(void, glSecondaryColor3ub, GLubyte, GLubyte, GLubyte);
SIGL_FUNC_DECLARE(void, glSecondaryColor3ubv, const GLubyte*);
SIGL_FUNC_DECLARE(void, glSecondaryColor3ui, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glSecondaryColor3uiv, const GLuint*);
SIGL_FUNC_DECLARE(void, glSecondaryColor3us, GLushort, GLushort, GLushort);
SIGL_FUNC_DECLARE(void, glSecondaryColor3usv, const GLushort*);
SIGL_FUNC_DECLARE(void, glSecondaryColorPointer, GLint, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glWindowPos2d, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glWindowPos2dv, const GLdouble*);
SIGL_FUNC_DECLARE(void, glWindowPos2f, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glWindowPos2fv, const GLfloat*);
SIGL_FUNC_DECLARE(void, glWindowPos2i, GLint, GLint);
SIGL_FUNC_DECLARE(void, glWindowPos2iv, const GLint*);
SIGL_FUNC_DECLARE(void, glWindowPos2s, GLshort, GLshort);
SIGL_FUNC_DECLARE(void, glWindowPos2sv, const GLshort*);
SIGL_FUNC_DECLARE(void, glWindowPos3d, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glWindowPos3dv, const GLdouble*);
SIGL_FUNC_DECLARE(void, glWindowPos3f, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glWindowPos3fv, const GLfloat*);
SIGL_FUNC_DECLARE(void, glWindowPos3i, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glWindowPos3iv, const GLint*);
SIGL_FUNC_DECLARE(void, glWindowPos3s, GLshort, GLshort, GLshort);
SIGL_FUNC_DECLARE(void, glWindowPos3sv, const GLshort*);
SIGL_FUNC_DECLARE(void, glBlendColor, GLfloat, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glBlendEquation, GLenum);
#endif
#if !defined(SIGL_NO_GL_1_5_H_FUNCS)
SIGL_FUNC_DECLARE(void, glGenQueries, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glDeleteQueries, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(GLboolean, glIsQuery, GLuint);
SIGL_FUNC_DECLARE(void, glBeginQuery, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glEndQuery, GLenum);
SIGL_FUNC_DECLARE(void, glGetQueryiv, GLenum, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetQueryObjectiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetQueryObjectuiv, GLuint, GLenum, GLuint*);
SIGL_FUNC_DECLARE(void, glBindBuffer, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glDeleteBuffers, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glGenBuffers, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(GLboolean, glIsBuffer, GLuint);
SIGL_FUNC_DECLARE(void, glBufferData, GLenum, GLsizeiptr, const void*, GLenum);
SIGL_FUNC_DECLARE(void, glBufferSubData, GLenum, GLintptr, GLsizeiptr, const void*);
SIGL_FUNC_DECLARE(void, glGetBufferSubData, GLenum, GLintptr, GLsizeiptr, void*);
SIGL_FUNC_DECLARE(void*, glMapBuffer, GLenum, GLenum);
SIGL_FUNC_DECLARE(GLboolean, glUnmapBuffer, GLenum);
SIGL_FUNC_DECLARE(void, glGetBufferParameteriv, GLenum, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetBufferPointerv, GLenum, GLenum, void*);
#endif /* !SIGL_NO_GL_1_5_H_FUNCS */
#if !defined(SIGL_NO_GL_2_0_H_FUNCS)
SIGL_FUNC_DECLARE(void, glBlendEquationSeparate, GLenum, GLenum);
SIGL_FUNC_DECLARE(void, glDrawBuffers, GLsizei, const GLenum*);
SIGL_FUNC_DECLARE(void, glStencilOpSeparate, GLenum, GLenum, GLenum, GLenum);
SIGL_FUNC_DECLARE(void, glStencilFuncSeparate, GLenum, GLenum, GLint, GLuint);
SIGL_FUNC_DECLARE(void, glStencilMaskSeparate, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glAttachShader, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glBindAttribLocation, GLuint, GLuint, const GLchar*);
SIGL_FUNC_DECLARE(void, glCompileShader, GLuint);
SIGL_FUNC_DECLARE(GLuint, glCreateProgram, void);
SIGL_FUNC_DECLARE(GLuint, glCreateShader, GLenum);
SIGL_FUNC_DECLARE(void, glDeleteProgram, GLuint);
SIGL_FUNC_DECLARE(void, glDeleteShader, GLuint);
SIGL_FUNC_DECLARE(void, glDetachShader, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glDisableVertexAttribArray, GLuint);
SIGL_FUNC_DECLARE(void, glEnableVertexAttribArray, GLuint);
SIGL_FUNC_DECLARE(void, glGetActiveAttrib, GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar*);
SIGL_FUNC_DECLARE(void, glGetActiveUniform, GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar*);
SIGL_FUNC_DECLARE(void, glGetAttachedShaders, GLuint, GLsizei, GLsizei*, GLuint*);
SIGL_FUNC_DECLARE(GLint, glGetAttribLocation, GLuint, const GLchar*);
SIGL_FUNC_DECLARE(void, glGetProgramiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetProgramInfoLog, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(void, glGetShaderiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetShaderInfoLog, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(void, glGetShaderSource, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(GLint, glGetUniformLocation, GLuint, const GLchar*);
SIGL_FUNC_DECLARE(void, glGetUniformfv, GLuint, GLint, GLfloat*);
SIGL_FUNC_DECLARE(void, glGetUniformiv, GLuint, GLint, GLint*);
SIGL_FUNC_DECLARE(void, glGetVertexAttribdv, GLuint, GLenum, GLdouble*);
SIGL_FUNC_DECLARE(void, glGetVertexAttribfv, GLuint, GLenum, GLfloat*);
SIGL_FUNC_DECLARE(void, glGetVertexAttribiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetVertexAttribPointerv, GLuint, GLenum, void**);
SIGL_FUNC_DECLARE(GLboolean, glIsProgram, GLuint);
SIGL_FUNC_DECLARE(GLboolean, glIsShader, GLuint);
SIGL_FUNC_DECLARE(void, glLinkProgram, GLuint);
SIGL_FUNC_DECLARE(void, glShaderSource, GLuint, GLsizei, const GLchar* const*, const GLint*);
SIGL_FUNC_DECLARE(void, glUseProgram, GLuint);
SIGL_FUNC_DECLARE(void, glUniform1f, GLint, GLfloat);
SIGL_FUNC_DECLARE(void, glUniform2f, GLint, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glUniform3f, GLint, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glUniform4f, GLint, GLfloat, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glUniform1i, GLint, GLint);
SIGL_FUNC_DECLARE(void, glUniform2i, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glUniform3i, GLint, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glUniform4i, GLint, GLint, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glUniform1fv, GLint, GLsizei, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniform2fv, GLint, GLsizei, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniform3fv, GLint, GLsizei, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniform4fv, GLint, GLsizei, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniform1iv, GLint, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glUniform2iv, GLint, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glUniform3iv, GLint, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glUniform4iv, GLint, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glUniformMatrix2fv, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniformMatrix3fv, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glValidateProgram, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttrib1d, GLuint, GLdouble);
SIGL_FUNC_DECLARE(void, glVertexAttrib1dv, GLuint, const GLdouble*);
SIGL_FUNC_DECLARE(void, glVertexAttrib1f, GLuint, GLfloat);
SIGL_FUNC_DECLARE(void, glVertexAttrib1fv, GLuint, const GLfloat*);
SIGL_FUNC_DECLARE(void, glVertexAttrib1s, GLuint, GLshort);
SIGL_FUNC_DECLARE(void, glVertexAttrib1sv, GLuint, const GLshort*);
SIGL_FUNC_DECLARE(void, glVertexAttrib2d, GLuint, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glVertexAttrib2dv, GLuint, const GLdouble*);
SIGL_FUNC_DECLARE(void, glVertexAttrib2f, GLuint, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glVertexAttrib2fv, GLuint, const GLfloat*);
SIGL_FUNC_DECLARE(void, glVertexAttrib2s, GLuint, GLshort, GLshort);
SIGL_FUNC_DECLARE(void, glVertexAttrib2sv, GLuint, const GLshort*);
SIGL_FUNC_DECLARE(void, glVertexAttrib3d, GLuint, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glVertexAttrib3dv, GLuint, const GLdouble*);
SIGL_FUNC_DECLARE(void, glVertexAttrib3f, GLuint, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glVertexAttrib3fv, GLuint, const GLfloat*);
SIGL_FUNC_DECLARE(void, glVertexAttrib3s, GLuint, GLshort, GLshort, GLshort);
SIGL_FUNC_DECLARE(void, glVertexAttrib3sv, GLuint, const GLshort*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nbv, GLuint, const GLbyte*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Niv, GLuint, const GLint*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nsv, GLuint, const GLshort*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nub, GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nubv, GLuint, const GLubyte*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nuiv, GLuint, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4Nusv, GLuint, const GLushort*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4bv, GLuint, const GLbyte*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4d, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glVertexAttrib4dv, GLuint, const GLdouble*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4f, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glVertexAttrib4fv, GLuint, const GLfloat*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4iv, GLuint, const GLint*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4s, GLuint, GLshort, GLshort, GLshort, GLshort);
SIGL_FUNC_DECLARE(void, glVertexAttrib4sv, GLuint, const GLshort*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4ubv, GLuint, const GLubyte*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4uiv, GLuint, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttrib4usv, GLuint, const GLushort*);
SIGL_FUNC_DECLARE(void, glVertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
#endif /* !SIGL_NO_GL_2_0_H_FUNCS */
#if !defined(SIGL_NO_GL_2_1_H_FUNCS)
SIGL_FUNC_DECLARE(void, glUniformMatrix2x3fv, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniformMatrix3x2fv, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniformMatrix2x4fv, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniformMatrix4x2fv, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniformMatrix3x4fv, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glUniformMatrix4x3fv, GLint, GLsizei, GLboolean, const GLfloat*);
#endif /* !SIGL_NO_GL_2_1_H_FUNCS */
#if !defined(SIGL_NO_GL_3_0_H_FUNCS)
SIGL_FUNC_DECLARE(void, glColorMaski, GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
SIGL_FUNC_DECLARE(void, glGetBooleani_v, GLenum, GLuint, GLboolean*);
SIGL_FUNC_DECLARE(void, glGetIntegeri_v, GLenum, GLuint, GLint*);
SIGL_FUNC_DECLARE(void, glEnablei, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glDisablei, GLenum, GLuint);
SIGL_FUNC_DECLARE(GLboolean, glIsEnabledi, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glBeginTransformFeedback, GLenum);
SIGL_FUNC_DECLARE(void, glEndTransformFeedback, void);
SIGL_FUNC_DECLARE(void, glBindBufferRange, GLenum, GLuint, GLuint, GLintptr, GLsizeiptr);
SIGL_FUNC_DECLARE(void, glBindBufferBase, GLenum, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glTransformFeedbackVaryings, GLuint, GLsizei, const GLchar* const*, GLenum);
SIGL_FUNC_DECLARE(void, glGetTransformFeedbackVarying, GLuint, GLuint, GLsizei, GLsizei*, GLsizei*, GLenum*, GLchar*);
SIGL_FUNC_DECLARE(void, glClampColor, GLenum, GLenum);
SIGL_FUNC_DECLARE(void, glBeginConditionalRender, GLuint, GLenum);
SIGL_FUNC_DECLARE(void, glEndConditionalRender, void);
SIGL_FUNC_DECLARE(void, glVertexAttribIPointer, GLuint, GLint, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glGetVertexAttribIiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetVertexAttribIuiv, GLuint, GLenum, GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttribI1i, GLuint, GLint);
SIGL_FUNC_DECLARE(void, glVertexAttribI2i, GLuint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glVertexAttribI3i, GLuint, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glVertexAttribI4i, GLuint, GLint, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glVertexAttribI1ui, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribI2ui, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribI3ui, GLuint, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribI4ui, GLuint, GLuint, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribI1iv, GLuint, const GLint*);
SIGL_FUNC_DECLARE(void, glVertexAttribI2iv, GLuint, const GLint*);
SIGL_FUNC_DECLARE(void, glVertexAttribI3iv, GLuint, const GLint*);
SIGL_FUNC_DECLARE(void, glVertexAttribI4iv, GLuint, const GLint*);
SIGL_FUNC_DECLARE(void, glVertexAttribI1uiv, GLuint, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttribI2uiv, GLuint, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttribI3uiv, GLuint, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttribI4uiv, GLuint, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttribI4bv, GLuint, const GLbyte*);
SIGL_FUNC_DECLARE(void, glVertexAttribI4sv, GLuint, const GLshort*);
SIGL_FUNC_DECLARE(void, glVertexAttribI4ubv, GLuint, const GLubyte*);
SIGL_FUNC_DECLARE(void, glVertexAttribI4usv, GLuint, const GLushort*);
SIGL_FUNC_DECLARE(void, glGetUniformuiv, GLuint, GLint, GLuint*);
SIGL_FUNC_DECLARE(void, glBindFragDataLocation, GLuint, GLuint, const GLchar*);
SIGL_FUNC_DECLARE(GLint, glGetFragDataLocation, GLuint, const GLchar*);
SIGL_FUNC_DECLARE(void, glUniform1ui, GLint, GLuint);
SIGL_FUNC_DECLARE(void, glUniform2ui, GLint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glUniform3ui, GLint, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glUniform4ui, GLint, GLuint, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glUniform1uiv, GLint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glUniform2uiv, GLint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glUniform3uiv, GLint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glUniform4uiv, GLint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glTexParameterIiv, GLenum, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glTexParameterIuiv, GLenum, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glGetTexParameterIiv, GLenum, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetTexParameterIuiv, GLenum, GLenum, GLuint*);
SIGL_FUNC_DECLARE(void, glClearBufferiv, GLenum, GLint, const GLint*);
SIGL_FUNC_DECLARE(void, glClearBufferuiv, GLenum, GLint, const GLuint*);
SIGL_FUNC_DECLARE(void, glClearBufferfv, GLenum, GLint, const GLfloat*);
SIGL_FUNC_DECLARE(void, glClearBufferfi, GLenum, GLint, GLfloat, GLint);
SIGL_FUNC_DECLARE(const GLubyte*, glGetStringi, GLenum, GLuint);
SIGL_FUNC_DECLARE(GLboolean, glIsRenderbuffer, GLuint);
SIGL_FUNC_DECLARE(void, glBindRenderbuffer, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glDeleteRenderbuffers, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glGenRenderbuffers, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glRenderbufferStorage, GLenum, GLenum, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glGetRenderbufferParameteriv, GLenum, GLenum, GLint*);
SIGL_FUNC_DECLARE(GLboolean, glIsFramebuffer, GLuint);
SIGL_FUNC_DECLARE(void, glBindFramebuffer, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glDeleteFramebuffers, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glGenFramebuffers, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(GLenum, glCheckFramebufferStatus, GLenum);
SIGL_FUNC_DECLARE(void, glFramebufferTexture1D, GLenum, GLenum, GLenum, GLuint, GLint);
SIGL_FUNC_DECLARE(void, glFramebufferTexture2D, GLenum, GLenum, GLenum, GLuint, GLint);
SIGL_FUNC_DECLARE(void, glFramebufferTexture3D, GLenum, GLenum, GLenum, GLuint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glFramebufferRenderbuffer, GLenum, GLenum, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glGetFramebufferAttachmentParameteriv, GLenum, GLenum, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGenerateMipmap, GLenum);
SIGL_FUNC_DECLARE(void, glBlitFramebuffer, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
SIGL_FUNC_DECLARE(void, glRenderbufferStorageMultisample, GLenum, GLsizei, GLenum, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glFramebufferTextureLayer, GLenum, GLenum, GLuint, GLint, GLint);
SIGL_FUNC_DECLARE(void*, glMapBufferRange, GLenum, GLintptr, GLsizeiptr, GLbitfield);
SIGL_FUNC_DECLARE(void, glFlushMappedBufferRange, GLenum, GLintptr, GLsizeiptr);
SIGL_FUNC_DECLARE(void, glBindVertexArray, GLuint);
SIGL_FUNC_DECLARE(void, glDeleteVertexArrays, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glGenVertexArrays, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(GLboolean, glIsVertexArray, GLuint);
#endif /* !SIGL_NO_GL_3_0_H_FUNCS */
#if !defined(SIGL_NO_GL_3_1_H_FUNCS)
SIGL_FUNC_DECLARE(void, glDrawArraysInstanced, GLenum, GLint, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glDrawElementsInstanced, GLenum, GLsizei, GLenum, const void*, GLsizei);
SIGL_FUNC_DECLARE(void, glTexBuffer, GLenum, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glPrimitiveRestartIndex, GLuint);
SIGL_FUNC_DECLARE(void, glCopyBufferSubData, GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr);
SIGL_FUNC_DECLARE(void, glGetUniformIndices, GLuint, GLsizei, const GLchar* const*, GLuint*);
SIGL_FUNC_DECLARE(void, glGetActiveUniformsiv, GLuint, GLsizei, const GLuint*, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetActiveUniformName, GLuint, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(GLuint, glGetUniformBlockIndex, GLuint, const GLchar*);
SIGL_FUNC_DECLARE(void, glGetActiveUniformBlockiv, GLuint, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetActiveUniformBlockName, GLuint, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(void, glUniformBlockBinding, GLuint, GLuint, GLuint);
#endif /* !SIGL_NO_GL_3_1_H_FUNCS */
#if !defined(SIGL_NO_GL_3_2_H_FUNCS)
SIGL_FUNC_DECLARE(void, glDrawElementsBaseVertex, GLenum, GLsizei, GLenum, const void*, GLint);
SIGL_FUNC_DECLARE(void, glDrawRangeElementsBaseVertex, GLenum, GLuint, GLuint, GLsizei, GLenum, const void*, GLint);
SIGL_FUNC_DECLARE(void, glDrawElementsInstancedBaseVertex, GLenum, GLsizei, GLenum, const void*, GLsizei, GLint);
SIGL_FUNC_DECLARE(void, glMultiDrawElementsBaseVertex, GLenum, const GLsizei*, GLenum, const void* const*, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glProvokingVertex, GLenum);
SIGL_FUNC_DECLARE(GLsync, glFenceSync, GLenum, GLbitfield);
SIGL_FUNC_DECLARE(GLboolean, glIsSync, GLsync);
SIGL_FUNC_DECLARE(void, glDeleteSync, GLsync);
SIGL_FUNC_DECLARE(GLenum, glClientWaitSync, GLsync, GLbitfield, GLuint64);
SIGL_FUNC_DECLARE(void, glWaitSync, GLsync, GLbitfield, GLuint64);
SIGL_FUNC_DECLARE(void, glGetInteger64v, GLenum, GLint64*);
SIGL_FUNC_DECLARE(void, glGetSynciv, GLsync, GLenum, GLsizei, GLsizei*, GLint*);
SIGL_FUNC_DECLARE(void, glGetInteger64i_v, GLenum, GLuint, GLint64*);
SIGL_FUNC_DECLARE(void, glGetBufferParameteri64v, GLenum, GLenum, GLint64*);
SIGL_FUNC_DECLARE(void, glFramebufferTexture, GLenum, GLenum, GLuint, GLint);
SIGL_FUNC_DECLARE(void, glTexImage2DMultisample, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLboolean);
SIGL_FUNC_DECLARE(void, glTexImage3DMultisample, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei, GLboolean);
SIGL_FUNC_DECLARE(void, glGetMultisamplefv, GLenum, GLuint, GLfloat*);
SIGL_FUNC_DECLARE(void, glSampleMaski, GLuint, GLbitfield);
#endif /* !SIGL_NO_GL_3_2_H_FUNCS */
#if !defined(SIGL_NO_GL_3_3_H_FUNCS)
SIGL_FUNC_DECLARE(void, glBindFragDataLocationIndexed, GLuint, GLuint, GLuint, const GLchar*);
SIGL_FUNC_DECLARE(GLint, glGetFragDataIndex, GLuint, const GLchar*);
SIGL_FUNC_DECLARE(void, glGenSamplers, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(void, glDeleteSamplers, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(GLboolean, glIsSampler, GLuint);
SIGL_FUNC_DECLARE(void, glBindSampler, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glSamplerParameteri, GLuint, GLenum, GLint);
SIGL_FUNC_DECLARE(void, glSamplerParameteriv, GLuint, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glSamplerParameterf, GLuint, GLenum, GLfloat);
SIGL_FUNC_DECLARE(void, glSamplerParameterfv, GLuint, GLenum, const GLfloat*);
SIGL_FUNC_DECLARE(void, glSamplerParameterIiv, GLuint, GLenum, const GLint*);
SIGL_FUNC_DECLARE(void, glSamplerParameterIuiv, GLuint, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glGetSamplerParameteriv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetSamplerParameterIiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetSamplerParameterfv, GLuint, GLenum, GLfloat*);
SIGL_FUNC_DECLARE(void, glGetSamplerParameterIuiv, GLuint, GLenum, GLuint*);
SIGL_FUNC_DECLARE(void, glQueryCounter, GLuint, GLenum);
SIGL_FUNC_DECLARE(void, glGetQueryObjecti64v, GLuint, GLenum, GLint64*);
SIGL_FUNC_DECLARE(void, glGetQueryObjectui64v, GLuint, GLenum, GLuint64*);
SIGL_FUNC_DECLARE(void, glVertexAttribDivisor, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribP1ui, GLuint, GLenum, GLboolean, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribP1uiv, GLuint, GLenum, GLboolean, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttribP2ui, GLuint, GLenum, GLboolean, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribP2uiv, GLuint, GLenum, GLboolean, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttribP3ui, GLuint, GLenum, GLboolean, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribP3uiv, GLuint, GLenum, GLboolean, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexAttribP4ui, GLuint, GLenum, GLboolean, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribP4uiv, GLuint, GLenum, GLboolean, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexP2ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glVertexP2uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexP3ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glVertexP3uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glVertexP4ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glVertexP4uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glTexCoordP1ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glTexCoordP1uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glTexCoordP2ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glTexCoordP2uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glTexCoordP3ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glTexCoordP3uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glTexCoordP4ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glTexCoordP4uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP1ui, GLenum, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP1uiv, GLenum, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP2ui, GLenum, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP2uiv, GLenum, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP3ui, GLenum, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP3uiv, GLenum, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP4ui, GLenum, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glMultiTexCoordP4uiv, GLenum, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glNormalP3ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glNormalP3uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glColorP3ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glColorP3uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glColorP4ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glColorP4uiv, GLenum, const GLuint*);
SIGL_FUNC_DECLARE(void, glSecondaryColorP3ui, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glSecondaryColorP3uiv, GLenum, const GLuint*);
#endif /* !SIGL_NO_GL_3_3_H_FUNCS */
#if !defined(SIGL_NO_GL_4_0_H_FUNCS)
SIGL_FUNC_DECLARE(void, glMinSampleShading, GLfloat);
SIGL_FUNC_DECLARE(void, glBlendEquationi, GLuint, GLenum);
SIGL_FUNC_DECLARE(void, glBlendEquationSeparatei, GLuint, GLenum, GLenum);
SIGL_FUNC_DECLARE(void, glBlendFunci, GLuint, GLenum, GLenum);
SIGL_FUNC_DECLARE(void, glBlendFuncSeparatei, GLuint, GLenum, GLenum, GLenum, GLenum);
SIGL_FUNC_DECLARE(void, glDrawArraysIndirect, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glDrawElementsIndirect, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glUniform1d, GLint, GLdouble);
SIGL_FUNC_DECLARE(void, glUniform2d, GLint, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glUniform3d, GLint, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glUniform4d, GLint, GLdouble, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glUniform1dv, GLint, GLsizei, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniform2dv, GLint, GLsizei, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniform3dv, GLint, GLsizei, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniform4dv, GLint, GLsizei, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniformMatrix2dv, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniformMatrix3dv, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniformMatrix4dv, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniformMatrix2x3dv, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniformMatrix2x4dv, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniformMatrix3x2dv, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniformMatrix3x4dv, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniformMatrix4x2dv, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glUniformMatrix4x3dv, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glGetUniformdv, GLuint, GLint, GLdouble*);
SIGL_FUNC_DECLARE(GLint, glGetSubroutineUniformLocation, GLuint, GLenum, const GLchar*);
SIGL_FUNC_DECLARE(GLuint, glGetSubroutineIndex, GLuint, GLenum, const GLchar*);
SIGL_FUNC_DECLARE(void, glGetActiveSubroutineUniformiv, GLuint, GLenum, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetActiveSubroutineUniformName, GLuint, GLenum, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(void, glGetActiveSubroutineName, GLuint, GLenum, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(void, glUniformSubroutinesuiv, GLenum, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glGetUniformSubroutineuiv, GLenum, GLint, GLuint*);
SIGL_FUNC_DECLARE(void, glGetProgramStageiv, GLuint, GLenum, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glPatchParameteri, GLenum, GLint);
SIGL_FUNC_DECLARE(void, glPatchParameterfv, GLenum, const GLfloat*);
SIGL_FUNC_DECLARE(void, glBindTransformFeedback, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glDeleteTransformFeedbacks, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glGenTransformFeedbacks, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(GLboolean, glIsTransformFeedback, GLuint);
SIGL_FUNC_DECLARE(void, glPauseTransformFeedback, void);
SIGL_FUNC_DECLARE(void, glResumeTransformFeedback, void);
SIGL_FUNC_DECLARE(void, glDrawTransformFeedback, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glDrawTransformFeedbackStream, GLenum, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glBeginQueryIndexed, GLenum, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glEndQueryIndexed, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glGetQueryIndexediv, GLenum, GLuint, GLenum, GLint*);
#endif /* !SIGL_NO_GL_4_0_H_FUNCS */
#if !defined(SIGL_NO_GL_4_1_H_FUNCS)
SIGL_FUNC_DECLARE(void, glReleaseShaderCompiler, void);
SIGL_FUNC_DECLARE(void, glShaderBinary, GLsizei, const GLuint*, GLenum, const void*, GLsizei);
SIGL_FUNC_DECLARE(void, glGetShaderPrecisionFormat, GLenum, GLenum, GLint*, GLint*);
SIGL_FUNC_DECLARE(void, glDepthRangef, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glClearDepthf, GLfloat);
SIGL_FUNC_DECLARE(void, glGetProgramBinary, GLuint, GLsizei, GLsizei*, GLenum*, void*);
SIGL_FUNC_DECLARE(void, glProgramBinary, GLuint, GLenum, const void*, GLsizei);
SIGL_FUNC_DECLARE(void, glProgramParameteri, GLuint, GLenum, GLint);
SIGL_FUNC_DECLARE(void, glUseProgramStages, GLuint, GLbitfield, GLuint);
SIGL_FUNC_DECLARE(void, glActiveShaderProgram, GLuint, GLuint);
SIGL_FUNC_DECLARE(GLuint, glCreateShaderProgramv, GLenum, GLsizei, const GLchar* const*);
SIGL_FUNC_DECLARE(void, glBindProgramPipeline, GLuint);
SIGL_FUNC_DECLARE(void, glDeleteProgramPipelines, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glGenProgramPipelines, GLsizei, GLuint*);
SIGL_FUNC_DECLARE(GLboolean, glIsProgramPipeline, GLuint);
SIGL_FUNC_DECLARE(void, glGetProgramPipelineiv, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glProgramUniform1i, GLuint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glProgramUniform1iv, GLuint, GLint, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glProgramUniform1f, GLuint, GLint, GLfloat);
SIGL_FUNC_DECLARE(void, glProgramUniform1fv, GLuint, GLint, GLsizei, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniform1d, GLuint, GLint, GLdouble);
SIGL_FUNC_DECLARE(void, glProgramUniform1dv, GLuint, GLint, GLsizei, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniform1ui, GLuint, GLint, GLuint);
SIGL_FUNC_DECLARE(void, glProgramUniform1uiv, GLuint, GLint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glProgramUniform2i, GLuint, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glProgramUniform2iv, GLuint, GLint, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glProgramUniform2f, GLuint, GLint, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glProgramUniform2fv, GLuint, GLint, GLsizei, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniform2d, GLuint, GLint, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glProgramUniform2dv, GLuint, GLint, GLsizei, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniform2ui, GLuint, GLint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glProgramUniform2uiv, GLuint, GLint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glProgramUniform3i, GLuint, GLint, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glProgramUniform3iv, GLuint, GLint, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glProgramUniform3f, GLuint, GLint, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glProgramUniform3fv, GLuint, GLint, GLsizei, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniform3d, GLuint, GLint, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glProgramUniform3dv, GLuint, GLint, GLsizei, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniform3ui, GLuint, GLint, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glProgramUniform3uiv, GLuint, GLint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glProgramUniform4i, GLuint, GLint, GLint, GLint, GLint, GLint);
SIGL_FUNC_DECLARE(void, glProgramUniform4iv, GLuint, GLint, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glProgramUniform4f, GLuint, GLint, GLfloat, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glProgramUniform4fv, GLuint, GLint, GLsizei, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniform4d, GLuint, GLint, GLdouble, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glProgramUniform4dv, GLuint, GLint, GLsizei, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniform4ui, GLuint, GLint, GLuint, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glProgramUniform4uiv, GLuint, GLint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2dv, GLuint, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3dv, GLuint, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4dv, GLuint, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2x3fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3x2fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2x4fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4x2fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3x4fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4x3fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2x3dv, GLuint, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3x2dv, GLuint, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix2x4dv, GLuint, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4x2dv, GLuint, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix3x4dv, GLuint, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glProgramUniformMatrix4x3dv, GLuint, GLint, GLsizei, GLboolean, const GLdouble*);
SIGL_FUNC_DECLARE(void, glValidateProgramPipeline, GLuint);
SIGL_FUNC_DECLARE(void, glGetProgramPipelineInfoLog, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(void, glVertexAttribL1d, GLuint, GLdouble);
SIGL_FUNC_DECLARE(void, glVertexAttribL2d, GLuint, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glVertexAttribL3d, GLuint, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glVertexAttribL4d, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glVertexAttribL1dv, GLuint, const GLdouble*);
SIGL_FUNC_DECLARE(void, glVertexAttribL2dv, GLuint, const GLdouble*);
SIGL_FUNC_DECLARE(void, glVertexAttribL3dv, GLuint, const GLdouble*);
SIGL_FUNC_DECLARE(void, glVertexAttribL4dv, GLuint, const GLdouble*);
SIGL_FUNC_DECLARE(void, glVertexAttribLPointer, GLuint, GLint, GLenum, GLsizei, const void*);
SIGL_FUNC_DECLARE(void, glGetVertexAttribLdv, GLuint, GLenum, GLdouble*);
SIGL_FUNC_DECLARE(void, glViewportArrayv, GLuint, GLsizei, const GLfloat*);
SIGL_FUNC_DECLARE(void, glViewportIndexedf, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
SIGL_FUNC_DECLARE(void, glViewportIndexedfv, GLuint, const GLfloat*);
SIGL_FUNC_DECLARE(void, glScissorArrayv, GLuint, GLsizei, const GLint*);
SIGL_FUNC_DECLARE(void, glScissorIndexed, GLuint, GLint, GLint, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glScissorIndexedv, GLuint, const GLint*);
SIGL_FUNC_DECLARE(void, glDepthRangeArrayv, GLuint, GLsizei, const GLdouble*);
SIGL_FUNC_DECLARE(void, glDepthRangeIndexed, GLuint, GLdouble, GLdouble);
SIGL_FUNC_DECLARE(void, glGetFloati_v, GLenum, GLuint, GLfloat*);
SIGL_FUNC_DECLARE(void, glGetDoublei_v, GLenum, GLuint, GLdouble*);
#endif /* !SIGL_NO_GL_4_1_H_FUNCS */
#if !defined(SIGL_NO_GL_4_2_H_FUNCS)
SIGL_FUNC_DECLARE(void, glDrawArraysInstancedBaseInstance, GLenum, GLint, GLsizei, GLsizei, GLuint);
SIGL_FUNC_DECLARE(void, glDrawElementsInstancedBaseInstance, GLenum, GLsizei, GLenum, const void*, GLsizei, GLuint);
SIGL_FUNC_DECLARE(void, glDrawElementsInstancedBaseVertexBaseInstance, GLenum, GLsizei, GLenum, const void*, GLsizei, GLint, GLuint);
SIGL_FUNC_DECLARE(void, glGetInternalformativ, GLenum, GLenum, GLenum, GLsizei, GLint*);
SIGL_FUNC_DECLARE(void, glGetActiveAtomicCounterBufferiv, GLuint, GLuint, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glBindImageTexture, GLuint, GLuint, GLint, GLboolean, GLint, GLenum, GLenum);
SIGL_FUNC_DECLARE(void, glMemoryBarrier, GLbitfield);
SIGL_FUNC_DECLARE(void, glTexStorage1D, GLenum, GLsizei, GLenum, GLsizei);
SIGL_FUNC_DECLARE(void, glTexStorage2D, GLenum, GLsizei, GLenum, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glTexStorage3D, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glDrawTransformFeedbackInstanced, GLenum, GLuint, GLsizei);
SIGL_FUNC_DECLARE(void, glDrawTransformFeedbackStreamInstanced, GLenum, GLuint, GLuint, GLsizei);
#endif /* !SIGL_NO_GL_4_2_H_FUNCS */
#if !defined(SIGL_NO_GL_4_3_H_FUNCS)
SIGL_FUNC_DECLARE(void, glClearBufferData, GLenum, GLenum, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glClearBufferSubData, GLenum, GLenum, GLintptr, GLsizeiptr, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glDispatchCompute, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glDispatchComputeIndirect, GLintptr);
SIGL_FUNC_DECLARE(void, glCopyImageSubData, GLuint, GLenum, GLint, GLint, GLint, GLint, GLuint, GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glFramebufferParameteri, GLenum, GLenum, GLint);
SIGL_FUNC_DECLARE(void, glGetFramebufferParameteriv, GLenum, GLenum, GLint*);
SIGL_FUNC_DECLARE(void, glGetInternalformati64v, GLenum, GLenum, GLenum, GLsizei, GLint64*);
SIGL_FUNC_DECLARE(void, glInvalidateTexSubImage, GLuint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glInvalidateTexImage, GLuint, GLint);
SIGL_FUNC_DECLARE(void, glInvalidateBufferSubData, GLuint, GLintptr, GLsizeiptr);
SIGL_FUNC_DECLARE(void, glInvalidateBufferData, GLuint);
SIGL_FUNC_DECLARE(void, glInvalidateFramebuffer, GLenum, GLsizei, const GLenum*);
SIGL_FUNC_DECLARE(void, glInvalidateSubFramebuffer, GLenum, GLsizei, const GLenum*, GLint, GLint, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glMultiDrawArraysIndirect, GLenum, const void*, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glMultiDrawElementsIndirect, GLenum, GLenum, const void*, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glGetProgramInterfaceiv, GLuint, GLenum, GLenum, GLint*);
SIGL_FUNC_DECLARE(GLuint, glGetProgramResourceIndex, GLuint, GLenum, const GLchar*);
SIGL_FUNC_DECLARE(void, glGetProgramResourceName, GLuint, GLenum, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(void, glGetProgramResourceiv, GLuint, GLenum, GLuint, GLsizei, const GLenum*, GLsizei, GLsizei*, GLint*);
SIGL_FUNC_DECLARE(GLint, glGetProgramResourceLocation, GLuint, GLenum, const GLchar*);
SIGL_FUNC_DECLARE(GLint, glGetProgramResourceLocationIndex, GLuint, GLenum, const GLchar*);
SIGL_FUNC_DECLARE(void, glShaderStorageBlockBinding, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glTexBufferRange, GLenum, GLenum, GLuint, GLintptr, GLsizeiptr);
SIGL_FUNC_DECLARE(void, glTexStorage2DMultisample, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLboolean);
SIGL_FUNC_DECLARE(void, glTexStorage3DMultisample, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei, GLboolean);
SIGL_FUNC_DECLARE(void, glTextureView, GLuint, GLenum, GLuint, GLenum, GLuint, GLuint, GLuint, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glBindVertexBuffer, GLuint, GLuint, GLintptr, GLsizei);
SIGL_FUNC_DECLARE(void, glVertexAttribFormat, GLuint, GLint, GLenum, GLboolean, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribIFormat, GLuint, GLint, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribLFormat, GLuint, GLint, GLenum, GLuint);
SIGL_FUNC_DECLARE(void, glVertexAttribBinding, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glVertexBindingDivisor, GLuint, GLuint);
SIGL_FUNC_DECLARE(void, glDebugMessageControl, GLenum, GLenum, GLenum, GLsizei, const GLuint*, GLboolean);
SIGL_FUNC_DECLARE(void, glDebugMessageInsert, GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*);
SIGL_FUNC_DECLARE(void, glDebugMessageCallback, GLDEBUGPROC, const void*);
SIGL_FUNC_DECLARE(GLuint, glGetDebugMessageLog, GLuint, GLsizei, GLenum*, GLenum*, GLuint*, GLenum*, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(void, glPushDebugGroup, GLenum, GLuint, GLsizei, const GLchar*);
SIGL_FUNC_DECLARE(void, glPopDebugGroup, void);
SIGL_FUNC_DECLARE(void, glObjectLabel, GLenum, GLuint, GLsizei, const GLchar*);
SIGL_FUNC_DECLARE(void, glGetObjectLabel, GLenum, GLuint, GLsizei, GLsizei*, GLchar*);
SIGL_FUNC_DECLARE(void, glObjectPtrLabel, const void*, GLsizei, const GLchar*);
SIGL_FUNC_DECLARE(void, glGetObjectPtrLabel, const void*, GLsizei, GLsizei*, GLchar*);
#endif /* !SIGL_NO_GL_4_3_H_FUNCS */
#if !defined(SIGL_NO_GL_4_4_H_FUNCS)
SIGL_FUNC_DECLARE(void, glBufferStorage, GLenum, GLsizeiptr, const void*, GLbitfield);
SIGL_FUNC_DECLARE(void, glClearTexImage, GLuint, GLint, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glClearTexSubImage, GLuint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const void*);
SIGL_FUNC_DECLARE(void, glBindBuffersBase, GLenum, GLuint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glBindBuffersRange, GLenum, GLuint, GLsizei, const GLuint*, const GLintptr*, const GLsizeiptr*);
SIGL_FUNC_DECLARE(void, glBindTextures, GLuint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glBindSamplers, GLuint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glBindImageTextures, GLuint, GLsizei, const GLuint*);
SIGL_FUNC_DECLARE(void, glBindVertexBuffers, GLuint, GLsizei, const GLuint*, const GLintptr*, const GLsizei*);
#endif /* !SIGL_NO_GL_4_4_H_FUNCS */
#if !defined(SIGL_NO_GL_4_5_H_FUNCS)
SIGL_FUNC_DECLARE(void, glClipControl, GLenum origin, GLenum depth);
SIGL_FUNC_DECLARE(void, glCreateTransformFeedbacks, GLsizei n, GLuint* ids);
SIGL_FUNC_DECLARE(void, glTransformFeedbackBufferBase, GLuint xfb, GLuint index, GLuint buffer);
SIGL_FUNC_DECLARE(void, glTransformFeedbackBufferRange, GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
SIGL_FUNC_DECLARE(void, glGetTransformFeedbackiv, GLuint xfb, GLenum pname, GLint* param);
SIGL_FUNC_DECLARE(void, glGetTransformFeedbacki_v, GLuint xfb, GLenum pname, GLuint index, GLint* param);
SIGL_FUNC_DECLARE(void, glGetTransformFeedbacki64_v, GLuint xfb, GLenum pname, GLuint index, GLint64* param);
SIGL_FUNC_DECLARE(void, glCreateBuffers, GLsizei n, GLuint* buffers);
SIGL_FUNC_DECLARE(void, glNamedBufferStorage, GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags);
SIGL_FUNC_DECLARE(void, glNamedBufferData, GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
SIGL_FUNC_DECLARE(void, glNamedBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
SIGL_FUNC_DECLARE(void, glCopyNamedBufferSubData, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
SIGL_FUNC_DECLARE(void, glClearNamedBufferData, GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void* data);
SIGL_FUNC_DECLARE(void, glClearNamedBufferSubData, GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void* data);
SIGL_FUNC_DECLARE(void*, glMapNamedBuffer, GLuint buffer, GLenum access);
SIGL_FUNC_DECLARE(void*, glMapNamedBufferRange, GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
SIGL_FUNC_DECLARE(GLboolean, glUnmapNamedBuffer, GLuint buffer);
SIGL_FUNC_DECLARE(void, glFlushMappedNamedBufferRange, GLuint buffer, GLintptr offset, GLsizeiptr length);
SIGL_FUNC_DECLARE(void, glGetNamedBufferParameteriv, GLuint buffer, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetNamedBufferParameteri64v, GLuint buffer, GLenum pname, GLint64* params);
SIGL_FUNC_DECLARE(void, glGetNamedBufferPointerv, GLuint buffer, GLenum pname, void** params);
SIGL_FUNC_DECLARE(void, glGetNamedBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
SIGL_FUNC_DECLARE(void, glCreateFramebuffers, GLsizei n, GLuint* framebuffers);
SIGL_FUNC_DECLARE(void, glNamedFramebufferRenderbuffer, GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
SIGL_FUNC_DECLARE(void, glNamedFramebufferParameteri, GLuint framebuffer, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glNamedFramebufferTexture, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
SIGL_FUNC_DECLARE(void, glNamedFramebufferTextureLayer, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
SIGL_FUNC_DECLARE(void, glNamedFramebufferDrawBuffer, GLuint framebuffer, GLenum mode);
SIGL_FUNC_DECLARE(void, glNamedFramebufferDrawBuffers, GLuint framebuffer, GLsizei n, const GLenum* bufs);
SIGL_FUNC_DECLARE(void, glNamedFramebufferReadBuffer, GLuint framebuffer, GLenum mode);
SIGL_FUNC_DECLARE(void, glInvalidateNamedFramebufferData, GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments);
SIGL_FUNC_DECLARE(void, glInvalidateNamedFramebufferSubData, GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glClearNamedFramebufferiv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint* value);
SIGL_FUNC_DECLARE(void, glClearNamedFramebufferuiv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value);
SIGL_FUNC_DECLARE(void, glClearNamedFramebufferfv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value);
SIGL_FUNC_DECLARE(void, glClearNamedFramebufferfi, GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
SIGL_FUNC_DECLARE(void, glBlitNamedFramebuffer, GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
SIGL_FUNC_DECLARE(GLenum, glCheckNamedFramebufferStatus, GLuint framebuffer, GLenum target);
SIGL_FUNC_DECLARE(void, glGetNamedFramebufferParameteriv, GLuint framebuffer, GLenum pname, GLint* param);
SIGL_FUNC_DECLARE(void, glGetNamedFramebufferAttachmentParameteriv, GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glCreateRenderbuffers, GLsizei n, GLuint* renderbuffers);
SIGL_FUNC_DECLARE(void, glNamedRenderbufferStorage, GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glNamedRenderbufferStorageMultisample, GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glGetNamedRenderbufferParameteriv, GLuint renderbuffer, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glCreateTextures, GLenum target, GLsizei n, GLuint* textures);
SIGL_FUNC_DECLARE(void, glTextureBuffer, GLuint texture, GLenum internalformat, GLuint buffer);
SIGL_FUNC_DECLARE(void, glTextureBufferRange, GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
SIGL_FUNC_DECLARE(void, glTextureStorage1D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
SIGL_FUNC_DECLARE(void, glTextureStorage2D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glTextureStorage3D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
SIGL_FUNC_DECLARE(void, glTextureStorage2DMultisample, GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
SIGL_FUNC_DECLARE(void, glTextureStorage3DMultisample, GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
SIGL_FUNC_DECLARE(void, glTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
SIGL_FUNC_DECLARE(void, glTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
SIGL_FUNC_DECLARE(void, glTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
SIGL_FUNC_DECLARE(void, glCompressedTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
SIGL_FUNC_DECLARE(void, glCompressedTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
SIGL_FUNC_DECLARE(void, glCompressedTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
SIGL_FUNC_DECLARE(void, glCopyTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
SIGL_FUNC_DECLARE(void, glCopyTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glCopyTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
SIGL_FUNC_DECLARE(void, glTextureParameterf, GLuint texture, GLenum pname, GLfloat param);
SIGL_FUNC_DECLARE(void, glTextureParameterfv, GLuint texture, GLenum pname, const GLfloat* param);
SIGL_FUNC_DECLARE(void, glTextureParameteri, GLuint texture, GLenum pname, GLint param);
SIGL_FUNC_DECLARE(void, glTextureParameterIiv, GLuint texture, GLenum pname, const GLint* params);
SIGL_FUNC_DECLARE(void, glTextureParameterIuiv, GLuint texture, GLenum pname, const GLuint* params);
SIGL_FUNC_DECLARE(void, glTextureParameteriv, GLuint texture, GLenum pname, const GLint* param);
SIGL_FUNC_DECLARE(void, glGenerateTextureMipmap, GLuint texture);
SIGL_FUNC_DECLARE(void, glBindTextureUnit, GLuint unit, GLuint texture);
SIGL_FUNC_DECLARE(void, glGetTextureImage, GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
SIGL_FUNC_DECLARE(void, glGetCompressedTextureImage, GLuint texture, GLint level, GLsizei bufSize, void* pixels);
SIGL_FUNC_DECLARE(void, glGetTextureLevelParameterfv, GLuint texture, GLint level, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetTextureLevelParameteriv, GLuint texture, GLint level, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetTextureParameterfv, GLuint texture, GLenum pname, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetTextureParameterIiv, GLuint texture, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glGetTextureParameterIuiv, GLuint texture, GLenum pname, GLuint* params);
SIGL_FUNC_DECLARE(void, glGetTextureParameteriv, GLuint texture, GLenum pname, GLint* params);
SIGL_FUNC_DECLARE(void, glCreateVertexArrays, GLsizei n, GLuint* arrays);
SIGL_FUNC_DECLARE(void, glDisableVertexArrayAttrib, GLuint vaobj, GLuint index);
SIGL_FUNC_DECLARE(void, glEnableVertexArrayAttrib, GLuint vaobj, GLuint index);
SIGL_FUNC_DECLARE(void, glVertexArrayElementBuffer, GLuint vaobj, GLuint buffer);
SIGL_FUNC_DECLARE(void, glVertexArrayVertexBuffer, GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
SIGL_FUNC_DECLARE(void, glVertexArrayVertexBuffers, GLuint vaobj, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizei* strides);
SIGL_FUNC_DECLARE(void, glVertexArrayAttribBinding, GLuint vaobj, GLuint attribindex, GLuint bindingindex);
SIGL_FUNC_DECLARE(void, glVertexArrayAttribFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
SIGL_FUNC_DECLARE(void, glVertexArrayAttribIFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
SIGL_FUNC_DECLARE(void, glVertexArrayAttribLFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
SIGL_FUNC_DECLARE(void, glVertexArrayBindingDivisor, GLuint vaobj, GLuint bindingindex, GLuint divisor);
SIGL_FUNC_DECLARE(void, glGetVertexArrayiv, GLuint vaobj, GLenum pname, GLint* param);
SIGL_FUNC_DECLARE(void, glGetVertexArrayIndexediv, GLuint vaobj, GLuint index, GLenum pname, GLint* param);
SIGL_FUNC_DECLARE(void, glGetVertexArrayIndexed64iv, GLuint vaobj, GLuint index, GLenum pname, GLint64* param);
SIGL_FUNC_DECLARE(void, glCreateSamplers, GLsizei n, GLuint* samplers);
SIGL_FUNC_DECLARE(void, glCreateProgramPipelines, GLsizei n, GLuint* pipelines);
SIGL_FUNC_DECLARE(void, glCreateQueries, GLenum target, GLsizei n, GLuint* ids);
SIGL_FUNC_DECLARE(void, glGetQueryBufferObjecti64v, GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
SIGL_FUNC_DECLARE(void, glGetQueryBufferObjectiv, GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
SIGL_FUNC_DECLARE(void, glGetQueryBufferObjectui64v, GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
SIGL_FUNC_DECLARE(void, glGetQueryBufferObjectuiv, GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
SIGL_FUNC_DECLARE(void, glMemoryBarrierByRegion, GLbitfield barriers);
SIGL_FUNC_DECLARE(void, glGetTextureSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
SIGL_FUNC_DECLARE(void, glGetCompressedTextureSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void* pixels);
SIGL_FUNC_DECLARE(GLenum, glGetGraphicsResetStatus, void);
SIGL_FUNC_DECLARE(void, glGetnCompressedTexImage, GLenum target, GLint lod, GLsizei bufSize, void* pixels);
SIGL_FUNC_DECLARE(void, glGetnTexImage, GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
SIGL_FUNC_DECLARE(void, glGetnUniformdv, GLuint program, GLint location, GLsizei bufSize, GLdouble* params);
SIGL_FUNC_DECLARE(void, glGetnUniformfv, GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
SIGL_FUNC_DECLARE(void, glGetnUniformiv, GLuint program, GLint location, GLsizei bufSize, GLint* params);
SIGL_FUNC_DECLARE(void, glGetnUniformuiv, GLuint program, GLint location, GLsizei bufSize, GLuint* params);
SIGL_FUNC_DECLARE(void, glReadnPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);
SIGL_FUNC_DECLARE(void, glGetnMapdv, GLenum target, GLenum query, GLsizei bufSize, GLdouble* v);
SIGL_FUNC_DECLARE(void, glGetnMapfv, GLenum target, GLenum query, GLsizei bufSize, GLfloat* v);
SIGL_FUNC_DECLARE(void, glGetnMapiv, GLenum target, GLenum query, GLsizei bufSize, GLint* v);
SIGL_FUNC_DECLARE(void, glGetnPixelMapfv, GLenum map, GLsizei bufSize, GLfloat* values);
SIGL_FUNC_DECLARE(void, glGetnPixelMapuiv, GLenum map, GLsizei bufSize, GLuint* values);
SIGL_FUNC_DECLARE(void, glGetnPixelMapusv, GLenum map, GLsizei bufSize, GLushort* values);
SIGL_FUNC_DECLARE(void, glGetnPolygonStipple, GLsizei bufSize, GLubyte* pattern);
SIGL_FUNC_DECLARE(void, glGetnColorTable, GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* table);
SIGL_FUNC_DECLARE(void, glGetnConvolutionFilter, GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* image);
SIGL_FUNC_DECLARE(void, glGetnSeparableFilter, GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void* row, GLsizei columnBufSize, void* column, void* span);
SIGL_FUNC_DECLARE(void, glGetnHistogram, GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
SIGL_FUNC_DECLARE(void, glGetnMinmax, GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
SIGL_FUNC_DECLARE(void, glTextureBarrier, void);
#endif /* !SIGL_NO_GL_4_5_H_FUNCS */
#if !defined(SIGL_NO_GL_4_6_H_FUNCS)
SIGL_FUNC_DECLARE(void, glSpecializeShader, GLuint, const GLchar*, GLuint, const GLuint*, const GLuint*);
SIGL_FUNC_DECLARE(void, glMultiDrawArraysIndirectCount, GLenum, const void*, GLintptr, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glMultiDrawElementsIndirectCount, GLenum, GLenum, const void*, GLintptr, GLsizei, GLsizei);
SIGL_FUNC_DECLARE(void, glPolygonOffsetClamp, GLfloat, GLfloat, GLfloat);
#endif /* !SIGL_NO_GL_4_6_H_FUNCS */
#if SIGL_SYSTEM_IS_WINDOWS && !defined(SIGL_NO_WGL_H_FUNCS)
#if defined(WGL_VERSION_1_0) && defined(SIGL_NO_WINDOWS_H)
SIGL_FUNC_DECLARE(int, ChoosePixelFormat, HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd);
SIGL_FUNC_DECLARE(int, DescribePixelFormat, HDC hdc, int iPixelFormat, UINT nBytes, PIXELFORMATDESCRIPTOR* ppfd);
SIGL_FUNC_DECLARE(UINT, GetEnhMetaFilePixelFormat, HENHMETAFILE hemf, const PIXELFORMATDESCRIPTOR* ppfd);
SIGL_FUNC_DECLARE(int, GetPixelFormat, HDC hdc);
SIGL_FUNC_DECLARE(BOOL, SetPixelFormat, HDC hdc, int iPixelFormat, const PIXELFORMATDESCRIPTOR* ppfd);
SIGL_FUNC_DECLARE(BOOL, SwapBuffers, HDC hdc);
SIGL_FUNC_DECLARE(BOOL, wglCopyContext, HGLRC hglrcSrc, HGLRC hglrcDst, UINT mask);
SIGL_FUNC_DECLARE(HGLRC, wglCreateContext, HDC hdc);
SIGL_FUNC_DECLARE(HGLRC, wglCreateLayerContext, HDC hdc, int iLayerPlane);
SIGL_FUNC_DECLARE(BOOL, wglDeleteContext, HGLRC hglrc);
SIGL_FUNC_DECLARE(BOOL, wglDescribeLayerPlane, HDC hdc, int iPixelFormat, int iLayerPlane, UINT nBytes, LAYERPLANEDESCRIPTOR* plpd);
SIGL_FUNC_DECLARE(HGLRC, wglGetCurrentContext, void);
SIGL_FUNC_DECLARE(HDC, wglGetCurrentDC, void);
SIGL_FUNC_DECLARE(int, wglGetLayerPaletteEntries, HDC hdc, int iLayerPlane, int iStart, int cEntries, const COLORREF* pcr);
SIGL_FUNC_DECLARE(PROC, wglGetProcAddress, LPCSTR lpszProc);
SIGL_FUNC_DECLARE(BOOL, wglMakeCurrent, HDC hdc, HGLRC hglrc);
SIGL_FUNC_DECLARE(BOOL, wglRealizeLayerPalette, HDC hdc, int iLayerPlane, BOOL bRealize);
SIGL_FUNC_DECLARE(int, wglSetLayerPaletteEntries, HDC hdc, int iLayerPlane, int iStart, int cEntries, const COLORREF* pcr);
SIGL_FUNC_DECLARE(BOOL, wglShareLists, HGLRC hglrc1, HGLRC hglrc2);
SIGL_FUNC_DECLARE(BOOL, wglSwapLayerBuffers, HDC hdc, UINT fuFlags);
SIGL_FUNC_DECLARE(BOOL, wglUseFontBitmaps, HDC hdc, DWORD first, DWORD count, DWORD listBase);
SIGL_FUNC_DECLARE(BOOL, wglUseFontBitmapsA, HDC hdc, DWORD first, DWORD count, DWORD listBase);
SIGL_FUNC_DECLARE(BOOL, wglUseFontBitmapsW, HDC hdc, DWORD first, DWORD count, DWORD listBase);
SIGL_FUNC_DECLARE(BOOL, wglUseFontOutlines, HDC hdc, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf);
SIGL_FUNC_DECLARE(BOOL, wglUseFontOutlinesA, HDC hdc, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf);
SIGL_FUNC_DECLARE(BOOL, wglUseFontOutlinesW, HDC hdc, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf);
#endif /* WGL_VERSION_1_0 */

#if defined(WGL_ARB_buffer_region) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(HANDLE, wglCreateBufferRegionARB, HDC hDC, int layerPlane, UINT type);
SIGL_FUNC_DECLARE(VOID, wglDeleteBufferRegionARB, HANDLE hRegion);
SIGL_FUNC_DECLARE(BOOL, wglSaveBufferRegionARB, HANDLE hRegion, int x, int y, int width, int height);
SIGL_FUNC_DECLARE(BOOL, wglRestoreBufferRegionARB, HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);
#endif /* WGL_ARB_buffer_region */

#if defined(WGL_ARB_create_context) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(HGLRC, wglCreateContextAttribsARB, HDC hDC, HGLRC hShareContext, const int* attribList);
#endif

#if defined(WGL_ARB_extensions_string) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(const char*, wglGetExtensionsStringARB, HDC hDC);
#endif

#if defined(WGL_ARB_make_current_read) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglMakeContextCurrentARB, HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
SIGL_FUNC_DECLARE(HDC, wglGetCurrentReadDCARB, void);
#endif

#if defined(WGL_ARB_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(HPBUFFERARB, wglCreatePbufferARB, HDC hDC, int pixelFormat, int width, int height, const int* attribList);
SIGL_FUNC_DECLARE(HDC, wglGetPbufferDCARB, HPBUFFERARB hPbuffer);
SIGL_FUNC_DECLARE(int, wglReleasePbufferDCARB, HPBUFFERARB hPbuffer, HDC hDC);
SIGL_FUNC_DECLARE(BOOL, wglDestroyPbufferARB, HPBUFFERARB hPbuffer);
SIGL_FUNC_DECLARE(BOOL, wglQueryPbufferARB, HPBUFFERARB hPbuffer, int iAttribute, int* piValue);
#endif

#if defined(WGL_ARB_pixel_format) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetPixelFormatAttribivARB, HDC hDC, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, int* piValues);
SIGL_FUNC_DECLARE(BOOL, wglGetPixelFormatAttribfvARB, HDC hDC, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, FLOAT* pfValues);
SIGL_FUNC_DECLARE(BOOL, wglChoosePixelFormatARB, HDC hDC, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
#endif

#if defined(WGL_ARB_render_texture) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglBindTexImageARB, HPBUFFERARB hPbuffer, int iBuffer);
SIGL_FUNC_DECLARE(BOOL, wglReleaseTexImageARB, HPBUFFERARB hPbuffer, int iBuffer);
SIGL_FUNC_DECLARE(BOOL, wglSetPbufferAttribARB, HPBUFFERARB hPbuffer, const int* piAttribList);
#endif

#if defined(WGL_3DL_stereo_control) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglSetStereoEmitterState3DL, HDC hDC, UINT uState);
#endif

#if defined(WGL_AMD_gpu_association) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(UINT, wglGetGPUIDsAMD, UINT maxCount, UINT* ids);
SIGL_FUNC_DECLARE(INT, wglGetGPUInfoAMD, UINT id, INT property, GLenum dataType, UINT size, void* data);
SIGL_FUNC_DECLARE(UINT, wglGetContextGPUIDAMD, HGLRC hglrc);
SIGL_FUNC_DECLARE(HGLRC, wglCreateAssociatedContextAMD, UINT id);
SIGL_FUNC_DECLARE(HGLRC, wglCreateAssociatedContextAttribsAMD, UINT id, HGLRC hShareContext, const int* attribList);
SIGL_FUNC_DECLARE(BOOL, wglDeleteAssociatedContextAMD, HGLRC hglrc);
SIGL_FUNC_DECLARE(BOOL, wglMakeAssociatedContextCurrentAMD, HGLRC hglrc);
SIGL_FUNC_DECLARE(HGLRC, wglGetCurrentAssociatedContextAMD, void);
SIGL_FUNC_DECLARE(VOID, wglBlitContextFramebufferAMD, HGLRC hSrcRC, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
#endif

#if defined(WGL_EXT_display_color_table) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(GLboolean, wglCreateDisplayColorTableEXT, GLushort id);
SIGL_FUNC_DECLARE(GLboolean, wglLoadDisplayColorTableEXT, const GLushort* table, GLuint length);
SIGL_FUNC_DECLARE(GLboolean, wglBindDisplayColorTableEXT, GLushort id);
SIGL_FUNC_DECLARE(VOID, wglDestroyDisplayColorTableEXT, GLushort id);
#endif

#if defined(WGL_EXT_extensions_string) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(const char*, wglGetExtensionsStringEXT, void);
#endif

#if defined(WGL_EXT_make_current_read) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglMakeContextCurrentEXT, HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
SIGL_FUNC_DECLARE(HDC, wglGetCurrentReadDCEXT, void);
#endif

#if defined(WGL_EXT_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(HPBUFFEREXT, wglCreatePbufferEXT, HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int* piAttribList);
SIGL_FUNC_DECLARE(HDC, wglGetPbufferDCEXT, HPBUFFEREXT hPbuffer);
SIGL_FUNC_DECLARE(int, wglReleasePbufferDCEXT, HPBUFFEREXT hPbuffer, HDC hDC);
SIGL_FUNC_DECLARE(BOOL, wglDestroyPbufferEXT, HPBUFFEREXT hPbuffer);
SIGL_FUNC_DECLARE(BOOL, wglQueryPbufferEXT, HPBUFFEREXT hPbuffer, int iAttribute, int* piValue);
#endif

#if defined(WGL_EXT_pixel_format) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetPixelFormatAttribivEXT, HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int* piAttributes, int* piValues);
SIGL_FUNC_DECLARE(BOOL, wglGetPixelFormatAttribfvEXT, HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int* piAttributes, FLOAT* pfValues);
SIGL_FUNC_DECLARE(BOOL, wglChoosePixelFormatEXT, HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
#endif

#if defined(WGL_EXT_swap_control) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglSwapIntervalEXT, int interval);
SIGL_FUNC_DECLARE(int, wglGetSwapIntervalEXT, void);
#endif

#if defined(WGL_I3D_digital_video_control) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetDigitalVideoParametersI3D, HDC hDC, int iAttribute, int* piValue);
SIGL_FUNC_DECLARE(BOOL, wglSetDigitalVideoParametersI3D, HDC hDC, int iAttribute, const int* piValue);
#endif

#if defined(WGL_I3D_gamma) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetGammaTableParametersI3D, HDC hDC, int iAttribute, int* piValue);
SIGL_FUNC_DECLARE(BOOL, wglSetGammaTableParametersI3D, HDC hDC, int iAttribute, const int* piValue);
SIGL_FUNC_DECLARE(BOOL, wglGetGammaTableI3D, HDC hDC, int iEntries, USHORT* puRed, USHORT* puGreen, USHORT* puBlue);
SIGL_FUNC_DECLARE(BOOL, wglSetGammaTableI3D, HDC hDC, int iEntries, const USHORT* puRed, const USHORT* puGreen, const USHORT* puBlue);
#endif

#if defined(WGL_I3D_genlock) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglEnableGenlockI3D, HDC hDC);
SIGL_FUNC_DECLARE(BOOL, wglDisableGenlockI3D, HDC hDC);
SIGL_FUNC_DECLARE(BOOL, wglIsEnabledGenlockI3D, HDC hDC, BOOL* pFlag);
SIGL_FUNC_DECLARE(BOOL, wglGenlockSourceI3D, HDC hDC, UINT uSource);
SIGL_FUNC_DECLARE(BOOL, wglGetGenlockSourceI3D, HDC hDC, UINT* uSource);
SIGL_FUNC_DECLARE(BOOL, wglGenlockSourceEdgeI3D, HDC hDC, UINT uEdge);
SIGL_FUNC_DECLARE(BOOL, wglGetGenlockSourceEdgeI3D, HDC hDC, UINT* uEdge);
SIGL_FUNC_DECLARE(BOOL, wglGenlockSampleRateI3D, HDC hDC, UINT uRate);
SIGL_FUNC_DECLARE(BOOL, wglGetGenlockSampleRateI3D, HDC hDC, UINT* uRate);
SIGL_FUNC_DECLARE(BOOL, wglGenlockSourceDelayI3D, HDC hDC, UINT uDelay);
SIGL_FUNC_DECLARE(BOOL, wglGetGenlockSourceDelayI3D, HDC hDC, UINT* uDelay);
SIGL_FUNC_DECLARE(BOOL, wglQueryGenlockMaxSourceDelayI3D, HDC hDC, UINT* uMaxLineDelay, UINT* uMaxPixelDelay);
#endif

#if defined(WGL_I3D_image_buffer) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(LPVOID, wglCreateImageBufferI3D, HDC hDC, DWORD dwSize, UINT uFlags);
SIGL_FUNC_DECLARE(BOOL, wglDestroyImageBufferI3D, HDC hDC, LPVOID pAddress);
SIGL_FUNC_DECLARE(BOOL, wglAssociateImageBufferEventsI3D, HDC hDC, const HANDLE* pEvent, const LPVOID* pAddress, const DWORD* pSize, UINT count);
SIGL_FUNC_DECLARE(BOOL, wglReleaseImageBufferEventsI3D, HDC hDC, const LPVOID* pAddress, UINT count);
#endif

#if defined(WGL_I3D_swap_frame_lock) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglEnableFrameLockI3D, void);
SIGL_FUNC_DECLARE(BOOL, wglDisableFrameLockI3D, void);
SIGL_FUNC_DECLARE(BOOL, wglIsEnabledFrameLockI3D, BOOL* pFlag);
SIGL_FUNC_DECLARE(BOOL, wglQueryFrameLockMasterI3D, BOOL* pFlag);
#endif

#if defined(WGL_I3D_swap_frame_usage) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetFrameUsageI3D, float* pUsage);
SIGL_FUNC_DECLARE(BOOL, wglBeginFrameTrackingI3D, void);
SIGL_FUNC_DECLARE(BOOL, wglEndFrameTrackingI3D, void);
SIGL_FUNC_DECLARE(BOOL, wglQueryFrameTrackingI3D, DWORD* pFrameCount, DWORD* pMissedFrames, float* pLastMissedUsage);
#endif

#if defined(WGL_NV_DX_interop) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglDXSetResourceShareHandleNV, void* dxObject, HANDLE shareHandle);
SIGL_FUNC_DECLARE(HANDLE, wglDXOpenDeviceNV, void* dxDevice);
SIGL_FUNC_DECLARE(BOOL, wglDXCloseDeviceNV, HANDLE hDevice);
SIGL_FUNC_DECLARE(HANDLE, wglDXRegisterObjectNV, HANDLE hDevice, void* dxObject, GLuint name, GLenum type, GLenum access);
SIGL_FUNC_DECLARE(BOOL, wglDXUnregisterObjectNV, HANDLE hDevice, HANDLE hObject);
SIGL_FUNC_DECLARE(BOOL, wglDXObjectAccessNV, HANDLE hObject, GLenum access);
SIGL_FUNC_DECLARE(BOOL, wglDXLockObjectsNV, HANDLE hDevice, GLint count, HANDLE* hObjects);
SIGL_FUNC_DECLARE(BOOL, wglDXUnlockObjectsNV, HANDLE hDevice, GLint count, HANDLE* hObjects);
#endif

#if defined(WGL_NV_copy_image) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglCopyImageSubDataNV, HGLRC hSrcRC, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, HGLRC hDstRC, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
#endif

#if defined(WGL_NV_gpu_affinity) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglEnumGpusNV, UINT iGpuIndex, HGPUNV* phGpu);
SIGL_FUNC_DECLARE(BOOL, wglEnumGpuDevicesNV, HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE pGpuDevice);
SIGL_FUNC_DECLARE(HDC, wglCreateAffinityDCNV, const HGPUNV* phGpuList);
SIGL_FUNC_DECLARE(BOOL, wglEnumGpusFromAffinityDCNV, HDC hAffinityDC, UINT iGpuIndex, HGPUNV* phGpu);
SIGL_FUNC_DECLARE(BOOL, wglDeleteDCNV, HDC hdc);
#endif

#if defined(WGL_NV_present_video) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(int, wglEnumerateVideoDevicesNV, HDC hDC, HVIDEOOUTPUTDEVICENV* pDeviceList);
SIGL_FUNC_DECLARE(BOOL, wglBindVideoDeviceNV, HDC hDC, unsigned int uVideoSlot, HVIDEOOUTPUTDEVICENV hVideoDevice, const int* piAttribList);
SIGL_FUNC_DECLARE(BOOL, wglQueryCurrentContextNV, int iAttribute, int* piValue);
#endif

#if defined(WGL_NV_swap_group) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglJoinSwapGroupNV, HDC hDC, GLuint group);
SIGL_FUNC_DECLARE(BOOL, wglBindSwapBarrierNV, GLuint group, GLuint barrier);
SIGL_FUNC_DECLARE(BOOL, wglQuerySwapGroupNV, HDC hDC, GLuint* group, GLuint* barrier);
SIGL_FUNC_DECLARE(BOOL, wglQueryMaxSwapGroupsNV, HDC hDC, GLuint* maxGroups, GLuint* maxBarriers);
SIGL_FUNC_DECLARE(BOOL, wglQueryFrameCountNV, HDC hDC, GLuint* count);
SIGL_FUNC_DECLARE(BOOL, wglResetFrameCountNV, HDC hDC);
#endif

#if defined(WGL_NV_vertex_array_range) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(void*, wglAllocateMemoryNV, GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority);
SIGL_FUNC_DECLARE(void, wglFreeMemoryNV, void* pointer);
#endif

#if defined(WGL_NV_video_capture) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglBindVideoCaptureDeviceNV, UINT uVideoSlot, HVIDEOINPUTDEVICENV hDevice);
SIGL_FUNC_DECLARE(UINT, wglEnumerateVideoCaptureDevicesNV, HDC hDC, HVIDEOINPUTDEVICENV* phDeviceList);
SIGL_FUNC_DECLARE(BOOL, wglLockVideoCaptureDeviceNV, HDC hDC, HVIDEOINPUTDEVICENV hDevice);
SIGL_FUNC_DECLARE(BOOL, wglQueryVideoCaptureDeviceNV, HDC hDC, HVIDEOINPUTDEVICENV hDevice, int iAttribute, int* piValue);
SIGL_FUNC_DECLARE(BOOL, wglReleaseVideoCaptureDeviceNV, HDC hDC, HVIDEOINPUTDEVICENV hDevice);
#endif

#if defined(WGL_NV_video_output) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_FUNC_DECLARE(BOOL, wglGetVideoDeviceNV, HDC hDC, int numDevices, HPVIDEODEV* hVideoDevice);
SIGL_FUNC_DECLARE(BOOL, wglReleaseVideoDeviceNV, HPVIDEODEV hVideoDevice);
SIGL_FUNC_DECLARE(BOOL, wglBindVideoImageNV, HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer);
SIGL_FUNC_DECLARE(BOOL, wglReleaseVideoImageNV, HPBUFFERARB hPbuffer, int iVideoBuffer);
SIGL_FUNC_DECLARE(BOOL, wglSendPbufferToVideoNV, HPBUFFERARB hPbuffer, int iBufferType, unsigned long* pulCounterPbuffer, BOOL bBlock);
SIGL_FUNC_DECLARE(BOOL, wglGetVideoInfoNV, HPVIDEODEV hpVideoDevice, unsigned long* pulCounterOutputPbuffer, unsigned long* pulCounterOutputVideo);
SIGL_FUNC_DECLARE(BOOL, wglGetSyncValuesOML, HDC hdc, INT64* ust, INT64* msc, INT64* sbc);
SIGL_FUNC_DECLARE(BOOL, wglGetMscRateOML, HDC hdc, INT32* numerator, INT32* denominator);
SIGL_FUNC_DECLARE(INT64, wglSwapBuffersMscOML, HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder);
SIGL_FUNC_DECLARE(INT64, wglSwapLayerBuffersMscOML, HDC hdc, INT fuPlanes, INT64 target_msc, INT64 divisor, INT64 remainder);
SIGL_FUNC_DECLARE(BOOL, wglWaitForMscOML, HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder, INT64* ust, INT64* msc, INT64* sbc);
SIGL_FUNC_DECLARE(BOOL, wglWaitForSbcOML, HDC hdc, INT64 target_sbc, INT64* ust, INT64* msc, INT64* sbc);
#endif

#endif

#if SIGL_SYSTEM_IS_UNIX
SIGL_FUNC_DECLARE(void, glXSwapIntervalEXT, Display *dpy, GLXDrawable drawable, int interval);
#endif

#if SIGL_SYSTEM_IS_WINDOWS
	#define SIGL_PROC_LOAD(name) (wglGetProcAddress(#name))
#else
	#define SIGL_PROC_LOAD(name) (glXGetProcAddress((GLubyte*)#name))
#endif

#define SIGL_PROC_DEF(name) \
    {  /* NOTE(EimaMei): A glued-together solution to get around win32's horrible
          design of using PROC instead of void* as the return type. May or may
          not break strict-aliasing, I don't even know or care anymore. */ \
        union { siglOsFuncType proc; name##SIPROC actualType; } value; \
        value.proc = SIGL_PROC_LOAD(name); \
        name = value.actualType; \
    }

#if !SIGL_SYSTEM_IS_WINDOWS && !SIGL_SYSTEM_IS_UNIX
	#undef SIGL_PROC_DEF
	#define SIGL_PROC_DEF(name)
#endif

void sigl_loadOpenGLAll(void) {
	sigl_loadOpenGL_1_2();
	sigl_loadOpenGL_1_3();
	sigl_loadOpenGL_1_4();
	sigl_loadOpenGL_1_5();
	sigl_loadOpenGL_2_0();
	sigl_loadOpenGL_2_1();
	sigl_loadOpenGL_3_0();
	sigl_loadOpenGL_3_1();
	sigl_loadOpenGL_3_2();
	sigl_loadOpenGL_3_3();
	sigl_loadOpenGL_4_0();
	sigl_loadOpenGL_4_1();
	sigl_loadOpenGL_4_2();
	sigl_loadOpenGL_4_3();
	sigl_loadOpenGL_4_4();
	sigl_loadOpenGL_4_5();
	sigl_loadOpenGL_4_6();

	sigl_loadOpenGLOS();
}
void sigl_loadOpenGLAllVer(int major, int minor, int osFuncs) {
	int ogMinor = minor;
	if (major >= 1) {
		minor = (major != 1) ? 5 : ogMinor;
		if (minor >= 2) { sigl_loadOpenGL_1_2(); }
		if (minor >= 3) { sigl_loadOpenGL_1_3(); }
		if (minor >= 4) { sigl_loadOpenGL_1_4(); }
		if (minor >= 5) { sigl_loadOpenGL_1_5(); }
	}

	if (major >= 2) {
		minor = (major != 2) ? 1 : ogMinor;
		if (minor >= 0) { sigl_loadOpenGL_2_0(); }
		if (minor >= 1) { sigl_loadOpenGL_2_1(); }
	}

	if (major >= 3) {
		minor = (major != 3) ? 3 : ogMinor;
		if (minor >= 0) { sigl_loadOpenGL_3_0(); }
		if (minor >= 1) { sigl_loadOpenGL_3_1(); }
		if (minor >= 2) { sigl_loadOpenGL_3_2(); }
		if (minor >= 3) { sigl_loadOpenGL_3_3(); }
	}

	if (major >= 4) {
		minor = ogMinor;
		if (minor >= 0) { sigl_loadOpenGL_4_0(); }
		if (minor >= 1) { sigl_loadOpenGL_4_1(); }
		if (minor >= 2) { sigl_loadOpenGL_4_2(); }
		if (minor >= 3) { sigl_loadOpenGL_4_3(); }
		if (minor >= 4) { sigl_loadOpenGL_4_4(); }
		if (minor >= 5) { sigl_loadOpenGL_4_5(); }
		if (minor >= 6) { sigl_loadOpenGL_4_6(); }
	}

	if (osFuncs) {
		sigl_loadOpenGLOS();
	}
}
void sigl_loadOpenGL_1_2(void) {
#if !defined(SIGL_NO_GL_1_2_H_FUNCS)
	SIGL_PROC_DEF(glDrawRangeElements);
	SIGL_PROC_DEF(glTexImage3D);
	SIGL_PROC_DEF(glTexSubImage3D);
	SIGL_PROC_DEF(glCopyTexSubImage3D);
#endif /* SIGL_NO_GL_1_2_H_FUNCS */
}
void sigl_loadOpenGL_1_3(void) {
#if !defined(SIGL_NO_GL_1_3_H_FUNCS)
	SIGL_PROC_DEF(glActiveTexture);
	SIGL_PROC_DEF(glSampleCoverage);
	SIGL_PROC_DEF(glCompressedTexImage3D);
	SIGL_PROC_DEF(glCompressedTexImage2D);
	SIGL_PROC_DEF(glCompressedTexImage1D);
	SIGL_PROC_DEF(glCompressedTexSubImage3D);
	SIGL_PROC_DEF(glCompressedTexSubImage2D);
	SIGL_PROC_DEF(glCompressedTexSubImage1D);
	SIGL_PROC_DEF(glGetCompressedTexImage);
	SIGL_PROC_DEF(glClientActiveTexture);
	SIGL_PROC_DEF(glMultiTexCoord1d);
	SIGL_PROC_DEF(glMultiTexCoord1dv);
	SIGL_PROC_DEF(glMultiTexCoord1f);
	SIGL_PROC_DEF(glMultiTexCoord1fv);
	SIGL_PROC_DEF(glMultiTexCoord1i);
	SIGL_PROC_DEF(glMultiTexCoord1iv);
	SIGL_PROC_DEF(glMultiTexCoord1s);
	SIGL_PROC_DEF(glMultiTexCoord1sv);
	SIGL_PROC_DEF(glMultiTexCoord2d);
	SIGL_PROC_DEF(glMultiTexCoord2dv);
	SIGL_PROC_DEF(glMultiTexCoord2f);
	SIGL_PROC_DEF(glMultiTexCoord2fv);
	SIGL_PROC_DEF(glMultiTexCoord2i);
	SIGL_PROC_DEF(glMultiTexCoord2iv);
	SIGL_PROC_DEF(glMultiTexCoord2s);
	SIGL_PROC_DEF(glMultiTexCoord2sv);
	SIGL_PROC_DEF(glMultiTexCoord3d);
	SIGL_PROC_DEF(glMultiTexCoord3dv);
	SIGL_PROC_DEF(glMultiTexCoord3f);
	SIGL_PROC_DEF(glMultiTexCoord3fv);
	SIGL_PROC_DEF(glMultiTexCoord3i);
	SIGL_PROC_DEF(glMultiTexCoord3iv);
	SIGL_PROC_DEF(glMultiTexCoord3s);
	SIGL_PROC_DEF(glMultiTexCoord3sv);
	SIGL_PROC_DEF(glMultiTexCoord4d);
	SIGL_PROC_DEF(glMultiTexCoord4dv);
	SIGL_PROC_DEF(glMultiTexCoord4f);
	SIGL_PROC_DEF(glMultiTexCoord4fv);
	SIGL_PROC_DEF(glMultiTexCoord4i);
	SIGL_PROC_DEF(glMultiTexCoord4iv);
	SIGL_PROC_DEF(glMultiTexCoord4s);
	SIGL_PROC_DEF(glMultiTexCoord4sv);
	SIGL_PROC_DEF(glLoadTransposeMatrixf);
	SIGL_PROC_DEF(glLoadTransposeMatrixd);
	SIGL_PROC_DEF(glMultTransposeMatrixf);
	SIGL_PROC_DEF(glMultTransposeMatrixd);
#endif /* SIGL_NO_GL_1_3_H_FUNCS */
}
void sigl_loadOpenGL_1_4(void) {
#if !defined(SIGL_NO_GL_1_4_H_FUNCS)
	SIGL_PROC_DEF(glBlendFuncSeparate);
	SIGL_PROC_DEF(glMultiDrawArrays);
	SIGL_PROC_DEF(glMultiDrawElements);
	SIGL_PROC_DEF(glPointParameterf);
	SIGL_PROC_DEF(glPointParameterfv);
	SIGL_PROC_DEF(glPointParameteri);
	SIGL_PROC_DEF(glPointParameteriv);
	SIGL_PROC_DEF(glFogCoordf);
	SIGL_PROC_DEF(glFogCoordfv);
	SIGL_PROC_DEF(glFogCoordd);
	SIGL_PROC_DEF(glFogCoorddv);
	SIGL_PROC_DEF(glFogCoordPointer);
	SIGL_PROC_DEF(glSecondaryColor3b);
	SIGL_PROC_DEF(glSecondaryColor3bv);
	SIGL_PROC_DEF(glSecondaryColor3d);
	SIGL_PROC_DEF(glSecondaryColor3dv);
	SIGL_PROC_DEF(glSecondaryColor3f);
	SIGL_PROC_DEF(glSecondaryColor3fv);
	SIGL_PROC_DEF(glSecondaryColor3i);
	SIGL_PROC_DEF(glSecondaryColor3iv);
	SIGL_PROC_DEF(glSecondaryColor3s);
	SIGL_PROC_DEF(glSecondaryColor3sv);
	SIGL_PROC_DEF(glSecondaryColor3ub);
	SIGL_PROC_DEF(glSecondaryColor3ubv);
	SIGL_PROC_DEF(glSecondaryColor3ui);
	SIGL_PROC_DEF(glSecondaryColor3uiv);
	SIGL_PROC_DEF(glSecondaryColor3us);
	SIGL_PROC_DEF(glSecondaryColor3usv);
	SIGL_PROC_DEF(glSecondaryColorPointer);
	SIGL_PROC_DEF(glWindowPos2d);
	SIGL_PROC_DEF(glWindowPos2dv);
	SIGL_PROC_DEF(glWindowPos2f);
	SIGL_PROC_DEF(glWindowPos2fv);
	SIGL_PROC_DEF(glWindowPos2i);
	SIGL_PROC_DEF(glWindowPos2iv);
	SIGL_PROC_DEF(glWindowPos2s);
	SIGL_PROC_DEF(glWindowPos2sv);
	SIGL_PROC_DEF(glWindowPos3d);
	SIGL_PROC_DEF(glWindowPos3dv);
	SIGL_PROC_DEF(glWindowPos3f);
	SIGL_PROC_DEF(glWindowPos3fv);
	SIGL_PROC_DEF(glWindowPos3i);
	SIGL_PROC_DEF(glWindowPos3iv);
	SIGL_PROC_DEF(glWindowPos3s);
	SIGL_PROC_DEF(glWindowPos3sv);
	SIGL_PROC_DEF(glBlendColor);
	SIGL_PROC_DEF(glBlendEquation);
#endif
}
void sigl_loadOpenGL_1_5(void) {
#if !defined(SIGL_NO_GL_1_5_H_FUNCS)
	SIGL_PROC_DEF(glGenQueries);
	SIGL_PROC_DEF(glDeleteQueries);
	SIGL_PROC_DEF(glIsQuery);
	SIGL_PROC_DEF(glBeginQuery);
	SIGL_PROC_DEF(glEndQuery);
	SIGL_PROC_DEF(glGetQueryiv);
	SIGL_PROC_DEF(glGetQueryObjectiv);
	SIGL_PROC_DEF(glGetQueryObjectuiv);
	SIGL_PROC_DEF(glBindBuffer);
	SIGL_PROC_DEF(glDeleteBuffers);
	SIGL_PROC_DEF(glGenBuffers);
	SIGL_PROC_DEF(glIsBuffer);
	SIGL_PROC_DEF(glBufferData);
	SIGL_PROC_DEF(glBufferSubData);
	SIGL_PROC_DEF(glGetBufferSubData);
	SIGL_PROC_DEF(glMapBuffer);
	SIGL_PROC_DEF(glUnmapBuffer);
	SIGL_PROC_DEF(glGetBufferParameteriv);
	SIGL_PROC_DEF(glGetBufferPointerv);
#endif /* !SIGL_NO_GL_1_5_H_FUNCS */
}
void sigl_loadOpenGL_2_0(void) {
#if !defined(SIGL_NO_GL_2_0_H_FUNCS)
	SIGL_PROC_DEF(glBlendEquationSeparate);
	SIGL_PROC_DEF(glDrawBuffers);
	SIGL_PROC_DEF(glStencilOpSeparate);
	SIGL_PROC_DEF(glStencilFuncSeparate);
	SIGL_PROC_DEF(glStencilMaskSeparate);
	SIGL_PROC_DEF(glAttachShader);
	SIGL_PROC_DEF(glBindAttribLocation);
	SIGL_PROC_DEF(glCompileShader);
	SIGL_PROC_DEF(glCreateProgram);
	SIGL_PROC_DEF(glCreateShader);
	SIGL_PROC_DEF(glDeleteProgram);
	SIGL_PROC_DEF(glDeleteShader);
	SIGL_PROC_DEF(glDetachShader);
	SIGL_PROC_DEF(glDisableVertexAttribArray);
	SIGL_PROC_DEF(glEnableVertexAttribArray);
	SIGL_PROC_DEF(glGetActiveAttrib);
	SIGL_PROC_DEF(glGetActiveUniform);
	SIGL_PROC_DEF(glGetAttachedShaders);
	SIGL_PROC_DEF(glGetAttribLocation);
	SIGL_PROC_DEF(glGetProgramiv);
	SIGL_PROC_DEF(glGetProgramInfoLog);
	SIGL_PROC_DEF(glGetShaderiv);
	SIGL_PROC_DEF(glGetShaderInfoLog);
	SIGL_PROC_DEF(glGetShaderSource);
	SIGL_PROC_DEF(glGetUniformLocation);
	SIGL_PROC_DEF(glGetUniformfv);
	SIGL_PROC_DEF(glGetUniformiv);
	SIGL_PROC_DEF(glGetVertexAttribdv);
	SIGL_PROC_DEF(glGetVertexAttribfv);
	SIGL_PROC_DEF(glGetVertexAttribiv);
	SIGL_PROC_DEF(glGetVertexAttribPointerv);
	SIGL_PROC_DEF(glIsProgram);
	SIGL_PROC_DEF(glIsShader);
	SIGL_PROC_DEF(glLinkProgram);
	SIGL_PROC_DEF(glShaderSource);
	SIGL_PROC_DEF(glUseProgram);
	SIGL_PROC_DEF(glUniform1f);
	SIGL_PROC_DEF(glUniform2f);
	SIGL_PROC_DEF(glUniform3f);
	SIGL_PROC_DEF(glUniform4f);
	SIGL_PROC_DEF(glUniform1i);
	SIGL_PROC_DEF(glUniform2i);
	SIGL_PROC_DEF(glUniform3i);
	SIGL_PROC_DEF(glUniform4i);
	SIGL_PROC_DEF(glUniform1fv);
	SIGL_PROC_DEF(glUniform2fv);
	SIGL_PROC_DEF(glUniform3fv);
	SIGL_PROC_DEF(glUniform4fv);
	SIGL_PROC_DEF(glUniform1iv);
	SIGL_PROC_DEF(glUniform2iv);
	SIGL_PROC_DEF(glUniform3iv);
	SIGL_PROC_DEF(glUniform4iv);
	SIGL_PROC_DEF(glUniformMatrix2fv);
	SIGL_PROC_DEF(glUniformMatrix3fv);
	SIGL_PROC_DEF(glUniformMatrix4fv);
	SIGL_PROC_DEF(glValidateProgram);
	SIGL_PROC_DEF(glVertexAttrib1d);
	SIGL_PROC_DEF(glVertexAttrib1dv);
	SIGL_PROC_DEF(glVertexAttrib1f);
	SIGL_PROC_DEF(glVertexAttrib1fv);
	SIGL_PROC_DEF(glVertexAttrib1s);
	SIGL_PROC_DEF(glVertexAttrib1sv);
	SIGL_PROC_DEF(glVertexAttrib2d);
	SIGL_PROC_DEF(glVertexAttrib2dv);
	SIGL_PROC_DEF(glVertexAttrib2f);
	SIGL_PROC_DEF(glVertexAttrib2fv);
	SIGL_PROC_DEF(glVertexAttrib2s);
	SIGL_PROC_DEF(glVertexAttrib2sv);
	SIGL_PROC_DEF(glVertexAttrib3d);
	SIGL_PROC_DEF(glVertexAttrib3dv);
	SIGL_PROC_DEF(glVertexAttrib3f);
	SIGL_PROC_DEF(glVertexAttrib3fv);
	SIGL_PROC_DEF(glVertexAttrib3s);
	SIGL_PROC_DEF(glVertexAttrib3sv);
	SIGL_PROC_DEF(glVertexAttrib4Nbv);
	SIGL_PROC_DEF(glVertexAttrib4Niv);
	SIGL_PROC_DEF(glVertexAttrib4Nsv);
	SIGL_PROC_DEF(glVertexAttrib4Nub);
	SIGL_PROC_DEF(glVertexAttrib4Nubv);
	SIGL_PROC_DEF(glVertexAttrib4Nuiv);
	SIGL_PROC_DEF(glVertexAttrib4Nusv);
	SIGL_PROC_DEF(glVertexAttrib4bv);
	SIGL_PROC_DEF(glVertexAttrib4d);
	SIGL_PROC_DEF(glVertexAttrib4dv);
	SIGL_PROC_DEF(glVertexAttrib4f);
	SIGL_PROC_DEF(glVertexAttrib4fv);
	SIGL_PROC_DEF(glVertexAttrib4iv);
	SIGL_PROC_DEF(glVertexAttrib4s);
	SIGL_PROC_DEF(glVertexAttrib4sv);
	SIGL_PROC_DEF(glVertexAttrib4ubv);
	SIGL_PROC_DEF(glVertexAttrib4uiv);
	SIGL_PROC_DEF(glVertexAttrib4usv);
	SIGL_PROC_DEF(glVertexAttribPointer);
#endif /* !SIGL_NO_GL_2_0_H_FUNCS */
}
void sigl_loadOpenGL_2_1(void) {
#if !defined(SIGL_NO_GL_2_1_H_FUNCS)
	SIGL_PROC_DEF(glUniformMatrix2x3fv);
	SIGL_PROC_DEF(glUniformMatrix3x2fv);
	SIGL_PROC_DEF(glUniformMatrix2x4fv);
	SIGL_PROC_DEF(glUniformMatrix4x2fv);
	SIGL_PROC_DEF(glUniformMatrix3x4fv);
	SIGL_PROC_DEF(glUniformMatrix4x3fv);
#endif /* !SIGL_NO_GL_2_1_H_FUNCS */
}
void sigl_loadOpenGL_3_0(void) {
#if !defined(SIGL_NO_GL_3_0_H_FUNCS)
	SIGL_PROC_DEF(glColorMaski);
	SIGL_PROC_DEF(glGetBooleani_v);
	SIGL_PROC_DEF(glGetIntegeri_v);
	SIGL_PROC_DEF(glEnablei);
	SIGL_PROC_DEF(glDisablei);
	SIGL_PROC_DEF(glIsEnabledi);
	SIGL_PROC_DEF(glBeginTransformFeedback);
	SIGL_PROC_DEF(glEndTransformFeedback);
	SIGL_PROC_DEF(glBindBufferRange);
	SIGL_PROC_DEF(glBindBufferBase);
	SIGL_PROC_DEF(glTransformFeedbackVaryings);
	SIGL_PROC_DEF(glGetTransformFeedbackVarying);
	SIGL_PROC_DEF(glClampColor);
	SIGL_PROC_DEF(glBeginConditionalRender);
	SIGL_PROC_DEF(glEndConditionalRender);
	SIGL_PROC_DEF(glVertexAttribIPointer);
	SIGL_PROC_DEF(glGetVertexAttribIiv);
	SIGL_PROC_DEF(glGetVertexAttribIuiv);
	SIGL_PROC_DEF(glVertexAttribI1i);
	SIGL_PROC_DEF(glVertexAttribI2i);
	SIGL_PROC_DEF(glVertexAttribI3i);
	SIGL_PROC_DEF(glVertexAttribI4i);
	SIGL_PROC_DEF(glVertexAttribI1ui);
	SIGL_PROC_DEF(glVertexAttribI2ui);
	SIGL_PROC_DEF(glVertexAttribI3ui);
	SIGL_PROC_DEF(glVertexAttribI4ui);
	SIGL_PROC_DEF(glVertexAttribI1iv);
	SIGL_PROC_DEF(glVertexAttribI2iv);
	SIGL_PROC_DEF(glVertexAttribI3iv);
	SIGL_PROC_DEF(glVertexAttribI4iv);
	SIGL_PROC_DEF(glVertexAttribI1uiv);
	SIGL_PROC_DEF(glVertexAttribI2uiv);
	SIGL_PROC_DEF(glVertexAttribI3uiv);
	SIGL_PROC_DEF(glVertexAttribI4uiv);
	SIGL_PROC_DEF(glVertexAttribI4bv);
	SIGL_PROC_DEF(glVertexAttribI4sv);
	SIGL_PROC_DEF(glVertexAttribI4ubv);
	SIGL_PROC_DEF(glVertexAttribI4usv);
	SIGL_PROC_DEF(glGetUniformuiv);
	SIGL_PROC_DEF(glBindFragDataLocation);
	SIGL_PROC_DEF(glGetFragDataLocation);
	SIGL_PROC_DEF(glUniform1ui);
	SIGL_PROC_DEF(glUniform2ui);
	SIGL_PROC_DEF(glUniform3ui);
	SIGL_PROC_DEF(glUniform4ui);
	SIGL_PROC_DEF(glUniform1uiv);
	SIGL_PROC_DEF(glUniform2uiv);
	SIGL_PROC_DEF(glUniform3uiv);
	SIGL_PROC_DEF(glUniform4uiv);
	SIGL_PROC_DEF(glTexParameterIiv);
	SIGL_PROC_DEF(glTexParameterIuiv);
	SIGL_PROC_DEF(glGetTexParameterIiv);
	SIGL_PROC_DEF(glGetTexParameterIuiv);
	SIGL_PROC_DEF(glClearBufferiv);
	SIGL_PROC_DEF(glClearBufferuiv);
	SIGL_PROC_DEF(glClearBufferfv);
	SIGL_PROC_DEF(glClearBufferfi);
	SIGL_PROC_DEF(glGetStringi);
	SIGL_PROC_DEF(glIsRenderbuffer);
	SIGL_PROC_DEF(glBindRenderbuffer);
	SIGL_PROC_DEF(glDeleteRenderbuffers);
	SIGL_PROC_DEF(glGenRenderbuffers);
	SIGL_PROC_DEF(glRenderbufferStorage);
	SIGL_PROC_DEF(glGetRenderbufferParameteriv);
	SIGL_PROC_DEF(glIsFramebuffer);
	SIGL_PROC_DEF(glBindFramebuffer);
	SIGL_PROC_DEF(glDeleteFramebuffers);
	SIGL_PROC_DEF(glGenFramebuffers);
	SIGL_PROC_DEF(glCheckFramebufferStatus);
	SIGL_PROC_DEF(glFramebufferTexture1D);
	SIGL_PROC_DEF(glFramebufferTexture2D);
	SIGL_PROC_DEF(glFramebufferTexture3D);
	SIGL_PROC_DEF(glFramebufferRenderbuffer);
	SIGL_PROC_DEF(glGetFramebufferAttachmentParameteriv);
	SIGL_PROC_DEF(glGenerateMipmap);
	SIGL_PROC_DEF(glBlitFramebuffer);
	SIGL_PROC_DEF(glRenderbufferStorageMultisample);
	SIGL_PROC_DEF(glFramebufferTextureLayer);
	SIGL_PROC_DEF(glMapBufferRange);
	SIGL_PROC_DEF(glFlushMappedBufferRange);
	SIGL_PROC_DEF(glBindVertexArray);
	SIGL_PROC_DEF(glDeleteVertexArrays);
	SIGL_PROC_DEF(glGenVertexArrays);
	SIGL_PROC_DEF(glIsVertexArray);
#endif /* !SIGL_NO_GL_3_0_H_FUNCS */
}
void sigl_loadOpenGL_3_1(void) {
#if !defined(SIGL_NO_GL_3_1_H_FUNCS)
	SIGL_PROC_DEF(glDrawArraysInstanced);
	SIGL_PROC_DEF(glDrawElementsInstanced);
	SIGL_PROC_DEF(glTexBuffer);
	SIGL_PROC_DEF(glPrimitiveRestartIndex);
	SIGL_PROC_DEF(glCopyBufferSubData);
	SIGL_PROC_DEF(glGetUniformIndices);
	SIGL_PROC_DEF(glGetActiveUniformsiv);
	SIGL_PROC_DEF(glGetActiveUniformName);
	SIGL_PROC_DEF(glGetUniformBlockIndex);
	SIGL_PROC_DEF(glGetActiveUniformBlockiv);
	SIGL_PROC_DEF(glGetActiveUniformBlockName);
	SIGL_PROC_DEF(glUniformBlockBinding);
#endif /* !SIGL_NO_GL_3_1_H_FUNCS */
}
void sigl_loadOpenGL_3_2(void) {
#if !defined(SIGL_NO_GL_3_2_H_FUNCS)
	SIGL_PROC_DEF(glDrawElementsBaseVertex);
	SIGL_PROC_DEF(glDrawRangeElementsBaseVertex);
	SIGL_PROC_DEF(glDrawElementsInstancedBaseVertex);
	SIGL_PROC_DEF(glMultiDrawElementsBaseVertex);
	SIGL_PROC_DEF(glProvokingVertex);
	SIGL_PROC_DEF(glFenceSync);
	SIGL_PROC_DEF(glIsSync);
	SIGL_PROC_DEF(glDeleteSync);
	SIGL_PROC_DEF(glClientWaitSync);
	SIGL_PROC_DEF(glWaitSync);
	SIGL_PROC_DEF(glGetInteger64v);
	SIGL_PROC_DEF(glGetSynciv);
	SIGL_PROC_DEF(glGetInteger64i_v);
	SIGL_PROC_DEF(glGetBufferParameteri64v);
	SIGL_PROC_DEF(glFramebufferTexture);
	SIGL_PROC_DEF(glTexImage2DMultisample);
	SIGL_PROC_DEF(glTexImage3DMultisample);
	SIGL_PROC_DEF(glGetMultisamplefv);
	SIGL_PROC_DEF(glSampleMaski);
#endif /* !SIGL_NO_GL_3_2_H_FUNCS */
}
void sigl_loadOpenGL_3_3(void) {
#if !defined(SIGL_NO_GL_3_3_H_FUNCS)
	SIGL_PROC_DEF(glBindFragDataLocationIndexed);
	SIGL_PROC_DEF(glGetFragDataIndex);
	SIGL_PROC_DEF(glGenSamplers);
	SIGL_PROC_DEF(glDeleteSamplers);
	SIGL_PROC_DEF(glIsSampler);
	SIGL_PROC_DEF(glBindSampler);
	SIGL_PROC_DEF(glSamplerParameteri);
	SIGL_PROC_DEF(glSamplerParameteriv);
	SIGL_PROC_DEF(glSamplerParameterf);
	SIGL_PROC_DEF(glSamplerParameterfv);
	SIGL_PROC_DEF(glSamplerParameterIiv);
	SIGL_PROC_DEF(glSamplerParameterIuiv);
	SIGL_PROC_DEF(glGetSamplerParameteriv);
	SIGL_PROC_DEF(glGetSamplerParameterIiv);
	SIGL_PROC_DEF(glGetSamplerParameterfv);
	SIGL_PROC_DEF(glGetSamplerParameterIuiv);
	SIGL_PROC_DEF(glQueryCounter);
	SIGL_PROC_DEF(glGetQueryObjecti64v);
	SIGL_PROC_DEF(glGetQueryObjectui64v);
	SIGL_PROC_DEF(glVertexAttribDivisor);
	SIGL_PROC_DEF(glVertexAttribP1ui);
	SIGL_PROC_DEF(glVertexAttribP1uiv);
	SIGL_PROC_DEF(glVertexAttribP2ui);
	SIGL_PROC_DEF(glVertexAttribP2uiv);
	SIGL_PROC_DEF(glVertexAttribP3ui);
	SIGL_PROC_DEF(glVertexAttribP3uiv);
	SIGL_PROC_DEF(glVertexAttribP4ui);
	SIGL_PROC_DEF(glVertexAttribP4uiv);
	SIGL_PROC_DEF(glVertexP2ui);
	SIGL_PROC_DEF(glVertexP2uiv);
	SIGL_PROC_DEF(glVertexP3ui);
	SIGL_PROC_DEF(glVertexP3uiv);
	SIGL_PROC_DEF(glVertexP4ui);
	SIGL_PROC_DEF(glVertexP4uiv);
	SIGL_PROC_DEF(glTexCoordP1ui);
	SIGL_PROC_DEF(glTexCoordP1uiv);
	SIGL_PROC_DEF(glTexCoordP2ui);
	SIGL_PROC_DEF(glTexCoordP2uiv);
	SIGL_PROC_DEF(glTexCoordP3ui);
	SIGL_PROC_DEF(glTexCoordP3uiv);
	SIGL_PROC_DEF(glTexCoordP4ui);
	SIGL_PROC_DEF(glTexCoordP4uiv);
	SIGL_PROC_DEF(glMultiTexCoordP1ui);
	SIGL_PROC_DEF(glMultiTexCoordP1uiv);
	SIGL_PROC_DEF(glMultiTexCoordP2ui);
	SIGL_PROC_DEF(glMultiTexCoordP2uiv);
	SIGL_PROC_DEF(glMultiTexCoordP3ui);
	SIGL_PROC_DEF(glMultiTexCoordP3uiv);
	SIGL_PROC_DEF(glMultiTexCoordP4ui);
	SIGL_PROC_DEF(glMultiTexCoordP4uiv);
	SIGL_PROC_DEF(glNormalP3ui);
	SIGL_PROC_DEF(glNormalP3uiv);
	SIGL_PROC_DEF(glColorP3ui);
	SIGL_PROC_DEF(glColorP3uiv);
	SIGL_PROC_DEF(glColorP4ui);
	SIGL_PROC_DEF(glColorP4uiv);
	SIGL_PROC_DEF(glSecondaryColorP3ui);
	SIGL_PROC_DEF(glSecondaryColorP3uiv);
#endif /* !SIGL_NO_GL_3_3_H_FUNCS */
}
void sigl_loadOpenGL_4_0(void) {
#if !defined(SIGL_NO_GL_4_0_H_FUNCS)
	SIGL_PROC_DEF(glMinSampleShading);
	SIGL_PROC_DEF(glBlendEquationi);
	SIGL_PROC_DEF(glBlendEquationSeparatei);
	SIGL_PROC_DEF(glBlendFunci);
	SIGL_PROC_DEF(glBlendFuncSeparatei);
	SIGL_PROC_DEF(glDrawArraysIndirect);
	SIGL_PROC_DEF(glDrawElementsIndirect);
	SIGL_PROC_DEF(glUniform1d);
	SIGL_PROC_DEF(glUniform2d);
	SIGL_PROC_DEF(glUniform3d);
	SIGL_PROC_DEF(glUniform4d);
	SIGL_PROC_DEF(glUniform1dv);
	SIGL_PROC_DEF(glUniform2dv);
	SIGL_PROC_DEF(glUniform3dv);
	SIGL_PROC_DEF(glUniform4dv);
	SIGL_PROC_DEF(glUniformMatrix2dv);
	SIGL_PROC_DEF(glUniformMatrix3dv);
	SIGL_PROC_DEF(glUniformMatrix4dv);
	SIGL_PROC_DEF(glUniformMatrix2x3dv);
	SIGL_PROC_DEF(glUniformMatrix2x4dv);
	SIGL_PROC_DEF(glUniformMatrix3x2dv);
	SIGL_PROC_DEF(glUniformMatrix3x4dv);
	SIGL_PROC_DEF(glUniformMatrix4x2dv);
	SIGL_PROC_DEF(glUniformMatrix4x3dv);
	SIGL_PROC_DEF(glGetUniformdv);
	SIGL_PROC_DEF(glGetSubroutineUniformLocation);
	SIGL_PROC_DEF(glGetSubroutineIndex);
	SIGL_PROC_DEF(glGetActiveSubroutineUniformiv);
	SIGL_PROC_DEF(glGetActiveSubroutineUniformName);
	SIGL_PROC_DEF(glGetActiveSubroutineName);
	SIGL_PROC_DEF(glUniformSubroutinesuiv);
	SIGL_PROC_DEF(glGetUniformSubroutineuiv);
	SIGL_PROC_DEF(glGetProgramStageiv);
	SIGL_PROC_DEF(glPatchParameteri);
	SIGL_PROC_DEF(glPatchParameterfv);
	SIGL_PROC_DEF(glBindTransformFeedback);
	SIGL_PROC_DEF(glDeleteTransformFeedbacks);
	SIGL_PROC_DEF(glGenTransformFeedbacks);
	SIGL_PROC_DEF(glIsTransformFeedback);
	SIGL_PROC_DEF(glPauseTransformFeedback);
	SIGL_PROC_DEF(glResumeTransformFeedback);
	SIGL_PROC_DEF(glDrawTransformFeedback);
	SIGL_PROC_DEF(glDrawTransformFeedbackStream);
	SIGL_PROC_DEF(glBeginQueryIndexed);
	SIGL_PROC_DEF(glEndQueryIndexed);
	SIGL_PROC_DEF(glGetQueryIndexediv);
#endif /* !SIGL_NO_GL_4_0_H_FUNCS */
}
void sigl_loadOpenGL_4_1(void) {
#if !defined(SIGL_NO_GL_4_1_H_FUNCS)
	SIGL_PROC_DEF(glReleaseShaderCompiler);
	SIGL_PROC_DEF(glShaderBinary);
	SIGL_PROC_DEF(glGetShaderPrecisionFormat);
	SIGL_PROC_DEF(glDepthRangef);
	SIGL_PROC_DEF(glClearDepthf);
	SIGL_PROC_DEF(glGetProgramBinary);
	SIGL_PROC_DEF(glProgramBinary);
	SIGL_PROC_DEF(glProgramParameteri);
	SIGL_PROC_DEF(glUseProgramStages);
	SIGL_PROC_DEF(glActiveShaderProgram);
	SIGL_PROC_DEF(glCreateShaderProgramv);
	SIGL_PROC_DEF(glBindProgramPipeline);
	SIGL_PROC_DEF(glDeleteProgramPipelines);
	SIGL_PROC_DEF(glGenProgramPipelines);
	SIGL_PROC_DEF(glIsProgramPipeline);
	SIGL_PROC_DEF(glGetProgramPipelineiv);
	SIGL_PROC_DEF(glProgramUniform1i);
	SIGL_PROC_DEF(glProgramUniform1iv);
	SIGL_PROC_DEF(glProgramUniform1f);
	SIGL_PROC_DEF(glProgramUniform1fv);
	SIGL_PROC_DEF(glProgramUniform1d);
	SIGL_PROC_DEF(glProgramUniform1dv);
	SIGL_PROC_DEF(glProgramUniform1ui);
	SIGL_PROC_DEF(glProgramUniform1uiv);
	SIGL_PROC_DEF(glProgramUniform2i);
	SIGL_PROC_DEF(glProgramUniform2iv);
	SIGL_PROC_DEF(glProgramUniform2f);
	SIGL_PROC_DEF(glProgramUniform2fv);
	SIGL_PROC_DEF(glProgramUniform2d);
	SIGL_PROC_DEF(glProgramUniform2dv);
	SIGL_PROC_DEF(glProgramUniform2ui);
	SIGL_PROC_DEF(glProgramUniform2uiv);
	SIGL_PROC_DEF(glProgramUniform3i);
	SIGL_PROC_DEF(glProgramUniform3iv);
	SIGL_PROC_DEF(glProgramUniform3f);
	SIGL_PROC_DEF(glProgramUniform3fv);
	SIGL_PROC_DEF(glProgramUniform3d);
	SIGL_PROC_DEF(glProgramUniform3dv);
	SIGL_PROC_DEF(glProgramUniform3ui);
	SIGL_PROC_DEF(glProgramUniform3uiv);
	SIGL_PROC_DEF(glProgramUniform4i);
	SIGL_PROC_DEF(glProgramUniform4iv);
	SIGL_PROC_DEF(glProgramUniform4f);
	SIGL_PROC_DEF(glProgramUniform4fv);
	SIGL_PROC_DEF(glProgramUniform4d);
	SIGL_PROC_DEF(glProgramUniform4dv);
	SIGL_PROC_DEF(glProgramUniform4ui);
	SIGL_PROC_DEF(glProgramUniform4uiv);
	SIGL_PROC_DEF(glProgramUniformMatrix2fv);
	SIGL_PROC_DEF(glProgramUniformMatrix3fv);
	SIGL_PROC_DEF(glProgramUniformMatrix4fv);
	SIGL_PROC_DEF(glProgramUniformMatrix2dv);
	SIGL_PROC_DEF(glProgramUniformMatrix3dv);
	SIGL_PROC_DEF(glProgramUniformMatrix4dv);
	SIGL_PROC_DEF(glProgramUniformMatrix2x3fv);
	SIGL_PROC_DEF(glProgramUniformMatrix3x2fv);
	SIGL_PROC_DEF(glProgramUniformMatrix2x4fv);
	SIGL_PROC_DEF(glProgramUniformMatrix4x2fv);
	SIGL_PROC_DEF(glProgramUniformMatrix3x4fv);
	SIGL_PROC_DEF(glProgramUniformMatrix4x3fv);
	SIGL_PROC_DEF(glProgramUniformMatrix2x3dv);
	SIGL_PROC_DEF(glProgramUniformMatrix3x2dv);
	SIGL_PROC_DEF(glProgramUniformMatrix2x4dv);
	SIGL_PROC_DEF(glProgramUniformMatrix4x2dv);
	SIGL_PROC_DEF(glProgramUniformMatrix3x4dv);
	SIGL_PROC_DEF(glProgramUniformMatrix4x3dv);
	SIGL_PROC_DEF(glValidateProgramPipeline);
	SIGL_PROC_DEF(glGetProgramPipelineInfoLog);
	SIGL_PROC_DEF(glVertexAttribL1d);
	SIGL_PROC_DEF(glVertexAttribL2d);
	SIGL_PROC_DEF(glVertexAttribL3d);
	SIGL_PROC_DEF(glVertexAttribL4d);
	SIGL_PROC_DEF(glVertexAttribL1dv);
	SIGL_PROC_DEF(glVertexAttribL2dv);
	SIGL_PROC_DEF(glVertexAttribL3dv);
	SIGL_PROC_DEF(glVertexAttribL4dv);
	SIGL_PROC_DEF(glVertexAttribLPointer);
	SIGL_PROC_DEF(glGetVertexAttribLdv);
	SIGL_PROC_DEF(glViewportArrayv);
	SIGL_PROC_DEF(glViewportIndexedf);
	SIGL_PROC_DEF(glViewportIndexedfv);
	SIGL_PROC_DEF(glScissorArrayv);
	SIGL_PROC_DEF(glScissorIndexed);
	SIGL_PROC_DEF(glScissorIndexedv);
	SIGL_PROC_DEF(glDepthRangeArrayv);
	SIGL_PROC_DEF(glDepthRangeIndexed);
	SIGL_PROC_DEF(glGetFloati_v);
	SIGL_PROC_DEF(glGetDoublei_v);
#endif /* !SIGL_NO_GL_4_1_H_FUNCS */
}
void sigl_loadOpenGL_4_2(void) {
#if !defined(SIGL_NO_GL_4_2_H_FUNCS)
	SIGL_PROC_DEF(glDrawArraysInstancedBaseInstance);
	SIGL_PROC_DEF(glDrawElementsInstancedBaseInstance);
	SIGL_PROC_DEF(glDrawElementsInstancedBaseVertexBaseInstance);
	SIGL_PROC_DEF(glGetInternalformativ);
	SIGL_PROC_DEF(glGetActiveAtomicCounterBufferiv);
	SIGL_PROC_DEF(glBindImageTexture);
	SIGL_PROC_DEF(glMemoryBarrier);
	SIGL_PROC_DEF(glTexStorage1D);
	SIGL_PROC_DEF(glTexStorage2D);
	SIGL_PROC_DEF(glTexStorage3D);
	SIGL_PROC_DEF(glDrawTransformFeedbackInstanced);
	SIGL_PROC_DEF(glDrawTransformFeedbackStreamInstanced);
#endif /* !SIGL_NO_GL_4_2_H_FUNCS */
}
void sigl_loadOpenGL_4_3(void) {
#if !defined(SIGL_NO_GL_4_3_H_FUNCS)
	SIGL_PROC_DEF(glClearBufferData);
	SIGL_PROC_DEF(glClearBufferSubData);
	SIGL_PROC_DEF(glDispatchCompute);
	SIGL_PROC_DEF(glDispatchComputeIndirect);
	SIGL_PROC_DEF(glCopyImageSubData);
	SIGL_PROC_DEF(glFramebufferParameteri);
	SIGL_PROC_DEF(glGetFramebufferParameteriv);
	SIGL_PROC_DEF(glGetInternalformati64v);
	SIGL_PROC_DEF(glInvalidateTexSubImage);
	SIGL_PROC_DEF(glInvalidateTexImage);
	SIGL_PROC_DEF(glInvalidateBufferSubData);
	SIGL_PROC_DEF(glInvalidateBufferData);
	SIGL_PROC_DEF(glInvalidateFramebuffer);
	SIGL_PROC_DEF(glInvalidateSubFramebuffer);
	SIGL_PROC_DEF(glMultiDrawArraysIndirect);
	SIGL_PROC_DEF(glMultiDrawElementsIndirect);
	SIGL_PROC_DEF(glGetProgramInterfaceiv);
	SIGL_PROC_DEF(glGetProgramResourceIndex);
	SIGL_PROC_DEF(glGetProgramResourceName);
	SIGL_PROC_DEF(glGetProgramResourceiv);
	SIGL_PROC_DEF(glGetProgramResourceLocation);
	SIGL_PROC_DEF(glGetProgramResourceLocationIndex);
	SIGL_PROC_DEF(glShaderStorageBlockBinding);
	SIGL_PROC_DEF(glTexBufferRange);
	SIGL_PROC_DEF(glTexStorage2DMultisample);
	SIGL_PROC_DEF(glTexStorage3DMultisample);
	SIGL_PROC_DEF(glTextureView);
	SIGL_PROC_DEF(glBindVertexBuffer);
	SIGL_PROC_DEF(glVertexAttribFormat);
	SIGL_PROC_DEF(glVertexAttribIFormat);
	SIGL_PROC_DEF(glVertexAttribLFormat);
	SIGL_PROC_DEF(glVertexAttribBinding);
	SIGL_PROC_DEF(glVertexBindingDivisor);
	SIGL_PROC_DEF(glDebugMessageControl);
	SIGL_PROC_DEF(glDebugMessageInsert);
	SIGL_PROC_DEF(glDebugMessageCallback);
	SIGL_PROC_DEF(glGetDebugMessageLog);
	SIGL_PROC_DEF(glPushDebugGroup);
	SIGL_PROC_DEF(glPopDebugGroup);
	SIGL_PROC_DEF(glObjectLabel);
	SIGL_PROC_DEF(glGetObjectLabel);
	SIGL_PROC_DEF(glObjectPtrLabel);
	SIGL_PROC_DEF(glGetObjectPtrLabel);
#endif /* !SIGL_NO_GL_4_3_H_FUNCS */
}
void sigl_loadOpenGL_4_4(void) {
#if !defined(SIGL_NO_GL_4_4_H_FUNCS)
	SIGL_PROC_DEF(glBufferStorage);
	SIGL_PROC_DEF(glClearTexImage);
	SIGL_PROC_DEF(glClearTexSubImage);
	SIGL_PROC_DEF(glBindBuffersBase);
	SIGL_PROC_DEF(glBindBuffersRange);
	SIGL_PROC_DEF(glBindTextures);
	SIGL_PROC_DEF(glBindSamplers);
	SIGL_PROC_DEF(glBindImageTextures);
	SIGL_PROC_DEF(glBindVertexBuffers);
#endif /* !SIGL_NO_GL_4_4_H_FUNCS */
}
void sigl_loadOpenGL_4_5(void) {
#if !defined(SIGL_NO_GL_4_5_H_FUNCS)
	SIGL_PROC_DEF(glClipControl);
	SIGL_PROC_DEF(glCreateTransformFeedbacks);
	SIGL_PROC_DEF(glTransformFeedbackBufferBase);
	SIGL_PROC_DEF(glTransformFeedbackBufferRange);
	SIGL_PROC_DEF(glGetTransformFeedbackiv);
	SIGL_PROC_DEF(glGetTransformFeedbacki_v);
	SIGL_PROC_DEF(glGetTransformFeedbacki64_v);
	SIGL_PROC_DEF(glCreateBuffers);
	SIGL_PROC_DEF(glNamedBufferStorage);
	SIGL_PROC_DEF(glNamedBufferData);
	SIGL_PROC_DEF(glNamedBufferSubData);
	SIGL_PROC_DEF(glCopyNamedBufferSubData);
	SIGL_PROC_DEF(glClearNamedBufferData);
	SIGL_PROC_DEF(glClearNamedBufferSubData);
	SIGL_PROC_DEF(glMapNamedBuffer);
	SIGL_PROC_DEF(glMapNamedBufferRange);
	SIGL_PROC_DEF(glUnmapNamedBuffer);
	SIGL_PROC_DEF(glFlushMappedNamedBufferRange);
	SIGL_PROC_DEF(glGetNamedBufferParameteriv);
	SIGL_PROC_DEF(glGetNamedBufferParameteri64v);
	SIGL_PROC_DEF(glGetNamedBufferPointerv);
	SIGL_PROC_DEF(glGetNamedBufferSubData);
	SIGL_PROC_DEF(glCreateFramebuffers);
	SIGL_PROC_DEF(glNamedFramebufferRenderbuffer);
	SIGL_PROC_DEF(glNamedFramebufferParameteri);
	SIGL_PROC_DEF(glNamedFramebufferTexture);
	SIGL_PROC_DEF(glNamedFramebufferTextureLayer);
	SIGL_PROC_DEF(glNamedFramebufferDrawBuffer);
	SIGL_PROC_DEF(glNamedFramebufferDrawBuffers);
	SIGL_PROC_DEF(glNamedFramebufferReadBuffer);
	SIGL_PROC_DEF(glInvalidateNamedFramebufferData);
	SIGL_PROC_DEF(glInvalidateNamedFramebufferSubData);
	SIGL_PROC_DEF(glClearNamedFramebufferiv);
	SIGL_PROC_DEF(glClearNamedFramebufferuiv);
	SIGL_PROC_DEF(glClearNamedFramebufferfv);
	SIGL_PROC_DEF(glClearNamedFramebufferfi);
	SIGL_PROC_DEF(glBlitNamedFramebuffer);
	SIGL_PROC_DEF(glCheckNamedFramebufferStatus);
	SIGL_PROC_DEF(glGetNamedFramebufferParameteriv);
	SIGL_PROC_DEF(glGetNamedFramebufferAttachmentParameteriv);
	SIGL_PROC_DEF(glCreateRenderbuffers);
	SIGL_PROC_DEF(glNamedRenderbufferStorage);
	SIGL_PROC_DEF(glNamedRenderbufferStorageMultisample);
	SIGL_PROC_DEF(glGetNamedRenderbufferParameteriv);
	SIGL_PROC_DEF(glCreateTextures);
	SIGL_PROC_DEF(glTextureBuffer);
	SIGL_PROC_DEF(glTextureBufferRange);
	SIGL_PROC_DEF(glTextureStorage1D);
	SIGL_PROC_DEF(glTextureStorage2D);
	SIGL_PROC_DEF(glTextureStorage3D);
	SIGL_PROC_DEF(glTextureStorage2DMultisample);
	SIGL_PROC_DEF(glTextureStorage3DMultisample);
	SIGL_PROC_DEF(glTextureSubImage1D);
	SIGL_PROC_DEF(glTextureSubImage2D);
	SIGL_PROC_DEF(glTextureSubImage3D);
	SIGL_PROC_DEF(glCompressedTextureSubImage1D);
	SIGL_PROC_DEF(glCompressedTextureSubImage2D);
	SIGL_PROC_DEF(glCompressedTextureSubImage3D);
	SIGL_PROC_DEF(glCopyTextureSubImage1D);
	SIGL_PROC_DEF(glCopyTextureSubImage2D);
	SIGL_PROC_DEF(glCopyTextureSubImage3D);
	SIGL_PROC_DEF(glTextureParameterf);
	SIGL_PROC_DEF(glTextureParameterfv);
	SIGL_PROC_DEF(glTextureParameteri);
	SIGL_PROC_DEF(glTextureParameterIiv);
	SIGL_PROC_DEF(glTextureParameterIuiv);
	SIGL_PROC_DEF(glTextureParameteriv);
	SIGL_PROC_DEF(glGenerateTextureMipmap);
	SIGL_PROC_DEF(glBindTextureUnit);
	SIGL_PROC_DEF(glGetTextureImage);
	SIGL_PROC_DEF(glGetCompressedTextureImage);
	SIGL_PROC_DEF(glGetTextureLevelParameterfv);
	SIGL_PROC_DEF(glGetTextureLevelParameteriv);
	SIGL_PROC_DEF(glGetTextureParameterfv);
	SIGL_PROC_DEF(glGetTextureParameterIiv);
	SIGL_PROC_DEF(glGetTextureParameterIuiv);
	SIGL_PROC_DEF(glGetTextureParameteriv);
	SIGL_PROC_DEF(glCreateVertexArrays);
	SIGL_PROC_DEF(glDisableVertexArrayAttrib);
	SIGL_PROC_DEF(glEnableVertexArrayAttrib);
	SIGL_PROC_DEF(glVertexArrayElementBuffer);
	SIGL_PROC_DEF(glVertexArrayVertexBuffer);
	SIGL_PROC_DEF(glVertexArrayVertexBuffers);
	SIGL_PROC_DEF(glVertexArrayAttribBinding);
	SIGL_PROC_DEF(glVertexArrayAttribFormat);
	SIGL_PROC_DEF(glVertexArrayAttribIFormat);
	SIGL_PROC_DEF(glVertexArrayAttribLFormat);
	SIGL_PROC_DEF(glVertexArrayBindingDivisor);
	SIGL_PROC_DEF(glGetVertexArrayiv);
	SIGL_PROC_DEF(glGetVertexArrayIndexediv);
	SIGL_PROC_DEF(glGetVertexArrayIndexed64iv);
	SIGL_PROC_DEF(glCreateSamplers);
	SIGL_PROC_DEF(glCreateProgramPipelines);
	SIGL_PROC_DEF(glCreateQueries);
	SIGL_PROC_DEF(glGetQueryBufferObjecti64v);
	SIGL_PROC_DEF(glGetQueryBufferObjectiv);
	SIGL_PROC_DEF(glGetQueryBufferObjectui64v);
	SIGL_PROC_DEF(glGetQueryBufferObjectuiv);
	SIGL_PROC_DEF(glMemoryBarrierByRegion);
	SIGL_PROC_DEF(glGetTextureSubImage);
	SIGL_PROC_DEF(glGetCompressedTextureSubImage);
	SIGL_PROC_DEF(glGetGraphicsResetStatus);
	SIGL_PROC_DEF(glGetnCompressedTexImage);
	SIGL_PROC_DEF(glGetnTexImage);
	SIGL_PROC_DEF(glGetnUniformdv);
	SIGL_PROC_DEF(glGetnUniformfv);
	SIGL_PROC_DEF(glGetnUniformiv);
	SIGL_PROC_DEF(glGetnUniformuiv);
	SIGL_PROC_DEF(glReadnPixels);
	SIGL_PROC_DEF(glGetnMapdv);
	SIGL_PROC_DEF(glGetnMapfv);
	SIGL_PROC_DEF(glGetnMapiv);
	SIGL_PROC_DEF(glGetnPixelMapfv);
	SIGL_PROC_DEF(glGetnPixelMapuiv);
	SIGL_PROC_DEF(glGetnPixelMapusv);
	SIGL_PROC_DEF(glGetnPolygonStipple);
	SIGL_PROC_DEF(glGetnColorTable);
	SIGL_PROC_DEF(glGetnConvolutionFilter);
	SIGL_PROC_DEF(glGetnSeparableFilter);
	SIGL_PROC_DEF(glGetnHistogram);
	SIGL_PROC_DEF(glGetnMinmax);
	SIGL_PROC_DEF(glTextureBarrier);
#endif /* !SIGL_NO_GL_4_5_H_FUNCS */
}
void sigl_loadOpenGL_4_6(void) {
#if !defined(SIGL_NO_GL_4_6_H_FUNCS)
	SIGL_PROC_DEF(glSpecializeShader);
	SIGL_PROC_DEF(glMultiDrawArraysIndirectCount);
	SIGL_PROC_DEF(glMultiDrawElementsIndirectCount);
	SIGL_PROC_DEF(glPolygonOffsetClamp);
#endif /* !SIGL_NO_GL_4_6_H_FUNCS */
}

void sigl_loadOpenGLOS(void) {
#if SIGL_SYSTEM_IS_WINDOWS && !defined(SIGL_NO_WGL_H_FUNCS)

#if defined(WGL_VERSION_1_0) && defined(SIGL_NO_WINDOWS_H)
SIGL_PROC_DEF(ChoosePixelFormat);
SIGL_PROC_DEF(DescribePixelFormat);
SIGL_PROC_DEF(GetEnhMetaFilePixelFormat);
SIGL_PROC_DEF(GetPixelFormat);
SIGL_PROC_DEF(SetPixelFormat);
SIGL_PROC_DEF(SwapBuffers);
SIGL_PROC_DEF(wglCopyContext);
SIGL_PROC_DEF(wglCreateContext);
SIGL_PROC_DEF(wglCreateLayerContext);
SIGL_PROC_DEF(wglDeleteContext);
SIGL_PROC_DEF(wglDescribeLayerPlane);
SIGL_PROC_DEF(wglGetCurrentContext);
SIGL_PROC_DEF(wglGetCurrentDC);
SIGL_PROC_DEF(wglGetLayerPaletteEntries);
SIGL_PROC_DEF(wglGetProcAddress);
SIGL_PROC_DEF(wglMakeCurrent);
SIGL_PROC_DEF(wglRealizeLayerPalette);
SIGL_PROC_DEF(wglSetLayerPaletteEntries);
SIGL_PROC_DEF(wglShareLists);
SIGL_PROC_DEF(wglSwapLayerBuffers);
SIGL_PROC_DEF(wglUseFontBitmaps);
SIGL_PROC_DEF(wglUseFontBitmapsA);
SIGL_PROC_DEF(wglUseFontBitmapsW);
SIGL_PROC_DEF(wglUseFontOutlines);
SIGL_PROC_DEF(wglUseFontOutlinesA);
SIGL_PROC_DEF(wglUseFontOutlinesW);
#endif /* WGL_VERSION_1_0 */

#if defined(WGL_ARB_buffer_region) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglCreateBufferRegionARB);
SIGL_PROC_DEF(wglDeleteBufferRegionARB);
SIGL_PROC_DEF(wglSaveBufferRegionARB);
SIGL_PROC_DEF(wglRestoreBufferRegionARB);
#endif /* WGL_ARB_buffer_region */

#if defined(WGL_ARB_create_context) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglCreateContextAttribsARB);
#endif

#if defined(WGL_ARB_extensions_string) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglGetExtensionsStringARB);
#endif

#if defined(WGL_ARB_make_current_read) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglMakeContextCurrentARB);
SIGL_PROC_DEF(wglGetCurrentReadDCARB);
#endif

#if defined(WGL_ARB_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglCreatePbufferARB);
SIGL_PROC_DEF(wglGetPbufferDCARB);
SIGL_PROC_DEF(wglReleasePbufferDCARB);
SIGL_PROC_DEF(wglDestroyPbufferARB);
SIGL_PROC_DEF(wglQueryPbufferARB);
#endif

#if defined(WGL_ARB_pixel_format) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglGetPixelFormatAttribivARB);
SIGL_PROC_DEF(wglGetPixelFormatAttribfvARB);
SIGL_PROC_DEF(wglChoosePixelFormatARB);
#endif

#if defined(WGL_ARB_render_texture) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglBindTexImageARB);
SIGL_PROC_DEF(wglReleaseTexImageARB);
SIGL_PROC_DEF(wglSetPbufferAttribARB);
#endif

#if defined(WGL_3DL_stereo_control) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglSetStereoEmitterState3DL);
#endif

#if defined(WGL_AMD_gpu_association) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglGetGPUIDsAMD);
SIGL_PROC_DEF(wglGetGPUInfoAMD);
SIGL_PROC_DEF(wglGetContextGPUIDAMD);
SIGL_PROC_DEF(wglCreateAssociatedContextAMD);
SIGL_PROC_DEF(wglCreateAssociatedContextAttribsAMD);
SIGL_PROC_DEF(wglDeleteAssociatedContextAMD);
SIGL_PROC_DEF(wglMakeAssociatedContextCurrentAMD);
SIGL_PROC_DEF(wglGetCurrentAssociatedContextAMD);
SIGL_PROC_DEF(wglBlitContextFramebufferAMD);
#endif

#if defined(WGL_EXT_display_color_table) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglCreateDisplayColorTableEXT);
SIGL_PROC_DEF(wglLoadDisplayColorTableEXT);
SIGL_PROC_DEF(wglBindDisplayColorTableEXT);
SIGL_PROC_DEF(wglDestroyDisplayColorTableEXT);
#endif

#if defined(WGL_EXT_extensions_string) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglGetExtensionsStringEXT);
#endif

#if defined(WGL_EXT_make_current_read) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglMakeContextCurrentEXT);
SIGL_PROC_DEF(wglGetCurrentReadDCEXT);
#endif

#if defined(WGL_EXT_pbuffer) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglCreatePbufferEXT);
SIGL_PROC_DEF(wglGetPbufferDCEXT);
SIGL_PROC_DEF(wglReleasePbufferDCEXT);
SIGL_PROC_DEF(wglDestroyPbufferEXT);
SIGL_PROC_DEF(wglQueryPbufferEXT);
#endif

#if defined(WGL_EXT_pixel_format) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglGetPixelFormatAttribivEXT);
SIGL_PROC_DEF(wglGetPixelFormatAttribfvEXT);
SIGL_PROC_DEF(wglChoosePixelFormatEXT);
#endif

#if defined(WGL_EXT_swap_control) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglSwapIntervalEXT);
SIGL_PROC_DEF(wglGetSwapIntervalEXT);
#endif

#if defined(WGL_I3D_digital_video_control) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglGetDigitalVideoParametersI3D);
SIGL_PROC_DEF(wglSetDigitalVideoParametersI3D);
#endif

#if defined(WGL_I3D_gamma) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglGetGammaTableParametersI3D);
SIGL_PROC_DEF(wglSetGammaTableParametersI3D);
SIGL_PROC_DEF(wglGetGammaTableI3D);
SIGL_PROC_DEF(wglSetGammaTableI3D);
#endif

#if defined(WGL_I3D_genlock) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglEnableGenlockI3D);
SIGL_PROC_DEF(wglDisableGenlockI3D);
SIGL_PROC_DEF(wglIsEnabledGenlockI3D);
SIGL_PROC_DEF(wglGenlockSourceI3D);
SIGL_PROC_DEF(wglGetGenlockSourceI3D);
SIGL_PROC_DEF(wglGenlockSourceEdgeI3D);
SIGL_PROC_DEF(wglGetGenlockSourceEdgeI3D);
SIGL_PROC_DEF(wglGenlockSampleRateI3D);
SIGL_PROC_DEF(wglGetGenlockSampleRateI3D);
SIGL_PROC_DEF(wglGenlockSourceDelayI3D);
SIGL_PROC_DEF(wglGetGenlockSourceDelayI3D);
SIGL_PROC_DEF(wglQueryGenlockMaxSourceDelayI3D);
#endif

#if defined(WGL_I3D_image_buffer) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglCreateImageBufferI3D);
SIGL_PROC_DEF(wglDestroyImageBufferI3D);
SIGL_PROC_DEF(wglAssociateImageBufferEventsI3D);
SIGL_PROC_DEF(wglReleaseImageBufferEventsI3D);
#endif

#if defined(WGL_I3D_swap_frame_lock) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglEnableFrameLockI3D);
SIGL_PROC_DEF(wglDisableFrameLockI3D);
SIGL_PROC_DEF(wglIsEnabledFrameLockI3D);
SIGL_PROC_DEF(wglQueryFrameLockMasterI3D);
#endif

#if defined(WGL_I3D_swap_frame_usage) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglGetFrameUsageI3D);
SIGL_PROC_DEF(wglBeginFrameTrackingI3D);
SIGL_PROC_DEF(wglEndFrameTrackingI3D);
SIGL_PROC_DEF(wglQueryFrameTrackingI3D);
#endif

#if defined(WGL_NV_DX_interop) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglDXSetResourceShareHandleNV);
SIGL_PROC_DEF(wglDXOpenDeviceNV);
SIGL_PROC_DEF(wglDXCloseDeviceNV);
SIGL_PROC_DEF(wglDXRegisterObjectNV);
SIGL_PROC_DEF(wglDXUnregisterObjectNV);
SIGL_PROC_DEF(wglDXObjectAccessNV);
SIGL_PROC_DEF(wglDXLockObjectsNV);
SIGL_PROC_DEF(wglDXUnlockObjectsNV);
#endif

#if defined(WGL_NV_copy_image) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglCopyImageSubDataNV);
#endif

#if defined(WGL_NV_gpu_affinity) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglEnumGpusNV);
SIGL_PROC_DEF(wglEnumGpuDevicesNV);
SIGL_PROC_DEF(wglCreateAffinityDCNV);
SIGL_PROC_DEF(wglEnumGpusFromAffinityDCNV);
SIGL_PROC_DEF(wglDeleteDCNV);
#endif

#if defined(WGL_NV_present_video) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglEnumerateVideoDevicesNV);
SIGL_PROC_DEF(wglBindVideoDeviceNV);
SIGL_PROC_DEF(wglQueryCurrentContextNV);
#endif

#if defined(WGL_NV_swap_group) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglJoinSwapGroupNV);
SIGL_PROC_DEF(wglBindSwapBarrierNV);
SIGL_PROC_DEF(wglQuerySwapGroupNV);
SIGL_PROC_DEF(wglQueryMaxSwapGroupsNV);
SIGL_PROC_DEF(wglQueryFrameCountNV);
SIGL_PROC_DEF(wglResetFrameCountNV);
#endif

#if defined(WGL_NV_vertex_array_range) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglAllocateMemoryNV);
SIGL_PROC_DEF(wglFreeMemoryNV);
#endif

#if defined(WGL_NV_video_capture) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglBindVideoCaptureDeviceNV);
SIGL_PROC_DEF(wglEnumerateVideoCaptureDevicesNV);
SIGL_PROC_DEF(wglLockVideoCaptureDeviceNV);
SIGL_PROC_DEF(wglQueryVideoCaptureDeviceNV);
SIGL_PROC_DEF(wglReleaseVideoCaptureDeviceNV);
#endif

#if defined(WGL_NV_video_output) || defined(SIGL_INCLUDE_WGL_EXT)
SIGL_PROC_DEF(wglGetVideoDeviceNV);
SIGL_PROC_DEF(wglReleaseVideoDeviceNV);
SIGL_PROC_DEF(wglBindVideoImageNV);
SIGL_PROC_DEF(wglReleaseVideoImageNV);
SIGL_PROC_DEF(wglSendPbufferToVideoNV);
SIGL_PROC_DEF(wglGetVideoInfoNV);
SIGL_PROC_DEF(wglGetSyncValuesOML);
SIGL_PROC_DEF(wglGetMscRateOML);
SIGL_PROC_DEF(wglSwapBuffersMscOML);
SIGL_PROC_DEF(wglSwapLayerBuffersMscOML);
SIGL_PROC_DEF(wglWaitForMscOML);
SIGL_PROC_DEF(wglWaitForSbcOML);
#endif

#elif SIGL_SYSTEM_IS_UNIX
SIGL_PROC_DEF(glXSwapIntervalEXT);
#endif

}

#undef SIGL_FUNC_DECLARE
#undef SIGL_PROC_DEF

#endif

#if defined(__cplusplus)
}
#endif

#endif /* SIGL_INCLUDE_SIGL_H */
