﻿# include "title.hpp"

Title::Title(const InitData& init)
	: IScene{ init } {

	StartFlag = false;
	connect_state = (uint8)0;
	InputSpeed = (uint8)100;

	m_exitB = Rect{ Point(650, 100), 50, 160 };
	m_exitT = Transition{ 0.6s, 0.3s };

	bg_tex = Texture{ Bg_pass };

	//graysea.Initialize();

}

void Title::update()
{

	m_exitT.update(m_exitB.mouseOver());

	if (SimpleGUI::Button(U"Bot initializing", Vec2{ 120, 60 }, 300)) {
		if (connect_state == 0 || connect_state == 2) {
			if (graysea.Initialize()) {
				connect_state = (uint8)1;
			}
			else {
				connect_state = (uint8)2;
			}
		}
		else {
			connect_state = (uint8)1;
		}
		
	}

	if (SimpleGUI::Button(U"AI starting", Vec2{ 180, 60 }, 300)) {
		if (connect_state == 1) {
			StartFlag = true;
		}
	}

	if (SimpleGUI::Button(U"AI stopping", Vec2{ 240, 60 }, 300)) {
		if (connect_state == 1) {
			StartFlag = false;
		}
	}

	if (m_exitB.leftClicked())
	{
		// 終了
		System::Exit();
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });
	bg_tex.draw(0, 0);

	/*FontAsset(U"TitleFont")(U"TetrisEmulator2 -v1.1-")
		.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 80, Vec2{ 600, 60 });*/

	m_exitB.draw(ColorF{ 1.0, 0.5 + m_exitT.value() / 2 }).drawFrame(2);

	FontAsset(U"Menu")(U"EXIT").drawAt(m_exitB.center(), ColorF{ 0.25 });

}

Title::~Title()
{

	graysea.Destroy();

}
