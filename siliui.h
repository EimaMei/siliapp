#include <deps/sili.h>
#include <siliapp.h>


typedef SI_ENUM(u32, siAlignment)  {
    SI_ALIGNMENT_LEFT  = SI_BIT(0),
    SI_ALIGNMENT_CENTER = SI_BIT(1),
    SI_ALIGNMENT_RIGHT = SI_BIT(2),

    SI_ALIGNMENT_UP = SI_BIT(3),
    SI_ALIGNMENT_MIDDLE = SI_BIT(4),
    SI_ALIGNMENT_DOWN = SI_BIT(5),

    SI_ALIGNMENT_DEFAULT = SI_ALIGNMENT_CENTER | SI_ALIGNMENT_MIDDLE,

    SI_ALIGNMENT_BITS_HORIZONTAL = SI_ALIGNMENT_LEFT | SI_ALIGNMENT_CENTER | SI_ALIGNMENT_RIGHT,
    SI_ALIGNMENT_BITS_VERTICAL = SI_ALIGNMENT_UP | SI_ALIGNMENT_MIDDLE | SI_ALIGNMENT_DOWN,
    SI_ALIGNMENT_BITS_ALL = SI_ALIGNMENT_BITS_VERTICAL | SI_ALIGNMENT_BITS_HORIZONTAL
};

typedef struct {
    b8 hovered      : 1;
    b8 clicked      : 1;
    b8 pressed      : 1;
    b8 released     : 1;
    b8 exited       : 1;
} siButtonState;

/* NOTE(EimaMei): Terminology:
 * SI_BUTTON_<object>_<verb> - Change <object> when it <verb> to <value>.
 * INTERACTED - when the object was hovered over/pressed on.
*/
typedef SI_ENUM(u32, siButtonConfigType) {
    SI_BUTTON_CURSOR_INTERACTED = SI_BIT(0),

    SI_BUTTON_COLOR_HOVERED = SI_BIT(1),
    SI_BUTTON_OUTLINE_HOVERED = SI_BIT(2),

    SI_BUTTON_COLOR_PRESSED = SI_BIT(3),
    SI_BUTTON_OUTLINE_PRESSED = SI_BIT(4),

    SI_BUTTON_CONFIG_COUNT = 5,
    SI_BUTTON_CONFIG_HOVERED_BITS = SI_BUTTON_COLOR_HOVERED | SI_BUTTON_OUTLINE_HOVERED,
};

typedef struct {
    siButtonConfigType type;
    u32 value[SI_BUTTON_CONFIG_COUNT];
} siButtonConfig;

typedef SI_ENUM(i32, siShapeType) {
    SI_SHAPE_NULL = 0,
    SI_SHAPE_RECT,
    SI_SHAPE_CIRCLE,
    SI_SHAPE_IMAGE,

    SI_SHAPE_RECT_4F,
};

typedef union {
    siRect rect;
    siVec4 rect4f;
    struct {
        siPoint pos;
        i32 radius;
    } circle;
    siImage image;
    siText text;
} siShape;

typedef SI_ENUM(i32, siDrawCommandFeatures) {
    SI_FEATURE_OUTLINE  = SI_BIT(0),
    SI_FEATURE_TEXT     = SI_BIT(1),
    SI_FEATURE_IMAGE    = SI_BIT(2)
};

typedef struct {
    i32 size;
    siColor color;
} siOutline;

typedef struct {
    siShapeType type;
    siDrawCommandFeatures features;
    siShape shape;

    siColor color;
    siOutline outline;

    siText text;
    i32 textSize;

    siImage image;
    siArea imageSize;
} siDrawCommand;


typedef struct {
    siWindow* win;
    siDrawCommand cmd;
    struct {
        siColor color;
        siOutline outline;
    } ogValues;

    siButtonState state;

    siButtonConfig config;
    siButtonConfigType previousActive;

    siAlignment textAlignment;
    siPoint textPosPad;

    siAlignment imageAlignment;
    siPoint imagePosPad;
} siButton;

siVec2 siui_alignmentCalculateAreaEx(siArea largerArea, siArea alignedArea,
        siAlignment align, siPoint posPad);
siVec2 siui_alignmentCalculateRectEx(siRect largerArea, siArea alignedArea,
        siAlignment align, siPoint posPad);

