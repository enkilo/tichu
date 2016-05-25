/* $Id: fan.h,v 1.52 2005/05/21 08:27:20 smoli Exp $
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

#ifndef FAN_H
#define FAN_H

/* -------------------------------------------------------------------------- *
 * Das FAN Modul ist f�r die Berechnung, das Bewegen und das Anzeigen des     *
 * Kartenf�chers zust�ndig                                                    *
 * -------------------------------------------------------------------------- */
#include "gfxutil.h"
#include "card.h"

/* -------------------------------------------------------------------------- *
 * Makros f�rs Loggen                                                         *
 * -------------------------------------------------------------------------- */
#define fan_log(l, s...) writelog(MOD_FAN, (l), s)

#if DEBUG_FAN
#define fan_debug(l, s...) fan_log((l), s)
#else
#define fan_debug(l, s...)
#endif

/* Initiale F�cher-Geometrie                                                  
 * -------------------------------------------------------------------------- */
#define FAN_ORIGIN_Y     (CLIENT_HEIGHT + 200) /* Y-Ursprung des F�cherbogens */
#define FAN_ORIGIN_X     (CLIENT_WIDTH / 2)    /* X-Ursprung des F�cherbogens */

#define FAN_IRADIUS      (CLIENT_HEIGHT / 1.50)/* Radius des F�cherbogens */
#define FAN_ORADIUS      (CLIENT_HEIGHT / 1.8) /* Radius des angehobenen Bogens */
#define FAN_ANGLE        3.2                   /* Winkel zwischen 2 Karten in � */

#define FAN_RIGHT        0                     /* Rechtsh�nder-F�cher */
#define FAN_LEFT         1                     /* Linksh�nder-F�cher */

/* F�cher-Physik
 * -------------------------------------------------------------------------- */
#if SLOW_FAN
#define FAN_MSPEED       25             /* Karten verschieben */
#define FAN_RSPEED       36             /* Geschwindigkeit f�rs Hervorheben */
#define FAN_LSPEED       6              /* absenken */
#else
#define FAN_MSPEED       250             /* Karten verschieben */
#define FAN_RSPEED       36              /* Geschwindigkeit f�rs Hervorheben */
#define FAN_LSPEED       6               /* absenken */
#endif

/* -------------------------------------------------------------------------- *
 * Konstanten                                                                 *
 * -------------------------------------------------------------------------- */
//#define FAN_STEPS        8                /* Frames pro Animation */

#define FAN_WAVE_WIDTH   4
#define FAN_WAVE_SAMPLES FAN_WAVE_WIDTH
#define FAN_WAVE_AMP     32


/* Status-Flags f�r den Stapel
 * -------------------------------------------------------------------------- */
#define FAN_HIDDEN       0x01          /* Kein Redraw */
#define FAN_LOCKED       0x02          /* Keine Events */
#define FAN_FROZEN       0x04          /* Kein Update */
#define FAN_DRAG         0x08          /* Drag & Drop */

#define FAN_DISABLED     (FAN_HIDDEN|FAN_LOCKED|FAN_FROZEN)

/* Redraw Flags f�r den Stapel
 * -------------------------------------------------------------------------- */
#define FAN_REDRAW       0x10          /* F�cher-Karten neu zeichnen */

/* -------------------------------------------------------------------------- *
 * Makros                                                                     *
 * -------------------------------------------------------------------------- */
#define fan_dragging() (fan_status & FAN_DRAG)

/* -------------------------------------------------------------------------- *
 * Konfiguration f�r den F�cher                                               *
 * -------------------------------------------------------------------------- */
struct fan_config
{
  int16_t  x;        /* Zentrum des Kreisbogens */
  int16_t  y;        /* 0� Punkt auf dem Bogen (Also Zentrum - Radius) */
  uint16_t raise;    /* Ausgew�hlte Karte um 'raise' Pixel anheben */
  uint16_t spacing;  /* Pixelabstand zwischen den Karten */
  float    bend;     /* Kr�mmungsquotient des F�chers */
  int      mirror;   /* F�cher spiegeln f�r Linksh�nder */
};
    
extern struct fan_config fan_config;


/* -------------------------------------------------------------------------- *
 * Modulstatus                                                                *
 * -------------------------------------------------------------------------- */
extern struct list  fan_list;              /* Liste aller Karten im F�cher */
extern struct card *fan_card;              /* Gew�hlte Karte */
extern struct card *fan_previous;          /* Letzte gew�hlte Karte */
extern struct color fan_tint;              /* Farbt�nung */
extern int          fan_status;
extern int          fan_target;

/* -------------------------------------------------------------------------- *
 * Geometrie                                                                  *
 * -------------------------------------------------------------------------- */
