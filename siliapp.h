/*
siliapp.h - an extension library to sili.h for custom GUI development
===========================================================================
	- You must define 'SIAPP_IMPLEMENTATION' in exactly _one_ C file that includes
	this header, as well as 'sili.h', _before_ the include like this:

		#define SIAPP_IMPLEMENTATION
		#include "sili.h"
		#include "siliapp.h"

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
	A FULLY CAPITALIZED KEYWORD. This is a general list of the keywords, what they
	mean and examples of them:
		- TYPE - argument is just the type name (siString, usize, rawptr).
		- TYPE* - same as TYPE except it's a pointer to it (siString*, usize*, rawptr*).
		- INT - argument can be any signed integer (50, -250LL, ISIZE_MAX).
		- UINT - argument can be any unsigned integer (50, 250LL, USIZE_MAX).
		- EXPRESSION - argument is just some kind of valid C value (60, "hello", SI_RGB(255, 255, 255)).
		- NAME - argument has to be regular text with _no_ enquotes (test, var, len).
		- ANYTHING - argument can be literally anything.

===========================================================================
CREDITS
	- Ginger Bill's 'gb.h' (https://github.com//gingerBill/gb) - inspired me to
	make the Sili Toolchain, as well as certain features were taken from the
	library and implemented here.

LICENSE:
	- This software is licensed under the zlib license (see the LICENSE at the
	bottom of the file).

WARNING
	- This library is _slightly_ experimental and features may not work as
	expected.
	- This also means that functions may not be documented.

*/

#ifndef SIAPP_INCLUDE_SIAPP_H
#define SIAPP_INCLUDE_SIAPP_H

#include <sili.h>
#include <stb_truetype.h> // TODO(EimaMei): Remove later.
#include <stb_image.h>

#if !defined(SI_INCLUDE_SI_H)
	#error "sili.h must be included to use this library."
#endif

#if defined(__cplusplus)
extern "C" {
#endif

// TODO(EimaMei): See if I can implement SIMD somewhere here
// TODO(EimaMei): Shadow the hedgehog sfx

#if defined(SIAPP_PLATFORM_API_WAYLAND)
	#error "siliapp.h is not implemented for Wayland"
	#define SIAPP_PLATFORM_API_WAYLAND
#elif defined(SI_SYSTEM_UNIX) && !defined(SI_SYSTEM_ANDROID)
//#error "siliapp.h is not implemented for Unix"
    #include <X11/Xlib.h>
    #include <X11/keysym.h>
    #include <X11/cursorfont.h>
    #include <X11/Xcursor/Xcursor.h>
	#define SIAPP_PLATFORM_API_X11
#elif defined(SI_SYSTEM_WINDOWS)
	#define NOMINMAX            1
	#define WIN32_LEAN_AND_MEAN 1
	#define WIN32_MEAN_AND_LEAN 1
	#define VC_EXTRALEAN        1
	#include <windows.h>
	#include <shlobj.h>
	#include <shellapi.h>
	#include <ole2.h>
	#undef NOMINMAX
	#undef WIN32_LEAN_AND_MEAN
	#undef WIN32_MEAN_AND_LEAN
	#undef VC_EXTRALEAN

    #define SIAPP_PLATFORM_API_WIN32
#elif defined(SI_SYSTEM_OS_X)
	#error "siliapp.h is not implemented for MacOS"
	#define SIAPP_PLATFORM_API_COCOA
#endif

#if !defined(SIAPP_IMAGE_LOAD)
	#if defined(STBI_INCLUDE_STB_IMAGE_H)
		/* buffer - siByte* | len - usize | width - i32* | height - i32* | channels - i32*
		 * A macro that returns a raw image data buffer from the buffer, as well
		 * as writes the image's resolution and channel count to the 3 int pointers. */
		#define SIAPP_IMAGE_LOAD(buffer, len, width, height, channels) \
			stbi_load_from_memory(content, len, width, height, channels, 0)
		#define SIAPP_IMAGE_FREE(buffer) free(buffer)
	#else
		#define SIAPP_IMAGE_LOAD(buffer, len, width, height, channels) \
			(siByte*)SI_PANIC_MSG( \
				"Macro 'SIAPP_IMAGE_LOAD' has not been set. Either include " \
				"'stb_image.h', define 'SIAPP_IMAGE_LOAD' with your own image loading " \
				"function or use 'siapp_imageLoadEx."\
			); SI_UNUSED(buffer); SI_UNUSED(len); SI_UNUSED(width); SI_UNUSED(height); SI_UNUSED(channels)
			#define SIAPP_IMAGE_FREE(buffer) SI_UNUSED(buffer)
	#endif
#endif

#ifndef SI_MAX_PATH_LEN
    #define SI_MAX_PATH_LEN 260 // TODO RELPACE
#endif

typedef SI_ENUM(b32, siWindowArg) {
	SI_WINDOW_CENTER               = SI_BIT(0),
	SI_WINDOW_FULLSCREEN           = SI_BIT(1),
	SI_WINDOW_BORDERLESS           = SI_BIT(2),
	SI_WINDOW_RESIZABLE            = SI_BIT(3),

	SI_WINDOW_MINIMIZED            = SI_BIT(4),
	SI_WINDOW_MAXIMIZED            = SI_BIT(5),
	SI_WINDOW_HIDDEN               = SI_BIT(6),

	SI_WINDOW_RENDERING_CPU        = SI_BIT(7),
	SI_WINDOW_RENDERING_OPENGL     = SI_BIT(8),

	SI_WINDOW_SCALING              = SI_BIT(9),
	SI_WINDOW_OPTIMAL_SIZE         = SI_BIT(10),
	SI_WINDOW_KEEP_ASPECT_RATIO    = SI_BIT(11),

	SI_WINDOW_DEFAULT              = SI_WINDOW_CENTER | SI_WINDOW_RESIZABLE | SI_WINDOW_RENDERING_OPENGL,
	SI_WINDOW_DESKTOP              = SI_WINDOW_FULLSCREEN | SI_WINDOW_BORDERLESS,
	SI_WINDOW_RENDERING_BITS       = SI_WINDOW_RENDERING_CPU | SI_WINDOW_RENDERING_OPENGL
};

typedef SI_ENUM(u8, siKeyType) {
	SK_UNKNOWN = 0,

	SK_0 = '0',
		SK_1, SK_2, SK_3, SK_4, SK_5, SK_6, SK_7, SK_8, SK_9,

	SK_A = 'A',
		SK_B, SK_C, SK_D, SK_E, SK_F, SK_G, SK_H, SK_I, SK_J, SK_K, SK_L,
		SK_M, SK_N, SK_O, SK_P, SK_Q, SK_R, SK_S, SK_T, SK_U, SK_V, SK_W,
		SK_X, SK_Y, SK_Z,

	SK_BRACKET_L  = '[', SK_BRACKET_R  = ']',  SK_SEMICOLON = ';', SK_COMMA     = ',',
	SK_PERIOD    = '.', SK_QUOTE     = '\'', SK_SLASH     = '/', SK_BACKSLASH = '\\',
	SK_GRAVE     = '`', SK_EQUALS    = '=',  SK_MINUS     = '-', SK_SPACE     = ' ',

	SK__PAD = 128,

	/* CTRL, Shift, Alt and System (Left and Right) */
	SK_CTRL_L, SK_SHIFT_L, SK_ALT_L, SK_SYSTEM_L, SK_CTRL_R, SK_SHIFT_R, SK_ALT_R, SK_SYSTEM_R,

	/* Lock keys. */
	SK_CAPS_LOCK, SK_SCROLL_LOCK, SK_NUM_LOCK,

	/* Misc. */
	SK_ESC, SK_MENU, SK_PAUSE, SK_ENTER, SK_RETURN = SK_ENTER, SK_BACKSPACE,
	SK_TAB, SK_PAGE_UP, SK_PAGE_DOWN, SK_END, SK_HOME, SK_INSERT, SK_DELETE, SK_CLEAR,
	SK_PLUS, SK_SUBTRACT, SK_MULTIPLY, SK_DIVIDE,

	/* Arrow keys */
	SK_LEFT, SK_UP, SK_RIGHT, SK_DOWN,

	/* Numpad keys. */
	SK_NUMPAD_0, SK_NUMPAD_1, SK_NUMPAD_2, SK_NUMPAD_3, SK_NUMPAD_4, SK_NUMPAD_5,
	SK_NUMPAD_6, SK_NUMPAD_7, SK_NUMPAD_8, SK_NUMPAD_9, SK_NUMPAD_DOT, SK_NUMPAD_ENTER,

	/* Functions keys. */
	SK_F1, SK_F2, SK_F3, SK_F4, SK_F5, SK_F6, SK_F7, SK_F8, SK_F9, SK_F10, SK_F11,
	SK_F12, SK_F13,  SK_F14, SK_F15,

	SK__EVENT, /* NOTE(EimaMei): Used for setting bit 7 and 6 to denote key and
				  mouse button event changes, do not use! */
	SK_COUNT,
};

typedef SI_ENUM(u32, siMouseButtonType) {
	SI_MOUSE_LEFT = 1,
	SI_MOUSE_MIDDLE,
	SI_MOUSE_RIGHT,
	SI_MOUSE_X1,
	SI_MOUSE_X2,

	SI_MOUSE_COUNT = SI_MOUSE_X2
};

typedef SI_ENUM(u32, siMouseWheelType) {
	SI_MOUSE_WHEEL_UP = 1,
	SI_MOUSE_WHEEL_DOWN,
	SI_MOUSE_WHEEL_LEFT,
	SI_MOUSE_WHEEL_RIGHT,

	SI_MOUSE_WHEEL_COUNT = SI_MOUSE_WHEEL_RIGHT
};

typedef SI_ENUM(i64, siCursorType) {
	SI_CURSOR_ARROW = 1,
	SI_CURSOR_HAND,
	SI_CURSOR_DOUBLE_ARROW_HORIZONTAL,
	SI_CURSOR_TEXT_SELECT,

    SI_CURSOR_COUNT,
	SI_CURSOR_DEFAULT = SI_CURSOR_ARROW
};

typedef SI_ENUM(u32, siTextureResizeEnum) {
	SI_RESIZE_NEAREST = 0x2600,
	SI_RESIZE_LINEAR,

	SI_RESIZE_DEFAULT = SI_RESIZE_LINEAR
};

typedef SI_ENUM(i32, siSwizzleEnum) {
	SI_SWIZZLE_R = 0x8E42,
	SI_SWIZZLE_G,
	SI_SWIZZLE_B,
	SI_SWIZZLE_A,
	SI_SWIZZLE_RGBA
};

typedef SI_ENUM(i32, siSwizzleValue) {
	SI_SWIZZLE_VAL_0 = 0,
	SI_SWIZZLE_VAL_1 = 1,
	SI_SWIZZLE_VAL_R = 0x1903,
	SI_SWIZZLE_VAL_G = 0x1904,
	SI_SWIZZLE_VAL_B = 0x1905,
	SI_SWIZZLE_VAL_A = 0x1906
};


typedef struct {
	b32 isClosed            : 1;

	b32 keyPress            : 1;
	b32 keyRelease          : 1;

	b32 mouseMove           : 1;
	b32 mousePress          : 1;
	b32 mouseRelease        : 1;
	b32 mouseScroll         : 1;

	b32 windowResize        : 1;
	b32 windowMove          : 1;
	b32 windowFocusChange   : 1;
} siEventType;
SI_STATIC_ASSERT(sizeof(siEventType) == 4); /* NOTE(EimaMei): If the type becomes
                                               larger than 4 bytes, parts of the
                                               code base should be reviewed in
                                               case of possible bugs. */

typedef SI_ENUM(u32, siEventTypeEnum) {
	SI_EVENT_CLOSE = 1,

	SI_EVENT_KEY_PRESS,
	SI_EVENT_KEY_RELEASE,

	SI_EVENT_MOUSE_MOVE,
	SI_EVENT_MOUSE_PRESS,
	SI_EVENT_MOUSE_RELEASE,
	SI_EVENT_MOUSE_SCROLL,

	SI_EVENT_WINDOW_RESIZE,
	SI_EVENT_WINDOW_MOVE,
	SI_EVENT_WINDOW_FOCUS,

	SI_EVENT_COUNT = SI_EVENT_WINDOW_FOCUS
};

typedef struct {
	b8 clicked  : 1;
	b8 pressed  : 1;
	b8 released : 1;
} siKeyState;


typedef struct {
	siEventType type;

	char charBuffer[32];
	usize charBufferLen;

	siKeyType curKey;
	siKeyState keys[SK_COUNT];

	b32 focus;
	b32 mouseInside;

	siPoint mouse;
	siVec2 mouseScaled;
	siPoint mouseRoot;

	siMouseButtonType curMouse;
	siKeyState mouseButtons[SI_MOUSE_COUNT];

	siMouseWheelType mouseWheel;

	siPoint windowPos;
	siArea windowSize;
	siArea windowSizeScaled;

	f64 curTime;
	f64 timeDelta;

	struct {
		siKeyType keyCache[16];
		usize keyCacheLen;
	} __private;
} siWindowEvent;


typedef union {
	f32 m[16];
    siVec4 column[4];
} siMatrix;

typedef struct {
	i32 renderingType;

	u32 texID;
	u32 texWidth;
	u32 texHeight;

    u32 curWidth;
	u32 totalWidth;

    u32 curCount;
} siTextureAtlas;

typedef struct {
	siTextureAtlas* atlas;
	siArea size;
	f32 x1, y1, x2, y2;
    u32 id;
} siImage;


typedef struct {
	b32 isLoaded;
	siVersion versionMax;
	i32 texSizeMax;

    siVersion versionSelected;
    u32 stencilSize;
    u32 sampleBuffers;
    b32 stereo;
    u32 auxBuffers;
} siOpenGLInfo;

typedef struct {
	u32 count;
	u32 instanceCount;
	u32 firstIndex;
	i32 baseVertex;
	u32 baseInstance;
} siOpenGLDrawCMD;

typedef struct {
    u32 texID;
    u32 matrixID;
} siOpenGLIDs;

typedef struct {
	rawptr context;
	siArea size;

	u32 programID;
	u32 VAO;
	u32 VBOs[5];

	u32 uniformTexture;
	u32 uniformMvp;

	siVec4 curColor;
	siVec2 curTexCoords;
    siVec4 rotation;
    siVec4 bgColor;

	const siColor* gradient;
	usize gradientLen;

	siImage* curTex;
	siImage defaultTex;

	siVec3* vertices;
	siVec4* colors;
	siVec2* texCoords;
	siOpenGLIDs* batchInfo;
	siMatrix* matrices;
	siOpenGLDrawCMD* CMDs;

	u32 vertexCounter;
	u32 drawCounter;

    u32 maxVertexCount;
} siWinRenderingCtxOpenGL;


typedef struct {
	siAllocator* alloc;
	siTextureAtlas* atlas;

#if defined(SIAPP_PLATFORM_API_WIN32)
	HWND hwnd;
	HDC hdc;
#elif defined(SIAPP_PLATFORM_API_X11)
    Display* display;
    Window hwnd;
#endif

	union {
		siWinRenderingCtxOpenGL opengl;
	} render;

	union {
		siVec4 vec4;
		siColor rgba;
	} textColor;
	siVec4 imageColor;

	siWindowEvent e;
	siWindowArg arg;

	siVec2 scaleFactor;
	siArea originalSize;

	siCursorType cursor;
	b32 cursorSet;
#if defined(SIAPP_PLATFORM_API_X11)
    Cursor __x11BlankCursor;
    rawptr __x11DndHead;
    rawptr __x11DndPrev;
#endif
} siWindow;


typedef struct {
	siSiliStr* items;
	usize len;
} siSearchResult;

typedef struct {
	cstring name;
	cstring filetype;
} siSearchFilterSpec;

typedef SI_ENUM(i32, siSearchOptions) {
	SI_SEARCH_FOLDERS_ONLY     = SI_BIT(0),
	SI_SEARCH_ALLOW_MULTIPLE   = SI_BIT(1)
};

typedef struct {
	cstring title;
	siSearchOptions options;
	cstring defaultPath;
	siSearchFilterSpec* filetypes;
	usize filetypesLen;
} siSearchConfig;

#define SIAPP_SEARCH_DEFAULT ((siSearchConfig){ \
	"Select a File", 0, nil, \
	(siSearchFilterSpec[1]){{"All Files", "*.*"}}, 1 \
})


typedef struct {
	siImage base;
	u32 frames;
	u32 widthRatio;
	siArea spriteSize;
	f32 spriteX2;
	f32 spriteY2;
} siSpriteSheet;


typedef struct {
	u32 start;
	u32 end;
} siGlyphSet;

typedef struct {
	siGlyphSet set;
	u32 index;
} siGlyphSetANDNIndex;

typedef struct {
	i32 codepoint;

	i32 texID;
	i32 glyphIndex;

	f32 x, y;
	f32 width, height;

	f32 advanceX;
} siGlyphInfo;

typedef struct {
	stbtt_fontinfo stbtt;
	siArray(siGlyphInfo) glyphs;
	siArray(siGlyphSetANDNIndex) sets;
	siSpriteSheet sheet;

	i32 size;
	f32	scale;

	i32 unknownSymbolIndex;
	struct {
		f32 space;
		f32 tab;
		f32 newline;
	} advance;
} siFont;

typedef struct {
	usize len;
	usize capacity;

	siFont* font;
	i32* chars;

	siVec2 totalArea;
	f32 curX, prevLargest;
} siText;

#define SI_GLYPHSET_ASCII                {0x0020, 0x007E}
#define SI_GLYPHSET_ISO_8859_1           {0x00A0, 0x00FF}

#define SI_GLYPHSET_LATIN_BASIC          {0x0020, 0x007E}
#define SI_GLYPHSET_LATIN_1_SUPPLEMENT   {0x00A0, 0x00FF}
#define SI_GLYPHSET_LATIN_EXTENDED_A     {0x0100, 0x017F}

#define SI_GLYPHSET_HIRAGANA {0x3040, 0x309F}
#define SI_GLYPHSET_KATAKANA {0x30A0, 0x30FF}
// TODO: Enforce a chronological order.

#define SI_GLYPHSET_END {'?', '?'}


#define SI_CHARSET_WGL4 \
		SI_GLYPHSET_ASCII, SI_GLYPHSET_ISO_8859_1, SI_GLYPHSET_LATIN_1_SUPPLEMENT, \
		SI_GLYPHSET_LATIN_EXTENDED_A, {0x0192, 0x0192}, {0x01FA, 0x01FF}, {0x02C6, 0x02C7}, \
		{0x02C9, 0x02C9}, {0x02D7, 0x02DD}, {0x0384, 0x038B}, {0x038C, 0x38C}, \
		{0x038E, 0x03A1}, {0x03A3, 0x03CE}, {0x400, 0x45F}, {0x0490, 0x0491}, \
		{0x1E80, 0x1E85}, {0x2013, 0x2015}, {0x2016, 0x201E}, {0x2020, 0x2022}, \
		{0x0001F5FF, 0x0001F5FF}
#define SI_CHARSET_DEFAULT SI_CHARSET_WGL4


typedef SI_ENUM(i32, siMessageBoxButton) {
	SI_MESSAGE_BOX_OK = 1,
	SI_MESSAGE_BOX_OK_CANCEL,
	SI_MESSAGE_BOX_YES_NO,
	SI_MESSAGE_BOX_YES_NO_CANCEL,
	SI_MESSAGE_BOX_RETRY_CANCEL,
	SI_MEESAGE_BOX_CANCEL_TRY_CONTINUE,
	SI_MESSAGE_BOX_HELP
};

typedef SI_ENUM(i32, siMessageBoxIcon) {
	SI_MESSAGE_BOX_ICON_ERROR = SI_MESSAGE_BOX_HELP + 1,
	SI_MESSAGE_BOX_ICON_WARNING,
	SI_MESSAGE_BOX_ICON_INFO,
	SI_MESSAGE_BOX_ICON_QUESTION
};

typedef SI_ENUM(i32, siMessageBoxResult) {
	SI_MESSAGE_BOX_RESULT_OK = 1,
	SI_MESSAGE_BOX_RESULT_CANCEL,
	SI_MESSAGE_BOX_RESULT_ABORT,
	SI_MESSAGE_BOX_RESULT_RETRY,
	SI_MESSAGE_BOX_RESULT_IGNORE,
	SI_MESSAGE_BOX_RESULT_YES,
	SI_MESSAGE_BOX_RESULT_NO,
	SI_MESSAGE_BOX_RESULT_TRY_AGAIN,
	SI_MESSAGE_BOX_RESULT_CONTINUE
};


typedef SI_ENUM(i32, siDropState) {
	SI_DRAG_ENTER = 1,
	SI_DRAG_OVER,
	SI_DRAG_LEAVE,
	SI_DRAG_DROP
};

typedef struct {
#if defined(SIAPP_PLATFORM_API_WIN32)
	IDropTarget target;
	IDataObject* pDataObj;
	siWindow* win;
	HWND subHwnd;
#elif defined(SIAPP_PLATFORM_API_X11)
    siRect rect;
    char* data;
    struct siDropEvent* next;
#endif
	siDropState state;
} siDropEvent;

typedef struct {
	/* Actual length of the path. */
	usize len;
	/* */
	char path[SI_MAX_PATH_LEN];
} siDropEntry;

typedef struct {
	/* */
	u32 len;
	/* */

#if defined(SIAPP_PLATFORM_API_WIN32)
	STGMEDIUM stgm;
    u32 curIndex;
#elif defined(SIAPP_PLATFORM_API_X11)
    char* __x11Data;
    u32 curIndex;
#endif
} siDropHandle;


/* Three points structure. */
typedef struct { siPoint p1, p2, p3; } siTriangle;
/* Three float points structure. */
typedef struct { siVec2 p1, p2, p3; } siTriangleF;

/* a - siPoint | b - siPoint | c - siPoint
 * Macro to define a triangle from three points. */
#define SI_TRIANGLE(a, b, c) (siTriangle){(a), (b), (c)}
/* a - siVec2 | b - siVec2 | c - siVec2
 * Macro to define a triangle from three float points. */
#define SI_TRIANGLE_F(a, b, c) (siTriangleF){(a), (b), (c)}

/* triangle - siTriangle
 * Creates a integer-based triangle into a float one. */
#define SI_TRIANGLE_INT_TO_FLOAT(triangle) \
	SI_TRIANGLE_F( \
		SI_VEC2(triangle.p1.x, triangle.p1.y), \
		SI_VEC2(triangle.p2.x, triangle.p2.y), \
		SI_VEC2(triangle.p3.x, triangle.p3.y) \
	)


/* Creates a windows based on the specified name, size and arguments, returns a
 * 'siWindow' object. */