F_TRAITS(inline)
siVec2 siui_alignmentCalculateArea(siArea largerArea, siArea alignedArea,
        siAlignment align) {
    return siui_alignmentCalculateAreaEx(largerArea, alignedArea, align, SI_POINT(0, 0));
}


F_TRAITS(inline)
siVec2 siui_alignmentCalculateRect(siRect largerArea, siArea alignedArea,
        siAlignment align) {
    return siui_alignmentCalculateRectEx(largerArea, alignedArea, align, SI_POINT(0, 0));
}

F_TRAITS(inline)
siVec2 siui_alignmentCalculateRectEx(siRect largerArea, siArea alignedArea,
        siAlignment align, siPoint posPad) {

    siVec2 pos = siui_alignmentCalculateAreaEx(
        SI_AREA(largerArea.width, largerArea.height),
        alignedArea,
        align,
        posPad
    );

    pos.x = si_max(pos.x + largerArea.x, largerArea.x + posPad.x);
    pos.y = si_max(pos.y + largerArea.y, largerArea.y + posPad.y);
    return pos;
}
siVec2 siui_alignmentCalculateArea2fEx(siVec2 largerArea, siVec2 alignedArea,
        siAlignment align, siPoint posPad);

F_TRAITS(inline)
siVec2 siui_alignmentCalculateRectExF(siVec4 largerArea, siVec2 alignedArea,
        siAlignment align, siPoint posPad) {

    siVec2 pos = siui_alignmentCalculateArea2fEx(
        SI_VEC2(largerArea.z, largerArea.w),
        alignedArea,
        align,
        posPad
    );

    pos.x = si_maxf(pos.x + largerArea.x, largerArea.x + posPad.x);
    pos.y = si_maxf(pos.y + largerArea.y, largerArea.y + posPad.y);
    return pos;
}

siVec2 siui_alignmentCalculateArea2fEx(siVec2 largerArea, siVec2 alignedArea,
        siAlignment align, siPoint posPad) {
    siVec2 pos = SI_VEC2(0, 0);

    switch (align & SI_ALIGNMENT_BITS_HORIZONTAL) {
        case SI_ALIGNMENT_LEFT: {
            pos.x = posPad.x;
            break;
        }
        case SI_ALIGNMENT_CENTER: {
            pos.x = (largerArea.x - alignedArea.x + posPad.x) / 2.0f;
            break;
        }
        case SI_ALIGNMENT_RIGHT: {
            pos.x = largerArea.x - alignedArea.x - posPad.x;
            break;
        }
    }

    switch (align & SI_ALIGNMENT_BITS_VERTICAL) {
        case SI_ALIGNMENT_UP: {
            pos.y = posPad.y;
            break;
        }
        case SI_ALIGNMENT_MIDDLE: {
            pos.y = (largerArea.y - alignedArea.y - posPad.y) / 2.0f;
            break;
        }
        case SI_ALIGNMENT_DOWN: {
            pos.y = largerArea.y - alignedArea.y - posPad.y;
            break;
        }
    }

    return pos;
}

siVec2 siui_alignmentCalculateAreaEx(siArea largerArea, siArea alignedArea,
        siAlignment align, siPoint posPad) {
    siVec2 pos = SI_VEC2(0, 0);

    switch (align & SI_ALIGNMENT_BITS_HORIZONTAL) {
        case SI_ALIGNMENT_LEFT: {
            pos.x = posPad.x;
            break;
        }
        case SI_ALIGNMENT_CENTER: {
            pos.x = (largerArea.width - alignedArea.width + posPad.x) / 2.0f;
            break;
        }
        case SI_ALIGNMENT_RIGHT: {
            pos.x = largerArea.width - alignedArea.width - posPad.x;
            break;
        }
    }

    switch (align & SI_ALIGNMENT_BITS_VERTICAL) {
        case SI_ALIGNMENT_UP: {
            pos.y = posPad.y;
            break;
        }
        case SI_ALIGNMENT_MIDDLE: {
            pos.y = (largerArea.height - alignedArea.height - posPad.y) / 2.0f;
            break;
        }
        case SI_ALIGNMENT_DOWN: {
            pos.y = largerArea.height - alignedArea.height - posPad.y;
            break;
        }
    }

    return pos;
}

