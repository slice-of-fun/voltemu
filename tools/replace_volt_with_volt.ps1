param (
    [string]$SearchDir = "."
)

# Files/Folders to explicitly exclude from modification
$excludePaths = @(
    "*\.git\*", "*\build\*", "*\externals\*", "*\.cache\*",
    "*\README.md", "*\ATTRIBUTION.md", "*\docs\upstream\*", "*\LICENSE.txt"
)

# Extensions to skip (binaries, images, etc.)
$excludeExts = @(
    "*.png", "*.jpg", "*.jpeg", "*.svg", "*.ico", "*.icns", "*.bmp",
    "*.ttf", "*.otf", "*.woff", "*.woff2",
    "*.bin", "*.dll", "*.exe", "*.so", "*.a", "*.lib", "*.obj", "*.o",
    "*.car", "*.jar", "*.apk", "*.aab", "*.keystore", "*.pck"
)

$files = Get-ChildItem -Path $SearchDir -Recurse -File -Exclude $excludeExts | Where-Object {
    $path = $_.FullName
    $skip = $false
    foreach ($ex in $excludePaths) {
        if ($path -like $ex) {
            $skip = $true
            break
        }
    }
    -not $skip
}

$modifiedCount = 0

foreach ($file in $files) {
    try {
        $content = Get-Content $file.FullName -Raw -Encoding UTF8 -ErrorAction Stop
        if ($content -match "(?i)volt") {
            $lines = $content -split "`r?`n"
            $newLines = @()
            $changed = $false
            
            foreach ($line in $lines) {
                # Preserve copyright and SPDX lines completely
                if ($line -match "SPDX-FileCopyrightText" -or $line -match "Copyright") {
                    $newLines += $line
                    continue
                }
                
                # Check for "volt" case insensitively
                if ($line -match "(?i)volt") {
                    $newLine = $line -creplace "VoltEmulator", "VoltEmulator"
                    $newLine = $newLine -creplace "Volt Emulator", "Volt Emulator"
                    $newLine = $newLine -creplace "volt-emu", "volt-emu"
                    $newLine = $newLine -creplace "Volt-Emu", "Volt-Emu"
                    $newLine = $newLine -creplace "volt_emu", "volt_emu"
                    $newLine = $newLine -creplace "Volt_Emu", "Volt_Emu"
                    $newLine = $newLine -creplace "VOLT_EMU", "VOLT_EMU"
                    $newLine = $newLine -creplace "VoltEmu", "VoltEmu"
                    $newLine = $newLine -creplace "voltemu", "voltemu"
                    $newLine = $newLine -creplace "VOLTEMU", "VOLTEMU"
                    $newLine = $newLine -creplace "volt_dir", "volt_dir"
                    $newLine = $newLine -creplace "VoltDir", "VoltDir"
                    $newLine = $newLine -creplace "voltDir", "voltDir"
                    
                    # Core words
                    $newLine = $newLine -creplace "VOLT", "VOLT"
                    $newLine = $newLine -creplace "Volt", "Volt"
                    $newLine = $newLine -creplace "volt", "volt"

                    if ($newLine -cne $line) {
                        $changed = $true
                    }
                    $newLines += $newLine
                } else {
                    $newLines += $line
                }
            }
            
            if ($changed) {
                $newContent = $newLines -join "`n"
                Set-Content $file.FullName -Value $newContent -NoNewline -Encoding UTF8
                $modifiedCount++
            }
        }
    } catch {
        # Ignore unreadable/binary false positives
    }
}

Write-Output "Successfully replaced 'Volt' with 'Volt' in $modifiedCount files."

# Now search for any files or folders that have 'volt' in their name
# and rename them to 'volt'
$renameCount = 0

# Do folders first (bottom up to avoid path invalidation)
$folders = Get-ChildItem -Path $SearchDir -Recurse -Directory | Where-Object { $_.Name -match "(?i)volt" -and $_.FullName -notmatch "\\\.git\\" -and $_.FullName -notmatch "\\build\\" -and $_.FullName -notmatch "\\externals\\" -and $_.FullName -notmatch "\\\.cache\\" } | Sort-Object -Property @{Expression={$_.FullName.Length}; Descending=$true}

foreach ($folder in $folders) {
    $newName = $folder.Name -creplace "Volt", "Volt"
    $newName = $newName -creplace "volt", "volt"
    $newName = $newName -creplace "VOLT", "VOLT"
    
    if ($newName -ne $folder.Name) {
        Rename-Item -Path $folder.FullName -NewName $newName
        $renameCount++
    }
}

# Now do files
$filesToRename = Get-ChildItem -Path $SearchDir -Recurse -File | Where-Object { $_.Name -match "(?i)volt" -and $_.FullName -notmatch "\\\.git\\" -and $_.FullName -notmatch "\\build\\" -and $_.FullName -notmatch "\\externals\\" -and $_.FullName -notmatch "\\\.cache\\" }

foreach ($file in $filesToRename) {
    $newName = $file.Name -creplace "Volt", "Volt"
    $newName = $newName -creplace "volt", "volt"
    $newName = $newName -creplace "VOLT", "VOLT"
    
    if ($newName -ne $file.Name) {
        Rename-Item -Path $file.FullName -NewName $newName
        $renameCount++
    }
}

Write-Output "Renamed $renameCount files/folders."
