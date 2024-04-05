#define SICDEF_STATIC
#include <siliapp.h>
#define DISABLE_SECOND_WINDOW  0 /* NOTE(EimaMei): For whatever reason, creating a
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


// fix viewport for opengl
// Fix the issue with trying to close both windows for GL

int main(void) {

	siWindow* win = siapp_windowMake(
		"Example window | ĄČĘĖĮŠŲ | 「ケケア」",
		SI_AREA(0, 0),
		SI_WINDOW_DEFAULT | SI_WINDOW_OPTIMAL_SIZE | SI_WINDOW_SCALING
	);
	siapp_windowRendererMake(win, SI_RENDERING_OPENGL, 4, SI_AREA(1024, 1024), 2);
	siapp_windowBackgroundSet(win, SI_RGB(128, 0, 0));

	siDropEvent drops[2];

	siColor sideClrs[countof(drops)] = {SI_RGBA(255, 0, 0, 128), SI_RGBA(0, 255, 0, 128)};
	i32 widthHalf = win->originalSize.width / 2;

	for_range (i, 0, countof(drops)) {
		siapp_windowDragAreaMake(
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
	u32 curRender = SI_RENDERING_OPENGL,
		newRender = SI_RENDERING_CPU;

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

	while (siapp_windowIsRunning(win) && !siapp_windowKeyClicked(win, SK_ESC)) {
		siapp_windowUpdate(win, true);
		const siWindowEvent* e = siapp_windowEventGet(win);


		if (e->type.windowMove) {
			si_printf("Window is being moved: %ix%i\n", e->windowPos.x, e->windowPos.y);
		}
		else if (e->type.windowResize) {
			si_printf("Window is being resized: %ix%i\n", e->windowSize.width, e->windowSize.height);
		}
		else if (e->type.keyPress) {
			switch (e->curKey * siapp_windowKeyClicked(win, e->curKey)) {
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
					static b32 show = false;
					siapp_mouseShow(show);
					show ^= true;
					break;
				}
				case SK_T: {
					siapp_windowCursorSet(win, newCursor);
					si_swap(curCursor, newCursor);
					break;
				}
				case SK_C: {
					siapp_windowRendererChange(win, newRender);
					si_swap(curRender, newRender);
					continue;
				}
			}
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

		siapp_windowRender(win);
		siapp_windowSwapBuffers(win);

#if DISABLE_SECOND_WINDOW != 1 && defined(SIAPP_PLATFORM_API_COCOA)
		secondWindowLoop(win2);
#endif
	}
    siapp_cursorFree(customCursor);

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