siWindow* siapp_windowMake(siAllocator* alloc, cstring name, siArea size,
		siWindowArg arg, u32 maxDrawCount, u32 maxTexCount, siArea maxTexRes);
/* Creates a windows based on the specified name, position, size and arguments,
 * returns a 'siWindow' object. */
siWindow* siapp_windowMakeEx(siAllocator* alloc, cstring name, siPoint pos,
		siArea size, siWindowArg arg, u32 maxDrawCount, u32 maxTexCount, siArea maxTexRes);

/* Checks the changes for the specified window and updates the contents of 'out'. */
const siWindowEvent* siapp_windowUpdate(siWindow* win, b32 await);

/* Writes new graphics into the current draw buffer. */
void siapp_windowRender(siWindow* win);
/* Clears the graphics of the screen. */
void siapp_windowClear(const siWindow* win);
/* Renders the graphics onto the screen and clears the current buffer. */
void siapp_windowSwapBuffers(const siWindow* win);

/* Closes the window. */
void siapp_windowClose(siWindow* win);

/* Returns a boolean indicating if the user hasn't closed the window yet. */
b32 siapp_windowIsRunning(const siWindow* win);

/* Shows/hides the window depending on the boolean value. */
void siapp_windowShow(const siWindow* win, b32 show);

/* Sets the background color of the window. */
void siapp_windowBackgroundSet(siWindow* win, siColor color);

/* Returns the current state of the specified key. */
siKeyState siapp_windowKeyGet(const siWindow* win, siKeyType key);
/* Returns a boolean indicating if the key was clicked this frame. */
b32 siapp_windowKeyClicked(const siWindow* win, siKeyType key);
/* Returns a boolean indicating if the key is being pressed right now. */
b32 siapp_windowKeyPressed(const siWindow* win, siKeyType key);
/* Returns a boolean indicating if the key was released this frame. */
b32 siapp_windowKeyReleased(const siWindow* win, siKeyType key);

/* Returns the current state of the specified mouse button. */
siKeyState siapp_windowMouseKeyGet(const siWindow* win, siMouseButtonType key);
/* Returns a boolean indicating if the key was clicked this frame. */
b32 siapp_windowMouseKeyClicked(const siWindow* win, siMouseButtonType key);
/* Returns a boolean indicating if the key is being pressed right now. */
b32 siapp_windowMouseKeyPressed(const siWindow* win, siMouseButtonType key);
/* Returns a boolean indicating if the key was released this frame. */
b32 siapp_windowMouseKeyReleased(const siWindow* win, siMouseButtonType key);

/* Returns a 'siWindowEvent' object from specified window. */
const siWindowEvent* siapp_windowEventGet(const siWindow* win);
/* */
b32 siapp_windowEventPoll(const siWindow* win, siEventTypeEnum* out);

/* Returns the current cursor type for the window. */
siCursorType siapp_windowCursorGet(const siWindow* win);
/* Sets a new cursor for the window. */
void siapp_windowCursorSet(siWindow* win, siCursorType cursor);
/* Creates a custom cursor based on the specified image buffer.
 * NOTE: Must be freed with 'siapp_cursorFree' after use. */
siCursorType siapp_cursorMake(siByte* data, siArea res, u32 channels);
/* Frees a custom-made cursor from memory. */
void siapp_cursorFree(siCursorType cursor);

/* Sets the color for when drawing text. */
void siapp_windowTextColorSet(siWindow* win, siColor color);
/* Sets the color for when drawing images. */
void siapp_windowImageColorSet(siWindow* win, siColor color);
/* */
void siapp_windowGradientSet(siWindow* win, const siColor gradient[], usize len);


/* */
void siapp_windowDragAreaMake(siWindow* win, siRect rect, siDropEvent* out);
/* */
void siapp_windowDragAreaEnd(siDropEvent event);


/* Returns the current resolution of the screen. */
siArea siapp_screenGetCurrentSize(void);
/* Returns the area of the resolution index. 0 is the lowest available. If the
 * specified index resolution doesn't exist, the function will return 'SI_AREA(-1, -1)'.
 * Using that you can find the end of the list by checking if the return's width is
 * negative one.*/
siArea siapp_screenGetAvailableResolution(usize index);


/* */
void siapp_mouseShow(b32 show);


/* */
usize siapp_clipboardTextGet(char* outBuffer, usize capacity);
/* */
b32 siapp_clipboardTextSet(cstring text);
/* */
usize siapp_clipboardTextLen(void);


siDropHandle siapp_dropEventHandle(siDropEvent event);

b32 siapp_dropEventPollEntry(siDropHandle* handle, siDropEntry* entry);

void siapp_dropEventEnd(siDropEvent* event);


/* Converts an OS keycode to 'siKeyType'.  */
siKeyType siapp_osKeyToSili(i32 key);
/* Converts an OS enum error intto a human-readable string (max length size is 128 bytes). */
cstring siapp_osErrToStr(i32 error);


/* Returns the username of the PC. */
siSiliStr siapp_usernameGet(void);


/* A open file dialog gets launched for the user to pick the allowed files depending on the
config. The paths gets written into the allocator. */
siSearchResult siapp_fileManagerOpen(siAllocator* alloc, siSearchConfig config); // TODO(EimaMei): Rework this function to use the entry system.


/* Creates a static string, with tis contents being "%APPDATA%\\<folderName>" */
siSiliStr siapp_appDataPathMake(cstring folderName);
/* Creates a static string, with tis contents being "%APPDATA%\\<folderName>" */
siSiliStr siapp_appDataPathMakeEx(cstring folderName, usize folderNameLen);


/* Creates a texture atlas from the given window. */
siTextureAtlas* siapp_textureAtlasMake(const siWindow* win, siArea area, u32 maxTexCount,
		siTextureResizeEnum enumName);
/* Frees the OpenGL context of the specified texture atlas. */
void siapp_textureAtlasFree(siTextureAtlas* atlas);
/* Applied swizzling mask to the 'param' channel from 'mask'. */
void siapp_textureSwizzleMask(const siTextureAtlas* atlas, siSwizzleEnum param,
		const siSwizzleValue mask[4]);
/* Changes the resize method of the specified atlas. Only applies to OpenGL. */
void siapp_textureResizeMethodChange(siTextureAtlas* atlas, siTextureResizeEnum
		resizeMethod);


/* Loads the specified file into the texture atlas.
 * IMPORTANT NOTE: Will not work if the 'SIAPP_IMAGE_LOAD' macro isn't defined
 * or "stb_image.h" isn't included! */
siImage siapp_imageLoad(siTextureAtlas* atlas, cstring filename);
/* Loads the specific buffer into the texture atlas from the specified information. */
siImage siapp_imageLoadEx(siTextureAtlas* atlas, const siByte* buffer, u32 width,
		u32 height, u32 channels);

/* Loads the specified file into the texture atlas as a sprite sheet.
 * IMPORTANT NOTE: Will not work if the 'SIAPP_IMAGE_LOAD' macro isn't defined
 * or "stb_image.h" isn't included! */
siSpriteSheet siapp_spriteSheetLoad(siTextureAtlas* atlas, cstring filename,
		siArea spriteSize);
/* Loads the specific buffer into the texture atlas from the specified information
 * as a sprite sheet. */
siSpriteSheet siapp_spriteSheetLoadEx(siTextureAtlas* atlas, const siByte* data,
		i32 width, i32 height, i32 channels, siArea spriteSize);
/* Returns a 'siImage' structure of the specified sprite. */
siImage siapp_spriteSheetSpriteGet(siSpriteSheet sheet, usize index);
/* Sets a new sprite image to the specified index. */
void siapp_spriteSheetSpriteSetEx(siSpriteSheet sheet, usize index, const siByte* data,
		i32 channels);


/* */
siFont siapp_fontLoad(siWindow* win, cstring path, i32 size);
/* */
siFont siapp_fontLoadExtra(siWindow* win, cstring path, i32 size, usize extraChars);
/* */
siFont siapp_fontLoadEx(siWindow* win, cstring path, i32 size, siGlyphSet* glyphs,
        u32 extraChars);
/* */
void siapp_fontFree(siFont font);

/* */
f32 siapp_fontCalculateScaleFactor(siFont font, u32 textSize);
/* */
siGlyphInfo* siapp_fontGlyphFind(siFont* font, i32 codepoint);

/* */
siText siapp_textLoad(siAllocator* alloc, siFont* font, cstring text);
/* */
siText siapp_textReserve(siAllocator* alloc, siFont* font, usize characterCount);
/* */
siText siapp_textLoadLen(siAllocator* alloc, siFont* font, cstring text, usize characterCount);

/* */
siGlyphInfo* siapp_textCharacterGet(siText text, usize index);
/* */
void siapp_textCharacterSet(siText* text, usize index, i32 codepoint);
/* */
i32 siapp_textCodepointGet(siText text, usize index);
/* */
f32 siapp_textAdvanceXGet(siText text, usize index);

/* */
siArea siapp_textGetArea(siText text, u32 size);
/* */
siVec2 siapp_textGetAreaF(siText text, u32 size);


/* Draws a vertex with the two specified NDC floats. */
void siapp_drawVertex2f(siWindow* win, f32 x, f32 y);
/* Draws a vertex with the three specified NDC floats. */
void siapp_drawVertex3f(siWindow* win, f32 x, f32 y, f32 z);

/* Draws a rectangle based on the specified 'siRect' and color. */
void siapp_drawRect(siWindow* win, siRect rect, siColor color);
/* Draws a rectangle based on the specified 'siVec4' and color. */
void siapp_drawRectF(siWindow* win, siVec4 rect, siColor color);
/* Draws an image based on the specified 'siRect'. */
void siapp_drawImage(siWindow* win, siRect rect, siImage img);
/* Draws an image based on the specified 'siVec4'. */
void siapp_drawImageF(siWindow* win, siVec4 rect, siImage img);

/* Draws a triangle based on the specified points and color.
 * NOTE: The points are drawn in a clockwise manner.*/
void siapp_drawTriangle(siWindow* win, siTriangle triangle, siColor color);
/* Draws a triangle based on the specified points and color.
 * NOTE: The points are drawn in a clockwise manner.*/
void siapp_drawTriangleF(siWindow* win, siTriangleF triangle, siColor color);
/* Draws a right triangle based on its hypotenuse length and the top's starting angle. */
void siapp_drawTriangleRight(siWindow* win, siPoint start, f32 hypotenuse,
		f32 startingAngle, siColor color);
/* Draws an isosceles triangle based on its sides' length and the top's starting angle. */
void siapp_drawTriangleIsosceles(siWindow* win, siPoint start, f32 length,
		f32 startingAngle, siColor color);

/* Draws a circle with 90 sides based on the specified position, radius and color, */
void siapp_drawCircle(siWindow* win, siPoint pos, i32 radius, siColor color);
/* */
void siapp_drawCircleEx(siWindow* win, siPoint pos, i32 radius, u32 sides, siColor color);
/* Draws a circle based on the specified position, radius, color and sides. */
void siapp_drawPolygon(siWindow* win, siRect rect, u32 sides, siColor color);
/* */
void siapp_drawCircleF(siWindow* win, siVec2 pos, i32 radius, siColor color);
/* */
void siapp_drawCircleExF(siWindow* win, siVec2 pos, i32 radius, u32 sides, siColor color);
/* Draws a circle based on the specified position, radius, color and sides. */
void siapp_drawPolygonF(siWindow* win, siVec4 rect, u32 sides, siColor color);

/* */
void siapp_drawText(siWindow* win, siText text, siPoint pos, u32 size);
/* */
void siapp_drawTextItalic(siWindow* win, siText text, siPoint pos, u32 size);
/* */
siVec2 siapp_drawTextFmt(siWindow* win, siText text, siPoint pos, u32 size);
/* */
void siapp_drawTextF(siWindow* win, siText text, siVec2 pos, u32 size);
/* */
void siapp_drawTextItalicF(siWindow* win, siText text, siVec2 pos, u32 size);
/* */
siVec2 siapp_drawTextFmtF(siWindow* win, siText text, siVec2 pos, u32 size);

/* */
f32 siapp_drawCharacter(siWindow* win, const siFont* font, const siGlyphInfo* glyph,
		siVec2 pos, u32 size);
/* */
f32 siapp_drawCharacterScale(siWindow* win, const siFont* font, const siGlyphInfo* glyph,
		siVec2 pos, u32 size, f32 scaleFactor);
/* */
f32 siapp_drawCharacterItalicScale(siWindow* win, const siFont* font, const siGlyphInfo* glyph,
		siVec2 pos, u32 size, f32 scaleFactor);


/* Sets the current vertex color for the window based on the specified RGB
 * values from 0.0f to 1.0f. */
void siapp_color3f(siWindow* win, f32 r, f32 g, f32 b);
/* Sets the current vertex color for the window based on the specified RGBA
 * values from 0.0f to 1.0f. */
void siapp_color4f(siWindow* win, f32 r, f32 g, f32 b, f32 a);
/* Sets the current vertex color for the window based on the specified RGBA
 * values from 0.0f to 1.0f. */
void siapp_colorVec4f(siWindow* win, siVec4 color);

/* Sets the current texture coordinates for the window based on the 2 NDC floats. */
void siapp_texCoords2f(siWindow* win, f32 x, f32 y);


/* Gets OpenGL information used for siliapp. */
siOpenGLInfo siapp_OpenGLGetInfo(void);

/* Initializes an OpenGL context of the specified window. Returns 'false' if the
 * context was failed to be created. */
b32 siapp_windowOpenGLInit(siWindow* win, u32 maxDrawCount, u32 maxTexCount, siArea maxTexRes);
/* Renders the OpenGL graphics onto the screen. */
void siapp_windowOpenGLRender(siWindow* win);
/* Destroys the OpenGL context of the window. */
void siapp_windowOpenGLDestroy(siWindow* win);

/* Sets the max OpenGL version used for the app (by default it picks the most up
 * to date version on the system). */
void siapp_OpenGLVersionSet(siVersion ver);
/* Sets the buffer bit size (8 by default). */
void siapp_OpenGLStencilSet(u32 stencil);
/* Sets the number of sampling buffers (4 by default). */
void siapp_OpenGLSamplesSet(u32 samples);
/* Sets a boolean for stereo (false by default). */
void siapp_OpenGLStereoSet(b32 stereo);
/* Sets the number of aux buffers (0 by default). */
void siapp_OpenGLAuxBuffersSet(u32 auxBuffers);


/* Opens an OS message box with the specified tittle, message, buttons and icons.
 * Returns the exact button that was selected. */
siMessageBoxResult siapp_messageBox(cstring title, cstring message,
		siMessageBoxButton buttons, siMessageBoxIcon icon);
/* Opens an OS message box with the specified tittle, message, buttons and icons.
 * If 'win' is not NULL, the message box window becomes a child window of it.
 * Returns the exact button that was selected. */
siMessageBoxResult siapp_messageBoxEx(const siWindow* win, cstring title, usize titleLen,
		cstring message, usize messageLen, siMessageBoxButton buttons,
		siMessageBoxIcon icon);


#if defined(SIAPP_IMPLEMENTATION)

#if 1 /* SETUP */

#define SIGL_IMPLEMENTATION
#define WGL_ARB_create_context
#define WGL_ARB_create_context_profile
#define WGL_EXT_swap_control
#include <siligl.h>


#if 1 /* Common. */

intern u32 SI_WINDOWS_NUM = 0;

#define SK_TO_INT(sk) (*((u8*)&sk))
#define INT_TO_SK(num) (*((siKeyState*)&(u8){num}))

#if defined (SIAPP_PLATFORM_API_WIN32)
    #define SIAPP_ERROR_CHECK(condition, function) \
        if (SI_UNLIKELY(condition)) { \
            siAllocator* tmp = si_allocatorMakeStack(256); \
            i32 err = GetLastError(); \
            siapp_messageBox( \
                    function " failed", \
                    si_cstrMakeFmt(tmp, "Error: %#X (%s)", err, siapp_osErrToStr(err)), \
                    SI_MESSAGE_BOX_OK, \
                    SI_MESSAGE_BOX_ICON_ERROR \
                    ); \
            return false; \
        }
#else
    #define SIAPP_ERROR_CHECK(condition, function) \
        if (SI_UNLIKELY(condition)) { \
            siapp_messageBox( \
                function " failed", \
                "", \
                SI_MESSAGE_BOX_OK, \
                SI_MESSAGE_BOX_ICON_ERROR \
            ); \
            return false; \
        }
#endif

#define SIAPP_ERROR_MSGBOX_GL(programID, title) \
    do { \
        GLint len; \
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len); \
        \
        char* msg = (char*)malloc(len); \
        glGetProgramInfoLog(programID, len, nil, msg); \
        siapp_messageBox(title, msg, SI_MESSAGE_BOX_OK, SI_MESSAGE_BOX_ICON_ERROR); \
        \
        free(msg); \
    } while (0)

#define siapp__mouseButtonPress(e, type) \
    do { \
        siKeyState* state = &e->mouseButtons[type - 1]; \
        state->clicked = true; \
        state->pressed = true; \
        state->released = false; \
        \
        SK_TO_INT(e->keys[SK__EVENT]) |= SI_BIT(6); \
        e->curMouse = type; \
    } while(0)
#define siapp__mouseButtonRelease(e, type) \
    do { \
        siKeyState* state = &e->mouseButtons[type - 1]; \
        state->clicked = false; \
        state->pressed = false; \
        state->released = true; \
        \
        SK_TO_INT(e->keys[SK__EVENT]) |= SI_BIT(6); \
        e->curMouse = type; \
    } while (0)


F_TRAITS(inline)
b32 siapp__collideRectPoint(siRect r, siPoint p) {
    return (p.x >= r.x &&  p.x <= r.x + r.width && p.y >= r.y && p.y <= r.y + r.height);
}


F_TRAITS(intern)
void siapp__resizeWindow(siWindow* win, i32 width, i32 height) {
   win->e.type.windowResize = true;
   win->e.windowSize = SI_AREA(width, height);

   switch (win->arg & SI_WINDOW_RENDERING_BITS) {
       case SI_WINDOW_RENDERING_OPENGL: {
           siWinRenderingCtxOpenGL* gl = &win->render.opengl;

           if (win->arg & SI_WINDOW_SCALING) {
               glViewport(0, 0, width, height);
           }
           if (win->arg & SI_WINDOW_KEEP_ASPECT_RATIO) {
               i32 newY = 0;

               if (height <= win->originalSize.height) {
                   newY = height - win->originalSize.height;
                   height = win->originalSize.height;
               }

               f32 aspect = (f32)height / gl->size.height;
               i32 newH = gl->size.height * aspect;
               i32 newW = gl->size.width * aspect;

               glViewport(0, newY, newW, newH);
           }
           else {
               glViewport(0, height - gl->size.height, gl->size.width, gl->size.height);
           }

           GLint view[4];
           glGetIntegerv(GL_VIEWPORT, view);
           f32 viewW = view[2];
           f32 viewH = view[3];

           win->scaleFactor = SI_VEC2(viewW / gl->size.width, viewH / gl->size.height);
           win->e.windowSizeScaled = SI_AREA(
                   si_round(width / win->scaleFactor.x),
                   si_round(height / win->scaleFactor.y)
                   );
           break;
       }
   }
}
#endif

#if defined(SIAPP_PLATFORM_API_WIN32)

intern siByte __win32KBState[256];

#define SI_LOWORD_XY(lw) ((i32)(i16)LOWORD(lw))
#define SI_HIWORD_XY(lw) ((i32)(i16)HIWORD(lw))


