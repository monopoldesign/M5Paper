#include "frame_basic_output.h"
#include "../epdgui/epdgui_button.h"

#include "basic_parser.h"

char *program;

Frame_Basic_Output::Frame_Basic_Output() : Frame_Base()
{
	_frame_name = "Frame_Basic_Output";

	exitbtn("Home");
	_canvas_title->drawString("BASIC-Output", 270, 34);

	_key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

Frame_Basic_Output::~Frame_Basic_Output(void)
{
	delete _canvas_output;
	delete _key_exit;
}

int Frame_Basic_Output::init(epdgui_args_vector_t &args)
{
	_is_run = 1;
	_is_first = true;

	EPDGUI_AddObject(_key_exit);
	_canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);

	if (args.size() > 0)
	{
		_data = *(String*)(args[0]);
		args.pop_back();
	}

	return 6;
}

int Frame_Basic_Output::run()
{
	Frame_Base::run();

	if(_is_first)
	{
		_is_first = false;
		_is_running = true;
		_loopCount = 0;
		_canvas_output = new M5EPD_Canvas(&M5.EPD);
		_canvas_output->createCanvas(520, (8 * 60) + 18);

		// Frame
		for (uint8_t i = 0; i < 4; i++)
			_canvas_output->drawRect(i, i, 520 - (2 * i), ((8 * 60) + 18) - (2 * i), 15);

		//_canvas_output->setTextSize(_size);
		//_canvas_output->setTextArea(_margin_left, _margin_top, 520 - _margin_left - _margin_right, ((8 * 60) + 18) - _margin_top - _margin_bottom);
		//_canvas_output->print(_data);

		program = (char *)malloc(_data.length() + 1);
		_data.toCharArray(program, _data.length());
		ubasic_init(program, _canvas_output);
	}

	if (_is_running)
	{
		if (!ubasic_finished())
			line_statement();
		else
		{
			_is_running = false;
			free(program);
		}
	}

	/*
	if (_loopCount < 10 && _is_running)
	{
		_loopCount++;
		uint16_t _x = random(520);
		uint16_t _y = random(((8 * 60) + 18));
		uint16_t _r = random(200);
		_canvas_output->drawCircle(_x, _y, _r, 15);
		_canvas_output->pushCanvas(10, 90, UPDATE_MODE_GC16);
	}
	*/

	return 1;
}
