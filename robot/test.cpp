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


