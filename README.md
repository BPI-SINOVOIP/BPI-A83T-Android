 A83T Android 5.1 Source code

----------

1. cd liche

   $ ./build.sh config          

Welcome to mkscript setup progress
All available chips:
   0. sun8iw1p1
   1. sun8iw3p1
   2. sun8iw5p1
   3. sun8iw6p1
   4. sun8iw7p1
   5. sun8iw8p1
   6. sun8iw9p1
   7. sun9iw1p1
 
Choice: 3


All available platforms:
   0. android
   1. dragonboard
   2. linux
 
Choice: 0


All available kernel:
   0. linux-3.4
 
Choice: 0


All available boards:
   0. f1
   1. fpga
   2. perf1_v1_0
   3. perf2_v1_0
   4. perf3_v1_0
   5. qc

Choice: 0


1.2
   $ ./build.sh 


**Build android** 

2.0 cd ../android 


2.1
   $source build/envsetup.sh
   
   
2.2
   $lunch    //(octopus_f1-eng)
   
   
2.3
   $extract-bsp
   
   
2.4
   $make -j4
   
   
2.5
   $pack
   
   









