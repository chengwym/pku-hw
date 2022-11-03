import cv2
import numpy as np

###########################################################################
#                            MY FUNCTION FOR BQ2                          #
###########################################################################

def compute_matches(features0, features1, matcher, knn=5, lowe=0.7):
    """
    the better method which use the knn algorithm to compute the match points
    """
    keypoints0, descriptors0 = features0
    keypoints1, descriptors1 = features1
    matches = matcher.knnMatch(np.asarray(descriptors0, np.float32), np.asarray(descriptors1, np.float32), k=knn)

    positive = []
    for match0, match1 in matches:
        if match0.distance < lowe * match1.distance:
            positive.append(match0)
    src_pts = np.array([keypoints0[good_match.queryIdx].pt for good_match in positive],
                          dtype=np.float32)
    src_pts = src_pts.reshape((-1, 1, 2))
    dst_pts = np.array([keypoints1[good_match.trainIdx].pt for good_match in positive],
                          dtype=np.float32)
    dst_pts = dst_pts.reshape((-1, 1, 2))
    return src_pts, dst_pts, len(positive)

def better_matrix(img_1, img_2):
    """
    the better performence use the SIFT to detect the interesting points
    """
    result_features = cv2.SIFT_create().detectAndCompute(img_2, None)
    image_features = cv2.SIFT_create().detectAndCompute(img_1, None)
    matches_src, matches_dst, n_matches = compute_matches(result_features,
    image_features,
    matcher=cv2.FlannBasedMatcher({'algorithm': 0, 'trees': 5}, {'checks': 50}),
    knn=2, lowe=1)
    homography, _ = cv2.findHomography(matches_src, matches_dst, cv2.RANSAC, 5.0)
    return homography