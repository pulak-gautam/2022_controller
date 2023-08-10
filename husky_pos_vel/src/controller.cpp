#include "controller.hpp"

namespace controller {
    Controller::Controller(ros::NodeHandle nh, ros::Rate r, std::string ref_topic, std::string odom_topic, std::string cmd_vel_topic) : nh(nh), r(r) {
        cmd_pub = nh.advertise<geometry_msgs::Twist>(cmd_vel_topic, 100);

        ref_sub = nh.subscribe(ref_topic, 1000, &Controller::refCallback, this);
        odom_sub = nh.subscribe(odom_topic, 1000, &Controller::odomCallback, this);
    }

    Eigen::Vector3f Controller::ToEulerAngles(const geometry_msgs::Quaternion& q) {
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

    void Controller::odomCallback(nav_msgs::Odometry msg){
        curr_X = msg.pose.pose.position.x;
        curr_Y = msg.pose.pose.position.y;

        vel_X = msg.twist.twist.linear.x;
        vel_Y = msg.twist.twist.linear.y;

        quat = msg.pose.pose.orientation;
    }

    void Controller::refCallback(husky_pos_vel::Position msg){
        ref_X = msg.x;
        ref_Y = msg.y;
    }
    
    void Controller::run(){
        geometry_msgs::Twist msg;

        ROS_INFO("Running controller node");

        while(ros::ok()){
            d = sqrt(pow(curr_X - ref_X, 2) + pow(curr_Y - ref_Y, 2));

            Eigen::Vector3f euler = ToEulerAngles(quat);

            float psi = euler[0]; //yaw

            float phi = psi - atan((ref_Y-curr_Y)/(ref_X-curr_X));

            if(phi > M_PI)          //to maintain angle in interval -pi to pi
                phi = phi - 2*M_PI;
            else if(phi < -M_PI)
                phi = phi + 2*M_PI;

            msg.linear.x = K1*d*cos(phi); //euler angles to angular velocity formulae
            msg.angular.z = -K1*sin(phi)*cos(phi) - K2*phi;

            cmd_pub.publish(msg);

            ROS_INFO("\nref_x:%f\t curr_x:%f\n ref_y:%f\t curr_y:%f\n", ref_X, curr_X, ref_Y, curr_Y);

            //TODO: add a condition to break the loop when the destination is reached
            /*if(d < distance_tolerance){
                if(ref_X != curr_X && ref_Y != curr_Y){    //to avoid the case of quitting the node, when the parameters are not yet initialized
                    ROS_INFO("Destination Reached");
                    break;
                } 
            }*/

            ros::spinOnce();
            r.sleep();
        }
            

        msg.linear.x = 0;  //bringing the husky to complete halt after reaching destination
        msg.angular.z = 0;

        cmd_pub.publish(msg);

        ROS_INFO("Destination Reached");
    }
}






//TwistWithCovariance = velocity (with uncertainity)


