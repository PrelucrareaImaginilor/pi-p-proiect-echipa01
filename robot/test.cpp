/*
#include "onchargingdetection.h"
#include <qi/applicationsession.hpp>

std::string robot_ip = "tcp://169.254.166.1:9559";

int main(int argc, char* argv[]) {

    qi::SessionPtr session = qi::makeSession();
    session->connect(robot_ip).wait();
    OnChargingDetection onChargingDetection(session);
    while (true) {
        qi::os::sleep(1);  
    }
    qi::Url robotUrl(robot_ip); 
    qi::ApplicationSession app(argc, argv);
    app.startSession();
    qi::SessionPtr session = app.session();

    try {
        session->connect(robot_ip).wait();

        OnChargingDetection onChargingDetection(session);

        std::cout << "Waiting for charging status changes..." << std::endl;

        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Failed to connect or run the application: " << e.what() << std::endl;
        return 1;
    }
    qi::Url robotUrl(robot_ip); 
    qi::ApplicationSession app(argc, argv, qi::ApplicationSession::Option_None, robotUrl);

    try {

        app.startSession();
        qi::SessionPtr session = app.session();


        OnChargingDetection onChargingDetection(session);
        std::cout << "Waiting for charging status changes..." << std::endl;
	 while (true) {
            qi::os::sleep(1);  
        }
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Failed to connect or run the application: " << e.what() << std::endl;
        return 1;
    }
    


    return 0;
}*/
/*
#include <iostream>
#include <qi/applicationsession.hpp>
#include <qi/session.hpp>
#include <qi/anyobject.hpp>
#include <qi/os.hpp>
#include <thread>

std::string robot_ip = "tcp://169.254.115.164:9559";


void onChargingStatusChanged(const qi::AnyValue& value) {
    bool isCharging = value.to<bool>();
    if (isCharging) {
        std::cout << "NAO is charging." << std::endl;
    } else {
        std::cout << "NAO is not charging." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    qi::ApplicationSession app(argc, argv, qi::ApplicationSession::Option_None, qi::Url(robot_ip));
    try {
        app.startSession();
        qi::SessionPtr session = app.session();


        qi::AnyObject memoryProxy = session->service("ALMemory");


        qi::AnyObject subscriber = memoryProxy.call<qi::AnyObject>("subscriber", "BatteryChargingFlagChanged");
        subscriber.connect("signal", &onChargingStatusChanged);

        std::cout << "Successfully subscribed to BatteryChargingFlagChanged event." << std::endl;


        app.run();

    } catch (const std::exception& e) {
        std::cerr << "Failed to connect or run the application: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
*/

#include <iostream>
#include <qi/applicationsession.hpp>
#include <qi/session.hpp>
#include "Behavior.hpp"

