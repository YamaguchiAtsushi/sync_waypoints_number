#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Quaternion.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Int16.h>


#include <map>
#include <math.h>
#include <vector>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>



std::map<std::string, double> params_; // パラメータをここに格納
//std_msgs::Int16 waypoints_A_number_now; // 今向かっているロボットAのwaypointの番号;
std_msgs::Int16 waypoints_A_number_next; // 送られてくる次に向かうロボットAのwaypointの番号

int waypoints_A_number_now = 0; //今向かっているロボットAのwaypointの番号
//int waypoints_A_number_next = 0; // 送られてくる次に向かうロボットAのwaypointの番号


void numberCallback(const std_msgs::Int16::ConstPtr &msg)
{
    waypoints_A_number_now = msg->data;
}

int main(int argc, char **argv)
{
	// 初期化関連
	ros::init(argc, argv, "sync_waypoints_number");
	ros::NodeHandle nh;
	ros::NodeHandle pnh("~");

	// Subscriber, Publisherの定義
    ros::Subscriber waypoints_A_number_sub = nh.subscribe("waypoints_A_number", 10, numberCallback);
    ros::Publisher waypoints_A_number_pub = nh.advertise<std_msgs::Int16>("waypoints_A_number", 10);

	ros::Rate loop_rate(100);


    int to_be_waypoints_number = 0;

    while(ros::ok()){
        ros::spinOnce();
        if (waypoints_A_number_now == to_be_waypoints_number){
            waypoints_A_number_next.data = waypoints_A_number_now + 1;
            waypoints_A_number_pub.publish(waypoints_A_number_next);
            to_be_waypoints_number +=1;
        }


        std::cout << waypoints_A_number_now << std::endl;

        loop_rate.sleep();
    }
	return 0;
}