#pragma once

#include <qi/applicationsession.hpp>
#include <qi/session.hpp>
#include <qi/anyobject.hpp>
#include <qi/os.hpp>
#include <vector>
#include <array>
#include <map>
#include "Behavior.hpp"

class EmotionDetection{
private:
	qi::SessionPtr session;
	qi::AnyObject memory;
	qi::AnyObject faceC;
	qi::AnyObject motion;
	qi::AnyObject speech;
	Behavior* behavior;	

	std::map<std::string, bool> parameters;
	std::map<std::string, float> tresholds;
	bool isRunning;
	unsigned int counter;
	std::array<std::string, 5> emotions;
	float recognitionValues[5]={0};
	float confidenceTreshold;
	unsigned int activeEmotions;

	void handleDetectedEmotion(const std::string& emotion);
	void searchForFace();
public:
	EmotionDetection(qi::SessionPtr Session);
	void setParameter(const std::string& param, float value);
	bool getParameter(const std::string& param);
	void startEmotionDetection();
};
