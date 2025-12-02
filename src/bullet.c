#include "bullet.h"
#include <stdlib.h>

BulletList *CreateBulletList() {
  BulletList *head = (BulletList *)malloc(sizeof(BulletList));
  head->bullet = (Bullet){0};
  head->next = NULL;
  return head;
}

void InsertBulletList(BulletList *baseptr, Bullet bullet) {
  while (baseptr->next != NULL) {
    baseptr = baseptr->next;
  }

  BulletList *b = (BulletList *)malloc(sizeof(BulletList));
  b->bullet = bullet;
  b->next = NULL;
  baseptr->next = b;
}

void DeleteBulletIf(BulletList *baseptr, bool (*fn)(BulletList *)) {
  BulletList *curr = baseptr;
  while (curr->next != NULL) {
    if (fn(curr->next)) {
      BulletList *toDelete = curr->next;
      curr->next = toDelete->next;
      free(toDelete);
    } else {
      curr = curr->next;
    }
  }
}

void ForEachBullet(BulletList *baseptr, void (*fn)(BulletList *)) {
  while (baseptr != NULL) {
    fn(baseptr);
    baseptr = baseptr->next;
  };
}

void UnAllocateBullets(BulletList *baseptr) {
  BulletList *temp;
  while (baseptr->next != NULL) {
    temp = baseptr->next;
    free(baseptr);
    baseptr = temp;
  }
}
