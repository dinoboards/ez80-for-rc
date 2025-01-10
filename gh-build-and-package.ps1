$version = $env:version
if (-not $version) {
    $version = (Get-Date).ToString("yy-MM-dd")
}

# Define the output zip file name
$releaseDir = "releases"
$zipFile = "$releaseDir/release-$version.zip"

# Define the directories to include
$firmwareBin = "firmware/bin"
$hardwareBin = "hardware/bin"

# Create the releases directory if it doesn't exist
if (-Not (Test-Path -Path $releaseDir)) {
  New-Item -ItemType Directory -Path $releaseDir
}

# Delete the existing zip file if it exists
if (Test-Path -Path $zipFile) {
  Remove-Item -Path $zipFile
}

# Function to run a batch file and check for errors
function Run-BatchFile {
  param (
    [string]$filePath
  )
  $directory = Split-Path -Path $filePath -Parent
  $fileName = Split-Path -Path $filePath -Leaf
  Push-Location -Path $directory
  $process = Start-Process -FilePath ".\$fileName" -NoNewWindow -Wait -PassThru
  Pop-Location
  if ($process.ExitCode -ne 0) {
    Write-Host "Error: Command $directory $fileName failed with exit code $($process.ExitCode)." -ForegroundColor Red
    exit $process.ExitCode
  }
}

# Run the make-jeds.bat command to build the hardware bin files
Run-BatchFile "hardware\make-jed.bat"

# Run the batch commands in the firmware directory
$firmwareCommands = @("make-debug.bat", "make-release.bat")
foreach ($cmd in $firmwareCommands) {
  Run-BatchFile "firmware\$cmd"
}

# Create the zip file
Compress-Archive -Path "$firmwareBin/*" -DestinationPath $zipFile

# Add files from the second directory to the existing zip file
Compress-Archive -Path "$hardwareBin/*" -Update -DestinationPath $zipFile

Write-Output "Zip file created: $zipFile"
