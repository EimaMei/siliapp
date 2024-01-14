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

#ifndef SIAPP_INCLUDE_SI_H
#define SIAPP_INCLUDE_SI_H

#include <deps/sili/sili.h>
#include <deps/stb/stb_truetype.h> // TODO(EimaMei): Remove later.

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
    #error "siliapp.h is not implemented for Unix"
    #define SIAPP_PLATFORM_API_X11
#elif defined(SI_SYSTEM_WINDOWS)
    #include <windows.h>
    #include <shlobj.h>
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

typedef SI_ENUM(b32, siWindowArg) {
    SI_WINDOW_CENTER          = SI_BIT(0),
    SI_WINDOW_FULLSCREEN      = SI_BIT(1),
    SI_WINDOW_BORDERLESS      = SI_BIT(2),
    SI_WINDOW_RESIZABLE       = SI_BIT(3),

    SI_WINDOW_MINIMIZED       = SI_BIT(4),
    SI_WINDOW_MAXIMIZED       = SI_BIT(5),
    SI_WINDOW_HIDDEN          = SI_BIT(6),

    SI_WINDOW_RENDERING_CPU   = SI_BIT(7),
    SI_WINDOW_RENDERING_OPENGL   = SI_BIT(8),

    SI_WINDOW_SCALING = SI_BIT(9),
    SI_WINDOW_OPTIMAL_SIZE = SI_BIT(10),
    SI_WINDOW_KEEP_ASPECT_RATIO = SI_BIT(11),

    SI_WINDOW_DEFAULT         = SI_WINDOW_CENTER | SI_WINDOW_RESIZABLE | SI_WINDOW_RENDERING_OPENGL,
    SI_WINDOW_DESKTOP         = SI_WINDOW_FULLSCREEN | SI_WINDOW_BORDERLESS,
    SI_WINDOW_RENDERING_BITS  = SI_WINDOW_RENDERING_CPU | SI_WINDOW_RENDERING_OPENGL
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

    /* TODO. */
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

    SK__EVENT, /* NOTE(EimaMei): Used for setting bit 7 and 6 to denote key changes,
                      do not use! */
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

typedef SI_ENUM(i32, siCursorType) {
    SI_CURSOR_ARROW = 0,
    SI_CURSOR_HAND,
    SI_CURSOR_DOUBLE_ARROW_HORIZONTAL,
    SI_CURSOR_TEXT_SELECT,
    SI_CURSOR_COUNT,

    SI_CURSOR_CUSTOM = -1
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
} siWinEventType;

typedef struct {
    b8 clicked : 1;
    b8 pressed : 1;
    b8 released : 1;
} siKeyState;


typedef struct {
    siWinEventType type;

    char charBuffer[32]; // x11 - complete, win32 - N/A, cocoa - N/A
    usize charBufferLen;

    siKeyState keys[SK_COUNT]; // x11 - complete, win32 - N/A, cocoa N/A

    b32 focus; // x11 - complete, win32 - N/A, cocoa - N/A
    b32 mouseInside; // x11 - complete, win32 - N/A, cocoa - N/A

    siPoint mouse; // x11 - complete, win32 - N/A, cocoa - N/A
    siPoint mouseRoot; // x11 - complete, KINDA N/A, win32 - N/A, cocoa - N/A
    siKeyState mouseButtons[SI_MOUSE_COUNT]; // x11 - complete, win32 - N/A, cocoa - N/A
    siMouseWheelType mouseWheel; // x11 - complete, win32 - N/A, cocoa - N/A

    siPoint windowPos; // x11 - complete, win32 - N/A, cocoa - N/A
    siArea windowSize; // x11 - complete, win32 - N/A, cocoa - N/A
    siArea windowSizeScaled;

    //f64 curr_time; // N/A
    //f64 frame_delta_time; // N/A
    struct {
        siKeyType keyCache[16];
        usize keyCacheLen;
    } __private;
} siWindowEvent;


typedef struct {
    f32 m[16];
} siMatrix;

typedef struct {
    i32 renderingType;

    u32 texID;
    u32 texWidth;
    u32 texHeight;
    u32 totalWidth;

    u32 curCount;
    u32 maxCount;
} siTextureAtlas;

typedef struct {
    siTextureAtlas* atlas;
    u32 id;
    siArea size;
    f32 x1, y1, x2, y2;
} siImage;


typedef struct {
    b32 isLoaded;
    siVersion versionSelected;
    siVersion versionMax;
    i32 max2DTexSize;
} siOpenGLInfo;

typedef struct {
    u32 count;
    u32 instanceCount;
    u32 firstIndex;
    i32 baseVertex;
    u32 baseInstance;
} siOpenGLDrawCMD;

typedef struct {
    rawptr context;
    siArea size;

    u32 programID;
    u32 VAO;
    u32 VBOs[5];

    u32 uniformTexture;
    u32 uniformMvp;

    siVec4 curColor;
    siVec4 bgColor;
    siVec2 curTexCoords;

    siImage* curTex;
    siImage defaultTex;

    f32* vertices;
    f32* colors;
    f32* texCoords;
    u32* batchInfo;
    u16* indices;
    siMatrix* matrices;
    siOpenGLDrawCMD* CMDs;

    u32 vertexCounter;
    u32 drawCounter;
} siWinRenderingCtxOpenGL;

typedef struct {
    siAllocator* alloc;
    siTextureAtlas* atlas;

#if defined(SIAPP_PLATFORM_API_WIN32)
    HWND hwnd;
    HDC hdc;
#endif

    union {
        siWinRenderingCtxOpenGL opengl;
    } render;

    union {
        siVec4 vec4;
        siColor rgba;
    } textColor;

    siWindowEvent e;
    siWindowArg arg;
    u32 maxDrawCount;

    siVec2 scaleFactor;
    siArea originalSize;
    siCursorType cursor; // x11 - complete, win32 - N/A, cocoa - N/A
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
    SI_SEARCH_FOLDERS_ONLY = SI_BIT(0),
    SI_SEARCH_ALLOW_MULTIPLE = SI_BIT(1)
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

#define SI_GLYPHSET_KATAKANA {0x30A0, 0x30FF}
// TODO: Enforce a chronological order.


#define SI_CHARSET_WGL4 \
    si_buf(siGlyphSet, \
        SI_GLYPHSET_ASCII, SI_GLYPHSET_ISO_8859_1, SI_GLYPHSET_LATIN_1_SUPPLEMENT, \
        SI_GLYPHSET_LATIN_EXTENDED_A, {0x0192, 0x0192}, {0x01FA, 0x01FF}, {0x02C6, 0x02C7}, \
        {0x02C9, 0x02C9}, {0x02D7, 0x02DD}, {0x0384, 0x038B}, {0x038C, 0x38C}, \
        {0x038E, 0x03A1}, {0x03A3, 0x03CE}, {0x400, 0x45F}, {0x0490, 0x0491}, \
        {0x1E80, 0x1E85}, {0x2013, 0x2015}, {0x2016, 0x201E}, {0x2020, 0x2022}, \
        {0x0001F5FF, 0x0001F5FF} \
    )

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


/* Creates a windows based on the specified name, size and arguments, returns a
 * 'siWindow' object. */
siWindow* siapp_windowMake(siAllocator* alloc, cstring name, siArea size,
        siWindowArg arg, u32 maxDrawCount, u32 maxTexCount, siArea maxTexRes);
/* Creates a windows based on the specified name, position, size and arguments,
 * returns a 'siWindow' object. */
siWindow* siapp_windowMakeEx(siAllocator* alloc, cstring name, siPoint pos,
        siArea size, siWindowArg arg, u32 maxDrawCount, u32 maxTexCount, siArea maxTexRes);

/* Checks the changes for the specified window and updates the contents of 'out'. */
void siapp_windowUpdate(siWindow* win, b32 await);
/* Writes new graphics into the current draw buffer. */
void siapp_windowRender(siWindow* win);
/* Clears the graphics of the screen. */
void siapp_windowClear(const siWindow* win);
/* Renders the graphics onto the screen. */
void siapp_windowSwapBuffers(const siWindow* win);
/* Closes the window. */
void siapp_windowClose(siWindow* win);

/* Hides the window depending on the boolean value. If 'true' - shows the window,
 * if 'false' - hides the window. */
void siapp_windowShow(const siWindow* win, b32 show);

/* Sets the background color of the app. */
void siapp_windowBackgroundSet(siWindow* win, siColor color);

/* Returns the current state of the specified key. */
siKeyState siapp_windowKeyGet(const siWindow* win, siKeyType key);
/* Returns a boolean indicating if the key was clicked this frame. */
b32 siapp_windowKeyClicked(const siWindow* win, siKeyType key);
/* Returns a boolean indicating if the key is being pressed right now. */
b32 siapp_windowKeyPressed(const siWindow* win, siKeyType key);
/* Returns a boolean indicating if the key was released this frame. */
b32 siapp_windowKeyReleased(const siWindow* win, siKeyType key);

/* Returns a 'siWindowEvent' object from specified window. */
const siWindowEvent* siapp_windowEventGet(const siWindow* win);

/* Returns the current cursor type for the window. */
siCursorType siapp_windowCursorGet(const siWindow* win);
/* Sets a new cursor for the window. */
void siapp_windowCursorSet(siWindow* win, siCursorType cursor);

/* */
void siapp_windowTextColorSet(siWindow* win, siColor color);


/* Returns the current resolution of the screen. */
siArea siapp_screenGetCurrentSize(void);
/* Returns the area of the resolution index. 0 is the lowest available. If the
 * specified index resolution doesn't exist, the function will return 'SI_AREA(-1, -1)'.
 * Using that you can find the end of the list by checking if the return's width is
 * negative one.*/
siArea siapp_screenGetAvailableResolution(usize index);


/* Converts an OS keycode to 'siKeyType'.  */
siKeyType siapp_osKeyToSili(i32 key);
/* Converts an OS enum error intto a human-readable string (max length size is 128 bytes). */
cstring siapp_osErrToStr(i32 error);


/* */
siSiliStr siapp_usernameGet(void);


/* */
siSearchResult siapp_fileManagerOpen(siAllocator* alloc, siSearchConfig config);


/* */
siSiliStr siapp_appDataPathMake(cstring folderName);
/* */
siSiliStr siapp_appDataPathMakeEx(cstring folderName, usize folderNameLen);


/* Creates a texture atlas from the given window. */
siTextureAtlas* siapp_textureAtlasMake(const siWindow* win, siArea area, u32 maxTexCount,
        siTextureResizeEnum enumName);
/* Frees the OpenGL context of the specified texture atlas. */
void siapp_textureAtlasFree(siTextureAtlas* atlas);
/* Applied swizzling mask to the 'param' channel from 'mask'. */
void siapp_textureSwizzleMask(const siTextureAtlas* atlas, siSwizzleEnum param,
        const siSwizzleValue mask[4]);


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
siFont siapp_fontLoad(siWindow* win, cstring path, i32 size, siGlyphSet* glyphs,
        usize arrayLen);
/* */
siFont siapp_fontLoadEx(siWindow* win, cstring path, i32 size, siGlyphSet* glyphs,
        usize arrayLen, u32 extraChars);

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
siVec2 siapp_textGetArea2f(siText text, u32 size);


/* Draws a vertex with the two specified NDC floats. */
void siapp_drawVertex2f(siWindow* win, f32 x, f32 y);
/* Draws a vertex with the three specified NDC floats. */
void siapp_drawVertex3f(siWindow* win, f32 x, f32 y, f32 z);
/* Draws a rectangle based on the specified 'siRect' and color. */

void siapp_drawRect(siWindow* win, siRect rect, siColor color);
/* */
void siapp_drawRect4f(siWindow* win, siVec4 rect, siColor color);
/* */
void siapp_drawImage(siWindow* win, siRect rect, siImage img);

/* Draws a triangle based on the specified position, size and color. */
void siapp_drawTriangle(siWindow* win, siPoint pos, i32 size, siColor color);

/* Draws a circle with 90 sides based on the specified position, radius and color, */
void siapp_drawCircle(siWindow* win, siPoint pos, i32 radius, siColor color);
/* Draws a circle based on the specified position, radius, color and sides. */
void siapp_drawCircleEx(siWindow* win, siPoint pos, i32 radius, siColor color,
        u32 sides);

/* */
void siapp_drawText(siWindow* win, siText text, siPoint pos, u32 size);
/* */
void siapp_drawTextItalic(siWindow* win, siText text, siPoint pos, u32 size);
/* */
siVec2 siapp_drawTextFmt(siWindow* win, siText text, siPoint pos, u32 size);
/* */
void siapp_drawText2f(siWindow* win, siText text, siVec2 pos, u32 size);
/* */
void siapp_drawTextItalic2f(siWindow* win, siText text, siVec2 pos, u32 size);
/* */
siVec2 siapp_drawTextFmt2f(siWindow* win, siText text, siVec2 pos, u32 size);

/* */
f32 siapp_drawCharacter(siWindow* win, const siFont* font, const siGlyphInfo* glyph,
        siVec2 pos, u32 size);
/* */
f32 siapp_drawCharacterScale(siWindow* win, const siFont* font, const siGlyphInfo* glyph,
        siVec2 pos, u32 size, f32 scaleFactor);
/* */
f32 siapp_drawCharacterItalicScale(siWindow* win, const siFont* font, const siGlyphInfo* glyph,
        siVec2 pos, u32 size, f32 scaleFactor);


/* Sets the current vertex color for the window based on the specified NDC RGB. */
void siapp_color3f(siWindow* win, f32 r, f32 g, f32 b);
/* Sets the current vertex color for the window based on the specified NDC RGBA. */
void siapp_color4f(siWindow* win, f32 r, f32 g, f32 b, f32 a);


/* Sets the current texture coordinates for the window based on the 2 NDC floats. */
void siapp_texCoords2f(siWindow* win, f32 x, f32 y);


/* Gets OpenGL information used for siliapp. */
siOpenGLInfo siapp_OpenGLGetInfo(void);

/* Initializes an OpenGL context of the specified window. Returns 'false' if the
 * context was failed to be created. */
b32 siapp_windowOpenGLInit(siWindow* win, siArea screenSize, u32 maxTexCount, siArea maxTexRes);
/* Renders the OpenGL graphics onto the screen. */
void siapp_windowOpenGLRender(siWindow* win);
/* Destroys the OpenGL context of the window. */
void siapp_windowOpenGLDestroy(siWindow* win);


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

#endif /* SIAPP_INCLUDE_SI_H */

#if !defined(MAIN)
#define SIAPP_IMPLEMENTATION
#endif

#if defined(SIAPP_IMPLEMENTATION)

#define SIGL_IMPLEMENTATION
#define WGL_ARB_create_context
#define WGL_ARB_create_context_profile
#define WGL_EXT_swap_control
#include <deps/sili/siligl.h>

#if 1 /* SETUP */

#define SK_TO_INT(sk) (*((u8*)&sk))
#define INT_TO_SK(num) (*((siKeyState*)&(u8){num}))

#if defined(SIAPP_PLATFORM_API_WIN32)

b32 SI_CLASS_INITIALIZED = false;

#define SI_LOWORD_XY(lw) ((i32)(i16)LOWORD(lw))
#define SI_HIWORD_XY(lw) ((i32)(i16)HIWORD(lw))


void __siapp_resizewindow(siWindow* win, i32 width, i32 height) {
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

F_TRAITS(inline)
siKeyType siapp_osKeyToSili(i32 key) {
    if (si_between(key, 'A', 'Z') || si_between(key, '0', '9')) {
        return key;
    }

    switch (key) {
        case VK_ESCAPE: return SK_ESC;

        case VK_LCONTROL: return SK_CTRL_L;
        case VK_LSHIFT:   return SK_SHIFT_L;
        case VK_LMENU:    return SK_ALT_L;
        case VK_LWIN:     return SK_SYSTEM_L;

        case VK_RCONTROL: return SK_CTRL_R;
        case VK_RSHIFT:   return SK_SHIFT_R;
        case VK_RMENU:    return SK_ALT_R;
        case VK_RWIN:     return SK_SYSTEM_R;

        case VK_CAPITAL: return SK_CAPS_LOCK;
        case VK_SCROLL:  return SK_SCROLL_LOCK;
        case VK_NUMLOCK: return SK_NUM_LOCK;

        case VK_PAUSE:      return SK_PAUSE;
        case VK_MENU:       return SK_MENU;
        case VK_OEM_4:      return SK_BRACKET_L;
        case VK_OEM_6:      return SK_BRACKET_R;
        case VK_OEM_1:      return SK_SEMICOLON;
        case VK_OEM_COMMA:  return SK_COMMA;
        case VK_OEM_PERIOD: return SK_PERIOD;
        case VK_OEM_7:      return SK_QUOTE;
        case VK_OEM_2:      return SK_SLASH;
        case VK_OEM_5:      return SK_BACKSLASH;
        case VK_OEM_3:      return SK_GRAVE;
        case VK_OEM_PLUS:   return SK_EQUALS;
        case VK_OEM_MINUS:  return SK_MINUS;

        case VK_SPACE:  return SK_SPACE;
        case VK_RETURN: return SK_RETURN;
        case VK_BACK:   return SK_BACKSPACE;
        case VK_TAB:    return SK_TAB;

        case VK_PRIOR:  return SK_PAGE_UP;
        case VK_NEXT:   return SK_PAGE_DOWN;
        case VK_END:    return SK_END;
        case VK_HOME:   return SK_HOME;
        case VK_INSERT: return SK_INSERT;
        case VK_DELETE: return SK_DELETE;
        case VK_CLEAR:  return SK_CLEAR;

        case VK_ADD:      return SK_PLUS;
        case VK_SUBTRACT: return SK_SUBTRACT;
        case VK_MULTIPLY: return SK_MULTIPLY;
        case VK_DIVIDE:   return SK_DIVIDE;

        case VK_LEFT:  return SK_LEFT;
        case VK_RIGHT: return SK_RIGHT;
        case VK_UP:    return SK_UP;
        case VK_DOWN:  return SK_DOWN;

        case VK_NUMPAD0:   return SK_NUMPAD_0;
        case VK_NUMPAD1:   return SK_NUMPAD_1;
        case VK_NUMPAD2:   return SK_NUMPAD_2;
        case VK_NUMPAD3:   return SK_NUMPAD_3;
        case VK_NUMPAD4:   return SK_NUMPAD_4;
        case VK_NUMPAD6:   return SK_NUMPAD_6;
        case VK_NUMPAD7:   return SK_NUMPAD_7;
        case VK_NUMPAD8:   return SK_NUMPAD_8;
        case VK_NUMPAD9:   return SK_NUMPAD_9;
        case VK_SEPARATOR: return SK_NUMPAD_ENTER;
        case VK_DECIMAL:   return SK_NUMPAD_DOT;

        case VK_F1:  return SK_F1;
        case VK_F2:  return SK_F2;
        case VK_F3:  return SK_F3;
        case VK_F4:  return SK_F4;
        case VK_F5:  return SK_F5;
        case VK_F6:  return SK_F6;
        case VK_F7:  return SK_F7;
        case VK_F8:  return SK_F8;
        case VK_F9:  return SK_F9;
        case VK_F10: return SK_F10;
        case VK_F11: return SK_F11;
        case VK_F12: return SK_F12;
        case VK_F13: return SK_F13;
        case VK_F14: return SK_F14;
        case VK_F15: return SK_F15;
    }

    return SK_UNKNOWN;
}

F_TRAITS(inline)
cstring siapp_osErrToStr(i32 error) {
    static char buf[128];

    DWORD len = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nil, error, 0, buf, sizeof(buf), nil
    );

    if (len == 0) {
        // The error code did not exist in the system errors.
        // Try getting message text from ntdsbmsg.

        siDllHandle handle = si_dllLoad("Ntdsbmsg.dll");

        len = FormatMessage(
            FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
            handle, error, 0, buf, sizeof(buf), nil
        );

        si_dllUnload(handle);
    }
    buf[len - 3] = '\0';

    return buf;
}


siSiliStr siapp_usernameGet(void) {
    static char buffer[sizeof(usize) + MAX_PATH + 1];
    siAllocator tmp = si_allocatorMakeTmp(buffer, sizeof(buffer));

    usize* len = si_mallocItem(&tmp, usize);
    u16 resultWide[MAX_PATH + 1];

    {
        DWORD wideLen = sizeof(buffer);
        b32 succeed = GetUserNameW(resultWide, &wideLen);
        SI_STOPIF(!succeed, return nil);
    }
    siSiliStr str = (siSiliStr)si_utf16ToUtf8Str(&tmp, resultWide, len);

    return str;
}


siSearchResult siapp_fileManagerOpen(siAllocator* alloc, siSearchConfig config) {
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

            if (res == 0) {
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
}


F_TRAITS(inline)
siSiliStr siapp_appDataPathMake(cstring folderName) {
    return siapp_appDataPathMakeEx(folderName, si_cstrLen(folderName));
}
siSiliStr siapp_appDataPathMakeEx(cstring folderName, usize folderNameLen) {
    static char buffer[sizeof(usize) + MAX_PATH + 1];
    siAllocator tmp = si_allocatorMakeTmp(buffer, sizeof(buffer));

    siSiliStr str;
    usize* len = si_mallocItem(&tmp, usize);
    u16 resultWide[MAX_PATH + 1];

    {
        i32 res = SHGetFolderPathW(nil, CSIDL_LOCAL_APPDATA, nil, 0, resultWide);
        SI_STOPIF(res != S_OK, return nil);
        str = (siSiliStr)si_utf16ToUtf8Str(&tmp, resultWide, len);
    }
    usize curLen = *len;

    str[curLen] = SI_PATH_SEPARATOR;
    memcpy(&str[curLen + 1], folderName, folderNameLen);
    str[curLen + 1 + folderNameLen] = SI_PATH_SEPARATOR;

    *len += 1 + folderNameLen + 1;

    return str;
}


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

#define SIAPP_ERROR_MSGBOX_GL(programID, title) \
    do { \
        GLint len; \
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len); \
        char* msg = (char*)malloc(len); \
        glGetProgramInfoLog(programID, len, nil, msg); \
        siapp_messageBox(title, msg, SI_MESSAGE_BOX_OK, SI_MESSAGE_BOX_ICON_ERROR); \
        free(msg); \
    } while (0)

BYTE __win32KBState[256];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    siWindow* win = (siWindow*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);

    if (msg == WM_CREATE) { // NOTE(bill): Doesn't need the platform
		// NOTE(bill): https://msdn.microsoft.com/en-us/library/windows/desktop/ms645536(v=vs.85).aspx
		RAWINPUTDEVICE rid[1] = {0};

		// NOTE(bill): Keyboard
		rid[0].usUsagePage = 0x01;
		rid[0].usUsage     = 0x06;
		rid[0].dwFlags     = RIDEV_NOLEGACY; // NOTE(bill): Do not generate legacy messages such as WM_KEYDOWN
		rid[0].hwndTarget  = hwnd;

		b32 res = RegisterRawInputDevices(rid, countof(rid), sizeof(rid[0]));
        SIAPP_ERROR_CHECK(res == false, "RegisterRawInputDevices");
	}

    if (win == nil) {
        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    switch (msg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            win->e.type.isClosed = true;
            return 0;
        }
        case WM_SIZE: {
            i32 width = SI_LOWORD_XY(lparam);
            i32 height = SI_HIWORD_XY(lparam);
            __siapp_resizewindow(win, width, height);
            break;
        }
        case WM_MOUSEMOVE: {
            i32 mouseX = SI_LOWORD_XY(lparam);
            i32 mouseY = SI_HIWORD_XY(lparam);

            win->e.type.mouseMove = true;
            win->e.mouse = SI_POINT(mouseX / win->scaleFactor.x, mouseY / win->scaleFactor.y);
            break;
        }
        case WM_MOUSEWHEEL: {
            //out->mousewheel_y = GET_WHEEL_DELTA_WPARAM(msg.wParam);
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN: {
            win->e.type.mousePress = true;
            break;
        }
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            win->e.type.mouseRelease = true;
            break;
        }
        case WM_INPUT: {
            RAWINPUT raw;
		    unsigned int size = sizeof(RAWINPUT);

            if (!GetRawInputData((HRAWINPUT)lparam, RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER))) {
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

            win->e.type.keyPress = win->e.type.keyPress || isDown;
            win->e.type.keyRelease = isUp;


            // NOTE(bill): Escaped sequences, turn vk into the correct scan code
            // except for VK_PAUSE (it's a bug)
            if (isE1) {
                if (vk == VK_PAUSE) {
                    scanCode = 0x45;
                }
                else {
                    scanCode = MapVirtualKeyW(vk, MAPVK_VK_TO_VSC);
                }

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
            siKeyState* keyState = &win->e.keys[key];
            keyState->clicked = (!keyState->pressed && isDown);
            keyState->pressed = isDown;
            keyState->released = isUp;

            if (keyState->clicked || isUp) {
                SK_TO_INT(win->e.keys[SK__EVENT]) |= SI_BIT(7);
                win->e.__private.keyCache[win->e.__private.keyCacheLen % 16] = key;
                win->e.__private.keyCacheLen += 1;
            }

            if (isDown && !isE1 && !isE0) {
                u16 buf[4];
                i32 numChars = ToUnicode(vk, scanCode, __win32KBState, buf, countof(buf) - 1, 0);
                SI_STOPIF(numChars == 0, break);

                usize len = win->e.charBufferLen;
                usize addedLen;

                siAllocator tmp = si_allocatorMakeTmp(&win->e.charBuffer[len], sizeof(win->e.charBuffer) - len);
                si_utf16ToUtf8Str(&tmp, buf, &addedLen);
                win->e.charBufferLen += addedLen;
            }
            break;
        }
    }
    return DefWindowProcW(hwnd, msg, wparam, lparam);
}
#endif

#if 1

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

#define i32ToNDCX(num, windowCord) (((f32)(num) / (f32)((windowCord) / 2.0f)) - 1.0f)
#define i32ToNDCY(num, windowCord) (((f32)(num) / (f32)((windowCord) / -2.0f)) + 1.0f)

#define GL_BUFFER_MAKE(ID, var, size) \
    do { \
        glBindBuffer(GL_ARRAY_BUFFER, gl->VBOs[ID]); \
        glBufferStorage(GL_ARRAY_BUFFER, size, nil, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT); \
        var = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT); \
    } while(0)

static siOpenGLInfo glInfo = {0};

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

void si_addVertexesToCMD(siWinRenderingCtxOpenGL* gl, u32 count, u32 vertexCount) {
    gl->batchInfo[2 * gl->drawCounter + 0] = gl->curTex->atlas->texID - 1;
    gl->batchInfo[2 * gl->drawCounter + 1] = gl->drawCounter;

    siOpenGLDrawCMD* cmd = &gl->CMDs[gl->drawCounter];
    cmd->count = count;
    cmd->instanceCount = 1;
    cmd->firstIndex = 0;
    cmd->baseVertex = gl->vertexCounter - vertexCount;
    cmd->baseInstance = gl->drawCounter;

    gl->drawCounter += 1;
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

siMatrix rglMatrixMultiply(float left[16], float right[16]) {
    return (siMatrix) {
        {
            left[0] * right[0] + left[1] * right[4] + left[2] * right[8] + left[3] * right[12],
            left[0] * right[1] + left[1] * right[5] + left[2] * right[9] + left[3] * right[13],
            left[0] * right[2] + left[1] * right[6] + left[2] * right[10] + left[3] * right[14],
            left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3] * right[15],
            left[4] * right[0] + left[5] * right[4] + left[6] * right[8] + left[7] * right[12],
            left[4] * right[1] + left[5] * right[5] + left[6] * right[9] + left[7] * right[13],
            left[4] * right[2] + left[5] * right[6] + left[6] * right[10] + left[7] * right[14],
            left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7] * right[15],
            left[8] * right[0] + left[9] * right[4] + left[10] * right[8] + left[11] * right[12],
            left[8] * right[1] + left[9] * right[5] + left[10] * right[9] + left[11] * right[13],
            left[8] * right[2] + left[9] * right[6] + left[10] * right[10] + left[11] * right[14],
            left[8] * right[3] + left[9] * right[7] + left[10] * right[11] + left[11] * right[15],
            left[12] * right[0] + left[13] * right[4] + left[14] * right[8] + left[15] * right[12],
            left[12] * right[1] + left[13] * right[5] + left[14] * right[9] + left[15] * right[13],
            left[12] * right[2] + left[13] * right[6] + left[14] * right[10] + left[15] * right[14],
            left[12] * right[3] + left[13] * right[7] + left[14] * right[11] + left[15] * right[15]
        }
    };
}

