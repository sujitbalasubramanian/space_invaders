#include "game.h"
#include "bullet.h"
#include "raylib.h"

static int screenWidth = 0;
static int screenHeight = 0;
static const char *screenTitle = "Space Invaders";
static int screenPadding = 20;

static bool pause = false;
static bool gameStarted = false;
static bool gameOver = false;

static int fontSize = 26;

static long highScore = 0;
static long score = 0;

static const char *playerImagePath = "assets/player.png";
static const char *heartImagePath = "assets/heart.png";
static const char *enemysImagePath[] = {
    "assets/enemy3.png", "assets/enemy3.png", "assets/enemy2.png",
    "assets/enemy2.png", "assets/enemy1.png", "assets/enemy1.png",
};

static Image heartImage;
static Texture2D heartTexture;

static Ship player = {0};
static float playerSpeed = 4.0f;

#define ENEMEY_ROW 6
#define ENEMEY_COL 12
static Ship enemyShips[ENEMEY_ROW][ENEMEY_COL] = {0};
static float enemySpeed = .8f;
static int enemyDirection = 1;

static BulletList *enemyBullets;
static BulletList *playerBullets;

#define PLAYER_DEBOUNCE 0.15
static double lastShootFromPlayer = 0;

#define ENEMEY_DEBOUNCE 0.6
static double lastShootFromEnemy = 0;

void InitWave() {
  for (int i = 0; i < ENEMEY_ROW; i++) {
    for (int j = 0; j < ENEMEY_COL; j++) {
      enemyShips[i][j].image = LoadImage(enemysImagePath[i]);
      enemyShips[i][j].texture = LoadTextureFromImage(enemyShips[i][j].image);
      enemyShips[i][j].position.x = j == 0
                                        ? 200
                                        : enemyShips[i][j - 1].position.x +
                                              enemyShips[i][j].image.width + 20;
      enemyShips[i][j].position.y = i == 0 ? 150
                                           : enemyShips[i - 1][j].position.y +
                                                 enemyShips[i][j].image.height +
                                                 20;
      enemyShips[i][j].color = WHITE;
      enemyShips[i][j].life = 3 * (ENEMEY_ROW - i);
    }
  }
}

void InitGame() {
#if defined(__EMSCRIPTEN__)
  screenWidth = 1280;
  screenHeight = 720;
  InitWindow(screenWidth, screenHeight, screenTitle);
#else
  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(0, 0, screenTitle);
  screenWidth = GetScreenWidth();
  screenHeight = GetScreenHeight();
#endif

  SetTargetFPS(60);

  player.image = LoadImage(playerImagePath);
  player.texture = LoadTextureFromImage(player.image);
  player.position.x = (float)screenWidth / 2 - (float)player.texture.width / 2;
  player.position.y = screenHeight - player.texture.height - 30;
  player.color = WHITE;
  player.life = 3;

  heartImage = LoadImage(heartImagePath);
  heartTexture = LoadTextureFromImage(heartImage);

  enemyBullets = CreateBulletList();
  playerBullets = CreateBulletList();

  InitWave();
}

void EndGame() {
  UnloadImage(player.image);
  UnloadTexture(player.texture);

  for (int i = 0; i < ENEMEY_ROW; i++) {
    for (int j = 0; j < ENEMEY_COL; j++) {
      UnloadImage(enemyShips[i][j].image);
      UnloadTexture(enemyShips[i][j].texture);
    }
  }

  UnAllocateBullets(playerBullets);
  UnAllocateBullets(enemyBullets);
}

void CheckKeyBindingEvents() {
  if (IsKeyPressed(KEY_P))
    pause = !pause;

  if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) &&
      IsKeyPressed(KEY_Q)) {
    EndGame();
    CloseWindow();
#if defined(__EMSCRIPTEN__)
    emscripten_run_script("window.close();");
#endif
  }

  if (IsKeyPressed(KEY_ENTER) && !gameStarted) {
    gameStarted = true;
    gameOver = false;
  }
}

bool IsPaused() { return pause; }

void PauseCanvas() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("Paused - Press P to Resume", 190, 200, fontSize, LIGHTGRAY);
  EndDrawing();
}

void ShootPlayerBullet() {
  Bullet b = {.rec = {.height = 20,
                      .width = 10,
                      .x = player.position.x + (float)player.image.width / 2 -
                           (float)10 / 2,
                      .y = player.position.y + 5},
              .color = GREEN,
              .speed = 4,
              .direction = -1};
  InsertBulletList(playerBullets, b);
}