F_TRAITS(inline intern)
u32 siapp__dropUpdatePress(IDropTarget* target, DWORD grfKeyState, POINTL pt,
		siDropState state) {
	siDropEvent* drop = (siDropEvent*)target;
	drop->state = state;

	POINT mouse = (POINT){pt.x, pt.y};
	ScreenToClient(drop->win->hwnd, (POINT*)&mouse);

	siWindowEvent* e = &drop->win->e;
	e->mouseRoot = SI_POINT(pt.x, pt.y);
	e->mouseScaled = SI_VEC2(
		(f32)mouse.x / drop->win->scaleFactor.x,
		(f32)mouse.x / drop->win->scaleFactor.y
	);
	e->mouse = SI_POINT(e->mouseScaled.x, e->mouseScaled.y);

	e->type.mouseMove = true;
	e->type.mousePress = true;

	switch (grfKeyState & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) {
		case MK_LBUTTON: {
			siKeyState* key = &e->mouseButtons[SI_MOUSE_LEFT - 1];
			key->clicked = false;
			key->pressed = true;
			key->released = false;
			break;
		}
		case MK_MBUTTON: {
			siKeyState* key = &e->mouseButtons[SI_MOUSE_RIGHT - 1];
			key->clicked = false;
			key->pressed = true;
			key->released = false;
			break;
		}
		case MK_RBUTTON: {
			siKeyState* key = &e->mouseButtons[SI_MOUSE_RIGHT - 1];
			key->clicked = false;
			key->pressed = true;
			key->released = false;
			break;
		}
	}


	switch (grfKeyState & (MK_CONTROL | MK_SHIFT)) {
		case MK_CONTROL:
		case MK_CONTROL | MK_SHIFT: {
			return DROPEFFECT_COPY;
		}
		case MK_SHIFT: {
			return DROPEFFECT_MOVE;
		}
	}

	return DROPEFFECT_COPY;
}
F_TRAITS(inline intern)
void siapp__dropUpdateRelease(IDropTarget* target, DWORD grfKeyState, POINTL pt,
		siDropState state) {
	siDropEvent* drop = (siDropEvent*)target;
	drop->state = state;

	POINT mouse = (POINT){pt.x, pt.y};
	ScreenToClient(drop->win->hwnd, (POINT*)&mouse);

	siWindowEvent* e = &drop->win->e;
	e->mouseRoot = SI_POINT(pt.x, pt.y);
	e->mouseScaled = SI_VEC2(
		(f32)mouse.x / drop->win->scaleFactor.x,
		(f32)mouse.x / drop->win->scaleFactor.y
	);
	e->mouse = SI_POINT(e->mouseScaled.x, e->mouseScaled.y);
	e->type.mouseRelease = true;

	switch (grfKeyState & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) {
		case MK_LBUTTON: {
			siKeyState* key = &e->mouseButtons[SI_MOUSE_LEFT - 1];
			key->clicked = false;
			key->pressed = false;
			key->released = true;
			break;
		}
		case MK_MBUTTON: {
			siKeyState* key = &e->mouseButtons[SI_MOUSE_RIGHT - 1];
			key->clicked = false;
			key->pressed = false;
			key->released = true;
			break;
		}
		case MK_RBUTTON: {
			siKeyState* key = &e->mouseButtons[SI_MOUSE_RIGHT - 1];
			key->clicked = false;
			key->pressed = false;
			key->released = true;
			break;
		}
	}
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	siWindow* win = (siWindow*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	if (win == nil) {
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}

	siWindowEvent* e = &win->e;

	switch (msg) {
		case WM_QUIT: {
			ReleaseDC(win->hwnd, win->hdc);
			DestroyWindow(win->hwnd);
			break;
		}
		case WM_CLOSE: {
			e->type.isClosed = true;
			break;
		}
		case WM_WINDOWPOSCHANGED: {
			WINDOWPOS* change = (WINDOWPOS*)lParam;
			siPoint pos = SI_POINT(change->x, change->y);

			if (!si_pointCmp(e->windowPos, pos)) {
				e->type.windowMove = true;
				e->windowPos = pos;
			}
			else {
				siapp__resizeWindow(win, change->cx, change->cy);
			}
			break;
		}
		case WM_KILLFOCUS: {
			e->type.windowFocusChange = true;
			e->focus = false;
			break;
		}
		case WM_SETFOCUS: {
			e->type.windowFocusChange = true;
			e->focus = true;
			break;
		}
		case WM_MOUSEMOVE: {
			i32 mouseX = SI_LOWORD_XY(lParam);
			i32 mouseY = SI_HIWORD_XY(lParam);

			e->type.mouseMove = true;
			e->mouseScaled = SI_VEC2((f32)mouseX / win->scaleFactor.x, (f32)mouseY / win->scaleFactor.y);
			e->mouse = SI_POINT(e->mouseScaled.x, e->mouseScaled.y);
			break;
		}
		case WM_MOUSEWHEEL: {
			e->type.mouseScroll = true;
			i32 delta = GET_WHEEL_DELTA_WPARAM(wParam);
			e->mouseWheel = (delta < 0) ? SI_MOUSE_WHEEL_DOWN : SI_MOUSE_WHEEL_UP;
			break;
		}
		case WM_LBUTTONDOWN: {
			e->type.mousePress = true;
			siapp__mouseButtonPress(e, SI_MOUSE_LEFT);
			break;
		}
		case WM_MBUTTONDOWN: {
			e->type.mousePress = true;
			siapp__mouseButtonPress(e, SI_MOUSE_MIDDLE);
			break;
		}
		case WM_RBUTTONDOWN: {
			e->type.mousePress = true;
			siapp__mouseButtonPress(e, SI_MOUSE_RIGHT);
			break;
		}
		case WM_XBUTTONDOWN: {
			e->type.mousePress = true;

			/* NOTE(EimaMei): 'GET_XBUTTON_WPARAM' either returns 1 for X1 or 2
			 * for X2. Since SI_MOUSE_X1 is 4 while SI_MOUSE_X2, we can easily
			 * convert the given result by adding 3. */
			siMouseButtonType xButton = GET_XBUTTON_WPARAM(wParam) + 3;
			siapp__mouseButtonPress(e, xButton);
			break;
		}

		case WM_LBUTTONUP: {
			e->type.mouseRelease = true;
			siapp__mouseButtonRelease(e, SI_MOUSE_LEFT);
			break;
		}
		case WM_MBUTTONUP: {
			e->type.mouseRelease = true;
			siapp__mouseButtonRelease(e, SI_MOUSE_MIDDLE);
			break;
		}
		case WM_RBUTTONUP: {
			e->type.mouseRelease = true;
			siapp__mouseButtonRelease(e, SI_MOUSE_RIGHT);
			break;
		}
		case WM_XBUTTONUP: {
			e->type.mouseRelease = true;

			/* NOTE(EimaMei): 'GET_XBUTTON_WPARAM' either returns 1 for X1 or 2
			 * for X2. Since SI_MOUSE_X1 is 4 while SI_MOUSE_X2, we can easily
			 * convert the given result by adding 3. */
			siMouseButtonType xButton = GET_XBUTTON_WPARAM(wParam) + 3;
			siapp__mouseButtonRelease(e, xButton);
			break;
		}

		case WM_INPUT: {
			RAWINPUT raw;
			unsigned int size = sizeof(RAWINPUT);

			if (!GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER))) {
				return 0;
			}

			// NOTE(bill): Many thanks to https://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input/
			// for the
			RAWKEYBOARD* rawKb = &raw.data.keyboard;
			unsigned int vk = rawKb->VKey;
			unsigned int scanCode = rawKb->MakeCode;
			unsigned int flags = rawKb->Flags;
			// NOTE(bill): e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
			// NOTE(bill): http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
			b32 isE0   = (flags & RI_KEY_E0) != 0;
			b32 isE1   = (flags & RI_KEY_E1) != 0;
			b32 isUp   = (flags & RI_KEY_BREAK) != 0;
			b32 isDown = !isUp;

			switch (vk) {
				case 255: return 0;
				case VK_SHIFT: vk = MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX); break;
				case VK_NUMLOCK: scanCode = MapVirtualKeyW(vk, MAPVK_VK_TO_VSC) | 0x100; break;
			}

			e->type.keyPress = win->e.type.keyPress || isDown;
			e->type.keyRelease = isUp;


			// NOTE(bill): Escaped sequences, turn vk into the correct scan code
			// except for VK_PAUSE (it's a bug)
			if (isE1) {
				scanCode = (vk != VK_PAUSE)
					? MapVirtualKeyW(vk, MAPVK_VK_TO_VSC)
					: 0x45;
			}

			switch (vk) {
				case VK_CONTROL: vk = (isE0) ? VK_RCONTROL : VK_LCONTROL; break;
				case VK_MENU:    vk = (isE0) ? VK_RMENU    : VK_LMENU;   break;

				case VK_RETURN: if (isE0)  vk = VK_SEPARATOR; break; // NOTE(bill): Numpad return
				case VK_DELETE: if (!isE0) vk = VK_DECIMAL;   break; // NOTE(bill): Numpad dot
				case VK_INSERT: if (!isE0) vk = VK_NUMPAD0;   break;
				case VK_HOME:   if (!isE0) vk = VK_NUMPAD7;   break;
				case VK_END:    if (!isE0) vk = VK_NUMPAD1;   break;
				case VK_PRIOR:  if (!isE0) vk = VK_NUMPAD9;   break;
				case VK_NEXT:   if (!isE0) vk = VK_NUMPAD3;   break;

				// NOTE(bill): The standard arrow keys will always have their e0 bit set, but the
				// corresponding keys on the NUMPAD will not.
				case VK_LEFT:  if (!isE0) vk = VK_NUMPAD4; break;
				case VK_RIGHT: if (!isE0) vk = VK_NUMPAD6; break;
				case VK_UP:    if (!isE0) vk = VK_NUMPAD8; break;
				case VK_DOWN:  if (!isE0) vk = VK_NUMPAD2; break;

				// NUMPAD 5 doesn't have its e0 bit set
				case VK_CLEAR: if (!isE0) vk = VK_NUMPAD5; break;
			}

			siKeyType key = siapp_osKeyToSili(vk);
			e->curKey = key;

			siKeyState* keyState = &e->keys[key];
			keyState->clicked = (!keyState->pressed && isDown);
			keyState->pressed = isDown;
			keyState->released = isUp;

			if (keyState->clicked || isUp) {
				SK_TO_INT(e->keys[SK__EVENT]) |= SI_BIT(7);
				e->__private.keyCache[e->__private.keyCacheLen % 16] = key;
				e->__private.keyCacheLen += 1;
			}

			if (isDown && !isE1 && !isE0) {
				u16 buf[4];
				i32 numChars = ToUnicode(vk, scanCode, __win32KBState, buf, countof(buf) - 1, 0);
				SI_STOPIF(numChars == 0, break);

				usize len = e->charBufferLen;
				usize addedLen;

				siAllocator tmp = si_allocatorMakeTmp(&e->charBuffer[len], sizeof(e->charBuffer) - len);
				si_utf16ToUtf8Str(&tmp, buf, &addedLen);
				e->charBufferLen += addedLen;
			}
			break;
		}
		default: return DefWindowProcW(hwnd, msg, wParam, lParam);
	}

	return 0;
}
LRESULT CALLBACK WndProcDropFile(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	siDropEvent* out = (siDropEvent*)dwRefData;

	switch (uMsg) {
		case WM_NCDESTROY: {
			RevokeDragDrop(hwnd);
			RemoveWindowSubclass(hwnd, &WndProcDropFile, uIdSubclass);
			return 0;
		}
	}

	WindowProc(out->win->hwnd, uMsg, wParam, lParam);
	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

#if 1 /* Drag 'n Drop functions. Can be ignored. */
intern ULONG IDropTarget_AddRef(IDropTarget* target) { return 1; SI_UNUSED(target); }
intern ULONG IDropTarget_Release(IDropTarget* target) { return 0; SI_UNUSED(target); }
F_TRAITS(intern)
HRESULT IDropTarget_QueryInterface(IDropTarget *target, REFIID riid, LPVOID* ppvObject) {
	*ppvObject = target;
	return S_OK;
	SI_UNUSED(riid);
}
F_TRAITS(intern)
HRESULT IDropTarget_DragEnter(IDropTarget* target, IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
	u32 effect = siapp__dropUpdatePress(target, grfKeyState, pt, SI_DRAG_ENTER);

	siDropEvent* drop = (siDropEvent*)target;
	drop->pDataObj = pDataObj;
	*pdwEffect &= effect;

	return S_OK;
}
F_TRAITS(intern)
HRESULT IDropTarget_DragOver(IDropTarget* target, DWORD grfKeyState,
		POINTL pt, DWORD* pdwEffect) {
	siDropEvent* drop = (siDropEvent*)target;
	SI_STOPIF(drop->state == SI_DRAG_ENTER, return S_OK);

	u32 effect = siapp__dropUpdatePress(target, grfKeyState, pt, SI_DRAG_OVER);
	*pdwEffect &= effect;
	return S_OK;
}
F_TRAITS(intern)
HRESULT IDropTarget_DragLeave(IDropTarget* target) {
	siDropEvent* drop = (siDropEvent*)target;
	drop->state = SI_DRAG_LEAVE;
	return S_OK;
}
F_TRAITS(intern)
HRESULT IDropTarget_Drop(IDropTarget* target, IDataObject* pDataObj,
		DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
	siapp__dropUpdateRelease(target, grfKeyState, pt, SI_DRAG_DROP);

	siDropEvent* drop = (siDropEvent*)target;
	drop->pDataObj = pDataObj;
	*pdwEffect = DROPEFFECT_NONE;

	return S_OK;
}
#endif

#elif defined(SIAPP_PLATFORM_API_X11)

intern Display* SI_X11_DISPLAY = nil;

intern Atom WM_DELETE_WINDOW,
            NET_WM_NAME,
            NET_WM_ICON,
            XA_STRING,
            UTF8_STRING,
            CLIPBOARD,
            XSEL_DATA,
            SAVE_TARGETS,
            TARGETS,
            MULTIPLE,
            ATOM_PAIR,
            CLIPBOARD_MANAGER;

intern Atom XdndAware,
            XdndTypeList,
            XdndSelection,
            XdndEnter,
            XdndPosition,
            XdndStatus,
            XdndLeave,
            XdndDrop,
            XdndFinished,
            XdndActionCopy;


intern Cursor SI_X11_CURSORS[4] = {0};
intern siWindow* SI_ROOT_WINDOW = nil;

F_TRAITS(inline intern)
void siapp__x11CheckStartup(void) {
    SI_STOPIF(SI_X11_DISPLAY != nil, return);

    XInitThreads();
    SI_X11_DISPLAY = XOpenDisplay(nil);
    SI_ASSERT_NOT_NULL(SI_X11_DISPLAY);

    NET_WM_NAME = XInternAtom(SI_X11_DISPLAY, "_NET_WM_NAME", False);
    NET_WM_ICON = XInternAtom(SI_X11_DISPLAY, "_NET_WM_ICON", False);
    UTF8_STRING = XInternAtom(SI_X11_DISPLAY, "UTF8_STRING", False);
    XA_STRING = XInternAtom(SI_X11_DISPLAY, "XA_STRING", False);
    CLIPBOARD   = XInternAtom(SI_X11_DISPLAY, "CLIPBOARD", False);
	SAVE_TARGETS = XInternAtom(SI_X11_DISPLAY, "SAVE_TARGETS", False);
	TARGETS = XInternAtom(SI_X11_DISPLAY, "TARGETS", False);
	MULTIPLE = XInternAtom(SI_X11_DISPLAY, "MULTIPLE", False);
	ATOM_PAIR = XInternAtom(SI_X11_DISPLAY, "ATOM_PAIR", False);
	CLIPBOARD_MANAGER = XInternAtom(SI_X11_DISPLAY, "CLIPBOARD_MANAGER", False);
    XSEL_DATA   = XInternAtom(SI_X11_DISPLAY, "XSEL_DATA", False);
	WM_DELETE_WINDOW = XInternAtom(SI_X11_DISPLAY, "WM_DELETE_WINDOW", True);
}
#endif


#if 1 /* OpenGL */

#define MULTILINE_STR(...) #__VA_ARGS__

typedef SI_ENUM(i32, siVboIndex) {
	SI_VBO_POS = 0,
	SI_VBO_TEX,
	SI_VBO_CLR,
	SI_VBO_ID,
	SI_VBO_ELM,
	SI_VBO_OFFSET
};

typedef SI_ENUM(i32, siShaderIndex) {
	SI_SHADER_POS = 0,
	SI_SHADER_TEX,
	SI_SHADER_CLR,
	SI_SHADER_ID,
	SI_SHADER_OFFSET
};

#define i32ToNDCX(num, windowCord) (((+2.0f * (num)) / (windowCord)) - 1.0f)
#define i32ToNDCY(num, windowCord) (((-2.0f * (num)) / (windowCord)) + 1.0f)

#define GL_BUFFER_MAKE(ID, var, size) \
	do { \
		glBindBuffer(GL_ARRAY_BUFFER, gl->VBOs[ID]); \
		glBufferStorage(GL_ARRAY_BUFFER, size, nil, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT); \
		var = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT); \
        memset(var, 0, size); \
	} while(0)

static siOpenGLInfo glInfo = {false, {0, 0}, 0, {0, 0}, 8, 4, false, 0};

static const char defaultVShaderCode[] = MULTILINE_STR(
	\x23version 440\n

	in vec3 pos;
	in vec2 tex;
	in vec4 clr;
	in uvec2 info;

	out vec2 fragTex;
	out vec4 fragClr;
	out uint fragTexID;
	uniform mat4 mvp[%u]; // TODO(EimaMei): FIX THIS!!!!!

	void main() {
		fragTex = tex;
		fragClr = clr;
		fragTexID = info.x;
		gl_Position = vec4(pos, 1.0) * mvp[info.y];
	}
);
static const char defaultFShaderCode[] = MULTILINE_STR(
	\x23version 440\n

	in vec2 fragTex;
	in vec4 fragClr;
	in flat uint fragTexID;

	out vec4 finalColor;

	uniform sampler2D textures[32]; // TODO(EimaMei): Add for more support.

	void main() {
		finalColor = texture(textures[fragTexID], fragTex) * fragClr;
	}
);

i32 si_OpenGLShaderMake(i32 shaderType, cstring source) {
	u32 shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	i32 success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
#if !defined(SI_RELEASE_MODE)
		char errMsg[SI_KILO(4)];
		glGetShaderInfoLog(shader, sizeof(errMsg), NULL, errMsg);
		si_fprint(SI_STDERR, errMsg);

#endif
		glDeleteShader(shader);
		return -1;
	}

	return shader;
}

void siapp__addVertexesToCMD(siWinRenderingCtxOpenGL* gl, u32 count, u32 vertexCount) {
	siOpenGLIDs* IDs = &gl->batchInfo[gl->drawCounter];
	IDs->texID = gl->curTex->atlas->texID - 1;
	IDs->matrixID = gl->drawCounter;

	siOpenGLDrawCMD* cmd = &gl->CMDs[gl->drawCounter];
	cmd->count = count;
	cmd->instanceCount = 1;
	cmd->firstIndex = 0;
	cmd->baseVertex = gl->vertexCounter - vertexCount;
	cmd->baseInstance = gl->drawCounter;

	gl->drawCounter += 1;

	SI_ASSERT(gl->gradientLen <= vertexCount);
	for_range (i, 0, gl->gradientLen) {
		siColor tmp = gl->gradient[i];

        siVec4 color = SI_VEC4(tmp.r / 255.0f, tmp.g / 255.0f, tmp.b / 255.0f, tmp.a / 255.0f);
		gl->colors[cmd->baseVertex + i] = color;
	}
	gl->gradientLen = 0;
}

void RGL_opengl_getError(void) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		switch (err) {
		case GL_INVALID_ENUM:
			si_print("OpenGL error: GL_INVALID_ENUM\n");
			break;
		case GL_INVALID_VALUE:
			si_print("OpenGL error: GL_INVALID_VALUE\n");
			break;
		case GL_INVALID_OPERATION:
			si_print("OpenGL error: GL_INVALID_OPERATION\n");
			break;
		case GL_STACK_OVERFLOW:
			si_print("OpenGL error: GL_STACK_OVERFLOW\n");
			break;
		case GL_STACK_UNDERFLOW:
			si_print("OpenGL error: GL_STACK_UNDERFLOW\n");
			break;
		default:
			si_printf("OpenGL error: Unknown error code %#X\n", err);
			break;
		}
	}
}
siMatrix rglMatrixIdentity(void) {
	return (siMatrix) {
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		}
	};
}
#endif

#endif


F_TRAITS(inline)
siWindow* siapp_windowMake(siAllocator* alloc, cstring name, siArea size,
		siWindowArg arg, u32 maxDrawCount, u32 maxTexCount, siArea maxTexRes) {
	return siapp_windowMakeEx(alloc, name, SI_POINT(0, 0), size, arg, maxDrawCount, maxTexCount, maxTexRes);
}
siWindow* siapp_windowMakeEx(siAllocator* alloc, cstring name, siPoint pos,
		siArea size, siWindowArg arg, u32 maxDrawCount, u32 maxTexCount, siArea maxTexRes) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(name);

	siWindow* win = si_mallocItem(alloc, siWindow);
#if 0
	SI_WINDOW_FULLSCREEN      = SI_BIT(1),
	SI_WINDOW_BORDERLESS      = SI_BIT(2),
	SI_WINDOW_RESIZABLE       = SI_BIT(3),
	SI_WINDOW_MINIMIZED       = SI_BIT(4),
	SI_WINDOW_MAXIMIZED       = SI_BIT(5),
	SI_WINDOW_RENDERING_CPU   = SI_BIT(7),
#endif

    if (arg & SI_WINDOW_OPTIMAL_SIZE) {
        SI_ASSERT_MSG(size.width == 0 && size.height == 0, "The selected resolution must be set to zeros beforehand to use 'SI_WINDOW_OPTIMAL_SIZE'.");
        siArea area = siapp_screenGetCurrentSize();
        size.width = area.width / 2;
        size.height = area.height / 2;
    }
    if (arg & SI_WINDOW_CENTER) {
        siArea area = siapp_screenGetCurrentSize();
        pos.x = (area.width - size.width) / 2;
        pos.y = (area.height - size.height) / 2;
    }

#if defined(SIAPP_PLATFORM_API_WIN32)
	siAllocator* stack = si_allocatorMakeStack(SI_KILO(4) * sizeof(u16));
	siUtf16String utf16name = si_utf8ToUtf16Str(stack, name, nil);
	HINSTANCE inh = GetModuleHandleW(nil);

	if (SI_WINDOWS_NUM == 0) {
		RAWINPUTDEVICE rid[1] = {0};
		rid[0].usUsagePage = 0x01;
		rid[0].usUsage     = 0x06;
		rid[0].dwFlags     = RIDEV_NOLEGACY;
		rid[0].hwndTarget  = nil;

		b32 res = RegisterRawInputDevices(rid, countof(rid), sizeof(rid[0]));
		SIAPP_ERROR_CHECK(res == false, "RegisterRawInputDevices");
	}
	SI_WINDOWS_NUM += 1;

	WNDCLASSW class = {0};
	class.style = CS_HREDRAW | CS_VREDRAW;
	class.lpszClassName = si_utf8ToUtf16Str(stack, si_cstrMakeFmt(stack, "%i-siliapp-klasė", SI_WINDOWS_NUM), nil);
	class.hInstance = inh;
	class.hCursor = LoadCursor(NULL, IDC_ARROW);
	class.lpfnWndProc = WindowProc;

	i32 err = RegisterClassW(&class);
	SIAPP_ERROR_CHECK(err == 0, "RegisterClassW");

	DWORD windowStyle =
		WS_CAPTION | WS_SYSMENU | WS_BORDER |
		WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;


	RECT adjustRect = {pos.x, pos.y, size.width, size.height};
	AdjustWindowRect(&adjustRect, windowStyle, 0);
	size.width = adjustRect.right;
	size.height = adjustRect.bottom;

	HWND hwnd = CreateWindowW(
		class.lpszClassName,
		utf16name, windowStyle,
		pos.x, pos.y, size.width, size.height,
		nil, nil, inh, nil
	);
	SIAPP_ERROR_CHECK(hwnd == nil, "CreateWindowW");

	SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)win);

	win->hwnd = hwnd;
	win->hdc = GetDC(hwnd);
#elif defined(SIAPP_PLATFORM_API_X11)
    siapp__x11CheckStartup();
    win->display = XOpenDisplay(nil);
    SIAPP_ERROR_CHECK(win->display == nil, "XOpenDisplay(nil)");

    i32 screen = XDefaultScreen(win->display);
    XSetWindowAttributes wa;
    wa.event_mask =
        KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
        PointerMotionMask | StructureNotifyMask | FocusChangeMask | EnterWindowMask |
        LeaveWindowMask;

    win->hwnd = XCreateWindow(
        win->display, XDefaultRootWindow(win->display),
        pos.x, pos.y, size.width, size.height, 0, XDefaultDepth(win->display, screen), InputOutput,
	    XDefaultVisual(win->display, screen),
        CWEventMask, &wa
    );

    XStoreName(win->display, win->hwnd, name);
    XChangeProperty(
        win->display, win->hwnd, NET_WM_NAME, UTF8_STRING,
        8, PropModeReplace, (siByte*)name, si_cstrLen(name)
    );
    XSetWMProtocols(win->display, win->hwnd, (Atom*)&WM_DELETE_WINDOW, 1);

    SI_WINDOWS_NUM += 1;
    SI_ROOT_WINDOW = win;
    win->__x11BlankCursor = 0;
    win->__x11DndHead = (rawptr)USIZE_MAX;
