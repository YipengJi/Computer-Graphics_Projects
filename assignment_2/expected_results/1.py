import cv2
import matplotlib.pyplot as plt

img1 = cv2.imread("cylinders.png",1)
img2 = cv2.imread("../../../../Downloads/cylinders.png",1)


img3 = img1-img2

cv2.imshow("image",img3)
cv2.waitKey(0)
cv2.destroyAllWindows()