#ifndef PI
    #define PI 3.14159265358979323846f
#endif
#define DEG2RAD (PI/180.0f)

void rglOrtho(siMatrix* matrix, double left, double right, double bottom, double top, double znear, double zfar) {
	float rl = (float)(right - left);
	float tb = (float)(top - bottom);
	float fn = (float)(zfar - znear);

	float tx = -((float)(right + left) / rl);
	float ty = -((float)(top + bottom) / tb);
	float tz = -((float)(zfar + znear) / fn);

	float matOrtho[16] =
			{
				2.0f / rl,   0.0f,          0.0f,    	  0.0f,
				0.0f,        2.0 / tb,      0.0f,         0.0f,
				0.0f,        0.0f,          -2.0f / fn,   0.0f,
				tx,          ty,            tz,           1.0f
			};

    *matrix = rglMatrixMultiply(matrix->m, matOrtho);
}

void rglTranslatef(siWindow* win, float x, float y, float z) {
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    f32 matTranslation[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        x,    y,    z,    1.0f
    };

    gl->matrices[gl->drawCounter] = rglMatrixMultiply(matTranslation, gl->matrices[gl->drawCounter].m);
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
    siArea framebufferSize = size;
#if 0
    SI_WINDOW_FULLSCREEN      = SI_BIT(1),
    SI_WINDOW_BORDERLESS      = SI_BIT(2),
    SI_WINDOW_RESIZABLE       = SI_BIT(3),
    SI_WINDOW_MINIMIZED       = SI_BIT(4),
    SI_WINDOW_MAXIMIZED       = SI_BIT(5),
    SI_WINDOW_RENDERING_CPU   = SI_BIT(7),
    SI_WINDOW_RENDERING_OPENGL   = SI_BIT(8),
    SI_WINDOW_RENDERING_NULL  = SI_BIT(9),
#endif

#if defined(SIAPP_PLATFORM_API_WIN32)
    siAllocator* stack = si_allocatorMakeStack(MAX_PATH * sizeof(u16));
    siUtf16String utf16name = si_utf8ToUtf16Str(stack, name, nil);
	HINSTANCE inh = GetModuleHandleW(NULL);

    if (!SI_CLASS_INITIALIZED) {
        WNDCLASSW class = {0};
        class.style = CS_HREDRAW | CS_VREDRAW;
        class.lpszClassName = L"siliapp-class-win32";
        class.hInstance = inh;
        class.hCursor = LoadCursor(NULL, IDC_ARROW);
        class.lpfnWndProc = WindowProc;

        i32 err = RegisterClassW(&class);
        SIAPP_ERROR_CHECK(err == 0, "RegisterClassW");

        SI_CLASS_INITIALIZED = true;
    }

    DWORD windowStyle =
        WS_CAPTION | WS_SYSMENU | WS_BORDER |
        WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

    if (arg & SI_WINDOW_OPTIMAL_SIZE) {
        siArea area = siapp_screenGetCurrentSize();
        size.width = area.width / 2;
        size.height = area.height / 2;
    }
    if (arg & SI_WINDOW_CENTER) {
        siArea area = siapp_screenGetCurrentSize();
        pos.x = (area.width - size.width) / 2;
        pos.y = (area.height - size.height) / 2;
    }

    RECT adjustRect = {pos.x, pos.y, size.width, size.height};
	AdjustWindowRect(&adjustRect, windowStyle, 0);
    size.width = adjustRect.right;
    size.height = adjustRect.bottom;

	HWND hwnd = CreateWindowW(
        L"siliapp-class-win32",
        utf16name, windowStyle,
        pos.x, pos.y, size.width, size.height,
        nil, nil, inh, nil
    );
    SIAPP_ERROR_CHECK(hwnd == nil, "CreateWindowW");

	HDC hdc = GetDC(hwnd);

    SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)win);

    win->hwnd = hwnd;
    win->hdc = hdc;
