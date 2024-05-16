DROP TABLE IF EXISTS Account
DROP TABLE IF EXISTS Player
DROP TABLE IF EXISTS Item
DROP TABLE IF EXISTS ItemEquipment
DROP TABLE IF EXISTS ItemDrop
DROP TABLE IF EXISTS Inventory
DROP TABLE IF EXISTS IpLog
DROP TABLE IF EXISTS QuestInfo
DROP TABLE IF EXISTS QuestState
DROP TABLE IF EXISTS QuestKillState

CREATE TABLE Account
(
	accountCode int IDENTITY(1,1) NOT NULL,
	id varchar(10) NOT NULL PRIMARY KEY,
	pwd varchar(10) NOT NULL,
)

CREATE TABLE Player
(
	playerCode int IDENTITY(1,1) NOT NULL,
	name varchar(10) NOT NULL PRIMARY KEY,
	jobCode int NOT NULL,
	mapCode int NOT NULL,
	accountCode int NOT NULL,
    gold int NOT NULL DEFAULT 0,
)

CREATE TABLE Item
(
	itemCode int NOT NULL PRIMARY KEY,
	type int NOT NULL,
	name varchar(20) NOT NULL,
	maxSize int NOT NULL,
	attack int NOT Null DEFAULT 0,
)

CREATE TABLE ItemDrop
(
    itemCode int NOT NULL,
    monsterCode int NOT NULL,
    CONSTRAINT ItemDrop_PK PRIMARY KEY(itemCode,monsterCode)
)

CREATE TABLE Inventory
(
	playerCode int NOT NULL,
	itemCode int NOT NULL,
	itemType int NOT NULL,
    itemCount int NOT NULL,
)

CREATE TABLE IpLog
(
	accountCode int NOT NULL,
	ip varchar(10) NOT NULL,
	currentTime smalldatetime NOT NULL default getdate()
)

CREATE TABLE QuestInfo
(
	questCode int NOT NULL PRIMARY KEY,
	name varchar(20) NOT NULL,
	type int NOT NULL,
)

CREATE TABLE QuestState
(
	questStateId int NOT NULL PRIMARY KEY,
	questState int NOT NULL,
	questCode int NOT NULL,
	playerCode int NOT NULL,
)

CREATE TABLE QuestKillState
(
	questStateId int NOT NULL,
	playerCode int NOT NULL,
	monsterCode int NOT NULL,
	monsterKill int NOT NULL
)

INSERT INTO Account (id, pwd) VALUES
('Gm', 1234)

INSERT INTO Player (name, jobCode, mapCode, accountCode) VALUES
('GmPlayer', 1, 1, 1)

INSERT INTO Item (itemCode, type, name, maxSize, attack) VALUES 
(1, 3, '해골 기사의 뼈조각', 200, 0),
(2, 3, '해골 궁수의 천조각', 200, 0),
(3, 3, '기마병의 심장', 200, 0),
(101, 2, '체력 회복 포션', 200, 0),
(1001, 1, '낡은 검', 1, 100),
(1002, 1, '낡은 활', 1, 150)

INSERT INTO QuestInfo (questCode, name, type) VALUES
(1, '병사 처치', 2),
(2, '재료 수급', 2)

INSERT INTO ItemDrop (itemCode, monsterCode) VALUES
(1001, 0),
(1, 0),
(4, 0),
(1002, 1),
(2, 1),
(4, 1)

INSERT INTO Inventory (playerCode, itemCode, itemType, itemCount) VALUES
(1, 1, 3, 10),
(1, 2, 3, 10),
(1, 101, 2, 10),
(1, 1001, 1, 1)
