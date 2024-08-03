#include "ScenePlay.hpp"
#include "Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.hpp"
#include "Components.hpp"
#include "Action.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <fstream>

ScenePlay::ScenePlay(GameEngine* gameEngine,
	const std::string& levelPath)
	: Scene(gameEngine)
	, m_levelPath(levelPath) {
	this->init(m_levelPath);
}

void ScenePlay::init(const std::string& levelPath) {
	this->loadLevel(levelPath);

	this->m_gridText.setCharacterSize(12);
	this->m_gridText.setFont(this->m_game->assets().getFont("Tech"));

	this->registerAction(sf::Keyboard::P,		static_cast<uint32_t>(ScenePlayAction::PAUSE));
	this->registerAction(sf::Keyboard::Escape,	static_cast<uint32_t>(ScenePlayAction::QUIT));
	this->registerAction(sf::Keyboard::T,		static_cast<uint32_t>(ScenePlayAction::TOGGLE_TEXTURE));
	this->registerAction(sf::Keyboard::C,		static_cast<uint32_t>(ScenePlayAction::TOGGLE_COLLISION));
	this->registerAction(sf::Keyboard::G,		static_cast<uint32_t>(ScenePlayAction::TOGGLE_GRID));
	this->registerAction(sf::Keyboard::F,		static_cast<uint32_t>(ScenePlayAction::TOGGLE_FOLLOW));

	this->registerAction(sf::Keyboard::W,		static_cast<uint32_t>(ScenePlayAction::UP));
	this->registerAction(sf::Keyboard::A,		static_cast<uint32_t>(ScenePlayAction::LEFT));
	this->registerAction(sf::Keyboard::S,		static_cast<uint32_t>(ScenePlayAction::DOWN));
	this->registerAction(sf::Keyboard::D,		static_cast<uint32_t>(ScenePlayAction::RIGHT));
	this->registerAction(sf::Keyboard::Space,	static_cast<uint32_t>(ScenePlayAction::ATTACK));

	this->m_game->playSound(SoundTag::MUSIC_PLAY);
}

void ScenePlay::loadLevel(const std::string& filename) {
	this->m_entityManager = EntityManager();

	std::ifstream fin(filename);
	std::string str_type{ "" };
	while (fin.good()) {
		fin >> str_type;
		EntityTag type = strEntityTagMap.at(str_type);

		switch (type)
		{
		case EntityTag::PLAYER: {
			fin >> this->m_playerConfig.X >> this->m_playerConfig.Y >>
				this->m_playerConfig.CW >> this->m_playerConfig.CH >>
				this->m_playerConfig.SPEED >> this->m_playerConfig.HEALTH;

			this->spawnPlayer();
			break;
		}
		case EntityTag::TILE: case EntityTag::NPC: case EntityTag::PORTAL: case EntityTag::HEART: {
			auto entity = m_entityManager.addEntity(type);

			switch (type)
			{
			case EntityTag::TILE: case EntityTag::PORTAL: case EntityTag::HEART: {
				std::string name{ "" };
				int RX{ 0 }, RY{ 0 }, TX{ 0 }, TY{ 0 }, BM{ 0 }, BV{ 0 };
				fin >> name >> RX >> RY >> TX >> TY >> BM >> BV;

				entity->add<CAnimation>(this->m_game->assets().getAnimation(strAnimationTagMap.at(name)), true);
				entity->add<CBoundingBox>(this->m_game->assets().getAnimation(strAnimationTagMap.at(name)).getSize(),
					static_cast<bool>(BM), static_cast<bool>(BV));
				entity->add<CTransform>(this->getPosition(RX, RY, TX, TY));
				break;
			}
			case EntityTag::NPC: {
				std::string name{ "" };
				int RX{ 0 }, RY{ 0 }, TX{ 0 }, TY{ 0 }, BM{ 0 }, BV{ 0 }, H{ 0 }, D{ 0 };
				fin >> name >> RX >> RY >> TX >> TY >> BM >> BV >> H >> D;

				entity->add<CAnimation>(this->m_game->assets().getAnimation(strAnimationTagMap.at(name)), true);
				entity->add<CBoundingBox>(this->m_game->assets().getAnimation(strAnimationTagMap.at(name)).getSize(),
					static_cast<bool>(BM), static_cast<bool>(BV));
				entity->add<CTransform>(this->getPosition(RX, RY, TX, TY));
				entity->add<CHealth>(H);
				entity->add<CDamage>(D);

				std::string AIType{ "" };
				float speed{ 0.0f };
				fin >> AIType >> speed;

				switch (strAITypeMap.at(AIType))
				{
				case AIType::FOLLOW: {
					entity->add<CFollowPlayer>(entity->get<CTransform>().m_pos, speed);
					break;
				}
				case AIType::PATROL: {
					int positionAmout{ 0 };
					fin >> positionAmout;

					float  Xi{ 0.0f }, Yi{ 0.0f };
					std::vector<Vec2f> positions;
					std::vector<Vec2f> velocities;

					if (positionAmout) {
						fin >> Xi >> Yi;
						positions.push_back(getPosition(RX, RY, Xi, Yi));
					}

					for (int i = 1; i < positionAmout; ++i) {
						fin >> Xi >> Yi;
						positions.push_back(getPosition(RX, RY, Xi, Yi));

						Vec2f diff{ positions[i].x - positions[i - 1].x, positions[i].y - positions[i - 1].y };
						diff.normalize();
						velocities.push_back(Vec2f{ speed * diff.x, speed * diff.y });
					}

					Vec2f diff{ positions[0].x - positions[positionAmout - 1].x, positions[0].y - positions[positionAmout - 1].y };
					diff.normalize();
					velocities.push_back(Vec2f{ speed * diff.x, speed * diff.y });

					entity->add<CPatrol>(positions, velocities, speed);

					break;
				}
				default:
					std::cerr << "Unknown AI behaviour type\n";
					exit(-1);
					break;
				}

				break;
			}
			}

			break;
		}
		default:
			std::cerr << "Unknown entity type\n";
			exit(-1);
			break;
		}
	}
}

