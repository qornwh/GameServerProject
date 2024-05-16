#include "pch.h"
#include "GameSession.h"
#include "GameGlobal.h"
#include "GameService.pb.h"
#include "PacketHeader.h"
#include "GamePacketHandler.h"
#include "GameRoomManager.h"
#include "GameObjectInfo.h"
#include "IRoom.h"
#include "SessionDB.h"

#ifdef IOCPMODE
GameSession::GameSession(EndPointUtil& ep) : Session(ep)
{
	static Atomic<uint16> GameSessionId(0);
	GameSessionId.fetch_add(1);
	_sessionId = GameSessionId;
}

GameSession::~GameSession()
{
    std::cout << "LogOut ID: " << _logId << std::endl;
}
#else
GameSession::GameSession(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep) : Session(
	io_context, ep)
{
	static Atomic<uint16> GameSessionId(0);
	GameSessionId.fetch_add(1);
	_sessionId = GameSessionId;
}
#endif

int32 GameSession::OnRecv(BYTE* buffer, int32 len)
{
	int32 offset = 0;
	while (true)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(&buffer[offset]);
		if (!GamePacketHandler::CheckPacketHeader(header, offset, len))
			break;
		HandlePacket(buffer, offset, header);
		offset += header->size;
	}
	return offset;
}

void GameSession::CreatePlayerInfo(int32 type, int32 hp)
{
	_player = std::make_shared<GamePlayerInfo>(std::reinterpret_pointer_cast<GameSession>(shared_from_this()), _sessionId,
		type, hp);
}

std::shared_ptr<GamePlayerInfo> GameSession::GetPlayer()
{
	return _player;
}

void GameSession::HandlePacket(BYTE* buffer, int32 offset, PacketHeader* header)
{
	uint16 id = header->id;

	switch (id)
	{
	case protocol::MessageCode::LOGIN:
	{
		LoginHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
	}
		break;
	case protocol::MessageCode::CREATECHARACTER:
	{
		CreateCharacterHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
	}
		break;
	case protocol::MessageCode::C_LOAD:
	{
		LoadHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));

		if (GRoomManger->getRoom(0) != nullptr)
		{
			GRoomManger->getRoom(0)->EnterSession(std::static_pointer_cast<GameSession>(shared_from_this()));
		}
	}
		break;
	case protocol::MessageCode::S_MOVE:
	{
		MoveHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
	}
		break;
	case protocol::MessageCode::S_CHAT:
	{
		ChatHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
	}
		break;
	case protocol::MessageCode::S_PLAYERDATA:
		break;
	case protocol::MessageCode::C_PLAYERATTACK:
	{
		AttackHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
	}
		break;
	case protocol::MessageCode::C_MOVEPOTAL:
	{
		ChangeRoomHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
	}
		break;
	}
}

void GameSession::MoveHandler(BYTE* buffer, PacketHeader* header, int32 offset)
{
	protocol::SMove pkt;

	if (GamePacketHandler::ParsePacketHandler(pkt, buffer, header->size - offset, offset))
	{
		auto& position = pkt.position();

		// 유니티 왼손 법칙이라 이렇게 받는다.
		GetPlayer()->SetPosition(position.z(), position.x());
		GetPlayer()->SetRotate(position.yaw());
		pkt.set_code(_sessionId);
		protocol::Position* playerPosition = new protocol::Position;
		playerPosition->set_x(position.x());
		playerPosition->set_y(position.y());
		playerPosition->set_z(position.z());
		playerPosition->set_yaw(position.yaw());
		pkt.set_allocated_position(playerPosition);
		pkt.set_is_monster(false);

		SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::S_MOVE);
		GetService()->BroadCast(sendBuffer);
	}
}

