# Position Controller for Husky
Designed and implemented a position controller for husky assuming no natural interference, and an empty world.

## Overview
It is assumed that we can control its velocities (linear and angular) which fall under the controllable degrees of freedom. 
In case of husky, we can only direct its linear velocity along the x-direction (i.e. forward and backward), and angular velocity along the z-direction (i.e. turn).

## Approach
- Creating a new package inside an existing workspace named let's say husky_pos_vel.
- Creating a custom msg husky_pos_vel/Position which transmits ros header and x, y (positions on X-Y grid)
- Create a node prop_control.cpp which takes data from a topic /husky/reference_pos (a topic which you've to subscribe to for now, of msg type husky_pos_vel/Position). This node would also take current odometry of Husky through the topic mentioned above.
Now using knowledge of P Controllers and generate velocities, that should now be published on /husky_velocity_controller/cmd_vel.

