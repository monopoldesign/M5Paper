#include "global_setting.h"

#include "frame_basic.h"
#include "frame_basic_menu.h"
#include "frame_basic_output.h"
#include "../epdgui/epdgui_button.h"

uint8_t textsize_basic = 26;

Frame_Basic::Frame_Basic(bool isHorizontal) : Frame_Base()
{
	_frame_name = "Frame_Basic";

	_key_run = new EPDGUI_Button("RUN", 4, 628, 260, 52);
	_key_textsize_plus = new EPDGUI_Button("+", 448, 628, 88, 52);
	_key_textsize_reset = new EPDGUI_Button("26", 360, 628, 88, 52);
	_key_textsize_minus = new EPDGUI_Button("-", 272, 628, 88, 52);

	_inputbox = new EPDGUI_Textbox(4, 100, 532, 510);
	_inputbox->SetTextSize(textsize_basic);

	_keyboard = new EPDGUI_Keyboard(isHorizontal);
	exitbtn("Home");
	menubtn("Menu");
	_canvas_title->drawString("BASIC", 270, 34);

	_key_run->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_run->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void *)_inputbox);
	_key_run->Bind(EPDGUI_Button::EVENT_RELEASED, run_basic);

    _key_textsize_plus->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)_inputbox);
    _key_textsize_plus->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)_key_textsize_reset);
    _key_textsize_plus->Bind(EPDGUI_Button::EVENT_RELEASED, textsize_plus_cb);
    _key_textsize_reset->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)_inputbox);
    _key_textsize_reset->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)_key_textsize_reset);
    _key_textsize_reset->Bind(EPDGUI_Button::EVENT_RELEASED, textsize_reset_cb);
    _key_textsize_minus->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)_inputbox);
    _key_textsize_minus->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)_key_textsize_reset);
    _key_textsize_minus->Bind(EPDGUI_Button::EVENT_RELEASED, textsize_minus_cb);

	_key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);

	_key_menu->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_menu->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, &_menuNo);
	_key_menu->Bind(EPDGUI_Button::EVENT_RELEASED, showMenu);
}

Frame_Basic::~Frame_Basic(void)
{
    delete _inputbox;
    delete _keyboard;
	delete _key_run;

	delete _key_exit;
	delete _key_menu;
}

int Frame_Basic::init(epdgui_args_vector_t &args)
{
	String sdString = "";

	_is_run = 1;
	M5.EPD.Clear();
	_canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);

	EPDGUI_AddObject(_key_exit);
	EPDGUI_AddObject(_key_menu);

	EPDGUI_AddObject(_key_run);
	EPDGUI_AddObject(_key_textsize_plus);
	EPDGUI_AddObject(_key_textsize_reset);
	EPDGUI_AddObject(_key_textsize_minus);

	EPDGUI_AddObject(_inputbox);
	EPDGUI_AddObject(_keyboard);

	if (args.size() > 0)
	{
		String *result = (String*)(args[0]);

		switch (_menuNo)
		{
			case 0:
				switch(result->toInt())
				{
					case 0:
						_menuNo = 1;
						_newMenu = 1;
						break;
					case 1:
						_menuNo = 2;
						_newMenu = 1;
						break;
					case 2:
						//inputbox2->SetText("Exiting...");
						break;
				}
				break;
			case 1:
				if (GetInitStatus(0))
				{
					File myFile = SD.open("/P"+ *result + ".BAS", FILE_READ);

					while (myFile.available())
						sdString += (char)myFile.read();

					myFile.close();
					_inputbox->SetText(sdString);
					_inputbox->setCursor(sdString.length());
				}
				//else
				//	inputbox2->SetText("No SD-Card found!");

				_menuNo = 0;
				break;
			case 2:
				if (GetInitStatus(0))
				{
					String input = _inputbox->GetText();
					File myFile = SD.open("/P"+ *result + ".BAS", FILE_WRITE);
					myFile.print(input);
					myFile.close();
				}
				//else
				//	inputbox2->SetText("No SD-Card found!");

				_menuNo = 0;
				break;
		}
		args.pop_back();
	}
	//else
	//	_inputbox->SetText("");

	_inputbox->SetState(EPDGUI_Textbox::EVENT_PRESSED);
	return 6;
}

