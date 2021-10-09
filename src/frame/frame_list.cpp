#include "frame_list.h"
#include "frame_list_menu.h"
#include "../epdgui/epdgui_button.h"

Frame_List::Frame_List(bool isHorizontal) : Frame_Base()
{
    _frame_name = "Frame_List";
 
	const uint16_t kKeyBaseY = 628;
	key_run1 = new EPDGUI_Button("RUN1", 4, kKeyBaseY, 250, 52);
	key_run2 = new EPDGUI_Button("RUN2", 286, kKeyBaseY, 250, 52);

	inputbox1 = new EPDGUI_Textbox(4, 100, 532, 250);
    inputbox1->SetState(EPDGUI_Textbox::EVENT_PRESSED);
	inputbox1->SetTextSize(48);

	inputbox2 = new EPDGUI_Textbox(4, 360, 532, 250);
    inputbox2->SetState(EPDGUI_Textbox::EVENT_PRESSED);
	inputbox2->SetTextSize(48);

	keyboard = new EPDGUI_Keyboard(isHorizontal);

	exitbtn("Home");
	menubtn("Menu");
	_canvas_title->drawString("Keyboard", 270, 34);
	
	key_run1->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)inputbox1);
	key_run1->Bind(EPDGUI_Button::EVENT_RELEASED, key_run_cb);

    key_run2->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)inputbox2);
    key_run2->Bind(EPDGUI_Button::EVENT_RELEASED, key_run_cb);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);

    _key_menu->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
    _key_menu->Bind(EPDGUI_Button::EVENT_RELEASED, showMenu);
}

Frame_List::~Frame_List(void)
{
    delete inputbox1;
    delete inputbox2;
    delete keyboard;

	delete _key_exit;
	delete _key_menu;

	delete key_run1;
	delete key_run2;
}

int Frame_List::init(epdgui_args_vector_t &args)
{
    _is_run = 1;
    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
 
    EPDGUI_AddObject(_key_exit);
	EPDGUI_AddObject(_key_menu);
	EPDGUI_AddObject(key_run1);
	EPDGUI_AddObject(key_run2);
    EPDGUI_AddObject(inputbox1);
    EPDGUI_AddObject(inputbox2);
    EPDGUI_AddObject(keyboard);

	if (args.size() > 0)
	{
		String *result = (String*)(args[0]);
		inputbox2->SetText("");
		inputbox2->AddText(*result);
		delete result;
		args.pop_back();
	}
	else
	{
		inputbox1->SetText("");
		inputbox2->SetText("");
	}

	return 6;
}

int Frame_List::run()
{
    Frame_Base::run();

	if (inputbox1->isSelected())
	    inputbox1->AddText(keyboard->getData());
	else if (inputbox2->isSelected())
	    inputbox2->AddText(keyboard->getData());

    return 1;
}

void Frame_List::key_run_cb(epdgui_args_vector_t &args)
{
    ((EPDGUI_Textbox*)(args[0]))->SetText("");
}

void Frame_List::showMenu(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_List_Menu");

	if (frame == NULL)
	{
		frame = new Frame_List_Menu();
		EPDGUI_AddFrame("Frame_List_Menu", frame);
	}

	EPDGUI_PushFrame(frame);
	*((int*)(args[0])) = 0;
}