void ScenePlay::spawnPlayer() {
	auto entity = this->m_entityManager.addEntity(EntityTag::PLAYER);
	entity->add<CAnimation>(this->m_game->assets().getAnimation(
		strAnimationTagMap.at(playerStateStrMap.at(PlayerState::STAND_DOWN))), true);
	entity->add<CTransform>(Vec2f(this->m_playerConfig.X, this->m_playerConfig.Y));
	entity->add<CBoundingBox>(Vec2f(this->m_playerConfig.CW, this->m_playerConfig.CH));
	entity->add<CHealth>(this->m_playerConfig.HEALTH);
}

void ScenePlay::spawnSword(std::shared_ptr<Entity> e) {
	if (e->tag() == EntityTag::PLAYER) {
		auto entity = this->m_entityManager.addEntity(EntityTag::SWORD);
		auto eAnimationSize = e->get<CAnimation>().m_animation.getSize();
		auto eTransform = e->get<CTransform>();
		auto& eState = e->get<CState>();
		Vec2f scale;
		Vec2f pos;

		switch (e->get<CState>().m_state)
		{
		case PlayerState::STAND_DOWN: {
			entity->add<CAnimation>(this->m_game->assets().getAnimation(AnimationTag::SWORD_UP), true);
			scale = { 1.0f, -1.0f };
			pos = { eTransform.m_pos.x, eTransform.m_pos.y - scale.y * eAnimationSize.y };
			
			if (eState.m_state != PlayerState::ATTACK_DOWN) {
				eState.m_prevState = eState.m_state;
				eState.m_state = PlayerState::ATTACK_DOWN;
			}

			break;
		}
		case PlayerState::STAND_RIGHT: {
			entity->add<CAnimation>(this->m_game->assets().getAnimation(AnimationTag::SWORD_RIGHT), true);
			scale = eTransform.m_scale;
			pos = { eTransform.m_pos.x + scale.x * eAnimationSize.x, eTransform.m_pos.y };
			
			if (eState.m_state != PlayerState::ATTACK_RIGHT) {
				eState.m_prevState = eState.m_state;
				eState.m_state = PlayerState::ATTACK_RIGHT;
			}
			
			break;
		}
		case PlayerState::STAND_UP: {
			entity->add<CAnimation>(this->m_game->assets().getAnimation(AnimationTag::SWORD_UP), true);
			scale = { 1.0f, 1.0f };
			pos = { eTransform.m_pos.x, eTransform.m_pos.y - scale.y * eAnimationSize.y };
			
			if (eState.m_state != PlayerState::ATTACK_UP) {
				eState.m_prevState = eState.m_state;
				eState.m_state = PlayerState::ATTACK_UP;
			}

			break;
		}
		default:
			break;
		}

		entity->add<CTransform>(pos, Vec2f(0.0f, 0.0f),
			scale, 0.0f);
		entity->add<CBoundingBox>(entity->get<CAnimation>().m_animation.getSize());
		entity->add<CLifeSpan>(10);
		entity->add<CDamage>(1);

		this->m_game->playSound(SoundTag::SWORD_SLASH);
	} else {
		std::cerr << "Unable to create a weapon for non-player entity\n";
	}
}

void ScenePlay::update() {
	srand(time(0));
	this->m_entityManager.update();

	if (!this->m_paused) {
		this->sAI();
		this->sMovement();
		this->sStatus();
		this->sCollision();
		this->sAnimation();
		this->sCamera();
		this->sGUI();
	}

	if (!this->player()->isActive()) {
		this->spawnPlayer();
	}
}

