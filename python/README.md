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

- Packaging and wheel strategy: _내용 작성 예정_
- Type hints and stubs: _내용 작성 예정_
- Compatibility policy across API revisions: _내용 작성 예정_
