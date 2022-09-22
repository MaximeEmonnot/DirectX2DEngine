#include "NetworkEnemyController.h"

#include "NetworkSystem.h"
#include "Pawn.h"
#include "ThreadPool.h"
#include "Window.h"

NetworkEnemyController::NetworkEnemyController(Pawn& owner)
	:
	Controller(owner),
	lastPosition(owner.GetPosition())
{
	THREAD.Enqueue([&]
		{
			while(WND.IsPlaying())
			{
				const std::vector<uint8_t> input = NETWORK.ReceiveData();
				// Reading X-Y position
				if (!input.empty()) {
					uint8_t position_x[4] = { input.at(0), input.at(1), input.at(2), input.at(3) };
					uint8_t position_y[4] = { input.at(4), input.at(5), input.at(6), input.at(7) };

					lastPosition = FVec2D(*reinterpret_cast<float*>(&position_x), *reinterpret_cast<float*>(&position_y));
				}
			}
		});
}

void NetworkEnemyController::Update()
{
	owner.SetPosition(lastPosition);
}
