﻿# pragma once
# include "common.hpp"

const s3d::String Bg_pass = U"tex\\background\\tetris_emulator_background03.bmp";

// タイトルシーン
class Title : public App::Scene
{
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;

private:

	std::atomic_bool StartFlag;
	double InputSpeed;

	Rect m_exitB;
	Transition m_exitT;

	Texture bg_tex;



};
