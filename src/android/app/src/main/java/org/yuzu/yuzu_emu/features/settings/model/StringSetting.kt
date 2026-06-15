// SPDX-FileCopyrightText: Copyright 2025 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

// SPDX-FileCopyrightText: 2023 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

package dev.volt_emu.volt.features.settings.model

import dev.volt_emu.volt.utils.NativeConfig

enum class StringSetting(override val key: String) : AbstractStringSetting {
    DRIVER_PATH("driver_path"),
    DEVICE_NAME("device_name"),

    WEB_TOKEN("volt_token"),
    WEB_USERNAME("volt_username")
    ;

    override fun getString(needsGlobal: Boolean): String = NativeConfig.getString(key, needsGlobal)

    override fun setString(value: String) {
        if (NativeConfig.isPerGameConfigLoaded()) {
            global = false
        }
        NativeConfig.setString(key, value)
    }

    override val defaultValue: String by lazy { NativeConfig.getDefaultToString(key) }

    override fun getValueAsString(needsGlobal: Boolean): String = getString(needsGlobal)

    override fun reset() = NativeConfig.setString(key, defaultValue)
}
