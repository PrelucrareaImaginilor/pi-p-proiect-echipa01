#pragma once

#include <alcommon/albroker.h>
#include <alproxies/albehaviormanagerproxy.h>
#include <qi/os.hpp>
#include <vector>
#include <map>

class Behavior{
private:
	boost::shared_ptr<AL::ALBroker> broker;
	AL::ALBehaviorManagerProxy behavior;
	std::map<std::string, std::string> behaviors;
public:
	Behavior(boost::shared_ptr<AL::ALBroker> Broker);
	void listBehaviors();
	void executeAnimation(const std::string& animationName);
};
