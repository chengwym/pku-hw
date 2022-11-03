import cv2
import os
from StitchingBlending import generate_panorama, stitch_blend
from BQ2 import better_matrix
from BQ3 import panorama360

###########################################################################
#                            MY FUNCTION FOR Q5                           #
###########################################################################

def stitch_img_function(path1, path2):
    """
    the helper function to stitch image
    """
    img1 = cv2.imread(path1)
    img2 = cv2.imread(path2)
    h_matrix = better_matrix(img1, img2)
    stitch_blend(img1, img2, h_matrix)

def generate_panorama_function(path):
    """
    the helper function to generate panorama
    """
    img = []
    for img_name in os.listdir(path):
        image = cv2.imread(path+'/'+img_name)
        img.append(image)
    generate_panorama(img)

# stitch two images and save the panorama 'blend_id.png'
# stitch_img_function('Problem2Images/3_1.jpg', 'Problem2Images/3_2.jpg')

# the parameter is the folder name and save the panorama 'panorama_id.png'
# generate_panorama_function('MyImage/translation')