void ScenePlay::sAI() {
	auto player = this->player();
	for (auto npc : this->m_entityManager.getEntities(EntityTag::NPC)) {
		if (npc->has<CPatrol>()) {
			auto& npcTransform = npc->get<CTransform>();
			auto& npcPatrol = npc->get<CPatrol>();
			Vec2f currentVelocity = npcPatrol.m_velocities[npcPatrol.m_currentPosition];
			Vec2f nextPos = npcPatrol.m_positions[(npcPatrol.m_currentPosition + 1) % npcPatrol.m_positions.size()];
			
			Vec2f nextTarget = npcTransform.m_pos + currentVelocity;

			if (currentVelocity.x > 0 && nextTarget.x > nextPos.x ||
				currentVelocity.x < 0 && nextTarget.x < nextPos.x ||
				currentVelocity.y > 0 && nextTarget.y > nextPos.y ||
				currentVelocity.y < 0 && nextTarget.y < nextPos.y) {

				Vec2f diff = nextPos - npcTransform.m_pos;

				if (abs(diff.x) >= 1.0f && abs(diff.y) >= 1.0f) {
					diff.normalize();
					npcTransform.m_velocity = Vec2f(npcPatrol.m_speed * diff.x, npcPatrol.m_speed * diff.y);
				} else {
					npcTransform.m_velocity = Vec2f(0.0f, 0.0f);
				}

				npcPatrol.m_currentPosition = (npcPatrol.m_currentPosition + 1) % npcPatrol.m_positions.size();
				
			} else {
				npcTransform.m_velocity = currentVelocity;
			}
		}
		else if (npc->has<CFollowPlayer>()) {
			auto& npcTransform = npc->get<CTransform>();
			auto npcFollow = npc->get<CFollowPlayer>();
			auto playerPos = player->get<CTransform>().m_pos;
			auto npcPos = npcTransform.m_pos;

			Vec2f aPos((npcPos.x + playerPos.x) / 2.0f, (npcPos.y + playerPos.y) / 2.0f);
			Vec2f aHalfSize(abs(npcPos.x - playerPos.x) / 2.0f, abs(npcPos.y - playerPos.y) / 2.0f);

			bool isIntersect = false;
			for (auto tile : this->m_entityManager.getEntities(EntityTag::TILE)) {
				if (tile->has<CBoundingBox>() && tile->get<CBoundingBox>().m_blockVision) {
					Vec2f overlap = Physics::GetOverlap(aPos, aHalfSize, tile);

					if (overlap.x > 0.0f && overlap.y > 0.0f) {
						if (Physics::EntityIntersect(playerPos, npcPos, tile)) {
							isIntersect = true;

							Vec2f diff = npcFollow.m_home - npcTransform.m_pos;

							if (abs(diff.x) >= 1.0f || abs(diff.y) >= 1.0f) {
								diff.normalize();
								npcTransform.m_velocity = Vec2f(npcFollow.m_speed * diff.x, npcFollow.m_speed * diff.y);
							}
							else {
								npcTransform.m_velocity = Vec2f(0.0f, 0.0f);
							}

							break;
						}
					}
				}
			}

			if (!isIntersect) {
				Vec2f desired = player->get<CTransform>().m_pos - npcTransform.m_pos;
				if (abs(desired.x) >= 1.0f || abs(desired.y) >= 1.0f) {
					desired.normalize();
					npcTransform.m_velocity = Vec2f(npcFollow.m_speed * desired.x, npcFollow.m_speed * desired.y);
					desired *= npcTransform.m_velocity.length();

					Vec2f steering = (desired - npcTransform.m_velocity) * npcFollow.m_velocityScale;

					npcTransform.m_velocity += steering;
				}
				else {
					npcTransform.m_velocity = Vec2f(0.0f, 0.0f);
				}
			}
		}
	}
}

