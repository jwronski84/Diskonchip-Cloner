$ErrorActionPreference = 'Stop'
$Out = Join-Path $PSScriptRoot '..\stl\generated'
New-Item -ItemType Directory -Force -Path $Out | Out-Null
$Scad = Join-Path $PSScriptRoot 'nucleo_h753zi_enclosure.scad'
$Parts = @('bottom','top','buttons','knob','assembly')
foreach ($p in $Parts) {
  $Target = Join-Path $Out "md2202_h753zi_enclosure_$p.stl"
  openscad -o $Target -D "part=\"$p\"" $Scad
}
Write-Host "Generated enclosure STL files in $Out"
