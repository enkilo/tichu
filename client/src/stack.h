/* $Id: stack.h,v 1.32 2005/05/21 08:27:20 smoli Exp $
 * -------------------------------------------------------------------------- *
 *  .___.    .                                                                *
 *    |  * _.|_ . .        Portabler, SDL-basierender Client f�r das          *
 *    |  |(_.[ )(_|             Multiplayer-Kartenspiel Tichu.                *
 *  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . *
 *                                                                            *
 *               (c) 2004-2005 by Martin Zangger, Roman Senn                  *
 *                                                                            *
 *    Dieses Programm ist freie Software. Sie k�nnen es unter den Bedingungen *
 * der GNU General Public License, wie von der Free Software Foundation ver-  *
 * �ffentlicht, weitergeben und/oder modifizieren, entweder gem�ss Version 2  *
 * der Lizenz oder (nach Ihrer Option) jeder sp�teren Version.                *
 *                                                                            *
 *    Die Ver�ffentlichung dieses Programms erfolgt in der Hoffnung, dass es  *
 * Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, sogar ohne die  *
 * implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT F�R EINEN BE-    *
 * STIMMTEN ZWECK. Details finden Sie in der GNU General Public License.      *
 *                                                                            *
 *    Sie sollten eine Kopie der GNU General Public License zusammen mit      *
 * diesem Programm erhalten haben. Falls nicht, schreiben Sie an die Free     *
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA          *
 * 02111-1307, USA.                                                           *
 * -------------------------------------------------------------------------- */

#ifndef STACK_H
#define STACK_H

/* -------------------------------------------------------------------------- *
 * Das STACK Modul zeigt den Kartenstapel an und nimmt die gespielten Karten  *
 * entgegen                                                                   *
 * -------------------------------------------------------------------------- */
#include "gfxutil.h"
#include "card.h"
#include "player.h"

/* -------------------------------------------------------------------------- *
 * Makros f�rs Loggen                                                         *
 * -------------------------------------------------------------------------- */
#define stack_log(l, s...) writelog(MOD_STACK, (l), s)

#if DEBUG_STK
#define stack_debug(l, s...) stack_log((l), s)
#else
#define stack_debug(l, s...)
#endif

/* Status Flags f�r den Stapel
 * -------------------------------------------------------------------------- */
#define STACK_HIDDEN    0x01   /* Kein Redraw */
#define STACK_LOCKED    0x02   /* Keine Events */
#define STACK_FROZEN    0x04   /* Kein Update */
#define STACK_DRAG      0x08

#define STACK_DISABLED  (STACK_LOCKED|STACK_HIDDEN|STACK_FROZEN)

/* Redraw Flags f�r den Stapel
 * -------------------------------------------------------------------------- */
#define STACK_MOVE      0x10   /* Position hat ge�ndert */
#define STACK_RENDER    0x20   /* Look (Farbe, Alpha) hat ge�ndert */
#define STACK_TRANSFORM 0x40   /* Winkel oder Zoom hat ge�ndert */

#define STACK_REDRAW    (STACK_MOVE|STACK_RENDER|STACK_TRANSFORM)

/* Geometrie & Physik f�r Stapel 
 * -------------------------------------------------------------------------- */
#define STACK_ANGLE     16     /* Winkel zwischen 2 Karten in � */
#define STACK_SPEED     90.0   /* Rotationsgeschwindigkeit in �/s */

#define STACK_IN        0      /* Innere Position */ 
#define STACK_OUT       1      /* �ussere Position */

/* -------------------------------------------------------------------------- *
 * Globale Variabeln                                                          *
 * -------------------------------------------------------------------------- */
extern struct position stack_pos;
extern SDL_Rect        stack_rect;
extern int             stack_status;

/* -------------------------------------------------------------------------- *
 * Initialisiert den Kartenstapel                                             *
 * -------------------------------------------------------------------------- */
void         stack_init      (void);

/* -------------------------------------------------------------------------- *
 * Gibt die Stapel-Ressourcen wieder frei                                     *
 * -------------------------------------------------------------------------- */
void         stack_shutdown  (void);

/* -------------------------------------------------------------------------- *
 * Setzt Stapel-Statusflags                                                   *
 * -------------------------------------------------------------------------- */
int          stack_set       (int            flags);
  
/* -------------------------------------------------------------------------- *
 * L�scht F�cher-Statusflags                                                  *
 * -------------------------------------------------------------------------- */
int          stack_unset     (int            flags);    

/* -------------------------------------------------------------------------- *
 * Setzt den Spieler der an der Reihe ist                                     *
 * -------------------------------------------------------------------------- */
void         stack_actor     (struct player *player);

/* -------------------------------------------------------------------------- *
 * Eine Karte auf den Stapel legen (vielleicht nur tempor�r)                  *
 * -------------------------------------------------------------------------- */
void         stack_add       (struct card   *card,
                              struct player *player);
  
/* -------------------------------------------------------------------------- *
 * Karte wieder vom Stapel nehmen                                             *
 * -------------------------------------------------------------------------- */
void         stack_remove    (struct card   *card);

