//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006-2015 Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
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

#ifndef HEADER_PHYSICAL_OBJECT_HPP
#define HEADER_PHYSICAL_OBJECT_HPP

#include <string>

#include "btBulletDynamicsCommon.h"

#include "physics/user_pointer.hpp"
#include "utils/vec3.hpp"
#include "utils/leak_check.hpp"


class Material;
class TrackObject;
class XMLNode;

/**
  * \ingroup physics
  */
class PhysicalObject
{
public:
    /** The supported collision shapes. */
    enum BodyTypes {MP_NONE,
                    MP_CONE_Y, MP_CONE_X, MP_CONE_Z,
                    MP_CYLINDER_Y, MP_CYLINDER_X, MP_CYLINDER_Z,
                    MP_BOX, MP_SPHERE, MP_EXACT};

    class Settings
    {
    public:
        /** ID of the object. */
        std::string               m_id;
        /** Mass of the object. */
        float                     m_mass;
        /** Radius of the object. */
        float                     m_radius;
        /** Shape of the object. */
        PhysicalObject::BodyTypes m_body_type;
        /** Trigger a reset in karts touching it? */
        bool                      m_crash_reset;
        /** Knock the kart around. */
        bool                      m_knock_kart;
        /** Flatten the kart when this object is touched. */
        bool                      m_flatten_kart;
        /** Reset the object when it falls under the track (useful
         *  e.g. for a boulder rolling down a hill). */
        bool                      m_reset_when_too_low;
        /** If the item is below that height, it is reset (when
         *  m_reset_when_too_low is true). */
        float                     m_reset_height;
    private:
        void init();
    public:
        Settings(BodyTypes type, float radius, float mass);
        Settings(const XMLNode &xml_node);
    };   // Settings

private:

    /** The initial XYZ position of the object. */
    core::vector3df       m_init_xyz;

    /** The initial hpr of the object. */
    core::vector3df       m_init_hpr;

    /** The initial scale of the object. */
    core::vector3df       m_init_scale;

    TrackObject          *m_object;

    /** The shape of this object. */
    BodyTypes             m_body_type;

    /** The bullet collision shape. */
    btCollisionShape     *m_shape;

    /** ID of the object. */
    std::string           m_id;

    /** The corresponding bullet rigid body. */
    btRigidBody          *m_body;

    /** Bullet's motion state for this object. */
    btDefaultMotionState *m_motion_state;

    /** The mass of this object. */
    float                 m_mass;

    /** The pointer that is stored in the bullet rigid body back to
     *  this object. */
    UserPointer           m_user_pointer;

    /** This is the initial position of the object for the physics. */
    btTransform           m_init_pos;

    /** The mesh might not have the same center as bullet does. This
     *  offset is used to offset the location of the graphical mesh
     *  so that the graphics are aligned with the bullet collision shape. */
    Vec3                  m_graphical_offset;

    /** Radius of the object - this obviously depends on the actual shape.
     *  As a default the radius is being determined from the shape of the
     *  mesh, but in somce cases that could lead to incorrect results
     *  (if the mesh does not closely resemble a sphere, see init() for
     *  details, but is supposed to be a sphere). In this case the radius
     *  can be set in the scene file. */
    float                 m_radius;

    /** True if a kart colliding with this object should be rescued. */
    bool                  m_crash_reset;

    /** True if kart should "explode" when touching this */
    bool                  m_explode_kart;

    bool                  m_flatten_kart;

    /** True if object should be reset to its initial position if it's
     *  too low (see m_reset_height). */
    bool                  m_reset_when_too_low;

    /** If m_reset_when_too_low this object is set back to its start
     *  position if its height is below this value. */
    float                 m_reset_height;

    /** If this body is a bullet dynamic body, i.e. affected by physics
     *  or not (static (not moving) or kinematic (animated outside
     *  of physics). */
    bool                  m_is_dynamic;

    /** Non-null only if the shape is exact */
    TriangleMesh         *m_triangle_mesh;

public:
                    PhysicalObject(bool is_dynamic, const Settings& settings,
                                   TrackObject* object);

    static PhysicalObject* fromXML(bool is_dynamic, const XMLNode &node,
                                   TrackObject* object);

    virtual     ~PhysicalObject ();
    virtual void reset          ();
    virtual void handleExplosion(const Vec3& pos, bool directHit);
    void         update         (float dt);
    void         init           ();
    void         move           (const Vec3& xyz, const core::vector3df& hpr);
    void         hit            (const Material *m, const Vec3 &normal);
    bool         isSoccerBall   () const;
    bool castRay(const btVector3 &from,
                 const btVector3 &to, btVector3 *hit_point,
                 const Material **material, btVector3 *normal,
                 bool interpolate_normal) const;

    // ------------------------------------------------------------------------
    /** Returns the ID of this physical object. */
    std::string getID()          { return m_id; }
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    /** Returns the rigid body of this physical object. */
    btRigidBody *getBody        ()          { return m_body; }
    // ------------------------------------------------------------------------
    /** Returns true if this object should trigger a rescue in a kart that
     *  hits it. */
    bool isCrashReset() const { return m_crash_reset; }
    // ------------------------------------------------------------------------
    /** Returns true if this object should cause an explosion if a kart hits
     *  it. */
    bool isExplodeKartObject () const { return m_explode_kart; }
    // ------------------------------------------------------------------------
    /** Returns true if this object should cause a kart that touches it to
     *  be flattened. */
    bool isFlattenKartObject () const { return m_flatten_kart; }
    // ------------------------------------------------------------------------
    /** Sets the interaction type */
    void setInteraction(std::string interaction);
    // ------------------------------------------------------------------------
    /** Remove body from dynamic world */
    void removeBody();
    // ------------------------------------------------------------------------
    /** Add body to dynamic world */
    void addBody();

    LEAK_CHECK()
};  // PhysicalObject

#endif
/* EOF */

