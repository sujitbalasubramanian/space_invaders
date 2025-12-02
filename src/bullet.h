#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

typedef struct {
  Rectangle rec;
  Color color;
  float speed;
  int direction;
} Bullet;

typedef struct BulletNode {
  Bullet bullet;
  struct BulletNode *next;
} BulletList;

BulletList *CreateBulletList(void);
void InsertBulletList(BulletList *, Bullet);
void DeleteBulletIf(BulletList *baseptr, bool (*fn)(BulletList *));
void ForEachBullet(BulletList *, void (*fn)(BulletList *));
void UnAllocateBullets(BulletList *baseptr);

#endif // !BULLET_H
