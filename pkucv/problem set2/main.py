from StitchingBlending import stitch_blend
from BQ2 import better_matrix
from BQ3 import panorama360
import cv2
import argparse

###########################################################################
#                            THE FUNCTION FOR TA                          #
###########################################################################
def TA_panorama(inpute_dir):
    """
    inpute_dir: a folder contain a set of images to be stitch to a panorama.
    """
    panorama360(inpute_dir)

def TA_stich_two_image(path1, path2):
    """
    path1: the path of the image1
    path2: the path of the image2
    """
    img1 = cv2.imread(path1)
    img2 = cv2.imread(path2)
    h_matrix = better_matrix(img1, img2)
    stitch_blend(img1, img2, h_matrix)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-f")
    parser.add_argument("-i1")
    parser.add_argument("-i2")
    args = parser.parse_args()

    if args.f != None:
        TA_panorama(args.f)
    if args.i1 != None and args.i2 != None:
        TA_stich_two_image(args.i1, args.i2)