void ScenePlay::sMovement() {
	if (this->player()->isActive()) {
		auto& input = this->player()->get<CInput>();
		auto& transform = this->player()->get<CTransform>();
		auto& playerState = this->player()->get<CState>();

		float playerVelocityY = 0.0f;
		if (input.m_up && !(input.m_down || input.m_right || input.m_left)) {
			playerVelocityY -= this->m_playerConfig.SPEED;
		}

		if (input.m_down && !(input.m_up || input.m_right || input.m_left)) {
			playerVelocityY += this->m_playerConfig.SPEED;
		}

		float playerVelocityX = 0.0f;
		if (input.m_left && !(input.m_down || input.m_right || input.m_up)) {
			playerVelocityX -= this->m_playerConfig.SPEED;
			transform.m_scale.x = -1.0f;
		}

		if (input.m_right && !(input.m_down || input.m_left || input.m_up)) {
			playerVelocityX += this->m_playerConfig.SPEED;
			transform.m_scale.x = 1.0f;
		}

		transform.m_velocity = Vec2f(playerVelocityX, playerVelocityY);

		if (transform.m_velocity == Vec2f(0.0f, 0.0f)) {
			switch (playerState.m_prevState)
			{
			case PlayerState::RUN_RIGHT:
				if (playerState.m_state != PlayerState::STAND_RIGHT) {
					playerState.m_prevState = playerState.m_state;
					playerState.m_state = PlayerState::STAND_RIGHT;
				}
				break;
			case PlayerState::RUN_DOWN:
				if (playerState.m_state != PlayerState::STAND_DOWN) {
					playerState.m_prevState = playerState.m_state;
					playerState.m_state = PlayerState::STAND_DOWN;
				}
				break;
			case PlayerState::RUN_UP:
				if (playerState.m_state != PlayerState::STAND_UP) {
					playerState.m_prevState = playerState.m_state;
					playerState.m_state = PlayerState::STAND_UP;
				}
				break;
			default:
				break;
			}
		} else if (playerVelocityX != 0.0f) {
			if (playerState.m_state != PlayerState::RUN_RIGHT) {
				playerState.m_prevState = playerState.m_state;
				playerState.m_state = PlayerState::RUN_RIGHT;
			}
		} else if (playerVelocityY > 0.0f) {
			if (playerState.m_state != PlayerState::RUN_DOWN) {
				playerState.m_prevState = playerState.m_state;
				playerState.m_state = PlayerState::RUN_DOWN;
			}
		}
		else if (playerVelocityY < 0.0f) {
			if (playerState.m_state != PlayerState::RUN_UP) {
				playerState.m_prevState = playerState.m_state;
				playerState.m_state = PlayerState::RUN_UP;
			}
		}
	}

	for (auto entity : this->m_entityManager.getEntities()) {
		if (entity->isActive() && entity->has<CTransform>()) {
			auto& entityTransform = entity->get<CTransform>();
			entityTransform.m_prevPos = entityTransform.m_pos;
			entityTransform.m_pos += entityTransform.m_velocity;
		}
	}
}

void ScenePlay::sStatus() {
	for (auto e : this->m_entityManager.getEntities()) {
		if (e->isActive()) {
			if (e->has<CLifeSpan>()) {
				auto& lifespan = e->get<CLifeSpan>();
				if (lifespan.m_remaining > 0) {
					lifespan.m_remaining--;
				} else {
					auto& playerState = this->player()->get<CState>();
					if (e->tag() == EntityTag::SWORD) {
						switch (playerState.m_prevState)
						{
						case PlayerState::ATTACK_RIGHT:
							playerState.m_prevState = playerState.m_state;
							playerState.m_state = PlayerState::STAND_RIGHT;
							break;
						case PlayerState::ATTACK_DOWN:
							playerState.m_prevState = playerState.m_state;
							playerState.m_state = PlayerState::STAND_DOWN;
							break;
						case PlayerState::ATTACK_UP:
							playerState.m_prevState = playerState.m_state;
							playerState.m_state = PlayerState::STAND_UP;
							break;
						default:
							break;
						}
					}

					e->destroy();
				}
			}

			if (e->has<CInvincibility>()) {
				auto& eInvincibility = e->get<CInvincibility>();

				if (eInvincibility.m_iFrames > 0) {
					eInvincibility.m_iFrames--;
				} 

				auto& sprite = e->get<CAnimation>().m_animation.getSprite();
				auto spriteColor = sprite.getColor();
				
				if (eInvincibility.m_iFrames > 0) {
					spriteColor.a = 128;
				} else {
					spriteColor.a = 255;
					e->remove<CInvincibility>();
				}

				sprite.setColor(spriteColor);
			}
		}
	}
}

