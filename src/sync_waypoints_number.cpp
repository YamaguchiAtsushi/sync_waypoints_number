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
std_msgs::Int16 waypoints_B_number_next; // 送られてくる次に向かうロボットAのwaypointの番号


int waypoints_A_number_now = -1; //今向かっているロボットAのwaypointの番号
int waypoints_B_number_now = -1; //今向かっているロボットAのwaypointの番号

//int waypoints_A_number_next = 0; // 送られてくる次に向かうロボットAのwaypointの番号
bool send_flag = false;
int before = 100;

void A_numberCallback(const std_msgs::Int16::ConstPtr &msg)
{

    waypoints_A_number_now = msg->data;

}

void B_numberCallback(const std_msgs::Int16::ConstPtr &msg)
{

    waypoints_B_number_now = msg->data;

}

int main(int argc, char **argv)
{
	// 初期化関連
	ros::init(argc, argv, "sync_waypoints_number");
	ros::NodeHandle nh;
	ros::NodeHandle pnh("~");

	// Subscriber, Publisherの定義
    ros::Subscriber waypoints_A_number_sub = nh.subscribe("robotA/waypoints_A_number_now", 10, A_numberCallback);
    ros::Publisher waypoints_A_number_pub = nh.advertise<std_msgs::Int16>("robotA/waypoints_A_number_next", 10);
    ros::Subscriber waypoints_B_number_sub = nh.subscribe("robotB/waypoints_A_number_now", 10, B_numberCallback);
    ros::Publisher waypoints_B_number_pub = nh.advertise<std_msgs::Int16>("robotB/waypoints_A_number_next", 10);

	ros::Rate loop_rate(100);


    while(ros::ok()){
        

        ros::spinOnce();

        std::cout << "waypoints_A_number_now: " << waypoints_A_number_now << std::endl;
        //std::cout << "waypoints_A_number_next: " << waypoints_A_number_next.data << std::endl;

        if (waypoints_A_number_now == waypoints_B_number_now){
            waypoints_A_number_next.data = waypoints_A_number_now + 1;
            waypoints_B_number_next.data = waypoints_B_number_now + 1;
        }
        waypoints_A_number_pub.publish(waypoints_A_number_next);
        waypoints_B_number_pub.publish(waypoints_B_number_next);


        loop_rate.sleep();
    }
	return 0;
}