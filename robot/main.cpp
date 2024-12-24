#include <iostream>

#include <alcommon/albroker.h>
#include "EmotionDetection.hpp"

int main(int argc, char* argv[]) {

    boost::shared_ptr<AL::ALBroker> broker =
    AL::ALBroker::createBroker("MyBroker", "", 0, "nao.local", 9559);

    try {
        EmotionDetection emotionDetection(broker);
        emotionDetection.startEmotionDetection();
    } catch (const std::exception& e) {
        std::cerr << "Failed to start EmotionDetection: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
