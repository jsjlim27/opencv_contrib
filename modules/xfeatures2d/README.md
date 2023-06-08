Extra 2D Features Framework
===========================


1. Experimental 2D feature algorithms
2. Non-free 2D feature algorithms


Extra 2D Features Framework containing experimental and non-free 2D feature detector/descriptor algorithms:
 SURF, BRIEF, Censure, Freak, LUCID, Daisy, BEBLID, TEBLID, Self-similar.


**Additional feature detector/descriptor algorithm in this fork: ASV-SIFT**


**src/asvsift.cpp** contains the implementation of ASV-SIFT, which is a descriptor proposed in this
[paper](https://www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Yang_Accumulated_Stability_Voting_CVPR_2016_paper.pdf).


The relevant header of the ASVSIFT class can be found in **include/opencv2/xfeatures2d.hpp**


### Implementation Overview of ASV-SIFT
I began my implementation of the ASVSIFT class with a copy of the [SIFT class in opencv_contrib's 3.7.1 release](https://github.com/opencv/opencv_contrib/blob/3.4.10/modules/xfeatures2d/src/sift.cpp).


Because Accumulated Stability Voting is a framework for obtaining a more robust descriptor, majority of the modification on the original SIFT class was made inside the later half of its **detectAndCompute()** method.


In ordinary SIFT::detectAndCompute(), the keypoints are detected and then passed to the **calcDescriptors()** method to obtain the SIFT descriptors at a particular scale.


ASVSIFT::detectAndCompute() however, first needs to obtain additional descriptor samples at multiple scales.


After building the descriptor samples at multiple scales, the ASV framework needs to obtain the *stability vectors* for all possible pairs of descriptor samples.


Next, we need to *quantize* these stability vectors into binary.


And finally, we need to tally all the stability votes in our binary stability vectors to obtain our ASV descriptors.
