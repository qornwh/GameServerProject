#include "GameGlobal.h"
#include "GameRoomManager.h"
#include "GameSkill.h"
#include "GameItem.h"

std::shared_ptr<GameRoomManager> GRoomManger = std::make_shared<GameRoomManager>();
std::shared_ptr<GameSkill> GSkill = std::make_shared<GameSkill>();
std::shared_ptr<GameItem> GItem = std::make_shared<GameItem>();

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
