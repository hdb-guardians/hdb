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

## Terminology and Design Intent

HDB borrows terms from neuroscience and cognitive science on purpose.
The naming is not decorative. It defines boundaries between responsibilities.

| Term | Borrowed from | Why this term | Design intent |
| --- | --- | --- | --- |
| `Prefrontal` | Prefrontal cortex | Associated with deliberate action and decision | Explicitly creates and links thought events (`Sprout`, `Fire`) |
| `Thalamus` | Thalamic relay in memory pathways | Associated with routing and consolidation flow | Persists derived representations (`Consolidate`) without changing source thought semantics |
| `Hippocampus` | Memory indexing and recall | Associated with retrieval by cue and time | Provides search and recall interfaces (`Resonate`, `Reminisce`) over stored events |
| `Cortex` | Pattern recombination and abstraction | Associated with simulation from memory traces | Runs in-memory propagation (`Imagine`) independent of storage backend |
| `Neuron` | Atomic unit in a network | Smallest named thought event | Represents a single stored thought with actor, payload, and moment |
| `Synapse` | Connection between neurons | Directed relation between thought events | Encodes a human-confirmed link (`from -> to`) with explicit authorship/time |
| `Dream` | Consolidated/derived memory artifact | Distinguishes derived vectors from original thoughts | Stores embedding-like payloads while preserving provenance via `neuron` |
| `Engram` | Memory trace concept | Cohesive recall bundle | Packages neurons and synapses from a recall window for downstream simulation |
| `Resonance` | Similarity/overlap notion | Cue-to-memory matching result | Returns matched neuron plus score (`fidelity`) |
| `Impulse` / `flux` | Signal propagation vocabulary | Models weighted activation transfer | Makes propagation strategy explicit and pluggable in `Imagine` |

Design principles behind the terminology:

- Keep the domain language close to the cognitive process being modeled.
- Separate event facts (`Neuron`, `Synapse`) from derived artifacts (`Dream`).
- Keep orchestration explicit at API level (no hidden automatic linking).
- Preserve storage-agnostic semantics in `core`, runtime-specific behavior in `standard`.

## Stewardship

HDB is stewarded by the **HDB Guardians**.

- Organization: https://github.com/hdb-guardians
- Founding guardian: Kyoung Chan Park (Park, Kyoung Chan), DPChanny @ DPC Studio
- Email: kyoungchan0516@gmail.com
- Phone: +82-10-8246-4691

Stewardship is for governance and continuity. Licensing terms are defined only by the AGPL-3.0 text in [LICENSE](LICENSE).
