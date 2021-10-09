#include "frame_list_menu.h"
#include "../epdgui/epdgui_button.h"

Frame_List_Menu::Frame_List_Menu() : Frame_Base()
{
    _frame_name = "Frame_List_Menu";
 
    _canvas_menu = new M5EPD_Canvas(&M5.EPD);
    _canvas_menu->createCanvas(520, 198);

	for (uint8_t i = 0; i < 4; i++)
		_canvas_menu->drawRect(i, i, 520 - (2 * i), 198 - (2 * i), 15);

	menu1 = new EPDGUI_Button("Load Project", 20, (0 * 60) + 100, 500, 60);
	menu2 = new EPDGUI_Button("Save Project", 20, (1 * 60) + 100, 500, 60);
	menu3 = new EPDGUI_Button("   Exit", 20, (2 * 60) + 100, 500, 60);

	menu1->SetCustomString("MENU1");
	menu1->SetID(1);
	menu1->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, menu1);
	menu1->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&_is_run));
	menu1->Bind(EPDGUI_Button::EVENT_RELEASED, select_menu);

	menu2->SetCustomString("MENU2");
	menu2->SetID(2);
	menu2->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, menu2);
	menu2->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&_is_run));
	menu2->Bind(EPDGUI_Button::EVENT_RELEASED, select_menu);

	menu3->SetCustomString("MENU3");
	menu3->SetID(3);
	menu3->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, menu3);
	menu3->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&_is_run));
	menu3->Bind(EPDGUI_Button::EVENT_RELEASED, select_menu);
}

Frame_List_Menu::~Frame_List_Menu(void)
{
	delete menu1;
	delete menu2;
	delete menu3;

	delete _canvas_menu;
}

int Frame_List_Menu::init(epdgui_args_vector_t &args)
{
	_is_run = 1;
	_is_first = true;
	EPDGUI_AddObject(menu1);
	EPDGUI_AddObject(menu2);
	EPDGUI_AddObject(menu3);
	return 6;
}

int Frame_List_Menu::run()
{
	Frame_Base::run();

	if(_is_first)
	{
		_is_first = false;
 		_canvas_menu->pushCanvas(10, 90, UPDATE_MODE_DU4);

		menu1->Draw();
		menu2->Draw();
		menu3->Draw();
	}

	return 1;
}

void Frame_List_Menu::select_menu(epdgui_args_vector_t &args)
{
	String *result = 0;

	if (((EPDGUI_Button *)(args[0]))->GetCustomString() == "MENU1")
		result = new String("Loading Project...");
	else if (((EPDGUI_Button *)(args[0]))->GetCustomString() == "MENU2")
		result = new String("Saving Project...");
	else if (((EPDGUI_Button *)(args[0]))->GetCustomString() == "MENU3")
		result = new String("Exiting...");

	/*
	switch (((EPDGUI_Button *)(args[0]))->GetID())
	{
		case 18:
			result = new String("Loading Project...");
			break;
		case 29:
			result = new String("Saving Project...");
			break;
		case 40:
			result = new String("Exiting...");
			break;
	}
	*/

	EPDGUI_AddFrameArg("Frame_List", 0, result);
	EPDGUI_PopFrame();
	*((int*)(args[1])) = 0;
}
