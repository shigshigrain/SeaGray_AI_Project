# include "title.hpp"

Title::Title(const InitData& init)
	: IScene{ init } {

	StartFlag = false;
	connect_state = (uint8)0;
	InputSpeed = (uint8)100;

	commandIndex = 0;

	m_exitB = Rect{ Point(100, 650), 50, 160};
	m_exitT = Transition{ 0.6s, 0.3s };

	bg_tex = Texture{ U"tex\\background\\tetris_emulator_background03.bmp"};

	//graysea.Initialize();

}

void Title::update()
{

	m_exitT.update(m_exitB.mouseOver());

	if (SimpleGUI::Button(U"Bot initializing", Vec2{ 60, 120 }, 240)) {
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

	if (SimpleGUI::Button(U"AI starting", Vec2{ 60, 180 }, 240)) {
		if (connect_state == 1 && StartFlag == false) {
			StartFlag = true;
			bot = Async(shig::ExecuteRun, std::ref(graysea), std::ref(StartFlag), std::ref(commandIndex));
		}
	}

	if (SimpleGUI::Button(U"AI stopping", Vec2{ 60, 240 }, 240)) {
		if (connect_state == 1) {
			StartFlag = false;
		}
	}

	if (m_exitB.leftClicked())
	{
		// 終了
		StartFlag = false;
		if (bot.isValid())bot.wait();
		System::Exit();
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });
	bg_tex.draw(0, 0);

	/*FontAsset(U"TitleFont")(U"TetrisEmulator2 -v1.1-")
		.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 80, Vec2{ 600, 60 });*/

	FontAsset(U"Menu")(ConnectStatus.at(connect_state)).draw(20, s3d::Vec2{90, 150}, Palette::Tomato);

	m_exitB.draw(ColorF{ 1.0, 0.5 + m_exitT.value() / 2 }).drawFrame(2);

	FontAsset(U"Menu")(U"EXIT").drawAt(m_exitB.center(), ColorF{ 0.25 });

}

Title::~Title()
{
	StartFlag = false;
	if (bot.isValid())bot.wait();
	graysea.Destroy();

}
