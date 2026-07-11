# HDB Core

**Parent project**: [HDB](../README.md)  
**License**: [GNU AGPL-3.0](../LICENSE)

## Responsibility

`core` is the storage-agnostic domain layer.

It defines:

- common types (`Nid`, `Sid`, `Did`, `Moment`, `Real`)
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

## Fixed Types

All primitive types are fixed at compile time.

| Type     | Definition                                                  | Aliases             |
| -------- | ----------------------------------------------------------- | ------------------- |
| `Id`     | `std::string`                                               | `Nid`, `Sid`, `Did` |
| `Clock`  | `std::chrono::system_clock`                                 |                     |
| `Moment` | `std::chrono::time_point<Clock, std::chrono::microseconds>` |                     |
| `Real`   | `double`                                                    |                     |

`Nid`, `Sid`, `Did` are aliases of `Id`. To the compiler they are the same type; to the reader they signal which table a value belongs to.

`Moment` uses microsecond resolution. Append-only semantics depend on `Moment`: the same thought recurring at a later moment is a new event, even if `actor` and `payload` are identical.

`Real` is used for `Resonance.fidelity`, `Thought.flux`, `creativity`, and `Impulse` weights in `Cortex::Imagine`.

### Raw bytes — `std::vector<std::byte>`

`actor`, `payload`, and `meta` are raw byte streams throughout. The core does not interpret their content. Encoding, serialization, and embedding generation are the responsibility of the outer layer.

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

### Neuron

| Field                          | Intent                                                                                                                                                                            |
| ------------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `name: Nid`                    | Human-readable handle for the `(moment, actor, payload, meta)` composite fingerprint.                                                                                             |
| `actor: vector<byte>`          | The subject that fired this thought. A thought must belong to someone — without an actor, origin and accountability are unknown. The core does not require the actor to be human. |
| `payload: vector<byte>`        | The actual content of the thought. The core imposes no format — text, embedding, JSON, or anything else. Interpretation is the outer layer's responsibility.                      |
| `moment: Moment`               | When the thought occurred. The same `payload` and `actor` at a different moment is a distinct event: the same thought recurring later is a new thought.                           |
| `meta: optional<vector<byte>>` | Auxiliary data attached to the thought. The core does not read it.                                                                                                                |

### Synapse

| Field                          | Intent                                                                                                               |
| ------------------------------ | -------------------------------------------------------------------------------------------------------------------- |
| `name: Sid`                    | Handle for the `(moment, actor, source, target, meta)` composite.                                                    |
| `actor: vector<byte>`          | The subject that confirmed this connection. The same two neurons connected by different actors are different events. |
| `source: Nid`                  | The neuron that fires.                                                                                               |
| `target: Nid`                  | The neuron that receives.                                                                                            |
| `moment: Moment`               | When this connection was confirmed. Two synapses between the same neurons at different moments are distinct records. |
| `meta: optional<vector<byte>>` | Auxiliary data. The core does not read it.                                                                           |

There is no edge type. The synapse carries one meaning only: a human-confirmed directed connection between two thoughts. Adding a type field would force the core to impose semantics on relationships.

### Dream

| Field                          | Intent                                                                                                                                                                     |
| ------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `name: Did`                    | Handle for the `(moment, actor, neuron, payload, meta)` composite.                                                                                                         |
| `actor: vector<byte>`          | The subject that produced this dream. The same neuron abstracted by different actors yields different perspectives.                                                        |
| `neuron: Nid`                  | The source neuron this dream originates from. A dream must be rooted in a real thought.                                                                                    |
| `payload: vector<byte>`        | Semantic vector (embedding) raw bytes. The sqlite-vec ANN search operates on this field. The core does not generate embeddings — that is the outer layer's responsibility. |
| `moment: Moment`               | When this dream was created.                                                                                                                                               |
| `meta: optional<vector<byte>>` | Auxiliary data (e.g. embedding model identifier). The core does not read it.                                                                                               |

## Public Operations

The following signatures reflect the actual core API.

### Prefrontal

```text
Sprout(name, actor, payload, meta?) -> optional<Neuron>
Awaken(name)                        -> optional<Neuron>
Fire(name, actor, source, target, meta?) -> optional<Synapse>
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
using Impulse = std::function<Real(const Synapse&)>

Imagine(engram, start, epochs, creativity, impulse) -> Imagination
```

`Imagine` performs spreading activation over an `Engram`:

1. Initialize flux map: `start → 1.0`
2. Per epoch: for each active node, apply `Impulse` to each outgoing synapse and propagate flux to `target`; add `creativity × noise` to each contribution
3. Accumulate flux across all epochs
4. Normalize (softmax over max-shifted flux) and sort descending → `Imagination`

`creativity` is a signal-to-noise knob:

- `0` — deterministic; only `Impulse` weight determines propagation
- `> 0` — noise added; weaker connections can activate (divergent association)
- `< 0` — noise subtracted; only strong connections survive (convergent association)

If `start` is not in the Engram, returns an empty `Imagination`. `Cortex` has no constructor dependencies.

## Error Model

Core operations use return types to express failure:

- `std::optional<T>`: missing value means failure or no result
- `std::vector<T>`: empty vector means no result

## Vocabulary

Method names are drawn from neuroscience, cognitive science, and quantum mechanics. The vocabulary is a contract, not decoration — it keeps cognitive operation boundaries explicit.

| Method                   | Korean     | Rationale                                   |
| ------------------------ | ---------- | ------------------------------------------- |
| `Prefrontal::Sprout`     | 싹트다     | A new thought event begins                  |
| `Prefrontal::Awaken`     | 깨어나다   | A dormant memory node is activated          |
| `Prefrontal::Fire`       | 발화하다   | Neuroscience action potential               |
| `Thalamus::Consolidate`  | 공고화하다 | Sleep-phase memory consolidation            |
| `Hippocampus::Resonate`  | 공명하다   | Stimulus-to-memory frequency matching       |
| `Hippocampus::Reminisce` | 회상하다   | Retrieve a time-bounded chunk of memory     |
| `Cortex::Imagine`        | 상상하다   | Recombine stored memory into new simulation |

Quantum mechanics terms used in value objects:

| Field                | Borrowed concept           | Meaning                                                |
| -------------------- | -------------------------- | ------------------------------------------------------ |
| `Resonance.fidelity` | Quantum fidelity `⟨ψ\|φ⟩²` | Overlap between stimulus state and stored memory state |
| `Thought.flux`       | Probability flux           | Activation share flowing through a node per epoch      |

## Out of Scope

The following concerns belong to [sqlite/README.md](../sqlite/README.md):

- SQLite schema and indices
- SQLite connection and pragmas
- sqlite-vec extension loading

The following concerns belong to [api/README.md](../api/README.md):

- Session facade construction (`hdb::api::Session`)
- Molecule wiring and store ownership
