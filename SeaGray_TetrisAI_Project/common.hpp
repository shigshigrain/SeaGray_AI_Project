# pragma once

# include <Siv3D.hpp>
# include "PPT2bot.hpp"
//# include "shigune_AI.h"
//# include "ShigUtility.h"
//# include "KeyConf.hpp"

using namespace shig;

// シーンの名前
enum class State
{
	Title,
	Solo,
	Ai,
	Setting,
};



// 共有するデータ
struct GameData
{

	//TetriEngine shared_TE;
	//shig::AiShigune shared_AI;

};

using App = SceneManager<State, GameData>;

constexpr uint64 refrashRate15 = 1000 / 15;
constexpr uint64 refrashRate30 = 1000 / 30;
constexpr uint64 refrashRate60 = 1000 / 60;
constexpr uint64 refrashRate120 = 1000 / 120;



static const std::vector<Color> minoC{
	Color(255, 255, 255), // px=0
	Color(0, 191, 255), // px=1
	Color(66, 66, 255), // px=2
	Color(255, 165, 0), // px=3
	Color(255, 255, 0), // px=4
	Color(0, 208, 0), // px=5
	Color(156, 73, 231), // px=6
	Color(255, 80, 49), // px=7
	Color(119, 136, 153), // px=8
	Color(128, 223, 255), // px=-1
	Color(104, 104, 255), // px=-2
	Color(255, 214, 136), // px=-3
	Color(255, 255, 155), // px=-4
	Color(140, 208, 140), // px=-5
	Color(208, 169, 243), // px=-6
	Color(255, 139, 119), // px=-7
	Color(0, 128, 128) // px=-8
};

constexpr int minotex_path_size = 16;

const std::vector<StringView> minotex_path = {
	U"tex\\mino\\flame_L.bmp",
	U"tex\\mino\\I-mino_100.bmp",
	U"tex\\mino\\J-mino_100.bmp",
	U"tex\\mino\\L-mino_100.bmp",
	U"tex\\mino\\O-mino_100.bmp",
	U"tex\\mino\\S-mino_100.bmp",
	U"tex\\mino\\T-mino_100.bmp",
	U"tex\\mino\\Z-mino_100.bmp",
	U"tex\\mino\\flame_S.bmp",
	U"tex\\mino\\I-mino_90.bmp",
	U"tex\\mino\\J-mino_90.bmp",
	U"tex\\mino\\L-mino_90.bmp",
	U"tex\\mino\\O-mino_90.bmp",
	U"tex\\mino\\S-mino_90.bmp",
	U"tex\\mino\\T-mino_90.bmp",
	U"tex\\mino\\Z-mino_90.bmp",
};
