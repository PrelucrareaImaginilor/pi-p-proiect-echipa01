import os
import glob
import cv2
import numpy as np

from sklearn.svm import SVC
from sklearn.metrics import confusion_matrix, classification_report
from joblib import load  # or pickle

def extractHOGFeatures(img):
    hog = cv2.HOGDescriptor(
        _winSize=(48, 48),
        _blockSize=(16, 16),
        _blockStride=(8, 8),
        _cellSize=(8, 8),
        _nbins=9
    )
    descriptors = hog.compute(img)
    return descriptors.flatten()

def evaluateTestSet(testPaths, model, labelToEmotion):
    y_true = []
    y_pred = []


    unique_labels_sorted = sorted(labelToEmotion.keys())

    for folder, true_label in testPaths.items():
        image_paths = glob.glob(os.path.join(folder, "*.jpg"))
        for imgPath in image_paths:
            img = cv2.imread(imgPath, cv2.IMREAD_GRAYSCALE)
            if img is None:
                continue

            img = cv2.resize(img, (48, 48))

            features = extractHOGFeatures(img)
            features = np.array(features, dtype=np.float32)  # ensure float32

            features = features.reshape(1, -1)

            retval, results = model.predict(features)

            predicted_label = int(results[0, 0])

            y_true.append(true_label)
            y_pred.append(predicted_label)

    cm = confusion_matrix(y_true, y_pred, labels=unique_labels_sorted)

    print("=== Confusion Matrix (rows=true, cols=pred) ===")
    print(cm)


    target_names = [labelToEmotion[l] for l in unique_labels_sorted]
    print("\n=== Classification Report ===")
    print(classification_report(
        y_true,
        y_pred,
        labels=unique_labels_sorted,
        target_names=target_names,
        digits=4
    ))


if __name__ == "__main__":
    test_root = r"C:\Users\h8win\Documents\fer13Dataset\test"
    model_path = "emotion_svm_model.yml"
    model = cv2.ml.SVM_load(model_path)
    if not model:
        print(f"Could not load model from {model_path}")
        exit(1)

    labelToEmotion = {
        0: "angry",
        3: "happy",
        4: "sad",
        6: "neutral"
    }

    testPaths = {
        test_root + r"\angry":    0,
        test_root + r"\happy":    3,
        test_root + r"\sad":      4,
        test_root + r"\neutral":  6
    }

    evaluateTestSet(testPaths, model, labelToEmotion)