#endif
	win->alloc = alloc;
	win->arg = arg;
	win->scaleFactor = SI_VEC2(1, 1);
	win->originalSize = size;
	win->cursor = SI_CURSOR_DEFAULT;
	win->e = (siWindowEvent){0};
	win->textColor.vec4 = SI_VEC4(1, 1, 1, 1);
	win->imageColor = SI_VEC4(1, 1, 1, 1);

    maxTexRes.width += 1;
    maxTexRes.height += 1;
    maxTexCount = si_max(1, maxTexCount);

	switch (arg & SI_WINDOW_RENDERING_BITS) {
		case SI_WINDOW_RENDERING_OPENGL: {
			siapp_windowOpenGLInit(win, maxDrawCount, maxTexCount, maxTexRes);
			break;
		}
		case SI_WINDOW_RENDERING_CPU: {
			SI_PANIC_MSG("CPU rendering isn't supported as of now");
			break;
		}
		default: {
			win->atlas = siapp_textureAtlasMake(win, maxTexRes, maxTexCount, SI_RESIZE_DEFAULT);
		}
	}

	siapp_windowShow(win, (arg & SI_WINDOW_HIDDEN) == 0);

	return win;
}
const siWindowEvent* siapp_windowUpdate(siWindow* win, b32 await) {
	SI_ASSERT_NOT_NULL(win);

	siWindowEvent* out = &win->e;
	win->cursorSet = false;

	out->type = (siEventType){0};

	{
		u32 eventBits = SK_TO_INT(out->keys[SK__EVENT]);

		if (SI_NUM_BIT_GET(eventBits, SI_BIT(7))) { // Keys.
			usize* keyCacheLen = &out->__private.keyCacheLen;
			siKeyType* keyCache = out->__private.keyCache;

			for_range (i, 0, *keyCacheLen) { // TODO(EimaMei): Optimize this like the mouse button clear (len might not even be required).
				siKeyState* s = &out->keys[keyCache[i]];
				SK_TO_INT(*s) &= ~0x5; // Clear the clicked and released state at once.
			}

			SK_TO_INT(out->keys[SK__EVENT]) &= ~SI_BIT(7);
			*keyCacheLen= 0;
		}
		if (SI_NUM_BIT_GET(eventBits, SI_BIT(6))) { // Mouse buttons.
			u64* mouseButtons = (u64*)out->mouseButtons;
			*mouseButtons &= ~0x505050505; // Clears the clicked and released states of every mouse button at once.
			SK_TO_INT(out->keys[SK__EVENT]) &= ~SI_BIT(6);
		}
	}

#if defined(SIAPP_PLATFORM_API_WIN32)
	{
		//if (win->keyListenerEnabled) {
		GetKeyState(VK_SHIFT);
		GetKeyState(VK_MENU);
		GetKeyboardState(__win32KBState);
		out->charBufferLen = 0;
	}


	{
		GetCursorPos((POINT*)&out->mouseRoot);
		siPoint point = out->mouseRoot;
		siRect rect = SI_RECT_PA(out->windowPos, out->windowSize);
		out->mouseInside = siapp__collideRectPoint(rect, point);
	}

	MSG msg = {0};
	while (true) {
		b32 isOkay = PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
		if (!isOkay) break;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
#elif defined (SIAPP_PLATFORM_API_X11)
    SI_ROOT_WINDOW = win;

    typedef struct {
        long source, version;
        i32 format;
    } XDND; /* data structure for xdnd events */
    static XDND xdnd;
    static siDropEvent* curNode = nil;


    XEvent event;
	if (await) {
		/* blocks until next event is recv'd, but DOES NOT REMOVE said event from queue so that
		it will still be caught during the while loop below */
		XPeekEvent(win->display, &event);
	}

    #define SI_CHECK_WIN(struct, win) SI_STOPIF(struct.window != win, break)

    while (XEventsQueued(win->display, QueuedAlready) + XEventsQueued(win->display, QueuedAfterReading)) {
        XNextEvent(win->display, &event);

        switch (event.type) {
            case ClientMessage: {
                SI_CHECK_WIN(event.xclient, win->hwnd);
                out->type.isClosed = (event.xclient.data.l[0] == (long)WM_DELETE_WINDOW);

				/* NOTE(EimaMei): All of this is from 'https://github.com/ColleagueRiley/RGFW/blob/main/RGFW.h'. */
                if (event.xclient.message_type == XdndEnter) {
				    u64 count;
                    b32 formFree = 0;
				    Atom* formats = (Atom*)0;
				    Bool list = event.xclient.data.l[1] & 1;

				    xdnd.source  = event.xclient.data.l[0];
				    xdnd.version = event.xclient.data.l[1] >> 24;
				    xdnd.format  = None;

				    if (xdnd.version > 5)
				    	break;

				    if (list) {
				    	Atom actualType;
				    	i32 actualFormat;
				    	u64 bytesAfter;

				    	XGetWindowProperty(
                            win->display, xdnd.source, XdndTypeList,
				    		0, INT64_MAX, False, 4,
				    		&actualType, &actualFormat, &count, &bytesAfter,
				    		(u8**)&formats
                        );
				    }
				    else {
				    	formats = (Atom*)malloc(event.xclient.data.l[2] + event.xclient.data.l[3] + event.xclient.data.l[4]);
				    	formFree = 1;

				    	count = 0;

				    	if (event.xclient.data.l[2] != None)
				    		formats[count++] = event.xclient.data.l[2];
				    	if (event.xclient.data.l[3] != None)
				    		formats[count++] = event.xclient.data.l[3];
				    	if (event.xclient.data.l[4] != None)
				    		formats[count++] = event.xclient.data.l[4];
				    }

				    u32 i;
				    for (i = 0;  i < count;  i++) {
				    	char* name = XGetAtomName(win->display, formats[i]);

				    	char* links[2] = {(char*)(const char*)"text/uri-list", (char*)(const char*)"text/plain"};
				    	for (; 1; name++) {
				    		u32 j;
				    		for (j = 0; j < 2; j++) {
				    			if (*links[j] != *name) {
				    				links[j] = (char*)(const char*)"\1";
				    				continue;
				    			}

				    			if (*links[j] == '\0' && *name == '\0')
				    				xdnd.format = formats[i];

				    			if (*links[j] != '\0' && *links[j] != '\1')
				    				links[j]++;
				    		}

				    		if (*name == '\0')
				    			break;
				    	}
				    }

				    if (list && formats) {
				    	XFree(formats);
				    	formats = (Atom*)0;
				    }
				    else if (formFree && formats != (Atom*)0) {
				    	free(formats);

				    	formats = (Atom*)0;
				    	formFree = 1;
				    }
                }
                else if (event.xclient.message_type == XdndPosition) {
                    const i32 xabs = (event.xclient.data.l[2] >> 16) & 0xffff;
				    const i32 yabs = (event.xclient.data.l[2]) & 0xffff;
				    Window dummy;
				    i32 xpos, ypos;

				    if (xdnd.version > 5)
				    	break;

				    XTranslateCoordinates(
                        win->display, XDefaultRootWindow(win->display), win->hwnd,
                        xabs, yabs, &xpos, &ypos, &dummy
                    );

                    siPoint pos = SI_POINT(xpos, ypos);
                    out->type.mouseMove = true;
                    out->mouse = pos;

                    siDropEvent* node = win->__x11DndHead;

                    while (node != nil) { // TODO: optimize this
                        if (siapp__collideRectPoint(node->rect, pos)) {
                            if (curNode != nil && curNode != node) {
                                curNode->state = SI_DRAG_LEAVE;
                                curNode = nil;
                            }
                            break;
                        }
                        node = (siDropEvent*)node->next;
                    }
                    node->state = (curNode == nil) ? SI_DRAG_ENTER : SI_DRAG_OVER;
                    curNode = node;


				    XEvent reply = { ClientMessage };
				    reply.xclient.window = xdnd.source;
				    reply.xclient.message_type = XdndStatus;
				    reply.xclient.format = 32;
				    reply.xclient.data.l[0] = win->hwnd;
				    reply.xclient.data.l[2] = 0;
				    reply.xclient.data.l[3] = 0;

				    if (xdnd.format) {
				    	reply.xclient.data.l[1] = 1;
				    	if (xdnd.version >= 2)
				    		reply.xclient.data.l[4] = XdndActionCopy;
				    }

				    XSendEvent(win->display, xdnd.source, False, NoEventMask, &reply);
				    XFlush(win->display);
                }
                else if (event.xclient.message_type == XdndDrop) {
                    if (xdnd.version > 5)
                        break;

                    if (xdnd.format) {
                        Time time = (xdnd.version >= 1)
                            ? event.xclient.data.l[2]
                            : CurrentTime;

                        XConvertSelection(
                            win->display, XdndSelection, xdnd.format,
                            XdndSelection, win->hwnd, time
                        );
                    }
                    else if (xdnd.version >= 2) {
                        XEvent reply = { ClientMessage };
                        reply.xclient.window = xdnd.source;
                        reply.xclient.message_type = XdndFinished;
                        reply.xclient.format = 32;
                        reply.xclient.data.l[0] = win->hwnd;
                        reply.xclient.data.l[1] = 0;
                        reply.xclient.data.l[2] = None;

                        XSendEvent(win->display, xdnd.source, False, NoEventMask, &reply);
                        XFlush(win->display);
                    }
                }
                else if (event.xclient.message_type == XdndLeave) {
                    curNode->state = SI_DRAG_LEAVE;
                    curNode = nil;
                }

                break;
            }
            case SelectionNotify: {
                SI_STOPIF(event.xselection.property != XdndSelection, break);
                SI_CHECK_WIN(event.xclient, win->hwnd);

                char* data;
                u64 result;

                Atom actualType;
                i32 actualFormat;
                u64 bytesAfter;

                XGetWindowProperty(
                    win->display, event.xselection.requestor, event.xselection.property,
                    0, INT64_MAX, False, event.xselection.target,
                    &actualType, &actualFormat, &result, &bytesAfter, (u8**)&data
                );
                curNode->state = SI_DRAG_DROP;
                curNode->data = data;
                curNode = nil;

                if (xdnd.version >= 2) {
                    XEvent reply = { ClientMessage };
                    reply.xclient.window = xdnd.source;
                    reply.xclient.message_type = XdndFinished;
                    reply.xclient.format = 32;
                    reply.xclient.data.l[0] = win->hwnd;
                    reply.xclient.data.l[1] = result;
                    reply.xclient.data.l[2] = XdndActionCopy;

                    XSendEvent(win->display, xdnd.source, False, NoEventMask, &reply);
                    XFlush(win->display);
                }

                break;
            }
            case KeyPress: {
                SI_CHECK_WIN(event.xkey, win->hwnd);
                KeySym sym = XLookupKeysym(&event.xkey, 0);
                siKeyType key = siapp_osKeyToSili(sym);

                out->type.keyPress = true;
                out->curKey = key;

                siKeyState* keyState = &out->keys[key];
                keyState->clicked = !keyState->pressed;
                keyState->pressed = true;
                keyState->released = false;

                if (keyState->clicked) {
                    SK_TO_INT(out->keys[SK__EVENT]) |= SI_BIT(7);
                    out->__private.keyCache[out->__private.keyCacheLen % 16] = key;
                    out->__private.keyCacheLen += 1;
                }
#if 0
                if (isDown && !isE1 && !isE0) {
                    u16 buf[4];
                    i32 numChars = ToUnicode(vk, scanCode, __win32KBState, buf, countof(buf) - 1, 0);
                    SI_STOPIF(numChars == 0, break);

                    usize len = e->charBufferLen;
                    usize addedLen;

                    siAllocator tmp = si_allocatorMakeTmp(&e->charBuffer[len], sizeof(e->charBuffer) - len);
                    si_utf16ToUtf8Str(&tmp, buf, &addedLen);
                    e->charBufferLen += addedLen;
                }
#endif
                break;
            }
            case KeyRelease: {
                SI_CHECK_WIN(event.xkey, win->hwnd);
                if (XEventsQueued(win->display, QueuedAfterReading)) { /* get next event if there is one*/
	    			XEvent next;
	    		    XPeekEvent(win->display, &next);

			    	if (event.xkey.time == next.xkey.time
                        && event.xkey.keycode == next.xkey.keycode) {
                        break;
                    }
		    	}

                KeySym sym = XLookupKeysym(&event.xkey, 0);
                siKeyType key = siapp_osKeyToSili(sym);

                out->type.keyRelease = true;
                out->curKey = key;

                siKeyState* keyState = &out->keys[key];
                keyState->clicked = false;
                keyState->pressed = false;
                keyState->released = true;

                SK_TO_INT(out->keys[SK__EVENT]) |= SI_BIT(7);
                out->__private.keyCache[out->__private.keyCacheLen % 16] = key;
                out->__private.keyCacheLen += 1;
                break;
            }
            case MotionNotify: {
                SI_CHECK_WIN(event.xmotion, win->hwnd);
                out->type.mouseMove = true;

                out->mouse = SI_POINT(event.xmotion.x, event.xmotion.y);
                out->mouseRoot = SI_POINT(event.xmotion.x_root, event.xmotion.y_root);
                break;
            }
            case ButtonPress: {
                SI_CHECK_WIN(event.xbutton, win->hwnd);

                i32 button = event.xbutton.button;
                if (si_between(button, 4, 7)) { // Mouse wheel up/down/left/right
                    out->type.mouseScroll = true;
                    out->mouseWheel = button - 3;
                    out->mouse = SI_POINT(event.xbutton.x, event.xbutton.y);
                    out->mouseRoot = SI_POINT(event.xbutton.x_root, event.xbutton.y_root);
                    break;
                }
                else if (button > 7) { button -= 4; } // X1/X2 buttons

                siapp__mouseButtonPress(out, button);
                out->type.mousePress = true;
                out->mouse = SI_POINT(event.xbutton.x, event.xbutton.y);
                out->mouseRoot = SI_POINT(event.xbutton.x_root, event.xbutton.y_root);
                break;
            }
            case ButtonRelease: {
                SI_CHECK_WIN(event.xbutton, win->hwnd);

                i32 button = event.xbutton.button;
                if (si_between(button, 4, 7)) { // Mouse wheel up/down/left/right
                    break;
                }
                else if (button > 7) { button -= 4; } // X1/X2 buttons

                siapp__mouseButtonRelease(out, button);
                out->type.mouseRelease = true;
                out->mouse = SI_POINT(event.xbutton.x, event.xbutton.y);
                out->mouseRoot = SI_POINT(event.xbutton.x_root, event.xbutton.y_root);
                break;
            }
            case ConfigureNotify: {
                SI_CHECK_WIN(event.xconfigure, win->hwnd);
                siPoint pos = SI_POINT(event.xconfigure.x, event.xconfigure.y);

                if (SI_TO_U64(&pos) != 0 && !si_pointCmp(out->windowPos, pos)) {
                    out->type.windowMove = true;
                    out->windowPos = pos;
                }
                else {
                    siapp__resizeWindow(win, event.xconfigure.width, event.xconfigure.height);
                }

                break;
            }
            case FocusIn: {
                SI_CHECK_WIN(event.xfocus, win->hwnd);
                out->type.windowFocusChange = true;
                out->focus = true;
                break;
            }
            case FocusOut: {
                SI_CHECK_WIN(event.xfocus, win->hwnd);
                out->type.windowFocusChange = true;
                out->focus = false;
                break;
            }
            case EnterNotify: {
                out->mouseInside = true;
                break;
            }
            case LeaveNotify: {
                out->mouseInside = false;
                break;
            }
        }
    }
    #undef SI_CHECK_WIN
#endif

	f64 prevTime = out->curTime;
	f64 curTime = si_clock();
	out->curTime = curTime;
	out->timeDelta = curTime - prevTime;

    return out;
}
void siapp_windowRender(siWindow* win) {
	SI_ASSERT_NOT_NULL(win);

	switch (win->arg & SI_WINDOW_RENDERING_BITS) {
		case SI_WINDOW_RENDERING_OPENGL: siapp_windowOpenGLRender(win); break;
		case SI_WINDOW_RENDERING_CPU: SI_PANIC();
	}
}
void siapp_windowClear(const siWindow* win) {
	SI_ASSERT_NOT_NULL(win);

	switch (win->arg & SI_WINDOW_RENDERING_BITS) {
		case SI_WINDOW_RENDERING_OPENGL: {
			const siWinRenderingCtxOpenGL* gl = &win->render.opengl;
			glClearColor(gl->bgColor.x, gl->bgColor.y, gl->bgColor.z, gl->bgColor.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			break;
		}
		case SI_WINDOW_RENDERING_CPU: SI_PANIC();
	}
}
F_TRAITS(inline)
void siapp_windowSwapBuffers(const siWindow* win) {
	SI_ASSERT_NOT_NULL(win);

	switch (win->arg & SI_WINDOW_RENDERING_BITS) {
		case SI_WINDOW_RENDERING_OPENGL: {
        #if defined(SIAPP_PLATFORM_API_WIN32)
            SwapBuffers(win->hdc);
        #elif defined(SIAPP_PLATFORM_API_X11)
            glXSwapBuffers(win->display, win->hwnd);
        #endif

			siapp_windowClear(win);
			break;
		}
        default: {
        #if defined(SIAPP_PLATFORM_API_X11)
            XFlush(win->display);
        #endif
        }
	}
}
void siapp_windowClose(siWindow* win) {
	SI_ASSERT_NOT_NULL(win);
	switch (win->arg & SI_WINDOW_RENDERING_BITS) {
		case SI_WINDOW_RENDERING_OPENGL: siapp_windowOpenGLDestroy(win); break;
		case SI_WINDOW_RENDERING_CPU: SI_PANIC_MSG("Other rendering types aren't supported as of now.");
	}

#if defined(SIAPP_PLATFORM_API_WIN32)
	ReleaseDC(win->hwnd, win->hdc);
	DestroyWindow(win->hwnd);
#elif defined(SIAPP_PLATFORM_API_X11)
    SI_STOPIF(win->__x11BlankCursor, XFreeCursor(win->display, win->__x11BlankCursor));
    SI_STOPIF(win->hwnd, XDestroyWindow(win->display, win->hwnd));
    SI_STOPIF(win->display, XCloseDisplay(win->display));
    SI_WINDOWS_NUM -= 1;
    win->__x11BlankCursor = 0;

    if (SI_WINDOWS_NUM == 0) {
        for_range (i, 0, countof(SI_X11_CURSORS)) {
            if (SI_X11_CURSORS[i] != 0) {
                XFreeCursor(SI_X11_DISPLAY, SI_X11_CURSORS[i]);
                SI_X11_CURSORS[i] = 0;
            }
        }

        XCloseDisplay(SI_X11_DISPLAY);
        SI_X11_DISPLAY = nil;
    }
#endif
}

F_TRAITS(inline)
b32 siapp_windowIsRunning(const siWindow* win) {
	SI_ASSERT_NOT_NULL(win);
	return !win->e.type.isClosed;
}

F_TRAITS(inline)
void siapp_windowShow(const siWindow* win, b32 show) {
	SI_ASSERT_NOT_NULL(win);
#if defined(SIAPP_PLATFORM_API_WIN32)
    ShowWindow(win->hwnd, show);
#elif defined(SIAPP_PLATFORM_API_X11)
    if (show) {
        XMapWindow(win->display, win->hwnd);
    }
    else {
        XUnmapWindow(win->display, win->hwnd);
    }
#endif
}


F_TRAITS(inline)
const siWindowEvent* siapp_windowEventGet(const siWindow* win) {
	SI_ASSERT_NOT_NULL(win);
	return &win->e;
}
F_TRAITS(inline)
b32 siapp_windowEventPoll(const siWindow* win, siEventTypeEnum* out) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT_NOT_NULL(out);

	u32 event = SI_TO_U32(&win->e.type);
	u32 bit = *out;
	b32 res = false;

	while (res == false && bit <= SI_EVENT_COUNT) {
		res = event & SI_BIT(bit);
		bit += 1;
	}
	*out = bit;

	return bit != 0 && bit <= SI_EVENT_COUNT;
}


F_TRAITS(inline)
siCursorType siapp_windowCursorGet(const siWindow* win) {
	SI_ASSERT_NOT_NULL(win);
	return win->cursor;
}
void siapp_windowCursorSet(siWindow* win, siCursorType cursor) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT(si_between(cursor, INT32_MIN, SI_CURSOR_COUNT));

    b32 isDif = cursor != siapp_windowCursorGet(win) && !win->cursorSet;
    win->cursorSet = true;

#if defined(SIAPP_PLATFORM_API_WIN32)
    LPCSTR cursorName = nil;

	switch (cursor * isDif) {
		case 0: return ;
		case SI_CURSOR_ARROW: cursorName = IDC_ARROW; break;
		case SI_CURSOR_HAND: cursorName = IDC_HAND; break;
		case SI_CURSOR_DOUBLE_ARROW_HORIZONTAL: cursorName = IDC_SIZEWE; break;
		case SI_CURSOR_TEXT_SELECT: cursorName = IDC_IBEAM; break;
		default: { /* NOTE(EimaMei): If the cursor is negative, it's custom. */
			usize handle = -cursor;

			SetClassLongPtr(win->hwnd, GCLP_HCURSOR, handle);
			SetCursor((HCURSOR)handle);
			win->cursor = cursor;
			return ;
		}
	}
    SetClassLongPtr(win->hwnd, GCLP_HCURSOR, (LONG_PTR)cursorName);
    SetCursor(LoadCursorA(nil, cursorName));
#else
    SI_STOPIF(!isDif, return);

    Cursor cursorVal;
    if (cursor < 0) { // is custom
        cursorVal = -cursor;
    }
    else {
        Cursor* ptr = &SI_X11_CURSORS[cursor];
        if (*ptr == 0) {
            siapp__x11CheckStartup();
            static i32 xCursors[countof(SI_X11_CURSORS) + 1] = {0, XC_left_ptr, XC_hand1, XC_sb_h_double_arrow, XC_xterm};
            *ptr = XCreateFontCursor(SI_X11_DISPLAY, xCursors[cursor]);
        }
        cursorVal = *ptr;
    }
    XDefineCursor(win->display, win->hwnd, cursorVal);
#endif
	win->cursor = cursor;

}
siCursorType siapp_cursorMake(siByte* data, siArea res, u32 channels) {
	SI_ASSERT_NOT_NULL(data);
	SI_ASSERT(si_betweenu(channels, 1, 4));

#if defined(SIAPP_PLATFORM_API_WIN32)
	BITMAPV5HEADER bi = {0};
	bi.bV5Size        = sizeof(bi);
	bi.bV5Width       = res.width;
	bi.bV5Height      = -res.height;
	bi.bV5Planes      = 1;
	bi.bV5BitCount    = channels * 8;
	bi.bV5Compression = (channels == 4) ? BI_BITFIELDS : BI_RGB;
	/* NOTE(EimaMei): For BI_BITFIELDS. */
	bi.bV5AlphaMask   = 0xFF000000;
	bi.bV5GreenMask   = 0x00FF0000;
	bi.bV5BlueMask    = 0x0000FF00;
	bi.bV5RedMask     = 0x000000FF;

	u8* target = nil;
	HDC DC = GetDC(nil);
	HBITMAP color = CreateDIBSection(
		DC, (BITMAPINFO*)&bi, DIB_RGB_COLORS,
		(rawptr*)&target, nil, 0
	);
	ReleaseDC(nil, DC);

	HBITMAP mask = CreateBitmap(res.width, res.height, 1, 1, nil);

	switch (channels) {
		case 4:
			for_range (i, 0, res.width * res.height) {
				memcpy(target, data, 4);
				target += 4;
				data += 4;
			}
			break;
		case 3:
			for_range (i, 0, res.width * res.height * 3) {
				target[i + 0] = data[i + 1];
				target[i + 1] = data[i + 0];
				target[i + 2] = data[i + 2];
			}
			break;
		default: SI_PANIC_MSG("Other channel types aren't supported as of now.");
	}

	ICONINFO icon = {0};
	icon.fIcon    = false;
	icon.xHotspot = 0;
	icon.yHotspot = 0;
	icon.hbmMask  = mask;
	icon.hbmColor = color;

	HCURSOR handle = CreateIconIndirect(&icon);
	DeleteObject(color);
	DeleteObject(mask);

    return -*(i32*)&handle;
#else
    XcursorImage* native = XcursorImageCreate(res.width, res.height);
    native->xhot = 0;
    native->yhot = 0;


    u8* source = data;
    XcursorPixel* target = native->pixels;

	for_range (i, 0, res.width * res.height) {
        u8 alpha;
        switch (channels) {
            case 4: {
                alpha = source[3];
                break;
            }
            case 3: {
                alpha = 255;
                break;
            }
            default: SI_PANIC_MSG("Other channel types aren't supported as of now.");
        }


        *target = (alpha << 24)
            | (((source[0] * alpha) / 255) << 16)
            | (((source[1] * alpha) / 255) <<  8)
            | (((source[2] * alpha) / 255) <<  0);

        target += 1;
        source += 4;
    }

    Cursor handle = XcursorImageLoadCursor(SI_ROOT_WINDOW->display, native);
	XcursorImageDestroy(native);
    return -(i64)handle;
#endif
}
F_TRAITS(inline)
void siapp_cursorFree(siCursorType cursor) {
	SI_ASSERT_MSG(cursor < 0, "Only custom-made cursors need to be freed.");
	usize handle = -cursor;
#if defined(SIAPP_PLATFORM_API_WIN32)
	DestroyCursor((HCURSOR)handle);
#elif defined(SIAPP_PLATFORM_API_X11)
    XFreeCursor(SI_ROOT_WINDOW->display, handle);
#endif
}