extern uint32_t        fan_radius[2];         /* Innerer und �usserer Radius */
extern float           fan_angle;             /* Winkel zwischen zwei Karten */
extern struct position fan_center;            /* Zentrum des Kreisbogens */

/* -------------------------------------------------------------------------- *
 * Initialisiert den F�cher                                                   *
 * -------------------------------------------------------------------------- */
void         fan_init           (void);

/* -------------------------------------------------------------------------- *
 * Gibt die F�cher-Ressourcen wieder frei                                     *
 * -------------------------------------------------------------------------- */
void         fan_shutdown       (void);

/* -------------------------------------------------------------------------- *
 * Liest die Konfiguration des F�chers aus der [fan] Sektion der angegebenen  *
 * ini-Datei                                                                  *
 * -------------------------------------------------------------------------- */
void         fan_configure      (struct ini  *ini);
  
/* -------------------------------------------------------------------------- *
 * Berechnet die F�chergeometrie                                              *
 * -------------------------------------------------------------------------- */
void         fan_geometry       (void);

/* -------------------------------------------------------------------------- *
 * Rendert eine Vorschau des F�chers auf eine Surface                         *
 * -------------------------------------------------------------------------- */
SDL_Surface *fan_preview        (SDL_Surface *bgnd,
                                 SDL_Rect     rect,
                                 struct list *cards);

/* -------------------------------------------------------------------------- *
 * Setzt F�cher-Statusflags                                                   *
 * -------------------------------------------------------------------------- */
int          fan_set            (int          flags);

/* -------------------------------------------------------------------------- *
 * L�scht F�cher-Statusflags                                                  *
 * -------------------------------------------------------------------------- */
int          fan_unset          (int          flags);
  
/* -------------------------------------------------------------------------- *
 * F�gt eine Karte hinzu                                                      *
 * -------------------------------------------------------------------------- */
void         fan_add            (struct card *card);

/* -------------------------------------------------------------------------- *
 * Nimmt eine Karte weg                                                       *
 * -------------------------------------------------------------------------- */
void         fan_remove         (struct card *card);

/* -------------------------------------------------------------------------- *
 * Startet mit dem Anzeigen des F�chers, nachdem die initialen Karten hinzu-  *
 * gef�gt wurden                                                              *
 * -------------------------------------------------------------------------- */
void         fan_start          (void);  

/* -------------------------------------------------------------------------- *
 * Gibt die Karte zur�ck auf welche x/y zeigt                                 *
 * -------------------------------------------------------------------------- */
struct card *fan_at             (int          x, 
                                 int          y);

/* -------------------------------------------------------------------------- *
 * Kalkuliert die Positionen und Winkel aller Karten und rotiert die Bilder   *
 * dementsprechend                                                            *
 * -------------------------------------------------------------------------- */
void         fan_calc           (void);

/* -------------------------------------------------------------------------- *
 * Ausgew�hlte Karten z�hlen                                                  *
 * -------------------------------------------------------------------------- */
int          fan_selected       (void);

/* -------------------------------------------------------------------------- *
 * Alle Karten abw�hlen                                                       *
 * -------------------------------------------------------------------------- */
int          fan_deselect       (void);

/* -------------------------------------------------------------------------- *
 * F�cher zeichnen                                                            *
 * -------------------------------------------------------------------------- */
void         fan_redraw         (SDL_Surface *surface);

/* -------------------------------------------------------------------------- *
 * Checkt ob eine Karte an der aktuellen Position ist und Hilited diese ge-   *
 * gegebenenfalls.                                                            *
 * -------------------------------------------------------------------------- */
int          fan_hilite         (int          x, 
                                 int          y);

/* -------------------------------------------------------------------------- *
 * Behandelt Mausbewegungen                                                   *
 * -------------------------------------------------------------------------- */
int          fan_motion         (uint8_t      type,
                                 uint8_t      state,
                                 int16_t      x,
                                 int16_t      y,
                                 int16_t      xrel,
                                 int16_t      yrel);

/* -------------------------------------------------------------------------- *
 * Behandelt Mausgeklicke                                                     *
 * -------------------------------------------------------------------------- */
int          fan_button         (uint8_t      type,
                                 uint8_t      button,
                                 uint8_t      state,
                                 int16_t      x,
                                 int16_t      y);

/* -------------------------------------------------------------------------- *
 * Behandelt Events die den F�cher betreffen                                  *
 * -------------------------------------------------------------------------- */
int          fan_event          (SDL_Event   *event);

/* -------------------------------------------------------------------------- *
 * Updatet Kartenpositionen und so                                            *
 * -------------------------------------------------------------------------- */
int          fan_update         (uint32_t     t);

/* -------------------------------------------------------------------------- *
 * Sortiert den F�cher                                                        *
 * -------------------------------------------------------------------------- */
void         fan_sort           (void);

#endif /* FAN_H */
