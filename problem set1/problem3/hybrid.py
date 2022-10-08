import numpy as np
from PIL import Image

"""""""""""""""""""""""""""""""""""""""""""""
    the six functions for the problem set1 
"""""""""""""""""""""""""""""""""""""""""""""

def cross_correlation_2d(image, kernal):
    """
    Cross-correlate the image

    Inputs:
    - image: A numpy array of shape (W, H, D) represents the image to be processed
    - kernal: A numpy array of shape (w, h)
    - stride: default 1

    Returns:
    - cross_correlation_image: A numpy array which has the same shape of the image, represents
    the image which has been cross-correlated.
    """
    import time
    tic = time.time()

    W = image.shape[0]
    H = image.shape[1]
    D = image.shape[2]
    w = kernal.shape[0]
    h = kernal.shape[1]
    _w = int((w-1)/2)
    _h = int((h-1)/2)

    img = np.zeros([W+2*_w, H+2*_h, D])
    img[_w:W+_w, _h:H+_h, :] = image
    W = img.shape[0]
    H = img.shape[1]
    
    cross_correlation_image = np.zeros(shape=image.shape)
    
    for d in range(D):
        for i in range(_w, W-_w, 1):
            for j in range(_h, H-_h, 1):
                cross_correlation_image[i-_w, j-_h, d] = np.sum(kernal * img[i-_w:i+_w+1,j-_h:j+_h+1,d])

    toc = time.time()
    print('the process took %f seconds' %(toc-tic))
    return cross_correlation_image

def convolve_2(image, kernal):
    """
    use cross-correlate method to convolve the image

    Inputs:
    - image: A numpy array represented the image to convolve
    - kernal: A numpy array
    - stride: default 1

    Returns:
    - convolve_img: A numpy array has the same shape of the image, representing the image that has been convolved
    """
    convolve_img = cross_correlation_2d(image=image, kernal=kernal.T)
    return convolve_img

def gaussian_blur_kernal_2d(width, height, sigma):
    """
    generate a guassian kernal

    Inputs:
    - width: A integer, the width of the kernal
    - height: A integer, the height of the kernal
    - sigma: A float gives the standard deviation of the kernal

    Returns:
    - gaussian_kernal: A numpy array of shape(width, height) 
    """
    if sigma == 0:
        raise NameError("sigma can not be 0")
    x = np.linspace(-1, 1, width)
    y = np.linspace(-1, 1, height)
    x, y = np.meshgrid(x, y)
    gaussian_kernal = 1/(2*np.pi*sigma**2) * (np.exp(-((x**2)+(y**2))/(2*sigma**2)))
    gaussian_kernal = gaussian_kernal / np.sum(gaussian_kernal)
    return gaussian_kernal

def low_pass(image, sigma):
    """
    use gaussian_blur to implement low pass filter

    Inputs:
    - image: A numpy array represented the image to be processed
    - sigma: A hyperparameter controls the gaussian blur

    Returns:
    - low_pass_img: A numpy array has the same shape of the img, represents the processed image
    """
    w, h = generate_kernalShape(image.shape[0], image.shape[1])
    gaussian_kernal = gaussian_blur_kernal_2d(width=w, height=h, sigma=sigma)
    low_pass_img = convolve_2(image=image, kernal=gaussian_kernal)
    return low_pass_img

def high_pass(image, sigma):
    """
    use gaussian_blur to implement high pass filter

    Inputs:
    - image: A numpy array represented the image to be processed
    - sigma: A hyperparameter controls the gaussian blur

    Returns:
    - high_pass_img: A numpy array has the same shape of the img, represents the processed image
    """
    w, h = generate_kernalShape(image.shape[0], image.shape[1])
    gaussian_kernal = gaussian_blur_kernal_2d(width=w, height=h, sigma=sigma)
    high_pass_img = convolve_2(image=image, kernal=gaussian_kernal)
    high_pass_img = image - high_pass_img + 128
    return high_pass_img

def hybrid_image(left_image, right_image, sigma1, sigma2):
    """
    hybrid the low frequence image and high frequence image

    Inputs:
    - left_image: A numpy array represents the low frequence image
    - right_image: A numpy array represents the high frequence image
    - sigma1: the low cutoff frequence
    - sigma2: the high cutoff frequence

    Returns:
    - hybrid_img: A numpy array represents the hybrid image
    """
    low_image = low_pass(image=left_image, sigma=sigma1)
    get_image(low_image, '1.png')
    high_image = high_pass(image=right_image, sigma=sigma2)
    get_image(high_image, '2.png')
    hybrid_image = (low_image + high_image) / 2
    return hybrid_image

"""""""""""""""""""""""""""""""""""""""""""""
            the helper functions 
"""""""""""""""""""""""""""""""""""""""""""""

def get_image(image, str):
    Image.fromarray(image.astype(np.uint8)).save(str)

def generate_kernalShape(W, H):
    w = (int)(W / 20)
    h = (int)(H / 20)
    if w <= 5:
        w = 5
    if h <= 5:
        h = 5
    w += (w%2 == 0)
    h += (h%2 == 0)
    return w, h

"""""""""""""""""""""""""""""""""""""""""""""
                hybrid images
"""""""""""""""""""""""""""""""""""""""""""""

left_img = np.array(Image.open('./left_id.png'))
right_img = np.array(Image.open('./right_id.png'))

hybrid_img1 = hybrid_image(left_img, right_img, 100, 100)

get_image(hybrid_img1, './hybrid.png')