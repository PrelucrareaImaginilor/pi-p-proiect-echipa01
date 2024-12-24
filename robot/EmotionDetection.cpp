#include "EmotionDetection.hpp"

EmotionDetection::EmotionDetection(boost::shared_ptr<AL::ALBroker> Broker)
	:broker(Broker), counter(0), isRunning(false), confidenceTreshold(0.5) {
	
	 try {
	    faceC = AL::ALFaceCharacteristicsProxy(broker);
	    memory = AL::ALMemoryProxy(broker);
            motion = AL::ALMotionProxy(broker);
	    speech = AL::ALTextToSpeechProxy(broker);
	    behavior = new Behavior(broker);
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Failed to initialize proxies: ") + e.what());
        }
	

	try {
	    AL::ALPeoplePerceptionProxy peoplePerception = AL::ALPeoplePerceptionProxy(broker);
	    peoplePerception.resetPopulation(); 
	} catch (const std::exception& e) {
	    std::cerr << "ALPeoplePerception service is not available: " << e.what() << std::endl;
	}

	//emotions = {"neutral", "happy", "surprised", "angry", "sad"};
	emotions.push_back("neutral");
        emotions.push_back("happy");
        emotions.push_back("surprised");
        emotions.push_back("angry");
        emotions.push_back("sad");	

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
	tresholds["sad"] = confidenceTreshold + 0.05;
}

EmotionDetection::~EmotionDetection() {
    delete behavior;
}

void EmotionDetection::setParameter(const std::string& param, float value) {
        parameters[param] = value;
	activeEmotions = 0;
	//for(auto it: parameters)
	//	it.second && activeEmotions++;
	for (std::map<std::string, bool>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
		if (it->second) {
		    activeEmotions++;
		}
	    }
}

bool EmotionDetection::getParameter(const std::string& param) {
        return parameters[param];
}

void EmotionDetection::startEmotionDetection(){
	isRunning = true;
	std::vector<int> ids;
	std::vector<float> properties;
	//std::array<float, 5> recognized;
	float recognized[5] = {0, 0, 0, 0, 0};
	while(isRunning){
		if(counter<4){
			try {
			    	ids = memory.getData("PeoplePerception/PeopleList");
			} catch (const std::exception& e) {
			    	std::cerr << "Failed to retrieve PeopleList: " << e.what() << std::endl;
			}

			if(ids.empty()){
				//std::cout<<"No face detected"<<std::endl;
				//searchForFaces
			}
			else if(ids.size()>1){
				std::cout<<"Multiple faces detected"<<std::endl;
				//add a queue
			}
			else{

				try {
				    faceC.analyzeFaceCharacteristics(ids[0]);
				} catch (const std::exception& e) {
				    std::cerr << "Failed to analyze face characteristics for person ID " << ids[0]
					      << ": " << e.what() << std::endl;
				    continue;
				}

				qi::os::sleep(1);
				
				try{
					char buffer[50];
					sprintf(buffer, "%d", ids[0]); 
					std::string personKey = "PeoplePerception/Person/" + std::string(buffer) + "/ExpressionProperties";
					properties = memory.getData(personKey);
					//properties = memory.getData("PeoplePerception/Person/" + std::to_string(ids[0]) + "/ExpressionProperties");
				} catch(const std::exception& e) {
				    std::cerr << "Failed to analyze get expression properties for person ID " << ids[0]
					      << ": " << e.what() << std::endl;
				    continue;
				}

				for(size_t i=0; i<properties.size(); i++){
					recognitionValues[i] += properties[i];
				}
				++counter;
			}
		}
		else{
			for (int i = 0; i < 5; ++i) {
				recognized[i] = 0;
				recognitionValues[i] = recognitionValues[i] / activeEmotions;
			}
			//recognized.fill(0);
			//for(auto& reco : recognitionValues) reco /= activeEmotions;
			
			if (getParameter("neutral") && recognitionValues[0] > tresholds["neutral"]) recognized[0] = recognitionValues[0];
			if (getParameter("happy") && recognitionValues[1] > tresholds["happy"]) recognized[1] = recognitionValues[1];
			if (getParameter("surprised") && recognitionValues[2] > tresholds["surprised"]) recognized[2] = recognitionValues[2];
			if (getParameter("angry") && recognitionValues[3] > tresholds["angry"]) recognized[3] = recognitionValues[3];
			if (getParameter("sad") && recognitionValues[4] > tresholds["sad"]) recognized[4] = recognitionValues[4];

			//for(auto& reco : recognitionValues) reco = 0;
			for (int i = 0; i < 5; ++i) {
				recognitionValues[i] = 0;
			}
			counter = 0;
			
			//handle neutral if nothing found
			std::string emotion = emotions[0];
			/*if (std::any_of(recognized.begin(), recognized.end(), [](float value) { return value > 0; })) {
			    auto max_it = std::max_element(recognized.begin(), recognized.end());
			    emotion = emotions[std::distance(recognized.begin(), max_it)];
			}*/
			float maxValue = 0;
		        for (size_t i = 0; i < 5; ++i) {
		            if (recognized[i] > maxValue) {
		                maxValue = recognized[i];
		                emotion = emotions[i];
		            }
		        }
			handleDetectedEmotion(emotion);
		}
	}

}

void EmotionDetection::handleDetectedEmotion(const std::string& emotion) {
        std::cout << "Detected emotion: " << emotion << std::endl;
	std::string phrase= "You are " + emotion; 
	speech.say(phrase);
	behavior->executeAnimation(emotion);
}

void EmotionDetection::searchForFace(){
	//
}