F_TRAITS(inline)
b32 siapp_collideMouseRect(const siWindow* win, siRect rect) {
    SI_ASSERT_NOT_NULL(win);
    siPoint point = win->e.mouse;
    return (point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height);
}
F_TRAITS(inline)
b32 siapp_collideMouseRect4f(const siWindow* win, siVec4 rect) {
    SI_ASSERT_NOT_NULL(win);
    siPoint point = win->e.mouse;
    return (point.x >= rect.x && point.x <= rect.x + rect.z && point.y >= rect.y && point.y <= rect.y + rect.w);
}

void siui_drawTextFieldEx(siWindow* win, siRect rect, siColor rectColor, siText text,
        u32 textSize, siAlignment textAlignment, siPoint textBasePos);
void siui_drawTextField4fEx(siWindow* win, siVec4 rect, siColor rectColor, siText text,
        u32 textSize, siAlignment textAlignment, siPoint textBasePos);

F_TRAITS(inline)
void siui_drawTextField(siWindow* win, siRect rect, siColor rectColor, siText text,
        u32 textSize, siAlignment textAlignment) {
    siui_drawTextFieldEx(win, rect, rectColor, text, textSize, textAlignment, SI_POINT(0, 0));
}

F_TRAITS(inline)
void siui_drawTextField4f(siWindow* win, siVec4 rect, siColor rectColor, siText text,
        u32 textSize, siAlignment textAlignment) {
    siui_drawTextField4fEx(win, rect, rectColor, text, textSize, textAlignment, SI_POINT(0, 0));
}

void siui_drawTextFieldEx(siWindow* win, siRect rect, siColor rectColor, siText text,
        u32 textSize, siAlignment textAlignment, siPoint textBasePos) {
    siVec2 alignedArea = siapp_textGetAreaF(text, textSize);
    siVec2 pos = siui_alignmentCalculateArea2fEx(SI_VEC2(rect.width, rect.height), alignedArea, textAlignment, textBasePos);
    pos.x += rect.x;
    pos.y += rect.y;

    siapp_drawRect(win, rect, rectColor);
    siapp_drawTextF(win, text, pos, textSize);
}

void siui_drawTextField4fEx(siWindow* win, siVec4 rect, siColor rectColor, siText text,
        u32 textSize, siAlignment textAlignment, siPoint textBasePos) {
    siVec2 alignedArea = siapp_textGetAreaF(text, textSize);
    siVec2 pos = siui_alignmentCalculateRectExF(rect, alignedArea, textAlignment, textBasePos);

    siapp_drawRectF(win, rect, rectColor);
    siapp_drawTextF(win, text, pos, textSize);
}

F_TRAITS(inline)
siButton siui_buttonMakeRect(siWindow* win, siRect rect, siColor color) {
    siButton button = {0};
    button.win = win;
    button.cmd.type = SI_SHAPE_RECT;
    button.cmd.color = color;
    button.cmd.shape.rect = rect;
    button.ogValues.color = color;

    return button;
}

F_TRAITS(inline)
siButton siui_buttonMakeRect4f(siWindow* win, siVec4 rect, siColor color) {
    siButton button = {0};
    button.win = win;
    button.cmd.type = SI_SHAPE_RECT_4F;
    button.cmd.color = color;
    button.cmd.shape.rect4f = rect;
    button.ogValues.color = color;

    return button;
}

F_TRAITS(inline)
void siui_buttonOutlineSet(siButton* button, i32 size, siColor color) {
    SI_ASSERT_NOT_NULL(button);
    button->cmd.features |= SI_FEATURE_OUTLINE;
    button->cmd.outline = (siOutline){size, color};
    button->ogValues.outline = button->cmd.outline;
}