// run with --qi-url <url> 
// or undo this std::string robot_ip = "";
/*
std::vector<std::string> emo = {
"animations/Stand/Emotions/Negative/Angry_1",
"animations/Stand/Emotions/Negative/Angry_2",
animations/Stand/Emotions/Negative/Angry_3
animations/Stand/Emotions/Negative/Angry_4
animations/Stand/Emotions/Negative/Anxious_1
animations/Stand/Emotions/Negative/Bored_1
animations/Stand/Emotions/Negative/Bored_2
animations/Stand/Emotions/Negative/Disappointed_1
animations/Stand/Emotions/Negative/Exhausted_1
animations/Stand/Emotions/Negative/Exhausted_2
animations/Stand/Emotions/Negative/Fear_1
animations/Stand/Emotions/Negative/Fear_2
animations/Stand/Emotions/Negative/Fearful_1
animations/Stand/Emotions/Negative/Frustrated_1
animations/Stand/Emotions/Negative/Humiliated_1
animations/Stand/Emotions/Negative/Hurt_1
animations/Stand/Emotions/Negative/Hurt_2
animations/Stand/Emotions/Negative/Late_1
animations/Stand/Emotions/Negative/Sad_1
animations/Stand/Emotions/Negative/Sad_2
animations/Stand/Emotions/Negative/Shocked_1
animations/Stand/Emotions/Negative/Sorry_1
animations/Stand/Emotions/Negative/Surprise_1
animations/Stand/Emotions/Negative/Surprise_2
animations/Stand/Emotions/Negative/Surprise_3
animations/Stand/Emotions/Neutral/Alienated_1
animations/Stand/Emotions/Neutral/Annoyed_1
animations/Stand/Emotions/Neutral/AskForAttention_1
animations/Stand/Emotions/Neutral/AskForAttention_2
animations/Stand/Emotions/Neutral/AskForAttention_3
animations/Stand/Emotions/Neutral/Cautious_1
animations/Stand/Emotions/Neutral/Confused_1
animations/Stand/Emotions/Neutral/Determined_1
animations/Stand/Emotions/Neutral/Embarrassed_1
animations/Stand/Emotions/Neutral/Hello_1
animations/Stand/Emotions/Neutral/Hesitation_1
animations/Stand/Emotions/Neutral/Innocent_1
animations/Stand/Emotions/Neutral/Lonely_1
animations/Stand/Emotions/Neutral/Mischievous_1
animations/Stand/Emotions/Neutral/Puzzled_1
animations/Stand/Emotions/Neutral/Sneeze
animations/Stand/Emotions/Neutral/Stubborn_1
animations/Stand/Emotions/Neutral/Suspicious_1
animations/Stand/Emotions/Positive/Amused_1
animations/Stand/Emotions/Positive/Confident_1
animations/Stand/Emotions/Positive/Ecstatic_1
animations/Stand/Emotions/Positive/Enthusiastic_1
animations/Stand/Emotions/Positive/Excited_1
animations/Stand/Emotions/Positive/Excited_2
animations/Stand/Emotions/Positive/Excited_3
animations/Stand/Emotions/Positive/Happy_1
animations/Stand/Emotions/Positive/Happy_2
animations/Stand/Emotions/Positive/Happy_3
animations/Stand/Emotions/Positive/Happy_4
animations/Stand/Emotions/Positive/Hungry_1
animations/Stand/Emotions/Positive/Hysterical_1
animations/Stand/Emotions/Positive/Interested_1
animations/Stand/Emotions/Positive/Interested_2
animations/Stand/Emotions/Positive/Laugh_1
animations/Stand/Emotions/Positive/Laugh_2
animations/Stand/Emotions/Positive/Laugh_3
animations/Stand/Emotions/Positive/Mocker_1
animations/Stand/Emotions/Positive/Optimistic_1
animations/Stand/Emotions/Positive/Peaceful_1
animations/Stand/Emotions/Positive/Proud_1
animations/Stand/Emotions/Positive/Proud_2
animations/Stand/Emotions/Positive/Proud_3
animations/Stand/Emotions/Positive/Relieved_1
animations/Stand/Emotions/Positive/Shy_1
animations/Stand/Emotions/Positive/Shy_2
animations/Stand/Emotions/Positive/Sure_1
animations/Stand/Emotions/Positive/Winner_1
animations/Stand/Emotions/Positive/Winner_2
};
*/
std::vector<std::string> emo = {
    "animations/Stand/Emotions/Negative/Angry_1",
    "animations/Stand/Emotions/Negative/Angry_2",
    "animations/Stand/Emotions/Negative/Angry_3",
    "animations/Stand/Emotions/Negative/Angry_4",
    "animations/Stand/Emotions/Negative/Anxious_1",
    "animations/Stand/Emotions/Negative/Bored_1",
    "animations/Stand/Emotions/Negative/Bored_2",
    "animations/Stand/Emotions/Negative/Disappointed_1",
    "animations/Stand/Emotions/Negative/Exhausted_1",
    "animations/Stand/Emotions/Negative/Exhausted_2",
    "animations/Stand/Emotions/Negative/Fear_1",
    "animations/Stand/Emotions/Negative/Fear_2",
    "animations/Stand/Emotions/Negative/Fearful_1",
    "animations/Stand/Emotions/Negative/Frustrated_1",
    "animations/Stand/Emotions/Negative/Humiliated_1",
    "animations/Stand/Emotions/Negative/Hurt_1",
    "animations/Stand/Emotions/Negative/Hurt_2",
    "animations/Stand/Emotions/Negative/Late_1",
    "animations/Stand/Emotions/Negative/Sad_1",
    "animations/Stand/Emotions/Negative/Sad_2",
    "animations/Stand/Emotions/Negative/Shocked_1",
    "animations/Stand/Emotions/Negative/Sorry_1",
    "animations/Stand/Emotions/Negative/Surprise_1",
    "animations/Stand/Emotions/Negative/Surprise_2",
    "animations/Stand/Emotions/Negative/Surprise_3",
    "animations/Stand/Emotions/Neutral/Alienated_1",
    "animations/Stand/Emotions/Neutral/Annoyed_1",
    "animations/Stand/Emotions/Neutral/AskForAttention_1",
    "animations/Stand/Emotions/Neutral/AskForAttention_2",
    "animations/Stand/Emotions/Neutral/AskForAttention_3",
    "animations/Stand/Emotions/Neutral/Cautious_1",
    "animations/Stand/Emotions/Neutral/Confused_1",
    "animations/Stand/Emotions/Neutral/Determined_1",
    "animations/Stand/Emotions/Neutral/Embarrassed_1",
    "animations/Stand/Emotions/Neutral/Hello_1",
    "animations/Stand/Emotions/Neutral/Hesitation_1",
    "animations/Stand/Emotions/Neutral/Innocent_1",
    "animations/Stand/Emotions/Neutral/Lonely_1",
    "animations/Stand/Emotions/Neutral/Mischievous_1",
    "animations/Stand/Emotions/Neutral/Puzzled_1",
    "animations/Stand/Emotions/Neutral/Sneeze",
    "animations/Stand/Emotions/Neutral/Stubborn_1",
    "animations/Stand/Emotions/Neutral/Suspicious_1",
    "animations/Stand/Emotions/Positive/Amused_1",
    "animations/Stand/Emotions/Positive/Confident_1",
    "animations/Stand/Emotions/Positive/Ecstatic_1",
    "animations/Stand/Emotions/Positive/Enthusiastic_1",
    "animations/Stand/Emotions/Positive/Excited_1",
    "animations/Stand/Emotions/Positive/Excited_2",
    "animations/Stand/Emotions/Positive/Excited_3",
    "animations/Stand/Emotions/Positive/Happy_1",
    "animations/Stand/Emotions/Positive/Happy_2",
    "animations/Stand/Emotions/Positive/Happy_3",
    "animations/Stand/Emotions/Positive/Happy_4",
    "animations/Stand/Emotions/Positive/Hungry_1",
    "animations/Stand/Emotions/Positive/Hysterical_1",
    "animations/Stand/Emotions/Positive/Interested_1",
    "animations/Stand/Emotions/Positive/Interested_2",
    "animations/Stand/Emotions/Positive/Laugh_1",
    "animations/Stand/Emotions/Positive/Laugh_2",
    "animations/Stand/Emotions/Positive/Laugh_3",
    "animations/Stand/Emotions/Positive/Mocker_1",
    "animations/Stand/Emotions/Positive/Optimistic_1",
    "animations/Stand/Emotions/Positive/Peaceful_1",
    "animations/Stand/Emotions/Positive/Proud_1",
    "animations/Stand/Emotions/Positive/Proud_2",
    "animations/Stand/Emotions/Positive/Proud_3",
    "animations/Stand/Emotions/Positive/Relieved_1",
    "animations/Stand/Emotions/Positive/Shy_1",
    "animations/Stand/Emotions/Positive/Shy_2",
    "animations/Stand/Emotions/Positive/Sure_1",
    "animations/Stand/Emotions/Positive/Winner_1",
    "animations/Stand/Emotions/Positive/Winner_2"
};

int main(int argc, char* argv[]) {
    // and this qi::ApplicationSession app(argc, argv, qi::ApplicationSession::Option_None, qi::Url(robot_ip));
    qi::ApplicationSession app(argc, argv);

    try {
        app.startSession();
        qi::SessionPtr session = app.session();

        Behavior beh(session);
        //beh.listBehaviors();
	for(auto e: emo){
		beh.executeAnimation(e);

	}
	
	//qi::os::sleep(1);

    } catch (const std::exception& e) {
        std::cerr << "Failed to connect or run the application: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

