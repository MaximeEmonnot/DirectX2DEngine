module;
#include <DirectXMath.h>
#include <d2d1helper.h>

export module GameEngine:UI;

import <memory>;
import <string>;
import <vector>;

import UIEngine;
import MathEngine;
import NetworkEngine;
import MainEngine;
import CoreEngine;

import :Levels;
import :Fighters;

export
class UICanvas_Connection :
    public AUICanvas
{
public:
    UICanvas_Connection()
        :
        pIpAddressTextBox(CreateUIElement<UITextBox>(FRect(0.f, 150.f, 600.f, 75.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), L"Cambria", L"Server IP Address")),
        pPortTextBox(CreateUIElement<UITextBox>(FRect(0.f, 0.f, 600.f, 75.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), L"Cambria", L"Server port")),
        pConnectionButton(CreateUIElement<UIButton>(FRect(0.f, -150.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
        pConnectionButtonTitle(CreateUIElement<UIText>(FRect(0.f, -150.f, 150.f, 75.f), L"Connect to Server", L"Cambria", 16.f, D2D1::ColorF(D2D1::ColorF::Black)))

    {
        pConnectionButton->SetTask([&]
            {
                NETWORK.ConnectTo(pIpAddressTextBox->GetText(), std::stoi(pPortTextBox->GetText()));
                ENGINE.SetLevel<MultiPlayerSelectionLevel>();
            });
    }

    virtual void    Update() override
    {
        pIpAddressTextBox->Update();
        pPortTextBox->Update();
        pConnectionButton->Update();
    }
private:
    std::shared_ptr<UITextBox>  pIpAddressTextBox;
    std::shared_ptr<UITextBox>  pPortTextBox;
    std::shared_ptr<UIButton>   pConnectionButton;
    std::shared_ptr<UIText>     pConnectionButtonTitle;
};

export
class UICanvas_MultiPlayerCombat :
    public AUICanvas
{
};

export
class UICanvas_MultiPlayerSelection :
	public AUICanvas
{
public:
	UICanvas_MultiPlayerSelection()
		:
		pSolBadguyImage(CreateUIElement<UIImage>(FRect(-100.f, 78.75f, 149.86f, 317.5f), "Textures/SolBadguy/selection.tga")),
		pSolBadguyButton(CreateUIElement<UIButton>(FRect(-100.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
		pSolBadguyButtonTitle(CreateUIElement<UIText>(FRect(-100.f, -200.f, 150.f, 75.f), L"Sol Badguy", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black))),
		pRoboKyImage(CreateUIElement<UIImage>(FRect(100.f, 78.75f, 256.413f, 317.5f), "Textures/RoboKy/selection.tga")),
		pRoboKyButton(CreateUIElement<UIButton>(FRect(100.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
		pRoboKyButtonTitle(CreateUIElement<UIText>(FRect(100.f, -200.f, 150.f, 75.f), L"Robo Ky", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black)))
	{
		pSolBadguyButton->SetTask(
			[&]
			{
				SelectFighter(1);
			}
			);

		pRoboKyButton->SetTask(
			[&]
			{
				SelectFighter(2);
			}
			);
	}

	void    Update() override
	{
		pSolBadguyButton->Update();
		pRoboKyButton->Update();
	}

private:
	void    SelectFighter(int fighter_value) const
	{
		if (std::shared_ptr<MultiPlayerSelectionLevel> level = std::dynamic_pointer_cast<MultiPlayerSelectionLevel>(ENGINE.GetCurrentLevel()))
		{
			std::vector<uint8_t> input;

			switch (NETWORK.GetPlace())
			{
			case 1: // If the client is the first one connected, we should wait for another client to send data
			{
				input = NETWORK.ReceiveDataTCP();
				std::vector<uint8_t> output;
				output.emplace_back(fighter_value);
				NETWORK.SendDataTCP(output);
			}
			break;
			case 2: // If the client is the second one connected, we should send the data first
			{
				std::vector<uint8_t> output;
				output.emplace_back(fighter_value);
				NETWORK.SendDataTCP(output);
				input = NETWORK.ReceiveDataTCP();
			}
			break;
			default:
				break;
			}
			TICKCLOCK(); // We reset the DeltaTime value as we used a blocking call on the main thread

			ENGINE.SetLevel<MultiPlayerCombatLevel>();
			if (std::shared_ptr<MultiPlayerCombatLevel> new_level = std::dynamic_pointer_cast<MultiPlayerCombatLevel>(ENGINE.GetCurrentLevel()))
			{
				new_level->SetSelection(std::pair(fighter_value, static_cast<int>(input.at(0))));
				new_level->BeginLevel();
			}
		}
	}

private:
	std::shared_ptr<UIImage>    pSolBadguyImage;
	std::shared_ptr<UIButton>   pSolBadguyButton;
	std::shared_ptr<UIText>     pSolBadguyButtonTitle;
	std::shared_ptr<UIImage>    pRoboKyImage;
	std::shared_ptr<UIButton>   pRoboKyButton;
	std::shared_ptr<UIText>     pRoboKyButtonTitle;
};

export
class UICanvas_SinglePlayerCombat :
	public AUICanvas
{
public:
	UICanvas_SinglePlayerCombat(std::shared_ptr<BaseFighter> pPlayer, std::shared_ptr<BaseFighter> pEnemy)
		:
		pPlayerHealthBar(CreateUIElement<UIProgressBar>(FRect(-240.f, 250.f, 300.f, 25.f), DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f), UIProgressBar::ProgressDirection::LeftToRight)),
		pPlayerIcon(CreateUIElement<UIImage>(FRect(-50.f, 250.f, 80.f, 100.f), pPlayer->GetIcon())),
		pPlayerName(CreateUIElement<UIText>(FRect(-250.f, 275.f, 200.f, 25.f), pPlayer->GetName(), L"Cambria", 22.f, D2D1::ColorF(D2D1::ColorF::Red))),
		pEnemyHealthBar(CreateUIElement<UIProgressBar>(FRect(240.f, 250.f, 300.f, 25.f), DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f), UIProgressBar::ProgressDirection::RightToLeft)),
		pEnemyIcon(CreateUIElement<UIImage>(FRect(50.f, 250.f, 80.f, 100.f), pEnemy->GetIcon())),
		pEnemyName(CreateUIElement<UIText>(FRect(250.f, 275.f, 200.f, 25.f), pEnemy->GetName(), L"Cambria", 22.f, D2D1::ColorF(D2D1::ColorF::Red))),
		pPlayer(pPlayer),
		pEnemy(pEnemy)
	{
		pEnemyIcon->SetInverted(true);
	}

	void    Update() override
	{
		if (const std::shared_ptr<BaseFighter> player = pPlayer.lock())
			pPlayerHealthBar->SetPercentage(player->GetHealth());
		if (const std::shared_ptr<BaseFighter> enemy = pEnemy.lock())
			pEnemyHealthBar->SetPercentage(enemy->GetHealth());
	}

private:
	std::shared_ptr<UIProgressBar>  pPlayerHealthBar;
	std::shared_ptr<UIImage>        pPlayerIcon;
	std::shared_ptr<UIText>         pPlayerName;
	std::shared_ptr<UIProgressBar>  pEnemyHealthBar;
	std::shared_ptr<UIImage>        pEnemyIcon;
	std::shared_ptr<UIText>         pEnemyName;

	std::weak_ptr<BaseFighter>      pPlayer;
	std::weak_ptr<BaseFighter>      pEnemy;
};

export
class UICanvas_SinglePlayerSelection :
	public AUICanvas
{
public:
	UICanvas_SinglePlayerSelection()
		:
		pSolBadguyImage(CreateUIElement<UIImage>(FRect(-100.f, 78.75f, 149.86f, 317.5f), "Textures/SolBadguy/selection.tga")),
		pSolBadguyButton(CreateUIElement<UIButton>(FRect(-100.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
		pSolBadguyButtonTitle(CreateUIElement<UIText>(FRect(-100.f, -200.f, 150.f, 75.f), L"Sol Badguy", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black))),
		pRoboKyImage(CreateUIElement<UIImage>(FRect(100.f, 78.75f, 256.413f, 317.5f), "Textures/RoboKy/selection.tga")),
		pRoboKyButton(CreateUIElement<UIButton>(FRect(100.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
		pRoboKyButtonTitle(CreateUIElement<UIText>(FRect(100.f, -200.f, 150.f, 75.f), L"Robo Ky", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black)))
	{
		pSolBadguyButton->SetTask(
			[&]
			{
				SelectFighter(1);
			}
			);

		pRoboKyButton->SetTask(
			[&]
			{
				SelectFighter(2);
			}
			);
	}

	void    Update() override
	{
		pSolBadguyButton->Update();
		pRoboKyButton->Update();
	}
private:
	void    SelectFighter(int fighterValue) const
	{
		if (std::shared_ptr<SinglePlayerSelectionLevel> level = std::dynamic_pointer_cast<SinglePlayerSelectionLevel>(ENGINE.GetCurrentLevel()))
		{
			// We chooses 2 characters until we pass into the next Level
			if (level->GetSelection().first == 0) level->SetSelectionValue(true, fighterValue);
			else if (level->GetSelection().second == 0) {
				level->SetSelectionValue(false, fighterValue);
				const std::pair<int, int> selection = level->GetSelection();
				ENGINE.SetLevel<SinglePlayerCombatLevel>();
				if (std::shared_ptr<SinglePlayerCombatLevel> new_level = std::dynamic_pointer_cast<SinglePlayerCombatLevel>(ENGINE.GetCurrentLevel()))
				{
					new_level->SetSelection(selection);
					new_level->BeginLevel();
				}
			}
		}
	}

private:
	std::shared_ptr<UIImage>    pSolBadguyImage;
	std::shared_ptr<UIButton>   pSolBadguyButton;
	std::shared_ptr<UIText>     pSolBadguyButtonTitle;
	std::shared_ptr<UIImage>    pRoboKyImage;
	std::shared_ptr<UIButton>   pRoboKyButton;
	std::shared_ptr<UIText>     pRoboKyButtonTitle;
};

export
class UICanvas_Title : public AUICanvas
{
public:
	UICanvas_Title()
		:
		pTitleLogo(CreateUIElement<UIImage>(FRect(0.f, 50.f, 538.f, 412.f), "Textures/logo.tga")),
		pSinglePlayerButton(CreateUIElement<UIButton>(FRect(-150.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
		pSinglePlayerButtonTitle(CreateUIElement<UIText>(FRect(-150.f, -200.f, 150.f, 75.f), L"Single Player", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black))),
		pMultiPlayerButton(CreateUIElement<UIButton>(FRect(150.f, -200.f, 150.f, 75.f), DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
		pMultiPlayerButtonTitle(CreateUIElement<UIText>(FRect(150.f, -200.f, 150.f, 75.f), L"Multi Player", L"Cambria", 25.f, D2D1::ColorF(D2D1::ColorF::Black)))

	{
		pSinglePlayerButton->SetTask([&]
			{
				ENGINE.SetLevel<SinglePlayerSelectionLevel>();
				ENGINE.GetCurrentLevel()->BeginLevel();
			});

		pMultiPlayerButton->SetTask([&]
			{
				ENGINE.SetLevel<ConnectionLevel>();
				ENGINE.GetCurrentLevel()->BeginLevel();
			});
	}

	virtual void	Update() override
	{
		pSinglePlayerButton->Update();
		pMultiPlayerButton->Update();
	}

private:
	std::shared_ptr<UIImage>	pTitleLogo;
	std::shared_ptr<UIButton>	pSinglePlayerButton;
	std::shared_ptr<UIText>		pSinglePlayerButtonTitle;
	std::shared_ptr<UIButton>	pMultiPlayerButton;
	std::shared_ptr<UIText>		pMultiPlayerButtonTitle;
};

export
class UICanvas_Victory :
	public AUICanvas
{
public:
	UICanvas_Victory(const std::string& winnerName, int playerNumber)
		:
		pVictoryText(CreateUIElement<UIText>(FRect((playerNumber == 1 ? -1 : 1) * 200.f, -250.f, 250.f, 75.f), L"YOU WON", L"Cambria", 35.f, D2D1::ColorF(D2D1::ColorF::Red))),
		pVictoryBackground(CreateUIElement<UIImage>(FRect(0.f, 0.f, 640.f, 480.f), "Textures/" + winnerName + "/victory.tga"))
	{
	}

	virtual void	Update() override
	{
		if (KBD.KeyIsPressed(VK_RETURN)) ENGINE.SetLevel<TitleLevel>();
	}

private:
	std::shared_ptr<UIText>		pVictoryText;
	std::shared_ptr<UIImage>	pVictoryBackground;
};