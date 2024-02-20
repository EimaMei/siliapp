#define SIAPP_IMPLEMENTATION
#include <siliapp.h>


// TODO: Do all TODOs.
// TODO: stb_tt unrequireent
// TODO: Make more performance benchmarks (RSGL, SDL2, legacy, etc)
// TODO: Fix instance rendering
// TODO: Add ortho and scale
// TODO: Fix matrices and make them actually good
// TODO: Make the cool siButton stuff a separate thing compared to siliapp.h
// TODO: Opengl frame time and gpu tiem and frame time?
// TODO: Add some macros from sili.h to siliapp.h and siligl.h


#include <shellapi.h>
#include <ole2.h>

typedef struct {
	IDataObject ido;
	int ref_count;
	FORMATETC *m_pFormatEtc;
	STGMEDIUM *m_pStgMedium;
	LONG	   m_nNumFormats;
	LONG m_lRefCount;
} WB_IDataObject;


typedef struct WB_IDropTargetVtbl
{
	BEGIN_INTERFACE

		HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IDropTarget_QueryInterface )(
				IDropTarget __RPC_FAR * target,
				/* [in] */ REFIID riid,
				/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
			IDropTarget __RPC_FAR * target);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
			IDropTarget __RPC_FAR * target);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DragEnter )(
			IDropTarget __RPC_FAR * target,
			/* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
			/* [in] */ DWORD grfKeyState,
			/* [in] */ POINTL pt,
			/* [out][in] */ DWORD __RPC_FAR *pdwEffect);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DragOver )(
			IDropTarget __RPC_FAR * target,
			/* [in] */ DWORD grfKeyState,
			/* [in] */ POINTL pt,
			/* [out][in] */ DWORD __RPC_FAR *pdwEffect);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DragLeave )(
			IDropTarget __RPC_FAR * target);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Drop )(
			IDropTarget __RPC_FAR * target,
			/* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
			/* [in] */ DWORD grfKeyState,
			/* [in] */ POINTL pt,
			/* [out][in] */ DWORD __RPC_FAR *pdwEffect);

	END_INTERFACE
} WB_IDropTargetVtbl;

F_TRAITS(inline)
void siapp_windowFileDragEnable(siWindow* win, b32 enable) {
	SI_ASSERT_NOT_NULL(win);
	DragAcceptFiles(win->hwnd, enable);
}

typedef SI_ENUM(i32, siDropState) {
	SI_DRAG_ENTER = 1,
	SI_DRAG_OVER,
	SI_DRAG_LEAVE,
	SI_DRAG_DROP
};

