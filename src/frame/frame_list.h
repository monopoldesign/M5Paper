#ifndef _FRAME_LIST_H_
#define _FRAME_LIST_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_List : public Frame_Base
{
	public:
	    Frame_List(bool isHorizontal = false);
	    ~Frame_List();
	    int init(epdgui_args_vector_t &args);
	    int run();

	private:
		static void key_run_cb(epdgui_args_vector_t &args);
		static void showMenu(epdgui_args_vector_t &args);

		EPDGUI_Button *key_run1;
		EPDGUI_Button *key_run2;

	    EPDGUI_Textbox *inputbox1;
	    EPDGUI_Textbox *inputbox2;
	    EPDGUI_Keyboard *keyboard;

		char _buffer[16];
};

#endif //_FRAME_LIST_H_
