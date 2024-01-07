#include "GameGlobal.h"

#include "GameRoomManager.h"

boost::shared_ptr<GameRoomManager> GRoomManger = boost::make_shared<GameRoomManager>();

class GameGlobal
{
public:
    GameGlobal()
    {
    }
    ~GameGlobal()
    {
        GRoomManger->ClearRoom();
    }
    
}GGameGlobal;
