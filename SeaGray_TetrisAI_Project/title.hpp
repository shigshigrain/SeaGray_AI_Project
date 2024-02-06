# pragma once
# include "common.hpp"

const s3d::StringView Bg_pass = U"tex\\background\\tetris_emulator_background03.bmp";

// BOTメイン画面
class Title : public App::Scene
{
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;

	~Title();

private:

	std::atomic_bool StartFlag;
	uint8 connect_state;
	uint8 InputSpeed;

	std::atomic_int commandIndex;

	s3d::AsyncTask<bool> bot;
	Rect m_exitB;

	Transition m_exitT;
	Texture bg_tex;

	shig::PPT2bot graysea;

};

const std::vector<s3d::StringView> ConnectStatus = {
	U"not connected yet",
	U"connection successful",
	U"connection failed (please retry)",
};
