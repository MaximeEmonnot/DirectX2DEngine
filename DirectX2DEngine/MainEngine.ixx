module;
#include <DirectXMath.h>

export module MainEngine;

import <string>;
import <memory>;
import <vector>;
import <map>;
import <unordered_set>;
import <unordered_map>;
import <filesystem>;
import <regex>;
import <functional>;

import "rapidjson/document.h";

import DebugEngine;
import MathEngine;
import CoreEngine;
import GraphicsEngine;
import UIEngine;
import SoundEngine;
import ParserEngine;

class World;
class Collider;
class Controller;

export
class Actor
{
public:
	class Hash
	{
	public:
		size_t operator()(const Actor& actor) const {
			constexpr std::hash<std::string> hasher;
			return hasher(actor.name);
		}
		size_t operator()(const std::shared_ptr<Actor>& pActor) const
		{
			constexpr std::hash<std::string> hasher;
			return hasher(pActor->name);
		}
	};

public:
	Actor() = delete;
	Actor(World& world, const FVec2D& pos, const std::string& name)
		:
		world(world),
		pos(pos),
		name(name)
	{}
	Actor(const Actor&) = default;
	virtual ~Actor() = default;

	Actor& operator=(const Actor& toCopy)
	{
		pos = toCopy.pos;
		name = toCopy.name;
		return *this;
	}

	virtual void Update()
	{}

	virtual void ApplyDamage(const Actor& damageCauser, int damage)
	{}

	FVec2D													GetPosition() const
	{
		return pos;
	}
	void													SetPosition(const FVec2D& newPosition)
	{
		pos = newPosition;
	}
	std::string												GetName() const
	{
		return name;
	}
	virtual std::vector<std::shared_ptr<Collider>>          GetColliders() const
	{
		return std::vector<std::shared_ptr<Collider>>();
	}

	World& GetWorld() const
	{
		return world;
	}

protected:
	World& world;
public:
	FVec2D			pos;
	std::string		name;
};


export
class World
{
public:
	World() = default;

	void	Update()
	{
		for (std::shared_ptr<Actor>& pActor : actors)
			pActor->Update();
	}
	void	Render() const
	{
		for (const std::pair<const int, std::shared_ptr<BaseModel>>& entry : models)
			if (entry.second->IsVisible()) entry.second->Render();
	}

	void	Clear()
	{
		actors.clear();
		models.clear();
	}

	template<class T>
	std::shared_ptr<T> SpawnActor()
	{
		std::shared_ptr<T> new_actor = std::make_shared<T>(*this);
		actors.emplace_back(new_actor);
		return new_actor;
	}

	template<class T, class... Args >
	std::shared_ptr<T> SpawnActor(Args&&... args)
	{
		std::shared_ptr<T> new_actor = std::make_shared<T>(*this, args...);
		actors.emplace_back(new_actor);
		return new_actor;
	}

	template<class T>
	std::shared_ptr<T> CreateModel(int priority)
	{
		std::shared_ptr<BaseModel> new_model = std::make_shared<T>();
		new_model->Initialize();
		new_model->SetDepth(static_cast<float>(priority));
		models.emplace(std::pair(priority, new_model));
		return std::dynamic_pointer_cast<T>(new_model);
	}

	template<class T, class... Args>
	std::shared_ptr<T> CreateModel(int priority, Args&&... args)
	{
		std::shared_ptr<BaseModel> new_model = std::make_shared<T>(args...);
		new_model->Initialize();
		new_model->SetDepth(static_cast<float>(priority));
		models.emplace(std::pair(priority, new_model));
		return std::dynamic_pointer_cast<T>(new_model);
	}

	std::vector<std::shared_ptr<Actor>>		GetActors() const
	{
		return actors;
	}

private:
	std::vector<std::shared_ptr<Actor>>		actors;

	std::multimap<int, std::shared_ptr<BaseModel>>	models;
};




class Pawn;

export
class Controller
{
public:
	Controller(Pawn& owner)
		:
		owner(owner)
	{}

	virtual void	Update() = 0;

protected:
	Pawn& owner;
};

