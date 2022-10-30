#include "MainEngine/Collider.h"

#include "CoreEngine/Timer.h"
#include "GraphicsEngine/Graphics.h"
#include "MainEngine/Engine.h"
#include "MainEngine/World.h"

Collider::Collider(Actor& owner)
	:
	owner(owner),
	origin(owner.pos),
	rect(FRect(0, 0, 0, 0)),
	pModel(owner.GetWorld().CreateModel<ColorModel>(256))
{
}

Collider::Collider(Actor& owner, const FRect& rect)
	:
	owner(owner),
	origin(owner.pos),
	rect(rect),
	pModel(owner.GetWorld().CreateModel<ColorModel>(256))
{
}

Collider& Collider::operator=(const Collider& collider)
{
	owner = collider.owner;
	rect = collider.rect;
	mode = collider.mode;
	channel = collider.channel;
	bIsGravityEnabled = collider.bIsGravityEnabled;
	bIsVisible = collider.bIsVisible;
	pModel = collider.pModel;
	return *this;
}

void Collider::Update()
{
	bIsBlocking = false;
	bIsOverlapping = false;

	if (mode == CollisionMode::Invicibility)
		UpdateInvincibility();

	// We apply the different forces, such as gravity, forces from other sources (for example a hit from an actor), friction and reaction
	if (bIsGravityEnabled) ApplyGravity();
	ApplyForces();
	ApplyFriction();
	TryMovingInThisDirection(velocity);

	owner.pos += velocity * pixelsPerMeter * DELTATIME;

	forces.clear();

	pModel->SetRectangle(FRect(invert * rect.pos.x, rect.pos.y, rect.width, rect.height) + origin);
}

void Collider::SetCollisionMode(CollisionMode collisionMode)
{
	mode = collisionMode;
}

void Collider::SetVisible(bool bValue)
{
	pModel->SetVisibility(bValue);
}

void Collider::SetCollisionChannel(CollisionChannel collisionChannel)
{
	channel = collisionChannel;

	switch (channel)
	{
	case CollisionChannel::Stun:
		pModel->SetColor(DirectX::XMFLOAT4(0.f, 1.f, 1.f, 0.25f)); //Cyan
		break;
	case CollisionChannel::Defense:
		pModel->SetColor(DirectX::XMFLOAT4(0.f, 0.f, 1.f, 0.25f)); //Blue
		break;
	case CollisionChannel::Attack:
		pModel->SetColor(DirectX::XMFLOAT4(1.f, 0.f, 0.f, 0.25f)); //Red
		break;
	case CollisionChannel::Gravity:
		pModel->SetColor(DirectX::XMFLOAT4(0.f, 1.f, 0.f, 0.25f)); //Green
		break;
	case CollisionChannel::None:
		pModel->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 0.25f)); //Gray
		break;
	default:
		break;
	}
}

void Collider::SetGravity(bool bValue)
{
	bIsGravityEnabled = bValue;
}

bool Collider::IsOverlapping() const
{
	return bIsOverlapping;
}

bool Collider::IsBlocking() const
{
	return bIsBlocking;
}

bool Collider::IsFalling() const
{
	return bIsGravityEnabled && !bIsBlocking;
}

std::unordered_set<std::shared_ptr<Actor>, Actor::Hash> Collider::GetOverlappingActors() const
{
	return overlappingActors;
}

void Collider::AddForce(const FVec2D& dir)
{
	forces.emplace_back(dir);
}

void Collider::AddImpulse(const FVec2D& dir)
{
	for (const std::shared_ptr<Collider>& col : owner.GetColliders())
		if (col->channel == CollisionChannel::Gravity) col->velocity += dir;
}

void Collider::AddInputMovement(const FVec2D& dir)
{
	FVec2D direction = dir;
	//TryMovingInThisDirection(direction);
	owner.pos += direction * DELTATIME;
}

void Collider::SetInvicibilityState()
{
	mode = CollisionMode::Invicibility;
	nInvincibilityFrames = 8;
}

void Collider::ApplyGravity()
{
	forces.emplace_back(gravity);
}

void Collider::ApplyForces()
{
	for (const FVec2D& force : forces) velocity += force * DELTATIME;
}

void Collider::ApplyFriction()
{
	const FVec2D friction = velocity * 0.02f;
	velocity -= friction;
}

void Collider::ApplyReaction(const FRect& testRect, FVec2D& direction)
{
	FVec2D final_normal;
	for (const std::shared_ptr<Actor>& overlapped_act : overlappingActors)
		for (const std::shared_ptr<Collider>& overlapped_act_collider : overlapped_act->GetColliders())
			final_normal += (overlapped_act_collider->rect + overlapped_act_collider->origin).GetNormalFromPoint(testRect.pos);

	final_normal.Normalize();
	direction += final_normal * direction.GetLength();

	if (direction.GetLength() * DELTATIME < 5.f) direction = FVec2D(0, 0);
}

void Collider::UpdateInvincibility()
{
	if (--nInvincibilityFrames <= 0) mode = CollisionMode::Overlapping;
}

void Collider::TryMovingInThisDirection(FVec2D& direction)
{
	overlappingActors.clear();
	const FRect test_rect = rect + origin + direction * DELTATIME;

	// For all other actors in the level...
	for (std::shared_ptr<Actor>& actor : owner.GetWorld().GetActors())
	{
		if (&owner != actor.get())
		{
			// ...we check for each one of their colliders...
			for (const std::shared_ptr<Collider>& c : actor->GetColliders())
			{
				// ...if there is a collision between our collider and their collider
				const FRect actor_collider = c->rect + c->origin;
				if (test_rect.IsCollidingRect(actor_collider))
				{
					overlappingActors.insert(std::make_shared<Actor>(c->owner));
					if (mode == c->mode) {
						switch (mode)
						{
						case CollisionMode::Overlapping:
							if (channel == CollisionChannel::Attack && c->channel == CollisionChannel::Defense)
							{
								constexpr FVec2D impulse = FVec2D(15, 200);
								c->AddImpulse(impulse);
								c->SetInvicibilityState();
							}
							bIsOverlapping = true;
							break;
						case CollisionMode::Blocking:
							bIsBlocking = true;
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
	if (bIsBlocking) 
		ApplyReaction(test_rect, direction);
}

void Collider::SetRectPos(const FRect& pos)
{
	rect = pos;
}

void Collider::SetDirection(int newInvert)
{
	invert = newInvert;
}