void siui_drawButton(siButton button) {
    siDrawCommand* cmd = &button.cmd;
    siVec2 imagePos;

    if (button.cmd.features & SI_FEATURE_IMAGE) {
        imagePos = siui_alignmentCalculateRectEx(
            cmd->shape.rect,
            cmd->imageSize,
            button.imageAlignment,
            button.imagePosPad
        );

        button.textPosPad.x += cmd->imageSize.width + button.imagePosPad.x;
    }

    switch (button.cmd.type) {
        case SI_SHAPE_RECT: {
            siRect rect = cmd->shape.rect;
            if (button.cmd.features & SI_FEATURE_OUTLINE) {
                siapp_drawRect(button.win, rect, cmd->outline.color);
                rect.x += cmd->outline.size;
                rect.y += cmd->outline.size;
                rect.width -= cmd->outline.size * 2;
                rect.height -= cmd->outline.size * 2;
            }

            if (button.cmd.features & SI_FEATURE_TEXT) {
                siui_drawTextFieldEx(
                    button.win,
                    rect, cmd->color,
                    cmd->text, cmd->textSize, button.textAlignment,
                    button.textPosPad
                );
            }
            else {
                siapp_drawRect(button.win, rect, cmd->color);
            }
            break;
        }
        case SI_SHAPE_RECT_4F: {
            siVec4 rect = cmd->shape.rect4f;
            if (button.cmd.features & SI_FEATURE_OUTLINE) {
                siapp_drawRectF(button.win, rect, cmd->outline.color);
                rect.x += cmd->outline.size;
                rect.y += cmd->outline.size;
                rect.z -= cmd->outline.size * 2;
                rect.w -= cmd->outline.size * 2;
            }

            if (button.cmd.features & SI_FEATURE_TEXT) {
                siui_drawTextField4fEx(
                    button.win,
                    rect, cmd->color,
                    cmd->text, cmd->textSize,
                    button.textAlignment, button.textPosPad
                );
            }
            else {
                siapp_drawRectF(button.win, rect, cmd->color);
            }
            break;
        }
    }

    if (button.cmd.features & SI_FEATURE_IMAGE) {
        siapp_drawImage(button.win, SI_RECT_PA(imagePos, cmd->imageSize), cmd->image);
    }
}

#define SIUI_CLR_TO_INT(clr) (*((u32*)&clr))
#define SIUI_INT_TO_CLR(num) (*((siColor*)&(u32){num}))


void siui_buttonUpdate(siButton* button) {
    siWindow* win = button->win;
    siButtonState* state = &button->state;
    siWindowEvent* e = &win->e;
    siDrawCommand* cmd = &button->cmd;

    b32 oldPressed = state->pressed;
    b32 oldHovered = state->hovered;

    switch (button->cmd.type) {
        case SI_SHAPE_RECT: {
            state->hovered = siapp_collideMouseRect(win, button->cmd.shape.rect);
            break;
        }
        case SI_SHAPE_RECT_4F: {
            state->hovered = siapp_collideMouseRect4f(win, button->cmd.shape.rect4f);
            break;
        }
        default: SI_PANIC();
    }

    state->clicked  = (state->hovered && e->type.mousePress);
    state->released = (state->pressed && e->type.mouseRelease);
    state->pressed  = (state->clicked || (!state->released && oldPressed));
    state->exited   = (!state->pressed && oldPressed) || (!oldPressed && !state->hovered && oldHovered);

    SI_STOPIF(button->config.type == 0, return);
    #define STATE(value) if ((button->config.type & value) != 0)

    siButtonConfigType active = 0;
    if (state->hovered) {
        STATE(SI_BUTTON_CURSOR_INTERACTED) {
            SI_STOPIF(button->state.pressed, goto end0);

            u32 cursor = button->config.value[0];
            u32 curCursor = siapp_windowCursorGet(win);
            if (cursor != curCursor) {
                siapp_windowCursorSet(win, cursor);
            }
            active |= SI_BUTTON_CURSOR_INTERACTED;

            SI_GOTO_LABEL(end0);
        }

        STATE(SI_BUTTON_COLOR_HOVERED) {
            u32 newClr = button->config.value[1];
            button->cmd.color = SIUI_INT_TO_CLR(newClr);
            active |= SI_BUTTON_COLOR_HOVERED;
        }

        STATE(SI_BUTTON_OUTLINE_HOVERED) {
            u32 newClr = button->config.value[2];
            cmd->outline.color = SIUI_INT_TO_CLR(newClr);
            active |= SI_BUTTON_OUTLINE_HOVERED;
        }
    }
    else {
        if (button->previousActive & SI_BUTTON_COLOR_HOVERED) {
            button->cmd.color = button->ogValues.color;
        }

        if (button->previousActive & SI_BUTTON_OUTLINE_HOVERED) {
            button->cmd.outline.color = button->ogValues.outline.color;
        }
    }
    if (state->pressed) {
        if (button->config.type & SI_BUTTON_CURSOR_INTERACTED  && !button->state.hovered) {
            u32 cursor = button->config.value[0];
            u32 curCursor = siapp_windowCursorGet(win);
            if (cursor != curCursor) {
                siapp_windowCursorSet(win, cursor);
            }
        }

        if (button->config.type & SI_BUTTON_COLOR_PRESSED) {
            u32 newClr = button->config.value[3];
            button->cmd.color = SIUI_INT_TO_CLR(newClr);
            active |= SI_BUTTON_COLOR_PRESSED;
        }

        if (button->config.type & SI_BUTTON_OUTLINE_PRESSED) {
            u32 newClr = button->config.value[4];
            cmd->outline.color = SIUI_INT_TO_CLR(newClr);
            active |= SI_BUTTON_OUTLINE_PRESSED;
        }
    }
    else {
        if (button->previousActive & SI_BUTTON_COLOR_PRESSED) {
            button->cmd.color = button->ogValues.color;
        }

        if (button->previousActive & SI_BUTTON_OUTLINE_PRESSED) {
            button->cmd.outline.color = button->ogValues.outline.color;
        }
    }

    if (state->exited) {
        if (button->config.type & SI_BUTTON_CURSOR_INTERACTED) {
            SI_STOPIF(button->state.pressed || (button->state.released && button->state.hovered), goto end1);

            u32 curCursor = siapp_windowCursorGet(win);
            if (curCursor != SI_CURSOR_ARROW) {
                siapp_windowCursorSet(win, SI_CURSOR_ARROW);
            }

            SI_GOTO_LABEL(end1);
        }
    }
    button->previousActive = active;
    #undef STATE
}