export
class Pawn :
	public Actor
{
public:
	Pawn() = delete;

	Pawn(World& world, const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> pController)
		:
		Actor(world, pos, name),
		pController(pController)
	{}
	Pawn(const Pawn&) = default;
	Pawn& operator= (const Pawn& toCopy)
	{
		pos = toCopy.pos;
		name = toCopy.name;
		pController = toCopy.pController;
		return *this;
	}
	virtual ~Pawn() override = default;

	virtual void    Update() override
	{
		Actor::Update();

		pController->Update();
	}

	std::shared_ptr<Controller> GetController() const
	{
		return pController;
	}

protected:
	std::shared_ptr<Controller>     pController;
};



export
class Collider
{
private:
	friend class MovingCollider;
public:
	enum class CollisionMode
	{
		None,
		Overlapping,
		Invicibility,
		Blocking
	};

	enum class CollisionChannel
	{
		Stun,
		Defense,
		Attack,
		Gravity,
		None
	};
public:
	Collider(Actor& owner)
		:
		owner(owner),
		origin(owner.pos),
		rect(FRect(0, 0, 0, 0)),
		pModel(owner.GetWorld().CreateModel<ColorModel>(256))
	{}
	Collider(Actor& owner, const FRect& rect)
		:
		owner(owner),
		origin(owner.pos),
		rect(rect),
		pModel(owner.GetWorld().CreateModel<ColorModel>(256))
	{}

	Collider& operator=(const Collider& collider)
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

	void														Update()
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

	void														SetCollisionMode(CollisionMode collisionMode)
	{
		mode = collisionMode;
	}
	void														SetVisible(bool bValue)
	{
		pModel->SetVisibility(bValue);
	}
	void														SetCollisionChannel(CollisionChannel collisionChannel)
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
	void														SetGravity(bool bValue)
	{
		bIsGravityEnabled = bValue;
	}
	void														SetRectPos(const FRect& pos)
	{
		rect = pos;
	}
	void														SetDirection(int newDirection)
	{
		invert = newDirection;
	}

	bool														IsOverlapping() const
	{
		return bIsOverlapping;
	}
	bool														IsBlocking() const
	{
		return bIsBlocking;
	}

	bool														IsFalling() const
	{
		return bIsGravityEnabled && !bIsBlocking;
	}

	std::unordered_set<std::shared_ptr<Actor>, Actor::Hash>		GetOverlappingActors() const
	{
		return overlappingActors;
	}

	void														AddForce(const FVec2D& dir)
	{
		forces.emplace_back(dir);
	}
	void														AddImpulse(const FVec2D& dir)
	{
		for (const std::shared_ptr<Collider>& col : owner.GetColliders())
			if (col->channel == CollisionChannel::Gravity) col->velocity += dir;
	}
	void														AddInputMovement(const FVec2D& dir)
	{
		FVec2D direction = dir;
		//TryMovingInThisDirection(direction);
		owner.pos += direction * DELTATIME;
	}


private:
	void														ApplyGravity()
	{
		forces.emplace_back(gravity);
	}
	void														ApplyForces()
	{
		for (const FVec2D& force : forces) velocity += force * DELTATIME;
	}
	void														ApplyFriction()
	{
		const FVec2D friction = velocity * 0.02f;
		velocity -= friction;
	}
	void														ApplyReaction(const FRect& testRect, FVec2D& direction)
	{
		FVec2D final_normal;
		for (const std::shared_ptr<Actor>& overlapped_act : overlappingActors)
			for (const std::shared_ptr<Collider>& overlapped_act_collider : overlapped_act->GetColliders())
				final_normal += (overlapped_act_collider->rect + overlapped_act_collider->origin).GetNormalFromPoint(testRect.pos);

		final_normal.Normalize();
		direction += final_normal * direction.GetLength();

		if (direction.GetLength() * DELTATIME < 5.f) direction = FVec2D(0, 0);
	}

