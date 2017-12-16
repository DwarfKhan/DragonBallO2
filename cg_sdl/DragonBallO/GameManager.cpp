/*
enemy attacks
	attack animations
		rework guard spritesheet
	player heath / death
		respawning?
		hp indicator (gui element class?)
	attack movements?
		would need to turn off move() temporarily
	attack effects?

game intro
	find main function
	display splash screen
	display main menu
		start game
		options
			sound
		save/load?
		credits
			myself
			music authors
			oden
			stephen

items
	inventory
		pause game
		sorted by use
			eg: consumables, weapons, quest items, gp
			idea: random garbage items that appear in your inventory to simulate the passage of time
				eg: pocket lint, empty bottles, crumbs
					could be thrown?
	gui
	mobs drop money / consumables
	item ideas:
		sensu bean restores full hp

levels
	seperate game into managable chunks with fewer entities
	level plan:
		starting village to introduce core mechanics
			farm with scarecrows for target practice
			basic shop
			smashable crates/boxes for small amounts of money
			introduce love interest
			love interest captured
			prepare for rescue quest
		crossroads village
			side quests?
			learn of foreigners conquering all in path
			establish doubt in the foreign threat through npc dialogue
				"threats from another world, how original..."
				"these doom prophecies are getting less and less believable"
				"i have more important things to worry about than magic men"
			mentor/sage character convinces player to get his priorities straight, romance problems can wait
				cutscene of destruction of sage's home by invaders
		temple
			a lot of potential for jokes about religion / god here
			elders proclaim player fits into incredibly vague prophecy as the chosen one
			send player on true quest to defeat invaders, first gathering powerful allies and artifacts
			cutscene of another young adventurer being told the same speech right afterwards
		refugee camp
			people fleeing destructioon of invaders
			other groups of adventurers going to join the fight
			establish sense of urgency
				"soon the whole world will be consumed by this chaos..."
				"we should just submit to them, nobody can stop them"
				"abandon your quest, this is god's judgement and it cant be stopped"




*/

#include "GameManager.h"
#include "SDLInit.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "MoveTrigger.h"
#include "LivingThing.h"
#include "Weapon.h"
#include "CollisionLayer.h"
#include "ProxTrigger.h"
#include "Grid.h"

#define CAMERA_MODE Camera::Mode::PAN
#define SHOW_COLLIDERS false

//Also camera dimension...
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

extern Grid gGrid;

Camera gCamera(CAMERA_MODE);
World gWorld;

SDLInit sdlInit;

namespace {
	Player player;
		Weapon playerWeapon;
	Sprite tree;
	Sprite boulder;
	LivingThing target;
		Animation targetIdle;
		Animation targetDamage;
		Animation targetDeath;
	LivingThing guard;
		Weapon guardWeapon;
		Animation guardDisplayAll;
		Animation guardIdle;
		Animation guardDamage;
		Animation guardDeath;
		Animation guardMoveUp;
		Animation guardMoveDown;
		Animation guardMoveLeft;
		Animation guardMoveRight;
		Animation guardAttackUp;
		Animation guardAttackDown;
		Animation guardAttackLeft;
		Animation guardAttackRight;
		//ProxTrigger guardProx;
	MoveTrigger moveTrigger;

	CollisionLayer lLivingThings;
	CollisionLayer lWeapons;
	CollisionLayer lMoveTriggers;
}

