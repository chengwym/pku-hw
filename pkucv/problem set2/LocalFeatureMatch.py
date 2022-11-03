from HarrisCornerDetector import corner_selection, harris_response
from HOG import histogram_of_gradients
import numpy as np
import cv2

###########################################################################
#                            MY FUNCTION FOR Q3                           #
###########################################################################

def feature_matching(img_1, img_2):
    R1 = harris_response(img_1)
    R2 = harris_response(img_2)
    interesting_point1 = corner_selection(R1)
    interesting_point2 = corner_selection(R2)
    features1 = histogram_of_gradients(img=img_1, pixels=interesting_point1)
    features2 = histogram_of_gradients(img=img_2, pixels=interesting_point2)
    features1 = np.array(features1)
    features2 = np.array(features2)
    len1 = len(features1)
    len2 = len(features2)
    
    pixels_1 = []
    pixels_2 = []

    ratio_threshold = 0.5

    if len1 < len2:
        for i in range(len1):
            distance = np.sqrt(np.sum((features1[i]-features2)**2, axis=1))
            index = np.argsort(distance)
            ratio = distance[index[0]] / distance[index[1]]
            if ratio < ratio_threshold:
                pixels_1.append(interesting_point1[i])
                pixels_2.append(interesting_point2[index[0]])
    else:
        for i in range(len2):
            distance = np.sqrt(np.sum((features2[i]-features1)**2, axis=1))
            index = np.argsort(distance)
            ratio = distance[index[0]] / distance[index[1]]
            if ratio > ratio_threshold:
                pixels_1.append(interesting_point1[index[0]])
                pixels_2.append(interesting_point2[i])

    return pixels_1, pixels_2
"""
the test module
img1 = cv2.imread('Problem2Images/1_1.jpg')
img2 = cv2.imread('Problem2Images/1_2.jpg')
img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
pixels1, pixels2 = feature_matching(img1, img2)
for x, y in pixels1:
    img1[x, y] = 255
for x, y in pixels2:
    img2[x, y] = 255
cv2.imwrite('0.png', img1)
cv2.imwrite('1.png', img2)
"""