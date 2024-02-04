# include "title.hpp"

Title::Title(const InitData& init)
	: IScene{ init } {

	StartFlag = false;
	InputSpeed = 0.0;

	/*m_soloB = Rect{ Point(100, 210), 500, 80 };
	m_soloT = Transition{ 0.6s, 0.3s };

	m_aiB = Rect{ Point(100, 310), 500, 80 };
	m_aiT = Transition{ 0.6s, 0.3s };

	m_setB = Rect{ Point(100, 510), 500, 80 };
	m_setT = Transition{ 0.6s, 0.3s };*/

	m_exitB = Rect{ Point(650, 100), 50, 160 };
	m_exitT = Transition{ 0.6s, 0.3s };

	bg_tex = Texture{ Bg_pass };
}

void Title::update()
{

	/*m_soloT.update(m_soloB.mouseOver());
	m_aiT.update(m_aiB.mouseOver());
	m_setT.update(m_setB.mouseOver());*/

	m_exitT.update(m_exitB.mouseOver());

	if (SimpleGUI::Button(U"AI starting", Vec2{ 120, 60 }, 240)) {
		StartFlag = true;
	}

	if (SimpleGUI::Button(U"AI stopping", Vec2{ 180, 60 }, 240)) {
		StartFlag = false;
	}

	//if (m_soloB.mouseOver() || m_aiB.mouseOver() || m_setB.mouseOver() || m_exitB.mouseOver())
	//{
	//	Cursor::RequestStyle(CursorStyle::Hand);
	//}
	//if (m_soloB.leftClicked())
	//{
	//	// ソロプレイ
	//	changeScene(State::Solo);
	//}
	//else if (m_aiB.leftClicked())
	//{
	//	// AIしぐれいん
	//	changeScene(State::Ai);
	//}
	//else if (m_setB.leftClicked())
	//{
	//	// セッティング
	//	changeScene(State::Setting);
	//}

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


	/*m_soloB.draw(ColorF{ 1.0, 0.5 + m_soloT.value() / 2 }).drawFrame(2);
	m_aiB.draw(ColorF{ 1.0, 0.5 + m_aiT.value() / 2 }).drawFrame(2);
	m_setB.draw(ColorF{ 1.0, 0.5 + m_setT.value() / 2 }).drawFrame(2);*/
	m_exitB.draw(ColorF{ 1.0, 0.5 + m_exitT.value() / 2 }).drawFrame(2);

	/*FontAsset(U"Menu")(U"SOLO PLAY").drawAt(m_soloB.center(), ColorF{ 0.25 });
	FontAsset(U"Menu")(U"Ai PLAY").drawAt(m_aiB.center(), ColorF{ 0.25 });
	FontAsset(U"Menu")(U"SETTING").drawAt(m_setB.center(), ColorF{ 0.25 });*/
	FontAsset(U"Menu")(U"EXIT").drawAt(m_exitB.center(), ColorF{ 0.25 });

}
