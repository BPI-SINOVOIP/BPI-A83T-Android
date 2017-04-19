# BPI-M3 Android 5.1 Source code
----------
1 Build Android BSP

 $ cd lichee
 
 $ ./build.sh config       

Welcome to mkscript setup progress
All available chips:
   1. sun8iw1p1
   2. sun8iw3p1
   3. sun8iw5p1
   4. sun8iw6p1
   5. sun8iw7p1
   6. sun8iw8p1
   7. sun8iw9p1
   8. sun9iw1p1
 
Choice: 4


All available platforms:
   1. android
   2. dragonboard
   3. linux
 
Choice: 1


All available kernel:
   1. linux-3.4
 
Choice: 1


All available boards:
   1. f1
   2. fpga
   3. perf1_v1_0
   4. perf2_v1_0
   5. perf3_v1_0
   6. qc

Choice: 1

   $ ./build.sh 

***********

2 Build Android 

   $cd ../android

   $source build/envsetup.sh
   
   $lunch    //(octopus_bpi_m3-eng)
   
   $extract-bsp
   
   $make -j8
   
   $pack
   
   
   
   