typedef struct {
	IDropTarget target;
	IDataObject* pDataObj;
	siWindow* win;
	HWND subHwnd;

	siDropState state;
} siDropEvent;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProcDropFile(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass,
		DWORD_PTR dwRefData) {
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



F_TRAITS(inline)
u32 siapp__internUpdateDropPress(IDropTarget* target, DWORD grfKeyState, POINTL pt,
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

F_TRAITS(inline)
void siapp__internUpdateDropRelease(IDropTarget* target, DWORD grfKeyState, POINTL pt,
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


ULONG IDropTarget_AddRef(IDropTarget* target) { return 1; SI_UNUSED(target); }
ULONG IDropTarget_Release(IDropTarget* target) { return 0; SI_UNUSED(target); }

HRESULT IDropTarget_QueryInterface(IDropTarget *target, REFIID riid, LPVOID* ppvObject) {
	*ppvObject = target;
	return S_OK;
	SI_UNUSED(riid);
}


HRESULT IDropTarget_DragEnter(IDropTarget* target, IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
	u32 effect = siapp__internUpdateDropPress(target, grfKeyState, pt, SI_DRAG_ENTER);
	*pdwEffect &= effect;

	siDropEvent* drop = (siDropEvent*)target;
	drop->pDataObj = pDataObj;

	return S_OK;
}

HRESULT IDropTarget_DragOver(IDropTarget* target, DWORD grfKeyState,
		POINTL pt, DWORD* pdwEffect) {
	siDropEvent* drop = (siDropEvent*)target;
	SI_STOPIF(drop->state == SI_DRAG_ENTER, return S_OK);

	u32 effect = siapp__internUpdateDropPress(target, grfKeyState, pt, SI_DRAG_OVER);
	*pdwEffect &= effect;
	return S_OK;
}
STDMETHODCALLTYPE
HRESULT IDropTarget_DragLeave(IDropTarget* target) {
	siDropEvent* drop = (siDropEvent*)target;
	drop->state = SI_DRAG_LEAVE;
	return S_OK;
}

HRESULT IDropTarget_Drop(IDropTarget* target, IDataObject* pDataObj,
		DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
	siapp__internUpdateDropRelease(target, grfKeyState, pt, SI_DRAG_DROP);
	*pdwEffect = DROPEFFECT_NONE;

	siDropEvent* drop = (siDropEvent*)target;
	drop->pDataObj = pDataObj;

	return S_OK;
}


void siapp_windowDragAreaMake(siWindow* win, siRect rect, siDropEvent* out) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT_NOT_NULL(out);

	u16 buf[64];
	GetClassNameW(win->hwnd, buf, countof(buf));
	si_printf("%ls\n", buf);

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
	static WB_IDropTargetVtbl vTable = {
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

	OleInitialize(0);
	RegisterDragDrop(subHwnd, (LPDROPTARGET)&out->target);
}

F_TRAITS(inline)
void siapp_windowDragAreaEnd(siDropEvent event) {
	RevokeDragDrop(event.subHwnd);
	RemoveWindowSubclass(event.subHwnd, &WndProcDropFile, 0);
	OleUninitialize();
}

F_TRAITS(inline)
void siapp_dropEventEnd(siDropEvent* event) {
	event->state = 0;
}

typedef struct {
	/* Actual length of the path. */
	usize len;
	/* */
	char path[256];
} siDropEntry;

typedef struct {
	/* */
	u32 len;
	/* */
	u32 curIndex;
	/* */
	STGMEDIUM stgm;
} siDropHandle;

siDropHandle siapp_dropEventHandle(siDropEvent event) {
	FORMATETC fmte = {CF_HDROP, nil, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	STGMEDIUM stgm;
	IDataObject* pDataObj = event.pDataObj;

	siDropHandle res = {0};

	b32 err = pDataObj->lpVtbl->GetData(pDataObj, &fmte, &stgm) == S_OK;
	SI_STOPIF(!err, goto end);

	HDROP hdrop = (HDROP)stgm.hGlobal;
	res.stgm = stgm;
	res.len = DragQueryFileW(hdrop, 0xFFFFFFFF, NULL, 0);

end:
	return res;
}

b32 siapp_dropEventPollEntry(siDropHandle* handle, siDropEntry* entry) {
	if (handle->curIndex >= handle->len) {
		ReleaseStgMedium(&handle->stgm);
		handle->curIndex = 0;
		return false;
	}

	siAllocator out = si_allocatorMakeTmp(entry->path, 256);
	siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
	u16* curPtr = (u16*)si_allocatorCurPtr(stack);

	DragQueryFileW(handle->stgm.hGlobal, handle->curIndex, curPtr, 256);
	si_utf16ToUtf8Str(&out, curPtr, &entry->len);
	handle->curIndex += 1;

	return true;
}



#define CURSOR_W 28
#define CURSOR_H 32


#define i32ToNDCX(num, windowCord) (((f32)(num) / (f32)((windowCord) / 2.0f)) - 1.0f)
#define i32ToNDCY(num, windowCord) (((f32)(num) / (f32)((windowCord) / -2.0f)) + 1.0f)

void siapp_drawTriangleF(siWindow* win, siTriangleF triangle, siColor color);


F_TRAITS(inline)
void siapp_windowGradientSet(siWindow* win, siColor gradient[], usize len) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT_NOT_NULL(gradient);

	siWinRenderingCtxOpenGL* gl = &win->render.opengl; // NOTE(EimaMei): Check if CPU rendering is able to do decent gradients.
	gl->gradient = gradient;
	gl->gradientLen = len;
}



F_TRAITS(inline)
b32 siapp_windowEventPoll(const siWindow* win, siEventTypeEnum* out) {
	SI_ASSERT_NOT_NULL(win);
	SI_ASSERT_NOT_NULL(out);

	u32 event = SI_CSTR_U16(&win->e.type);
	u32 bit = *out;
	b32 res = false;

	while (res == 0 && bit <= SI_EVENT_COUNT) {
		res = event & SI_BIT(bit);
		bit += 1;
	}
	*out = bit;

	return bit != 0 && bit <= SI_EVENT_COUNT;
}

#if 0
typedef SI_ENUM(u32, siEventTypeEnum) {
	SI_EVENT_MOUSE_SCROLL,

	SI_EVENT_WINDOW_MOVE,
	SI_EVENT_WINDOW_FOCUS,

	SI_EVENT_COUNT = SI_EVENT_WINDOW_FOCUS
};
#endif

int main(void) {
	siAllocator* alloc = si_allocatorMake(SI_KILO(8));

	siWindow* win = siapp_windowMake(
		alloc, "Siliapp events",
		SI_AREA(0, 0),  SI_WINDOW_CENTER | SI_WINDOW_OPTIMAL_SIZE | SI_WINDOW_SCALING,
		0, 0, SI_AREA(1, 1)
	);
	siapp_windowBackgroundSet(win, SI_RGB(0, 0, 0));

	const siWindowEvent* e = siapp_windowEventGet(win);
	while (siapp_windowIsRunning(win)) {
		siapp_windowUpdate(win, true);

		siEventTypeEnum type = 0;
		while (siapp_windowEventPoll(win, &type)) {
			switch (type) {
				case SI_EVENT_CLOSE: {
					si_printf("The window is going to get closed now.\n");
					break;
				}
				case SI_EVENT_KEY_PRESS: {
					si_printf(
						"A key has been  pressed, the most recent one being '%#X/%c' [Char(s): '%*s'].\n",
						e->curKey, e->curKey, e->charBufferLen, e->charBuffer
					);
					break;
				}
				case SI_EVENT_KEY_RELEASE: {
					si_printf(
						"A key has been released, the most recent one being '%#X/%c'.\n",
						e->curKey, e->curKey
					);
					break;
				}
				case SI_EVENT_MOUSE_MOVE: {
					if (siapp_windowKeyPressed(win, SK_SHIFT_L)) {
						si_printf(
							"The mouse has been moved, its coordinates being:"
							"(Window: %ix%i), (Global: %ix%i), (Scaled accordingly: %4.2fx%4.2f).\n",
							e->mouse.x, e->mouse.y, e->mouseRoot.x, e->mouseRoot.y,
							e->mouseScaled.x, e->mouseScaled.y
						);
					}
					break;
				}
				case SI_EVENT_MOUSE_PRESS: {
					si_printf(
						"A mouse button has been pressed, the most recent one being Button %i\n",
						e->curMouse
					);
					break;
				}
				case SI_EVENT_MOUSE_RELEASE: {
					si_printf(
						"A mouse button has been released, the most recent one being Button %i\n",
						e->curMouse
					);
					break;
				}
				case SI_EVENT_MOUSE_SCROLL: {
					si_printf(
						"A mouse wheel was scrolled, the most recent one being Wheel %i\n",
						e->mouseWheel
					);
					break;
				}
				case SI_EVENT_WINDOW_RESIZE: {
					si_printf(
						"The window has been resized, its area now being: %ix%i, (Scale: %ix%i)\n",
						e->windowSize.width, e->windowSize.height,
						e->windowSizeScaled.width, e->windowSizeScaled.height
					);
					break;
				}
				case SI_EVENT_WINDOW_MOVE: {
					si_printf(
						"The window has been moved, its position now being: %ix%i\n",
						e->windowPos.x, e->windowPos.y
					);
					break;
				}
				case SI_EVENT_WINDOW_FOCUS: {
					si_printf(
						"The focus of the window has been changed, current focus: %B\n",
						e->focus
					);
					break;
				}
			}
		}
	}
	siapp_windowClose(win);
	si_allocatorFree(alloc);

	return 0;
}