#endif
    win->alloc = alloc;
    win->arg = arg;
    win->maxDrawCount = maxDrawCount;
    win->scaleFactor = SI_VEC2(1, 1);
    win->originalSize = size;
    win->cursor = SI_CURSOR_ARROW;
    win->e = (siWindowEvent){0};
    win->textColor.vec4 = SI_VEC4(1, 1, 1, 1);

    switch (arg & SI_WINDOW_RENDERING_BITS) {
        case SI_WINDOW_RENDERING_OPENGL: {
            siapp_windowOpenGLInit(win, framebufferSize, maxTexCount, maxTexRes);
            break;
        }
        case SI_WINDOW_RENDERING_CPU: {
            SI_PANIC_MSG("CPU rendering isn't supported as of now");
            break;
        }
        default: {
            win->atlas = siapp_textureAtlasMake(win, maxTexRes, maxTexCount + 1, SI_RESIZE_DEFAULT);
        }
    }

    siapp_windowShow(win, (arg & SI_WINDOW_HIDDEN) == 0);
    return win;
}
void siapp_windowUpdate(siWindow* win, b32 await) {
    SI_ASSERT_NOT_NULL(win);

#if 1
    siWindowEvent* out = &win->e;

    b32 keyPress = out->type.keyPress;
    out->type = (siWinEventType){0};
    out->type.keyPress = keyPress;
    if (SI_NUM_BIT_GET(SK_TO_INT(out->keys[SK__EVENT]), SI_BIT(7))) { // Keys.
        usize* keyCacheLen = &out->__private.keyCacheLen;
        siKeyType* keyCache = out->__private.keyCache;

        for_range (i, 0, *keyCacheLen) { // TODO(EimaMei): Optimize this like the mouse button clear (len might not even be required).
            siKeyState* s = &out->keys[keyCache[i]];
            SK_TO_INT(*s) &= ~0x5; // Clear the clicked and released state at once.
        }

        SK_TO_INT(out->keys[SK__EVENT]) &= ~SI_BIT(7);
        *keyCacheLen= 0;
    }
    if (SI_NUM_BIT_GET(SK_TO_INT(out->keys[SK__EVENT]), SI_BIT(6))) { // Mouse buttons.
        u64* mouseButtons = (u64*)out->mouseButtons;
        *mouseButtons &= ~0x505050505; // Clears the clicked and released states of every mouse button at once.
        SK_TO_INT(out->keys[SK__EVENT]) &= ~SI_BIT(6);
    }
#endif

    //if (win->keyListenerEnabled) {
    GetKeyState(VK_SHIFT);
    GetKeyState(VK_MENU);
    GetKeyboardState(__win32KBState);
    out->charBufferLen = 0;
    //}

    MSG msg = {0};
	while (true) {
		b32 isOkay = PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
		if (!isOkay) break;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
    }

}
void siapp_windowRender(siWindow* win) {
    SI_ASSERT_NOT_NULL(win);

    switch (win->arg & SI_WINDOW_RENDERING_BITS) {
        case SI_WINDOW_RENDERING_OPENGL: siapp_windowOpenGLRender(win); break;
        default: SI_PANIC();
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
        default: SI_PANIC();
    }
}
F_TRAITS(inline)
void siapp_windowSwapBuffers(const siWindow* win) {
    SI_ASSERT_NOT_NULL(win);

    switch (win->arg & SI_WINDOW_RENDERING_BITS) {
        case SI_WINDOW_RENDERING_OPENGL: {
            SwapBuffers(win->hdc);
            siapp_windowClear(win);
            break;
        }
        default: SI_PANIC();
    }
}
void siapp_windowClose(siWindow* win) {
    SI_ASSERT_NOT_NULL(win);
    switch (win->arg & SI_WINDOW_RENDERING_BITS) {
        case SI_WINDOW_RENDERING_OPENGL: siapp_windowOpenGLDestroy(win); break;
        default: SI_PANIC_MSG("Other rendering types aren't supported as of now.");
    }

    PostQuitMessage(0);
    ReleaseDC(win->hwnd, win->hdc);
    DestroyWindow(win->hwnd);
}

