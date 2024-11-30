import os
import cv2
import hashlib
import yaml
from pathlib import Path
from random import sample
import textwrap
import progressbar

#incercam sa obtinem keypoints din datasetul fer, iar dupa
#impachetama datele in formatul citit de robot

face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

dataset_path = r''
output_image_path = ''
output_yaml_path = ''

Path(output_image_path).mkdir(parents=True, exist_ok=True)
Path(output_yaml_path).mkdir(parents=True, exist_ok=True)

target_resolution = (320, 240)

images_per_folder = 500

orb = cv2.ORB_create(100)
progress = 0
noface = 0
face = 0

bar = progressbar.ProgressBar(maxval=3500).start()

for emotion_folder in os.listdir(dataset_path):

    emotion_folder_path = os.path.join(dataset_path, emotion_folder)
    if not os.path.isdir(emotion_folder_path):
        continue

    all_images = os.listdir(emotion_folder_path)
    selected_images = sample(all_images, min(images_per_folder, len(all_images)))
    #selected_images = all_images

    for img_name in selected_images:
        img_path = os.path.join(emotion_folder_path, img_name)

        img = cv2.imread(img_path)
        if img is None:
            print(f"Error reading {img_path}")
            continue
        resized_img = cv2.resize(img, target_resolution)

        #gray = cv2.cvtColor(resized_img, cv2.COLOR_BGR2GRAY)

        faces = face_cascade.detectMultiScale(resized_img, scaleFactor=1.1, minNeighbors=5)

        if len(faces) == 0:
           # print(f"No face detected in {img_name}")
            progress+=1
            noface+=1
            continue

        for (x, y, w, h) in faces:
            face_roi = resized_img[y:y + h, x:x + w]
            face+=1
            boundary_points = [x, y, x + w, y, x + w, y + h, x, y + h]

            # bp = [(x, y), (x + w, y), (x + w, y + h), (x, y + h)]
            #
            # boundary_points = list(
            #     sum(
            #         [(int(x), int(y))
            #          for (x, y)
            #          in bp],
            #         ()
            #     )
            # )

            kps, des = orb.detectAndCompute(face_roi, None)

            keypoints = ["{:e}".format(coord) for kp in kps for coord in kp.pt]

            if des is not None:
                descriptors = list(int(x) for des1 in des for x in des1)
            else:
                descriptors = []


            hash_name = hashlib.md5(img_name.encode()).hexdigest()
            output_img_name = f"{hash_name}.0.png"
            output_img_path = os.path.join(output_image_path, output_img_name)

            cv2.imwrite(output_img_path, resized_img)
            
            yaml_content = textwrap.dedent(f"""\
            %YAML:1.0
            ---
            version_vision_reco: 1
            name: {emotion_folder}
            hash: "{hash_name}"
            tags: [{emotion_folder}, front]
            meta_ids:
              ids: [0, 8]
              labels: [front, {emotion_folder}]
            image_analyses:
               - 
                image_name: "{output_img_name}"
                original_name: "{output_img_name}"
                boundary_points: [{', '.join(map(str, boundary_points))}]
                keypoints: [{', '.join(keypoints)}]
                descriptors:
                  rows: {len(des) if des is not None else 0}
                  cols: {des.shape[1] if des is not None else 0}
                  dt: u
                  data: [{', '.join(map(str, descriptors))}]
            """)

            yaml_file_name = f"{hash_name}.yaml"
            output_yaml_path_full = os.path.join(output_yaml_path, yaml_file_name)
            with open(output_yaml_path_full, 'w') as yaml_file:
                yaml_file.write(yaml_content)

            #print(f"Processed and saved: {output_img_path} with YAML metadata {output_yaml_path_full}")
            if(progress<3500):
                bar.update(progress)
                progress=progress+1



print(f"Total found faces: {face}, Total unfound faces: {noface}")
