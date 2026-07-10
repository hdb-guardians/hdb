# HDB Python Bindings

**Parent project**: [HDB](../README.md)  
**Depends on**: [core](../core), [standard](../standard)

## Responsibility

`python` exposes the `hdb::standard::Session` runtime as a Python extension module.

It focuses on:

- Python-friendly input and output conversion
- byte payload/meta transport via `bytes`
- session orchestration from Python

It does not define:

- domain semantics (owned by `core`)
- persistence/runtime internals (owned by `standard`)

## Build Prerequisites

- Python 3.9+
- pybind11 with CMake config package (`pybind11Config.cmake`)
- CMake 3.26+

Root build option:

- `HDB_BUILD_PYTHON` (default: `ON`)

## Development Flow (uv)

From the repository root:

```powershell
uv sync --project python --group dev --no-install-project
cmake -S . -B build -DHDB_BUILD_STANDARD=ON -DHDB_BUILD_PYTHON=ON -DPython_EXECUTABLE="c:/Devs/generals/hdb/python/.venv/Scripts/python.exe"
cmake --build build
```

What this produces:

- `build/standard/libhdb_standard.a` (native intermediate static library)
- `build/python/bindings/_hdb.<python-tag>.pyd` (Python extension module)

## How Python Consumption Works

`hdb_standard` is a native build input for the binding module.
Python users do not link `libhdb_standard.a` directly.

Python-facing artifact is the package:

```text
hdb/
|- __init__.py
`- _hdb.<python-tag>.pyd
```

The package entrypoint is `hdb.Session`.

## Use In Another Python Project

Recommended path is wheel-based installation.

1. Build a wheel from this repository.
2. Install that wheel into the target project's virtual environment.
3. Import and use `hdb`.

Example workflow:

```powershell
uv sync --project python --group dev --no-install-project
uv build ./python --wheel -o ./dist
uv pip install --python python/.venv/Scripts/python.exe ./dist/hdb-0.1.0-*.whl
python/.venv/Scripts/python.exe -c "import hdb; print(hdb.Session)"
```

Note:

- CMake install rules place `__init__.py` and `_hdb` under the `hdb` package path used by wheel builds.
- `python/CMakeLists.txt` resolves `pybind11` from the selected interpreter via `python -m pybind11 --cmakedir`, so `uv` virtual environments work without manually setting `pybind11_DIR`.
- `uv sync` can be run with `--no-install-project` during initial environment provisioning to avoid editable-build failures when a C/C++ toolchain is not yet fully configured.
- This package intentionally avoids OS-specific runtime DLL bundling.
- `sqlite-vec` is treated as an external runtime dependency.
- `Session(db_path)` requires either `sqlite_vec_extension_path` argument or environment variable `HDB_SQLITE_VEC_EXTENSION`.

Example:

```powershell
$env:HDB_SQLITE_VEC_EXTENSION="C:/path/to/sqlite_vec.dll"
python/.venv/Scripts/python.exe -c "import hdb; s=hdb.Session('hdb.db')"
```

## Module Layout

```text
python/
|- bindings/
|  |- CMakeLists.txt
|  `- module.cpp
|- src/hdb/__init__.py
`- pyproject.toml
```

## Python API

Entry point: `hdb.Session`

Methods:

- `sprout(name, actor, payload, meta=None)`
- `awaken(name)`
- `fire(name, actor, from, to, meta=None)`
- `consolidate(name, actor, neuron, payload, meta=None)`
- `resonate(stimulus, limit=10)`
- `reminisce(since_ticks, until_ticks)`
- `imagine(engram, start, epochs, creativity, impulse=None)`

## Data Conventions

- Binary fields (`actor`, `payload`, `meta`) are Python `bytes`.
- `moment` is exposed as `moment_ticks` (`Clock::duration::rep` cast to integer).
- `reminisce` returns a dict with `neurons` and `synapses`.
- `imagine` returns a list of `{ neuron, flux }` dictionaries.

## Planned Details

- Packaging and wheel strategy: maintained via scikit-build-core and CMake install rules
- Type hints and stubs: _내용 작성 예정_
- Compatibility policy across API revisions: _내용 작성 예정_
