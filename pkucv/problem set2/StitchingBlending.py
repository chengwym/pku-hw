from array import array
from email.mime import image
import numpy as np
import random
import cv2
from BQ2 import better_matrix
from tqdm import tqdm

from LocalFeatureMatch import feature_matching

###########################################################################
#                            MY FUNCTION FOR Q4                           #
###########################################################################

def compute_homography(pixels_1, pixels_2):
    """
    compute the homo_matrix from image_1 to image_2 using the SVD method
    """
    n = len(pixels_1)
    A = []
    for i in range(n):
        x, y = pixels_1[i]
        u, v = pixels_2[i]
        A.append([-x, -y, -1, 0, 0, 0, x*u, y*u, u])
        A.append([0, 0, 0, -x, -y, -1, x*v, y*v, v])
    A = np.array(A)
    U, S, Vt = np.linalg.svd(A)
    homo_matrix = Vt[-1, :].reshape(3,3)
    homo_matrix = homo_matrix / homo_matrix[2, 2]
    return homo_matrix

def enclidean_distance(pixel_1, pixel_2):
    """
    the helper function to compute the error
    """
    return np.sqrt((pixel_1[0]-pixel_2[0])**2 + (pixel_1[1]-pixel_2[1])**2)


def align_pair(pixels_1, pixels_2):
    """
    using RANSAC to find the homography with the most inliers
    """
    iters = 10
    total = 0
    threshold = 1e-3
    est_homo = np.zeros((3, 3))

    size = len(pixels_1)
    index_list = range(size)
    for _ in range(iters):
        sample_index = random.sample(index_list, 4)
        forcompute_pixels_1 = []
        forcompute_pixels_2 = []
        for index in sample_index:
            forcompute_pixels_1.append(pixels_1[index])
            forcompute_pixels_2.append(pixels_2[index])
        h = compute_homography(forcompute_pixels_1, forcompute_pixels_2)
        
        total_inliers = 0

        for index in range(size):
            
            pixel_input = pixels_1[index]
            pixel_output = h.dot(np.array([pixel_input[0], pixel_input[1], 1]))
            estimate_pixel = (pixel_output[0]/pixel_output[2], pixel_output[1]/pixel_output[2])
            if enclidean_distance(estimate_pixel, pixels_2[index]) < threshold:
                total_inliers += 1

        if total_inliers > total:
            est_homo = h
            total = total_inliers
    return est_homo

def stitch_blend(img0, img1, h_matrix):
    '''
    this takes two images and the homography matrix from 0 to 1 and combines the images together!
    the logic is convoluted here and needs to be simplified!
    '''
    points0 = np.array(
        [[0, 0], [0, img0.shape[0]], [img0.shape[1], img0.shape[0]], [img0.shape[1], 0]],
        dtype=np.float32)
    points0 = points0.reshape((-1, 1, 2))
    points1 = np.array(
        [[0, 0], [0, img1.shape[0]], [img1.shape[1], img1.shape[0]], [img1.shape[1], 0]],
        dtype=np.float32)
    points1 = points1.reshape((-1, 1, 2))
    points2 = cv2.perspectiveTransform(points1, h_matrix)
    points = np.concatenate((points0, points2), axis=0)

    [x_min, y_min] = (points.min(axis=0).ravel() - 0.5).astype(np.int32)
    [x_max, y_max] = (points.max(axis=0).ravel() + 0.5).astype(np.int32)

    h_translation = np.array([[1, 0, -x_min], [0, 1, -y_min], [0, 0, 1]])

    output_img = cv2.warpPerspective(img1, h_translation.dot(h_matrix),
                                     (x_max - x_min, y_max - y_min))
    output_img[-y_min:img0.shape[0] - y_min, -x_min:img0.shape[1] - x_min] = img0
    cv2.imwrite('blend_id.png', output_img)
    return output_img


def generate_panorama(images):
    """
    generate a panorama from a sequence of images
    """
    count = len(images)
    left_idx = (count - 1) // 2
    right_idx = (int)(left_idx + 1)
    left_img = images[left_idx]
    right_img = images[right_idx]
    for i in range(left_idx-1, -1, -1):
        img = images[i]
        h_matrix = better_matrix(img, left_img)
        left_img = stitch_blend(img, left_img, h_matrix)
    for i in range(right_idx+1, count):
        img = images[i]
        h_matrix = better_matrix(img, left_img)
        left_img = stitch_blend(img, left_img, h_matrix)
    h = better_matrix(left_img, right_img)
    result_img = stitch_blend(left_img, right_img, h)
    cv2.imwrite('panoroma_id.png', result_img)