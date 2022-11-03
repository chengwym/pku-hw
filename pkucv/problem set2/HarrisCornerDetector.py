import numpy as np
import cv2
from scipy.signal import convolve

###########################################################################
#                            MY FUNCTION FOR Q1                           #
###########################################################################


def gradient_x(img):
    kernel_x = np.array([[-1, 0, 1]])
    grad_x = convolve(img, kernel_x, mode='same')
    return grad_x

def gradient_y(img):
    kernel_y = np.array([[-1], [0], [1]])
    grad_y = convolve(img, kernel_y, mode='same')
    return grad_y

def harris_response(img, alpha=0.04):
    I_x = gradient_x(img)
    I_y = gradient_y(img)
    gaussian_kernel = cv2.getGaussianKernel(2, 3)
    I_xx = convolve(I_x*I_x, gaussian_kernel, mode='same')
    I_yy = convolve(I_y*I_y, gaussian_kernel, mode='same')
    I_xy = convolve(I_x*I_y, gaussian_kernel, mode='same')
    
    detA = I_xx * I_yy - I_xy ** 2
    traceA = I_xx + I_yy

    R = detA - alpha * traceA ** 2
    
    return R

def corner_selection(R, threshold=None, min_distance=None):
    if threshold == None:
        threshold = 0.01*R.max()
    R = cv2.dilate(R,None)
    ret,R = cv2.threshold(R, threshold, 255, 0)
    R = np.uint8(R)
    ret,labels,stats,centroids = cv2.connectedComponentsWithStats(R)
    centroids = np.int0(centroids)
    pixels = []
    for point in centroids:
        pixels.append(tuple([point[1], point[0]]))
    return pixels

"""
the test module
"""
"""
img1 = cv2.imread('Problem2Images/1_1.jpg')
img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
img2 = cv2.imread('Problem2Images/1_2.jpg')
img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
cv2.imwrite('0.png', img1)
cv2.imwrite('4.png', img2)
R1 = harris_response(img1)
R2 = harris_response(img2)
pixels1 = corner_selection(R1)
pixels2 = corner_selection(R2)
for x, y in pixels1:
    img1[x, y] = 255
for x, y in pixels2:
    img2[x, y] = 255
cv2.imwrite('2.png', img1)
cv2.imwrite('5.png', img2)
"""