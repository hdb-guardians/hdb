# HDB Core

**Parent project**: [HDB](../README.md)  
**License**: [GNU AGPL-3.0](../LICENSE)

## Responsibility

`core` is the storage-agnostic domain layer.

It defines:

- common types (`Nid`, `Sid`, `Did`, `Moment`, `Real`, `Natural`)
- value objects (`Neuron`, `Synapse`, `Dream`, `Engram`, `Resonance`, `Thought`, `Imagination`)
- storage interfaces (`NeuronTable`, `SynapseTable`, `DreamTable`)
- cognitive operations (`Prefrontal`, `Thalamus`, `Hippocampus`, `Cortex`)

It does not define:

- any database engine integration
- vector extension loading
- process lifecycle orchestration

## Header Layout

```text
core/include/
|- hdb.hpp
`- hdb/
   |- types.hpp
   |- quark/
   |- atom/
   |- store/
   `- molecule/
```

Dependency direction:

```text
types.hpp -> quark/ -> atom/ -> store/ -> molecule/
```

## Configurable Types

The core supports type overrides via CMake cache variables:

- `HDB_ID_TYPE` (default: `std::string`)
- `HDB_CLOCK_TYPE` (default: `std::chrono::system_clock`)
- `HDB_REAL_TYPE` (default: `float`)
- `HDB_NATURAL_TYPE` (default: `std::size_t`)

## Borrowed Domains and Design Intent

HDB core borrows terms from neuroscience, cognitive science, and quantum mechanics intentionally.
These terms are used to keep boundaries explicit between facts, computation, and orchestration.

Core philosophical constraints:

- only explicit human-confirmed links are recorded (no automatic linking)
- writes are append-only (past thought events are not overwritten)
- synapse semantics stay single-purpose (directed confirmed relation)
- lifecycle control stays outside C++ core (core executes explicit calls)

This vocabulary is a contract, not decoration.

## Domain Layers

### Quark

The `quark` layer groups persistent value objects.

| Concept     | Field                  | Role                                  |
| ----------- | ---------------------- | ------------------------------------- |
| `Neuron`    | Neuroscience           | Minimal thought event                 |
| `Synapse`   | Neuroscience           | Directed relation                     |
| `Dream`     | Cognitive neuroscience | Derived memory artifact               |
| `Resonance` | Quantum mechanics      | Overlap-style similarity (`fidelity`) |

### Atom

The `atom` layer groups computation-ready aggregates and policies.

| Concept       | Field                     | Role                           |
| ------------- | ------------------------- | ------------------------------ |
| `Thought`     | Cognitive science         | Activated recall unit (`flux`) |
| `Imagination` | Cognitive simulation      | Propagation output             |
| `Impulse`     | Quantum mechanics         | Pluggable weighting policy     |
| `Engram`      | Neuroscience memory trace | Retrieval bundle               |

### Molecule

The `molecule` layer groups orchestration surfaces.

| Concept       | Field                        | Role                                     |
| ------------- | ---------------------------- | ---------------------------------------- |
| `Prefrontal`  | Prefrontal cortex            | Deliberate action surface                |
| `Thalamus`    | Thalamic relay/consolidation | Consolidation boundary                   |
| `Hippocampus` | Memory indexing/recall       | Retrieval boundary                       |
| `Cortex`      | Quantum mechanics            | Wave-function / entropy-style simulation |

## Domain Records

- `Neuron { name, actor, payload, moment, meta }`
- `Synapse { name, actor, from, to, moment, meta }`
- `Dream { name, actor, neuron, payload, moment, meta }`

`actor`, `payload`, and `meta` are byte-oriented (`std::vector<std::byte>`). The core does not interpret their content.

## Public Operations

The following signatures reflect the actual core API.

### Prefrontal

```text
Sprout(name, actor, payload, meta?) -> optional<Neuron>
Awaken(name)                        -> optional<Neuron>
Fire(name, actor, from, to, meta?)  -> optional<Synapse>
```

### Thalamus

```text
Consolidate(name, actor, neuron, payload, meta?) -> optional<Dream>
```

### Hippocampus

```text
Resonate(stimulus, limit=10) -> vector<Resonance>
Reminisce(since, until)      -> optional<Engram>
```

### Cortex

```text
Imagine(engram, start, epochs, creativity, impulse) -> Imagination
```

## Error Model

Core operations use return types to express failure:

- `std::optional<T>`: missing value means failure or no result
- `std::vector<T>`: empty vector means no result

## Out of Scope

The following concerns belong to [standard/README.md](../standard/README.md):

- SQLite schema and indices
- SQLite connection and pragmas
- sqlite-vec extension loading
- runtime facade construction (`hdb::standard::Session`)
