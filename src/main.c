#include "game.h"

int main(void) {
  InitGame();

  while (!WindowShouldClose()) {
    CheckKeyBindingEvents();
    PauseLock();
    UpdateGameCanvas();
  }

  UnloadAssets();
  CloseWindow();
  return 0;
}
