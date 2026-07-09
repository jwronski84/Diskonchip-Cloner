$ErrorActionPreference = 'Stop'
$Out = Join-Path $PSScriptRoot '..\stl\generated'
New-Item -ItemType Directory -Force -Path $Out | Out-Null
$Scad = Join-Path $PSScriptRoot 'MD2202_enclosure.scad'
$Parts = @('bottom','top','knob','buttons')
foreach ($p in $Parts) {
    $Target = Join-Path $Out "MD2202_cloner_$p.stl"
    openscad -o $Target -D "part=\"$p\"" $Scad
}
Write-Host "STL files written to $Out"