#define siui_buttonConfigSet(buttonPtr, type, value) \
    siui_buttonConfigSetEx(buttonPtr, type, si_anyMake(value))

void siui_buttonConfigSetEx(siButton* button, siButtonConfigType type, siAny value) {
    SI_ASSERT_NOT_NULL(button);
    SI_ASSERT_MSG(value.typeSize <= 4, "The type for value is too large (maximum limit is 4 bytes).");
    SI_ASSERT_MSG(si_between(type, SI_BIT(0), SI_BIT(30)), "Invalid type.");
    button->config.type |= type;

    usize index = 0; // TODO(EimaMei): Make this faster like si_pow10.
    while (type > 1) {
        type >>= 1;
        index += 1;
    }

    memcpy(&button->config.value[index], value.ptr, value.typeSize);
}

void siui_buttonTextSetEx(siButton* button, siText text, i32 textSize, siAlignment align,
        siPoint textPosPad);

F_TRAITS(inline)
void siui_buttonTextSet(siButton* button, siText text, i32 textSize, siAlignment align) {
    siui_buttonTextSetEx(button, text, textSize, align, SI_POINT(0, 0));
}

F_TRAITS(inline)
void siui_buttonTextSetEx(siButton* button, siText text, i32 textSize, siAlignment align,
        siPoint textPosPad) {
    SI_ASSERT_NOT_NULL(button);
    button->cmd.features |= SI_FEATURE_TEXT;
    button->cmd.text = text;
    button->cmd.textSize = textSize;

    button->textAlignment = align;
    button->textPosPad = textPosPad;
}

F_TRAITS(inline)
void siui_buttonImageSetEx(siButton* button, siImage image, siArea size, siAlignment align,
        siPoint imagePosPad) {
    SI_ASSERT_NOT_NULL(button);
    button->cmd.features |= SI_FEATURE_IMAGE;
    button->cmd.image = image;
    button->cmd.imageSize = size;

    button->imageAlignment = align;
    button->imagePosPad = imagePosPad;
}


typedef siButton siExpandable;

typedef SI_ENUM(b32, siSide) {
    SI_SIDE_LEFT = SI_BIT(0),
    SI_SIDE_UP = SI_BIT(1),
    SI_SIDE_RIGHT = SI_BIT(2),
    SI_SIDE_DOWN = SI_BIT(3)
};

F_TRAITS(inline)
i32 siui_shapeGetWidth(siShape shape, siShapeType type) {
    switch (type) {
        case SI_SHAPE_RECT_4F: return shape.rect4f.z;
        default: SI_PANIC();
    }
    return 0.0f;
}