void GameSession::LoginHandler(BYTE* buffer, PacketHeader* header, int32 offset)
{
	protocol::Login readPkt;
	if (GamePacketHandler::ParsePacketHandler(readPkt, buffer, header->size - offset, offset))
	{
		SessionDB sdb;
		WCHAR* wId = GameUtils::Utils::CharToWchar(readPkt.id().c_str());
		WCHAR* wPwd = GameUtils::Utils::CharToWchar(readPkt.pwd().c_str());
		bool isAccount = sdb.LoginDB(wId, _accountCode);
		if (isAccount)
		{
			bool loginCheck = sdb.LoginCheck(wPwd);
			protocol::LoginAccess logPkt;
			logPkt.set_success(loginCheck);
			if (loginCheck)
			{
				// 로그인 성공
				if (sdb.PlayerDB(_accountCode))
				{
					int32 playerCode = 0;
					int32 jobCode = 0;
					int32 mapCode = 0;
					WCHAR name[10] = {0 ,};
					sdb.GetPlayerDBInfo(playerCode, name, jobCode, mapCode);
					protocol::Charater* charater = logPkt.add_charater();
					charater->set_code(playerCode);
					charater->set_type(jobCode);
					char* nameByte = GameUtils::Utils::WcharToChar(name);
					charater->set_name(nameByte);
				}
				_logId.append(readPkt.id());
				std::cout << "Login Access ID: " << readPkt.id().c_str() << std::endl;
			}
			SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(
				logPkt, protocol::MessageCode::LOGINACCESS);
			AsyncWrite(sendBuffer);
		}
		else
		{
			// 계정 생성
			bool result = sdb.CreateAccount(wId, wPwd);
			protocol::CreateAccount caPkt;
			caPkt.set_success(result);
			SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(
				caPkt, protocol::MessageCode::CREATEACCOUNT);
			AsyncWrite(sendBuffer);
			if (result)
				std::cout << "Create Account ID: " << readPkt.id().c_str() << std::endl;
		}
		delete wId;
		delete wPwd;
	}
}

void GameSession::CreateCharacterHandler(BYTE* buffer, PacketHeader* header, int32 offset)
{
	protocol::CreateCharacter pkt;
	if (GamePacketHandler::ParsePacketHandler(pkt, buffer, header->size - offset, offset))
	{
		SessionDB sdb;
		WCHAR* wName = GameUtils::Utils::CharToWchar(pkt.charater().name().c_str());
		int32 type = pkt.charater().type();
		CrashFunc(_accountCode >= 0);
		bool result = sdb.CreateCharacter(wName, type, _accountCode);
		
		if (result)
		{
			if (sdb.PlayerDB(_accountCode))
			{
				int32 playerCode = 0;
				int32 jobCode = 0;
				int32 mapCode = 0;
				WCHAR name[10] = { 0 , };
				sdb.GetPlayerDBInfo(playerCode, name, jobCode, mapCode);
				protocol::Charater* character = new protocol::Charater();
				char* nameByte = GameUtils::Utils::WcharToChar(name);
				character->set_name(nameByte);
				character->set_type(jobCode);
				character->set_code(playerCode);
				pkt.set_allocated_charater(character);

				std::cout << "Create Charater : " << pkt.charater().name().c_str()
					<< " jobCode : " << jobCode << " playerCode : " << playerCode << std::endl;
			}
		}
		SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(
			pkt, protocol::MessageCode::CREATECHARACTER);
		AsyncWrite(sendBuffer);
	}
}

void GameSession::LoadHandler(BYTE* buffer, PacketHeader* header, int32 offset)
{
	protocol::CLoad readPkt;

	if (GamePacketHandler::ParsePacketHandler(readPkt, buffer, header->size - offset, offset))
	{
		SessionDB sdb;
		bool result = sdb.PlayerDB(_accountCode);
		if (result)
		{
			int32 jobCode = 0;
			int32 mapCode = 0;
			WCHAR name[10] = { 0 , };
			sdb.GetPlayerDBInfo(_playerCode, name, jobCode, mapCode);
			String nameStr(GameUtils::Utils::WcharToChar(name));

			CreatePlayerInfo(jobCode, 1000);
			GetPlayer()->SetName(nameStr);
			GetPlayer()->SetPlayerCode(_playerCode);

			// 나를 확인용 메시지 전달.
			if (GetService() != nullptr)
			{
				protocol::SPlayerData sendPkt;
				protocol::Player* player = new protocol::Player();
				protocol::Unit* unit = new protocol::Unit();
				unit->set_name(_player->GetName());
				unit->set_code(_player->GetCode());
				unit->set_type(_player->GetType());
				unit->set_hp(_player->GetHp());
				player->set_allocated_unit(unit);
				sendPkt.set_allocated_player(player);

				// 초기 위치 설정
				GetPlayer()->SetPosition(-15, 0);
				GetPlayer()->SetRotate(0);

				SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(
					sendPkt, protocol::MessageCode::S_PLAYERDATA);
				AsyncWrite(sendBuffer);
			}
		}
		else
		{
			// 더미 클라이언트만 !!!
			auto& position = readPkt.position();
			GetPlayer()->SetPosition(position.z(), position.x());
			GetPlayer()->SetRotate(position.yaw());
		}
	}
}

