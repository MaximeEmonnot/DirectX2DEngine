export module GameEngine:Levels;

import <codecvt>;
import <locale>;

import MainEngine;
import :UI;

export
class ConnectionLevel :
    public Level
{
public:
    ConnectionLevel()
    {
        CreateCanvas<UICanvas_Connection>();
    }

    virtual void    Update() override
    {
        Level::Update();
    }
    virtual void    BeginLevel() override
    {}
};

export
class MultiPlayerCombatLevel :
	public Level
{
public:
	MultiPlayerCombatLevel() = default;

	virtual void    Update() override
	{
		// Send Data
		if (NETWORK.GetPlace() == 1)
		{
			Level::Update();
			SendData();
		}
		else
		{
			SendData();
			Level::Update();
		}

		if (!(pPlayer->GetFighter()->IsAlive() && pEnemy->GetFighter()->IsAlive()))
		{
			SFX.Pause("Sounds\\BeJustOrBeDead.wav");
			NETWORK.Disconnect();
			ENGINE.SetLevel<VictoryLevel>();
			if (std::shared_ptr<VictoryLevel> new_level = std::dynamic_pointer_cast<VictoryLevel>(ENGINE.GetCurrentLevel()))
			{
				new_level->SetWinner(*(pPlayer->GetFighter()->IsAlive() ? pPlayer->GetFighter() : pEnemy->GetFighter()),
					(pPlayer->GetFighter()->IsAlive() && NETWORK.GetPlace() == 1) ? 1 : (pEnemy->GetFighter()->IsAlive() && NETWORK.GetPlace() == 2) ? 1 : 2);
				new_level->BeginLevel();
			}
		}
	}
	virtual void    BeginLevel() override
	{
		// Clear level from previous access
		Clear();

		// Spawn Actors
		GetWorld().SpawnActor<Background>(FVec2D(0, 180), "Background");
		switch (NETWORK.GetPlace())
		{
		case 1:
			pPlayer = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(-250, -125), "Player", static_cast<EFighterName>(selection.first - 1), 15);
			pEnemy = GetWorld().SpawnActor<FighterCharacter<NetworkEnemyController>>(FVec2D(250, -125), "Enemy", static_cast<EFighterName>(selection.second - 1), 12);
			// Create UI Canvas
			CreateCanvas<UICanvas_SinglePlayerCombat>(pPlayer->GetFighter(), pEnemy->GetFighter());
			break;
		case 2:
			pPlayer = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(250, -125), "Player", static_cast<EFighterName>(selection.first - 1), 15);
			pEnemy = GetWorld().SpawnActor<FighterCharacter<NetworkEnemyController>>(FVec2D(-250, -125), "Enemy", static_cast<EFighterName>(selection.second - 1), 12);
			// Create UI Canvas
			CreateCanvas<UICanvas_SinglePlayerCombat>(pEnemy->GetFighter(), pPlayer->GetFighter());
			break;
		default:
			break;
		}

		// Linking Player and Enemy so they face each other
		pPlayer->GetFighter()->SetEnemy(pEnemy->GetFighter());
		pEnemy->GetFighter()->SetEnemy(pPlayer->GetFighter());

		// Play background sounds
		SFX.Play("Sounds\\BeJustOrBeDead.wav");
	}

	void            SetSelection(std::pair<int, int> _selection)
	{
		selection = _selection;
	}

private:
	void            SendData()
	{
		FVec2D playerPosition = pPlayer->GetPosition();
		FVec2D enemyPosition = pEnemy->GetPosition();
		FightingController::EAction playerAction = std::dynamic_pointer_cast<FightingController>(pPlayer->GetController())->GetAction();
		FightingController::EAction enemyAction = std::dynamic_pointer_cast<FightingController>(pEnemy->GetController())->GetAction();

		// We reinterpret the floating point values as uint8_t arrays of size 4
		std::vector<uint8_t> output;
		output.push_back(static_cast<uint8_t>(20)); // Size of package
		output.push_back(static_cast<uint8_t>(NETWORK.GetPlace()));
		// Player data
		uint8_t* player_position_x = reinterpret_cast<uint8_t*>(&playerPosition.x);
		uint8_t* player_position_y = reinterpret_cast<uint8_t*>(&playerPosition.y);
		for (int i = 0; i < 4; i++) output.emplace_back(player_position_x[i]);
		for (int i = 0; i < 4; i++) output.emplace_back(player_position_y[i]);
		output.emplace_back(static_cast<uint8_t>(playerAction));
		// Enemy data
		uint8_t* enemy_position_x = reinterpret_cast<uint8_t*>(&enemyPosition.x);
		uint8_t* enemy_position_y = reinterpret_cast<uint8_t*>(&enemyPosition.y);
		for (int i = 0; i < 4; i++) output.emplace_back(enemy_position_x[i]);
		for (int i = 0; i < 4; i++) output.emplace_back(enemy_position_y[i]);
		output.emplace_back(static_cast<uint8_t>(enemyAction));

		NETWORK.SendDataUDP(output);
	}

