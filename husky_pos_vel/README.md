## Position Controller for Husky
Designed and implemented a position controller for husky assuming no natural interference, and an empty world.

### Overview
It is assumed that we can control its velocities (linear and angular) which fall under the controllable degrees of freedom. 
In case of husky, we can only direct its linear velocity along the x-direction (i.e. forward and backward), and angular velocity along the z-direction (i.e. turn).

### Approach
- Creating a new package inside an existing workspace named let's say ``husky_pos_vel``.
- Creating a custom msg ``husky_pos_vel/Position`` which transmits ros header and x, y (positions on X-Y grid)
- Create a node prop_control.cpp which takes data from a topic /husky/reference_pos (a topic which you've to subscribe to for now, of msg type ``husky_pos_vel/Position``). This node would also take current odometry of Husky through the topic mentioned above.
Now using knowledge of P Controllers and generate velocities, that should now be published on ``/husky_velocity_controller/cmd_vel``.

### Platform 
- Ubuntu 20.04
- ROS Noetic 


### Running Instructions

- Install Dependencies <br>
    For installing please refer to ROS official docs.<br>
    To install ``husky_gazebo`` (model used in this package for simulation) follow the below instructions:
    ```
    $ sudo apt-get install ros-noetic-husky-simulator
    $ export HUSKY_GAZEBO_DESCRIPTION=$(rospack find husky_gazebo)/urdf/description.gazebo.xacro
    ```
    
- Initialize a ROS Workspace
    ```
    $ mkdir -p ~/catkin_ws/src
    $ cd ~/catkin_ws/src
    ```
- Clone the package
    ```
    $ cd ~/catkin_ws/src
    $ git clone https://github.com/pulak-gautam/basic-controller-design.git . 
    ```
- Building & Launching the package
    ```
    $ cd ~/catkin_ws
    $ catkin build
    $ source devel/setup.bash
    $ roslaunch husky_pos_vel husky_pos_vel.launch 
    ```