F_TRAITS(inline)
f32 siui_shapeGetWidthF(siShape shape, siShapeType type) {
    switch (type) {
        case SI_SHAPE_RECT_4F: return shape.rect4f.z;
        default: SI_PANIC();
    }
    return 0.0f;
}

siExpandable siui_expandableMakeRect4f(siWindow* win, i32 expandableSize, siSide side) {
    siVec4 collideRect = {0};
    if (side & SI_SIDE_RIGHT) {
        collideRect = SI_VEC4(
            0, 0,
            expandableSize,
            0
        );
    }
    else SI_PANIC();

    siExpandable expand = siui_buttonMakeRect4f(win, collideRect, SI_RGBA(0, 0, 0, 0));
    siui_buttonConfigSet(&expand, SI_BUTTON_CURSOR_INTERACTED, SI_CURSOR_DOUBLE_ARROW_HORIZONTAL);

    return expand;
}

b32 siui_expandableUpdate(siExpandable* expand, siRect* srcOut) {
    SI_ASSERT_NOT_NULL(expand);
    SI_ASSERT_NOT_NULL(srcOut);

    siRect* expandRect = &expand->cmd.shape.rect;
    expandRect->x = srcOut->x + srcOut->width - siui_shapeGetWidth(expand->cmd.shape, expand->cmd.type);
    expandRect->y = srcOut->y;
    expandRect->height = srcOut->height;

    siui_buttonUpdate(expand);

    if (expand->state.pressed) {
        siPoint mouse = expand->win->e.mouse;
        SI_STOPIF(mouse.x <= srcOut->x || mouse.x <= 0, return false);

        expandRect->x = mouse.x;
        srcOut->width = mouse.x - srcOut->x;
        return true;
    }
    return false;
}

b32 siui_expandableUpdate4f(siExpandable* expand, siVec4* srcOut) {
    SI_ASSERT_NOT_NULL(expand);
    SI_ASSERT_NOT_NULL(srcOut);

    siVec4* expandRect = &expand->cmd.shape.rect4f;
    expandRect->x = srcOut->x + srcOut->z - expandRect->z;
    expandRect->y = srcOut->y;
    expandRect->w = srcOut->w;

    siui_drawButton(*expand);
    siui_buttonUpdate(expand);

    if (expand->state.pressed) {
        siPoint mouse = expand->win->e.mouse;
        SI_STOPIF(mouse.x <= srcOut->x || mouse.x <= 0, return false);

        expandRect->x = mouse.x;
        srcOut->z = mouse.x - srcOut->x;
        return true;
    }
    return false;
}

void siui_buttonRectSet(siButton* button, siRect rect) {
    SI_ASSERT_NOT_NULL(button);
    switch (button->cmd.type) {
        case SI_SHAPE_RECT: button->cmd.shape.rect = rect; break;
        default: SI_PANIC();
    }
}

typedef struct {
    siButton button;
    b32 selected;
    i32 curIndex;
    u64 clockStart;
    siVec2 cursor;
} siTextInput;

F_TRAITS(inline)
siTextInput siui_textInputMake(siAllocator* alloc, siWindow* win, siRect rect,
        siColor color, siFont* font, u32 maxChars) {
    siButton button = siui_buttonMakeRect(win, rect, color);
    siText text = siapp_textReserve(alloc, font, maxChars);
    siui_buttonTextSet(&button, text, 6, 0);
    siui_buttonConfigSet(&button, SI_BUTTON_CURSOR_INTERACTED, SI_CURSOR_TEXT_SELECT);

    siTextInput inp;
    inp.selected = false;
    inp.button = button;
    inp.curIndex = 0;
    inp.cursor = SI_VEC2(0, 0);
    inp.clockStart = 0;

    return inp;
}

siSiliStr siui_textInputStringGet(siAllocator* alloc, siTextInput t) {
    SI_ASSERT_NOT_NULL(alloc);
    siText text = t.button.cmd.text;

    usize* strLen = si_mallocCopy(alloc, &text.len);
    siSiliStr str = (siSiliStr)strLen + sizeof(usize);

    for_range (i, 0, text.len) {
        i32 codepoint = siapp_textCodepointGet(text, i);

        siUtf8Char utf8 = si_utf8Encode(codepoint);
        char* res = si_mallocArray(alloc, char, utf8.len);
        memcpy(res, utf8.codepoint, utf8.len);
    }

    char* res = si_mallocItem(alloc, char);
    *res = '\0';

    return str;
}

