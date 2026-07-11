# HDB API

**Parent project**: [HDB](../README.md)  
**Depends on**: [core](../core)  
**License**: [GNU AGPL-3.0](../LICENSE)

## Responsibility

`api` is the session facade layer.

It composes the four core cognitive molecules — `Prefrontal`, `Thalamus`, `Hippocampus`, `Cortex` — into a single `Session` class.

It does not define:

- domain semantics (owned by `core`)
- any database engine integration (owned by `sqlite` or other backends)
- lifecycle or process orchestration

`api` is always built alongside `core`.

## Session

`hdb::api::Session` is the primary C++ entry point.

```cpp
#include <hdb/api/session.hpp>

hdb::api::Session session(neurons, synapses, dreams);
```

The constructor takes three `std::shared_ptr` to the core store interfaces:

- `std::shared_ptr<NeuronTable>`
- `std::shared_ptr<SynapseTable>`
- `std::shared_ptr<DreamTable>`

Ownership of the stores is shared. All four molecules are wired to the same store references at construction time.

## Public Operations

### Prefrontal — deliberate write

```cpp
std::optional<Neuron> Sprout(name, actor, payload, meta = nullopt)
std::optional<Neuron> Awaken(name)
std::optional<Synapse> Fire(name, actor, from, to, meta = nullopt)
```

| Method   | Description                                                                               |
| -------- | ----------------------------------------------------------------------------------------- |
| `Sprout` | Records a new neuron. Returns `nullopt` if insertion fails (e.g. duplicate name).         |
| `Awaken` | Retrieves an existing neuron by name. Returns `nullopt` if not found.                     |
| `Fire`   | Records a new directed synapse between two neurons. Returns `nullopt` if insertion fails. |

### Thalamus — consolidation

```cpp
std::optional<Dream> Consolidate(name, actor, neuron, payload, meta = nullopt)
```

Records a derived memory artifact anchored to a neuron. `payload` is a vector-searchable blob (e.g. an embedding).

### Hippocampus — retrieval

```cpp
std::vector<Resonance>  Resonate(stimulus, limit = 10)
std::optional<Engram>   Reminisce(since = Moment::min(), until = Moment::max())
```

| Method      | Description                                                                                                                                                 |
| ----------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Resonate`  | Returns the top-`limit` dreams whose payload is closest to `stimulus`. Each `Resonance` carries the matched neuron name and a `fidelity` score in `(0, 1]`. |
| `Reminisce` | Returns a subgraph of all neurons and synapses recorded in `[since, until]`. Returns `nullopt` if the store query fails.                                    |

### Cortex — simulation

```cpp
Imagination Imagine(engram, start, epochs, creativity, impulse)
```

Runs a graph walk from `start` through `engram` for `epochs` steps. `creativity` adds noise to propagation. `impulse` is a pluggable `std::function<Real(const Synapse&)>` edge-weighting policy. Pass a default `Impulse` for uniform weight `1.0`.

`Imagination` is `std::vector<Thought>`, where each `Thought` holds a `Neuron` and its activation `flux`.

## Error Model

Operations communicate failure through return types, not exceptions:

- `std::optional<T>` — `std::nullopt` means failure or not found
- `std::vector<T>` — empty vector means no result
