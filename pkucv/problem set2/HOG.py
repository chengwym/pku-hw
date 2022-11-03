from HarrisCornerDetector import corner_selection, gradient_x, gradient_y, harris_response
import numpy as np
import cv2

###########################################################################
#                            MY FUNCTION FOR Q2                           #
###########################################################################

def gradient_magnitude(grad_x, grad_y):
    """
    compute the magnitude
    """
    return np.sqrt(grad_x**2 + grad_y**2) 

def gradient_direction(grad_x, grad_y):
    """
    compute the degree
    """
    grad_x = np.float32(grad_x)
    grad_y = np.float32(grad_y)
    grad_x[grad_x==0] = 1e-5
    return np.arctan(grad_y / grad_x)

def cell_histogram(cell_direction, cell_magnitude):
    """
    compute the histogram of a cell
    """
    HOG_cell_hist, _ = np.histogram(cell_direction, bins=9, range=(-np.pi/2, np.pi/2), weights=cell_magnitude)
    HOG_cell_hist = np.float32(HOG_cell_hist)
    return HOG_cell_hist

def norm_block_feature(cell_histograms, epsilon):
    """
    normalize the block feature
    """
    feature = cell_histograms/np.sqrt(np.sum(np.square(cell_histograms)) + epsilon)
    return feature.tolist()

def histogram_of_gradients(img, pixels):
    hight = img.shape[0]
    width = img.shape[1]
    features = []
    
    cell_size = 8
    block_size = 2
    num_bins = 9

    grad_x = gradient_x(img)
    grad_y = gradient_y(img)
    grad_direction = gradient_direction(grad_x, grad_y)
    grad_magnitude = gradient_magnitude(grad_x, grad_y)

    max_h = int(((hight-8)/8)+1)
    max_w = int(((width-8)/8)+1)
    cell_array = []

    i = 0
    j = 0
    h = 0
    w = 0
    while i < max_h:
        w = 0
        j = 0
        while j < max_w:
            cell_dir = grad_direction[h: h+cell_size, w: w+cell_size]
            cell_mag = grad_magnitude[h: h+cell_size, w: w+cell_size]
            hist_val = cell_histogram(cell_dir, cell_mag)
            cell_array.append(hist_val)
            j += 1
            w += cell_size
        i += 1
        h += cell_size

    cell_array = np.reshape(cell_array, (max_h, max_w, num_bins))

    max_h = max_h - block_size + 1
    max_w = max_w - block_size + 1
    block_array = []

    i = 0
    j = 0
    h = 0
    w = 0

    while i < max_h:
        w = 0
        j = 0
        while j < max_w:
            cell_histograms = np.reshape(cell_array[h: h+block_size, w: w+block_size], -1)
            norm_cell_histograms = norm_block_feature(cell_histograms, 1e-5)
            block_array.append(norm_cell_histograms)
            j += 1
            w += 1
        i += 1
        h += 1

    N = len(pixels)

    for index in range(N):
        point = pixels[index]
        h = int(point[0]/8)
        w = int(point[1]/8)
        if h >= max_h:
            h = max_h-1
        if w >= max_w:
            w = max_w-1
        features.append(block_array[h*max_w+w])

    return features