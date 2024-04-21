#include "GameGlobal.h"

#include "GameRoomManager.h"
#include "GameSkill.h"
#include "GameItem.h"

boost::shared_ptr<GameRoomManager> GRoomManger = boost::make_shared<GameRoomManager>();
boost::shared_ptr<GameSkill> GSkill = boost::make_shared<GameSkill>();
boost::shared_ptr<GameItem> GItem = boost::make_shared<GameItem>();

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
        GItem->ClearItem();
    }
    
}GGameGlobal;
