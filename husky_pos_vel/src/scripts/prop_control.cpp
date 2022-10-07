#include <ros/ros.h>
#include <cmath>
#include <Eigen/Geometry>   
#include <Eigen/Dense>  

#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <husky_pos_vel/Position.h>

#include <sstream>

float K = 1000;

float distance_tolerance = 0.1;
float K1 = 0.5;
float K2 = 0.5;

float ref_X;
float ref_Y;

float curr_X = 0;
float curr_Y = 0;

float vel_X;
float vel_Y;

geometry_msgs::Quaternion quat;

Eigen::Vector3f ToEulerAngles(const geometry_msgs::Quaternion& q) {
    Eigen::Vector3f angles;    //yaw pitch roll
    const auto x = q.x;
    const auto y = q.y;
    const auto z = q.z;
    const auto w = q.w;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (w * x + y * z);
    double cosr_cosp = 1 - 2 * (x * x + y * y);
    angles[2] = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (w * y - z * x);
    if (std::abs(sinp) >= 1)
        angles[1] = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles[1] = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (w * z + x * y);
    double cosy_cosp = 1 - 2 * (y * y + z * z);
    angles[0] = std::atan2(siny_cosp, cosy_cosp);
    return angles;
}

int abs_diff(int a, int b){
    if(a>b) return (a-b);
    return (b-a);
}

void Callback(husky_pos_vel::Position msg){
    ref_X = msg.x;
    ref_Y = msg.y;
}

//TwistWithCovariance = velocity (with uncertainity)

void Callback_odom(nav_msgs::Odometry msg){

    curr_X = msg.pose.pose.position.x;
    curr_Y = msg.pose.pose.position.y;

    vel_X = msg.twist.twist.linear.x;
    vel_Y = msg.twist.twist.linear.y;

    quat = msg.pose.pose.orientation;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "prop_control");
    ros::NodeHandle n;

    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/husky_velocity_controller/cmd_vel", 100);

    ros::Subscriber sub = n.subscribe("/husky/reference_pos", 1000, Callback);
    ros::Subscriber sub_odom = n.subscribe("/husky_velocity_controller/odom", 1000, Callback_odom);
    
    ros::Rate r(10);

    //REFINED CODE-->

    int d = sqrt(pow(curr_X - ref_X, 2) + pow(curr_Y - ref_Y, 2));

    geometry_msgs::Twist msg;

    while(ros::ok()){

        d = sqrt(pow(curr_X - ref_X, 2) + pow(curr_Y - ref_Y, 2));

        Eigen::Vector3f euler = ToEulerAngles(quat);

        float psi = euler[0]; //yaw

        float phi = psi - atan((ref_Y-curr_Y)/(ref_X-curr_X));

        if(phi > M_PI)          //to maintain angle in interval -pi to pi
            phi = phi - 2*M_PI;
        else if(phi < -M_PI)
            phi = phi + 2*M_PI;

        msg.linear.x = K1*d*cos(phi);  //euler angles to angular velocity formulae
        msg.angular.z = -K1*sin(phi)*cos(phi) - K2*phi;

        pub.publish(msg);
        r.sleep();

        ROS_INFO("\nref_x:%f\t curr_x:%f\n ref_y:%f\t curr_y:%f\n", ref_X, curr_X, ref_Y, curr_Y);
        ros::spinOnce();
    }

    msg.linear.x = 0;  //bringing the husky to complete halt after reaching destination
    msg.angular.z = 0;

    pub.publish(msg);
    







    //OLD CODE-->
    /*
    while(ros::ok()){
        geometry_msgs::Twist msg;

        
        
        double theta = atan((ref_Y-curr_Y)/(ref_X-curr_X));
        msg.linear.x = K*abs_diff(curr_X, ref_X);
        msg.linear.y = K*abs_diff(curr_Y, ref_Y);
        msg.linear.z = 0;

        Eigen::Vector3f euler = ToEulerAngles(quat);

        msg.angular.x = 0;
        msg.angular.y = 0;
        msg.angular.z = K/200*abs_diff(euler[0], atan((ref_Y-curr_Y)/(ref_X-curr_X)));
        // msg.angular.z = atan((ref_Y-curr_Y)/(ref_X-curr_X));
        



        r.sleep();


    
        pub.publish(msg);
        ros::spinOnce();

        r.sleep();
    }
    */
    
}