void ScenePlay::sCollision() {
	auto player = this->player();
	auto& playerPos = player->get<CTransform>().m_pos;

	EntityTagMask tile_portal_heartMask;
	tile_portal_heartMask |= EntityTag::TILE;
	tile_portal_heartMask |= EntityTag::PORTAL;
	tile_portal_heartMask |= EntityTag::HEART;

	EntityTagMask player_npcMask;
	player_npcMask |= EntityTag::PLAYER;
	player_npcMask |= EntityTag::NPC;

	for (auto entity : this->m_entityManager.getEntities(tile_portal_heartMask)) {
		if (entity->isActive()) {
			for (auto creature : this->m_entityManager.getEntities(player_npcMask)) {
				if (creature->isActive()) {
					Vec2f overlap = Physics::GetOverlap(creature, entity);
					Vec2f prevOverlap = Physics::GetPreviousOverlap(creature, entity);

					if (overlap.x > 0.0f && overlap.y > 0.0f) {
						if (entity->tag() == EntityTag::HEART) {
							auto& creatureHealth = creature->get<CHealth>();
							if (creatureHealth.m_current != creatureHealth.m_max) {
								creatureHealth.m_current = creatureHealth.m_max;
								entity->destroy();
								this->m_game->playSound(SoundTag::GET_ITEM);
							}
						} else if (entity->tag() == EntityTag::PORTAL) {
							auto& creatureTransform = creature->get<CTransform>();
							auto creatureBoundigBox = creature->get<CBoundingBox>();
							if (Physics::IsInside(
								creatureTransform.m_pos + creatureBoundigBox.m_halfSize * creatureTransform.m_scale.y, 
								entity)) {
								auto portals = this->m_entityManager.getEntities(EntityTag::PORTAL);
								size_t currentPortalId = entity->id();
								EntityVec filteredPortals;
								std::copy_if(portals.begin(), portals.end(), std::back_inserter(filteredPortals),
									[currentPortalId](const std::shared_ptr<Entity>& portal) {
										return portal->id() != currentPortalId;
									});

								// TODO: CHECK IF THERE IS BLOCK MOVEMENT TILE
								creatureTransform.m_pos = filteredPortals[rand() % filteredPortals.size()]->get<CTransform>().m_pos +
									creatureBoundigBox.m_halfSize;

								if (creature->tag() == EntityTag::PLAYER) {
									auto& playerState = creature->get<CState>();

									if (playerState.m_state != PlayerState::STAND_DOWN) {
										playerState.m_prevState = playerState.m_state;
										playerState.m_state = PlayerState::STAND_DOWN;
									}
								}
							}
						} else if (entity->get<CBoundingBox>().m_blockMove) {
							auto& entityPos = entity->get<CTransform>().m_pos;
							auto& creaturePos = creature->get<CTransform>().m_pos;
							auto& creaturePrevPos = creature->get<CTransform>().m_prevPos;

							if (prevOverlap.y > 0.0f) {
								creaturePos.x += creaturePrevPos.x < entityPos.x ?
									-overlap.x : overlap.x;
							}

							if (prevOverlap.x > 0.0f) {
								creaturePos.y += creaturePrevPos.y > entityPos.y ?
									overlap.y : -overlap.y;
							}
						}
					}
				}
			}
		}
	}

	for (auto npc : this->m_entityManager.getEntities(EntityTag::NPC)) {
		if (npc->isActive()) {
			if (player->isActive() && !player->has<CInvincibility>()) {
				Vec2f overlap = Physics::GetOverlap(player, npc);

				if (overlap.x > 0.0f && overlap.y > 0.0f) {
					auto& playerHealth = player->get<CHealth>();
					auto& npcDamage = npc->get<CDamage>();

					playerHealth.m_current -= npcDamage.m_damage;

					if (playerHealth.m_current <= 0) {
						auto explosion = m_entityManager.addEntity(EntityTag::EXPLOSION);
						explosion->add<CAnimation>(this->m_game->assets().getAnimation(AnimationTag::EXPLOSION), false);
						explosion->add<CTransform>(player->get<CTransform>().m_pos);

						player->destroy();
						this->m_game->playSound(SoundTag::LINK_DIE);
					} else {
						player->add<CInvincibility>(60.0f);
						this->m_game->playSound(SoundTag::LINK_HURT);
					}
				}
			}

			for (auto sword : this->m_entityManager.getEntities(EntityTag::SWORD)) {
				if (sword->isActive()) {
					Vec2f overlap = Physics::GetOverlap(sword, npc);

					if (overlap.x > 0.0f && overlap.y > 0.0f) {
						auto& npcHealth = npc->get<CHealth>();
						auto& swordDamage = sword->get<CDamage>();

						npcHealth.m_current -= swordDamage.m_damage;

						if (npcHealth.m_current <= 0) {
							auto explosion = m_entityManager.addEntity(EntityTag::EXPLOSION);
							explosion->add<CAnimation>(this->m_game->assets().getAnimation(AnimationTag::EXPLOSION), false);
							explosion->add<CTransform>(npc->get<CTransform>().m_pos);

							npc->destroy();
							this->m_game->playSound(SoundTag::ENEMY_DIE);
						}

						swordDamage.m_damage = 0;
					}
				}
			}
		}
	}
}

void ScenePlay::sAnimation() {
	auto player = this->player();
	if (player->isActive()) {
		auto& playerState = player->get<CState>();
		if (playerState.m_state != playerState.m_prevState) {
			playerState.m_prevState = playerState.m_state;
			player->add<CAnimation>(this->m_game->assets().getAnimation(
				strAnimationTagMap.at(playerStateStrMap.at(player->get<CState>().m_state))), true);
		}
	}

	for (auto entity : this->m_entityManager.getEntities()) {
		if (entity->has<CAnimation>() && entity->isActive()) {
			entity->get<CAnimation>().m_animation.update();

			if (!entity->get<CAnimation>().m_repeat &&
				entity->get<CAnimation>().m_animation.hasEnded()) {
				entity->destroy();
			}
		}
	}
}

