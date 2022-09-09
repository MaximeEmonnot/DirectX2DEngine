#include "CombatLevel.h"

#include "Background.h"
#include "Engine.h"
#include "LoggerManager.h"
#include "PlayerCharacter.h"
#include "Rect.h"
#include "SandBag.h"
#include "SoundSystem.h"
#include "Timer.h"
#include "UICanvas_Combat.h"

CombatLevel::CombatLevel()
{
	// Spawn Actors
	GetWorld().SpawnActor<Background>(FVec2D(0, 180), "Background");
	std::shared_ptr<PlayerCharacter> player = GetWorld().SpawnActor<PlayerCharacter>(FVec2D(250, -125), "Player");

	// Create UI Canvas
	CreateCanvas<UICanvas_Combat>(player);

	// Play background sounds
	SFX.Play("Sounds/BeJustOrBeDead.wav");
}

void CombatLevel::Update()
{
	Level::Update();
}
