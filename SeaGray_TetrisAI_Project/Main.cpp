
//# include <Siv3D.hpp> // Siv3D v0.6.13

//# include "common.hpp"
# include "title.hpp"

void Main()
{
	// 背景の色を設定 | Set background color
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	// タイトル変更
	Window::SetTitle(U"SeaGray <TetrisAI> -v0.1-");

	// サイズ変更

	Window::SetStyle(WindowStyle::Sizable);
	Window::Resize(360, 720);
	Scene::SetResizeMode(ResizeMode::Keep);

	//FontAsset::Register(U"TitleFont", FontMethod::MSDF, 50, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	//FontAsset(U"TitleFont").setBufferThickness(4);
	FontAsset::Register(U"Menu", FontMethod::MSDF, 48, Typeface::Medium);
	//FontAsset::Register(U"Txt20px", FontMethod::MSDF, 20, Typeface::Medium);
	//FontAsset::Register(U"Ranking", 40, Typeface::Heavy);
	//FontAsset::Register(U"GameScore", 30, Typeface::Light);
	//AudioAsset::Register(U"Brick", GMInstrument::Woodblock, PianoKey::C5, 0.2s, 0.1s);

	App manager;
	manager.add<Title>(State::Title);
	//manager.add<Solo>(State::Solo);
	//manager.add<Ai>(State::Ai);
	//manager.add<Ranking>(State::Ranking);

	// 通常のフォントを作成 | Create a new font
	const Font font{ 30 };

	// 使用変数
	shig::BoolSwitch resize_flag(false);
	auto PreSceneRect = Window::GetState().virtualSize;
	int Resize_counter = -1; // 画面リサイズ処理の遅延用
	constexpr int Resize_delay = 0;
	//auto DefWindowScale = Window::GetState().scaling;

	while (System::Update())
	{
		auto SceneRect = Window::GetState().virtualSize;
		auto ScenePos = Window::GetPos();

		if (Window::GetState().sizeMove) {
			resize_flag.set(true);
		}
		else {
			if (resize_flag.get() && (SceneRect != PreSceneRect)) {
				Resize_counter = Resize_delay;
				resize_flag.set(false);
			}
		}

		if (Resize_counter == 0) {
			double Resized_x = ((double)SceneRect.x);
			double Resized_y = ((double)Resized_x * 9.0 / 16.0);
			Window::ResizeVirtual((int)Resized_x, (int)Resized_y, YesNo<Centering_tag>::No);
			Window::ResizeVirtual((int)Resized_x, (int)Resized_y, YesNo<Centering_tag>::No);
			Window::SetPos(ScenePos);
			resize_flag.set(false);
			Resize_counter = -1;
		}
		else {
			Resize_counter--;
		}


		if (not manager.update())
		{
			break;
		}

		// マウスカーソルに追随する半透明な円を描く | Draw a red transparent circle that follows the mouse cursor
		//Circle{ Cursor::PosF(), 5 }.draw(ColorF{ 1, 1, 1, 0.8 });


	}
}
