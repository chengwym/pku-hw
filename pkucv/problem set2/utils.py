# coding: utf-8

import os
import sys
import cv2
import math
import numpy as np
import matplotlib.pyplot as plt


def load_images(source_dir):
    imgs = []
    
    for (dirpath, dirnames, filenames) in os.walk(source_dir):
        imgs.extend([filename for filename in filenames if filename.endswith('.jpg') or filename.endswith('.png')])
        break
    
    image_list = [cv2.imread(os.path.join(source_dir, img), 1) for img in imgs]

    return image_list


def parse(source_dir):
    """
    Parse the input dir.
    Args:
        source_dir: the dir that have images and a 'image_list.txt'
    Returns:
        A image array and focal_length array
    """
    focal_length = []
    filenames = []
    img = []
    for filename in os.listdir(source_dir):
        filenames += [filename]
        focal_length += [800]
    img_list = [cv2.imread(os.path.join(source_dir, f), 1) for f in filenames]
    return (img_list, focal_length)


def cylindrical_projection(img, focal_length):
    """
    Project image to cylinder

    Args:
        img: input image
        focal_length: input image's focal length

    Return:
        Cylindrical projection of input image
    """
    height, width, _ = img.shape
    cylinder_proj = np.zeros(shape=img.shape, dtype=np.uint8)
    
    for y in range(-int(height/2), int(height/2)):
        for x in range(-int(width/2), int(width/2)):
            cylinder_x = focal_length*math.atan(x/focal_length)
            cylinder_y = focal_length*y/math.sqrt(x**2+focal_length**2)
            
            cylinder_x = round(cylinder_x + width/2)
            cylinder_y = round(cylinder_y + height/2)

            if cylinder_x >= 0 and cylinder_x < width and cylinder_y >= 0 and cylinder_y < height:
                cylinder_proj[cylinder_y][cylinder_x] = img[y+int(height/2)][x+int(width/2)]
    
    _, thresh = cv2.threshold(cv2.cvtColor(cylinder_proj, cv2.COLOR_BGR2GRAY), 1, 255, cv2.THRESH_BINARY)
    x, y, w, h = cv2.boundingRect(thresh)
        
    return cylinder_proj[y:y+h, x:x+w]

def matched_pairs_plot(p1, p2, mp):
    _, offset, _ = p1.shape
    plt_img = np.concatenate((p1, p2), axis=1)
    plt.figure(figsize=(10,10))
    plt.imshow(plt_img)
    for i in range(len(mp)):
        plt.scatter(x=mp[i][0][1], y=mp[i][0][0], c='r')
        plt.plot([mp[i][0][1], offset+mp[i][1][1]], [mp[i][0][0], mp[i][1][0]], 'y-', lw=1)
        plt.scatter(x=offset+mp[i][1][1], y=mp[i][1][0], c='b')
    plt.show()
    cv2.waitKey(0)