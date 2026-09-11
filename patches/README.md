# Submodule patches

Fixes for third-party submodules that can't be pushed from this repo
(no write access to the upstream remote). Apply them directly inside
the submodule's own checkout, then commit/push there as normal.

## blizzard-database-library-0001-fix-missing-algorithm-include.patch

Target repo: `src/external/blizzard-database-library`
(https://gitlab.com/T1ti/blizzard-database-library.git)
Base commit: `da25ad7` (the commit currently pinned by this repo's
`.gitmodules` / submodule pointer)

Fixes a GCC/Linux build failure: `VectorExtensions.h` calls
`std::find`/`std::distance` without including `<algorithm>`, which only
happens to work on MSVC because `<vector>` transitively pulls it in
there.

Apply:

```bash
cd src/external/blizzard-database-library
git checkout da25ad7   # or whatever commit is currently pinned
git am ../../../patches/blizzard-database-library-0001-fix-missing-algorithm-include.patch
# push the resulting commit to your own fork/remote of blizzard-database-library,
# then update this repo's submodule pointer to it.
```

If `git am` complains about a dirty or diverged tree, `git apply
../../../patches/blizzard-database-library-0001-fix-missing-algorithm-include.patch`
will apply just the diff without creating a commit.

## blizzard-database-library-0002-fix-missing-cstdint-include.patch

Target repo: `src/external/blizzard-database-library`
(https://gitlab.com/T1ti/blizzard-database-library.git)
Base commit: `da25ad7` (same base as 0001; applies cleanly on top of it too)

Fixes another GCC/Linux build failure, same root cause as 0001 but a
different header: `WDBCTableReader.h` uses `uint32_t` throughout
(member fields, a return type, casts) without including `<cstdint>`.
This produces `error: 'uint32_t' does not name a type`, which then
cascades into unrelated-looking errors further down the same header
(`has no member named 'Insert'`, `expected ')' before 'fieldCount'`)
once the malformed declarations it causes get used later in the file.

Apply the same way as 0001 (both patches touch different files, so
either order works, and both can be `git am`'d in one go):

```bash
cd src/external/blizzard-database-library
git checkout da25ad7   # or whatever commit is currently pinned
git am ../../../patches/blizzard-database-library-0001-fix-missing-algorithm-include.patch \
       ../../../patches/blizzard-database-library-0002-fix-missing-cstdint-include.patch
# push the resulting commits to your own fork/remote of blizzard-database-library,
# then update this repo's submodule pointer to it.
```
