#ifndef _FRAME_BASIC_MENU_H_
#define _FRAME_BASIC_MENU_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_Basic_Menu : public Frame_Base
{
	public:
	    Frame_Basic_Menu();
	    ~Frame_Basic_Menu();
	    int init(epdgui_args_vector_t &args);
	    int run();

	private:
		static void select_menu(epdgui_args_vector_t &args);
		static void run_basic(epdgui_args_vector_t &args);

	    M5EPD_Canvas *_canvas_menu;
	    EPDGUI_Button *_key_menu[8];

		bool _is_first;
		uint8_t _menuNo;
};

#endif //_FRAME_BASIC_MENU_H_
