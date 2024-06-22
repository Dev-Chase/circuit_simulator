#include "button.h"
#include "raylib.h"

bool button_is_clicked(Button button[static 1]) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
		   CheckCollisionPointRec(GetMousePosition(), button->RECT);
}

void button_render(const Button button[static 1]) {
	DrawRectangleRounded(button->RECT, BTN_CORNER_ROUNDNESS,
						 BTN_CORNER_SEGMENTS, button->bg);
	DrawRectangleRoundedLines(button->RECT, BTN_CORNER_ROUNDNESS,
							  BTN_CORNER_SEGMENTS, BTN_BORDER_THICKNESS,
							  button->fg);
	DrawText(button->txt,
			 (button->RECT.x + button->RECT.width / 2) -
				 (MeasureText(button->txt, BTN_FONT_SIZE) / 2),
			 (button->RECT.y + button->RECT.height / 2) - (BTN_FONT_SIZE / 2),
			 BTN_FONT_SIZE, button->fg);
}

void button_set_state(Button button[static 1], const char *txt, Color bg) {
	button->txt = txt;
	button->bg = bg;
}
