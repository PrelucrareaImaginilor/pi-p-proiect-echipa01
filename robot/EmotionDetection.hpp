#pragma once

#include <alcommon/albroker.h>
#include <alproxies/almemoryproxy.h>
#include <alproxies/alfacecharacteristicsproxy.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/alpeopleperceptionproxy.h>
#include <alproxies/altexttospeechproxy.h>
#include <qi/os.hpp>
#include <vector>
//#include <array>
#include <map>
#include "Behavior.hpp"

class EmotionDetection{
private:
	boost::shared_ptr<AL::ALBroker> broker;
	AL::ALMemoryProxy memory;
	AL::ALFaceCharacteristicsProxy faceC;
	AL::ALMotionProxy motion;
	AL::ALTextToSpeechProxy speech;
	Behavior* behavior;	

	std::map<std::string, bool> parameters;
	std::map<std::string, float> tresholds;
	bool isRunning;
	unsigned int counter;
	//std::array<std::string, 5> emotions;
	std::vector<std::string> emotions;
	float recognitionValues[5];
	float confidenceTreshold;
	unsigned int activeEmotions;

	void handleDetectedEmotion(const std::string& emotion);
	void searchForFace();
public:
	EmotionDetection(boost::shared_ptr<AL::ALBroker> Broker);
        ~EmotionDetection();
	void setParameter(const std::string& param, float value);
	bool getParameter(const std::string& param);
	void startEmotionDetection();
};