void GameSession::ChatHandler(BYTE* buffer, PacketHeader* header, int32 offset)
{
	protocol::SChat pkt;
	if (GamePacketHandler::ParsePacketHandler(pkt, buffer, header->size - offset, offset))
	{
		pkt.set_code(GetPlayer()->GetCode());
		SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::S_CHAT);
		// 해당 함수는 보류다 바로바로 가야 되므로 strand처리를 제외한다.
		if (GRoomManger->getRoom(_roomId) != nullptr)
		{
			GRoomManger->getRoom(_roomId)->BroadCast(sendBuffer);
		}
	}
}

void GameSession::AttackHandler(BYTE* buffer, PacketHeader* header, int32 offset)
{
	protocol::CPlayerAttack pkt;
	if (GRoomManger->getRoom(GetRoomId()) != nullptr)
	{
		if (GamePacketHandler::ParsePacketHandler(pkt, buffer, header->size - offset, offset))
		{
			int32 SkillCode = pkt.skill_code();
			int32 TargetCode = pkt.target_code();

			auto& position = pkt.position();
			GetPlayer()->SetPosition(position.z(), position.x());
			GetPlayer()->SetRotate(position.yaw());
			GetPlayer()->SetObjecteState(static_cast<ObjectState>(SkillCode));
			GetPlayer()->SetTarget(TargetCode);

			if (ObjectState::SKILL2 == SkillCode)
			{
				// 힐, 버프 등.
				GRoomManger->getRoom(GetRoomId())->BuffSession(std::static_pointer_cast<GameSession>(shared_from_this()));
			}
			else
			{
				// 공격
				GRoomManger->getRoom(GetRoomId())->AttackSession(std::static_pointer_cast<GameSession>(shared_from_this()));

				protocol::SUnitAttack sendPkt;
				protocol::Attack* attackPkt = sendPkt.add_attack();
				attackPkt->set_code(GetPlayer()->GetCode());
				attackPkt->set_is_monster(false);
				attackPkt->set_skill_code(SkillCode);
				protocol::Position* playerPosition = new protocol::Position();
				playerPosition->set_x(position.x());
				playerPosition->set_y(position.y());
				playerPosition->set_z(position.z());
				playerPosition->set_yaw(position.yaw());
				attackPkt->set_allocated_position(playerPosition);

				SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(
					sendPkt, protocol::MessageCode::S_UNITATTACK);
				GRoomManger->getRoom(GetRoomId())->BroadCastAnother(sendBuffer, GetPlayer()->GetCode());
			}
		}
	}
}

void GameSession::ChangeRoomHandler(BYTE* buffer, PacketHeader* header, int32 offset)
{
	protocol::CMovePotal readPkt;

	if (GamePacketHandler::ParsePacketHandler(readPkt, buffer, header->size - offset, offset))
	{
		int32 currentRoomId = readPkt.pre_room_id();
		int32 nextRoomId = readPkt.next_room_id();

		if (GRoomManger->getRoom(GetRoomId()) != nullptr)
		{
			GRoomManger->getRoom(GetRoomId())->OutSession(std::static_pointer_cast<GameSession>(shared_from_this()));
		}

		if (currentRoomId == 1)
		{
			Disconnect();
		}
		else if (GRoomManger->getRoom(nextRoomId) != nullptr)
		{
			// 초기 위치 설정
			GetPlayer()->SetPosition(-20, 0);
			GetPlayer()->SetRotate(0);
			GRoomManger->getRoom(nextRoomId)->EnterSession(std::static_pointer_cast<GameSession>(shared_from_this()));
		}
	}
}
