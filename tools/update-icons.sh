#!/bin/sh -ex

# SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

# Updates main icons for volt

which magick || exit
which optipng || exit

VARIATION=${VARIATION:-base}

VOLT_BASE_SVG="dist/icon_variations/${VARIATION}.svg"
VOLT_BG_COLOR="dist/icon_variations/${VARIATION}_bgcolor"
# TODO: VOLT_MONOCHROME_SVG Variation

[ -f "$VOLT_BASE_SVG" ] && [ -f "$VOLT_BG_COLOR" ] || { echo "Error: missing ${VARIATION}.svg/${VARIATION}_bgcolor" >&2; exit; }

# Desktop / Windows / Qt icons

VOLT_DESKTOP_SVG="dist/dev.volt_emu.volt.svg"

cp "$VOLT_BASE_SVG" "$VOLT_DESKTOP_SVG"

magick -density 256x256 -background transparent "$VOLT_BASE_SVG" -define icon:auto-resize -colors 256 dist/volt.ico || exit
magick -density 256x256 -background transparent "$VOLT_BASE_SVG" -resize 256x256 dist/volt.bmp || exit

magick -size 256x256 -background transparent "$VOLT_BASE_SVG" -resize 256x256 dist/qt_themes/default/icons/256x256/volt.png || exit

optipng -o7 dist/qt_themes/default/icons/256x256/volt.png

# Android adaptive icon (API 26+)

VOLT_ANDROID_RES="src/android/app/src/main/res"
VOLT_ANDROID_FG="$VOLT_ANDROID_RES/drawable/ic_launcher_foreground.png"
VOLT_ANDROID_BG_COLOR=$(cat $VOLT_BG_COLOR)

# Update Icon Background Color
echo "<?xml version='1.0' encoding='utf-8'?><resources><color name='ic_launcher_background'>${VOLT_ANDROID_BG_COLOR}</color></resources>" > "$VOLT_ANDROID_RES/values/colors.xml"

magick -size 1080x1080 -background transparent "$VOLT_BASE_SVG" -gravity center -resize 660x660 -extent 1080x1080 "$VOLT_ANDROID_FG" || exit
magick -background transparent "$VOLT_BASE_SVG" -gravity center -resize 512x512 "$VOLT_ANDROID_RES/drawable/ic_yuzu.png" || exit
magick -size 512x512 -background transparent "$VOLT_BASE_SVG" -gravity center -resize 338x338 -extent 512x512 "$VOLT_ANDROID_RES/drawable/ic_yuzu_splash.png" || exit

optipng -o7 "$VOLT_ANDROID_FG"
optipng -o7 "$VOLT_ANDROID_RES/drawable/ic_yuzu.png"
optipng -o7 "$VOLT_ANDROID_RES/drawable/ic_yuzu_splash.png"

# Android legacy launcher icon (API <= 24)

BASE_LEGACY="$VOLT_ANDROID_RES/mipmap-xxxhdpi/ic_launcher.png"

magick -size 512x512 xc:${VOLT_ANDROID_BG_COLOR} "$VOLT_ANDROID_FG" -gravity center -resize 384x384 -composite "$BASE_LEGACY" || exit

magick "$BASE_LEGACY" -resize 192x192 "$VOLT_ANDROID_RES/mipmap-xxhdpi/ic_launcher.png"
magick "$BASE_LEGACY" -resize 144x144 "$VOLT_ANDROID_RES/mipmap-xhdpi/ic_launcher.png"
magick "$BASE_LEGACY" -resize 96x96 "$VOLT_ANDROID_RES/mipmap-hdpi/ic_launcher.png"
magick "$BASE_LEGACY" -resize 72x72 "$VOLT_ANDROID_RES/mipmap-mdpi/ic_launcher.png"

optipng -o7 "$VOLT_ANDROID_RES"/mipmap-*/ic_launcher.png

# macOS
# TODO: Update Assets.car too

TMP_PNG="dist/volt-tmp.png"

magick -size 1024x1024 -background none "$VOLT_BASE_SVG" "$TMP_PNG" || exit

png2icns dist/volt.icns "$TMP_PNG" || echo 'non fatal'

rm "$TMP_PNG"
