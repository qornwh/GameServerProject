#include "GameInit.h"
#include "GameGlobal.h"
#include "GameMapInfo.h"
#include "GameSkill.h"
#include "IRoom.h"

GameInit::GameInit(boost::asio::io_context& io_context)
{
	string configFilePath = "./config.json";
	string jsonStr = "";
	boost::json::value json;

	ifstream openFile(configFilePath.data());

	CrashFunc(openFile.is_open());

	string line = "";
	while (getline(openFile, line))
		jsonStr.append(line);
	openFile.close();
	json = GameUtils::JsonParser::GetStrParser(jsonStr);

	boost::json::value unitJson = GameUtils::JsonParser::Parser("units", json);
	boost::json::value playerJson = GameUtils::JsonParser::Parser("players", unitJson);
	boost::json::value monsterJson = GameUtils::JsonParser::Parser("monsters", unitJson);
	boost::json::value mapJson = GameUtils::JsonParser::Parser("maps", json);

	// skill 설정
	SetSkill(playerJson, false);
	SetSkill(monsterJson, true);

	// map 설정
	SetMap(mapJson, io_context);

	// 
}

void GameInit::SetSkill(boost::json::value& unitJson, bool isMonster)
{
	for (auto& unit : unitJson.as_array())
	{
		int32 type = GameUtils::JsonParser::Parser("type", unit).get_int64();
		if (!isMonster)
			GSkill->AddPlayerSkill(type);
		else
			GSkill->AddMonsterSkill(type);
		boost::json::value skillJson = GameUtils::JsonParser::Parser("skills", unit);
		for (auto& skill : skillJson.as_array())
		{
			int32 skillCode = GameUtils::JsonParser::Parser("code", skill).as_int64();
			int32 isRectType = GameUtils::JsonParser::Parser("type", skill).as_int64();
			bool target = GameUtils::JsonParser::Parser("target", skill).as_bool();
			if (isRectType == 0)
			{
				int32 width = GameUtils::JsonParser::Parser("width", skill).as_int64();
				int32 height = GameUtils::JsonParser::Parser("height", skill).as_int64();
				int32 damage = GameUtils::JsonParser::Parser("damage", skill).as_int64();
				if (!isMonster)
					GSkill->GetPlayerSkill()[type].AddSkill(skillCode, isRectType, target, width, height, damage);
				else
					GSkill->GetMonsterSkill()[type].AddSkill(skillCode, isRectType, target, width, height, damage);
			}
			else if (isRectType == 1)
			{
				int32 radius = GameUtils::JsonParser::Parser("radius", skill).as_int64();
				int32 damage = GameUtils::JsonParser::Parser("damage", skill).as_int64();
				if (!isMonster)
					GSkill->GetPlayerSkill()[type].AddSkill(skillCode, isRectType, target, radius, damage);
				else
					GSkill->GetMonsterSkill()[type].AddSkill(skillCode, isRectType, target, radius, damage);
			}
			else if (isRectType == 2)
			{
				int32 heal = GameUtils::JsonParser::Parser("heal", skill).as_int64();
				if (!isMonster)
					GSkill->GetPlayerSkill()[type].AddSkill(skillCode, isRectType, target, heal);
				else
					GSkill->GetMonsterSkill()[type].AddSkill(skillCode, isRectType, target, heal);
			}
		}
	}
}

void GameInit::SetMap(boost::json::value& mapJson, boost::asio::io_context& io_context)
{
	for (int i = 0; i < mapJson.get_array().size(); i++)
	{
		auto& map = mapJson.get_array()[i];
		int32 type = GameUtils::JsonParser::Parser("type", map).get_int64();

		int32 code = GameUtils::JsonParser::Parser("mapCode", map).get_int64();
		int32 x = GameUtils::JsonParser::Parser("x", map).get_int64();
		int32 y = GameUtils::JsonParser::Parser("y", map).get_int64();
		int32 centerX = GameUtils::JsonParser::Parser("centerX", map).get_int64();
		int32 centerY = GameUtils::JsonParser::Parser("centerY", map).get_int64();

		GameRoomRef room = GRoomManger->CreateRoom(io_context, code);
		if (room == nullptr)
		{
			// not create room
			CrashFunc(false);
		}
		room->CreateMapInfo(type);
		room->GetGameMap()->SetMapCode(code);
		room->GetGameMap()->GetMapInfo().GetRect().X = x;
		room->GetGameMap()->GetMapInfo().GetRect().Y = y;
		room->GetGameMap()->GetMapInfo().GetRect().CenterX = centerX;
		room->GetGameMap()->GetMapInfo().GetRect().CenterY = centerY;

		if (type > 0)
		{
			boost::json::value monsterMapJson = GameUtils::JsonParser::Parser("monsterMap", map);

			x = GameUtils::JsonParser::Parser("x", monsterMapJson).get_int64();
			y = GameUtils::JsonParser::Parser("y", monsterMapJson).get_int64();
			centerX = GameUtils::JsonParser::Parser("centerX", monsterMapJson).get_int64();
			centerY = GameUtils::JsonParser::Parser("centerY", monsterMapJson).get_int64();

			room->GetGameMap()->GetMonsterMapInfo()->GetRect().X = x;
			room->GetGameMap()->GetMonsterMapInfo()->GetRect().Y = y;
			room->GetGameMap()->GetMonsterMapInfo()->GetRect().CenterX = centerX;
			room->GetGameMap()->GetMonsterMapInfo()->GetRect().CenterY = centerY;
		}
		room->InitMonsters();
	}
}
