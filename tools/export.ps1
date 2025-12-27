param(
  [int]$startMs = 0,
  [int]$endMs = 5000,
  [string]$inFile,
  [string]$outFile
)
if (-not (Test-Path $inFile)) { Write-Error "Input file not found"; exit 1 }
$start = $startMs / 1000.0
$dur = ($endMs - $startMs) / 1000.0
ffmpeg -y -ss $start -i "$inFile" -t $dur -c:v libx264 -preset veryfast -c:a aac "$outFile"
