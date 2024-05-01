#pragma once

// iocp 사용모드, boost::asio 사용모드 분기처리용 
// #define IOCPMODE 0

#ifdef _DEBUG
// #pragma comment(lib, "CoreLibIOCP\\Debug\\CoreLibIOCP.lib")
#pragma comment(lib, "CoreLib\\Debug\\CoreLib.lib")
#pragma comment(lib, "EngineLib\\Debug\\GameEngine.lib")
#else
#pragma comment(lib, "CoreLib\\Release\\CoreLib.lib")
#pragma comment(lib, "EngineLib\\Release\\GameEngine.lib")
#endif

#include "CorePch.h"

using GameSessionRef = std::shared_ptr<class GameSession>;
using GameServiceRef = std::shared_ptr<class GameService>;
using GameMapInfoRef = std::shared_ptr<class GameMapInfo>;
using MapInfoRef = std::shared_ptr<class MapInfo>;
using GameRoomRef = std::shared_ptr<class GameRoom>;
using GameObjectInfoRef = std::shared_ptr<class GameObjectInfo>;
using GameMosterInfoRef = std::shared_ptr<class GameMosterInfo>;
using GameBossInfoRef = std::shared_ptr<class GameBossInfo>;
using GamePlayerInfoRef = std::shared_ptr<class GamePlayerInfo>;
using GameRoomQuestRef = std::shared_ptr<class GameRoomQuest>;
