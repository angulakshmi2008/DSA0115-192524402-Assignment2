param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string[]]$Sources,

    [Parameter(Mandatory = $false)]
    [string]$Out = "",

    [Parameter(Mandatory = $false)]
    [ValidateSet("O0", "O1", "O2", "O3")]
    [string]$Opt = "O0",

    [switch]$UseDefaultOpt
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($Out)) {
    $baseName = [System.IO.Path]::GetFileNameWithoutExtension($Sources[0])
    $dirName = [System.IO.Path]::GetDirectoryName($Sources[0])
    if ([string]::IsNullOrWhiteSpace($dirName)) {
        $Out = "$baseName.exe"
    } else {
        $Out = Join-Path $dirName "$baseName.exe"
    }
}

# Workaround for MinGW-w64 GCC when installed under a user profile path containing spaces
# (unquoted default-manifest.o expansion in built-in *endfile spec), without modifying GCC binaries.
$tempSpec = Join-Path ([System.IO.Path]::GetTempPath()) ("mingw_endfile_" + [guid]::NewGuid().ToString("N") + ".specs")
$specContent = "*endfile:`n%{mdaz-ftz:crtfastmath.o%s;Ofast|ffast-math|funsafe-math-optimizations:%{!shared:%{!mno-daz-ftz:crtfastmath.o%s}}} %{fvtable-verify=none:%s; fvtable-verify=preinit:vtv_end.o%s; fvtable-verify=std:vtv_end.o%s} crtend.o%s`n"
Set-Content -Path $tempSpec -Value $specContent -Encoding Ascii

try {
    $argsList = @("-std=c++17")
    if (-not $UseDefaultOpt) {
        $argsList += "-$Opt"
    }
    $argsList += @("-Wall", "-Wextra", "-pedantic", "-specs=$tempSpec")
    $argsList += $Sources
    $argsList += @("-o", $Out)

    & g++ @argsList
    exit $LASTEXITCODE
}
finally {
    if (Test-Path $tempSpec) {
        Remove-Item $tempSpec -Force -ErrorAction SilentlyContinue
    }
}
