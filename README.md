# HDB

Human Database (HDB) is a C++23 engine for storing, retrieving, and simulating thought events.

**Author**: Kyoung Chan Park (Park, Kyoung Chan) - DPChanny @ DPC Studio  
**License**: [GNU AGPL-3.0](LICENSE)

## Project Structure

```
core/         → storage-agnostic domain model and cognitive operations
store/
  sqlite/     → SQLite-backed store implementations (optional)
python/       → Python bindings (always built)
```

Dependency direction:

```
core ──► store/sqlite
core + store/sqlite ──► python
```

- [core](core): types, quarks, atoms, store interfaces, and molecule operations
- [store/sqlite](store/sqlite): `SqliteNeuronTable`, `SqliteSynapseTable`, `SqliteDreamTable`, and `open_sqlite`
- [python](python): pybind11 bindings — `hdb.quark`, `hdb.atom`, `hdb.store`, `hdb.molecule`

## Build

Requirements:

- CMake 3.26+
- C++23 compiler (Clang 17+ or GCC 13+ recommended)
- SQLite3 development package (when building `store/sqlite`)
- pybind11 CMake package (when building `python`)

### Toolchain

CMake auto-detects the compiler from `PATH`. For local overrides (custom compiler path, generator, etc.) create `CMakeUserPresets.json` — it is gitignored:

```json
{
  "version": 6,
  "configurePresets": [
    {
      "name": "base",
      "generator": "Ninja",
      "cacheVariables": {
        "CMAKE_CXX_COMPILER": "/path/to/clang++"
      }
    }
  ]
}
```

Configure:

```bash
cmake --preset debug
```

Build:

```bash
cmake --build --preset debug
```

Root CMake options:

| Option             | Default | Description                   |
| ------------------ | ------- | ----------------------------- |
| `HDB_BUILD_STORE`  | `ON`    | Build store backends (SQLite) |
| `HDB_BUILD_PYTHON` | `OFF`   | Build Python bindings         |

`core` is always built.

## Documentation

- Domain model and cognitive operations: [core/README.md](core/README.md)
- SQLite runtime: [store/sqlite/README.md](store/sqlite/README.md)
- Python bindings: [python/README.md](python/README.md)

## Stewardship

HDB is stewarded by the **HDB Guardians**.

- Organization: https://github.com/hdb-guardians
- Founding guardian: Kyoung Chan Park (Park, Kyoung Chan), DPChanny @ DPC Studio
- Email: kyoungchan0516@gmail.com
- Phone: +82-10-8246-4691

Stewardship is for governance and continuity. Licensing terms are defined only by the AGPL-3.0 text in [LICENSE](LICENSE).
