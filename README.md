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

HDB borrows terms from neuroscience, cognitive science, and signal/physics vocabulary intentionally.
The terms are used to lock boundaries between facts, computation, and orchestration.

### Core philosophical constraints (from historical core notes)

- only explicit human-confirmed links are recorded (no automatic linking)
- writes are append-only (past thought events are not overwritten)
- synapse semantics stay single-purpose (directed confirmed relation)
- lifecycle control stays outside C++ core (core executes explicit calls)

These constraints explain the naming policy: terminology in HDB is a contract, not decoration.

### Borrowed domain map

| Term          | Borrowed field                                                         | Borrowing intent                                          | Design intent                                                                                   |
| ------------- | ---------------------------------------------------------------------- | --------------------------------------------------------- | ----------------------------------------------------------------------------------------------- |
| `Neuron`      | Neuroscience                                                           | Represent minimal thought unit with provenance            | Persist a single thought event as `{name, actor, payload, moment, meta}`                        |
| `Synapse`     | Neuroscience                                                           | Model directed association between thoughts               | Persist explicit `from -> to` relation with actor/time                                          |
| `Dream`       | Cognitive neuroscience                                                 | Separate derived memory artifact from original event      | Store embedding-like payload tied to source `neuron`                                            |
| `Engram`      | Neuroscience memory trace                                              | Represent retrievable memory bundle                       | Package neurons/synapses with adjacency index for traversal                                     |
| `Resonance`   | Quantum information theory + signal similarity                         | Express cue-to-memory overlap as a fidelity-like quantity | Return retrieval pair `{neuron, fidelity}` using overlap-style scoring                          |
| `Thought`     | Cognitive science + quantum-mechanics vocabulary                       | Model activated recall unit with transferable magnitude   | Couple `Neuron` with activation `flux`                                                          |
| `Imagination` | Cognitive simulation                                                   | Model propagation output as a sequence                    | Return `std::vector<Thought>` from simulation                                                   |
| `Impulse`     | Signal transfer / physics                                              | Externalize propagation weighting rule                    | Use pluggable `std::function<Real(const Synapse&)>`                                             |
| `Prefrontal`  | Prefrontal cortex                                                      | Deliberate action surface                                 | Own explicit create/link operations (`Sprout`, `Awaken`, `Fire`)                                |
| `Thalamus`    | Thalamic relay/consolidation                                           | Consolidation-oriented write boundary                     | Persist derived artifacts via `Consolidate`                                                     |
| `Hippocampus` | Memory indexing/recall                                                 | Retrieval-oriented read boundary                          | Provide `Resonate` and `Reminisce` over stored records                                          |
| `Cortex`      | Cortical recombination + computational interpretation of wave dynamics | Simulation boundary independent from storage              | Execute in-memory propagation (`Imagine`) on `Engram` as wave-function/entropy-style simulation |

### Quark and Atom structure

The `quark` and `atom` terms are boundary labels for data and computation.

#### Quark (`core/include/hdb/quark`)

| Concept     | Structure                                        | Design emphasis                                 |
| ----------- | ------------------------------------------------ | ----------------------------------------------- |
| `Neuron`    | `{ name, actor, payload, moment, meta }`         | Event fact with provenance and time             |
| `Synapse`   | `{ name, actor, from, to, moment, meta }`        | Explicit directed relation; no auto-edge typing |
| `Dream`     | `{ name, actor, neuron, payload, moment, meta }` | Derived artifact anchored to a source event     |
| `Resonance` | `{ neuron, fidelity }`                           | Retrieval overlap result                        |

Quark design intent:

- preserve event facts and retrieval outcomes with minimal interpretation
- keep `actor`, `payload`, `meta` as byte-oriented data for external-layer interpretation
- treat same payload at different moments as different events (time is semantic identity)

#### Atom (`core/include/hdb/atom`)

| Concept       | Structure                               | Design emphasis                       |
| ------------- | --------------------------------------- | ------------------------------------- |
| `Thought`     | `{ neuron, flux }`                      | Activated recall unit                 |
| `Imagination` | `std::vector<Thought>`                  | Simulation result container           |
| `Impulse`     | `std::function<Real(const Synapse&)>`   | Pluggable edge-weight policy          |
| `Engram`      | `neurons + synapses + adjacency_by_nid` | Indexed recall bundle for propagation |

Atom design intent:

- define computation-ready aggregates on top of quark facts
- pre-index traversal structure in `Engram` to keep propagation local and efficient
- keep propagation strategy externalized (`Impulse`) rather than hard-coding one decay law

### Quantum-mechanics vocabulary and computational interpretation

- `fidelity` is borrowed from quantum information language to express overlap between a stimulus state and a recalled state.
- `flux` is borrowed as a flow/intensity term for propagated activation mass in traversal.
- `Cortex::Imagine` is treated as a computer-science interpretation of wave-function-like propagation and entropy control:
  - propagation over adjacency approximates state evolution on a graph
  - normalization/weighting behavior models distribution shaping
  - creativity controls exploration entropy (higher: broader stochastic spread, lower: more concentrated trajectories)

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
