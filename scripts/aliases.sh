# Optional shell shortcuts for the default (GCC Debug) preset.
# Source from your shell rc:  source /path/to/Astra-Engine/scripts/aliases.sh
# Works in bash and zsh from any directory.

ASTRA_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]:-${(%):-%x}}")/.." && pwd)"

astra_build() { (cd "$ASTRA_ROOT" && cmake --build --preset build-default "$@"); }
astra_test()  { (cd "$ASTRA_ROOT" && ctest --preset test-default "$@"); }
astra_run()   { (cd "$ASTRA_ROOT" && cmake --build --preset build-default >/dev/null && ./build/gcc-debug/src/astra_engine "$@"); }
astra_fmt()   { (cd "$ASTRA_ROOT" && cmake --build --preset format); }
cda()         { cd "$ASTRA_ROOT"; }
