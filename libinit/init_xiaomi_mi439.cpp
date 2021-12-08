/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <libinit_dalvik_heap.h>
#include <libinit_utils.h>
#include <libinit_variant.h>

#include "vendor_init.h"

#include <android-base/file.h>
#include <filesystem>

using std::filesystem::directory_iterator;
using std::filesystem::filesystem_error;

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

static void set_acdb_path_props(std::string device)
{
    int i = 0;
    try {
        for (const auto& acdb : directory_iterator("/vendor/etc/acdbdata/" + device + "/"))
            property_override(("persist.vendor.audio.calfile" + std::to_string(i++)).c_str(), acdb.path().c_str());
    } catch (const filesystem_error&) {
        // Ignore
    }
}

static void determine_device()
{
    std::string fdt_model;
    android::base::ReadFileToString("/sys/firmware/devicetree/base/model", &fdt_model, true);
    if (fdt_model.find("PINE QRD") != fdt_model.npos) {
        set_variant_props(pine_info);
        set_acdb_path_props("pine");
    } else if (fdt_model.find("Olive QRD") != fdt_model.npos) {
        set_variant_props(olive_info);
        set_acdb_path_props("olive");
    }
}

void vendor_load_properties() {
    determine_device();
    set_dalvik_heap();
}