void InitEntities() {

//PLAYER

	//path name
	player.SetTexturePath("textures/link_sheet.png");

	//load texture
	sdlInit.LoadTexture(player);

	//position
	player.SetPosition({0, 0});

	//size
	player.SetSize(70, 70);

	//Init Sprite Sheet
	player.InitSpriteSheet(0, 14, 6);

	//walk sprite clips
	player.SetSpriteClip(90, 1, 30, 30, 3);			//up...
	player.SetSpriteClip(90, 31, 30, 30, 17);		//up move...
	player.SetSpriteClip(30, 1, 30, 30, 1);			//down...
	player.SetSpriteClip(30, 31, 30, 30, 15);		//down move...
	player.SetSpriteClip(120, 1, 30, 30, 4);		//right...
	player.SetSpriteClip(120, 31, 30, 30, 18);		//right move...
	player.SetSpriteClip(60, 1, 30, 30, 2);			//left...
	player.SetSpriteClip(60, 31, 30, 30, 16);		//left move...

	//Attack sprite clips
	player.SetSpriteClip(170, 141, 30, 31, 5);		//up attack...
	player.SetSpriteClip(173, 71, 30, 30, 6);		//left attack...
	player.SetSpriteClip(203, 142, 30, 30, 7);		//right attack...
	player.SetSpriteClip(295, 59, 30, 32, 8);		//down attack...

	//attack anchor positions
	player.SetAnchorOffset({-16, -13}, 5);			//up attack...
	player.SetAnchorOffset({-26, -5}, 6);			//left attack...
	player.SetAnchorOffset({4, 0}, 7);				//right attack...
	player.SetAnchorOffset({ -23, 8 }, 8);		//down attack...
	
	//collision
	player.ConfigureCollision(true, true, { 2, 17 }, { 36, 20 });
	lLivingThings.AddEntity(player);
	
	//setWeapon
	player.SetWeapon(&playerWeapon, 33);



//END PLAYER

//PLAYERWEAPON



	//size
	playerWeapon.SetSize(5,5);

	//collision
	playerWeapon.ConfigureCollision(false, true);
	lWeapons.AddEntity(playerWeapon);

//END PLAYERWEAPON

//TREE

	//path name
	tree.SetTexturePath("textures/tree_green.gif");

	//load texture
	sdlInit.LoadTexture(tree);

	//position
	tree.SetPosition({ 200, 300 });

	//size
	tree.SetSize(64, 78);

	//collision
	tree.ConfigureCollision(true, false);
	lLivingThings.AddEntity(tree);

//END TREE

//BOULDER

	//path name
	boulder.SetTexturePath("textures/boulder.png");

	//load texture
	sdlInit.LoadTexture(boulder);

	//position
	boulder.SetPosition({ 200, 150 });

	//size
	boulder.SetSize(45, 45);

	//collision
	boulder.ConfigureCollision(true, true);
	lLivingThings.AddEntity(boulder);


//END BOULDER

//MOVETRIGGER

	//position
	moveTrigger.SetPosition({ 300, 150 });

	//size
	moveTrigger.SetSize(45, 45);

	//collision
	moveTrigger.ConfigureCollision(false, false);
	lMoveTriggers.AddEntity(moveTrigger);

	//destination
	moveTrigger.SetMovePos({ 960, 200 });

//END MOVETRIGGER

//TARGET

	//path name
	target.SetTexturePath("textures/scarecrow.png");

	//load texture
	sdlInit.LoadTexture(target);

	//init sprite sheet
	target.InitSpriteSheet(0, 4, 2);

	//sprite clips
	target.SetSpriteClip(0, 0, 31, 31, 0);
	target.SetSpriteClip(32, 0, 32, 32, 1);
	target.SetSpriteClip(64, 0, 32, 32, 2);
	target.SetSpriteClip(96, 0, 32, 32, 3);
	target.SetSpriteClip(0, 32, 32, 32, 4);
	target.SetSpriteClip(32, 32, 32, 32,5);

	//anchor offsets
	//target.SetAnchorOffset({ 0, -13 }, 0);

	//animation

	target.SetAnimIdle(&targetIdle);
	targetIdle.active = true;
	targetIdle.SetAnimSpeed(5);
	targetIdle.AddSpriteClip(0);
	targetIdle.AddSpriteClip(0);
	
	
	target.SetAnimDamage(&targetDamage);
	targetDamage.loops = false;
	targetDamage.SetAnimSpeed(15);
	targetDamage.AddSpriteClip(1);
	targetDamage.AddSpriteClip(1);


	target.SetAnimDeath(&targetDeath);
	targetDeath.loops = false;
	targetDeath.SetAnimSpeed(7.5);
	targetDeath.AddSpriteClip(1);
	targetDeath.AddSpriteClip(2);
	targetDeath.AddSpriteClip(3);
	targetDeath.AddSpriteClip(4);
	targetDeath.AddSpriteClip(5);


	//Health
	target.Destructible::SetHealth(3, 3);

	//position
	target.SetPosition({990 , 200 });

	//size
	target.SetSize(50, 50);

	//collision
	target.ConfigureCollision(true, false);
	lLivingThings.AddEntity(target);

	//sound
	target.SetDeathSound(sdlInit.sfxDeath01);
	

//END TARGET

// GUARD
	//health
	guard.Destructible::SetHealth(5,5);
	//Path name
	guard.SetTexturePath("textures/Guard.png");
	//Load Texture
	sdlInit.LoadTexture(guard);
	//Init sprite sheet
	guard.InitSpriteSheet(0, 11, 6);
	//position
	guard.SetPosition({408,220});
	//size
	guard.SetSize(80, 80);
	//collision
	guard.ConfigureCollision(false, true, { 2,3 }, {47,35});
	lLivingThings.AddEntity(guard);

	//weapon
	guard.SetWeapon(&guardWeapon);
		//size
		guardWeapon.SetSize(5, 5);

		//collision
		guardWeapon.ConfigureCollision(false, true);
		lWeapons.AddEntity(guardWeapon);
	//endweapon


	////proxTrigger
	//guardProx.SetFollowingEntity(guard);
	//guardProx.SetSize(80,80);
	//guardProx.ConfigureCollision(false,false);
	//guardProx.AddCollidableEntity(player);

	//navigation

	guard.SetMoveSpeed(70.0f);
	guard.moveState = LivingThing::MoveState::sDirectFollow;
	guard.attackState = LivingThing::AttackState::sAttack1;
	guard.SetFollowTarget(&player);

	//sound
	guard.SetAlertSound(sdlInit.sfxAlert01);
	guard.SetWalkSound(sdlInit.sfxWalk01);
	//guard.SetDamageSound(sdlInit.sfxDamage01);
	guard.SetDeathSound(sdlInit.sfxDeath01);

	//Spriteclips
	guard.SetSpriteClip(0  , 0  , 31, 31, 0);
	guard.SetSpriteClip(31 , 0  , 31, 31, 1);
	guard.SetSpriteClip(63 , 0  , 31, 31, 2);
	guard.SetSpriteClip(95 , 0  , 31, 31, 3);

	guard.SetSpriteClip(0  , 31 , 31, 31, 4);
	guard.SetSpriteClip(31 , 31 , 31, 31, 5);
	guard.SetSpriteClip(63 , 31 , 31, 31, 6);
	guard.SetSpriteClip(95 , 31 , 31, 31, 7);
 
	guard.SetSpriteClip(0  , 63 , 31, 31, 8);
	guard.SetSpriteClip(31 , 63 , 31, 31,  9);
	guard.SetSpriteClip(63 , 63 , 31, 31,  10);
	guard.SetSpriteClip(95 , 63 , 31, 31,  11);

	guard.SetSpriteClip(0  , 95 , 31, 31, 12);
	guard.SetSpriteClip(31 , 95 , 31, 31,  13);
	guard.SetSpriteClip(63 , 95 , 31, 31,  14);
	guard.SetSpriteClip(95 , 95 , 31, 31,  15);

	guard.SetSpriteClip(0  , 127, 31, 31, 16);
	guard.SetSpriteClip(31 , 127, 31, 31,  17);
	guard.SetSpriteClip(63 , 127, 31, 31,  18);

	guard.SetSpriteClip(0  , 159, 31, 31, 20);
	guard.SetSpriteClip(31 , 159, 31, 31,  21);
	guard.SetSpriteClip(63 , 159, 31, 31,  22);

	//AttackRight
	guard.SetSpriteClip(95 , 127, 31, 31,  19);
	guard.SetSpriteClip(95 , 159, 31, 31,  23);

	guard.SetSpriteClip(223, 31 , 31, 31, 24);
	guard.SetSpriteClip(255, 31 , 31, 31, 25);

	guard.SetSpriteClip(159, 0  , 31, 31, 26);
	guard.SetSpriteClip(159, 31 , 31, 31,  27);

	guard.SetSpriteClip(127, 0, 31, 31, 28);
	guard.SetSpriteClip(127, 31, 31, 31, 29);


	//Anchor offsets
	guard.SetAnchorOffset({ -3,2 }, 2);
	guard.SetAnchorOffset({ -3,1 }, 3);
	guard.SetAnchorOffset({ 2,-1 }, 4);

	guard.SetAnchorOffset({ -2,0 }, 6);
	guard.SetAnchorOffset({ -2,0 }, 7);
	guard.SetAnchorOffset({ -3,0 }, 9);



	guard.SetAnchorOffset({ 3,0 }, 12);
	guard.SetAnchorOffset({ 0,2 }, 13);

	guard.SetAnchorOffset({ -2,0 }, 17);
	guard.SetAnchorOffset({-30,0}, 19);
	guard.SetAnchorOffset({ 1,-15 }, 20);
	guard.SetAnchorOffset({ -2,-17 }, 21);

	guard.SetAnchorOffset({-32,0}, 23);
	guard.SetAnchorOffset({ -3,0 }, 25);

	guard.SetAnchorOffset({ -5,-3 }, 28);
	guard.SetAnchorOffset({ -5,-3 }, 29);

	//Animation
	//guard.animState = LivingThing::AnimState::sDisplayAll;

	guard.SetAnimIdle(&guardIdle);
	guardIdle.active = true;
	guardIdle.SetAnimSpeed(2.5);
	guardIdle.AddSpriteClip(16);
	
	
	guard.SetAnimDamage(&guardDamage);
	guardDamage.loops = false;
	guardDamage.SetAnimSpeed(15);
	guardDamage.AddSpriteClip(24);
	guardDamage.AddSpriteClip(24);
	guardDamage.AddSpriteClip(24);

	guard.SetAnimDeath(&guardDeath);
	guardDeath.loops = false;
	guardDeath.SetAnimSpeed(7.5);
	guardDeath.AddSpriteClip(25);

	//MOVE

	guard.SetAnimMoveUp(&guardMoveUp);
	guardMoveUp.loops = false;
	guardMoveUp.SetAnimSpeed(20);
	guardMoveUp.AddSpriteClip(4);
	guardMoveUp.AddSpriteClip(5);
	guardMoveUp.AddSpriteClip(4);
	guardMoveUp.AddSpriteClip(5);

	guard.SetAnimMoveDown(&guardMoveDown);
	guardMoveDown.loops = false;
	guardMoveDown.SetAnimSpeed(20);
	guardMoveDown.AddSpriteClip(8);
	guardMoveDown.AddSpriteClip(9);
	guardMoveDown.AddSpriteClip(8);
	guardMoveDown.AddSpriteClip(9);
	
	guard.SetAnimMoveLeft(&guardMoveLeft);
	guardMoveLeft.loops = false;
	guardMoveLeft.SetAnimSpeed(20);
	guardMoveLeft.AddSpriteClip(2);
	guardMoveLeft.AddSpriteClip(3);
	guardMoveLeft.AddSpriteClip(2);
	guardMoveLeft.AddSpriteClip(3);

	guard.SetAnimMoveRight(&guardMoveRight);
	guardMoveRight.loops = false;
	guardMoveRight.SetAnimSpeed(20);
	guardMoveRight.AddSpriteClip(12);
	guardMoveRight.AddSpriteClip(13);
	guardMoveRight.AddSpriteClip(12);
	guardMoveRight.AddSpriteClip(13);

	//ATTACKS

	guard.SetAnimAttackUp(&guardAttackUp);
	guardAttackUp.loops = false;
	guardAttackUp.SetAnimSpeed(20);
	guardAttackUp.AddSpriteClip(20); 
	guardAttackUp.AddSpriteClip(21);
	guardAttackUp.AddSpriteClip(20);
	guardAttackUp.AddSpriteClip(21);

	guard.SetAnimAttackDown(&guardAttackDown);
	guardAttackDown.loops = false;
	guardAttackDown.SetAnimSpeed(20);
	guardAttackDown.AddSpriteClip(29);
	guardAttackDown.AddSpriteClip(28);
	guardAttackDown.AddSpriteClip(29);

	guard.SetAnimAttackLeft(&guardAttackLeft);
	guardAttackLeft.loops = false;
	guardAttackLeft.SetAnimSpeed(20);
	guardAttackLeft.AddSpriteClip(19); 
	guardAttackLeft.AddSpriteClip(23);
	guardAttackLeft.AddSpriteClip(19);
	guardAttackLeft.AddSpriteClip(23);

	guard.SetAnimAttackRight(&guardAttackRight);
	guardAttackRight.loops = false; 
	guardAttackRight.SetAnimSpeed(20);
	guardAttackRight.AddSpriteClip(18);
	guardAttackRight.AddSpriteClip(22);
	guardAttackRight.AddSpriteClip(18);
	guardAttackRight.AddSpriteClip(22);


// END GUARD 

// Must come after all entities have been added to their layers
	lLivingThings.CollideWith(player);
	lLivingThings.CollideWith(boulder);
	lLivingThings.CollideWith(guard);
	lLivingThings.CollideWith(target);
	lLivingThings.CollideWith(tree);
	lLivingThings.CollideWith(playerWeapon);
	lMoveTriggers.CollideWith(player);

	//TODO: Don't hard-code this...
	gWorld.InitWorldGrid({ 0, 70 - 35, 14, 70 - 16});
}


