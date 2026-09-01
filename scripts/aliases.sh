# Optional shell shortcuts for the default (GCC Debug) preset.
# Source from your shell rc:  source /path/to/Astra-Engine/scripts/aliases.sh
# Works in bash and zsh from any directory.

ASTRA_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]:-${(%):-%x}}")/.." && pwd)"

astra_build() { (cd "$ASTRA_ROOT" && cmake --build --preset build-default "$@"); }
astra_test()  { (cd "$ASTRA_ROOT" && ctest --preset test-default "$@"); }
astra_run()   { (cd "$ASTRA_ROOT" && cmake --build --preset build-default >/dev/null && ./build/gcc-debug/bin/astra_engine "$@"); }
astra_fmt()   { (cd "$ASTRA_ROOT" && cmake --build --preset format); }
cda()         { cd "$ASTRA_ROOT"; }

# Short build-and-run shortcuts. Work from any directory (they cd into the
# repo in a subshell). Configure automatically if the preset hasn't been yet.
#   ad [args]  -> configure (if needed) + build default (GCC Debug), then run
#   ar [args]  -> configure (if needed) + build gcc-release, then run
#   ac         -> `ninja clean` in every configured build dir (keeps the CMake
#                 cache, so no vcpkg re-configure). For a full wipe: rm -rf build/<preset>
# Note: `ar` shadows the GNU archiver for interactive use only; build tools
# spawn their own `ar` and are unaffected. Use `command ar` if you ever need it.
_astra_build_run() {
  local preset=$1 dir=$2; shift 2
  (
    cd "$ASTRA_ROOT" || exit 1
    [ -f "build/$dir/CMakeCache.txt" ] || cmake --preset "$preset" || exit 1
    cmake --build --preset "build-$preset" || exit 1
    exec "./build/$dir/bin/astra_engine" "$@"
  )
}
ad() { _astra_build_run default gcc-debug "$@"; }
ar() { _astra_build_run gcc-release gcc-release "$@"; }
ac() {
  (
    cd "$ASTRA_ROOT" || exit 1
    local d
    for d in build/*/; do
      [ -f "$d/CMakeCache.txt" ] || continue
      echo "clean: $d"
      cmake --build "$d" --target clean || exit 1
    done
  )
}
