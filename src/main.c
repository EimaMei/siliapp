#define SIAPP_X11_RESIZE_BORDER 4
#include <siliapp.h>
#define DISABLE_SECOND_WINDOW  1 /* NOTE(EimaMei): For whatever reason, creating a
								   window on a subthread is not allowed  on MacOS,
								   meaning you have to create and update your windows
								   on the main thread only. */


#if DISABLE_SECOND_WINDOW != 1
void secondWindowLoop(siWindow* firstWindow);

#if defined(SIAPP_PLATFORM_API_COCOA)
b32 stopRenderingWin2 = false;
#endif
#endif

#define CURSOR_W 16
#define CURSOR_H 32

// add resize support for win32 borderless windows
// add the option to move borderless windows
// fix viewport for opengl
// improve instancing for laptop, especially trying to draw more text
// add a way to check how much memory you can take
// transformed cursor returns back to normal when it returns to the og window


int main(void) {
	siWindow* win = siapp_windowMake(
		"Example window | ĄČĘĖĮŠŲ | 「ケケア」",
		SI_AREA(0, 0),
		SI_WINDOW_DEFAULT | SI_WINDOW_OPTIMAL_SIZE
	);
	siapp_windowRendererMake(win, SI_RENDERING_CPU, 1, SI_AREA(1024, 1024), 2);
	siapp_windowBackgroundSet(win, SI_RGB(128, 0, 0));

	siDropEvent drops[2];

	siColor sideClrs[countof(drops)] = {SI_RGBA(255, 0, 0, 128), SI_RGBA(0, 255, 0, 128)};
	i32 widthHalf = win->originalSize.width / 2;

	for_range (i, 0, countof(drops)) {
		siapp_windowDragAreaMake( // NOTE(EimaMei): Update this on windows and linux.
			win,
			SI_RECT(widthHalf * i, 0, widthHalf, win->originalSize.height),
			&drops[i]
		);
	}


	siAllocator* alloc = si_allocatorMake(sizeof(siColor) * CURSOR_W * CURSOR_H);
	siColor* icon = si_mallocArray(alloc, siColor, CURSOR_W * CURSOR_H);
	srand(si_timeNowUTC()); // We create a random cusor because why not.

	for_range (y, 0, CURSOR_H) {
		siColor clr = SI_RGB(rand() % 256, rand() % 256, rand() % 256);
		for_range (x, 0, CURSOR_W) {
			icon[x + y * CURSOR_W] = clr;
		}
	}
	siCursorType customCursor = siapp_cursorMake((siByte*)icon, SI_AREA(CURSOR_W, CURSOR_H), 4);

	siCursorType curCursor = SI_CURSOR_DEFAULT,
				 newCursor = customCursor;
	u32 curRender = SI_RENDERING_CPU,
		newRender = SI_RENDERING_OPENGL;

#if DISABLE_SECOND_WINDOW != 1
#if !defined(SIAPP_PLATFORM_API_COCOA)
	siThread t = si_threadCreate(secondWindowLoop, win);
	si_threadStart(&t);
#else
	siWindow* win2 = siapp_windowMakeEx(
		"Second window", SI_POINT(200, 200), SI_AREA(400, 400),
		SI_WINDOW_RESIZABLE
	);
	siapp_windowRendererMake(win2, SI_RENDERING_OPENGL, 2, SI_AREA(0, 0), 0);
	siapp_windowBackgroundSet(win2, SI_RGB(113, 57, 173));
#endif
#endif

#if 0
	usize i = 0;
	while (true) {
		siArea area = siapp_screenGetAvailableResolution(i);
		SI_STOPIF(area.width == -1, break);
		si_printf("%ix%i\n", area.width, area.height);
		i += 1;
	}
#endif

#if 0
	siSearchConfig config = SI_SEARCH_DEFAULT;
	config.filetypes = (siSearchFilterSpec[]){{"Test", "out;mm"}, {"C/C++ stuff", "cpp;c"}};
	config.filetypesLen = 2;
	config.options = SI_SEARCH_ALLOW_MULTIPLE;

	siSearchHandle handle = siapp_fileManagerOpen(config);
	siSearchEntry search;
	usize x = 0;
	si_printf("damn %i\n", handle.len);
	while (siapp_searchPollEntry(&handle, &search)) {
		si_printf("%i: %s %i\n",  x, search.path, search.len);
        x += 1;
	}
#endif

#if 0
	siSiliStr username = siapp_appDataPathMake("smm");
	si_printf("%s | %i\n", username, SI_SILISTR_LEN(username));

#endif
	//siFont f = siapp_fontLoad(win, "res/calibri.ttf", 64);

#if 0
	for_range (i, 0, 63) {
		u32 m = i / 10;
		i32 base = (i - 10 * m) + 9;

		u32 actual = (m * 3) + ((base > -3) + (base > -6) + (base > -11));
		si_printf("%i: %i %llu\n", i, actual, SI_BIT(i));
	}
#endif

	siFont f = siapp_fontLoad(win, "res/calibri.ttf", 128);

	siAllocator* textAlloc = si_allocatorMake(SI_KILO(1));
	siText txt = siapp_textLoad(textAlloc, &f, "Kęstučio ąsotis");

	while (siapp_windowIsRunning(win) && !siapp_windowKeyClicked(win, SK_ESC)) {
		const siWindowEvent* e = siapp_windowUpdate(win, true);

		if (0) {
		siKeyState s0 = siapp_windowKeyGet(win, SK_SYSTEM_L);
		siKeyState s1 = siapp_windowKeyGet(win, SK_ALT_L);
		siKeyState s2 = siapp_windowKeyGet(win, SK_CAPS_LOCK);
		si_printf(
			"SYS: [%u, %u, %u], ALT: [%u, %u, %u], CAPS: [%u %u %u]\n",
			s0.clicked, s0.pressed, s0.released,
			s1.clicked, s1.pressed, s1.released,
			s2.clicked, s2.pressed, s2.released
		);
		}
		//si_printf("%i %i\n", e->mouseRoot.x, e->mouseRoot.y);

		u32 key = e->curKey * siapp_windowKeyClicked(win, e->curKey);
		switch (key) {
			case 0: { break; } /* The key is being pressed but the clicked frame has since passed. */
			case SK_UP: {
				char buf[512];
				usize len = siapp_clipboardTextLen();
				usize bytesWritten = siapp_clipboardTextGet(buf, sizeof(buf));

				if (bytesWritten) {
					si_printf("Clipboard: %s | %i;%i\n", buf, bytesWritten, len);
				}
				break;
			}
			case SK_DOWN: {
				siapp_clipboardTextSet("DOWN");
				break;
			}
			case SK_W: {
				static b32 showMouse = false;
				siapp_mouseShow(showMouse);
				showMouse ^= true;
				break;
			}
			case SK_H: {
				static siWindowShowState state = SI_SHOW_ACTIVATE;
				siapp_windowShow(win, state);
				state += 1;
				SI_STOPIF(state > SI_SHOW_RESTORE, state = 0);
				break;
			}
			case SK_F11:
			case SK_F: {
				static b32 fullscreen = true;
				siapp_windowFullscreen(win, fullscreen);
				fullscreen ^= true;
				break;
			}
			case SK_T: {
				SI_STOPIF(siapp_windowKeyPressed(win, SK_SHIFT_L), break);

				siapp_windowCursorSet(win, newCursor);
				si_swap(curCursor, newCursor);
				break;
			}
			case SK_C: {
				siapp_windowRendererChange(win, newRender);
				si_swap(curRender, newRender);
				continue;
			}
			case SK_M: {
				siPoint pos = SI_POINT(
					e->windowPos.x + e->windowSize.width / 2,
					e->windowPos.y + e->windowSize.height / 2
				);
				siapp_mouseMove(pos);
				break;
			}
			case SK_B: {
				static b32 borderless = true;
				siapp_windowBorderlessSet(win, borderless);
				borderless ^= true;
				break;
			}
			case SK_R: {
				siResizeOperation op = siapp_windowResizeOperationFind(win);
				if (op != 0) {
					siapp_windowResizeOperationLaunch(win, op);
				}
				break;
			}
		}
		if (siapp_windowKeyMul(win, SK_SHIFT_L, SK_T)) {
			usize len = 0;
			char title[255] = {0};
			siapp_messageBoxEx(
				win,
				"Enter the title", "Type out the new Window's title and once you're done, hit enter.",
				SI_MESSAGE_BOX_OK, SI_MESSAGE_BOX_ICON_INFO
			);

			while (siapp_windowIsRunning(win)) {
				e = siapp_windowUpdate(win, true);
				SI_STOPIF(e->type.keyPress && e->curKey == SK_ENTER, break);

				if (e->charBufferLen != 0 && len + e->charBufferLen < sizeof(title)) {
					memcpy(&title[len], e->charBuffer, e->charBufferLen);
					len += e->charBufferLen;
				}
			}

			siapp_windowTitleSetEx(win, title, len);
			continue;
		}

		for_range (i, 0, countof(drops)) {
			siDropEvent* drop = &drops[i];

			switch (drop->state) {
				case SI_DRAG_ENTER: {
					sideClrs[i].a += 64;
					break;
				}
				case SI_DRAG_LEAVE: {
					sideClrs[i].a -= 64;
					break;
				}
				case SI_DRAG_DROP: {
					siDropEntry entry;
					siDropHandle handle = siapp_dropEventHandle(*drop);

                    usize x = 0;
					while (siapp_dropEventPollEntry(&handle, &entry)) {
						si_printf("%i.%i: %s %i\n", i + 1, x, entry.path, entry.len);
                        x += 1;
					}
					sideClrs[i].a -= 64;
					break;
				}
			}
            siapp_dropEventEnd(drop);
			siapp_drawRect(
				win,
				SI_RECT(i * widthHalf, 0, widthHalf, win->originalSize.height),
				sideClrs[i]
			);
		}
		siColor gradient[3] = {
			SI_RGB(255, 0, 0), SI_RGB(0, 255, 0), SI_RGB(0, 0, 255)
		};
		siapp_windowGradientSet(win, gradient, countof(gradient));

		i32 length = win->originalSize.height - 50;
		siapp_drawTriangleIsosceles(
			win,
			SI_POINT(widthHalf - length / 2, 50), length, 60,
			SI_RGB(0, 0, 255)
		);


		//siapp_drawText(win, woah, SI_POINT(300, 0), 64);
		siapp_drawText(win, txt, SI_POINT(0, 0), 128);

		siapp_windowRender(win);
		siapp_windowSwapBuffers(win);


#if DISABLE_SECOND_WINDOW != 1 && defined(SIAPP_PLATFORM_API_COCOA)
		secondWindowLoop(win2);
#endif
	}
	si_allocatorFree(textAlloc);
	siapp_fontFree(f);
    siapp_cursorFree(customCursor);
	//siapp_fontFree(f);

	for_range (i, 0, countof(drops)) {
		siapp_windowDragAreaEnd(win, &drops[i]);
	}
	siapp_windowClose(win);
	si_allocatorFree(alloc);

#if DISABLE_SECOND_WINDOW != 1 && defined(SIAPP_PLATFORM_API_COCOA)
	SI_STOPIF(!stopRenderingWin2, siapp_windowClose(win2));
#endif
}


