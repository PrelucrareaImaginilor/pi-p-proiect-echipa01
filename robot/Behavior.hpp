#pragma once

#include <qi/session.hpp>
#include <qi/anyobject.hpp>
#include <vector>


class Behavior{
private:
	qi::SessionPtr session;
	qi::AnyObject behavior;
	//std::array<std::string> behaviors;
public:
	Behavior(qi::SessionPtr Session);
	void listBehaviors();
	void executeAnimation(const std::string& animationName);
};
