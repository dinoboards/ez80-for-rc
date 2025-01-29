# Define the output zip file name
$releaseDir = "releases"

# Create the releases directory if it doesn't exist
if (-Not (Test-Path -Path $releaseDir)) {
  New-Item -ItemType Directory -Path $releaseDir
}

# Set the version to the provided version or the current date if not provided
$version = $env:version
if (-not $version) {
    $version = (Get-Date).ToString("yy-MM-dd")
}

# Get the last release tag
# $lastRelease = git tag -l | Sort-Object | Select-Object -Last 1
$lastRelease = "d9d96e9ab841e3c31260fefac6805b54e7b9a232"  # for first release, just start from the top

# Output the last release tag
Write-Output $lastRelease

# Define the release file path
$relfile = "./releases/release-$version.md"

# Remove the release file if it exists
Remove-Item -Force $relfile

# Create the release notes file
Add-Content -Path $relfile -Value "## Release Notes`r`n`r`n"
Add-Content -Path $relfile -Value "``````"

# Append the git log to the release notes file
$gitLog = git log --pretty=format:"%ad: %s" --date=short --submodule=short "$lastRelease..HEAD"
Add-Content -Path $relfile -Value $gitLog

# Close the code block in the release notes file
Add-Content -Path $relfile -Value "`r`n```````r`n"
