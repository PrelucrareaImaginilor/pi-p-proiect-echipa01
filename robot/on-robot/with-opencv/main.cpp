#include <iostream>
#include <alcommon/albroker.h>
#include <alproxies/alvideodeviceproxy.h>
#include <alvision/alimage.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

extern "C" void cvShowImage() {
    // 
}


int main(int argc, char* argv[]) {
    std::cout << "In main\n";

    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load("haarcascade_frontalface_default.xml")) {
        std::cerr << "Error loading Haar cascade file" << std::endl;
        return 1;
    }


    boost::shared_ptr<AL::ALBroker> broker =
        AL::ALBroker::createBroker("MyBroker", "", 0, "nao.local", 9559);

    try {

        AL::ALVideoDeviceProxy videoDeviceProxy(broker);


        std::string clientName = videoDeviceProxy.subscribeCamera(
            "TestClient", 0, 1, 13, 30);

        while (true) {

            AL::ALValue image = videoDeviceProxy.getImageRemote(clientName);
            if (image.getSize() == 0) {
                std::cerr << "Failed to get image from camera" << std::endl;
                continue;
            }


            int width = image[0];
            int height = image[1];
            int channels = image[2];
            const unsigned char* imageData = static_cast<const unsigned char*>(image[6].GetBinary());


            cv::Mat frame(height, width, CV_8UC3, const_cast<unsigned char*>(imageData));


            cv::Mat gray;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);


            std::vector<cv::Rect> faces;
            faceCascade.detectMultiScale(gray, faces);

          

            std::cout << "Detected " << faces.size() << " face(s)" << std::endl;

            videoDeviceProxy.releaseImage(clientName);
        }


        videoDeviceProxy.unsubscribe(clientName);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
