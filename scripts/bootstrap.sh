#!/usr/bin/env sh

set -eu

vcpkg_tag="2026.05.25"
vcpkg_commit="d015e31e90838a4c9dfa3eed45979bc70d9357fc"
vcpkg_repository="https://github.com/microsoft/vcpkg.git"

script_dir=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
repository_root=$(CDPATH='' cd -- "${script_dir}/.." && pwd)
dependencies_dir="${repository_root}/.deps"
vcpkg_dir="${dependencies_dir}/vcpkg"

if ! command -v git >/dev/null 2>&1; then
    echo "Git is required to bootstrap Astra's dependencies." >&2
    exit 1
fi

if ! command -v cmake >/dev/null 2>&1; then
    echo "CMake 3.25 or newer is required to build Astra." >&2
    exit 1
fi

if ! command -v ninja >/dev/null 2>&1; then
    echo "Ninja is required to build Astra (apt install ninja-build, brew install ninja)." >&2
    exit 1
fi

mkdir -p "${dependencies_dir}/downloads" "${dependencies_dir}/binary-cache"

if [ ! -d "${vcpkg_dir}/.git" ]; then
    if [ -e "${vcpkg_dir}" ]; then
        echo "${vcpkg_dir} exists but is not a Git checkout; move or remove it first." >&2
        exit 1
    fi

    git clone --branch "${vcpkg_tag}" --depth 1 "${vcpkg_repository}" "${vcpkg_dir}"
fi

if [ -n "$(git -C "${vcpkg_dir}" status --porcelain)" ]; then
    echo "${vcpkg_dir} has local changes; refusing to overwrite them." >&2
    exit 1
fi

current_commit=$(git -C "${vcpkg_dir}" rev-parse HEAD)
if [ "${current_commit}" != "${vcpkg_commit}" ]; then
    git -C "${vcpkg_dir}" fetch --depth 1 origin \
        "refs/tags/${vcpkg_tag}:refs/tags/${vcpkg_tag}"
    git -C "${vcpkg_dir}" checkout --detach "${vcpkg_commit}"
fi

current_commit=$(git -C "${vcpkg_dir}" rev-parse HEAD)
if [ "${current_commit}" != "${vcpkg_commit}" ]; then
    echo "The vcpkg checkout does not match Astra's pinned revision." >&2
    exit 1
fi

"${vcpkg_dir}/bootstrap-vcpkg.sh" -disableMetrics

echo "Astra dependency tooling is ready."
echo "Next: cmake --preset default"
