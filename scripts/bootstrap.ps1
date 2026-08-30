$ErrorActionPreference = "Stop"

$vcpkgTag = "2026.05.25"
$vcpkgCommit = "d015e31e90838a4c9dfa3eed45979bc70d9357fc"
$vcpkgRepository = "https://github.com/microsoft/vcpkg.git"

$repositoryRoot = Split-Path -Parent $PSScriptRoot
$dependenciesDir = Join-Path $repositoryRoot ".deps"
$vcpkgDir = Join-Path $dependenciesDir "vcpkg"

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    throw "Git is required to bootstrap Astra's dependencies."
}

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    throw "CMake 3.25 or newer is required to build Astra."
}

New-Item -ItemType Directory -Force (Join-Path $dependenciesDir "downloads") | Out-Null
New-Item -ItemType Directory -Force (Join-Path $dependenciesDir "binary-cache") | Out-Null

if (-not (Test-Path (Join-Path $vcpkgDir ".git") -PathType Container)) {
    if (Test-Path $vcpkgDir) {
        throw "$vcpkgDir exists but is not a Git checkout; move or remove it first."
    }

    git clone --branch $vcpkgTag --depth 1 $vcpkgRepository $vcpkgDir
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to clone vcpkg."
    }
}

$localChanges = git -C $vcpkgDir status --porcelain
if ($LASTEXITCODE -ne 0) {
    throw "Failed to inspect the vcpkg checkout."
}
if ($localChanges) {
    throw "$vcpkgDir has local changes; refusing to overwrite them."
}

$currentCommit = (git -C $vcpkgDir rev-parse HEAD).Trim()
if ($LASTEXITCODE -ne 0) {
    throw "Failed to read the vcpkg revision."
}

if ($currentCommit -ne $vcpkgCommit) {
    $tagRef = "refs/tags/${vcpkgTag}:refs/tags/${vcpkgTag}"
    git -C $vcpkgDir fetch --depth 1 origin $tagRef
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to fetch the pinned vcpkg revision."
    }

    git -C $vcpkgDir checkout --detach $vcpkgCommit
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to check out the pinned vcpkg revision."
    }
}

$currentCommit = (git -C $vcpkgDir rev-parse HEAD).Trim()
if ($LASTEXITCODE -ne 0 -or $currentCommit -ne $vcpkgCommit) {
    throw "The vcpkg checkout does not match Astra's pinned revision."
}

& (Join-Path $vcpkgDir "bootstrap-vcpkg.bat") -disableMetrics
if ($LASTEXITCODE -ne 0) {
    throw "Failed to bootstrap vcpkg."
}

Write-Host "Astra dependency tooling is ready."
Write-Host "Next: cmake --preset msvc-debug"
