# HDB

Human Database (HDB) is a C++23 engine for storing, retrieving, and simulating thought events.

**Author**: Kyoung Chan Park (Park, Kyoung Chan) - DPChanny @ DPC Studio  
**License**: [GNU AGPL-3.0](LICENSE)

## Project Structure

```
core/   → storage-agnostic domain model and cognitive operations
api/    → Session facade (composes core molecules); always built
sqlite/ → SQLite-backed store implementations (optional)
python/ → Python bindings (optional)
```

Dependency direction:

```
core ──► api
core ──► sqlite
api + sqlite ──► python
```

- [core](core): types, quarks, atoms, store interfaces, and molecule orchestration surfaces
- [api](api): `Session` facade that wires all four molecules to a store bundle
- [sqlite](sqlite): `SqliteNeuronTable`, `SqliteSynapseTable`, `SqliteDreamTable`, and `open_sqlite`
- [python](python): pybind11 bindings — `_hdb` (api) and `_hdb_sqlite` (sqlite)

## Build

Requirements:

- CMake 3.26+
- C++23 compiler
- SQLite3 development package (when building `sqlite`)
- pybind11 CMake package (when building `python`)

Configure:

```bash
cmake -S . -B build -DHDB_BUILD_SQLITE=ON -DHDB_BUILD_PYTHON=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Build:

```bash
cmake --build build
```

Root CMake options:

| Option             | Default | Description                               |
| ------------------ | ------- | ----------------------------------------- |
| `HDB_BUILD_SQLITE` | `ON`    | Build SQLite-backed store implementations |
| `HDB_BUILD_PYTHON` | `ON`    | Build Python bindings                     |

`core` and `api` are always built.

## Documentation

- Domain model and cognitive operations: [core/README.md](core/README.md)
- Session facade: [api/README.md](api/README.md)
- SQLite runtime: [sqlite/README.md](sqlite/README.md)
- Python bindings: [python/README.md](python/README.md)

## Stewardship

HDB is stewarded by the **HDB Guardians**.

- Organization: https://github.com/hdb-guardians
- Founding guardian: Kyoung Chan Park (Park, Kyoung Chan), DPChanny @ DPC Studio
- Email: kyoungchan0516@gmail.com
- Phone: +82-10-8246-4691

Stewardship is for governance and continuity. Licensing terms are defined only by the AGPL-3.0 text in [LICENSE](LICENSE).
