# HDB Python Bindings

**Parent project**: [HDB](../README.md)  
**Depends on**: [core](../core), [api](../api), [sqlite](../sqlite)  
**License**: [GNU AGPL-3.0](../LICENSE)

## Responsibility

`python` exposes `hdb::api::Session` and `hdb::sqlite::open_sqlite` as Python extension modules via pybind11.

It focuses on:

- Python-friendly input/output conversion
- byte payload and meta transport via `bytes`
- session and store construction from Python

It does not define:

- domain semantics (owned by `core`)
- session or storage internals (owned by `api` and `sqlite`)

## Module Layout

```text
python/
├── bindings_api/        → builds _hdb (Session, abstract table types)
├── bindings_sqlite/     → builds _hdb_sqlite (open_sqlite)
├── src/hdb/
│   ├── __init__.py      → re-exports all public symbols
│   ├── _hdb.pyi         → type stubs for _hdb
│   └── _hdb_sqlite.pyi  → type stubs for _hdb_sqlite
└── pyproject.toml
```

## Development Flow (uv)

```powershell
# 1. Create venv and install dev dependencies
uv sync --project python --group dev

# 2. Configure CMake pointing at the venv interpreter
cmake -S . -B build `
  -DHDB_BUILD_SQLITE=ON `
  -DHDB_BUILD_PYTHON=ON `
  -DPython_EXECUTABLE="python/.venv/Scripts/python.exe"

# 3. Build
cmake --build build
```

This produces:

- `build/python/bindings_api/_hdb.<python-tag>.pyd`
- `build/python/bindings_sqlite/_hdb_sqlite.<python-tag>.pyd`

## Python API

### Store Construction

```python
import hdb

neurons, synapses, dreams = hdb.open_sqlite(db_path, sqlite_vec_extension_path="")
```

`sqlite_vec_extension_path` defaults to `""`, which causes `resolve_vec_extension_path` to fall back to the `HDB_SQLITE_VEC_EXTENSION` environment variable.

### Session

```python
session = hdb.Session(neurons, synapses, dreams)
```

#### Write (Prefrontal)

```python
session.sprout(name, actor, payload, meta=None)             -> dict | None
session.awaken(name)                                        -> dict | None
session.fire(name, actor, source, target, meta=None)        -> dict | None
```

#### Consolidate (Thalamus)

```python
session.consolidate(name, actor, neuron, payload, meta=None) -> dict | None
```

#### Retrieve (Hippocampus)

```python
session.resonate(stimulus, limit=10)                       -> list[tuple[str, float]]
session.reminisce(since_ticks=..., until_ticks=...)        -> dict | None
```

- `resonate` returns `[(neuron_name, fidelity), ...]` ordered by descending fidelity
- `reminisce` returns `{"neurons": [...], "synapses": [...]}`

#### Simulate (Cortex)

```python
session.imagine(engram, start, epochs, creativity, impulse=None) -> list[dict]
```

- `engram`: `{"neurons": [...], "synapses": [...]}`
- `impulse`: `Callable[[dict], float]` — edge-weighting function; `None` defaults to uniform weight `1.0`
- Returns `[{"neuron": {...}, "flux": float}, ...]`

## Data Conventions

| Field                       | Python type         | Notes                                |
| --------------------------- | ------------------- | ------------------------------------ |
| `actor`, `payload`, `meta`  | `bytes`             | Core treats these as opaque blobs    |
| `moment`                    | `moment_ticks: int` | `Clock::duration::rep` cast to `int` |
| synapse `source` / `target` | Python field names  | Match C++ struct fields after rename |

## Wheel Build

```powershell
uv sync --project python --group dev
uv build ./python --wheel -o ./dist
uv pip install --python python/.venv/Scripts/python.exe ./dist/hdb-*.whl
python/.venv/Scripts/python.exe -c "import hdb; print(hdb.Session)"
```

`python/CMakeLists.txt` bootstraps `core`, `api`, and `sqlite` automatically during standalone wheel builds (e.g. `scikit-build-core`), so the `python/` directory can be built independently.

## sqlite-vec Runtime Dependency

sqlite-vec is loaded at connection time by `open_sqlite`. Provide the path via:

1. Explicit argument: `hdb.open_sqlite(db_path, "/path/to/vec0.dll")`
2. Environment variable: `HDB_SQLITE_VEC_EXTENSION`

```powershell
$env:HDB_SQLITE_VEC_EXTENSION = "C:/path/to/vec0.dll"
python -c "import hdb; neurons, synapses, dreams = hdb.open_sqlite('hdb.db')"
```
