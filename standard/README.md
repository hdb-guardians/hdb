# HDB Standard Runtime

**Parent project**: [HDB](../README.md)  
**Depends on**: [core](../core)

## Responsibility

`standard` is the default SQLite-backed runtime for HDB.

It implements the storage interfaces defined in `core` and exposes a single runtime facade, `hdb::standard::Session`.

## Components

- `SqliteContext`: opens the SQLite database, sets pragmas, loads sqlite-vec, initializes schema
- `SqliteNeuronTable`: `NeuronTable` implementation
- `SqliteSynapseTable`: `SynapseTable` implementation
- `SqliteDreamTable`: `DreamTable` implementation
- `Session`: facade that wires `Prefrontal`, `Thalamus`, `Hippocampus`, and `Cortex`

## Planned Details

- `SqliteContext`: _내용 작성 예정_
- `SqliteNeuronTable`: _내용 작성 예정_
- `SqliteSynapseTable`: _내용 작성 예정_
- `SqliteDreamTable`: _내용 작성 예정_
- `Session`: _내용 작성 예정_

## Session API

The public API in [include/hdb/standard/session.hpp](include/hdb/standard/session.hpp):

- `Sprout(name, actor, payload, meta?)`
- `Awaken(name)`
- `Fire(name, actor, from, to, meta?)`
- `Consolidate(name, actor, neuron, payload, meta?)`
- `Resonate(stimulus, limit=10)`
- `Reminisce(since, until)`
- `Imagine(engram, start, epochs, creativity, impulse)`

## Data Model

Schema source: [sql/schema.sql](sql/schema.sql)

Tables:

1. `neurons`
2. `synapses`
3. `dreams`

Primary key and data style:

- each table uses `name TEXT PRIMARY KEY`
- payload fields are stored as `BLOB`
- `moment` is stored as `INTEGER` clock ticks (`Moment::duration::rep`)

Indices:

- `idx_neurons_moment`
- `idx_synapses_moment`
- `idx_synapses_from_to`
- `idx_dreams_moment`
- `idx_dreams_neuron`

## Dependencies

Build-time:

- SQLite3 (`find_package(SQLite3 REQUIRED)`)

Runtime:

- valid database path
- sqlite-vec extension path (loaded by `SqliteContext`)

## Behavior Notes

- `Session` constructor initializes the schema automatically.
- Database open or extension load failures can throw `std::runtime_error` from `SqliteContext`.
- Core methods still return `std::optional` and `std::vector` for operation-level success and emptiness.

## Out of Scope

The following concerns belong to [core/README.md](../core/README.md):

- domain type definitions
- storage interface contracts
- cognitive algorithm semantics
- compile-time type override options
