#ifndef BOT_CARD_H
#define BOT_CARD_H

#include <libchaos/ini.h>

#include "bot.h"

/* -------------------------------------------------------------------------- *
 * Constants                                                                  *
 * -------------------------------------------------------------------------- */
#define CARD_LAME 0
#define CARD_LEET 1

/* -------------------------------------------------------------------------- *
 * Types                                                                      *
 * -------------------------------------------------------------------------- */
struct card {
  struct node     node;          /* node for lclient_list */
  struct node     pnode;         /* node for player */
  struct node     gnode;         /* node for game */
  uint32_t        serial;
  uint32_t        refcount;      /* how many times this block is referenced */
  uint32_t        hash;
  int             choose;        /* Jokerkarte wenn 1, erlaube dem Spieler eine Karte zu w�hlen */
  char            name[3];
  int             points;
  int             value;
};

/* -------------------------------------------------------------------------- *
 * Global variables                                                           *
 * -------------------------------------------------------------------------- */
extern int             card_log;
extern struct sheap    card_heap;     /* heap for card_t */
extern uint32_t        card_serial;
extern struct list     card_list;     /* list with all of them */

/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 * Initialize card module                                                     *
 * -------------------------------------------------------------------------- */
extern void            card_init         (void);

/* -------------------------------------------------------------------------- *
 * Shutdown card module                                                       *
 * -------------------------------------------------------------------------- */
extern void            card_shutdown     (void);

/* -------------------------------------------------------------------------- *
 * Garbage collect card data                                                  *
 * -------------------------------------------------------------------------- */
extern void            card_collect      (void);

/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
extern struct card    *card_by_mode      (struct list  *list, 
                                          int           mode);

/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
extern struct card    *card_higher       (struct list  *list, 
                                          int           value);
  
/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
extern void            card_by_value     (struct list  *in,
                                          struct list  *out, 
                                          int           value,
                                          int           max);

  /* -------------------------------------------------------------------------- *
 * Load all card from INI file                                                *
 * -------------------------------------------------------------------------- */
extern void            card_load         (struct ini   *ini);

/* -------------------------------------------------------------------------- *
 * Create a new card block                                                    *
 * -------------------------------------------------------------------------- */
extern struct card    *card_new          (const char   *name);

/* -------------------------------------------------------------------------- *
 * Delete a  card block                                                       *
 * -------------------------------------------------------------------------- */
extern void            card_delete       (struct card  *cptr);

/* -------------------------------------------------------------------------- *
 * Loose all references of an card block                                      *
 * -------------------------------------------------------------------------- */
extern void            card_release      (struct card  *cptr);

/* -------------------------------------------------------------------------- *
 * Find a card by its name                                                    *
 * -------------------------------------------------------------------------- */
extern struct card    *card_find_name    (const char   *name);

/* -------------------------------------------------------------------------- *
 * Find a card by its id                                                      *
 * -------------------------------------------------------------------------- */
extern struct card    *card_find_id      (uint32_t      id);

/* -------------------------------------------------------------------------- *
 * Dump cards and card heap.                                                  *
 * -------------------------------------------------------------------------- */
extern void            card_dump         (struct card  *cptr);
  
#endif /* BOT_CARD_H */