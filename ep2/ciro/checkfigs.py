import numpy as np
from skimage import io
import os

# get seq figure and show
img_seq = io.imread("0.ppm")
io.imshow(img_seq)
io.show()

# read all ppm files in folder
files = os.listdir()
ppm_files = [f for f in files if ".ppm" in f]

# compare with seq
match_with_seq = []
for f in ppm_files:
    img = io.imread(f)
    check = np.all(img == img_seq)
    match_with_seq.append(check)

# check if all match with seq figure
print(np.all(match_with_seq))