private:
	std::shared_ptr<FighterCharacter<PlayerController>>         pPlayer;
	std::shared_ptr<FighterCharacter<NetworkEnemyController>>   pEnemy;

	std::pair<int, int>                                         selection;
};

export
class MultiPlayerSelectionLevel :
	public Level
{
public:
	MultiPlayerSelectionLevel()
	{
		CreateCanvas<UICanvas_MultiPlayerSelection>();
	}

	virtual void    Update() override
	{
		Level::Update();
	}
	virtual void    BeginLevel() override
	{}

	void            SetSelectionValue(int value)
	{
		selection = value;
	}
	int             GetSelection() const
	{
		return selection;
	}

private:
	int             selection = 0;
};

export
class SinglePlayerCombatLevel :
	public Level
{
public:
	SinglePlayerCombatLevel() = default;

	virtual void    Update() override
	{
		Level::Update();

		if (!(pPlayer->GetFighter()->IsAlive() && pEnemy->GetFighter()->IsAlive()))
		{
			SFX.Pause("Sounds\\BeJustOrBeDead.wav");
			ENGINE.SetLevel<VictoryLevel>();
			if (std::shared_ptr<VictoryLevel> new_level = std::dynamic_pointer_cast<VictoryLevel>(ENGINE.GetCurrentLevel()))
			{
				new_level->SetWinner(*(pPlayer->GetFighter()->IsAlive() ? pPlayer->GetFighter() : pEnemy->GetFighter()),
					pPlayer->GetFighter()->IsAlive() ? 1 : 2);
				new_level->BeginLevel();
			}
		}
	}
	virtual void    BeginLevel() override
	{
		// Clear level from previous access
		Clear();

		// Spawn Actors
		GetWorld().SpawnActor<Background>(FVec2D(0, 180), "Background");
		pPlayer = GetWorld().SpawnActor<FighterCharacter<PlayerController>>(FVec2D(-250, -25), "Player", static_cast<EFighterName>(selection.first - 1), 15);
		pEnemy = GetWorld().SpawnActor<FighterCharacter<DummyController>>(FVec2D(250, -25), "Enemy", static_cast<EFighterName>(selection.second - 1), 12);

		// Linking Player and Enemy so they face each other
		pPlayer->GetFighter()->SetEnemy(pEnemy->GetFighter());
		pEnemy->GetFighter()->SetEnemy(pPlayer->GetFighter());

		// Create UI Canvas
		CreateCanvas<UICanvas_SinglePlayerCombat>(pPlayer->GetFighter(), pEnemy->GetFighter());

		// Play background sounds
		SFX.Play("Sounds\\BeJustOrBeDead.wav");
	}

	void            SetSelection(std::pair<int, int> _selection)
	{
		selection = _selection;
	}
private:
	std::pair<int, int>     selection;

	std::shared_ptr<FighterCharacter<PlayerController>> pPlayer;
	std::shared_ptr<FighterCharacter<DummyController>> pEnemy;
};

export
class SinglePlayerSelectionLevel :
	public Level
{
public:
	SinglePlayerSelectionLevel()
	{
		CreateCanvas<UICanvas_SinglePlayerSelection>();
	}

	virtual void            Update() override
	{
		Level::Update();
	}
	virtual void            BeginLevel() override
	{
		selection = std::pair(0, 0);
	}

	void                    SetSelectionValue(bool bIsLeft, int value)
	{
		if (bIsLeft) selection.first = value;
		else selection.second = value;
	}
	std::pair<int, int>     GetSelection() const
	{
		return selection;
	}

private:
	std::pair<int, int>     selection = { 0, 0 };
};

export
class TitleLevel :
	public Level
{
public:
	TitleLevel()
	{
		CreateCanvas<UICanvas_Title>();
	}

	virtual void    Update() override
	{
		Level::Update();
	}
	virtual void    BeginLevel() override
	{}
};

export
class VictoryLevel :
	public Level
{
public:
	VictoryLevel() = default;

	virtual void    Update() override
	{
		Level::Update();
	}
	virtual void    BeginLevel() override
	{
		Clear();

		CreateCanvas<UICanvas_Victory>(winnerFighter, playerNumber);
	}

	void			SetWinner(const BaseFighter& newWinnerFighter, int newPlayerNumber)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		winnerFighter = converter.to_bytes(newWinnerFighter.GetName());
		std::erase_if(winnerFighter, [](char x) { return x == ' '; });
		playerNumber = newPlayerNumber;
	}

private:
	std::string		winnerFighter;
	int				playerNumber;
};