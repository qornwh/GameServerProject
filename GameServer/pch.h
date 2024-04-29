#pragma once

#ifdef _DEBUG
#pragma comment(lib, "CoreLib\\Debug\\CoreLib.lib")
#pragma comment(lib, "EngineLib\\Debug\\GameEngine.lib")
#else
#pragma comment(lib, "CoreLib\\Release\\CoreLib.lib")
#pragma comment(lib, "EngineLib\\Release\\GameEngine.lib")
#endif

#include "CorePch.h"

using GameSessionRef = boost::shared_ptr<class GameSession>;
using GameServiceRef = boost::shared_ptr<class GameService>;

using GameMapInfoRef = boost::shared_ptr<class GameMapInfo>;
using MapInfoRef = boost::shared_ptr<class MapInfo>;

using GameRoomRef = boost::shared_ptr<class GameRoom>;
using GameObjectInfoRef = boost::shared_ptr<class GameObjectInfo>;
using GameMosterInfoRef = boost::shared_ptr<class GameMosterInfo>;
using GameBossInfoRef = boost::shared_ptr<class GameBossInfo>;
using GamePlayerInfoRef = boost::shared_ptr<class GamePlayerInfo>;

using GameRoomQuestRef = boost::shared_ptr<class GameRoomQuest>;
