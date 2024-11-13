#include "Behavior.hpp"

Behavior::Behavior(qi::SessionPtr Session) :session(Session){
	try{
		behavior = session->service("ALBehaviorManager");
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
	std::vector<std::string> behaviors = behavior.call<std::vector<std::string>>("getInstalledBehaviors");

    	std::cout << "Installed Behaviors:" << std::endl;
        for (const auto& behavior : behaviors) {
            std::cout << behavior << std::endl;
        }
}

void Behavior::executeAnimation(const std::string& emotion) {
    try {
        std::string animationName = behaviors[emotion];
        if (!behavior.call<bool>("isBehaviorInstalled", animationName)) {
            std::cerr << "Behavior " << animationName << " is not installed on the robot." << std::endl;
            return;
        }

        if (behavior.call<bool>("isBehaviorRunning", animationName)) {
            std::cerr << "Behavior " << animationName << " is already running." << std::endl;
            return;
        }

        std::cout << "Starting animation: " << animationName << std::endl;
        behavior.call<void>("runBehavior", animationName);

        qi::os::sleep(1);

    } catch (const std::exception& e) {
        std::cerr << "Failed to execute animation: " << e.what() << std::endl;
    }
}


