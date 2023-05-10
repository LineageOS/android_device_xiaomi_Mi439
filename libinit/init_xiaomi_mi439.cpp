/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <libinit_dalvik_heap.h>
#include <libinit_variant.h>

#include "vendor_init.h"

#include <android-base/file.h>

static const variant_info_t pine_info = {
    .brand = "Xiaomi",
    .device = "pine",
    .marketname = "",
    .model = "Redmi 7A",
    .build_fingerprint = "Xiaomi/pine/pine:10/QKQ1.191014.001/V12.5.1.0.QCMMIXM:user/release-keys",
};

static const variant_info_t olive_info = {
    .brand = "Xiaomi",
    .device = "olive",
    .marketname = "",
    .model = "Redmi 8",
    .build_fingerprint = "Xiaomi/olive/olive:10/QKQ1.191014.001/V12.5.1.0.QCNMIXM:user/release-keys",
};

static const variant_info_t olivelite_info = {
    .brand = "Xiaomi",
    .device = "olivelite",
    .marketname = "",
    .model = "Redmi 8A",
    .build_fingerprint = "Xiaomi/olive/olive:10/QKQ1.191014.001/V12.5.1.0.QCNMIXM:user/release-keys",
};

static const variant_info_t olivewood_info = {
    .brand = "Xiaomi",
    .device = "olivewood",
    .marketname = "",
    .model = "Redmi 8A Dual",
    .build_fingerprint = "Xiaomi/olive/olive:10/QKQ1.191014.001/V12.5.1.0.QCNMIXM:user/release-keys",
};

static void determine_device()
{
    std::string fdt_model, mach_codename;
    android::base::ReadFileToString("/sys/firmware/devicetree/base/model", &fdt_model, true);
    if (fdt_model.find("PINE QRD") != fdt_model.npos) {
        set_variant_props(pine_info);
    } else if (fdt_model.find("Olive QRD") != fdt_model.npos) {
        android::base::ReadFileToString("/sys/xiaomi-sdm439-mach/codename", &mach_codename, true);
        mach_codename.pop_back();
        if (mach_codename == "olivelite")
            set_variant_props(olivelite_info);
        else if (mach_codename == "olivewood")
            set_variant_props(olivewood_info);
        else
            set_variant_props(olive_info);
    }
}

void vendor_load_properties() {
    determine_device();
    set_dalvik_heap();
}
