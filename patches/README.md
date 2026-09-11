# Submodule patches

Fixes for the `blizzard-database-library` submodule
(https://gitlab.com/T1ti/blizzard-database-library.git) that can't be
pushed from this repo (no write access to that remote). Every patch
here is `git am`-able against base commit `da25ad7` (the commit
currently pinned by this repo's submodule pointer), applies cleanly
together in any order since each touches a different file, and is
mirrored by an equivalent auto-patch step in the root `CMakeLists.txt`
so a plain `git submodule update` still gets a building tree without
any of this.

All of them are the same story: a standard-library header
(`<algorithm>`, `<cstdint>`, or `<cstring>`) is relied on without being
included, and only compiles by accident on toolchains where some other
header happens to drag it in transitively -- GCC/libstdc++ (observed
building on Linux, including at least one case that didn't reproduce
against every GCC version tested) doesn't guarantee that.

| Patch | File | Missing header | Symptom |
|---|---|---|---|
| 0001 | `include/extensions/VectorExtensions.h` | `<algorithm>` | `no matching function for call to 'find(...)'` |
| 0002 | `include/readers/wdbc/WDBCTableReader.h` | `<cstdint>` | `'uint32_t' does not name a type`, cascading into `has no member named 'Insert'` etc. further down the file |
| 0003 | `src/readers/wdc5/WDC5TableReader.cpp` | `<cstdint>`, `<cstring>` | `'uint32_t' was not declared in this scope`, cascading into `'encryptedIDCount' was not declared` |
| 0004 | `src/stream/BitReader.cpp` | `<cstdint>`, `<cstring>` | `'uint32_t' was not declared in this scope` |
| 0005 | `src/readers/wdbc/WDBCTableReader.cpp` | `<cstring>` | relies on the memcpy/memset family |
| 0006 | `src/readers/wdc3/WDC3TableReader.cpp` | `<cstring>` | relies on the memcpy/memset family |
| 0007 | `include/extensions/MemoryExtensions.h` | `<cstring>` | relies on the memcpy/memset family |
| 0008 | `src/DatabaseDefinition.cpp` | `<algorithm>` | uses `std::find` |

Apply all of them in one `git am` (or drop the ones you don't need --
they're independent):

```bash
cd src/external/blizzard-database-library
git checkout da25ad7   # or whatever commit is currently pinned
git am ../../../patches/blizzard-database-library-0001-fix-missing-algorithm-include.patch \
       ../../../patches/blizzard-database-library-0002-fix-missing-cstdint-include.patch \
       ../../../patches/blizzard-database-library-0003-fix-missing-cstdint-cstring-wdc5.patch \
       ../../../patches/blizzard-database-library-0004-fix-missing-cstdint-cstring-bitreader.patch \
       ../../../patches/blizzard-database-library-0005-fix-missing-cstring-wdbc-cpp.patch \
       ../../../patches/blizzard-database-library-0006-fix-missing-cstring-wdc3-cpp.patch \
       ../../../patches/blizzard-database-library-0007-fix-missing-cstring-memoryextensions.patch \
       ../../../patches/blizzard-database-library-0008-fix-missing-algorithm-databasedefinition.patch
# push the resulting commits to your own fork/remote of blizzard-database-library,
# then update this repo's submodule pointer to it.
```

If `git am` complains about a dirty or diverged tree, `git apply
<patch-file>` will apply just the diff without creating a commit.
