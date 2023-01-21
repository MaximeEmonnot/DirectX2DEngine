#include "GameEngine/Controllers/NetworkEnemyController.h"

#include "CoreEngine/ThreadPool.h"
#include "CoreEngine/Window.h"
#include "NetworkEngine/NetworkSystem.h"
#include "MainEngine/Pawn.h"

NetworkEnemyController::NetworkEnemyController(Pawn& owner)
	:
	FightingController(owner),
	lastPosition(owner.GetPosition())
{
	THREAD.Enqueue([&]
		{
			while(WND.IsPlaying() && NETWORK.IsOnline())
			{
				const std::vector<uint8_t> input = NETWORK.ReceiveDataUDP();
				// Reading X-Y position from vector, we will reinterpret them as floating point values
				if (!input.empty() && input.at(0) == 11 && input.at(1) == NETWORK.GetPlace()) {
					uint8_t position_x[4] = { input.at(2), input.at(3), input.at(4), input.at(5) };
					uint8_t position_y[4] = { input.at(6), input.at(7), input.at(8), input.at(9) };

					lastPosition = FVec2D(*reinterpret_cast<float*>(&position_x), *reinterpret_cast<float*>(&position_y));
					action = static_cast<EAction>(input.at(10));
				}
			}
		});
}

void NetworkEnemyController::Update()
{
	owner.SetPosition(lastPosition);
}
