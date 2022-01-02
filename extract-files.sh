#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

function blob_fixup() {
    case "${1}" in
        vendor/bin/sensors.qti.pine)
            sed -i 's|sensor_def_|pine___def_|g' "${2}"
            ;;
        vendor/bin/sensors.qti.olive)
            sed -i 's|sensor_def_|olive__def_|g' "${2}"
            ;;
        vendor/lib/libmmcamera2_sensor_modules.so)
            # Allow up to 0xFF CameraModuleConfig nodes on camera_config.xml
            sed -i -e 's|\x68\x1e\x15\x28|\x68\x1e\xff\x28|g' "${2}"
            PATTERN_FOUND=$(hexdump -ve '1/1 "%.2x"' "${2}" | grep -E -o "681eff28" | wc -l)
            if [ $PATTERN_FOUND != "1" ]; then
                echo "Critical blob modification weren't applied on ${2}!"
                exit;
            fi
            ;;
    esac
}

# If we're being sourced by the common script that we called,
# stop right here. No need to go down the rabbit hole.
if [ "${BASH_SOURCE[0]}" != "${0}" ]; then
    return
fi

set -e

export DEVICE=Mi439
export DEVICE_COMMON=mithorium-common
export VENDOR=xiaomi

"./../../${VENDOR}/${DEVICE_COMMON}/extract-files.sh" "$@"