F_TRAITS(inline)
void siapp_windowShow(const siWindow* win, b32 show) {
    SI_ASSERT_NOT_NULL(win);
    ShowWindow(win->hwnd, show);
}


F_TRAITS(inline)
const siWindowEvent* siapp_windowEventGet(const siWindow* win) {
    SI_ASSERT_NOT_NULL(win);
    return &win->e;
}


F_TRAITS(inline)
siCursorType siapp_windowCursorGet(const siWindow* win) {
    SI_ASSERT_NOT_NULL(win);
    return win->cursor;
}
void siapp_windowCursorSet(siWindow* win, siCursorType cursor) {
    SI_ASSERT_NOT_NULL(win);
    SI_ASSERT(si_between(cursor, 0, SI_CURSOR_COUNT));

    LPCSTR cursorName = nil;
    switch (cursor) {
        case SI_CURSOR_ARROW: cursorName = IDC_ARROW; break;
        case SI_CURSOR_HAND: cursorName = IDC_HAND; break;
        case SI_CURSOR_DOUBLE_ARROW_HORIZONTAL: cursorName = IDC_SIZEWE; break;
        case SI_CURSOR_TEXT_SELECT: cursorName = IDC_IBEAM; break;
        default: SI_PANIC_MSG("Cursor type doesn't exist.");
    }
    win->cursor = cursor;

    SetCursor(LoadCursorA(nil, cursorName));
    SetClassLongPtr(win->hwnd, GCLP_HCURSOR, (LONG_PTR)cursorName);
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
siArea siapp_screenGetCurrentSize(void) {
    DEVMODEW mode = {0};
    EnumDisplaySettingsW(nil, ENUM_CURRENT_SETTINGS, &mode);
    return SI_AREA(mode.dmPelsWidth, mode.dmPelsHeight);
}
F_TRAITS(inline)
siArea siapp_screenGetAvailableResolution(usize index) {
    DEVMODEW mode = {0};
    i32 res = EnumDisplaySettingsW(nil, index, &mode);
    SI_STOPIF(res == 0, return SI_AREA(-1, -1));

    return SI_AREA(mode.dmPelsWidth, mode.dmPelsHeight);
}


siTextureAtlas* siapp_textureAtlasMake(const siWindow* win, siArea area, u32 maxTexCount,
        siTextureResizeEnum enumName) {
    SI_ASSERT_NOT_NULL(win);

    siTextureAtlas* atlas = si_mallocItem(win->alloc, siTextureAtlas);
    atlas->renderingType = win->arg & SI_WINDOW_RENDERING_BITS;
    atlas->curCount = 0;
    atlas->texWidth = area.width;
    atlas->texHeight = area.height;
    atlas->maxCount = maxTexCount;
    atlas->totalWidth = atlas->texWidth * atlas->maxCount;

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
    SI_ASSERT_MSG(
        width <= atlas->texWidth && height <= atlas->texHeight,
        "The image's size dimensions be less or equal to the texture atlas'."
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

            u32 curWidth = res.id * atlas->texWidth;
            glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                curWidth, 0,
                width, height, c,
                GL_UNSIGNED_BYTE,
                buffer
            );

            res.x1 = (f32)curWidth / atlas->totalWidth;
            res.x2 = res.x1 + (f32)width / atlas->totalWidth;
            res.y1 = 0;
            res.y2 = (f32)height / atlas->texHeight;

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
siFont siapp_fontLoad(siWindow* win, cstring path, i32 size, siGlyphSet* glyphs,
        usize arrayLen) {
    return siapp_fontLoadEx(win, path, size, glyphs, arrayLen, 0);
}


siFont siapp_fontLoadEx(siWindow* win, cstring path, i32 size, siGlyphSet* glyphs,
        usize arrayLen, u32 extraChars) {
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
    for_range (i, 0, arrayLen) {
        siGlyphSet set = glyphs[i];
        charCount += set.end - set.start + 1;
    }

    font.glyphs = si_arrayMakeReserve(win->alloc, sizeof(siGlyphInfo), charCount);
    font.sets = si_arrayMakeReserve(win->alloc, sizeof(siGlyphSetANDNIndex), arrayLen);
    SI_ARRAY_HEADER(font.glyphs)->len = charCount;
    SI_ARRAY_HEADER(font.sets)->len = arrayLen;
    //memset(font.glyphs, 0, charCount * sizeof(siGlyphInfo));

    {
        isize texSize = 128;
        while (true) {
            isize res = (charCount / (texSize / (f32)size)) * size;
            SI_STOPIF(res <= texSize, break);
            texSize *= 2;
        }

        // TODO(EimaMei): Merge.
        siTextureAtlas* atlas = siapp_textureAtlasMake(win, SI_AREA(texSize, texSize), 1, SI_RESIZE_DEFAULT);//texSize, texSize), texCount + extraTextures);
        font.sheet = siapp_spriteSheetLoadEx(atlas, nil, texSize, texSize, 4, SI_AREA(size, size));

        siapp_textureSwizzleMask(
            atlas, SI_SWIZZLE_RGBA,
            si_buf(i32, SI_SWIZZLE_VAL_1, SI_SWIZZLE_VAL_1, SI_SWIZZLE_VAL_1, SI_SWIZZLE_VAL_R)
        );
    }

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
    si_allocatorFree(tmpAlloc);

    return font;
}

F_TRAITS(inline)
f32 siapp_fontCalculateScaleFactor(siFont font, u32 textSize) {
    return (f32)textSize / font.size * 1.15f;
}
siGlyphInfo* siapp_fontGlyphFind(siFont* font, i32 codepoint) {
    SI_ASSERT(codepoint != SI_UTF8_INVALID);

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
siVec2 siapp_textGetArea2f(siText text, u32 size) {
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
            case SI_UTF8_INVALID: SI_PANIC_MSG("Invalid UTF-8 character."); break;
            case '\0': i = characterCount; continue;
            case ' ':
            case '\t':
            case '\r':
            case '\n': {
                chars[i] = -utf32.codepoint;
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
        switch (-index) {
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
void siapp_drawVertex2f(siWindow* win, f32 x, f32 y) {
    siapp_drawVertex3f(win, x, y, 0.0f);
}
F_TRAITS(inline)
void siapp_drawVertex3f(siWindow* win, f32 x, f32 y, f32 z) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    SI_ASSERT_MSG(gl->vertexCounter < 4 * win->maxDrawCount, "Not enough space is left inside the buffer.");

    gl->vertices[3 * gl->vertexCounter + 0] = x;
    gl->vertices[3 * gl->vertexCounter + 1] = y;
    gl->vertices[3 * gl->vertexCounter + 2] = z;

    memcpy(&gl->colors[4 * gl->vertexCounter], &gl->curColor, sizeof(gl->curColor));
    memcpy(&gl->texCoords[2 * gl->vertexCounter], &gl->curTexCoords, sizeof(gl->curTexCoords));

    gl->vertexCounter += 1;
}
void siapp_drawRect(siWindow* win, siRect rect, siColor color) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    if (gl->vertexCounter + 4 > 4 * win->maxDrawCount) {
        //si_print("We reached our end! Executing siapp_windowRender\n");
        siapp_windowRender(win);
    }

    f32 x1 = i32ToNDCX(rect.x, gl->size.width); // TODO(EimaMei): Optimize i32ToNDCX. SIMD MAYBE?
    f32 y1 = i32ToNDCY(rect.y, gl->size.height);
    f32 x2 = i32ToNDCX(rect.x + rect.width, gl->size.width);
    f32 y2 = i32ToNDCY(rect.y + rect.height, gl->size.height);


    siapp_color4f(win, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

    siapp_drawVertex2f(win, x1, y1);
    siapp_drawVertex2f(win, x2, y1);
    siapp_drawVertex2f(win, x2, y2);
    siapp_drawVertex2f(win, x1, y2);

    // NOTE(EimaMei): Check if this can be optimized with instances.
    si_addVertexesToCMD(gl, 6, 4);
}
void siapp_drawRect4f(siWindow* win, siVec4 rect, siColor color) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    if (gl->vertexCounter + 4 > 4 * win->maxDrawCount) {
        si_print("We reached our end! Executing siapp_windowRender\n");
        siapp_windowRender(win);
    }

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
    si_addVertexesToCMD(gl, 6, 4);
}
void siapp_drawImage(siWindow* win, siRect rect, siImage img) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    if (gl->vertexCounter + 4 > 4 * win->maxDrawCount) {
        si_print("We reached our end! Executing siapp_windowRender\n");
        siapp_windowRender(win);
    }

    f32 x1 = i32ToNDCX(rect.x, gl->size.width);
    f32 y1 = i32ToNDCY(rect.y, gl->size.height);
    f32 x2 = i32ToNDCX(rect.x + rect.width, gl->size.width);
    f32 y2 = i32ToNDCY(rect.y + rect.height, gl->size.height);

    siapp_color4f(win, 1.0f, 1.0f, 1.0f, 1.0f);
    gl->curTex = &img;

    siapp_texCoords2f(win, img.x1, img.y1); siapp_drawVertex2f(win, x1, y1);
    siapp_texCoords2f(win, img.x2, img.y1); siapp_drawVertex2f(win, x2, y1);
    siapp_texCoords2f(win, img.x2, img.y2); siapp_drawVertex2f(win, x2, y2);
    siapp_texCoords2f(win, img.x1, img.y2); siapp_drawVertex2f(win, x1, y2);
    siapp_texCoords2f(win, 0, 0);

    gl->curTex = &gl->defaultTex;

    // NOTE(EimaMei): Check if this can be optimized with instances.
    si_addVertexesToCMD(gl, 6, 4);
}
void siapp_drawTriangle(siWindow* win, siPoint pos, i32 size, siColor color) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;

    f32 x = i32ToNDCX(pos.x, gl->size.width);
    f32 y = i32ToNDCY(pos.y, gl->size.height);
    f32 width = i32ToNDCX(pos.x + size, gl->size.width);
    f32 height = i32ToNDCY(pos.y + size, gl->size.height);

    siapp_color4f(win, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    siapp_drawVertex2f(win, (x + width) / 2,  y);
    siapp_drawVertex2f(win, width, height);
    siapp_drawVertex2f(win, x, height);

    si_addVertexesToCMD(gl, 3, 3);
}
F_TRAITS(inline)
void siapp_drawCircle(siWindow* win, siPoint pos, i32 radius, siColor color) {
    siapp_drawCircleEx(win, pos, radius, color, 90);
}
void siapp_drawCircleEx(siWindow* win, siPoint pos, i32 radius, siColor color, u32 sides) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;

    radius /= 2;


    f32 x = (f32)radius / (f32)si_cast(i32, gl->size.width / 2);
    f32 x2 = i32ToNDCX(pos.x + radius, gl->size.width);
    f32 y = 0;
    f32 y2 = i32ToNDCY(pos.y + radius, gl->size.height);

    f32 theta = SI_TAU / (f32)sides;
	f32 cc = si_cos(theta);
	f32 s = si_sin(theta);

    siapp_color4f(win, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	for_range (i, 0, sides) {
		siapp_drawVertex2f(win, x + x2, y * 1.77 + y2);

		f32 t = x;
		x = cc * x - s * y;
		y = s * t + cc * y;
	}

    si_addVertexesToCMD(gl, sides + sides / 2, sides);
}

F_TRAITS(inline)
void siapp_drawText(siWindow* win, siText text, siPoint pos, u32 size) {
    siapp_drawText2f(win, text, SI_VEC2(pos.x, pos.y), size);
}
F_TRAITS(inline)
void siapp_drawTextItalic(siWindow* win, siText text, siPoint pos, u32 size) {
    siapp_drawTextItalic2f(win, text, SI_VEC2(pos.x, pos.y), size);
}
F_TRAITS(inline)
siVec2 siapp_drawTextFmt(siWindow* win, siText text, siPoint pos, u32 size) {
    return siapp_drawTextFmt2f(win, text, SI_VEC2(pos.x, pos.y), size);
}
void siapp_drawText2f(siWindow* win, siText text, siVec2 pos, u32 size) {
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
        f32 advance = siapp_drawCharacterScale(win, text.font, glyph, base, size, scaleFactor);
        base.x += advance;
    }
}
void siapp_drawTextItalic2f(siWindow* win, siText text, siVec2 pos, u32 size) {
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
siVec2 siapp_drawTextFmt2f(siWindow* win, siText text, siVec2 pos, u32 size) {
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
                        siapp_drawRect4f(
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
    if (gl->vertexCounter + 4 > 4 * win->maxDrawCount) {
        //si_print("We reached our end! Executing siapp_windowRender\n");
        siapp_windowRender(win);
    }

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
        siVec4 bgColor = win->textColor.vec4;
        siapp_color4f(win, bgColor.x, bgColor.y, bgColor.z, bgColor.w);
        gl->curTex = &img;

        siapp_texCoords2f(win, img.x1, img.y1); siapp_drawVertex2f(win, x1, y1);
        siapp_texCoords2f(win, img.x2, img.y1); siapp_drawVertex2f(win, x2, y1);
        siapp_texCoords2f(win, img.x2, img.y2); siapp_drawVertex2f(win, x2, y2);
        siapp_texCoords2f(win, img.x1, img.y2); siapp_drawVertex2f(win, x1, y2);
        siapp_texCoords2f(win, 0, 0);

        si_addVertexesToCMD(gl, 6, 4);
        gl->curTex = &gl->defaultTex;
    }

    return glyph->advanceX * scaleFactor;
}
f32 siapp_drawCharacterItalicScale(siWindow* win, const siFont* font, const siGlyphInfo* character,
        siVec2 pos, u32 size, f32 scaleFactor) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;

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

        si_addVertexesToCMD(gl, 6, 4);
        gl->curTex = &gl->defaultTex;
    }

    return (glyph->advanceX + shift) * scaleFactor;
}

F_TRAITS(inline)
void siapp_color3f(siWindow* win, f32 r, f32 g, f32 b) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    gl->curColor.x = r;
    gl->curColor.y = g;
    gl->curColor.z = b;
    gl->curColor.w = 1.0f;
}
F_TRAITS(inline)
void siapp_color4f(siWindow* win, f32 r, f32 g, f32 b, f32 a) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    gl->curColor.x = r;
    gl->curColor.y = g;
    gl->curColor.z = b;
    gl->curColor.w = a;
}