int Frame_Basic::run()
{
    Frame_Base::run();

	if (_newMenu)
		this->openMenu();
	else
	{
		if (_inputbox->isSelected())
		    _inputbox->AddText(_keyboard->getData());
	}

    return 1;
}

void Frame_Basic::showMenu(epdgui_args_vector_t &args)
{
	String *result = 0;

	Frame_Base *frame = EPDGUI_GetFrame("Frame_Basic_Menu");

	if (frame == NULL)
	{
		frame = new Frame_Basic_Menu();
		EPDGUI_AddFrame("Frame_Basic_Menu", frame);
	}

	// _menuNo - Argument
	result = new String(*((int*)(args[1])), 10);
	EPDGUI_AddFrameArg("Frame_Basic_Menu", 0, result);

	EPDGUI_PushFrame(frame);

	// _is_run=0 -> deactivate Frame
	*((int*)(args[0])) = 0;
}

void Frame_Basic::openMenu()
{
	String *result = 0;

	Frame_Base *frame = EPDGUI_GetFrame("Frame_Basic_Menu");

	if (frame == NULL)
	{
		frame = new Frame_Basic_Menu();
		EPDGUI_AddFrame("Frame_Basic_Menu", frame);
	}

	result = new String(_menuNo, 10);
	EPDGUI_AddFrameArg("Frame_Basic_Menu", 0, result);

	EPDGUI_PushFrame(frame);

	_newMenu = 0;
	_is_run = 0;
}

void Frame_Basic::textsize_plus_cb(epdgui_args_vector_t &args)
{
	textsize_basic += 4;

	if(textsize_basic > 96)
		textsize_basic = 96;

	char buf[10];
	sprintf(buf, "%d", textsize_basic);
	((EPDGUI_Button*)(args[1]))->setLabel(buf);
	((EPDGUI_Button*)(args[1]))->Draw(UPDATE_MODE_GL16);
	((EPDGUI_Textbox*)(args[0]))->SetTextSize(textsize_basic);
}

void Frame_Basic::textsize_minus_cb(epdgui_args_vector_t &args)
{
	textsize_basic -= 4;

	if(textsize_basic < 12)
		textsize_basic = 12;

	char buf[10];
	sprintf(buf, "%d", textsize_basic);
	((EPDGUI_Button*)(args[1]))->setLabel(buf);
	((EPDGUI_Button*)(args[1]))->Draw(UPDATE_MODE_GL16);
	((EPDGUI_Textbox*)(args[0]))->SetTextSize(textsize_basic);
}

void Frame_Basic::textsize_reset_cb(epdgui_args_vector_t &args)
{
	textsize_basic = 26;
	char buf[10];
	sprintf(buf, "%d", textsize_basic);
	((EPDGUI_Button*)(args[1]))->setLabel(buf);
	((EPDGUI_Button*)(args[1]))->Draw(UPDATE_MODE_GL16);
	((EPDGUI_Textbox*)(args[0]))->SetTextSize(textsize_basic);
}

void Frame_Basic::run_basic(epdgui_args_vector_t &args)
{
	String *result = 0;

	Frame_Base *frame = EPDGUI_GetFrame("Frame_Basic_Output");

	if (frame == NULL)
	{
		frame = new Frame_Basic_Output();
		EPDGUI_AddFrame("Frame_Basic_Output", frame);
	}

	result = new String(((EPDGUI_Textbox *)(args[1]))->GetText());
	EPDGUI_AddFrameArg("Frame_Basic_Output", 0, result);

	EPDGUI_PushFrame(frame);

	// _is_run=0 -> deactivate Frame
	*((int*)(args[0])) = 0;
}
