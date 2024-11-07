#include <iostream>
#include <qi/applicationsession.hpp>
#include <qi/session.hpp>
#include "EmotionDetection.hpp"

// run with --qi-url <url> 
// or undo this std::string robot_ip = "";
int main(int argc, char* argv[]) {
    // and this qi::ApplicationSession app(argc, argv, qi::ApplicationSession::Option_None, qi::Url(robot_ip));
    qi::ApplicationSession app(argc, argv);

    try {
        app.startSession();
        qi::SessionPtr session = app.session();

        EmotionDetection emotionDetection(session);
        emotionDetection.startEmotionDetection();

    } catch (const std::exception& e) {
        std::cerr << "Failed to connect or run the application: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