void siapp_texCoords2f(siWindow* win, f32 x, f32 y) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    gl->curTexCoords.x  = x;
    gl->curTexCoords.y = y;
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

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar* message, const void* userParam) {
    si_printf("SILI DEBUG: %s %s %i %s: '%s'\n",
              GetStringForEnum(source), GetStringForEnum(type), id, GetStringForEnum(severity), length, message);
}

b32 siapp_windowOpenGLInit(siWindow* win, siArea screenSize, u32 maxTexCount, siArea maxTexRes) {
    SI_ASSERT_NOT_NULL(win);
    siWinRenderingCtxOpenGL* gl = &win->render.opengl;
    gl->size = screenSize;

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
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glInfo.max2DTexSize);

        SI_STOPIF(glInfo.versionSelected.major == 0, glInfo.versionSelected.major = glInfo.versionMax.major);
        SI_STOPIF(glInfo.versionSelected.minor == 0, glInfo.versionSelected.minor = glInfo.versionMax.minor);
    }
#if 0
    SI_STOPIF(wglCreateContextAttribsARB == nil, goto LABEL_WIN32_WGL_END);
    {
        HGLRC oldRc = gl->context;

        i32 attribs[] = {
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            WGL_CONTEXT_MAJOR_VERSION_ARB, glInfo.versionSelected.major,
            WGL_CONTEXT_MINOR_VERSION_ARB, glInfo.versionSelected.minor,
            0
        };

        gl->context = wglCreateContextAttribsARB(win->hdc, NULL, attribs);
        SIAPP_ERROR_CHECK(gl->context == nil, "wglCreateContextAttribsARB");

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(oldRc);
        wglMakeCurrent(win->hdc, gl->context);
    }
    SI_GOTO_LABEL(LABEL_WIN32_WGL_END)