	void														UpdateInvincibility()
	{
		if (--nInvincibilityFrames <= 0) mode = CollisionMode::Overlapping;
	}
	void														TryMovingInThisDirection(FVec2D& direction)
	{
		overlappingActors.clear();
		const FRect test_rect = FRect(invert * rect.pos.x, rect.pos.y, rect.width, rect.height) + origin + direction * DELTATIME;

		// For all other actors in the level...
		for (std::shared_ptr<Actor>& actor : owner.GetWorld().GetActors())
		{
			if (&owner != actor.get())
			{
				// ...we check for each one of their colliders...
				for (const std::shared_ptr<Collider>& c : actor->GetColliders())
				{
					// ...if there is a collision between our collider and their collider
					const FRect actor_collider = FRect(c->invert * c->rect.pos.x, c->rect.pos.y, c->rect.width, c->rect.height) + c->origin;
					if (test_rect.IsCollidingRect(actor_collider))
					{
						overlappingActors.insert(std::make_shared<Actor>(c->owner));
						if (mode == c->mode) {
							switch (mode)
							{
							case CollisionMode::Overlapping:
							{
								const bool attackerCondition = channel == CollisionChannel::Attack;
								const bool defenderCondition = (c->channel == CollisionChannel::Defense || c->channel == CollisionChannel::Stun);
								if (attackerCondition && defenderCondition)
								{
									//constexpr FVec2D impulse = FVec2D(15, 200);
									//c->AddImpulse(impulse);
									c->SetInvicibilityState();
									actor->ApplyDamage(owner, 10);
								}
								bIsOverlapping = true;
							}
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
	void														SetInvicibilityState()
	{
		mode = CollisionMode::Invicibility;
		nInvincibilityFrames = static_cast<int>(0.5f / DELTATIME);
	}

private:
	bool														bIsBlocking = false;
	bool														bIsOverlapping = false;
	bool														bIsVisible = false;
	bool														bIsGravityEnabled = false;

	CollisionMode												mode = CollisionMode::None;
	CollisionChannel											channel = CollisionChannel::None;

	std::shared_ptr<ColorModel>									pModel;

	Actor& owner;
	FVec2D& origin;
	FRect														rect;
	int															invert = 1;

	FVec2D														velocity;

	std::unordered_set<std::shared_ptr<Actor>, Actor::Hash>		overlappingActors;

	std::vector<FVec2D>											forces;

	static constexpr FVec2D										topNormal = FVec2D(0, 1);
	static constexpr FVec2D										rightNormal = FVec2D(1, 0);
	static constexpr FVec2D										gravity = FVec2D(0, -30.f);
	static constexpr float										pixelsPerMeter = 140.6593407f;

	int															nInvincibilityFrames = 0;
};


export
class MovingCollider
{
public:
	MovingCollider(Actor& owner)
		:
		pCollider(std::make_shared<Collider>(owner, defaultPos))
	{
		pCollider->SetCollisionMode(Collider::CollisionMode::None);
	}

	void						Update()
	{
		pCollider->SetRectPos(positions[curRectPos]);
		pCollider->Update();
		curFrameTime += DELTATIME;
		while (curFrameTime > holdTime) Advance();
	}

	void						SetLoop(Animation::AnimationMode _loopMode)
	{
		loopMode = _loopMode;
	}
	void						SetHoldTime(float _holdTime)
	{
		holdTime = _holdTime;
	}
	void						SetCollisionMode(Collider::CollisionMode collisionMode)
	{
		pCollider->SetCollisionMode(collisionMode);
	}
	void						SetCollisionChannel(Collider::CollisionChannel collisionChannel)
	{
		pCollider->SetCollisionChannel(collisionChannel);
	}
	void						SetVisible(bool bValue) const
	{
		pCollider->SetVisible(bValue);
	}
	void						SetDirection(int newDirection) const
	{
		pCollider->SetDirection(newDirection);
	}
	void						AddPosition(const FRect& pos)
	{
		positions.emplace_back(pos);
	}

	void						Reset()
	{
		bIsFinished = false;
		curRectPos = 0;
	}

	bool						IsFinished() const
	{
		return bIsFinished;
	}

	std::shared_ptr<Collider>	GetCollider() const
	{
		return pCollider;
	}

private:
	void						Advance()
	{
		// We go into the next frame, depending on the AnimationLoopingMode
		curRectPos += threshold;
		if (curRectPos >= positions.size() || curRectPos < 0) {
			switch (loopMode)
			{
			case Animation::AnimationMode::NoLoop:
				curRectPos = static_cast<int>(positions.size() - 1);
				bIsFinished = true;
				break;
			case Animation::AnimationMode::LoopingAll:
				curRectPos = 0;
				break;
			case Animation::AnimationMode::LoopingBack:
				bIsFinished = (curRectPos < 0);
				threshold *= -1;
				curRectPos += 2 * threshold;
				break;
			default:
				break;
			}
		}
		else bIsFinished = false;
		curFrameTime -= holdTime;
	}

private:
	FRect						defaultPos = FRect(0.0, 0.0, 0.0, 0.0);

	std::shared_ptr<Collider>	pCollider;
	std::vector<FRect>			positions;
	float						curFrameTime = 0.0f;
	float						holdTime;
	int							curRectPos = 0;
	int							threshold = 1;

	Animation::AnimationMode	loopMode = Animation::AnimationMode::NoLoop;
	bool						bIsFinished = false;
};


export
class CollisionGroup
{
public:
	CollisionGroup() = default;

	void									Update()
	{
		for (auto& collider : colliders) collider.Update();
	}

	void									Reset()
	{
		for (auto& collider : colliders) collider.Reset();
	}

	void									AddCollider(const MovingCollider& collider)
	{
		colliders.emplace_back(collider);
	}

	bool									IsFinished() const
	{
		bool out = false;
		for (auto& col : colliders) out |= col.IsFinished();
		return out;
	}

	std::vector<std::shared_ptr<Collider>>	GetColliders() const
	{
		std::vector<std::shared_ptr<Collider>> out;
		for (auto& movingCollider : colliders) out.emplace_back(movingCollider.GetCollider());
		return out;
	}

	void									SetDirection(int direction) const
	{
		for (auto& movingCollider : colliders) movingCollider.SetDirection(direction);
	}
	void									SetVisible(bool bValue) const
	{
		for (auto& movingCollider : colliders) movingCollider.SetVisible(bValue);
	}

private:
	std::vector<MovingCollider>				colliders;
};


export
class CollisionSystem
{
public:
	CollisionSystem(Actor& owner, const std::string& filePath, const std::vector<std::string>& animations)
	{
		// From the JSON File, for each animation...
		JSONParser::Reader jsonParser = JSON(filePath);
		auto& jsonCollisions = jsonParser.GetValueOf("collisions");
		for (auto& anim : animations)
		{
			if (jsonCollisions.HasMember(anim.c_str()))
			{
				//... we get the HoldTime value, the LoopingMode...
				CollisionGroup collision_group;
				auto& jsonCollider = jsonCollisions[anim.c_str()];

				const float hold_time = jsonParser.GetValueOf("animations")[anim.c_str()].GetArray()[1].GetFloat();
				const Animation::AnimationMode loop_mode = static_cast<Animation::AnimationMode>(jsonParser.GetValueOf("animations")[anim.c_str()].GetArray()[2].GetInt());
				const std::string animation_path = jsonParser.GetValueOf("character").GetString() + anim + std::string("\\");

				//... and we setup the different colliders to the collision group
				AddCollisionsToGroup(collision_group, owner, jsonCollider, "Head", Collider::CollisionChannel::Stun, hold_time, loop_mode, animation_path);
				AddCollisionsToGroup(collision_group, owner, jsonCollider, "Body", Collider::CollisionChannel::Defense, hold_time, loop_mode, animation_path);
				AddCollisionsToGroup(collision_group, owner, jsonCollider, "Attack", Collider::CollisionChannel::Attack, hold_time, loop_mode, animation_path);

				collisionGroups.insert(std::pair(anim, collision_group));
			}
		}

		animState = "Idle";
	}

	void									Update()
	{
		collisionGroups.at(animState).Update();
		for (auto& entry : transitions) if (entry.first == animState && entry.second.second()) SetCollisionGroup(entry.second.first);
	}

	void									AddTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition)
	{
		transitions.insert(std::pair(from, std::pair(to, condition)));
	}

	bool									AnimationIsFinished() const
	{
		return collisionGroups.at(animState).IsFinished();
	}

	void									SetDirection(int newDirection) const
	{
		for (const auto& pCollider : collisionGroups.at(animState).GetColliders()) pCollider->SetDirection(newDirection);
	}

	std::vector<std::shared_ptr<Collider>>	GetColliders() const
	{
		std::vector<std::shared_ptr<Collider>> out;
		for (auto& pCollider : collisionGroups.at(animState).GetColliders())
			out.emplace_back(pCollider);
		return out;
	}

private:
	void									SetCollisionGroup(const std::string& state)
	{
		if (animState != state) {
			collisionGroups.at(animState).SetVisible(false);
			animState = state;
			collisionGroups.at(animState).Reset();
			collisionGroups.at(animState).SetVisible(true);
		}
	}
	void									AddCollisionsToGroup(CollisionGroup& collisionGroup, Actor& owner, rapidjson::GenericValue<rapidjson::UTF8<>>& jsonObject, const std::string& name, Collider::CollisionChannel collisionChannel, float holdTime, Animation::AnimationMode loopMode, const std::string& animation) const
	{
		// We initialize the collider and we add it to the collision group
		if (jsonObject.HasMember(name.c_str()))
		{
			auto& collider = jsonObject[name.c_str()];
			MovingCollider moving_collider(owner);
			moving_collider.SetVisible(true);
			moving_collider.SetCollisionMode(Collider::CollisionMode::Overlapping);
			moving_collider.SetCollisionChannel(collisionChannel);
			moving_collider.SetHoldTime(holdTime);
			moving_collider.SetLoop(loopMode);
			for (auto itr = collider.MemberBegin(); itr != collider.MemberEnd(); ++itr)
			{
				const float x_pos = itr->value.GetArray()[0].GetFloat();
				const float y_pos = itr->value.GetArray()[1].GetFloat();
				const float width = itr->value.GetArray()[2].GetFloat();
				const float height = itr->value.GetArray()[3].GetFloat();
				const FVec2D texture_center = ANIMATION_TEXTURE(animation + std::string(itr->name.GetString()) + std::string(".tga")).GetCenter();
				moving_collider.AddPosition(FRect(x_pos - texture_center.x, y_pos - texture_center.y, width, height));
			}
			collisionGroup.AddCollider(moving_collider);
		}
	}

private:
	std::string																				animState;
	std::unordered_map<std::string, CollisionGroup>											collisionGroups;
	std::unordered_multimap<std::string, std::pair<std::string, std::function<bool()>>>		transitions;
};



export
class Character :
	public Pawn
{
public:
	Character() = delete;
	Character(World& world, const FVec2D& pos, const std::string& name, std::shared_ptr<Controller> controller)
		:
		Pawn(world, pos, name, controller),
		pRootCollider(std::make_shared<Collider>(*this))
	{}

	virtual void                Update() override
	{
		Pawn::Update();
		pRootCollider->Update();
	}

	std::shared_ptr<Collider>   GetRootCollider() const
	{
		return pRootCollider;
	}

	bool                        IsGrounded() const
	{
		return !pRootCollider->IsFalling();
	}

protected:
	std::shared_ptr<Collider>   pRootCollider;
};


export
class Level
{
public:
	Level()
		:
		pWorld(std::make_shared<World>())
	{}
	Level(const Level& toCopy)
	{
		*this = toCopy;
	}
	Level(Level&& toMove) noexcept
		:
		pWorld(std::move(toMove.pWorld))
	{}

	Level& operator= (const Level& toCopy)
	{
		pWorld = toCopy.pWorld;
		return *this;
	}
	Level& operator= (Level&& toMove) noexcept
	{
		pWorld = std::move(toMove.pWorld);
		return *this;
	}

	virtual ~Level() = default;

	virtual void	Update()
	{
		pWorld->Update();
		for (const std::shared_ptr<AUICanvas>& pCanvas : canvasList) pCanvas->Update();
	}

	virtual void	BeginLevel() = 0;

	void			Render() const
	{
		pWorld->Render();

		for (const std::shared_ptr<AUICanvas>& pCanvas : canvasList) pCanvas->Render();
	}

	void			Clear()
	{
		pWorld->Clear();
		canvasList.clear();
	}

	World& GetWorld() const
	{
		return *pWorld;
	}

protected:
	template<class T>
	void CreateCanvas()
	{
		canvasList.emplace_back(std::make_shared<T>());
	}

	template<class T, class... Args>
	void CreateCanvas(Args&&... args)
	{
		canvasList.emplace_back(std::make_shared<T>(args...));
	}

protected:
	std::shared_ptr<World>						pWorld;
	std::vector<std::shared_ptr<AUICanvas>>		canvasList;
};


export
class Engine
{
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine() = default;

	static Engine& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<Engine>();
		return *pInstance;
	}

	void			Update() const
	{
		TICKCLOCK();

		if (pCurrentLevel) pCurrentLevel->Update();
	}
	void			Render() const
	{
		if (pCurrentLevel) pCurrentLevel->Render();
	}

	void			Clear()
	{
		for (std::shared_ptr<Level>& pLevel : levels) pLevel->Clear();
		levels.clear();
	}

	template<class T>
	void AddLevel()
	{
		levels.emplace_back(std::make_shared<T>());
		if (!pCurrentLevel) pCurrentLevel = levels.front();
	}

	template<class T>
	void SetLevel()
	{
		for (std::shared_ptr<Level> level : levels)
		{
			if (std::dynamic_pointer_cast<T>(level))
			{
				pCurrentLevel = level;
				return;
			}
		}
		throw ENGINE_EXCEPTION("DirectX 2D Engine - Standard exception", "This level doesn't exist! Please check your code.");
	}

	std::shared_ptr<Level>	GetCurrentLevel() const
	{
		return pCurrentLevel;
	}

private:
	static std::unique_ptr<Engine>			pInstance;

	std::vector<std::shared_ptr<Level>>		levels;
	std::shared_ptr<Level>					pCurrentLevel;
};
// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Engine> Engine::pInstance = nullptr;

export Engine& ENGINE = Engine::GetInstance();

export
class Game
{
public:
	Game()
	{
		// Graphics System Initialization
		GFX;

		// Textures Initialization
		for (const auto& entry : std::filesystem::recursive_directory_iterator("Textures")) {
			std::regex regex = std::regex("(.+/)*[1-9]?[0-9]\.tga");
			if (std::regex_search(entry.path().string(), regex)) ANIMATION_TEXTURE(entry.path().string());
			else if (entry.path().string().ends_with(".tga")) TEXTURE(entry.path().string());
		}

		// JSON Initialization
		for (const auto& entry : std::filesystem::recursive_directory_iterator("json"))
			if (entry.path().string().ends_with(".json")) JSON(entry.path().string());

		// Audio Initialization
		for (const auto& entry : std::filesystem::recursive_directory_iterator("Sounds"))
			if (entry.path().string().ends_with(".wav")) SFX.AddSong(entry.path().string(), true);

		// Levels Loading
		ENGINE.AddLevel<TitleLevel>();
		ENGINE.AddLevel<SinglePlayerSelectionLevel>();
		ENGINE.AddLevel<SinglePlayerCombatLevel>();
		ENGINE.AddLevel<ConnectionLevel>();
		ENGINE.AddLevel<MultiPlayerSelectionLevel>();
		ENGINE.AddLevel<MultiPlayerCombatLevel>();
		ENGINE.AddLevel<VictoryLevel>();
	}
	~Game() = default;

	void	Go()
	{
		GFX.BeginDraw();
		UpdateFrame();
		RenderFrame();
		GFX.EndDraw();
	}

private:
	void	UpdateFrame() const
	{
		ENGINE.Update();
	}
	void	RenderFrame() const
	{
		ENGINE.Render();

		TEXT_ENGINE.Render(std::to_wstring(static_cast<int>(1 / DELTATIME)), L"Arial Black", 32.f, FRect(350.f, 300.f, 256.f, 64.f));
	}
};
