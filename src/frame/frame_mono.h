#ifndef _FRAME_MONO_H_
#define _FRAME_MONO_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_Mono : public Frame_Base
{
public:
    Frame_Mono();
    ~Frame_Mono();
    int init(epdgui_args_vector_t &args);
    int run();
	void err(String info);
	void exit();

private:
    M5EPD_Canvas *_canvas_picture;
    M5EPD_Canvas *_canvas_labels;
    M5EPD_Canvas *_canvas_data;
    bool _is_first;
	uint8_t _count;
	char _buffer[16];
};

#endif //_FRAME_MONO_H_