F_TRAITS(inline)
void siapp_windowTextColorSet(siWindow* win, siColor color) {
	SI_ASSERT_NOT_NULL(win);
	win->textColor.vec4 = SI_VEC4(
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
		color.a / 255.0f
	);
}
F_TRAITS(inline)
void siapp_windowImageColorSet(siWindow* win, siColor color) {
	SI_ASSERT_NOT_NULL(win);
	win->imageColor = SI_VEC4(
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
		color.a / 255.0f
	);
}
F_TRAITS(inline)
void siapp_windowGradientSet(siWindow* win, const siColor gradient[], usize len) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT_NOT_NULL(gradient);

	siWinRenderingCtxOpenGL* gl = &win->render.opengl; // NOTE(EimaMei): Check if CPU rendering is able to do decent gradients.
	gl->gradient = gradient;
	gl->gradientLen = len;
}

void siapp_windowDragAreaMake(siWindow* win, siRect rect, siDropEvent* out) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT_NOT_NULL(out);

#if defined(SIAPP_PLATFORM_API_WIN32)
	u16 buf[64];
	GetClassNameW(win->hwnd, buf, countof(buf));

	HWND subHwnd = CreateWindowW(
		buf,
		L"", WS_VISIBLE | WS_CHILD,
		rect.x, rect.y, rect.width, rect.height,
		win->hwnd, 0, nil, nil
	);
	SI_ASSERT_NOT_NULL(subHwnd);

	SetWindowSubclass(subHwnd, &WndProcDropFile, 0, (DWORD_PTR)out);

	/* NOTE(EimaMei): Jei kada nors sutiksiu būtent tą daną, kuris sukurė šį
	 * košmarą, jisai gaus į snukį už savo nusikaltimus žmonijai, sąmoningumui
	 * ir protui. Tebūnie tos kalbos mirtis artėjančiais metais. */
	static IDropTargetVtbl vTable = {
		IDropTarget_QueryInterface,
		IDropTarget_AddRef,
		IDropTarget_Release,
		IDropTarget_DragEnter,
		IDropTarget_DragOver,
		IDropTarget_DragLeave,
		IDropTarget_Drop
	};
	out->win = win;
	out->subHwnd = subHwnd;
	out->state = 0;
	out->target.lpVtbl = (IDropTargetVtbl*)&vTable;

	OleInitialize(nil);
	RegisterDragDrop(subHwnd, (LPDROPTARGET)&out->target);
#elif defined(SIAPP_PLATFORM_API_X11)
    if (win->__x11DndHead == (rawptr)USIZE_MAX) {
        XdndAware         = XInternAtom(SI_X11_DISPLAY, "XdndAware",         False);
        XdndTypeList      = XInternAtom(SI_X11_DISPLAY, "XdndTypeList",      False);
        XdndSelection     = XInternAtom(SI_X11_DISPLAY, "XdndSelection",     False);

        /* client messages */
        XdndEnter         = XInternAtom(SI_X11_DISPLAY, "XdndEnter",         False);
        XdndPosition      = XInternAtom(SI_X11_DISPLAY, "XdndPosition",      False);
        XdndStatus        = XInternAtom(SI_X11_DISPLAY, "XdndStatus",        False);
        XdndLeave         = XInternAtom(SI_X11_DISPLAY, "XdndLeave",         False);
        XdndDrop          = XInternAtom(SI_X11_DISPLAY, "XdndDrop",          False);
        XdndFinished      = XInternAtom(SI_X11_DISPLAY, "XdndFinished",      False);

        /* actions */
        XdndActionCopy    = XInternAtom(SI_X11_DISPLAY, "XdndActionCopy",    False);
        const Atom version = 5;

        XChangeProperty(
            win->display, win->hwnd,
            XdndAware, 4, 32,
            PropModeReplace, (u8*)&version, 1
        );

        win->__x11DndHead = win->__x11DndPrev = out;
    }

    siDropEvent* prev = win->__x11DndPrev;
    prev->next = (struct siDropEvent*)out;
    win->__x11DndPrev = out;

    out->rect = rect;
    out->state = 0;
    out->next = nil;
    out->data = nil;
#endif
}
void siapp_windowDragAreaEnd(siDropEvent event) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	RevokeDragDrop(event.subHwnd);
	RemoveWindowSubclass(event.subHwnd, &WndProcDropFile, 0);
	OleUninitialize();
#endif
}


F_TRAITS(inline)
siArea siapp_screenGetCurrentSize(void) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	DEVMODEW mode = {0};
	EnumDisplaySettingsW(nil, ENUM_CURRENT_SETTINGS, &mode);
	return SI_AREA(mode.dmPelsWidth, mode.dmPelsHeight);
#else
    siapp__x11CheckStartup();
    Screen* size = DefaultScreenOfDisplay(SI_X11_DISPLAY);
    return SI_AREA(size->width, size->height);
#endif
}
F_TRAITS(inline)
siArea siapp_screenGetAvailableResolution(usize index) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	DEVMODEW mode = {0};
	i32 res = EnumDisplaySettingsW(nil, index, &mode);
	SI_STOPIF(res == 0, return SI_AREA(-1, -1));

	return SI_AREA(mode.dmPelsWidth, mode.dmPelsHeight);
#endif
}


F_TRAITS(inline)
void siapp_mouseShow(b32 show) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	ShowCursor(show);
#else
    siWindow* win = SI_ROOT_WINDOW;

    if (show) {
        siCursorType cursor = siapp_windowCursorGet(win);
        Cursor cursorVal = (cursor < 0)
            ? -cursor
            : SI_X11_CURSORS[cursor - 1];
        XDefineCursor(win->display, win->hwnd, cursorVal);
    }
    else {
        if (win->__x11BlankCursor == 0) {
            siByte data[] = {0xFF, 0xFF, 0xFF, 0x00};
            win->__x11BlankCursor = -siapp_cursorMake(data, SI_AREA(1, 1), 4);
        }

        XDefineCursor(win->display, win->hwnd, win->__x11BlankCursor);
    }
#endif
}

usize siapp_clipboardTextGet(char* outBuffer, usize capacity) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	b32 res = OpenClipboard(nil);
	SI_STOPIF(!res, return 0);

	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	SI_STOPIF(hData == nil, CloseClipboard(); return 0);

	siAllocator tmp = si_allocatorMakeTmp(outBuffer, capacity);
	usize len;
	si_utf16ToUtf8Str(&tmp, GlobalLock(hData), &len);

	GlobalUnlock(hData);
	CloseClipboard();

	return len;
#elif defined (SIAPP_PLATFORM_API_X11)
    siapp__x11CheckStartup();

    Atom type;
    int di;
    unsigned long size, dul;
    siByte* allocatedStr = nil;

    siWindow* win = SI_ROOT_WINDOW;
    XConvertSelection(
        win->display, CLIPBOARD, UTF8_STRING, XSEL_DATA, win->hwnd, CurrentTime
    );

    XEvent event;
	do {
		XNextEvent(win->display, &event);
	} while (event.type != SelectionNotify || event.xselection.selection != CLIPBOARD);

    XGetWindowProperty(
        SI_X11_DISPLAY, win->hwnd, XSEL_DATA, 0, 0, False, AnyPropertyType,
        &type, &di, &dul, &size, &allocatedStr
    );
    XFree(allocatedStr);

    usize len = si_min(capacity - 1, size);
    XGetWindowProperty(
        SI_X11_DISPLAY, win->hwnd, XSEL_DATA, 0, len, False, AnyPropertyType,
        &type, &di, &dul, &size, &allocatedStr
    );

    memcpy(outBuffer, allocatedStr, len);
    outBuffer[len] = '\0';
    XFree(allocatedStr);

    return len;
#endif
}
b32 siapp_clipboardTextSet(cstring text) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	usize len;
	siAllocator* stack = si_allocatorMakeStack(SI_KILO(8));
	siUtf16String str = si_utf8ToUtf16Str(stack, text, &len);

	b32 res = OpenClipboard(nil);
	SI_STOPIF(!res, return false);
	EmptyClipboard();

	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, len + sizeof(u16));
	SI_STOPIF(hData == nil, CloseClipboard(); return false);

	u16* pData = (u16*)GlobalLock(hData);
	memcpy(pData, str, len + sizeof(u16));
	GlobalUnlock(hData);

	b32 status = SetClipboardData(CF_UNICODETEXT, hData) != nil;

	GlobalFree(hData);
	CloseClipboard();

	return status;
#elif defined (SIAPP_PLATFORM_API_X11)
    siapp__x11CheckStartup();

    siWindow* win = SI_ROOT_WINDOW;
    XSetSelectionOwner((Display*)win->display, CLIPBOARD, (Window)win->hwnd, CurrentTime);

    XConvertSelection((Display*)win->display, CLIPBOARD_MANAGER, SAVE_TARGETS, None, (Window)win->hwnd, CurrentTime);

    for (;;) {
        XEvent event;

        XNextEvent((Display*)win->display, &event);
        if (event.type != SelectionRequest)
			return false;

		const XSelectionRequestEvent* request = &event.xselectionrequest;

		XEvent reply = { SelectionNotify };

		char* selectionString = NULL;
		const Atom formats[] = { UTF8_STRING, XA_STRING };
		const i32 formatCount = sizeof(formats) / sizeof(formats[0]);

		selectionString = (char*)text;

		if (request->target == TARGETS) {
			const Atom targets[] = { TARGETS,
									MULTIPLE,
									UTF8_STRING,
									XA_STRING };

			XChangeProperty((Display*)win->display,
							request->requestor,
							request->property,
							4,
							32,
							PropModeReplace,
							(u8*) targets,
							sizeof(targets) / sizeof(targets[0]));

			reply.xselection.property = request->property;
		}

		if (request->target == MULTIPLE) {

			Atom* targets;

			Atom actualType;
			i32 actualFormat;
			u64 count, bytesAfter;

			XGetWindowProperty((Display*)win->display, request->requestor, request->property, 0, INT64_MAX, False, ATOM_PAIR,  &actualType, &actualFormat, &count, &bytesAfter, (u8**) &targets);

			u64 i;
			for (i = 0;  i < count;  i += 2) {
				i32 j;

				for (j = 0;  j < formatCount;  j++) {
					if (targets[i] == formats[j])
						break;
				}

				if (j < formatCount)
				{
					XChangeProperty((Display*)win->display,
									request->requestor,
									targets[i + 1],
									targets[i],
									8,
									PropModeReplace,
									(u8 *) selectionString,
									si_cstrLen(text));
				}
				else
					targets[i + 1] = None;
			}

			XChangeProperty((Display*)win->display,
							request->requestor,
							request->property,
							ATOM_PAIR,
							32,
							PropModeReplace,
							(u8*) targets,
							count);

			XFree(targets);

			reply.xselection.property = request->property;
		}

		reply.xselection.display = request->display;
		reply.xselection.requestor = request->requestor;
		reply.xselection.selection = request->selection;
		reply.xselection.target = request->target;
		reply.xselection.time = request->time;

		XSendEvent((Display*)win->display, request->requestor, False, 0, &reply);
	}
    return true;
#endif
}
usize siapp_clipboardTextLen(void) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	b32 res = OpenClipboard(nil);
	SI_STOPIF(!res, return 0);

	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	SI_STOPIF(hData == nil, CloseClipboard(); return 0);

	usize len = GlobalSize(GlobalLock(hData));

	GlobalUnlock(hData);
	CloseClipboard();

	return len;
#else
    siapp__x11CheckStartup();

    Atom type;
    int di;
    unsigned long size, dul;
    siByte* allocatedStr = nil;

    siWindow* win = SI_ROOT_WINDOW;
    XConvertSelection(
        win->display, CLIPBOARD, UTF8_STRING, XSEL_DATA, win->hwnd, CurrentTime
    );

    XEvent event;
	do {
		XNextEvent(win->display, &event);
	} while (event.type != SelectionNotify || event.xselection.selection != CLIPBOARD);

    XGetWindowProperty(
        SI_X11_DISPLAY, win->hwnd, XSEL_DATA, 0, 0, False, AnyPropertyType,
        &type, &di, &dul, &size, &allocatedStr
    );
    XFree(allocatedStr);

    return size;
#endif
}


siDropHandle siapp_dropEventHandle(siDropEvent event) {
    SI_ASSERT_MSG(event.state == SI_DRAG_DROP, "This function should only get called after a confirmed successful drop.");

#if defined(SIAPP_PLATFORM_API_WIN32)
	FORMATETC fmte = {CF_HDROP, nil, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	STGMEDIUM stgm;
	IDataObject* pDataObj = event.pDataObj;


	b32 err = pDataObj->lpVtbl->GetData(pDataObj, &fmte, &stgm) == S_OK;
	SI_STOPIF(!err, goto end);

	HDROP hdrop = (HDROP)stgm.hGlobal;

    siDropHandle res;
    res.curIndex = 0;
	res.len = DragQueryFileW(hdrop, 0xFFFFFFFF, NULL, 0);
    res.stgm = stgm;
end:
#elif defined(SIAPP_PLATFORM_API_X11)
    SI_ASSERT_NOT_NULL(event.data);

    char* data = event.data;
    usize len = 0;
    usize index = countof("file://") - 1;
    while (true) {
        if (data[index] == '\0') {
            break;
        }

        if (SI_TO_U16(&data[index]) == SI_TO_U16("\r\n")) {
            len += 1;
            index += countof("file://") - 1 + 2;
            continue;
        }
        index += 1;
    }

    siDropHandle res;
    res.curIndex = 0;
    res.len = len;
    res.__x11Data = event.data;
#endif
    return res;
}
b32 siapp_dropEventPollEntry(siDropHandle* handle, siDropEntry* entry) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	if (handle->curIndex >= handle->len) {
		ReleaseStgMedium(&handle->stgm);
		return false;
	}

	siAllocator out = si_allocatorMakeTmp(entry->path, sizeof(entry->path));
	siAllocator* stack = si_allocatorMakeStack(SI_KILO(8));
	u16* curPtr = (u16*)si_allocatorCurPtr(stack);

	DragQueryFileW(handle->stgm.hGlobal, handle->curIndex, curPtr, 256);
	si_utf16ToUtf8Str(&out, curPtr, &entry->len);
	handle->curIndex += 1;

	return true;
#else
	if (handle->__x11Data[handle->curIndex] == '\0') {
        XFree(handle->__x11Data);
		return false;
	}

    handle->curIndex += countof("file://") - 1;
    char* data = &handle->__x11Data[handle->curIndex];
    usize len = 0;

    while (true) {
        if (SI_TO_U16(&data[len]) == SI_TO_U16("\r\n")) {
            entry->path[len] = '\0';
            break;
        }
        else if (data[len] == '%') { /* NOTE(EimaMei): Detected a '%XX' in the path. */
            char x = si_cstrToU64Ex(&data[len + 1], 2, 16);
            entry->path[len] = x;

            len += 1;
            data += 2;
            handle->curIndex += 2;
            continue;
        }
        entry->path[len] = data[len];
        len += 1;
    }

    entry->len = len;
    handle->curIndex += len + 2;

    return true;
#endif
}
F_TRAITS(inline)
void siapp_dropEventEnd(siDropEvent* event) {
	event->state = 0;
}


F_TRAITS(inline)
siKeyType siapp_osKeyToSili(i32 key) {
#if defined(SIAPP_PLATFORM_API_WIN32)
    if (si_between(key, 'A', 'Z') || si_between(key, '0', '9')) {
        return key;
    }
#elif defined (SIAPP_PLATFORM_API_X11)
    if (si_between(key, '0', '9')) {
        return key;
    }
    if (si_between(key, 'a', 'z')) {
        return key - 32;
    }
#endif

	switch (key) {
#if defined(SIAPP_PLATFORM_API_WIN32)
		case VK_ESCAPE:         return SK_ESC;

		case VK_LCONTROL:       return SK_CTRL_L;
		case VK_LSHIFT:         return SK_SHIFT_L;
		case VK_LMENU:          return SK_ALT_L;
		case VK_LWIN:           return SK_SYSTEM_L;

		case VK_RCONTROL:       return SK_CTRL_R;
		case VK_RSHIFT:         return SK_SHIFT_R;
		case VK_RMENU:          return SK_ALT_R;
		case VK_RWIN:           return SK_SYSTEM_R;

		case VK_CAPITAL:        return SK_CAPS_LOCK;
		case VK_SCROLL:         return SK_SCROLL_LOCK;
		case VK_NUMLOCK:        return SK_NUM_LOCK;

		case VK_PAUSE:          return SK_PAUSE;
		case VK_MENU:           return SK_MENU;
		case VK_OEM_4:          return SK_BRACKET_L;
		case VK_OEM_6:          return SK_BRACKET_R;
		case VK_OEM_1:          return SK_SEMICOLON;
		case VK_OEM_COMMA:      return SK_COMMA;
		case VK_OEM_PERIOD:     return SK_PERIOD;
		case VK_OEM_7:          return SK_QUOTE;
		case VK_OEM_2:          return SK_SLASH;
		case VK_OEM_5:          return SK_BACKSLASH;
		case VK_OEM_3:          return SK_GRAVE;
		case VK_OEM_PLUS:       return SK_EQUALS;
		case VK_OEM_MINUS:      return SK_MINUS;

		case VK_SPACE:          return SK_SPACE;
		case VK_RETURN:         return SK_RETURN;
		case VK_BACK:           return SK_BACKSPACE;
		case VK_TAB:            return SK_TAB;

		case VK_PRIOR:          return SK_PAGE_UP;
		case VK_NEXT:           return SK_PAGE_DOWN;
		case VK_END:            return SK_END;
		case VK_HOME:           return SK_HOME;
		case VK_INSERT:         return SK_INSERT;
		case VK_DELETE:         return SK_DELETE;
		case VK_CLEAR:          return SK_CLEAR;

		case VK_ADD:            return SK_PLUS;
		case VK_SUBTRACT:       return SK_SUBTRACT;
		case VK_MULTIPLY:       return SK_MULTIPLY;
		case VK_DIVIDE:         return SK_DIVIDE;

		case VK_LEFT:           return SK_LEFT;
		case VK_RIGHT:          return SK_RIGHT;
		case VK_UP:             return SK_UP;
		case VK_DOWN:           return SK_DOWN;

		case VK_NUMPAD0:        return SK_NUMPAD_0;
		case VK_NUMPAD1:        return SK_NUMPAD_1;
		case VK_NUMPAD2:        return SK_NUMPAD_2;
		case VK_NUMPAD3:        return SK_NUMPAD_3;
		case VK_NUMPAD4:        return SK_NUMPAD_4;
		case VK_NUMPAD6:        return SK_NUMPAD_6;
		case VK_NUMPAD7:        return SK_NUMPAD_7;
		case VK_NUMPAD8:        return SK_NUMPAD_8;
		case VK_NUMPAD9:        return SK_NUMPAD_9;
		case VK_SEPARATOR:      return SK_NUMPAD_ENTER;
		case VK_DECIMAL:        return SK_NUMPAD_DOT;

		case VK_F1:             return SK_F1;
		case VK_F2:             return SK_F2;
		case VK_F3:             return SK_F3;
		case VK_F4:             return SK_F4;
		case VK_F5:             return SK_F5;
		case VK_F6:             return SK_F6;
		case VK_F7:             return SK_F7;
		case VK_F8:             return SK_F8;
		case VK_F9:             return SK_F9;
		case VK_F10:            return SK_F10;
		case VK_F11:            return SK_F11;
		case VK_F12:            return SK_F12;
		case VK_F13:            return SK_F13;
		case VK_F14:            return SK_F14;
		case VK_F15:            return SK_F15;
#elif defined(SIAPP_PLATFORM_API_X11)
        case XK_Escape:         return SK_ESC;

        case XK_Control_L:      return SK_CTRL_L;
        case XK_Shift_L:        return SK_SHIFT_L;
        case XK_Alt_L:          return SK_ALT_L;
        case XK_Super_L:        return SK_SYSTEM_L;

        case XK_Control_R:      return SK_CTRL_L;
        case XK_Shift_R:        return SK_SHIFT_L;
        case XK_Alt_R:          return SK_ALT_L;
        case XK_Super_R:        return SK_SYSTEM_R;

        case XK_Caps_Lock:      return SK_CAPS_LOCK;
        case XK_Scroll_Lock:    return SK_SCROLL_LOCK;
        case XK_Num_Lock:       return SK_NUM_LOCK;

        case XK_Pause:          return SK_PAUSE;
        case XK_Menu:           return SK_MENU;
        case XK_bracketleft:    return SK_BRACKET_L;
        case XK_bracketright:   return SK_BRACKET_R;
        case XK_semicolon:      return SK_SEMICOLON;
        case XK_comma:          return SK_COMMA;
        case XK_period:         return SK_PERIOD;
        case XK_apostrophe:     return SK_QUOTE;
        case XK_slash:          return SK_SLASH;
        case XK_backslash:      return SK_BACKSLASH;
        case XK_grave:          return SK_GRAVE;
        case XK_equal:          return SK_EQUALS;

        case XK_minus:          return SK_MINUS;

        case XK_space:          return SK_SPACE;
        case XK_Return:         return SK_RETURN;
        case XK_BackSpace:      return SK_BACKSPACE;
        case XK_Tab:            return SK_TAB;

        case XK_Prior:          return SK_PAGE_UP;
        case XK_Next:           return SK_PAGE_DOWN;
        case XK_End:            return SK_END;
        case XK_Home:           return SK_HOME;
        case XK_Insert:         return SK_INSERT;
        case XK_Delete:         return SK_DELETE;
        case XK_Clear:          return SK_CLEAR;

        case XK_KP_Add:         return SK_PLUS;
        case XK_KP_Subtract:    return SK_MINUS;
        case XK_KP_Multiply:    return SK_MULTIPLY;
        case XK_KP_Divide:      return SK_DIVIDE;

        case XK_Left:           return SK_LEFT;
        case XK_Right:          return SK_RIGHT;
        case XK_Up:             return SK_UP;
        case XK_Down:           return SK_DOWN;

        case XK_KP_Insert:
        case XK_KP_0:           return SK_NUMPAD_0;
        case XK_KP_End:
        case XK_KP_1:           return SK_NUMPAD_1;
        case XK_KP_Down:
        case XK_KP_2:           return SK_NUMPAD_2;
        case XK_KP_Page_Down:
        case XK_KP_3:           return SK_NUMPAD_3;
        case XK_KP_Left:
        case XK_KP_4:           return SK_NUMPAD_4;
        case XK_KP_5:           return SK_NUMPAD_5;
        case XK_KP_Right:
        case XK_KP_6:           return SK_NUMPAD_6;
        case XK_KP_Home:
        case XK_KP_7:           return SK_NUMPAD_7;
        case XK_KP_Up:
        case XK_KP_8:           return SK_NUMPAD_8;
        case XK_KP_Page_Up:
        case XK_KP_9:           return SK_NUMPAD_9;
        case XK_KP_Enter:       return SK_NUMPAD_ENTER;
        case XK_KP_Delete:
        case XK_KP_Decimal:     return SK_NUMPAD_DOT;


        case XK_F1:             return SK_F1;
        case XK_F2:             return SK_F2;
        case XK_F3:             return SK_F3;
        case XK_F4:             return SK_F4;
        case XK_F5:             return SK_F5;
        case XK_F6:             return SK_F6;
        case XK_F7:             return SK_F7;
        case XK_F8:             return SK_F8;
        case XK_F9:             return SK_F9;
        case XK_F10:            return SK_F10;
        case XK_F11:            return SK_F11;
        case XK_F12:            return SK_F12;
        case XK_F13:            return SK_F13;
        case XK_F14:            return SK_F14;
        case XK_F15:            return SK_F15;
#endif
	}

	return SK_UNKNOWN;
}