void ScenePlay::sDoAction(const Action& action) {
	if (this->player()->isActive()) {
		auto& inputComponent = this->player()->get<CInput>();
		if (action.type() == ActionType::START) {
			switch (action.name())
			{
			case static_cast<uint32_t>(ScenePlayAction::TOGGLE_TEXTURE): {
				this->m_drawTextures = !this->m_drawTextures;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::TOGGLE_COLLISION): {
				this->m_drawCollision = !this->m_drawCollision;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::TOGGLE_GRID): {
				this->m_drawGrid = !this->m_drawGrid;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::TOGGLE_FOLLOW): {
				this->m_follow = !this->m_follow;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::PAUSE): {
				this->setPaused(!this->m_paused);
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::QUIT): {
				this->onEnd();
				this->m_game->stopSound(SoundTag::MUSIC_PLAY);
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::LEFT): {
				inputComponent.m_left = true;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::RIGHT): {
				inputComponent.m_right = true;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::UP): {
				inputComponent.m_up = true;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::DOWN): {
				inputComponent.m_down = true;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::ATTACK): {
				inputComponent.m_attack = true; this->spawnSword(this->player());
				break;
			}
			default:
				break;
			}
		}
		else if (action.type() == ActionType::END) {
			switch (action.name())
			{
			case static_cast<uint32_t>(ScenePlayAction::LEFT): {
				inputComponent.m_left = false;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::RIGHT): {
				inputComponent.m_right = false;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::UP): {
				inputComponent.m_up = false;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::DOWN): {
				inputComponent.m_down = false;
				break;
			}
			case static_cast<uint32_t>(ScenePlayAction::ATTACK): {
				inputComponent.m_attack = false;
				break;
			}
			default:
				break;
			}
		}
	}
}

void ScenePlay::onEnd() {
	this->m_game->changeScene(SceneTag::MENU, nullptr, true);
}

