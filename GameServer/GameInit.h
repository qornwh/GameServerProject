﻿#pragma once
#include <fstream>
#include <boost/json.hpp>

#include "GameGlobal.h"
#include "GameSkill.h"
#include "GameUtils.h"

class GameInit
{
public:
    GameInit()
    {
        string configFilePath = "./config.json";
        string jsonStr = "";
        boost::json::value json;

        ifstream openFile(configFilePath.data());
        if (openFile.is_open())
        {
            string line = "";
            while (getline(openFile, line))
                jsonStr.append(line);
            openFile.close();
            json = GameUtils::JsonParser::GetStrParser(jsonStr);
        }

        boost::json::value objJson = GameUtils::JsonParser::Parser("units", json);
        boost::json::value playerJson = GameUtils::JsonParser::Parser("players", objJson);
        boost::json::value monsterJson = GameUtils::JsonParser::Parser("monsters", objJson);

        SetSkill(playerJson, false);
        SetSkill(monsterJson, true);

        cout << "skill !!!" << endl;
    }

    void SetSkill(boost::json::value& unitJson, bool isMonster)
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
                    if (!isMonster)
                        GSkill->GetPlayerSkill()[skillCode].AddSkill(skillCode, isRectType, target, width, height);
                    else
                        GSkill->GetMonsterSkill()[skillCode].AddSkill(skillCode, isRectType, target, width, height);
                }
                else
                {
                    int32 radius = GameUtils::JsonParser::Parser("radius", skill).as_int64();
                    if (!isMonster)
                        GSkill->GetPlayerSkill()[skillCode].AddSkill(skillCode, isRectType, target, radius);
                    else
                        GSkill->GetMonsterSkill()[skillCode].AddSkill(skillCode, isRectType, target, radius);
                }
            }
        }
    }
};
