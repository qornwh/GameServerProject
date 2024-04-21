DROP TABLE IF EXISTS Account
DROP TABLE IF EXISTS Player
DROP TABLE IF EXISTS Item
DROP TABLE IF EXISTS ItemEquipment
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
)

CREATE TABLE Item
(
	itemCode int NOT NULL PRIMARY KEY,
	type int NOT NULL,
	name varchar(20) NOT NULL,
	maxSize int NOT NULL,
)

CREATE TABLE ItemEquipment
(
	itemCode int NOT NULL PRIMARY KEY,
	type int NOT NULL,
	name varchar(20) NOT NULL,
	attack int Not Null,
)

CREATE TABLE Inventory
(
	playerCode int NOT NULL,
	itemCode int NOT NULL,
	itemType int NOT NULL,
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
('Gm1', 1234)

INSERT INTO Player (name, jobCode, mapCode, accountCode) VALUES
('Gm1', 1, 1, 1)

INSERT INTO Item (itemCode, type, name, maxSize) VALUES 
(1, 1, '해골 기사의 뼈조각', 200),
(2, 1, '해골 궁수의 천조각', 200),
(3, 1, '기마병의 심장', 200),
(4, 2, '체력 회복 포션', 200)

INSERT INTO ItemEquipment (itemCode, type, name, attack) VALUES
(1, 1, '낡은 검', 100),
(2, 1, '낡은 활', 150)

INSERT INTO QuestInfo (questCode, name, type) VALUES
(1, '병사 처치', 2),
(2, '재료 수급', 2)

