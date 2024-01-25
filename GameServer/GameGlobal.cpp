#include "GameGlobal.h"

#include "GameRoomManager.h"
#include "GameSkill.h"

boost::shared_ptr<GameRoomManager> GRoomManger = boost::make_shared<GameRoomManager>();
boost::shared_ptr<GameSkill> GSkill = boost::make_shared<GameSkill>();

class GameGlobal
{
public:
    GameGlobal()
    {
    }
    ~GameGlobal()
    {
        GRoomManger->ClearRoom();
        GSkill->ClearSkill();
    }
    
}GGameGlobal;
