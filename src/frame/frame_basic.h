#ifndef _FRAME_BASIC_H_
#define _FRAME_BASIC_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_Basic : public Frame_Base
{
	public:
	    Frame_Basic(bool isHorizontal = false);
	    ~Frame_Basic();
	    int init(epdgui_args_vector_t &args);
	    int run();
		void openMenu();

	private:
		static void showMenu(epdgui_args_vector_t &args);
		static void textsize_plus_cb(epdgui_args_vector_t &args);
		static void textsize_minus_cb(epdgui_args_vector_t &args);
		static void textsize_reset_cb(epdgui_args_vector_t &args);
		static void run_basic(epdgui_args_vector_t &args);

		EPDGUI_Button *_key_run;
		EPDGUI_Button *_key_textsize_plus;
		EPDGUI_Button *_key_textsize_reset;
		EPDGUI_Button *_key_textsize_minus;

	    EPDGUI_Textbox *_inputbox;
	    EPDGUI_Keyboard *_keyboard;

		uint8_t _menuNo = 0;
		uint8_t _newMenu = 0;
};

#endif //_FRAME_BASIC_H_
