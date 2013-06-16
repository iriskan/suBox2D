/*
 * Copyright (c) 2010—2013, David Wicks
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include <Box2D/Box2D.h>
#include "cinder/app/App.h"
#include "cinder/Vector.h"

namespace sansumbrella
{
  class Sandbox
  {
  public:
    Sandbox() = default;
    ~Sandbox() = default;
    // run the physics timestep
    void step();
    // remove all bodies
    void clear();

    // have a look at what's in the physics system (scaled up to screen space)
    void debugDraw( bool drawBodies=true, bool drawContacts=true );

    // initialize the box2d world, optionally create boundaries at edges of screen
    void init( bool useScreenBounds=true );
    // create custom boundary
    b2Body* createBoundaryRect( ci::Rectf screen_bounds, float thickness=1.0f );

    // add a BoxElement
    b2Body* createBox( ci::Vec2f pos, ci::Vec2f size );
    b2Body* createBody( const b2BodyDef &body_def, const std::vector<b2FixtureDef> &fixture_defs );
    b2Body* createBody( const b2BodyDef &body_def, const b2FixtureDef &fixture_def );
    inline void   destroyBody( b2Body *body ){ mWorld.DestroyBody( body ); }
    inline void   destroyJoint( b2Joint *joint ){ mWorld.DestroyJoint( joint ); }

    // wrappers for some b2world functions
    int32 getBodyCount(){ return mWorld.GetBodyCount(); }
    int32 getContactCount(){ return mWorld.GetContactCount(); }
    b2Body* getBodyList(){ return mWorld.GetBodyList(); }

    // some useful settings
    void setGravity( ci::Vec2f gravity );
    void setVelocityIterations( int vi ){ mVelocityIterations = vi; }
    void setPositionIterations( int pi ){ mPositionIterations = pi; }
    void setTimeStep( float hz ){ mTimeStep = hz; }

    // get the world
    b2World& getWorld(){ return mWorld; }

    // set the filter function for your objects
    void setContactFilter( b2ContactFilter filter );

    // enable user interaction (needs to know what window the interaction is coming from)
    void connectUserSignals( ci::app::WindowRef window );
    void disconnectUserSignals();

    // handle user interaction
    bool mouseDown( ci::app::MouseEvent &event );
    bool mouseUp( ci::app::MouseEvent &event );
    bool mouseDrag( ci::app::MouseEvent &event );

    //! how many points (dpi-agnostic pixels) are there in a meter?
    //! default value is 50 (5 pixels == 1 decimeter)
    void setPointsPerMeter( float points );
    //! Conversion from screen units to physical measurements
    template<typename T>
    inline T toPhysics( const T &points )
    {
      return points * mMetersPerPoint;
    }
    //! Conversion from physical measurements to screen units
    template<typename T>
    inline T toPoints( const T &physical_measure )
    {
      return physical_measure * mPointsPerMeter;
    }
  private:
    int mVelocityIterations = 8;
    int mPositionIterations = 3;
    float mTimeStep = 1.0f / 60.0f;
    float mPointsPerMeter = 50.0f;
    float mMetersPerPoint = 1.0f / mPointsPerMeter;
    // the box2d world
    b2World         mWorld = b2World( b2Vec2( 0, 10.0f ) );
    // optional contact filter (kept here to ensure it stays in scope)
    // set this if you want to control what collides with what
    b2ContactFilter mContactFilter;

    // storing
    float           mBoundaryDepth = 5.0f;
    // our mouse, for simple interaction
    b2MouseJoint*   mMouseJoint = nullptr;
    b2Body*         mBoundaryBody = nullptr;
  };
}