F_TRAITS(inline)
cstring siapp_osErrToStr(i32 error) {
	static char buf[128];

#if defined(SIAPP_PLATFORM_API_WIN32)
	DWORD len = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nil, error, 0, buf, sizeof(buf), nil
	);

	if (len == 0) {
		siDllHandle handle = si_dllLoad("Ntdsbmsg.dll");

		len = FormatMessageA(
			FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
			handle, error, 0, buf, sizeof(buf), nil
		);

		si_dllUnload(handle);
	}
	buf[len] = '\0';
#endif

	return buf;
}


siSiliStr siapp_usernameGet(void) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	static char buffer[sizeof(usize) + SI_MAX_PATH_LEN + 1];
	siAllocator tmp = si_allocatorMakeTmp(buffer, sizeof(buffer));

	usize* len = si_mallocItem(&tmp, usize);
	u16 resultWide[SI_MAX_PATH_LEN + 1];

	{
		DWORD wideLen = sizeof(buffer);
		b32 succeed = GetUserNameW(resultWide, &wideLen);
		SI_STOPIF(!succeed, return nil);
	}
	siSiliStr str = (siSiliStr)si_utf16ToUtf8Str(&tmp, resultWide, len);

	return str;
#endif
}


siSearchResult siapp_fileManagerOpen(siAllocator* alloc, siSearchConfig config) {
#if defined(SIAPP_PLATFORM_API_WIN32)
	siAllocator* stack = si_allocatorMake(SI_KILO(4));
	IFileOpenDialog* pfd;
	IShellItemArray* items;


	CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_ALL, &IID_IFileOpenDialog, (rawptr*)&pfd);

	{
		if (config.title != nil) {
			siUtf16String utf16 = si_utf8ToUtf16Str(stack, config.title, nil);
			pfd->lpVtbl->SetTitle(pfd, utf16);
			si_allocatorReset(stack);
		}
	}
	{
		FILEOPENDIALOGOPTIONS fos = FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST;
		switch (config.options) {
			case SI_SEARCH_ALLOW_MULTIPLE: {
				fos |= FOS_ALLOWMULTISELECT;
				break;
			}
			case SI_SEARCH_FOLDERS_ONLY: {
				fos |= FOS_PICKFOLDERS;
				break;
			}
			case SI_SEARCH_FOLDERS_ONLY | SI_SEARCH_ALLOW_MULTIPLE: {
				fos |= FOS_PICKFOLDERS | FOS_ALLOWMULTISELECT;
				break;
			}
		}
		pfd->lpVtbl->SetOptions(pfd, fos);
	}
	{
		if (config.defaultPath != nil) {
			siUtf16String utf16 = si_utf8ToUtf16Str(stack, config.defaultPath, nil);
			IShellItem* folder;
			HRESULT res = SHCreateItemFromParsingName(
				utf16, nil, &IID_IShellItem, (rawptr*)&folder
			);

			if (res == SI_OKAY) {
				pfd->lpVtbl->SetFolder(pfd, folder);
				folder->lpVtbl->Release(folder);
			}
			si_allocatorReset(stack);
		}
	}
	{
		if (config.filetypes != nil) {
			COMDLG_FILTERSPEC* tmp = si_mallocArray(stack, COMDLG_FILTERSPEC, config.filetypesLen);

			for_range (i, 0, config.filetypesLen) {
				siSearchFilterSpec spec = config.filetypes[i];

				tmp[i].pszName = si_utf8ToUtf16Str(stack, spec.name, nil);
				tmp[i].pszSpec = si_utf8ToUtf16Str(stack, spec.filetype, nil);
			}

			pfd->lpVtbl->SetFileTypes(pfd, config.filetypesLen, tmp);
			si_allocatorReset(stack);
		}
	}

	pfd->lpVtbl->Show(pfd, nil);
	{
		HRESULT err = pfd->lpVtbl->GetResults(pfd, &items);
		SI_STOPIF(err != 0, return (siSearchResult){nil, 0});
	}

	DWORD len;
	items->lpVtbl->GetCount(items, &len);

	siSearchResult res;
	res.len = len;
	res.items = si_mallocArray(alloc, siSiliStr, len);

	stack->offset = 0;
	for_range (i, 0, len) {
		IShellItem* item;
		LPWSTR widePath = nil;

		items->lpVtbl->GetItemAt(items, i, &item);
		item->lpVtbl->GetDisplayName(item, SIGDN_FILESYSPATH, &widePath);

		usize* strLen = si_mallocItem(alloc, usize);
		res.items[i] = (siSiliStr)si_utf16ToUtf8Str(alloc, widePath, strLen);
	}

	items->lpVtbl->Release(items);
	pfd->lpVtbl->Release(pfd);

	return res;
#endif
}


F_TRAITS(inline)
siSiliStr siapp_appDataPathMake(cstring folderName) {
	return siapp_appDataPathMakeEx(folderName, si_cstrLen(folderName));
}
siSiliStr siapp_appDataPathMakeEx(cstring folderName, usize folderNameLen) {
	static char buffer[sizeof(usize) + SI_MAX_PATH_LEN + 1];
	siAllocator tmp = si_allocatorMakeTmp(buffer, sizeof(buffer));

	siSiliStr str;
	usize* len = si_mallocItem(&tmp, usize);
	u16 resultWide[SI_MAX_PATH_LEN + 1];

    {
#if defined(SIAPP_PLATFORM_API_WIN32)
		i32 res = SHGetFolderPathW(nil, CSIDL_LOCAL_APPDATA, nil, 0, resultWide);
		SI_STOPIF(res != S_OK, return nil);
		str = (siSiliStr)si_utf16ToUtf8Str(&tmp, resultWide, len);
#endif
    }
	usize curLen = *len;

	str[curLen] = SI_PATH_SEPARATOR;
	memcpy(&str[curLen + 1], folderName, folderNameLen);
	str[curLen + 1 + folderNameLen] = SI_PATH_SEPARATOR;

	*len += 1 + folderNameLen + 1;

	return str;
}


siTextureAtlas* siapp_textureAtlasMake(const siWindow* win, siArea area, u32 maxTexCount,
		siTextureResizeEnum enumName) {
	SI_ASSERT_NOT_NULL(win);

	siTextureAtlas* atlas = si_mallocItem(win->alloc, siTextureAtlas);
	atlas->renderingType = win->arg & SI_WINDOW_RENDERING_BITS;
	atlas->texWidth = area.width;
	atlas->texHeight = area.height;
    atlas->curCount = 0;
    atlas->curWidth = 0;
	atlas->totalWidth = atlas->texWidth * maxTexCount;

	switch (atlas->renderingType) {
		case SI_WINDOW_RENDERING_OPENGL: {
			glGenTextures(1, &atlas->texID);
			u32 index = atlas->texID - 1;

			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, atlas->texID);

			glTexStorage2D(
				GL_TEXTURE_2D,
				1,
				GL_RGBA8,
				atlas->totalWidth, atlas->texHeight
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, enumName);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, enumName);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glUniform1i(win->render.opengl.uniformTexture + index, index);
		}
	}

	return atlas;
}
void siapp_textureAtlasFree(siTextureAtlas* atlas) {
	SI_ASSERT_NOT_NULL(atlas);

	glDeleteTextures(1, &atlas->texID);
	*atlas = (siTextureAtlas){0};
}
void siapp_textureSwizzleMask(const siTextureAtlas* atlas, siSwizzleEnum param,
		const siSwizzleValue mask[4]) {
	SI_ASSERT_NOT_NULL(atlas);
	SI_ASSERT_NOT_NULL(mask);

	switch (atlas->renderingType) {
		case SI_WINDOW_RENDERING_OPENGL: {
			glActiveTexture(GL_TEXTURE0 + atlas->texID - 1);
			glBindTexture(GL_TEXTURE_2D, atlas->texID);
			glTexParameteriv(GL_TEXTURE_2D, param, mask);
		}
	}
}
void siapp_textureResizeMethodChange(siTextureAtlas* atlas, siTextureResizeEnum
		resizeMethod) {
	SI_ASSERT_NOT_NULL(atlas);

	u32 index = atlas->texID - 1;

	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, atlas->texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, resizeMethod);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, resizeMethod);
}


F_TRAITS(inline)
siImage siapp_imageLoad(siTextureAtlas* atlas, cstring filename) {
	SI_ASSERT_NOT_NULL(atlas);
	SI_ASSERT_NOT_NULL(filename);

	siFile file = si_fileOpen(filename);

	siAllocator* tmp = si_allocatorMake(file.size);
	siByte* content = si_fileReadContents(tmp, file);

	i32 width, height, channels;
	siByte* buffer = SIAPP_IMAGE_LOAD(content, file.size, &width, &height, &channels);
	SI_ASSERT_NOT_NULL(buffer);

	siImage res = siapp_imageLoadEx(atlas, buffer, width, height, channels);
	SIAPP_IMAGE_FREE(buffer);

	si_fileClose(file);
	si_allocatorFree(tmp);
	return res;
}


siImage siapp_imageLoadEx(siTextureAtlas* atlas, const siByte* buffer, u32 width,
		u32 height, u32 channels) {
	SI_ASSERT_NOT_NULL(atlas);
	SI_ASSERT_FMT(
		width + atlas->curWidth <= atlas->totalWidth && height <= atlas->texHeight,
		"The image's size dimensions be less or equal to the texture atlas'. (%i > %i)",
        width + atlas->curWidth, atlas->totalWidth
	);
	siImage res;
	res.size = SI_AREA(width, height);

	switch (atlas->renderingType) {
		case SI_WINDOW_RENDERING_OPENGL: {
			u32 c = 0;
			switch (channels) {
				case 1: c = GL_ALPHA; break;
				case 2: c = GL_RG; break;
				case 3: c = GL_RGB; break;
				case 4: c = GL_RGBA; break;
			}

			res.atlas = atlas;
            res.id = atlas->curCount;

			glActiveTexture(GL_TEXTURE0 + atlas->texID - 1);
			glBindTexture(GL_TEXTURE_2D, atlas->texID);

			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				atlas->curWidth, 0,
				width, height, c,
				GL_UNSIGNED_BYTE,
				buffer
			);

			res.x1 = (f32)atlas->curWidth / atlas->totalWidth;
			res.x2 = res.x1 + (f32)width / atlas->totalWidth;
			res.y1 = 0;
			res.y2 = (f32)height / atlas->texHeight;

            atlas->curWidth += width;
            atlas->curCount += 1;
			break;
		}
	}

	return res;
}

F_TRAITS(inline)
siSpriteSheet siapp_spriteSheetLoad(siTextureAtlas* atlas, cstring filename,
		siArea spriteSize) {
	siSpriteSheet res;
	res.base = siapp_imageLoad(atlas, filename);

	SI_ASSERT_MSG(
		spriteSize.width <= res.base.size.width
		&& spriteSize.height <= res.base.size.height,
		"Sprite's size dimensions must be less or equal to the spritesheet's."
	);

	res.spriteSize = spriteSize;
	res.widthRatio = res.base.size.width / spriteSize.width;
	res.frames = res.widthRatio * (res.base.size.width / spriteSize.height);
	res.spriteX2 = (f32)spriteSize.width / atlas->totalWidth;
	res.spriteY2 = (f32)spriteSize.height / atlas->texHeight;

	return res;
}
F_TRAITS(inline)
siSpriteSheet siapp_spriteSheetLoadEx(siTextureAtlas* atlas, const siByte* data,
		i32 width, i32 height, i32 channels, siArea spriteSize) {
	SI_ASSERT_MSG(
		spriteSize.width <= width
		&& spriteSize.height <= height,
		"Sprite's size dimensions must be less or equal to the spritesheet's."
	);

	siSpriteSheet res;
	res.base = siapp_imageLoadEx(atlas, data, width, height, channels);
	res.spriteSize = spriteSize;
	res.widthRatio = width / spriteSize.width;
	res.frames = res.widthRatio * (width / spriteSize.height);
	res.spriteX2 = (f32)spriteSize.width / atlas->totalWidth;
	res.spriteY2 = (f32)spriteSize.height / atlas->texHeight;

	return res;
}

siImage siapp_spriteSheetSpriteGet(siSpriteSheet sheet, usize index) {
	SI_ASSERT_FMT(index < sheet.frames, "Index '%i' is not a valid frame index.", index);

	siPoint XY = SI_POINT(
		index % sheet.widthRatio * sheet.spriteSize.width,
		index / sheet.widthRatio * sheet.spriteSize.height
	);

	siImage copy = sheet.base;
	copy.size = sheet.spriteSize;

	copy.x1 = sheet.base.x1 + (f32)XY.x / sheet.base.atlas->totalWidth;
	copy.x2 = copy.x1 + sheet.spriteX2;
	copy.y1 = sheet.base.y1 + (f32)XY.y / sheet.base.atlas->texHeight;
	copy.y2 = copy.y1 + sheet.spriteY2;

	return copy;
}

void siapp_spriteSheetSpriteSetEx(siSpriteSheet sheet, usize index, const siByte* data,
		i32 channels) {
	SI_ASSERT_FMT(index < sheet.frames, "Index '%i' is not a valid frame index.", index);

	siPoint XY = SI_POINT(
		index % sheet.widthRatio * sheet.spriteSize.width,
		index / sheet.widthRatio * sheet.spriteSize.height
	);

	u32 c;
	switch (channels) {
		case 1: c = GL_RED; break;
		case 2: c = GL_RG; break;
		case 3: c = GL_RGB; break;
		case 4: c = GL_RGBA; break;
		default: c = 0;
	}

	siImage img = sheet.base;
	siTextureAtlas* atlas = img.atlas;
	usize xOffset = img.id * atlas->texWidth;

	switch (atlas->renderingType) {
		case SI_WINDOW_RENDERING_OPENGL: {
			glActiveTexture(GL_TEXTURE0 + atlas->texID - 1);
			glBindTexture(GL_TEXTURE_2D, atlas->texID);
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				xOffset + XY.x, XY.y,
				sheet.spriteSize.width, sheet.spriteSize.height, c,
				GL_UNSIGNED_BYTE,
				data
			);
		}
	}
}

F_TRAITS(inline)
siFont siapp_fontLoad(siWindow* win, cstring path, i32 size) {
	return siapp_fontLoadEx(win, path, size, si_buf(siGlyphSet, SI_CHARSET_WGL4, SI_GLYPHSET_END), 0);
}
F_TRAITS(inline)
siFont siapp_fontLoadExtra(siWindow* win, cstring path, i32 size, usize extraChars) {
	return siapp_fontLoadEx(win, path, size, si_buf(siGlyphSet, SI_CHARSET_WGL4, SI_GLYPHSET_END), extraChars);
}

siFont siapp_fontLoadEx(siWindow* win, cstring path, i32 size, siGlyphSet* glyphs,
        u32 extraChars) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT_NOT_NULL(glyphs);

	siAllocator* tmpAlloc;
	siFont font;
	font.size = size;

	{
		siFile file = si_fileOpen(path);
        tmpAlloc = si_allocatorMake(file.size + size * size);

        siByte* content = si_fileReadContents(tmpAlloc, file);
		stbtt_InitFont(&font.stbtt, content, 0);

        si_fileClose(file);
	}

	u32 charCount = extraChars;
    u32 arrayLen = 0;
    while (true) {
		siGlyphSet set = glyphs[arrayLen];
        SI_STOPIF(set.end == '?' && set.start == '?', break);

        charCount += set.end - set.start + 1;
        arrayLen += 1;
	}
    arrayLen += 1;

	{
		isize texSize = 128;
		while (true) {
			isize res = (charCount / (texSize / (f32)size)) * size;
			SI_STOPIF(res <= texSize, break);
			texSize *= 2;
		}

		// TODO(EimaMei): Merge.
		siTextureAtlas* atlas = siapp_textureAtlasMake(win, SI_AREA(texSize, texSize), 1, SI_RESIZE_DEFAULT);
		font.sheet = siapp_spriteSheetLoadEx(atlas, nil, texSize, texSize, 4, SI_AREA(size, size));

		siapp_textureSwizzleMask(
			atlas, SI_SWIZZLE_RGBA,
			si_buf(i32, SI_SWIZZLE_VAL_1, SI_SWIZZLE_VAL_1, SI_SWIZZLE_VAL_1, SI_SWIZZLE_VAL_R)
		);
	}
    font.sets = si_arrayMakeReserve(win->alloc, sizeof(siGlyphSetANDNIndex), arrayLen);
    font.glyphs = si_arrayMakeReserve(win->alloc, sizeof(siGlyphInfo), charCount);
    SI_ARRAY_HEADER(font.glyphs)->len = charCount;
    SI_ARRAY_HEADER(font.sets)->len = arrayLen;

	siByte* tmpBuf = si_mallocArray(tmpAlloc, siByte, size * size);
	font.scale = stbtt_ScaleForPixelHeight(&font.stbtt, size);
	{
		font.advance.space = font.size / 3.45f;
		font.advance.tab = font.size / 1.15f;
		font.advance.newline = (font.size + font.size * 0.25f) / 1.15f;
	}

	usize indexCount = 0;
	for_range (i, 0, arrayLen) {
		siGlyphSet set = glyphs[i];
		{
			siGlyphSetANDNIndex* info = &font.sets[i];
			info->set = set;
			info->index = indexCount;
		}

		for_range (codepoint, set.start, set.end + 1) {
			siGlyphInfo* glyph = &font.glyphs[indexCount];
			i32 glyphIndex = stbtt_FindGlyphIndex(&font.stbtt, codepoint);
            SI_STOPIF(glyphIndex == 0, continue);

			glyph->codepoint = codepoint;
			glyph->glyphIndex = glyphIndex;
			glyph->texID = indexCount;

			i32 advanceX;
			stbtt_GetGlyphHMetrics(&font.stbtt, glyphIndex, &advanceX, nil);
			glyph->advanceX = advanceX * font.scale;

			i32 x0, y0, x1, y1;
			b32 res = stbtt_GetGlyphBox(&font.stbtt, glyphIndex, &x0, &y0, &x1, &y1);

			if (!res) {
				glyph->x = glyph->y = glyph->width = glyph->height = 0;
				indexCount += 1;
				continue;
			}
			else {
				glyph->x = si_floor(x0 * font.scale);
				glyph->y = si_floor(-y1 * font.scale);
				glyph->width = si_ceil(x1 * font.scale) - glyph->x;
				glyph->height = si_ceil(-y0 * font.scale) - glyph->y;
			}

			stbtt_MakeGlyphBitmap(
				&font.stbtt, tmpBuf,
				size, size, size,
				font.scale, font.scale, glyphIndex
			);
			siapp_spriteSheetSpriteSetEx(font.sheet, indexCount, tmpBuf, 1);

			indexCount += 1;
		}
	}
	font.unknownSymbolIndex = siapp_fontGlyphFind(&font, '?') - font.glyphs;
    if (extraChars == 0 ) {
        si_arrayShrinkToFit(&font.glyphs);
    }

	si_allocatorFree(tmpAlloc);

	return font;
}
void siapp_fontFree(siFont font) {
    siapp_textureAtlasFree(font.sheet.base.atlas);
}

