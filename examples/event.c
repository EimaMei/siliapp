#include <siliapp.h>


int main(void) {
	siWindow* win = siapp_windowMake(
		"Siliapp events",
		SI_AREA(0, 0),  SI_WINDOW_CENTER | SI_WINDOW_OPTIMAL_SIZE
	);
	siapp_windowRendererMake(win, SI_RENDERING_CPU, 0, SI_AREA(0, 0), 0);
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
						"A key has been pressed, the most recent one being '%#X/%c' [Char(s): '%*s'].\n",
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
				/* SI_EVENT_MOUSE_CHANGE */
			}
		}
    }

	siapp_windowClose(win);
	return 0;
}