bool GameManager::Init(){
	bool initSuccess = sdlInit.Setup();
	if (initSuccess) {
		InitEntities();
	}

	return initSuccess;
}

void GameManager::Cleanup(){
	sdlInit.CleanupSprite(player);
	sdlInit.CleanupSprite(tree);
	sdlInit.CleanupSprite(boulder);
	sdlInit.CleanupSprite(target);
	sdlInit.CleanupSprite(guard);
	sdlInit.Cleanup();
}

//TODO: Add deltatime later...
void GameManager::Update() {
	tree.Update();
	target.Update();
	boulder.Update();
	guard.Update();
	//guardProx.Update();
	player.Update();

	//Needs to come last...
	gCamera.LookAt(player);

	//gCamera.RestrictTargetToWorld(player);

	sdlInit.Update();
}

void GameManager::Render(){
	sdlInit.Render();

	sdlInit.DrawSprite(tree);
	sdlInit.DrawSprite(target);
	sdlInit.DrawSprite(guard);
	sdlInit.DrawSprite(boulder);
	sdlInit.DrawSprite(player);

	//Needs to come last...
	if (sdlInit.showColliders) {
		sdlInit.DrawEntityCollider(moveTrigger);
		sdlInit.DrawEntityCollider(target);
		sdlInit.DrawEntityCollider(tree);
		sdlInit.DrawEntityCollider(boulder);
		sdlInit.DrawEntityCollider(player);
		sdlInit.DrawEntityCollider(playerWeapon);
		sdlInit.DrawEntityCollider(guard);
		//sdlInit.DrawEntityCollider(guardProx);
	}
}
