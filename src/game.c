#include "game.h"

// Globals
static int ScreenWidth = 0;
static int ScreenHeight = 0;
static const char *ScreenTitle = "Space Invaders";

static bool Pause = false;
static bool GameStarted = false;
static bool GameOver = false;

static float speed = 5.0f;

static const char *PlayerImage = "assets/player.png";

typedef struct {
  Image image;
  Texture2D texture;
  Vector2 position;
  Color color;
} Player;

static Player player = {0};

// Implementations
void InitGame() {
#if defined(__EMSCRIPTEN__)
  ScreenWidth = 800;
  ScreenHeight = 600;
  InitWindow(ScreenWidth, ScreenHeight, ScreenTitle);
#else
  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(0, 0, ScreenTitle);

  ScreenWidth = GetScreenWidth();
  ScreenHeight = GetScreenHeight();
#endif
  SetTargetFPS(60);

  player.image = LoadImage(PlayerImage);
  player.texture = LoadTextureFromImage(player.image);

  player.position.x = ScreenWidth / 2 - player.texture.width / 2;
  player.position.y = ScreenHeight - player.texture.height - 30;
  player.color = WHITE;
}

void UnloadAssets() {
  UnloadImage(player.image);
  UnloadTexture(player.texture);
}

void CheckKeyBindingEvents() {
  if (IsKeyPressed(KEY_P))
    Pause = true;

  if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) &&
      IsKeyPressed(KEY_Q)) {
    UnloadAssets();
    CloseWindow();
  }

  if (IsKeyPressed(KEY_ENTER) && !GameStarted) {
    GameStarted = true;
    GameOver = false;
  }
}

void PauseLock() {
  while (Pause) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Paused - Press P to Resume", 190, 200, 20, LIGHTGRAY);
    EndDrawing();

    if (IsKeyPressed(KEY_P))
      Pause = false;
  }
}

static void UpdateGame() {
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
    if (player.position.x + player.texture.width < ScreenWidth)
      player.position.x += speed;
  }

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
    if (player.position.x > 0)
      player.position.x -= speed;
  }

  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
    if (player.position.y + player.texture.height < ScreenHeight)
      player.position.y += speed;
  }

  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
    if (player.position.y > 0)
      player.position.y -= speed;
  }
}

static void DrawGameFrame() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawTexture(player.texture, player.position.x, player.position.y,
              player.color);
  EndDrawing();
}

static void GameOverCanvas() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("Game Over", 190, 200, 20, LIGHTGRAY);
  EndDrawing();
}

static void GameStartCanvas() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("Press [Enter] to Start the Game", 190, 200, 20, LIGHTGRAY);
  EndDrawing();
}

void UpdateGameCanvas() {
  if (GameOver)
    GameOverCanvas();
  else if (!GameStarted)
    GameStartCanvas();
  else {
    UpdateGame();
    DrawGameFrame();
  }
}