void ScenePlay::sRender() {
	this->m_game->window().clear(!this->m_paused ?
		sf::Color(255, 192, 122) : sf::Color(205, 142, 72));

	sf::RectangleShape tick({ 1.0f, 6.0f });
	tick.setFillColor(sf::Color::Black);

	if (this->m_drawTextures) {
		for (auto e : this->m_entityManager.getEntities()) {
			auto& transform = e->get<CTransform>();
			sf::Color c = sf::Color::White;
			if (e->has<CInvincibility>()) {
				c = sf::Color(255, 255, 255, 128);
			}

			if (e->has<CAnimation>()) {
				auto& animation = e->get<CAnimation>().m_animation;
				animation.getSprite().setRotation(transform.m_angle);
				animation.getSprite().setPosition(transform.m_pos.x, transform.m_pos.y);
				animation.getSprite().setScale(transform.m_scale.x, transform.m_scale.y);
				animation.getSprite().setColor(c);
				this->m_game->window().draw(animation.getSprite());
			}
		}

		// draw health bars
		for (auto e : this->m_entityManager.getEntities()) {
			auto& transform = e->get<CTransform>();
			if (e->has<CHealth>()) {
				auto& h = e->get<CHealth>();
				Vec2f size(64, 6);
				sf::RectangleShape rect({ size.x, size.y });
				rect.setPosition(transform.m_pos.x - 32, transform.m_pos.y - 48);
				rect.setFillColor(sf::Color(96, 96, 96));
				rect.setOutlineColor(sf::Color::Black);
				rect.setOutlineThickness(2.0f);
				this->m_game->window().draw(rect);

				float ratio = static_cast<float>(h.m_current) / h.m_max;
				size.x *= ratio;
				rect.setSize({ size.x, size.y });
				rect.setFillColor(sf::Color(255, 0, 0));
				rect.setOutlineThickness(0.0f);
				this->m_game->window().draw(rect);

				for (int i = 0; i < h.m_max; ++i) {
					tick.setPosition(rect.getPosition() + sf::Vector2f(i * 64.0f * 1 / h.m_max, 0.0f));
					this->m_game->window().draw(tick);
				}
			}
		}
	}

	if (this->m_drawGrid) {
		float leftX = this->m_game->window().getView().getCenter().x - width() / 2;
		float rightX = leftX + width() + this->m_gridSize.x;
		float topY = this->m_game->window().getView().getCenter().y - height() / 2;
		float bottomY = topY + height() + this->m_gridSize.y;
		float nextGridX = leftX - (static_cast<int>(leftX) %
			static_cast<int>(this->m_gridSize.x));
		float nextGridY = topY - (static_cast<int>(topY) %
			static_cast<int>(this->m_gridSize.y));

		for (float x = nextGridX; x < rightX; x += this->m_gridSize.x) {
			drawLine(Vec2f(x, topY), Vec2f(x, bottomY));
		}

		for (float y = nextGridY; y < bottomY; y += this->m_gridSize.y) {
			drawLine(Vec2f(leftX, y), Vec2f(rightX, y));

			for (float x = nextGridX; x < rightX; x += this->m_gridSize.x) {
				std::string xCell = std::to_string(static_cast<int>(x) /
					static_cast<int>(this->m_gridSize.x));
				std::string yCell = std::to_string(static_cast<int>(y) /
					static_cast<int>(this->m_gridSize.y));
				this->m_gridText.setString("(" + xCell + "," + yCell + ")");
				this->m_gridText.setPosition(x + 3.0f, y + 2.0f);
				this->m_game->window().draw(this->m_gridText);
			}
		}
	}

	if (this->m_drawCollision) {
		sf::CircleShape dot(4.0f);
		dot.setFillColor(sf::Color::Black);
		for (auto e : this->m_entityManager.getEntities()) {
			if (e->has<CBoundingBox>()) {
				auto& box = e->get<CBoundingBox>();
				auto& transform = e->get<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.m_size.x - 1, box.m_size.y - 1));
				rect.setOrigin(sf::Vector2f(box.m_halfSize.x, box.m_halfSize.y));
				rect.setPosition(transform.m_pos.x, transform.m_pos.y);
				rect.setFillColor(sf::Color::Transparent);

				if (box.m_blockMove && box.m_blockVision) {
					rect.setOutlineColor(sf::Color::Black);
				}
				else if (box.m_blockMove && !box.m_blockVision) {
					rect.setOutlineColor(sf::Color::Blue);
				}
				else if (!box.m_blockMove && box.m_blockVision) {
					rect.setOutlineColor(sf::Color::Red);
				}
				else if (!box.m_blockMove && !box.m_blockVision) {
					rect.setOutlineColor(sf::Color::White);
				}

				rect.setOutlineThickness(1.0f);
				this->m_game->window().draw(rect);
			}

			if (e->has<CPatrol>()) {
				auto& patrol = e->get<CPatrol>().m_positions;
				for (size_t p = 0; p < patrol.size(); ++p) {
					dot.setPosition(patrol[p].x, patrol[p].y);
					this->m_game->window().draw(dot);
				}
			}

			if (e->has<CFollowPlayer>()) {
				sf::VertexArray lines(sf::LinesStrip, 2);
				lines[0].position.x = e->get<CTransform>().m_pos.x;
				lines[0].position.y = e->get<CTransform>().m_pos.y;
				lines[0].color = sf::Color::Black;
				lines[1].position.x = player()->get<CTransform>().m_pos.x;
				lines[1].position.y = player()->get<CTransform>().m_pos.y;
				lines[1].color = sf::Color::Black;
				this->m_game->window().draw(lines);
				dot.setPosition(e->get<CFollowPlayer>().m_home.x,
					e->get<CFollowPlayer>().m_home.y);
				this->m_game->window().draw(dot);
			}
		}
	}
}

Vec2f ScenePlay::getPosition(int rx, int ry, int tx, int ty) const {
	return Vec2f(
		rx * static_cast<float>(this->width()) + tx * this->m_gridSize.x + this->m_gridSize.x / 2.0f,
		ry * static_cast<float>(this->height()) + ty * this->m_gridSize.y + this->m_gridSize.y / 2.0f
	);
}

std::shared_ptr<Entity> ScenePlay::player() {
	auto& entities = this->m_entityManager.getEntities(EntityTag::PLAYER);
	if (entities.empty()) {
		std::cerr << "There is currently no player entity" << std::endl;
		exit(-1);
	}

	return entities[0];
}

void ScenePlay::sCamera() {
	sf::View view = this->m_game->window().getView();
	auto player = this->player();
	auto pPos = player->get<CTransform>().m_pos;
	auto viewSize = view.getSize();

	float windowCenterX{ 0.0f }, windowCenterY{ 0.0f };

	if (this->m_follow) {
		float diffX = pPos.x - viewSize.x / 2.0f;
		float diffY = pPos.y - viewSize.y / 2.0f;

		windowCenterX = std::max(
			diffX >= 0 ? view.getSize().x / 2.0f : diffX, 
			pPos.x);
		windowCenterY = std::max(
			diffY >= 0 ? view.getSize().y / 2.0f : diffY,
			pPos.y);
	} else {
		int RX = pPos.x >= 0.0f ?
			pPos.x / viewSize.x : (pPos.x - viewSize.x) / viewSize.x;
		int RY = pPos.y >= 0.0f ?
			pPos.y / viewSize.y : (pPos.y - viewSize.y) / viewSize.y;

		windowCenterX = viewSize.x * RX + viewSize.x / 2.0f;
		windowCenterY = viewSize.y * RY + viewSize.y / 2.0f;
	}

	view.setCenter(windowCenterX, windowCenterY);
	this->m_game->window().setView(view);
}

