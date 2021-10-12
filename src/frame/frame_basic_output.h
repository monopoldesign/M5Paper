#ifndef _FRAME_BASIC_OUTPUT_H_
#define _FRAME_BASIC_OUTPUT_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_Basic_Output : public Frame_Base
{
	public:
	    Frame_Basic_Output();
	    ~Frame_Basic_Output();
	    int init(epdgui_args_vector_t &args);
	    int run();

	private:
	    M5EPD_Canvas *_canvas_outback;
	    M5EPD_Canvas *_canvas_output;

		bool _is_first;
		bool _is_running;
		uint8_t _loopCount;

		uint8_t _size;
		int16_t _margin_left, _margin_right, _margin_top, _margin_bottom;
		String _data;
};

#endif //_FRAME_OUTPUT_MENU_H_