F_TRAITS(inline)
f32 siapp_fontCalculateScaleFactor(siFont font, u32 textSize) {
	return (f32)textSize / font.size * 1.15f;
}
siGlyphInfo* siapp_fontGlyphFind(siFont* font, i32 codepoint) {
	SI_ASSERT(codepoint != SI_UNICODE_INVALID);

	siGlyphSetANDNIndex* sets = font->sets;
	for_range (i, 0, si_arrayLen(sets)) {
		b32 res = si_betweenu((u32)codepoint, sets[i].set.start, sets[i].set.end);
		SI_STOPIF(res == 0, continue);
		usize realIndex = (codepoint - sets[i].set.start) + sets[i].index;
		return &font->glyphs[realIndex];
	}

	return nil;
}

F_TRAITS(inline)
siText siapp_textLoad(siAllocator* alloc, siFont* font, cstring text) {
	return siapp_textLoadLen(alloc, font, text, si_utf8StrLen(text));
}

siText siapp_textReserve(siAllocator* alloc, siFont* font, usize characterCount) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(font);

	siText text;
	text.font = font;
	text.chars = si_calloc(alloc, sizeof(i32), characterCount);
	text.len = 0;
	text.capacity = characterCount;
	text.totalArea = SI_VEC2(0, font->size);
	text.curX = 0;

	return text;
}

F_TRAITS(inline)
siArea siapp_textGetArea(siText text, u32 size) {
	f32 scaleFactor = (f32)size / text.font->size * 1.15;
	return SI_AREA(text.totalArea.x * scaleFactor, text.totalArea.y * scaleFactor);
}

F_TRAITS(inline)
siVec2 siapp_textGetAreaF(siText text, u32 size) {
	f32 scaleFactor = (f32)size / text.font->size * 1.15;
	return SI_VEC2(text.totalArea.x * scaleFactor, text.totalArea.y * scaleFactor);
}

siText siapp_textLoadLen(siAllocator* alloc, siFont* font, cstring text, usize characterCount) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(font);
	SI_ASSERT_NOT_NULL(text);

	i32* chars = si_mallocArray(alloc, i32, characterCount);
	siVec2 area = SI_VEC2(0, font->size);

	siText txt;
	txt.chars = chars;
	txt.len = characterCount;
	txt.capacity = characterCount;
	txt.font = font;

	cstring strPtr = text;
	for_range (i, 0, characterCount) {
		siUtf32Char utf32 = si_utf8Decode(strPtr);

		switch (utf32.codepoint) {
			case SI_UNICODE_INVALID: SI_PANIC_MSG("Invalid UTF-8 character."); break;
			case '\0': i = characterCount; continue;

			case ' ':
			case '\t':
			case '\r':
			case '\n': {
				chars[i] = -utf32.codepoint;
				area.x += siapp_textAdvanceXGet(txt, i);
				strPtr += 1;
				continue;
			}
			case '%': {
				SI_STOPIF(i == characterCount - 1 , break);
				switch (strPtr[1]) {
					case '_':
					case '*': {
						chars[i] = -strPtr[1];
						strPtr += 2;
						continue;
					}
				}
				break;
			}
			default: break;
		}

		siGlyphInfo* glyph = siapp_fontGlyphFind(font, utf32.codepoint);

		chars[i] = (glyph != nil)
			? glyph - font->glyphs
			: -utf32.codepoint;

		area.x += siapp_textAdvanceXGet(txt, i);
		strPtr += utf32.len;
	}

	txt.totalArea = area;

	return txt;
}

siGlyphInfo* siapp_textCharacterGet(siText text, usize index) {
	SI_ASSERT(index < text.capacity);
	i32 realIndex = text.chars[index];

	if (realIndex > 0) {
		return &text.font->glyphs[realIndex];
	}
	else if (si_charIsSpace(-realIndex)) {
		return nil;
	}
	else {
		return &text.font->glyphs[text.font->unknownSymbolIndex];
	}
}
void siapp_textCharacterSet(siText* text, usize index, i32 codepoint) {
	SI_ASSERT_FMT(
		si_betweenu(index, 0, text->capacity),
		"The specified index (%zu) is not in between '0' and '%z'.",
		index, text->capacity
	);

	siFont* font = text->font;
	i32* chars = text->chars;
	i32 prev = chars[index];

	switch (codepoint) {
		case '\0': return ;

		case ' ':
		case '\t': {
			chars[index] = -codepoint;
			f32 advanceX = siapp_textAdvanceXGet(*text, index);
			text->curX += advanceX;
			text->totalArea.x = si_maxf(text->curX, text->totalArea.x);
			goto end;
		}

		case '\r':
		case '\n': {
			chars[index] = -codepoint;
			text->prevLargest = si_maxf(text->prevLargest, text->curX);
			text->curX = 0;
			text->totalArea.y += font->advance.newline;
			goto end;
		}

		case '\b': {
			f32 prevValue = siapp_textAdvanceXGet(*text, index);
			b32 isNewline = -chars[index];
			isNewline = (isNewline == '\n' || isNewline == '\r');

			chars[index] = 0;

			if (!isNewline) {
				text->curX -= prevValue;
				if (si_betweenf(text->curX, text->prevLargest, text->totalArea.x)) {
					text->totalArea.x = text->curX;
				}
			}
			else {
				text->curX = 0;
				text->totalArea.y -= font->advance.newline;
				isize i = index - 1;
				while (i != -1) {
					i32 newIndex = text->chars[i];
					text->curX += siapp_textAdvanceXGet(*text, newIndex);
					b32 newlineFound = (-newIndex == '\n' || -newIndex == '\r');
					i -= 1;
					SI_STOPIF(newlineFound, break);
				}

				text->totalArea.x = si_maxf(text->curX, text->totalArea.x);

				f32 largest = 0;
				while (i != -1) { // TODO optimize this later
					f32 possible = 0;
					while (true) {
						possible += siapp_textAdvanceXGet(*text, i);
						i32 codepoint = siapp_textCodepointGet(*text, i);
						b32 newlineFound = (codepoint == '\n' || codepoint == '\r');
						i -= 1;
						SI_STOPIF(newlineFound || i == -1, break);
					}
					largest = si_maxf(possible, largest);
				}
				text->prevLargest = largest;
			}
			text->len -= 1;
			goto end;
		}
	}


	siGlyphInfo* glyph = siapp_fontGlyphFind(font, codepoint);
	f32 advanceX;

	if (glyph != nil) {
		chars[index] = glyph - font->glyphs;
		advanceX = glyph->advanceX;
	}
	else {
		chars[index] = -codepoint;
		advanceX = font->glyphs[font->unknownSymbolIndex].advanceX;
	}

	text->curX += advanceX;
	text->totalArea.x = si_maxf(text->curX, text->totalArea.x);

end:
	text->len += (prev == 0);
}
F_TRAITS(inline)
i32 siapp_textCodepointGet(siText text, usize index) {
	siFont* font = text.font;
	SI_ASSERT(index < si_arrayLen(font));

	i32 realIndex = text.chars[index];
	return (realIndex > 0)
			? font->glyphs[realIndex].codepoint
			: -realIndex;
}
F_TRAITS(inline)
f32 siapp_textAdvanceXGet(siText text, usize index) {
	SI_ASSERT(index < text.capacity);
	siFont* font = text.font;
	i32 realIndex = text.chars[index];

	if (realIndex < 0) {
		switch (-realIndex) {
			case ' ': return font->advance.space;
			case '\t': return font->advance.tab;
			case '\r':
			case '\n': return 0;  // NOTE(EimaMei): 0 / (1.0f * 1.15f)
			default: {
				f32 advanceX = font->glyphs[font->unknownSymbolIndex].advanceX;
				return advanceX;
			}
		}
	}
	return font->glyphs[realIndex].advanceX;
}


F_TRAITS(inline)
void siapp_windowBackgroundSet(siWindow* win, siColor color) {
	SI_ASSERT_NOT_NULL(win);

	switch (win->arg & SI_WINDOW_RENDERING_BITS) {
		case SI_WINDOW_RENDERING_OPENGL: {
			siVec4* bg = &win->render.opengl.bgColor;
			bg->x = color.r / 255.0f;
			bg->y = color.g / 255.0f;
			bg->z = color.b / 255.0f;
			bg->w = color.a / 255.0f;
			siapp_windowClear(win);
			break;
		}
        default: {
            #if defined(SIAPP_PLATFORM_API_X11)
                u32 clr = ((u32)color.r << 16) | ((u32)color.g << 8) | color.b;
                XSetWindowBackground(win->display, win->hwnd, clr);
                XFlush(win->display);
            #endif
        }
	}
}

F_TRAITS(inline)
siKeyState siapp_windowKeyGet(const siWindow* win, siKeyType key) {
	SI_ASSERT_NOT_NULL(win);
	return win->e.keys[key];
}
F_TRAITS(inline)
b32 siapp_windowKeyClicked(const siWindow* win, siKeyType key) {
	SI_ASSERT_NOT_NULL(win);
	return win->e.keys[key].clicked;
}
F_TRAITS(inline)
b32 siapp_windowKeyPressed(const siWindow* win, siKeyType key) {
	SI_ASSERT_NOT_NULL(win);
	return win->e.keys[key].pressed;
}
F_TRAITS(inline)
b32 siapp_windowKeyReleased(const siWindow* win, siKeyType key) {
	SI_ASSERT_NOT_NULL(win);
	return win->e.keys[key].released;
}

F_TRAITS(inline)
siKeyState siapp_windowMouseKeyGet(const siWindow* win, siMouseButtonType key)  {
	SI_ASSERT_NOT_NULL(win);
	return win->e.mouseButtons[key - 1];
}
F_TRAITS(inline)
b32 siapp_windowMouseKeyClicked(const siWindow* win, siMouseButtonType key) {
	SI_ASSERT_NOT_NULL(win);
	return win->e.mouseButtons[key - 1].clicked;
}
F_TRAITS(inline)
b32 siapp_windowMouseKeyPressed(const siWindow* win, siMouseButtonType key) {
	SI_ASSERT_NOT_NULL(win);
	return win->e.mouseButtons[key - 1].pressed;
}
F_TRAITS(inline)
b32 siapp_windowMouseKeyReleased(const siWindow* win, siMouseButtonType key) {
	SI_ASSERT_NOT_NULL(win);
	return win->e.mouseButtons[key - 1].released;
}

F_TRAITS(inline)
void siapp_drawVertex2f(siWindow* win, f32 x, f32 y) {
	siapp_drawVertex3f(win, x, y, 0.0f);
}
F_TRAITS(inline)
void siapp_drawVertex3f(siWindow* win, f32 x, f32 y, f32 z) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
	SI_ASSERT_MSG(gl->vertexCounter < gl->maxVertexCount, "Not enough space is left inside the buffer.");

	gl->vertices[gl->vertexCounter] = SI_VEC3(x, y, z);
    gl->texCoords[gl->vertexCounter] = gl->curTexCoords;
	gl->colors[gl->vertexCounter] = gl->curColor;

	gl->vertexCounter += 1;
}
void siapp_drawRect(siWindow* win, siRect rect, siColor color) {
	siapp_drawRectF(win, SI_VEC4R(rect), color);
}
void siapp_drawRectF(siWindow* win, siVec4 rect, siColor color) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    SI_STOPIF(gl->vertexCounter + 4 > gl->maxVertexCount, siapp_windowRender(win));

	f32 x1 = i32ToNDCX(rect.x, gl->size.width); // TODO(EimaMei): Optimize i32ToNDCX. SIMD MAYBE?
	f32 y1 = i32ToNDCY(rect.y, gl->size.height);
	f32 x2 = i32ToNDCX(rect.x + rect.z, gl->size.width);
	f32 y2 = i32ToNDCY(rect.y + rect.w, gl->size.height);

	siapp_color4f(win, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

	siapp_drawVertex2f(win, x1, y1);
	siapp_drawVertex2f(win, x2, y1);
	siapp_drawVertex2f(win, x2, y2);
	siapp_drawVertex2f(win, x1, y2);

	// NOTE(EimaMei): Check if this can be optimized with instances.
	siapp__addVertexesToCMD(gl, 6, 4);
}
F_TRAITS(inline)
void siapp_drawImage(siWindow* win, siRect rect, siImage img) {
	siapp_drawImageF(win, SI_VEC4R(rect), img);
}
void siapp_drawImageF(siWindow* win, siVec4 rect, siImage img) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    SI_STOPIF(gl->vertexCounter + 4 > gl->maxVertexCount, siapp_windowRender(win));

	f32 x1 = i32ToNDCX(rect.x, gl->size.width);
	f32 y1 = i32ToNDCY(rect.y, gl->size.height);
	f32 x2 = i32ToNDCX(rect.x + rect.z, gl->size.width);
	f32 y2 = i32ToNDCY(rect.y + rect.w, gl->size.height);

	siapp_colorVec4f(win, win->imageColor);
	gl->curTex = &img;

	siapp_texCoords2f(win, img.x1, img.y1); siapp_drawVertex2f(win, x1, y1);
	siapp_texCoords2f(win, img.x2, img.y1); siapp_drawVertex2f(win, x2, y1);
	siapp_texCoords2f(win, img.x2, img.y2); siapp_drawVertex2f(win, x2, y2);
	siapp_texCoords2f(win, img.x1, img.y2); siapp_drawVertex2f(win, x1, y2);
	siapp_texCoords2f(win, 0, 0);

	gl->curTex = &gl->defaultTex;

	siapp__addVertexesToCMD(gl, 6, 4);
}
F_TRAITS(inline)
void siapp_drawTriangle(siWindow* win, siTriangle triangle, siColor color) {
	siapp_drawTriangleF(win, SI_TRIANGLE_INT_TO_FLOAT(triangle), color);
}
void siapp_drawTriangleF(siWindow* win, siTriangleF triangle, siColor color) {
	SI_ASSERT_NOT_NULL(win);

	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    SI_STOPIF(gl->vertexCounter + 3 > gl->maxVertexCount, siapp_windowRender(win));

	f32 x1 = i32ToNDCX(triangle.p1.x, gl->size.width),
		y1 = i32ToNDCY(triangle.p1.y, gl->size.height);
	f32 x2 = i32ToNDCX(triangle.p2.x, gl->size.width),
		y2 = i32ToNDCY(triangle.p2.y, gl->size.height);
	f32 x3 = i32ToNDCX(triangle.p3.x, gl->size.width),
		y3 = i32ToNDCY(triangle.p3.y, gl->size.height);

	siapp_color4f(win, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	siapp_drawVertex2f(win, x1, y1);
	siapp_drawVertex2f(win, x2, y2);
	siapp_drawVertex2f(win, x3, y3);

	siapp__addVertexesToCMD(gl, 3, 3);
}
void siapp_drawTriangleRight(siWindow* win, siPoint start, f32 hypotenuse,
		f32 startingAngle, siColor color) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT(hypotenuse > 0);

	f32 x1 = hypotenuse * si_cos(SI_TO_RADIANS(startingAngle)),
		y1 = hypotenuse * si_sin(SI_TO_RADIANS(startingAngle));

	siTriangleF t = SI_TRIANGLE_F(
		SI_VEC2(start.x, start.y),
		SI_VEC2(start.x + x1, start.y + y1),
		SI_VEC2(start.x, start.y + y1)
	);

	siapp_drawTriangleF(win, t, color);
}
void siapp_drawTriangleIsosceles(siWindow* win, siPoint start, f32 length,
		f32 startingAngle, siColor color) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT(length > 0);
	SI_ASSERT(startingAngle < 180);

	f32 angleAlpha = (180 - startingAngle) / 2;
	f32 x = length * si_cos(SI_TO_RADIANS(angleAlpha)),
		y = length * si_sin(SI_TO_RADIANS(angleAlpha));

	siTriangleF t = SI_TRIANGLE_F(
		SI_VEC2(start.x + x, start.y),
		SI_VEC2(start.x + x * 2, start.y + y),
		SI_VEC2(start.x, start.y + y)
	);

	siapp_drawTriangleF(win, t, color);
}
F_TRAITS(inline)
void siapp_drawCircle(siWindow* win, siPoint pos, i32 radius, siColor color) {
	siapp_drawCircleEx(win, pos, radius, 90, color);
}
F_TRAITS(inline)
void siapp_drawCircleF(siWindow* win, siVec2 pos, i32 radius, siColor color) {
	siapp_drawCircleExF(win, pos, radius, 90, color);
}
F_TRAITS(inline)
void siapp_drawCircleEx(siWindow* win, siPoint pos, i32 radius, u32 sides, siColor color) {
	siapp_drawCircleExF(win, SI_VEC2(pos.x, pos.y), radius, sides, color);
}
F_TRAITS(inline)
void siapp_drawCircleExF(siWindow* win, siVec2 pos, i32 radius, u32 sides, siColor color) {
    f32 diameter = radius * 2.0f;
	siapp_drawPolygonF(win, SI_VEC4(pos.x, pos.y, diameter, diameter), sides, color);
}
F_TRAITS(inline)
void siapp_drawPolygon(siWindow* win, siRect rect, u32 sides, siColor color) {
	siapp_drawPolygonF(
        win,
        SI_VEC4(rect.x, rect.y, rect.width, rect.height),
        sides, color
    );
}
void siapp_drawPolygonF(siWindow* win, siVec4 rect, u32 sides, siColor color) {
	SI_ASSERT_NOT_NULL(win);

    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    SI_STOPIF(gl->vertexCounter + sides > gl->maxVertexCount, siapp_windowRender(win));

    f32 radiusX = i32ToNDCX(rect.x + rect.z / 2.0f, gl->size.width);
    f32 radiusY = i32ToNDCY(rect.y + rect.w / 2.0f, gl->size.height);
    f32 x2 = rect.z / gl->size.width;
    f32 y2 = rect.w / gl->size.height; /* NOTE(EimaMei): We can just use the
                                            width/height directly instead of
                                            radiuses due to the NDC formula
                                            requiring a multiplication by two,
                                            so the divide and multiply get
                                            canceled out. */


	f32 theta = SI_TO_RADIANS(360.0f / (f32)sides);
    f32 angle = 0;

	siapp_color4f(win, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

    f32 x, y;
    for_range (i, 0, sides) {
        x = radiusX + x2 * si_sin(angle),
        y = radiusY + y2 * si_cos(angle);

        siapp_drawVertex2f(win, x, y);
        angle += theta;
	}

    u32 half = sides / 2;
    half &= ~SI_BIT(0); /* NOTE(EimaMei): Makes the number even by clearing bit 0. */
	siapp__addVertexesToCMD(gl, sides + half, sides);
}

F_TRAITS(inline)
void siapp_drawText(siWindow* win, siText text, siPoint pos, u32 size) {
	siapp_drawTextF(win, text, SI_VEC2(pos.x, pos.y), size);
}
F_TRAITS(inline)
void siapp_drawTextItalic(siWindow* win, siText text, siPoint pos, u32 size) {
	siapp_drawTextItalicF(win, text, SI_VEC2(pos.x, pos.y), size);
}
F_TRAITS(inline)
siVec2 siapp_drawTextFmt(siWindow* win, siText text, siPoint pos, u32 size) {
	return siapp_drawTextFmtF(win, text, SI_VEC2(pos.x, pos.y), size);
}
void siapp_drawTextF(siWindow* win, siText text, siVec2 pos, u32 size) {
	SI_ASSERT_NOT_NULL(win);

	f32 scaleFactor = (f32)size / text.font->size * 1.15f;
	siVec2 base = pos;

	for_range (i, 0, text.len) {
		i32 realIndex = text.chars[i];

		if (realIndex < 0) { // TODO(EimaMei): Benchmark is a benchless verion is faster.
			switch (-realIndex) {
				case ' ': {
					base.x += text.font->advance.space * scaleFactor;
					continue;
				}
				case '\t': {
					base.x += text.font->advance.tab * scaleFactor;
					continue;
				}
				case '\n': {
					base.x = pos.x;
					base.y += text.font->advance.newline * scaleFactor;
					continue;
				}
				case '\r': {
					SI_STOPIF((usize)i + 1 <= text.len && -text.chars[i + 1] == '\n', continue);
					base.x = pos.x;
					base.y += size + size * 0.25f;
					continue;
				}
				default: {
					realIndex = text.font->unknownSymbolIndex;
				}
			}
		}

		siGlyphInfo* glyph = &text.font->glyphs[realIndex];
		f32 advance = siapp_drawCharacterScale(win, text.font, glyph, base, size, scaleFactor);
		base.x += advance;
	}
}
void siapp_drawTextItalicF(siWindow* win, siText text, siVec2 pos, u32 size) {
	SI_ASSERT_NOT_NULL(win);

	f32 scaleFactor = (f32)size / text.font->size * 1.15;
	siVec2 base = pos;

	for_range (i, 0, text.len) {
		i32 realIndex = text.chars[i];

		if (realIndex < 0) { // TODO(EimaMei): Benchmark is a benchless verion is faster.
			switch (-realIndex) {
				case ' ': {
					base.x += text.font->advance.space * scaleFactor;
					continue;
				}
				case '\t': {
					base.x += text.font->advance.tab * scaleFactor;
					continue;
				}
				case '\n': {
					base.x = pos.x;
					base.y += text.font->advance.newline * scaleFactor;
					continue;
				}
				case '\r': {
					SI_STOPIF((usize)i + 1 <= text.len && -text.chars[i + 1] == '\n', continue);
					base.x = pos.x;
					base.y += size + size * 0.25f;
					continue;
				}
				default: {
					realIndex = text.font->unknownSymbolIndex;
				}
			}
		}

		siGlyphInfo* glyph = &text.font->glyphs[realIndex];
		f32 advance = siapp_drawCharacterItalicScale(win, text.font, glyph, base, size, scaleFactor);
		base.x += advance;
	}
}
siVec2 siapp_drawTextFmtF(siWindow* win, siText text, siVec2 pos, u32 size) {
	SI_ASSERT_NOT_NULL(win);

	f32 scaleFactor = (f32)size / text.font->size * 1.15;
	siVec2 base = pos;
	f32 maxWidth = 0;

	b32 italic = false;
	b32 underline = false;
	siVec2 underlinePos = {0};

	for_range (i, 0, text.len) {
		i32 realIndex = text.chars[i];

		if (realIndex < 0) { // TODO(EimaMei): Benchmark is a benchless verion is faster.
			switch (-realIndex) {
				case ' ': {
					base.x += text.font->advance.space * scaleFactor;
					f32 curWidth = 0;
					for_range (x, i + 1, text.len) {
						SI_STOPIF(text.chars[x] < 0, break);
						f32 advanceX = siapp_textAdvanceXGet(text, x);
						curWidth += advanceX * scaleFactor;
					}

					if (base.x + curWidth > win->e.windowSizeScaled.width) {
						maxWidth = base.x;
						base.x = pos.x;
						base.y += text.font->advance.newline * scaleFactor;
					}
					continue;
				}
				case '\t': {
					base.x += text.font->advance.newline * scaleFactor;
					continue;
				}
				case '\r':
				case '\n': {
					base.x = pos.x;
					base.y += text.font->advance.newline * scaleFactor;
					continue;
				}

				case '*' : {
					italic = !italic;
					continue;
				}
				case '_': {
					if (underline == false) {
						underline = true;
						underlinePos = base;
					}
					else {
						underline = false;
						siVec2 start = underlinePos;
						siapp_drawRectF(
							win,
							SI_VEC4(start.x, start.y + size + size * 0.15, base.x - start.x, size / 16.0f * 1.15f),
							SI_RGB(255, 255, 255)
						);
					}
					continue;
				}
				default: {
					realIndex = text.font->unknownSymbolIndex;
				}
			}
		}

		siGlyphInfo* glyph = &text.font->glyphs[realIndex];
		f32 advance = (!italic)
			? siapp_drawCharacterScale(win, text.font, glyph, base, size, scaleFactor)
			: siapp_drawCharacterItalicScale(win, text.font, glyph, base, size, scaleFactor);

		base.x += advance;
	}
	return SI_VEC2(maxWidth, base.y - pos.y + size);
}

f32 siapp_drawCharacterScale(siWindow* win, const siFont* font, const siGlyphInfo* character,
		siVec2 pos, u32 size, f32 scaleFactor) {
	SI_ASSERT_NOT_NULL(win);

    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    SI_STOPIF(gl->vertexCounter + 4 > gl->maxVertexCount, siapp_windowRender(win));

	const siGlyphInfo* glyph = character;
	siImage img = siapp_spriteSheetSpriteGet(font->sheet, glyph->texID);

	img.x2 = img.x1 + (glyph->width / img.atlas->totalWidth);
	img.y2 = img.y1 + (glyph->height / img.atlas->texHeight);

	f32 x = glyph->x * scaleFactor;
	f32 y = glyph->y * scaleFactor;
	f32 width = glyph->width * scaleFactor;
	f32 height = glyph->height * scaleFactor;

	f32 baseX = pos.x + x,
		baseY = pos.y + y + size;

	f32 x1 = i32ToNDCX(baseX, gl->size.width);
	f32 x2 = i32ToNDCX(baseX + width, gl->size.width);
	f32 y1 = i32ToNDCY(baseY, gl->size.height);
	f32 y2 = i32ToNDCY(baseY + height, gl->size.height);

	{
		siVec4 clr = win->textColor.vec4;
		siapp_color4f(win, clr.x, clr.y, clr.z, clr.w);
		gl->curTex = &img;

		siapp_texCoords2f(win, img.x1, img.y1); siapp_drawVertex2f(win, x1, y1);
		siapp_texCoords2f(win, img.x2, img.y1); siapp_drawVertex2f(win, x2, y1);
		siapp_texCoords2f(win, img.x2, img.y2); siapp_drawVertex2f(win, x2, y2);
		siapp_texCoords2f(win, img.x1, img.y2); siapp_drawVertex2f(win, x1, y2);
		siapp_texCoords2f(win, 0, 0);

		siapp__addVertexesToCMD(gl, 6, 4);
		gl->curTex = &gl->defaultTex;
	}

	return glyph->advanceX * scaleFactor;
}
f32 siapp_drawCharacterItalicScale(siWindow* win, const siFont* font, const siGlyphInfo* character,
		siVec2 pos, u32 size, f32 scaleFactor) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    SI_STOPIF(gl->vertexCounter + 4 > gl->maxVertexCount, siapp_windowRender(win));

	const siGlyphInfo* glyph = character;
	siImage img = siapp_spriteSheetSpriteGet(font->sheet, glyph->texID);

	img.x2 = img.x1 + (glyph->width / img.atlas->totalWidth);
	img.y2 = img.y1 + (glyph->height / img.atlas->texHeight);

	f32 x = glyph->x * scaleFactor;
	f32 y = glyph->y * scaleFactor;
	f32 width = glyph->width * scaleFactor;
	f32 height = glyph->height * scaleFactor;

	f32 baseX = pos.x + x,
		baseY = pos.y + y + size;

	f32 shift = height * si_sin(SI_TO_RADIANS(14));

	f32 x1 = i32ToNDCX(baseX + shift, gl->size.width);
	f32 x2 = i32ToNDCX(baseX + shift + width, gl->size.width);
	f32 x3 = i32ToNDCX(baseX +  width, gl->size.width);
	f32 x4 = i32ToNDCX(baseX, gl->size.width);

	f32 y1 = i32ToNDCY(baseY, gl->size.height);
	f32 y2 = i32ToNDCY(baseY + height, gl->size.height);

	{
		siVec4 clr = win->textColor.vec4;
		siapp_color4f(win, clr.x, clr.y, clr.z, clr.w);
		gl->curTex = &img;

		siapp_texCoords2f(win, img.x1, img.y1); siapp_drawVertex2f(win, x1, y1);
		siapp_texCoords2f(win, img.x2, img.y1); siapp_drawVertex2f(win, x2, y1);
		siapp_texCoords2f(win, img.x2, img.y2); siapp_drawVertex2f(win, x3, y2);
		siapp_texCoords2f(win, img.x1, img.y2); siapp_drawVertex2f(win, x4, y2);
		siapp_texCoords2f(win, 0, 0);

		siapp__addVertexesToCMD(gl, 6, 4);
		gl->curTex = &gl->defaultTex;
	}

	return (glyph->advanceX + shift) * scaleFactor;
}

