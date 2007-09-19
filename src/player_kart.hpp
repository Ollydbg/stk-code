//  $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2004-2005 Steve Baker <sjbaker1@airmail.net>
//  Copyright (C) 2006 Joerg Henrichs, Steve Baker
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


#ifndef HEADER_PLAYERKART_H
#define HEADER_PLAYERKART_H

#include "kart.hpp"
#include "player.hpp"

class Player;
class Camera;

/** PlayerKart manages control events from the player and moves
    them to the Kart */
class PlayerKart : public Kart
{
private:
    int m_steer_val, m_steer_val_l, m_steer_val_r, m_accel_val;

    Player *m_player;
    float   m_penalty_time;
    Camera *m_camera;

    void steer(float, int);
public:
    PlayerKart(const KartProperties *kart_properties,
               int position, Player *_player,
               sgCoord init_pos, Camera *cam) :
        Kart(kart_properties, position, init_pos), m_player(_player),
        m_penalty_time(0.0), m_camera(cam)       {reset(); }

    int     earlyStartPenalty () {return m_penalty_time>0; }
    Player* getPlayer         () {return m_player;        }
    void    update            (float);
    void    addMessages       ();
    void    action            (KartActions action, int value);
    void    forceCrash        ();
    void    handleZipper      ();
    void    collectedHerring  (Herring* herring);
    int     isPlayerKart      () const {return 1;}

    void    reset();
};

#endif

/* EOF */