/* -------------------------------------------------------------------------- *
 * Setzt die Stapelfarbe                                                      *
 * -------------------------------------------------------------------------- */
void         stack_setcolor  (struct color   color);

/* -------------------------------------------------------------------------- *
 * �berblendet die Stapelfarbe                                                *
 * -------------------------------------------------------------------------- */
void         stack_blend     (struct color   color);

/* -------------------------------------------------------------------------- *
 * Setzt den Winkel des Stapels                                               *
 * -------------------------------------------------------------------------- */
void         stack_setangle  (int            a);

/* -------------------------------------------------------------------------- *
 * Setzt den Zoomfaktor des Stapels                                           *
 * -------------------------------------------------------------------------- */
void         stack_setzoom   (float          z);

/* -------------------------------------------------------------------------- *
 * Rotiert den Stapel an die angegebene Position mit einer Geschwindigkeit    *
 * von v� pro Sekunde                                                         *
 * -------------------------------------------------------------------------- */
void         stack_rotate    (int            a,
                              uint32_t       v);

/* -------------------------------------------------------------------------- *
 * Zoomt den Stapel w�hrend einer Rotation                                    *
 * -------------------------------------------------------------------------- */
void         stack_zoom      (float          z);

/* -------------------------------------------------------------------------- *
 * Kalkuliert die Positionen und Winkel aller Karten und rotiert die Bilder   *
 * dementsprechend                                                            *
 *                                                                            *
 * 'center' gibt den Startwinkel an, 0� f�r den lokalen Spieler, 90� f�r den  *
 * n�chsten Spieler                                                           *
 * -------------------------------------------------------------------------- */
void         stack_calc      (struct player *player,
                              uint32_t       speed);

/* -------------------------------------------------------------------------- *
 * Findet per Rechteck-�berlappung und Pixel-Test heraus ob sich an der       *
 * angegebenen Position tats�chlich der Stapel befindet                       *
 * -------------------------------------------------------------------------- */
int          stack_isat      (int            x, 
                              int            y);

/* -------------------------------------------------------------------------- *
 * Blittet das rotierte und gezoomte Stack-Surface nach stack_sf und f�rbt    *
 * es dabei ein                                                               *
 * -------------------------------------------------------------------------- */
void         stack_render    (void);

/* -------------------------------------------------------------------------- *
 * Rotiert und zoomt den Stack nach den aktuellen Werten                      *
 * -------------------------------------------------------------------------- */
void         stack_transform (void);

/* -------------------------------------------------------------------------- *
 * updatet die werte f�r zoom, rotation und farbe des kartenstapels f�r die   *
 * vergangene zeit <diff> (millisekunden)                                     *
 * -------------------------------------------------------------------------- */
int          stack_update    (uint32_t t);

/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
void         stack_cardpos   (struct card   *card, 
                              double         angle);

/* -------------------------------------------------------------------------- *
 * Karten provisorisch auf dem Stapel anzeigen                                *
 * (vor dem Ablegen oder Zur�ckweisen)                                        *
 * -------------------------------------------------------------------------- */
void         stack_float     (void);

/* -------------------------------------------------------------------------- *
 * Legt Karten auf dem Stapel ab                                              *
 * -------------------------------------------------------------------------- */
void         stack_drop      (void);
  
/* -------------------------------------------------------------------------- *
 * Weist Karten zur�ck                                                        *
 * -------------------------------------------------------------------------- */
void         stack_refuse    (void);

/* -------------------------------------------------------------------------- *
 * Nimmt die Karten definitiv entgegen und zeichnet sie auf das Surface f�r   *
 * die abgelegten Karten                                                      *
 * -------------------------------------------------------------------------- */
void         stack_accept    (void);
  
/* -------------------------------------------------------------------------- *
 * Schiebt Karten aus Position von anderem Spieler richtung Stapel            *
 *                                                                            *
 * Winkel wird aus player_index und player_count berechnet,                   *
 * player_index 0 ist der lokale Spieler                                      *
 * -------------------------------------------------------------------------- */
void         stack_appear    (struct list   *cards,
                              struct player *player,
                              int            radius);

/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
void         stack_set_actor (struct player *player);

/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
int          stack_motion    (uint8_t        type,
                              uint8_t        state,
                              uint16_t       x, 
                              uint16_t       y,
                              int16_t        xrel,
                              int16_t        yrel);

/* -------------------------------------------------------------------------- *
 * Behandelt Mausgeklicke                                                     *
 * -------------------------------------------------------------------------- */
int          stack_button    (uint8_t        type,
                              uint8_t        button,
                              uint8_t        state, 
                              int16_t        x, 
                              int16_t        y);  

/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
int          stack_event     (SDL_Event     *event);
  
/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
void         stack_redraw    (SDL_Surface   *surface);

/* -------------------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
void         stack_clear     (void);

/* -------------------------------------------------------------------------- *
 * Stapeldaten ausgeben                                                       *
 * -------------------------------------------------------------------------- */
#ifdef DEBUG
void         stack_dump      (void);
#endif /* DEBUG */

#endif /* STACK_H */
