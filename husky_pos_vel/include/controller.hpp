#include <ros/ros.h>
#include <cmath>
#include <Eigen/Geometry>   
#include <Eigen/Dense>  
#include <geometry_msgs/Twist.h> //TwistWithCovariance = velocity (with uncertainity)
                                 //Twist = velocity (without uncertainity)
#include <nav_msgs/Odometry.h>
#include <husky_pos_vel/Position.h>
#include <sstream>
#include <string>

namespace controller {
    class Controller {
        public:
            Controller(ros::NodeHandle nh, ros::Rate r, std::string ref_topic, std::string odom_topic, std::string cmd_vel_topic); // constructor
            ~Controller(){}; // destructor

            void run();

        private:
            ros::NodeHandle nh;

            ros::Publisher cmd_pub;
            ros::Subscriber ref_sub;
            ros::Subscriber odom_sub;
            ros::Rate r;

            geometry_msgs::Quaternion quat;
            int d;

            float ref_X;
            float ref_Y;
            float curr_X = 0;
            float curr_Y = 0;
            float vel_X;
            float vel_Y;

            float distance_tolerance = 0.5;
            float K1 = 1;
            float K2 = 0.5;
            float K = 1000;

            static Eigen::Vector3f ToEulerAngles(const geometry_msgs::Quaternion& q);

            void odomCallback(nav_msgs::Odometry msg);
            void refCallback(husky_pos_vel::Position msg); 

    }; 


}





