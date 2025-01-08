#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace cv;

cv::Mat extractHOGFeatures(const cv::Mat& img) {
    cv::HOGDescriptor hog(
        cv::Size(48, 48), // WinSize
        cv::Size(16, 16), // BlockSize
        cv::Size(8, 8),   // BlockStride
        cv::Size(8, 8),   // CellSize
        9                 // NumBins
    );

    std::vector<float> descriptors;
    hog.compute(img, descriptors);
    return cv::Mat(descriptors).clone().reshape(1, 1);
}

void trainEmotionModel(const std::map<std::string, int>& emotionPaths, const std::string& modelOutput) {
    cv::Mat features; 
    cv::Mat labels;  

    for (const auto& pair : emotionPaths) {
        const std::string& emotionPath = pair.first;
        int label = pair.second;

        std::vector<std::string> imagePaths;
        for (int i = 1; i < 3001; i++) {
            imagePaths.push_back(emotionPath +"/"+ std::to_string(i) + ".jpg");
        }

        for (const auto& imgPath : imagePaths) {
            cv::Mat img = cv::imread(imgPath, cv::IMREAD_GRAYSCALE);
            if (img.empty()) {
                std::cerr << "Failed to load image: " << imgPath << std::endl;
                continue;
            }

            cv::resize(img, img, cv::Size(48, 48));

            cv::Mat hogFeatures = extractHOGFeatures(img);

            features.push_back(hogFeatures);
            labels.push_back(label);
        }
    }

    if (features.empty() || labels.empty()) {
        std::cerr << "No training data provided. Please check the paths and images." << std::endl;
        return;
    }

    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::LINEAR); 
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 1000, 1e-6));

    std::cout << "Training SVM..." << std::endl;
    svm->train(features, cv::ml::ROW_SAMPLE, labels);

    std::cout << "Saving trained model to " << modelOutput << std::endl;
    svm->save(modelOutput);
}

std::string predictEmotion(const cv::Mat& img, cv::Ptr<cv::ml::SVM> svm, const std::map<int, std::string>& labelToEmotion) {
    cv::Mat resizedImg;
    cv::resize(img, resizedImg, cv::Size(48, 48));
    cv::Mat features = extractHOGFeatures(resizedImg);

    float predictedLabel = svm->predict(features);

    if (labelToEmotion.find(static_cast<int>(predictedLabel)) != labelToEmotion.end()) {
        return labelToEmotion.at(static_cast<int>(predictedLabel));
    }
    else {
        return "Unknown";
    }
}

int main() {
    std::string modelPath = "emotion_svm_model.yml";

    /*
    std::map<std::string, int> emotionPaths = {
        {"train/angry", 0},  
       // {"train/disgust", 1}, 
       // {"train/fear", 2},   
        {"train/happy", 3},   
        {"train/sad", 4},     
        //{"train/surprise", 5},
        {"train/neutral", 6}  
    };
    trainEmotionModel(emotionPaths, modelPath);
    */
    
    
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::load(modelPath);

    std::map<int, std::string> labelToEmotion = {
        {0, "angry"},
        //{1, "disgust"},
        //{2, "fear"},
        {3, "happy"},
        {4, "sad"},
        //{5, "surprise"},
        {6, "neutral"}
    };

    CascadeClassifier faceCascade;
    if (!faceCascade.load("haarcascade_frontalface_default.xml")) {
        std::cerr << "Error loading Haar cascade file" << std::endl;
        return -1;
    }

    VideoCapture capture(0); 
    if (!capture.isOpened()) {
        std::cerr << "Error opening video capture!\n";
        return -1;
    }

    Mat frame;
    while (capture.read(frame)) {
        if (frame.empty()) {
            std::cerr << "Empty frame captured!\n";
            break;
        }

        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        std::vector<Rect> faces;
        faceCascade.detectMultiScale(
            gray,
            faces,
            1.1,
            3,
            0,
            Size(30, 30)
        );
        for (size_t i = 0; i < faces.size(); i++) {
            rectangle(frame, faces[i], Scalar(255, 0, 0), 2);

            Mat faceROI = gray(faces[i]);

            std::string emotion = predictEmotion(faceROI, svm, labelToEmotion);
            int baseLine = 0;
            Size textSize = getTextSize(emotion, FONT_HERSHEY_SIMPLEX, 0.7, 2, &baseLine);
            Point textOrg(faces[i].x, faces[i].y - 5);

            putText(
                frame,
                emotion,
                textOrg,
                FONT_HERSHEY_SIMPLEX,
                0.7,
                Scalar(0, 255, 0),
                2
            );

            imshow("Emotion Detection", frame);

            char c = (char)waitKey(10);
            if (c == 'q') {
                break;
            }
        }
        //std::cout<<predictEmotion(gray, svm, labelToEmotion)<<std::endl;

    }
    capture.release();
    destroyAllWindows();
    return 0;
}
