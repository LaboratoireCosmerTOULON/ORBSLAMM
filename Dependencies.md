##List of Known Dependencies
###ORBSLAMM on top of ORB-SLAM2 version 1.0

In this document we list all the pieces of code included  by ORBSLAMM and ORB-SLAM2 and linked libraries which are not property of the authors of ORBSLAMM or ORB-SLAM2.


#####Code in **src** and **include** folders

* *ORBextractor.cc*.
This is a modified version of orb.cpp of OpenCV library. The original code is BSD licensed.

* *PnPsolver.h, PnPsolver.cc*.
This is a modified version of the epnp.h and epnp.cc of Vincent Lepetit. 
This code can be found in popular BSD licensed computer vision libraries as [OpenCV](https://github.com/Itseez/opencv/blob/master/modules/calib3d/src/epnp.cpp) and [OpenGV](https://github.com/laurentkneip/opengv/blob/master/src/absolute_pose/modules/Epnp.cpp). The original code is FreeBSD.

* Function *ORBmatcher::DescriptorDistance* in *ORBmatcher.cc*.
The code is from: http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel.
The code is in the public domain.

#####Code in Thirdparty folder

* All code in **DBoW2** folder.
This is a modified version of [DBoW2](https://github.com/dorian3d/DBoW2) and [DLib](https://github.com/dorian3d/DLib) library. All files included are BSD licensed.

* All code in **g2o** folder.
This is a modified version of [g2o](https://github.com/RainerKuemmerle/g2o). All files included are BSD licensed.

* All code in **v4l2loopback** folder.
This is a modified version of [v4l2loopback](https://github.com/umlaeute/v4l2loopback). All files included are BSD licensed.

#####Library dependencies 

* **Pangolin (visualization and user interface)**.
https://github.com/stevenlovegrove/Pangolin

* **OpenCV**.
BSD licensed.

* **Eigen3**.
For versions greater than 3.1.1 is MPL2, earlier versions are LGPLv3.

* **BLAS** (required by g2o).
[Freely-available software](http://www.netlib.org/blas/#_licensing).

* **LAPACK**(required by g2o).
BSD licensed. 

* **ROS (Optional, only if you build Examples/ROS)**.
BSD licensed. In the manifest.xml the only declared package dependencies are roscpp, tf, sensor_msgs, image_transport, cv_bridge, which are all BSD licensed.

* **Opencv-yuv**
[Freely-available tool](https://github.com/mpenkov/opencv-yuv) used when running SLAMM with Bebop to load YUV videos with OpenCV

Updated: 23/01/2016 Raúl Mur Artal
Updated: 04/04/2018 Hayyan Afeef Daoud