void ScenePlay::sGUI() {
	ImGui::Begin("Scene Properties");

	if (ImGui::BeginTabBar("MyTabBar"))
	{
		if (ImGui::BeginTabItem("Debug"))
		{
			ImGui::Checkbox("Draw Grid",		&this->m_drawGrid);
			ImGui::Checkbox("Draw Textures",	&this->m_drawTextures);
			ImGui::Checkbox("Draw Debug",		&this->m_drawDebug);
			ImGui::Checkbox("Follow Cam",		&this->m_follow);

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Sounds"))
		{
			for (const auto& sound : this->m_game->assets().getSounds()) {
				if (ImGui::Button(("Play##" + soundTagStrMap.at(sound.first)).c_str())) {
					this->m_game->playSound(sound.first);
				}

				ImGui::SameLine();

				if (ImGui::Button(("Stop##" + soundTagStrMap.at(sound.first)).c_str())) {
					this->m_game->stopSound(sound.first);
				}

				ImGui::SameLine();

				ImGui::Text("%s", soundTagStrMap.at(sound.first).c_str());
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Animations"))
		{
			const size_t animInRow = 6;
			size_t currentAnimNum = 1;
			auto currentFrame = this->currentFrame();

			for (const auto& animation : this->m_game->assets().getAnimations()) {
				auto entityAnimation = animation.second;

				ImTextureID currentTextureID =
					reinterpret_cast<void*>(static_cast<intptr_t>(
						entityAnimation.getSprite().getTexture()->getNativeHandle()));

				auto entityUV = this->calcAnimationUV(entityAnimation);

				ImGui::ImageButton(currentTextureID, { 48.0f, 48.0f }, ImVec2(entityUV.first.x, entityUV.first.y), 
					ImVec2(entityUV.second.x, entityUV.second.y));

				if (currentAnimNum % animInRow) {
					ImGui::SameLine();
				}

				currentAnimNum++;
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Entity Manager"))
		{
			auto currentFrame = this->currentFrame();
			auto constructEntityTable{ [currentFrame, this](const EntityVec& entities) {
				if (ImGui::BeginTable("Entities", 4)) {
					for (auto e : entities) {
						ImGui::TableNextRow();

						ImGui::TableNextColumn();

						auto entityAnimation = e->get<CAnimation>().m_animation;

						ImTextureID currentTextureID =
							reinterpret_cast<void*>(static_cast<intptr_t>(
								entityAnimation.getSprite().getTexture()->getNativeHandle()));

						auto entityUV = this->calcAnimationUV(entityAnimation);

						ImGui::ImageButton(currentTextureID, { 24.0f, 24.0f }, ImVec2(entityUV.first.x, entityUV.first.y),
							ImVec2(entityUV.second.x, entityUV.second.y));

						ImGui::TableNextColumn();
						ImGui::Text("%d", e->id());
						ImGui::TableNextColumn();
						ImGui::Text("%s", entityTagStrMap.at(e->tag()).c_str());
						ImGui::TableNextColumn();
						ImGui::Text("(%.f, %.f)", e->get<CTransform>().m_pos.x, e->get<CTransform>().m_pos.y);
					}
					ImGui::EndTable();
				}
			} };

			if (ImGui::CollapsingHeader("Entities by Tag")) {
				ImGui::PushItemWidth(100);
				for (const auto& tag : entityTagStrMap) {
					if (tag.first != EntityTag::UNDEFINED) {
						ImGui::Indent();
						if (ImGui::CollapsingHeader(tag.second.c_str())) {
							constructEntityTable(this->m_entityManager.getEntities(tag.first));
						}
						ImGui::Unindent();
					}
				}
				
				ImGui::PopItemWidth();
			}

			if (ImGui::CollapsingHeader("All Entities")) {
				constructEntityTable(this->m_entityManager.getEntities());
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

std::pair<Vec2f, Vec2f> ScenePlay::calcAnimationUV(const Animation& animation) {
	auto currentFrame = this->m_currentFrame;
	auto entityAnimationTextureWidth = animation.getSprite().getTexture()->getSize().x;
	auto amountOfSprites = entityAnimationTextureWidth / static_cast<int>(animation.getSize().x);

	Vec2f uv0 = Vec2f(((currentFrame / 16) % amountOfSprites) * animation.getSize().x /
		entityAnimationTextureWidth, 0.0f);
	Vec2f uv1 = Vec2f((((currentFrame / 16) % amountOfSprites) * animation.getSize().x + animation.getSize().x) /
		entityAnimationTextureWidth, 1.0f);

	return { uv0, uv1 };
}