void ShootEnemyBullet() {
  int col = GetRandomValue(0, ENEMEY_COL - 1);
  int lastExistingRow = -1;

  for (int i = ENEMEY_ROW - 1; i >= 0; i--) {
    if (enemyShips[i][col].life > 0) {
      lastExistingRow = i;
      break;
    }
  }

  if (lastExistingRow == -1)
    return;

  Bullet b = {
      .rec = {.height = 20,
              .width = 10,
              .x = enemyShips[lastExistingRow][col].position.x +
                   (float)enemyShips[lastExistingRow][col].image.width / 2 - 5,
              .y = enemyShips[lastExistingRow][col].position.y +
                   enemyShips[lastExistingRow][col].image.height + 5},
      .color = RED,
      .speed = 4,
      .direction = 1};

  InsertBulletList(enemyBullets, b);
}

bool EnemyBulletCleanUpLogic(BulletList *b) {
  return b->bullet.rec.y > GetScreenHeight();
}

bool PlayerBulletCleanUpLogic(BulletList *b) { return b->bullet.rec.y <= 0; }

void RemoveOutOfSopeBullets() {
  DeleteBulletIf(enemyBullets, EnemyBulletCleanUpLogic);
  DeleteBulletIf(playerBullets, PlayerBulletCleanUpLogic);
}

void CheckCollision() {
  // TODO:
  // check player collides with alien
  // check alien reach end
  // check bullets collides and delete
  // increase score and high score
}

void UpdateBullet(BulletList *bptr) {
  bptr->bullet.rec.y += (bptr->bullet.speed * bptr->bullet.direction);
};

void CheckWaveEnd() {
  // TODO:
}

void UpdateGame() {
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
    if (player.position.x + player.texture.width < screenWidth)
      player.position.x += playerSpeed;
  }

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
    if (player.position.x > 0)
      player.position.x -= playerSpeed;
  }

  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
    if (player.position.y + player.texture.height < screenHeight)
      player.position.y += playerSpeed;
  }

  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
    if (player.position.y > 0)
      player.position.y -= playerSpeed;
  }

  ForEachBullet(playerBullets, UpdateBullet);

  if (IsKeyDown(KEY_SPACE) &&
      (GetTime() - lastShootFromPlayer) > PLAYER_DEBOUNCE) {
    ShootPlayerBullet();
    lastShootFromPlayer = GetTime();
  }

  if (enemyShips[ENEMEY_ROW - 1][ENEMEY_COL - 1].position.x >
          screenWidth - enemyShips[0][0].image.width - screenPadding ||
      enemyShips[0][0].position.x < screenPadding) {
    enemyDirection = enemyDirection == 1 ? -1 : 1;
    for (int i = 0; i < ENEMEY_ROW; i++) {
      for (int j = 0; j < ENEMEY_COL; j++) {
        enemyShips[i][j].position.y += 4;
      }
    }
  }

  ForEachBullet(enemyBullets, UpdateBullet);

  if (GetTime() - lastShootFromEnemy > ENEMEY_DEBOUNCE) {
    ShootEnemyBullet();
    lastShootFromEnemy = GetTime();
  }

  for (int i = 0; i < ENEMEY_ROW; i++) {
    for (int j = 0; j < ENEMEY_COL; j++) {
      enemyShips[i][j].position.x += (enemySpeed * enemyDirection);
    }
  }

  CheckCollision();
  RemoveOutOfSopeBullets();
  CheckWaveEnd();
}

void RenderBullet(BulletList *bptr) {
  DrawRectangleRec(bptr->bullet.rec, bptr->bullet.color);
};

void DrawGameCanvas() {
  BeginDrawing();
  ClearBackground(BLACK);

  DrawText(TextFormat("High Score: %ld", highScore), 10, 10, fontSize + 6, RED);
  DrawText(TextFormat("Score: %ld", score), 10, 50, fontSize + 6, RED);

  for (int i = 0; i < player.life; i++) {
    DrawTexture(heartTexture, screenWidth - ((heartImage.width) * (i + 1) + 10),
                10, WHITE);
  }

  for (int i = 0; i < ENEMEY_ROW; i++) {
    for (int j = 0; j < ENEMEY_COL; j++) {
      Ship enemy = enemyShips[i][j];
      if (enemy.life > 0) {
        DrawTexture(enemy.texture, enemy.position.x, enemy.position.y,
                    enemy.color);
      }
    }
  }

  ForEachBullet(playerBullets, RenderBullet);
  ForEachBullet(enemyBullets, RenderBullet);

  DrawTexture(player.texture, player.position.x, player.position.y,
              player.color);
  EndDrawing();
}

void GameStartCanvas() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("Press [Enter] to start the game", 190, 200, fontSize, LIGHTGRAY);
  EndDrawing();
}

void GameOverCanvas() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("Game Over! Press [Enter] to restart the game", 190, 200, fontSize,
           LIGHTGRAY);
  EndDrawing();
}

void UpdateGameCanvas() {
  if (gameOver)
    GameOverCanvas();
  else if (!gameStarted)
    GameStartCanvas();
  else {
    UpdateGame();
    DrawGameCanvas();
  }
}
