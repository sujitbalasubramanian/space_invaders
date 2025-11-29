#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdbool.h>

void InitGame(void);
void UnloadAssets(void);
void CheckKeyBindingEvents(void);
void PauseLock(void);
void UpdateGameCanvas(void);

#endif // GAME_H
