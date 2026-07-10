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

Configure:

```bash
cmake -S . -B build -DHDB_BUILD_STANDARD=ON -DHDB_BUILD_PYTHON=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Build:

```bash
cmake --build build
```

Root CMake options:

- `HDB_BUILD_STANDARD` (default: `ON`)
- `HDB_BUILD_PYTHON` (default: `OFF`)

## Documentation

- Core scope and API: [core/README.md](core/README.md)
- Standard runtime scope and API: [standard/README.md](standard/README.md)

## Borrowed Domains, Borrowing Intent, and Design Intent

HDB borrows terms from neuroscience, cognitive science, and quantum mechanics intentionally.
The terms are used to lock boundaries between facts, computation, and orchestration.

### Core philosophical constraints (from historical core notes)

- only explicit human-confirmed links are recorded (no automatic linking)
- writes are append-only (past thought events are not overwritten)
- synapse semantics stay single-purpose (directed confirmed relation)
- lifecycle control stays outside C++ core (core executes explicit calls)

These constraints explain the naming policy: terminology in HDB is a contract, not decoration.

## Quark

The `quark` layer groups persistent value objects.

| Concept     | Field                  | Role                                  |
| ----------- | ---------------------- | ------------------------------------- |
| `Neuron`    | Neuroscience           | Minimal thought event                 |
| `Synapse`   | Neuroscience           | Directed relation                     |
| `Dream`     | Cognitive neuroscience | Derived memory artifact               |
| `Resonance` | Quantum mechanics      | Overlap-style similarity (`fidelity`) |

### `Neuron`

_내용 작성 예정_

### `Synapse`

_내용 작성 예정_

### `Dream`

_내용 작성 예정_

### `Resonance`

_내용 작성 예정_

## Atom

The `atom` layer groups computation-ready aggregates and policies.

| Concept       | Field                     | Role                           |
| ------------- | ------------------------- | ------------------------------ |
| `Thought`     | Cognitive science         | Activated recall unit (`flux`) |
| `Imagination` | Cognitive simulation      | Propagation output             |
| `Impulse`     | Quantum mechanics         | Pluggable weighting policy     |
| `Engram`      | Neuroscience memory trace | Retrieval bundle               |

### `Thought`

_내용 작성 예정_

### `Imagination`

_내용 작성 예정_

### `Impulse`

_내용 작성 예정_

### `Engram`

_내용 작성 예정_

## Molecule

The `molecule` layer groups orchestration surfaces.

| Concept       | Field                        | Role                                     |
| ------------- | ---------------------------- | ---------------------------------------- |
| `Prefrontal`  | Prefrontal cortex            | Deliberate action surface                |
| `Thalamus`    | Thalamic relay/consolidation | Consolidation boundary                   |
| `Hippocampus` | Memory indexing/recall       | Retrieval boundary                       |
| `Cortex`      | Quantum mechanics            | Wave-function / entropy-style simulation |

### `Prefrontal`

_내용 작성 예정_

### `Thalamus`

_내용 작성 예정_

### `Hippocampus`

_내용 작성 예정_

### `Cortex`

_내용 작성 예정_

### Why this vocabulary improves architecture

- it keeps `core` storage-agnostic but semantically strict
- it lets `standard` implement persistence/runtime details without redefining domain meaning
- it leaves embedding generation and lifecycle orchestration to external layers (including Python)

## Stewardship

HDB is stewarded by the **HDB Guardians**.

- Organization: https://github.com/hdb-guardians
- Founding guardian: Kyoung Chan Park (Park, Kyoung Chan), DPChanny @ DPC Studio
- Email: kyoungchan0516@gmail.com
- Phone: +82-10-8246-4691

Stewardship is for governance and continuity. Licensing terms are defined only by the AGPL-3.0 text in [LICENSE](LICENSE).