b32 siui_textInputStringSetEx(siTextInput* t, cstring str, usize characterCount);

F_TRAITS(inline)
b32 siui_textInputStringSet(siTextInput* t, cstring str) {
    return siui_textInputStringSetEx(t, str, si_utf8StrLen(str));
}

b32 siui_textInputStringSetEx(siTextInput* t, cstring str, usize characterCount) {
    SI_ASSERT_NOT_NULL(str);
    SI_ASSERT_NOT_NULL(t);

    siText* text = &t->button.cmd.text;
    SI_STOPIF(characterCount > text->capacity, return false);

    cstring ptr = str;
    for_range (i, 0, characterCount) {
        siUtf32Char utf32 = si_utf8Decode(ptr);
        siapp_textCharacterSet(text, i, utf32.codepoint);
        ptr += utf32.len;
    }
    text->len = characterCount;

    return true;
}

void siui_drawTextInputEx(siTextInput* t, i32 textSize, siAlignment align);

F_TRAITS(inline)
void siui_drawTextInput(siTextInput* t, i32 textSize) {
    siui_drawTextInputEx(t, textSize, SI_ALIGNMENT_LEFT);
}

F_TRAITS(inline)
void siui_textInputRectSet(siTextInput* t, siRect rect) {
    SI_ASSERT_NOT_NULL(t);
    t->button.cmd.shape.rect = rect;
}

void siui_drawTextInputEx(siTextInput* t, i32 textSize, siAlignment align) {
    //siTimeStamp ts = si_timeStampStart();

    SI_ASSERT_NOT_NULL(t);
    siText* text = &t->button.cmd.text;
    siWindowEvent* e = &t->button.win->e;

    t->button.cmd.textSize = textSize;
    t->button.textAlignment = align;

    siui_drawButton(t->button);
    siui_buttonUpdate(&t->button);

    if (t->button.state.clicked) {
        t->selected = true;
        t->clockStart = si_clock();
    }
    else if (e->type.mousePress) {
        t->selected = false;
    }
    SI_STOPIF(!t->selected, return);

    u64 end = si_clock(); // TODO(EimaMei): Optimize this out for timeDelta in the window event.
    u64 delta = (end - t->clockStart) / SI_CLOCKS_PER_MILISECOND;

    if (delta >= 500) {
        siRect* buttonR = &t->button.cmd.shape.rect;
        u32 outline = t->button.cmd.outline.size;

        siVec4 base;
        base.x = buttonR->x + outline + t->cursor.x;
        base.y = buttonR->y + outline + t->cursor.y;
        base.z = 0.25;
        base.w = buttonR->height - outline * 2;

        siapp_drawRectF(t->button.win, base, SI_RGB(255, 255, 255));
        SI_STOPIF(delta >= 1000, t->clockStart = end);
    }


    if (e->charBufferLen != 0 && (text->len < text->capacity)) {
        usize offset = 0;
        f32 scaleFactor = (f32)textSize / text->font->size * 1.15;

        while (offset < e->charBufferLen) {
            siUtf32Char utf32 = si_utf8Decode(&e->charBuffer[offset]);
            offset += utf32.len;

            if (utf32.codepoint == '\b') {
                SI_STOPIF(t->curIndex == 0, continue);
                t->curIndex -= 1;
            }
            siapp_textCharacterSet(text, t->curIndex, utf32.codepoint);

            switch (utf32.codepoint) {
                case '\r': {
                    t->cursor.x = 0;
                    t->cursor.y += text->font->advance.newline * scaleFactor;
                    text->len += 1;
                    break;
                }
                case '\b': {
                    t->cursor.x = text->curX * scaleFactor;
                    t->cursor.y = (text->totalArea.y - text->font->size) * scaleFactor;
                    break;
                }
                default: {
                    f32 advanceX = siapp_textAdvanceXGet(*text, t->curIndex);
                    t->cursor.x += advanceX * scaleFactor;
                    t->curIndex += 1;
                    break;
                }
            }
        }
    }

    //si_timeStampPrintSince(ts);
}
