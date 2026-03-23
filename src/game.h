#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdbool.h>

typedef struct {
  Image image;
  Texture2D texture;
  Vector2 position;
  Color color;
  int life;
} Ship;

void InitWave(void);

void InitGame(void);
void EndGame(void);

void PauseGame(void);
bool IsPaused(void);
void CheckKeyBindingEvents(void);
void PauseCanvas(void);

void GameStartCanvas(void);
void GameOverCanvas(void);

void RemoveOutOfSopeBullets(void);
void ShootPlayerBullet(void);
void ShootEnemyBullet(void);
void CheckCollision(void);
void CheckWaveEnd(void);
void UpdateGame(void);
void DrawGameCanvas(void);

void UpdateGameCanvas(void);

#endif // GAME_H
