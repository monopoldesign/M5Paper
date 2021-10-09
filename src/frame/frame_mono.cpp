#include "frame_mono.h"

Frame_Mono::Frame_Mono(void)
{
    _frame_name = "Frame_Mono";
 
     _canvas_picture = new M5EPD_Canvas(&M5.EPD);
    _canvas_picture->createCanvas(540, 350);

	_canvas_labels = new M5EPD_Canvas(&M5.EPD);
	_canvas_labels->createCanvas(100, 100);
	_canvas_labels->setTextSize(26);

	_canvas_data = new M5EPD_Canvas(&M5.EPD);
	_canvas_data->createCanvas(200, 100);
	_canvas_data->setTextSize(26);

	M5.SHT30.Begin();

	exitbtn("Back");
	_canvas_title->drawString("MoNo-Test", 270, 34);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);

    _is_first = true;
}

Frame_Mono::~Frame_Mono(void)
{
	delete _canvas_data;
	delete _canvas_labels;
	delete _canvas_picture;
}

int Frame_Mono::init(epdgui_args_vector_t &args)
{
    _is_run = 1;
	_is_first = true;
	_count = 0;

	M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    EPDGUI_AddObject(_key_exit);
    return 3;
}

int Frame_Mono::run()
{
    Frame_Base::run();

    if(_is_first)
    {
		_is_first = false;
		LoadingAnime_32x32_Start(254, 500);

		bool ret = _canvas_picture->drawJpgFile(SD, "/test.jpg");
		if (ret == 0)
			err("Error opening /test.jpg");

		LoadingAnime_32x32_Stop();
		_canvas_picture->pushCanvas(0, 72, UPDATE_MODE_GC16);

		_canvas_labels->drawString("Count:", 10, 10);
		_canvas_labels->drawString("Temp: ", 10, 50);
		_canvas_labels->pushCanvas(0, 450, UPDATE_MODE_A2);

		//M5.update();
		//log_d("");
	}

	sprintf(_buffer, "%02d", _count);
	_canvas_data->drawString(_buffer, 10, 10);

	M5.SHT30.UpdateData();
	float temp = M5.SHT30.GetTemperature();

	sprintf(_buffer, "%.2f℃", temp);
	_canvas_data->drawString(_buffer, 10, 50);
	_canvas_data->pushCanvas(100, 450, UPDATE_MODE_A2);

	_count++;

	return 1;
}

void Frame_Mono::err(String info)
{
    _canvas_picture->fillCanvas(0);
    _canvas_picture->fillRect(254 - 150, 500 - 50, 300, 100, 15);
    _canvas_picture->drawString(info, 150, 55);
}

void Frame_Mono::exit()
{
}
