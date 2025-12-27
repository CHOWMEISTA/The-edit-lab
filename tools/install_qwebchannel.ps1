param(
  [string]$qtBase = "C:\\Qt",
  [string]$outDir = "..\\ui"
)

Write-Host "Searching for qwebchannel.js under $qtBase ..."

$candidates = Get-ChildItem -Path $qtBase -Recurse -Filter qwebchannel.js -ErrorAction SilentlyContinue | Select-Object -First 1
if (-not $candidates) {
  Write-Error "qwebchannel.js not found under $qtBase. Try passing a custom path to Qt installation or copy it manually."
  exit 1
}

$src = $candidates.FullName
$dst = Join-Path -Path (Resolve-Path $outDir).Path -ChildPath "qwebchannel.js"
Write-Host "Copying $src -> $dst"
Copy-Item -Path $src -Destination $dst -Force
Write-Host "Done. qwebchannel.js installed to $outDir"
