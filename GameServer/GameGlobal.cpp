#include "GameGlobal.h"

#include "GameDrop.h"
#include "GameRoomManager.h"
#include "GameSkill.h"
#include "GameItem.h"

std::shared_ptr<GameRoomManager> GRoomManger = std::make_shared<GameRoomManager>();
std::shared_ptr<GameSkill> GSkill = std::make_shared<GameSkill>();
std::shared_ptr<GameItem> GItem = std::make_shared<GameItem>();
std::shared_ptr<GameDrop> GDropItem = std::make_shared<GameDrop>();

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
        GDropItem->ClearItem();
    }
    
}GGameGlobal;
