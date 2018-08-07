import numpy as np
import argparse
import cv2

points = np.zeros((4,2), np.float32)
count = 0

def getPoint(event, x, y, flags, param):
    global points, count
    if event == cv2.EVENT_LBUTTONDOWN:
        if count < 4 :
            cv2.circle(image, (x,y), 1, (0, 255, 0), -1)
            points[count,0] = x
            points[count,1] = y
            print(count, x, y)
            print(points)
            count = count + 1
    elif event  == cv2.EVENT_RBUTTONDOWN:
        if count > 0:
            points[count,0] = 0
            points[count,1] = 0
            print(count)
            print(points)
            count = count - 1

def getTransformedImage(img):
    src = points
    dst = np.array([[0, 0], [1140, 0], [1140, 720], [0, 720]], dtype="float32")
    M = cv2.getPerspectiveTransform(src, dst)
    warped = cv2.warpPerspective(img, M, (1140, 720))
    return warped

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, help="Path to the image")
ap.add_argument("-o", "--output", required=True, help="Image name to save")
args = vars(ap.parse_args())


image = cv2.imread(args["image"])
clone = image.copy()
cv2.namedWindow("image")
cv2.setMouseCallback("image", getPoint)

while True:
    cv2.imshow("image", image)
    key = cv2.waitKey(1)&0xFF

    if key == ord("t"):
        if count == 4:
            warped = getTransformedImage(clone)
            cv2.imshow("warped", warped)
            cv2.imwrite(args["output"], warped)
        else:
            print("Not enough points!!")
    elif key == ord("q"):
        break

cv2.destroyAllWindows()
