#include "EmotionDetection.hpp"

EmotionDetection::EmotionDetection(qi::SessionPtr Session)
	: session(Session), counter(0), isRunning(false), confidenceTreshold(0.5){
	
	 try {
            faceC = session->service("ALFaceCharacteristics");
            memory = session->service("ALMemory");
            motion = session->service("ALMotion");
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Failed to connect to services: ") + e.what());
        }
		
	emotions = {"neutral", "happy", "surprised", "angry", "sad"};

	parameters["neutral"] = true;
	parameters["happy"] = true;
	parameters["surprised"] = true;
	parameters["angry"] = true;
	parameters["sad"] = true;
	
	activeEmotions = 5;

	tresholds["neutral"] = confidenceTreshold - 0.05;
	tresholds["happy"] = confidenceTreshold;
	tresholds["surprised"] = confidenceTreshold;
	tresholds["angry"] = confidenceTreshold;
	tresholds["sad"] = confidenceTreshold + 0.1;
}

void EmotionDetection::setParameter(const std::string& param, float value) {
        parameters[param] = value;
	activeEmotions = 0;
	for(auto it: parameters)
		it.second && activeEmotions++;
}

bool EmotionDetection::getParameter(const std::string& param) {
        return parameters[param];
}

void EmotionDetection::startEmotionDetection(){
	isRunning = true;
	std::vector<int> ids;
	std::vector<float> properties;
	std::array<float, 5> recognized;
	while(isRunning){
		if(counter<4){
			ids = memory.call<std::vector<int>>("getData", "PeoplePerception/PeopleList");
			if(ids.empty()){
				std::cout<<"No face detected"<<std::endl;
				//searchForFaces
			}
			else if(ids.size()>1){
				std::cout<<"Multiple faces detected"<<std::endl;
				//add a queue
			}
			else{
				faceC.call<void>("analyzeFaceCharacteristics", ids[0]);

				qi::os::sleep(1);
				
				properties = memory.call<std::vector<float>>("getData", "PeoplePerception/Person/" + std::to_string(ids[0]) + "/ExpressionProperties");

				for(size_t i=0; i<properties.size(); i++){
					recognitionValues[i] += properties[i];
				}
				++counter;
			}
		}
		else{
			recognized.fill(0);
			for(auto& reco : recognitionValues) reco /= activeEmotions;
			
			if (getParameter("neutral") && recognitionValues[0] > tresholds["neutral"]) recognized[0] = recognitionValues[0];
			if (getParameter("happy") && recognitionValues[1] > tresholds["happy"]) recognized[1] = recognitionValues[1];
			if (getParameter("surprised") && recognitionValues[2] > tresholds["surprised"]) recognized[2] = recognitionValues[2];
			if (getParameter("angry") && recognitionValues[3] > tresholds["angry"]) recognized[3] = recognitionValues[3];
			if (getParameter("sad") && recognitionValues[4] > tresholds["sad"]) recognized[4] = recognitionValues[4];

			for(auto& reco : recognitionValues) reco = 0;
			
			counter = 0;
			
			//handle neutral if nothing found
			std::string emotion = emotions[0];
			if (std::any_of(recognized.begin(), recognized.end(), [](float value) { return value > 0; })) {
			    auto max_it = std::max_element(recognized.begin(), recognized.end());
			    emotion = emotions[std::distance(recognized.begin(), max_it)];
			}
		}
	}

}

void EmotionDetection::handleDetectedEmotion(const std::string& emotion) {
        std::cout << "Detected emotion: " << emotion << std::endl;
}

void EmotionDetection::searchForFace(){
	//
}


