#include "logger.h"
#include <cstdlib>
#include <iostream>
#include <std_msgs/String.h>
using std::rand;
using std::ostringstream;
Logger::Logger(ros::NodeHandle* handle){
	ROS_INFO("LOGGER :: (nh) --> entering.");
//	int zero = 0;
//	ros::init(zero,NULL,"commanderloggerpub");
	nh = handle;
	pub = nh->advertise<std_msgs::String>("/master/logger",1000);
//	while (pub.getNumSubscribers() <= 0) {
//		std::cout << ".";
//	}
	ID = rand();
	messageNumber = 0;
	name = "Commander";
	ROS_INFO("LOGGER :: (nh) --> handle initialized. leaving.");
}

void Logger::logStatus(string message){
	std_msgs::String msg;
	string outString = "";
	outString += name + " ";
	//cast int to string for ID
	outString += static_cast<ostringstream*>( &(ostringstream() << ID) )->str(); 
	outString += " ";
	//Cast int to string for messageNumber
	outString += static_cast<ostringstream*>( &(ostringstream() << messageNumber) )->str(); 
	outString += message;

	msg.data = outString;

	pub.publish(msg);

	ros::spinOnce();
}
