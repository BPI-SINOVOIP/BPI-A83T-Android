$(call inherit-product, build/target/product/full_base.mk)
$(call inherit-product, device/softwinner/octopus-common/octopus-common.mk)
$(call inherit-product-if-exists, device/softwinner/bpi-m3-hdmi/modules/modules.mk)

DEVICE_PACKAGE_OVERLAYS := device/softwinner/bpi-m3-hdmi/overlay \
                           $(DEVICE_PACKAGE_OVERLAYS)

PRODUCT_PACKAGES += gatord


PRODUCT_PACKAGES += \
    ESFileExplorer \
    VideoPlayer \
    Bluetooth 


PRODUCT_COPY_FILES += \
    device/softwinner/bpi-m3-hdmi/kernel:kernel \
    device/softwinner/bpi-m3-hdmi/fstab.sun8i:root/fstab.sun8i \
    device/softwinner/bpi-m3-hdmi/init.sun8i.rc:root/init.sun8i.rc \
    device/softwinner/bpi-m3-hdmi/init.recovery.sun8i.rc:root/init.recovery.sun8i.rc \
    device/softwinner/bpi-m3-hdmi/ueventd.sun8i.rc:root/ueventd.sun8i.rc \
    device/softwinner/bpi-m3-hdmi/recovery.fstab:recovery.fstab \
    device/softwinner/bpi-m3-hdmi/modules/modules/nand.ko:root/nand.ko \
    device/softwinner/bpi-m3-hdmi/modules/modules/sunxi_tr.ko:root/sunxi_tr.ko \
    device/softwinner/bpi-m3-hdmi/modules/modules/disp.ko:root/disp.ko \
    device/softwinner/bpi-m3-hdmi/modules/modules/sw-device.ko:obj/sw-device.ko \
    device/softwinner/bpi-m3-hdmi/modules/modules/gt9xxnew_ts.ko:obj/gt9xxnew_ts.ko \

PRODUCT_COPY_FILES += \
    device/softwinner/bpi-m3-hdmi/configs/camera.cfg:system/etc/camera.cfg \
    device/softwinner/bpi-m3-hdmi/configs/cfg-Gallery2.xml:system/etc/cfg-Gallery2.xml \
    device/softwinner/bpi-m3-hdmi/configs/gsensor.cfg:system/usr/gsensor.cfg \
    device/softwinner/bpi-m3-hdmi/configs/media_profiles.xml:system/etc/media_profiles.xml \
    device/softwinner/bpi-m3-hdmi/configs/sunxi-keyboard.kl:system/usr/keylayout/sunxi-keyboard.kl \
	device/softwinner/bpi-m3-hdmi/configs/sunxi-ir.kl:system/usr/keylayout/sunxi-ir.kl \
    device/softwinner/bpi-m3-hdmi/configs/tp.idc:system/usr/idc/tp.idc

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.ethernet.xml:system/etc/permissions/android.hardware.ethernet.xml


PRODUCT_COPY_FILES += \
    device/softwinner/bpi-m3-hdmi/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml   \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml

# Low mem(memory <= 512M) device should not copy android.software.managed_users.xml
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.software.managed_users.xml:system/etc/permissions/android.software.managed_users.xml

PRODUCT_COPY_FILES += \
	device/softwinner/bpi-m3-hdmi/media/bootanimation.zip:system/media/bootanimation.zip \

#camera config for camera detector
PRODUCT_COPY_FILES += \
        device/softwinner/bpi-m3-hdmi/hawkview/sensor_list_cfg.ini:system/etc/hawkview/sensor_list_cfg.ini

# Radio Packages and Configuration Flie
$(call inherit-product, device/softwinner/common/rild/radio_common.mk)
#$(call inherit-product, device/softwinner/common/ril_modem/huawei/mu509/huawei_mu509.mk)
#$(call inherit-product, device/softwinner/common/ril_modem/Oviphone/em55/oviphone_em55.mk)

# Justin Porting 20151026 Start  
# camera config for isp
PRODUCT_COPY_FILES += \
    device/softwinner/octopus-common/hawkview/8M/ov8865_4lane/isp_3a_param.ini:system/etc/hawkview/ov8865_4lane/isp_3a_param.ini \
   device/softwinner/octopus-common/hawkview/8M/ov8865_4lane/isp_iso_param.ini:system/etc/hawkview/ov8865_4lane/isp_iso_param.ini \
    device/softwinner/octopus-common/hawkview/8M/ov8865_4lane/isp_test_param.ini:system/etc/hawkview/ov8865_4lane/isp_test_param.ini \
   device/softwinner/octopus-common/hawkview/8M/ov8865_4lane/isp_tuning_param.ini:system/etc/hawkview/ov8865_4lane/isp_tuning_param.ini \
   device/softwinner/octopus-common/hawkview/8M/ov8865_4lane/bin/gamma_tbl.bin:system/etc/hawkview/ov8865_4lane/bin/gamma_tbl.bin \
   device/softwinner/octopus-common/hawkview/8M/ov8865_4lane/bin/hdr_tbl.bin:system/etc/hawkview/ov8865_4lane/bin/hdr_tbl.bin \
    device/softwinner/octopus-common/hawkview/8M/ov8865_4lane/bin/lsc_tbl.bin:system/etc/hawkview/ov8865_4lane/bin/lsc_tbl.bin

# Justin Porting 20151026 End


#sensor    
PRODUCT_COPY_FILES += \
	device/softwinner/bpi-m3-hdmi/sensor.sh:system/bin/sensor.sh
  
# gps
$(call inherit-product, device/softwinner/bpi-m3-hdmi/gps/gps.mk)

# usb
#Justin 20151005 Porting Start
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.usb.config=mtp,adb\
    ro.adb.secure=0 \
    ro.sys.mutedrm=true \
    rw.logger=0
#Justin 20151005 Porting End

PRODUCT_PROPERTY_OVERRIDES += \
    ro.product.firmware=v2.1rc3

PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.heapsize=512m \
    dalvik.vm.heapstartsize=8m \
    dalvik.vm.heapgrowthlimit=192m \
    dalvik.vm.heaptargetutilization=0.75 \
    dalvik.vm.heapminfree=2m \
    dalvik.vm.heapmaxfree=8m \
    ro.zygote.disable_gl_preload=true

#Justin 20161118 Porting Start
# BPI_M3 Set Density
# HDMI Display = ro.sf.lcd_density=160
# LCD  Display = ro.sf.lcd_density=120
PRODUCT_PROPERTY_OVERRIDES += \
    ro.property.tabletUI=false \
    ro.sf.lcd_density=160 \
#Justin 20161118 Porting End


# function
# Justin Porting 20151029 Start 
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sys.bootfast=true \
    ro.dmic.used=false
# Justin Porting 20151030 End


PRODUCT_PROPERTY_OVERRIDES += \
   	persist.sys.timezone=Asia/Taipei \
	persist.sys.language=EN \
	persist.sys.country=US



PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := tablet

$(call inherit-product-if-exists, vendor/google/products/gms_base.mk)

PRODUCT_BRAND := Allwinner
PRODUCT_NAME := bpi_m3_hdmi
PRODUCT_DEVICE := bpi-m3-hdmi
PRODUCT_MODEL :=  BPI-M3
PRODUCT_MANUFACTURER := Allwinner
