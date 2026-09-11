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
