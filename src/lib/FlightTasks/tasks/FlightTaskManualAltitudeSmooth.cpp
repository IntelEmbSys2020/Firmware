/****************************************************************************
 *
 *   Copyright (c) 2017 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file FlightManualAltitude.cpp
 */

#include "FlightTaskManualAltitudeSmooth.hpp"
#include <mathlib/mathlib.h>
#include <float.h>

using namespace matrix;

FlightTaskManualAltitudeSmooth::FlightTaskManualAltitudeSmooth(control::SuperBlock *parent, const char *name) :
	FlightTaskManualAltitude(parent, name),
	_smoothing(_velocity(2), _sticks(2))
{}


bool FlightTaskManualAltitudeSmooth::activate()
{
	_vel_sp_prev_z = _velocity(2);
	return FlightTaskManualStabilized::activate();
}

void FlightTaskManualAltitudeSmooth::_updateSetpoints()
{
	/* Get yaw, thrust */
	FlightTaskManualStabilized::_updateSetpoints();

	/* Smooth velocity setpoint */
	float vel_sp[2] = {_vel_sp_z, _vel_sp_prev_z};
	_smoothing.smoothVelFromSticks(vel_sp, _deltatime);
	_vel_sp_z = vel_sp[0];

	/* Check for altitude lock*/
	_updateAltitudeLock();

	/* Update previous velocity setpoint for next smoothing iteration */
	_vel_sp_prev_z = _vel_sp_z;
}
