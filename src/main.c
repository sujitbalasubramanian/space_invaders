#include "game.h"

int main(void) {
  InitGame();

  while (!WindowShouldClose()) {
    CheckKeyBindingEvents();

    if (IsPaused()) {
      PauseCanvas();
    } else {
      UpdateGameCanvas();
    }
  }

  EndGame();
  CloseWindow();
  return 0;
}
