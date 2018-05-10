#pragma once

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
//const int SCREEN_WIDTH = 1280;
//const int SCREEN_HEIGHT = 720;
const int LEVEL_WIDTH = 2800;
const int LEVEL_HEIGHT = 1800;
const int TILE_WIDTH = 100;
const float BULLET_LIFETIME = 4;

const float PLAYER_SPEED = 400;
const float PLAYER_BULLET_SPEED = 600;
const int PLAYER_BULLET_POOL_SIZE = 32;
const int PLAYER_HEALTH = 5;
const float PLAYER_FIRE_RATE = 0.4f;
const float DASH_TIME = 0.1f;
const float DASH_SPEED = 3000;
const float CURSOR_RADIUS = 100;

const float ENEMY_SPEED = 250;
const float ENEMY_BULLET_SPEED = 450;
const int ENEMY_BULLET_POOL_SIZE = 250;
const int ENEMY_HEALTH = 3;
const int NUM_ENEMIES = 8;
const float ENEMY_FIRE_RATE = 1;
const float ENEMY_DISTANCE_FROM_PLAYER = 400;
const int NUM_INITIAL_ENEMIES = 3;
const int ENEMY_WAVE_SIZE = 2;

const float BOSS_SPEED = 150;
const int BOSS_BULLET_SPEED = 350;
const float BOSS_FIRE_RATE = 0.35f;
const float BOSS_ROTATION_SPEED = 40;
const int BOSS_HEALTH = 50;
const float BOSS_ATTACK_DURATION = 8;
const float BOSS_PAUSE_DURATION = 3;
const Vector2 INITIAL_BOSS_POSITION = Vector2(800, 800);

