#include "Behavior.hpp"

Behavior::Behavior(boost::shared_ptr<AL::ALBroker> Broker) :broker(Broker){
	try{
		behavior = AL::ALBehaviorManagerProxy(broker);
	} catch(const std::exception &e) {
		throw std::runtime_error(std::string("Failed to connect to service: ") + e.what());
	}
	behaviors["neutral"] = "animations/Stand/Emotions/Neutral/Hello_1";
	behaviors["happy"] = "animations/Stand/Emotions/Positive/Mocker_1";
	behaviors["surprised"] = "animations/Stand/Emotions/Negative/Surprise_1";
	behaviors["angry"] = "animations/Stand/Emotions/Negative/Angry_2";
	behaviors["sad"] = "animations/Stand/Emotions/Negative/Bored_1";
}

void Behavior::listBehaviors(){
	std::vector<std::string> behaviors = behavior.getInstalledBehaviors();

    	std::cout << "Installed Behaviors:" << std::endl;
        for(int i = 0; i<behaviors.size(); i++) {
            std::cout << behaviors[i] << std::endl;
        }
}

void Behavior::executeAnimation(const std::string& emotion) {
    try {
        std::string animationName = behaviors[emotion];
        if (!behavior.isBehaviorInstalled(animationName)) {
            std::cerr << "Behavior " << animationName << " is not installed on the robot." << std::endl;
            return;
        }

        if (behavior.isBehaviorRunning(animationName)) {
            std::cerr << "Behavior " << animationName << " is already running." << std::endl;
            return;
        }

        std::cout << "Starting animation: " << animationName << std::endl;
        behavior.runBehavior(animationName);

        qi::os::sleep(1);

    } catch (const std::exception& e) {
        std::cerr << "Failed to execute animation: " << e.what() << std::endl;
    }
}


