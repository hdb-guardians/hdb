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

`Session` unifies all four cognitive molecules into a single surface. See [core/README.md](../core/README.md) for semantics.

### Prefrontal

```cpp
std::optional<Neuron> Sprout(name, actor, payload, meta = nullopt)
std::optional<Neuron> Awaken(name)
std::optional<Synapse> Fire(name, actor, from, to, meta = nullopt)
```

### Thalamus

```cpp
std::optional<Dream> Consolidate(name, actor, neuron, payload, meta = nullopt)
```

### Hippocampus

```cpp
std::vector<Resonance>  Resonate(stimulus, limit = 10)
std::optional<Engram>   Reminisce(since = Moment::min(), until = Moment::max())
```

### Cortex

```cpp
Imagination Imagine(engram, start, epochs, creativity, impulse)
```
