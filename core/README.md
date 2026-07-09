# HDB Core

**Parent project**: [HDB](../README.md)  
**License**: [GNU AGPL-3.0](../LICENSE)

---

## Philosophy

This core provides only what can be done. When, what, and how much to do are responsibilities of external layers.

- **Only human-confirmed thought links are stored.** There is no automatic linking. Neurons and synapses are created only when a person explicitly calls `Fire`.
- **All writes are append-only.** The past is never modified or deleted. Thought history is permanent. Even when payload is identical, a different moment means a different event.
- **Synapse has a single meaning.** It represents one idea: a human-confirmed thought connection. Type taxonomies dilute that meaning.
- **The C++ core does not know sleep/awake state.** Lifecycle orchestration belongs to external layers. The core executes when called.

---

## Header Layout

```text
core/include/
|- hdb.hpp                        # umbrella include
`- hdb/
   |- types.hpp                   # Id / Moment / Real / Natural - CMake-overridable
   |- quark/                      # primitive value objects - knows only types.hpp
   |  |- neuron.hpp
   |  |- synapse.hpp
   |  |- dream.hpp
   |  `- resonance.hpp
   |- atom/                       # composite value objects - depends on quark/
   |  |- engram.hpp
   |  |- impulse.hpp
   |  |- thought.hpp
   |  `- imagination.hpp
   |- store/                      # pure virtual interfaces
   |  |- neuron_table.hpp
   |  |- synapse_table.hpp
   |  `- dream_table.hpp
   `- molecule/                   # higher-order cognitive operations
      |- prefrontal.hpp
      |- thalamus.hpp
      |- hippocampus.hpp
      `- cortex.hpp
```

Dependencies flow in one direction only.

```text
types.hpp -> quark/ -> atom/ -> store/ -> molecule/
```

- `quark/` depends only on `types.hpp`.
- `atom/` depends on `quark/` (`Engram` uses `Neuron`/`Synapse`, `Thought` uses `Neuron`, `Impulse` uses `Synapse`).
- `store/` depends on `quark/` and `atom/` because interface return types are value objects.
- `molecule/` accesses data through `store/`.
- `Cortex` is a pure simulator that works directly on `Engram` and has no repository dependency.

---

## Type System

All four types are replaceable at compile time through CMake options.

| CMake option       | Default value               | Alias               |
| ------------------ | --------------------------- | ------------------- |
| `HDB_ID_TYPE`      | `std::string`               | `Nid`, `Sid`, `Did` |
| `HDB_CLOCK_TYPE`   | `std::chrono::system_clock` | `Moment`            |
| `HDB_REAL_TYPE`    | `float`                     | -                   |
| `HDB_NATURAL_TYPE` | `std::size_t`               | -                   |

### Id

Default is `std::string`. `Nid`/`Sid`/`Did` are the same underlying type for the compiler, but they make table intent obvious to readers. For performance-sensitive environments, you can switch with `-DHDB_ID_TYPE=uint64_t`.

The conceptual primary key of each record is the composite: `moment + actor + payload + meta`. `Nid`/`Sid`/`Did` are human-friendly handles for that composite fingerprint.

### Moment

Default is `std::chrono::system_clock::time_point`. Replacing `Clock` with `-DHDB_CLOCK_TYPE` changes `Moment` resolution and epoch together. If you need a wider range than standard chrono clocks provide, inject a custom clock type.

### Real

Floating-point real number. Default is `float`. Replacing with `-DHDB_REAL_TYPE=double` changes precision for `Impulse` weights, `Resonance.fidelity`, `Thought.flux`, and `Cortex` creativity parameters.

### Natural

Natural number including zero (ISO 80000-2). Default is `std::size_t`. It is used for counts and iteration parameters such as `limit` and `epochs`. You can replace it with `-DHDB_NATURAL_TYPE=uint32_t`.

### Variable Data - `std::vector<std::byte>`

`payload`, `actor`, and `meta` are raw byte streams. The core does not interpret content. Serialization and deserialization (embedding, text, JSON, etc.) are responsibilities of external layers.

---

## Schema

### Neuron

| Field                          | Intent                                                                                                                                         |
| ------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------- |
| `name: Nid`                    | `moment`, `actor`, `payload`, and `meta` together define an event. `name` is a practical representative handle for that composite fingerprint. |
| `actor: vector<byte>`          | The subject that emitted the thought. Thought must belong to someone.                                                                          |
| `payload: vector<byte>`        | Raw thought content. The core enforces no format.                                                                                              |
| `moment: Moment`               | Event timestamp. Same payload at a different moment is a different event.                                                                      |
| `meta: optional<vector<byte>>` | Optional auxiliary data interpreted by external layers.                                                                                        |

### Synapse

| Field                          | Intent                                                                                                               |
| ------------------------------ | -------------------------------------------------------------------------------------------------------------------- |
| `name: Sid`                    | `moment`, `actor`, `from`, `to`, and `meta` together define a connection event. `name` is its representative handle. |
| `actor: vector<byte>`          | The subject that confirmed the connection.                                                                           |
| `from: Nid`                    | Source neuron.                                                                                                       |
| `to: Nid`                      | Target neuron.                                                                                                       |
| `moment: Moment`               | Confirmation timestamp. Same endpoints at different moments are separate events.                                     |
| `meta: optional<vector<byte>>` | Optional connection metadata interpreted externally.                                                                 |

There is intentionally no synapse type (`EdgeType`). The connection means only one thing: a human-confirmed thought link.

### Dream

