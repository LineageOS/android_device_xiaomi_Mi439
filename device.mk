#
# Copyright (C) 2021 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from sdm439-common
$(call inherit-product, device/xiaomi/sdm439-common/sdm439.mk)

# Overlays
DEVICE_PACKAGE_OVERLAYS += \
    $(LOCAL_PATH)/overlay \
    $(LOCAL_PATH)/overlay-lineage

# Boot animation
TARGET_SCREEN_HEIGHT := 1440
TARGET_SCREEN_WIDTH := 720

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.consumerir.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.consumerir.xml \
    frameworks/native/data/etc/android.hardware.fingerprint.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.fingerprint.xml

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/permissions/blank.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/blank.xml

# Audio
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/audio/audio_platform_info.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio_platform_info.xml \
    $(LOCAL_PATH)/audio/mixer_paths_sdm439_pm8953.xml:$(TARGET_COPY_OUT_VENDOR)/etc/mixer_paths_sdm439_pm8953.xml

# Consumer IR
PRODUCT_PACKAGES += \
    android.hardware.ir@1.0-impl \
    android.hardware.ir@1.0-service

# Fingerprint
PRODUCT_PACKAGES += \
    android.hardware.biometrics.fingerprint@2.1-service.xiaomi_mi439 \
    android.hardware.biometrics.fingerprint@2.2

# Rootdir
PRODUCT_PACKAGES += \
    init.xiaomi.device.rc

# Sensors
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/sensors/sensor_def_qcomdev.conf:$(TARGET_COPY_OUT_VENDOR)/etc/sensors/sensor_def_qcomdev.conf

# Soong namespaces
PRODUCT_SOONG_NAMESPACES += \
    $(LOCAL_PATH)

# Inherit from vendor blobs
$(call inherit-product, vendor/xiaomi/mi439/mi439-vendor.mk)