F_TRAITS(inline)
void siapp_color3f(siWindow* win, f32 r, f32 g, f32 b) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
	gl->curColor = SI_VEC4(r, g, b, 1.0f);
}
F_TRAITS(inline)
void siapp_color4f(siWindow* win, f32 r, f32 g, f32 b, f32 a) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
	gl->curColor = SI_VEC4(r, g, b, a);
}
F_TRAITS(inline)
void siapp_colorVec4f(siWindow* win, siVec4 color) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
	gl->curColor = color;
}

void siapp_texCoords2f(siWindow* win, f32 x, f32 y) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
	gl->curTexCoords = SI_VEC2(x, y);
}


F_TRAITS(inline)
siOpenGLInfo siapp_OpenGLGetInfo(void) {
	return glInfo;
}

const char* GetStringForEnum(GLenum value) {
	switch (value) {
		// Source enums
		case GL_DEBUG_SOURCE_API:
			return "GL_DEBUG_SOURCE_API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			return "GL_DEBUG_SOURCE_SHADER_COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			return "GL_DEBUG_SOURCE_THIRD_PARTY";
		case GL_DEBUG_SOURCE_APPLICATION:
			return "GL_DEBUG_SOURCE_APPLICATION";
		case GL_DEBUG_SOURCE_OTHER:
			return "GL_DEBUG_SOURCE_OTHER";

		// Type enums
		case GL_DEBUG_TYPE_ERROR:
			return "GL_DEBUG_TYPE_ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY:
			return "GL_DEBUG_TYPE_PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE:
			return "GL_DEBUG_TYPE_PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER:
			return "GL_DEBUG_TYPE_MARKER";
		case GL_DEBUG_TYPE_PUSH_GROUP:
			return "GL_DEBUG_TYPE_PUSH_GROUP";
		case GL_DEBUG_TYPE_POP_GROUP:
			return "GL_DEBUG_TYPE_POP_GROUP";
		case GL_DEBUG_TYPE_OTHER:
			return "GL_DEBUG_TYPE_OTHER";

		// Severity enums
		case GL_DEBUG_SEVERITY_HIGH:
			return "GL_DEBUG_SEVERITY_HIGH";
		case GL_DEBUG_SEVERITY_MEDIUM:
			return "GL_DEBUG_SEVERITY_MEDIUM";
		case GL_DEBUG_SEVERITY_LOW:
			return "GL_DEBUG_SEVERITY_LOW";
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			return "GL_DEBUG_SEVERITY_NOTIFICATION";

		default:
			return "UNKNOWN_ENUM";
	}
}

void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
							GLsizei length, const GLchar* message, const void* userParam) {
	si_printf(
		"SILI DEBUG: %s %s %i %s: '%s'\n",
			GetStringForEnum(source), GetStringForEnum(type),
			id, GetStringForEnum(severity), length, message
	);
	SI_UNUSED(userParam);
}

b32 siapp_windowOpenGLInit(siWindow* win, u32 maxDrawCount, u32 maxTexCount,
        siArea maxTexRes) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;
	gl->size = win->originalSize;

#if defined(SIAPP_PLATFORM_API_WIN32)
	PIXELFORMATDESCRIPTOR pfd = {0};
	pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion     = 1;
	pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType   = PFD_TYPE_RGBA;
	pfd.cColorBits   = 32;
	pfd.cDepthBits   = 24;
	pfd.cStencilBits = 8;

	i32 format = ChoosePixelFormat(win->hdc, &pfd);
	SIAPP_ERROR_CHECK(format == 0, "ChoosePixelFormat");

	PIXELFORMATDESCRIPTOR suggested;
	b32 res = DescribePixelFormat(win->hdc, format, sizeof(suggested), &suggested);
	SIAPP_ERROR_CHECK(format == 0, "DescribePixelFormat");

	if (
		(suggested.dwFlags & PFD_GENERIC_ACCELERATED) == 0
		&& (suggested.dwFlags & PFD_GENERIC_FORMAT) != 0
	) {
		pfd.dwFlags |= PFD_GENERIC_FORMAT;
	}

	res = SetPixelFormat(win->hdc, format, &pfd);
	SIAPP_ERROR_CHECK(res == false, "DescribePixelFormat");

	gl->context = wglCreateContext(win->hdc);
	SIAPP_ERROR_CHECK(gl->context == nil, "wglCreateContext");

	res = wglMakeCurrent(win->hdc, gl->context);
	SIAPP_ERROR_CHECK(res == false, "wglMakeCurrent");

#elif defined (SIAPP_PLATFORM_API_X11)
    // NOTE(EimaMei): Most of this is taken from https://github.com/ColleagueRiley/RGFW/blob/main/RGFW.h.

    i32 defaultAttribs[] = {
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_DOUBLEBUFFER    , True,
		GLX_STENCIL_SIZE	, glInfo.stencilSize,
		GLX_STEREO			, glInfo.stereo,
		GLX_AUX_BUFFERS		, glInfo.auxBuffers,
		None
	};

	i32 fbCount;
	GLXFBConfig* fbList = glXChooseFBConfig(win->display, DefaultScreen(win->display), defaultAttribs, &fbCount);
	SI_ASSERT(fbCount > 0);

    if (fbList == NULL) { /* resort to single-buffer if DB not available */
        i32 fallbackAttribs[] = { GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, None };
        si_printf("fallback\n");

        fbList = glXChooseFBConfig(win->display, DefaultScreen(win->display), fallbackAttribs, &fbCount);
		SI_ASSERT(fbCount > 0);
	}

    usize fbIndex = 0;
    i32 bufSamples;

	for_range (i, 0, fbCount) {
        glXGetFBConfigAttrib(win->display, fbList[i], GLX_SAMPLE_BUFFERS, &bufSamples);

		if ((u32)bufSamples == glInfo.sampleBuffers) {
            fbIndex = i;
            break;
        }
	}

    GLXFBConfig bestFbc = fbList[fbIndex];
	XVisualInfo* vi = glXGetVisualFromFBConfig(win->display, bestFbc);

    GLXContext context = glXCreateContext(win->display, vi, NULL, GL_TRUE);
    glXMakeCurrent(win->display, win->hwnd, context);
    gl->context = context;

    XFree(fbList);
    XFree(vi);

#endif
    if (glInfo.isLoaded == false) {
        sigl_loadOpenGLAll();
        glInfo.isLoaded = true;

        glGetIntegerv(GL_MAJOR_VERSION, &glInfo.versionSelected.major);
        glGetIntegerv(GL_MINOR_VERSION, &glInfo.versionSelected.major);

       if (glInfo.versionSelected.major == 0 && glInfo.versionSelected.minor == 0) {
            siapp_messageBox(
                    "OpenGL not present",
                    "OpenGL version on this system is set to 0.0, meaning it doesn't exist.\n"
                    "If you have a GPU in your computer, install the latest OpenGL "
                    "drivers from your GPU's vendor. If not, this application cannot "
                    "be ran properly on this computer setup.",
                    SI_MESSAGE_BOX_OK, SI_MESSAGE_BOX_ICON_ERROR
                    );
            return false;
        }
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glInfo.texSizeMax);

        SI_STOPIF(glInfo.versionSelected.major == 0, glInfo.versionSelected.major = glInfo.versionMax.major);
        SI_STOPIF(glInfo.versionSelected.minor == 0, glInfo.versionSelected.minor = glInfo.versionMax.minor);
    }

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(DebugCallback, nil);

    glGenVertexArrays(1, &gl->VAO);
	glBindVertexArray(gl->VAO);
	glGenBuffers(countof(gl->VBOs), gl->VBOs);

	gl->programID = glCreateProgram();
	SI_ASSERT_MSG(gl->programID != 0, "gl->programID cannot be zero.");

	{
		char updatedVShader[sizeof(defaultVShaderCode) + 20];
		si_snprintf(updatedVShader, countof(updatedVShader), defaultVShaderCode, maxDrawCount);

		i32 vertexShader = si_OpenGLShaderMake(GL_VERTEX_SHADER, updatedVShader);
		i32 fragmentShader = si_OpenGLShaderMake(GL_FRAGMENT_SHADER, defaultFShaderCode);
		SI_STOPIF(vertexShader == -1, SIAPP_ERROR_MSGBOX_GL(gl->programID, "Failed to create vertex shader"); return false);
		SI_STOPIF(fragmentShader == -1, SIAPP_ERROR_MSGBOX_GL(gl->programID, "Failed to create fragment shader"); return false);

		glAttachShader(gl->programID, vertexShader);
		glAttachShader(gl->programID, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	glLinkProgram(gl->programID);

	GLint linkStatus;
	glGetProgramiv(gl->programID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		SIAPP_ERROR_MSGBOX_GL(gl->programID, "Failed to link programID.");
		return false;
	}

	glUseProgram(gl->programID);

	glBindAttribLocation(gl->programID, SI_SHADER_POS, "pos");
	glBindAttribLocation(gl->programID, SI_SHADER_TEX, "tex");
	glBindAttribLocation(gl->programID, SI_SHADER_CLR, "clr");
	glBindAttribLocation(gl->programID, SI_SHADER_ID,  "info");

	GL_BUFFER_MAKE(SI_VBO_POS, gl->vertices,  sizeof(siVec3) * 4 * maxDrawCount);
	GL_BUFFER_MAKE(SI_VBO_CLR, gl->colors,    sizeof(siVec4) * 4 * maxDrawCount);
	GL_BUFFER_MAKE(SI_VBO_TEX, gl->texCoords, sizeof(siVec2) * 4 * maxDrawCount);
	GL_BUFFER_MAKE(SI_VBO_ID,  gl->batchInfo, sizeof(siOpenGLIDs) * maxDrawCount);

	gl->CMDs = si_mallocArray(win->alloc, siOpenGLDrawCMD, maxDrawCount);
	gl->matrices = si_mallocArray(win->alloc, siMatrix, maxDrawCount);

    typedef u16 siOpenGLIndices[6];
    siOpenGLIndices* indices = (siOpenGLIndices*)malloc(sizeof(siOpenGLIndices) * maxDrawCount);

	usize k = 0;
    for_range (i, 0, maxDrawCount) {
        gl->matrices[i] = rglMatrixIdentity();

		indices[i][0] = k + 0;
		indices[i][1] = k + 1;
		indices[i][2] = k + 2;

		indices[i][3] = k + 0;
		indices[i][4] = k + 2;
		indices[i][5] = k + 3;

		k += 4;
	}


	// TODO(EimaMei): See if this could be optimized as well.
	glBindBuffer(GL_ARRAY_BUFFER, gl->VBOs[SI_VBO_POS]);
	glVertexAttribPointer(SI_SHADER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(SI_SHADER_POS);

	glBindBuffer(GL_ARRAY_BUFFER, gl->VBOs[SI_VBO_TEX]);
	glVertexAttribPointer(SI_SHADER_TEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(SI_SHADER_TEX);

	glBindBuffer(GL_ARRAY_BUFFER, gl->VBOs[SI_VBO_CLR]);
	glVertexAttribPointer(SI_SHADER_CLR, 4, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(SI_SHADER_CLR);

	glBindBuffer(GL_ARRAY_BUFFER, gl->VBOs[SI_VBO_ID]);
	glVertexAttribIPointer(SI_SHADER_ID, 2, GL_UNSIGNED_INT, 0, 0);
	glEnableVertexAttribArray(SI_SHADER_ID);
	glVertexAttribDivisor(SI_SHADER_ID, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->VBOs[SI_VBO_ELM]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(siOpenGLIndices) * maxDrawCount, indices, GL_STATIC_DRAW);
    free(indices);

	glUniformMatrix4fv(gl->uniformMvp, maxDrawCount, GL_FALSE, gl->matrices->m);

	gl->vertexCounter = 0;
	gl->drawCounter = 0;
	gl->bgColor = SI_VEC4(1, 1, 1, 1);
	gl->curTexCoords = SI_VEC2(0, 0);
	gl->gradientLen = 0;
    gl->maxVertexCount = maxDrawCount * 4;

	gl->uniformTexture = glGetUniformLocation(gl->programID, "textures");
	gl->uniformMvp = glGetUniformLocation(gl->programID, "mvp");

	win->atlas = siapp_textureAtlasMake(win, maxTexRes, maxTexCount, SI_RESIZE_DEFAULT);
	gl->defaultTex = siapp_imageLoadEx(win->atlas, si_buf(siByte, 255, 255, 255, 255), 1, 1, 4);
	gl->curTex = &gl->defaultTex;

	return true;
}
void siapp_windowOpenGLRender(siWindow* win) {
	SI_ASSERT_NOT_NULL(win);

	siWinRenderingCtxOpenGL* gl = &win->render.opengl;

	glUseProgram(gl->programID);
	glBindVertexArray(gl->VAO);
	glUniformMatrix4fv(gl->uniformMvp, gl->drawCounter, GL_FALSE, gl->matrices->m);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->VBOs[SI_VBO_ELM]);
	glMultiDrawElementsIndirect(GL_TRIANGLE_FAN, GL_UNSIGNED_SHORT, gl->CMDs, gl->drawCounter, 0);
	glFinish();

	gl->vertexCounter = 0;
	gl->drawCounter = 0;
}
void siapp_windowOpenGLDestroy(siWindow* win) {
	SI_ASSERT_NOT_NULL(win);
	siWinRenderingCtxOpenGL* gl = &win->render.opengl;

	glUseProgram(gl->programID);
	glBindVertexArray(gl->VAO);

	for_range (i, 0, countof(gl->VBOs)) {
		glBindBuffer(GL_ARRAY_BUFFER, gl->VBOs[i]);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	si_free(win->alloc, gl->CMDs);

	glDeleteBuffers(countof(gl->VBOs), gl->VBOs);
	glDeleteVertexArrays(1, &gl->VAO);
	glDeleteTextures(1, &win->atlas->texID);

	glDeleteProgram(gl->programID);

#if defined(SIAPP_PLATFORM_API_WIN32)
    wglDeleteContext(gl->context);
#elif defined(SIAPP_PLATFORM_API_X11)
    glXDestroyContext(win->display, gl->context);
#endif
}

void siapp_OpenGLVersionSet(siVersion ver) {
    SI_ASSERT(si_betweenu(ver.major, 0, 4));
    SI_ASSERT(si_betweenu(ver.minor, 0, 6));
    glInfo.versionSelected = ver;
}
void siapp_OpenGLStencilSet(u32 stencil) { glInfo.stencilSize = stencil; }
void siapp_OpenGLSamplesSet(u32 samples) { glInfo.sampleBuffers = samples; }
void siapp_OpenGLStereoSet(b32 stereo) { glInfo.stereo = stereo & 1; }
void siapp_OpenGLAuxBuffersSet(u32 auxBuffers) { glInfo.auxBuffers = auxBuffers; }



F_TRAITS(inline)
siMessageBoxResult siapp_messageBox(cstring title, cstring message,
		siMessageBoxButton buttons, siMessageBoxIcon icon) {
	return siapp_messageBoxEx(nil, title, si_cstrLen(title), message, si_cstrLen(message), buttons, icon);
}

siMessageBoxResult siapp_messageBoxEx(const siWindow* win, cstring title, usize titleLen,
		cstring message, usize messageLen, siMessageBoxButton buttons,
		siMessageBoxIcon icon) {
	SI_ASSERT_NOT_NULL(title);
	SI_ASSERT_NOT_NULL(message);

#if defined(SIAPP_PLATFORM_API_WIN32)
	siAllocator* tmp = si_allocatorMake((titleLen + messageLen + 2) * sizeof(u16) + 255);

	siUtf16String wideTitle = si_utf8ToUtf16Str(tmp, title, nil);
	siUtf16String wideMessage = si_utf8ToUtf16Str(tmp, message, nil);

	i32 flags = 0;
	switch (buttons) {
		case SI_MESSAGE_BOX_OK: flags |= MB_OK; break;
		case SI_MESSAGE_BOX_OK_CANCEL: flags |= MB_OKCANCEL; break;
		case SI_MESSAGE_BOX_YES_NO: flags |= MB_YESNO; break;
		case SI_MESSAGE_BOX_YES_NO_CANCEL: flags |= MB_YESNOCANCEL; break;
		case SI_MESSAGE_BOX_RETRY_CANCEL: flags |= MB_RETRYCANCEL; break;
		case SI_MEESAGE_BOX_CANCEL_TRY_CONTINUE: flags |= MB_CANCELTRYCONTINUE; break;
		case SI_MESSAGE_BOX_HELP: flags |= MB_HELP; break;
	}
	switch (icon) {
		case SI_MESSAGE_BOX_ICON_ERROR: flags |= MB_ICONERROR; break;
		case SI_MESSAGE_BOX_ICON_WARNING: flags |= MB_ICONWARNING; break;
		case SI_MESSAGE_BOX_ICON_INFO: flags |= MB_ICONINFORMATION; break;
		case SI_MESSAGE_BOX_ICON_QUESTION: flags |= MB_ICONQUESTION; break;
	}

	HWND hwnd = (win != nil) ? win->hwnd : nil;
	siMessageBoxResult res = MessageBoxW(hwnd, wideMessage, wideTitle, flags);

	switch (res) {
		case IDTRYAGAIN: res = SI_MESSAGE_BOX_RESULT_TRY_AGAIN; break;
		case IDCONTINUE: res = SI_MESSAGE_BOX_RESULT_CONTINUE; break;
	}

	si_allocatorFree(tmp);
    return res;
#endif

}

#endif /* SIAPP_IMPLEMENTATION */

#if defined(__cplusplus)
}
#endif

#endif /* SIAPP_INCLUDE_SIAPP_H */

/*
------------------------------------------------------------------------------
Copyright (C) 2024 EimaMei

This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
------------------------------------------------------------------------------
*/
