#include <ros/ros.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sensor_msgs/Imu.h>
#include <tf/transform_datatypes.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Int32.h>


using namespace std;

static bool isIMUready = false;
static bool isODOready = false;

double roll = 0;
double pitch = 0;
double yaw = 0;
float x = 0;
float y= 0;

void chatterIMU(const sensor_msgs::Imu::ConstPtr& msg)
{
    tf::Quaternion q(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
    tf::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);

    //ROS_INFO("Imu Seq: [%d]", msg->header.seq);
    //ROS_INFO("EULER Imu Orientation Roll: [%f], Pitch: [%f], Yaw: [%f]", roll, pitch, yaw);
    //ROS_INFO("QUATERNION Imu Orientation x: [%f], y: [%f], z: [%f], w: [%f]", msg->orientation.x,msg->orientation.y,msg->orientation.z,msg->orientation.w);

    isIMUready = true;
}

void chatterODO(const nav_msgs::Odometry::ConstPtr& msg)
{
    //ROS_INFO("Seq: [%d]", msg->header.seq);
    //ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
    //ROS_INFO("Orientation-> x: [%f], y: [%f], z: [%f], w: [%f]", msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
    //ROS_INFO("Vel-> Linear: [%f], Angular: [%f]", msg->twist.twist.linear.x,msg->twist.twist.angular.z);

    x =  msg->pose.pose.position.x;
    y =  msg->pose.pose.position.y;

    isODOready = true;
}


int main(int argc, char **argv)
{
    cout<<"Ros Turtlebot Initialize"<<endl;
    ros::init(argc, argv, "imu_listener");

    ros::NodeHandle n;

    ros::Rate rate(5);

    ros::spinOnce();

    ros::Subscriber sub = n.subscribe("/imu", 100, chatterIMU);
    ros::Subscriber sub1 = n.subscribe("odom", 100, chatterODO);
    ros::Publisher  pub = n.advertise<std_msgs::Int32>("liczba", 10);

    //ros::spin();


    while (ros::ok()) {
        if(!isIMUready && !isIMUready) {

            cout<<"Ros Turtlebot Initialize"<<endl;
            ros::spinOnce();
            rate.sleep();
            continue;
        }

        std_msgs::Int32 message;
        message.data = 123;
        cout << "Position x "<< x <<setprecision(5)<< endl;
        cout << "Yaw angle "<< yaw <<setprecision(5)<< endl;
        cout << "Message data "<< message.data <<setprecision(5)<< endl;
        rate.sleep();
        ros::spinOnce();
    }

    return 0;
}