| Field                          | Intent                                                                                                                 |
| ------------------------------ | ---------------------------------------------------------------------------------------------------------------------- |
| `name: Did`                    | `moment`, `actor`, `neuron`, `payload`, and `meta` together define a dream event. `name` is its representative handle. |
| `actor: vector<byte>`          | The subject that produced the dream representation.                                                                    |
| `neuron: Nid`                  | Origin neuron. Every dream is rooted in a real thought.                                                                |
| `payload: vector<byte>`        | Raw embedding bytes. Any model output can be stored.                                                                   |
| `moment: Moment`               | Dream creation timestamp.                                                                                              |
| `meta: optional<vector<byte>>` | Optional metadata such as embedding model info.                                                                        |

Embedding generation is not the core's responsibility. External layers generate vectors and pass them through `Thalamus::Consolidate()`.

---

## Cognitive Operations

### Prefrontal

```text
Sprout(actor, payload, meta?) -> optional<Neuron>
Awaken(name)                  -> optional<Neuron>
Fire(actor, from, to, meta?)  -> optional<Synapse>
```

Neuroscience mapping: prefrontal cortex initiates conscious thought and decision-making.

- **Sprout**: Seeds a new thought into storage and returns `Neuron`.
- **Awaken**: Retrieves a dormant neuron and returns `Neuron`.
- **Fire**: Records a connection between two neurons and returns `Synapse`.

Constructor dependencies: `NeuronTable&`, `SynapseTable&`.

### Thalamus

```text
Consolidate(actor, neuron, payload, meta?) -> optional<Dream>
```

Neuroscience mapping: during sleep, hippocampal replay passes through thalamus to support cortical consolidation.

- **Consolidate**: Persists external embedding output as `Dream` in `DreamTable` and returns `Dream` on success.

Constructor dependency: `DreamTable&`.

### Hippocampus

```text
Resonate(stimulus, limit?) -> vector<Resonance>
Reminisce(since, until)    -> optional<Engram>
```

Neuroscience mapping: hippocampus indexes short-term to long-term memory with spatial and temporal context.

- **Resonate**: Finds nearest dreams to `stimulus` (embedding) via ANN search and returns `Resonance { neuron, fidelity }`.
- **Reminisce**: Retrieves all neurons and synapses in `[since, until]` as `Engram { neurons, synapses }`. Defaults are `Moment::min()` and `Moment::max()`.

Constructor dependencies: `NeuronTable&`, `SynapseTable&`, `DreamTable&`.

### Cortex

```text
using Impulse = std::function<Real(const Synapse&)>
Imagine(engram, start, epochs, creativity, impulse) -> Imagination
```

Neuroscience mapping: cortex abstracts memory patterns and reactivates them to form dream-like simulations.

- **Imagine** performs spreading activation over an `Engram`:

1. Build `neuron_map` and `synapse_map` (adjacency list).
2. Start from `start` with `flux = 1`.
3. For each epoch, apply `Impulse` to outgoing synapses, propagate flux, and add noise by `creativity`.
4. Accumulate flux across all epochs, sort descending, and return `Imagination`.

`Impulse` is an injected strategy for per-synapse weighting (decay, uniform, custom). `creativity` acts as an SNR control:

- `0`: deterministic behavior from `Impulse` only.
- `> 0`: adds noise and can activate weak links (divergent association).
- `< 0`: suppresses weak links and keeps strong links (convergent association).

If `start` is not in the engram, an empty `Imagination` is returned.

`Cortex` has no constructor dependency.

---

## Error Handling

No exceptions. Failure is represented by types.

| Return form   | Meaning                            |
| ------------- | ---------------------------------- |
| `optional<T>` | Single value, `nullopt` on failure |
| `vector<T>`   | Multiple values, empty on failure  |

---

## Technology Stack

| Role                 | Technology                    | Why                                                      |
| -------------------- | ----------------------------- | -------------------------------------------------------- |
| Facts + relations    | SQLite                        | Single-file portability and proven durability            |
| Semantic index + ANN | sqlite-vec                    | Vector search in the same SQLite file (no extra DB)      |
| Graph simulation     | C++ in-memory                 | `Cortex` runs directly on `Engram`; no graph DB required |
| Graph traversal      | SQLite CTE (`WITH RECURSIVE`) | Relational traversal without external graph DB           |

---

## Terminology

Method names are borrowed directly from neuroscience and cognitive science.

| Method                   | Meaning                                      |
| ------------------------ | -------------------------------------------- |
| `Prefrontal::Sprout`     | A new thought first appears                  |
| `Prefrontal::Awaken`     | A dormant memory becomes active              |
| `Prefrontal::Fire`       | Action-potential-like firing between neurons |
| `Thalamus::Consolidate`  | Memory consolidation flow                    |
| `Hippocampus::Resonate`  | Stimulus and memory alignment                |
| `Hippocampus::Reminisce` | Recalling a memory bundle                    |
| `Cortex::Imagine`        | Recombining memories into simulation         |

### Quantum-Inspired Terms

| Field                | Borrowed concept   | Meaning                              |
| -------------------- | ------------------ | ------------------------------------ | ---- | --- | ----------------------------------------------- |
| `Resonance.fidelity` | Quantum fidelity ` | <psi                                 | phi> | ^2` | Overlap between two states (stimulus vs memory) |
| `Thought.flux`       | Probability flux   | Activation flow at a simulation step |
| `Dream.neuron`       | Neuron reference   | Origin neuron of the dream           |

### Type Terms

| Type      | Borrowed concept | Meaning                                                 |
| --------- | ---------------- | ------------------------------------------------------- |
| `Real`    | Real number      | Continuous values such as fidelity, flux, creativity    |
| `Natural` | Natural number   | Count and iteration values (ISO 80000-2, includes zero) |
