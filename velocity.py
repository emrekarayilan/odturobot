#!/usr/bin/env python3
import math
import asyncio
import time
import mavsdk.offboard
from mavsdk import System
from mavsdk.offboard import (OffboardError, PositionNedYaw, VelocityNedYaw)

import asyncio

from mavsdk import System
from mavsdk.offboard import (OffboardError, VelocityBodyYawspeed)


async def run():
    """ Does Offboard control using velocity body coordinates. """

    drone = System()
    await drone.connect(system_address="udp://:14540")

    print("Waiting for drone to connect...")
    async for state in drone.core.connection_state():
        if state.is_connected:
            print(f"Drone discovered!")
            break

    print("-- Arming")
    await drone.action.arm()

    print("Taking off")
    await drone.action.takeoff()
    await asyncio.sleep(10)

    print("-- Setting initial BODY setpoint")
    await drone.offboard.set_velocity_body(
        VelocityBodyYawspeed(0.0, 0.0, 0.0, 0.0))

    print("-- Setting initial NED setpoint")
    await drone.offboard.set_velocity_ned(VelocityNedYaw(0.0, 0.0, 0.0, 0.0))

    print("-- Setting initial Position setpoint")
    await drone.offboard.set_position_ned(PositionNedYaw(0, 0, 0, 0))

    print("-- Starting offboard")
    try:
        await drone.offboard.start()
    except OffboardError as error:
        print(f"Starting offboard mode failed with error code: \
              {error._result.result}")
        print("-- Disarming")
        await drone.action.disarm()
        return

    print("Turning North")

    await drone.offboard.set_position_ned(PositionNedYaw(0, 0, -2.5, 0))
    await asyncio.sleep(5)

    await drone.offboard.set_velocity_body(VelocityBodyYawspeed(0, 0, 0, 30))
    await asyncio.sleep(2)

    velocity_vector_angle = 120

    yaw_change_rate = 0

    await drone.offboard.set_velocity_body(
        VelocityBodyYawspeed(math.sin(velocity_vector_angle), math.cos(velocity_vector_angle), 0, yaw_change_rate))

    await asyncio.sleep(0.1)
    t = 0
    x = 0

    degisen_yaw = 0

    for e in range(0,3):
        for j in range(0, 3):

            velocity_vector_angle = 120
            degisen_yaw = 0
            await drone.offboard.set_velocity_body(VelocityBodyYawspeed(0,0,0,0))
            await asyncio.sleep(1)
            for i in range(0, 500):

                if (x < 38.21):

                    velocity_vector_magnitude = math.sqrt(pow(math.sin(math.radians(velocity_vector_angle)), 2) + pow(
                        math.cos(math.radians(velocity_vector_angle)), 2))
                    print(f"Velocity Vector Magnitude ", velocity_vector_magnitude)

                    yaw_change_rate = ((13.074) * (velocity_vector_magnitude * t))

                    velocity_vector_angle = velocity_vector_angle + (yaw_change_rate / 100)
                    print("velocity vector angle: ", velocity_vector_angle, velocity_vector_magnitude*math.sin(math.radians(velocity_vector_angle)),
                          velocity_vector_magnitude*math.cos(math.radians(velocity_vector_angle)), 0, "Yaw change rate:", yaw_change_rate,
                          degisen_yaw)
                    await drone.offboard.set_velocity_body(
                        VelocityBodyYawspeed(velocity_vector_magnitude*math.sin(math.radians(velocity_vector_angle)),
                                             velocity_vector_magnitude*math.cos(math.radians(velocity_vector_angle)), 0, yaw_change_rate))

                    await asyncio.sleep(0.01)
                    i += 1
                    t += 0.01

                    x = yaw_change_rate
                    degisen_yaw += yaw_change_rate / 100

                else:

                    t = 0.01
                    velocity_vector_magnitude = math.sqrt(
                        pow(math.sin(math.radians(velocity_vector_angle)), 2) + pow(
                            math.cos(math.radians(velocity_vector_angle)), 2))

                    print(f"Velocity Vector Magnitude ", velocity_vector_magnitude)

                    yaw_change_rate = yaw_change_rate - ((13.074) * (velocity_vector_magnitude * t))

                    velocity_vector_angle = velocity_vector_angle + (yaw_change_rate / 100)
                    print("velocity vector angle: ", velocity_vector_angle, velocity_vector_magnitude*math.sin(math.radians(velocity_vector_angle)),
                          velocity_vector_magnitude*math.cos(math.radians(velocity_vector_angle)), 0, "Yaw change rate:", yaw_change_rate,
                          degisen_yaw)
                    await drone.offboard.set_velocity_body(
                        VelocityBodyYawspeed(velocity_vector_magnitude*math.sin(math.radians(velocity_vector_angle)),
                                             velocity_vector_magnitude*math.cos(math.radians(velocity_vector_angle)), 0,
                                             yaw_change_rate))

                    await asyncio.sleep(0.01)
                    i += 1
                    t += 0.01

                    degisen_yaw += yaw_change_rate / 100

                    if (yaw_change_rate < 14):
                        x = 14

                    j = +1
        await asyncio.sleep(1)
        e +=1

    await drone.offboard.set_velocity_body(
        VelocityBodyYawspeed(0.0, 0.0, 0.0, 0.0))
    await asyncio.sleep(2)

    print("Mission Successfull \n -- Stopping offboard")
    try:
        await drone.offboard.stop()
    except OffboardError as error:
        print(f"Stopping offboard mode failed with error code: {error._result.result}")

    print("returning to launch")
    await drone.action.return_to_launch()
    await asyncio.sleep(10)
    print("Mission Is Successfull.Bravo!")


if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run())