#if DISABLE_SECOND_WINDOW != 1
void secondWindowLoop(siWindow* firstWindow) {
#if !defined(SIAPP_PLATFORM_API_COCOA)
    siAllocator* alloc = si_allocatorMake(SI_KILO(1));

	siWindow* win = siapp_windowMakeEx(
		alloc, "Second window", SI_POINT(200, 200), SI_AREA(400, 400),
		SI_WINDOW_RESIZABLE, SI_RENDERING_OPENGL, 2, 0, SI_AREA(0, 0)
	);
	siapp_windowBackgroundSet(win, SI_RGB(113, 57, 173));

	while (siapp_windowIsRunning(win) && siapp_windowIsRunning(firstWindow)) {
#else
	siWindow* win = firstWindow;
	if (stopRenderingWin2 || !siapp_windowIsRunning(win)) {
		SI_STOPIF(!stopRenderingWin2, siapp_windowClose(win); stopRenderingWin2 = true);
		return ;
	}
#endif
		siapp_windowUpdate(win, true);

		siColor gradient[3] = {
			SI_RGB(50, 168, 82), SI_RGB(235, 169, 47), SI_RGBA(69, 107, 222, 64)
		};

		siapp_windowGradientSet(win, gradient, countof(gradient));

		i32 widthHalf = win->originalSize.width / 2;
		i32 length = win->originalSize.height - 50;
		siapp_drawTriangleRight(
			win,
			SI_POINT(widthHalf - length / 2, 50), length, 45,
			SI_RGB(0, 0, 0)
		);

		siapp_windowRender(win);
		siapp_windowSwapBuffers(win);

#if !defined(SIAPP_PLATFORM_API_COCOA)
	}
	siapp_windowClose(win); // We must close the window in the same thread where
							// the window was created at!
    si_allocatorFree(alloc);
#endif
}
#endif
