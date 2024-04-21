#pragma once
#include <fstream>
#include <boost/json.hpp>

#include "pch.h"

class GameInit
{
public:
    GameInit(boost::asio::io_context& io_context);

    void SetSkill(boost::json::value& unitJson, bool isMonster);

    void SetMap(boost::json::value& mapJson, boost::asio::io_context& io_context);
};
