#pragma once


#ifdef _DEBUG
#pragma comment(lib, "CoreLib\\Debug\\CoreLib.lib")
#else
#pragma comment(lib, "CoreLib\\Release\\CoreLib.lib")
#endif

#include "CorePch.h"

// 일단 나중에 정리 !!!
using GameSessionRef = boost::shared_ptr<class GameSession>;
using GameServiceRef = boost::shared_ptr<class GameService>;

using GameMapInfoRef = boost::shared_ptr<class GameMapInfo>;
using MapInfoRef = boost::shared_ptr<class MapInfo>;

using GameObjectInfoRef = boost::shared_ptr<class GameObjectInfo>;
using GameMosterInfoRef = boost::shared_ptr<class GameMosterInfo>;
using GamePlayerInfoRef = boost::shared_ptr<class GamePlayerInfo>;

using GameRoomQuestRef = boost::shared_ptr<class GameRoomQuest>;
