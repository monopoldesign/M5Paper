#include "frame_basic_menu.h"
#include "../epdgui/epdgui_button.h"

Frame_Basic_Menu::Frame_Basic_Menu() : Frame_Base()
{
	_frame_name = "Frame_Basic_Menu";
 }

Frame_Basic_Menu::~Frame_Basic_Menu(void)
{
	switch (_menuNo)
	{
		case 0:
			for (uint8_t i = 0; i < 3; i++)
				delete _key_menu[i];
			break;
		case 1:
		case 2:
			for (uint8_t i = 0; i < 8; i++)
				delete _key_menu[i];
			break;
	}

	delete _canvas_menu;
}

int Frame_Basic_Menu::init(epdgui_args_vector_t &args)
{
	_is_run = 1;
	_is_first = true;

	if (args.size() > 0)
	{
		String *result = (String*)(args[0]);
		_menuNo = result->toInt();

		// Add Menu-Buttons
		switch (_menuNo)
		{
			case 0:
				for (uint8_t i = 0; i < 3; i++)
				{
					switch (i)
					{
						case 0:
							_key_menu[i] = new EPDGUI_Button("Load Project", 20, (i * 60) + 100, 500, 60);
							break;
						case 1:
							_key_menu[i] = new EPDGUI_Button("Save Project", 20, (i * 60) + 100, 500, 60);
							break;
						case 2:
							_key_menu[i] = new EPDGUI_Button("   Exit", 20, (i * 60) + 100, 500, 60);
							break;
					}

					_key_menu[i]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _key_menu[i]);
					_key_menu[i]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&_is_run));
					_key_menu[i]->Bind(EPDGUI_Button::EVENT_RELEASED, select_menu);
				}
				break;
			case 1:
				for (uint8_t i = 0; i < 8; i++)
				{
					_key_menu[i] = new EPDGUI_Button("Load File " + String(i + 1, 10), 20, (i * 60) + 100, 500, 60);

					_key_menu[i]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _key_menu[i]);
					_key_menu[i]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&_is_run));
					_key_menu[i]->Bind(EPDGUI_Button::EVENT_RELEASED, select_menu);
				}
				break;
			case 2:
				for (uint8_t i = 0; i < 8; i++)
				{
					_key_menu[i] = new EPDGUI_Button("Save File " + String(i + 1, 10), 20, (i * 60) + 100, 500, 60);

					_key_menu[i]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _key_menu[i]);
					_key_menu[i]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&_is_run));
					_key_menu[i]->Bind(EPDGUI_Button::EVENT_RELEASED, select_menu);
				}
				break;
		}

		switch (_menuNo)
		{
			case 0:
				for (uint8_t i = 0; i < 3; i++)
				{
					EPDGUI_AddObject(_key_menu[i]);
					_key_menu[i]->SetID(i);
				}
				break;
			case 1:
			case 2:
				for (uint8_t i = 0; i < 8; i++)
				{
					EPDGUI_AddObject(_key_menu[i]);
					_key_menu[i]->SetID(i);
				}
				break;
			default:
				EPDGUI_PopFrame();
				_is_run = 0;
				break;
		}
		args.pop_back();
	}
	return 6;
}

int Frame_Basic_Menu::run()
{
	Frame_Base::run();

	if(_is_first)
	{
		_is_first = false;

		_canvas_menu = new M5EPD_Canvas(&M5.EPD);

		switch (_menuNo)
		{
			case 0:
				_canvas_menu->createCanvas(520, (3 * 60) + 18);
				break;
			case 1:
			case 2:
				_canvas_menu->createCanvas(520, (8 * 60) + 18);
				break;
		}

		// Menu-Frame
		for (uint8_t i = 0; i < 4; i++)
		{
			switch (_menuNo)
			{
				case 0:
					_canvas_menu->drawRect(i, i, 520 - (2 * i), ((3 * 60) + 18) - (2 * i), 15);
					break;
				case 1:
				case 2:
					_canvas_menu->drawRect(i, i, 520 - (2 * i), ((8 * 60) + 18) - (2 * i), 15);
					break;
			}
		}

 		//_canvas_menu->pushCanvas(10, 90, UPDATE_MODE_DU4);

		switch (_menuNo)
		{
			case 0:
				for (uint8_t i = 0; i < 3; i++)
					_key_menu[i]->Draw();
				break;
			case 1:
			case 2:
				for (uint8_t i = 0; i < 8; i++)
					_key_menu[i]->Draw();
				break;
		}
	}

	return 1;
}

void Frame_Basic_Menu::select_menu(epdgui_args_vector_t &args)
{
	String *result = 0;

	result = new String(((EPDGUI_Button *)(args[0]))->GetID(), 10);
	EPDGUI_AddFrameArg("Frame_Basic", 0, result);

	EPDGUI_PopFrame();

	// _is_run=0 -> deactivate Frame
	*((int*)(args[1])) = 0;
}