#endif
#endif

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
        si_snprintf(updatedVShader, countof(updatedVShader), defaultVShaderCode, win->maxDrawCount);

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
        SIAPP_ERROR_MSGBOX_GL(gl->programID, "Failed to create vertex shader");
        return false;
    }

    glUseProgram(gl->programID);

    glBindAttribLocation(gl->programID, SI_SHADER_POS, "pos");
    glBindAttribLocation(gl->programID, SI_SHADER_TEX, "tex");
    glBindAttribLocation(gl->programID, SI_SHADER_CLR, "clr");
    glBindAttribLocation(gl->programID, SI_SHADER_ID,  "info");


    GL_BUFFER_MAKE(SI_VBO_POS, gl->vertices,  sizeof(siVec3) * 4 * win->maxDrawCount);
    GL_BUFFER_MAKE(SI_VBO_CLR, gl->colors,    sizeof(siVec4) * 4 * win->maxDrawCount);
    GL_BUFFER_MAKE(SI_VBO_TEX, gl->texCoords, sizeof(siVec2) * 4 * win->maxDrawCount);
    GL_BUFFER_MAKE(SI_VBO_ID,  gl->batchInfo, (sizeof(u32) * 2) * win->maxDrawCount);

    gl->CMDs = si_mallocArray(win->alloc, siOpenGLDrawCMD, win->maxDrawCount);
    gl->matrices = si_mallocArray(win->alloc, siMatrix, win->maxDrawCount);
    gl->indices = si_mallocArray(win->alloc, u16, win->maxDrawCount * 6);

    usize k = 0;
    usize j;
    for (j = 0; j < (6 * win->maxDrawCount); j += 6) {
        gl->indices[j + 0] = k + 0;
        gl->indices[j + 1] = k + 1;
        gl->indices[j + 2] = k + 2;

        gl->indices[j + 3] = k + 0;
        gl->indices[j + 4] = k + 2;
        gl->indices[j + 5] = k + 3;

        k += 4;
    }

    for_range (i, 0, win->maxDrawCount) {
        gl->matrices[i] = rglMatrixIdentity();
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * 6 * win->maxDrawCount, gl->indices, GL_STATIC_DRAW);
    si_allocatorResetFrom(win->alloc, win->alloc->offset - sizeof(u16) * 2 * win->maxDrawCount);

    glUniformMatrix4fv(gl->uniformMvp, win->maxDrawCount, GL_FALSE, gl->matrices->m);

    gl->vertexCounter = 0;
    gl->drawCounter = 0;
    gl->bgColor = SI_VEC4(0, 0, 0, 0);
    gl->curTexCoords = SI_VEC2(0, 0);

    gl->uniformTexture = glGetUniformLocation(gl->programID, "textures");
    gl->uniformMvp = glGetUniformLocation(gl->programID, "mvp");

    win->atlas = siapp_textureAtlasMake(win, maxTexRes, maxTexCount + 1, SI_RESIZE_DEFAULT);
    gl->defaultTex = siapp_imageLoadEx(win->atlas, si_buf(siByte, 0xFF, 0xFF, 0xFF, 0xFF), 1, 1, 4);
    gl->curTex = &gl->defaultTex;
    siapp_windowClear(win);

    return true;
}
void siapp_windowOpenGLRender(siWindow* win) {
    SI_ASSERT_NOT_NULL(win);

    siWinRenderingCtxOpenGL* gl = &win->render.opengl;

    glUseProgram(gl->programID);
    glBindVertexArray(gl->VAO);
    //glUniformMatrix4fv(gl->uniformMvp, gl->drawCounter, GL_FALSE, gl->matrices->m);

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
    wglDeleteContext(win->render.opengl.context);
}


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
}

#endif /* SIAPP_IMPLEMENTATION */

#if defined(__cplusplus)
}
#endif

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
