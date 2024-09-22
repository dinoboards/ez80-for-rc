$version = $env:version
if (-not $version) {
    $version = (Get-Date).ToString("yy-MM-dd")
}

# Create the GitHub release command
$releaseCommand = "gh release create $version --draft -F ./releases/release-$version.md -t `"$version`" releases/release-$version.zip"

# Output the command
Write-Output $releaseCommand
