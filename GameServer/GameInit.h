#pragma once
#include <boost/json.hpp>

#ifndef IOCPMODE
#include <boost/asio.hpp>
#endif

class GameInit
{
public:
#ifdef IOCPMODE
    GameInit();
    void SetMap(boost::json::value& mapJson);
#else
    GameInit(boost::asio::io_context& io_context);
    void SetMap(boost::json::value& mapJson, boost::asio::io_context& io_context);
#endif
    void SetSkill(boost::json::value& unitJson, bool isMonster);
    void SetItem();
    void SetDropItem();
};
