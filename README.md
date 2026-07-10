# HDB

Human Database (HDB) is a C++23 engine for storing, retrieving, and simulating thought events.

**Author**: Kyoung Chan Park (Park, Kyoung Chan) - DPChanny @ DPC Studio  
**License**: [GNU AGPL-3.0](LICENSE)

## Project Structure

- [core](core): storage-agnostic domain model and cognitive operations
- [standard](standard): SQLite-backed runtime implementation of the core interfaces
- [python](python): optional Python bindings (disabled by default)

## Build

Requirements:

- CMake 3.26+
- C++23 compiler
- SQLite3 development package (when building `standard`)
- pybind11 CMake package (when building `python`)

Configure:

```bash
cmake -S . -B build -DHDB_BUILD_STANDARD=ON -DHDB_BUILD_PYTHON=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Build:

```bash
cmake --build build
```

Root CMake options:

- `HDB_BUILD_STANDARD` (default: `ON`)
- `HDB_BUILD_PYTHON` (default: `ON`)

## Documentation

- Core scope and API: [core/README.md](core/README.md)
- Standard runtime scope and API: [standard/README.md](standard/README.md)
- Python bindings scope and API: [python/README.md](python/README.md)

## Stewardship

HDB is stewarded by the **HDB Guardians**.

- Organization: https://github.com/hdb-guardians
- Founding guardian: Kyoung Chan Park (Park, Kyoung Chan), DPChanny @ DPC Studio
- Email: kyoungchan0516@gmail.com
- Phone: +82-10-8246-4691

Stewardship is for governance and continuity. Licensing terms are defined only by the AGPL-3.0 text in [LICENSE](LICENSE).
