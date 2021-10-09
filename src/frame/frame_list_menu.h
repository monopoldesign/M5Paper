#ifndef _FRAME_LIST_MENU_H_
#define _FRAME_LIST_MENU_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_List_Menu : public Frame_Base
{
	public:
	    Frame_List_Menu();
	    ~Frame_List_Menu();
	    int init(epdgui_args_vector_t &args);
	    int run();

	private:
		static void select_menu(epdgui_args_vector_t &args);

		bool _is_first;
	    M5EPD_Canvas *_canvas_menu;
	    EPDGUI_Button *_key_menu[3];

		EPDGUI_Button *menu1;
		EPDGUI_Button *menu2;
		EPDGUI_Button *menu3;

		char _buffer[16];
};

#endif //_FRAME_LIST_MENU_H_
