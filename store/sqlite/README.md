# HDB SQLite

**Parent project**: [HDB](../../README.md)  
**Depends on**: [core](../../core)  
**License**: [GNU AGPL-3.0](../../LICENSE)

## Responsibility

`store/sqlite` provides SQLite-backed implementations of the three core store interfaces.

It owns:

- database connection lifecycle (`SqliteContext`)
- schema initialization
- sqlite-vec extension loading (required for Dream vector search)
- the `open_sqlite` factory function

It does not define:

- domain semantics (owned by `core`)

Enabled via CMake option `HDB_BUILD_STORE` (default: `ON`).

## Components

### SqliteContext

`hdb::sqlite::SqliteContext` manages a single SQLite connection.

On construction it:

- opens the database file (creates it if absent, `SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE`)
- enables WAL journal mode
- enables foreign key enforcement
- loads the sqlite-vec extension (required; throws `std::runtime_error` if loading fails)

`initialize_schema()` creates the `neurons`, `synapses`, and `dreams` tables and all necessary indices. It is idempotent (`CREATE TABLE IF NOT EXISTS`).

### Store Implementations

Each class implements the corresponding core abstract interface via a shared `SqliteContext`:

| Class                | Interface      | Notable detail                                                           |
| -------------------- | -------------- | ------------------------------------------------------------------------ |
| `SqliteNeuronTable`  | `NeuronTable`  | Primary key on `name`; range query on `moment`                           |
| `SqliteSynapseTable` | `SynapseTable` | Primary key on `name`; indexed on `(source, target)`                     |
| `SqliteDreamTable`   | `DreamTable`   | `find()` uses sqlite-vec's `vec0` virtual table (`dreams_vec`) KNN index |

### Resonance Formula

Dream similarity search (implemented in `SqliteDreamTable::find`) uses:

```
fidelity = 1.0 / (1.0 + distance)
```

where `distance` is the L2 distance reported by the `dreams_vec` `vec0` KNN
query (`payload MATCH ?1 AND k = ?2`). `fidelity` is in `(0, 1]`. Higher
values mean closer vectors.

### open_sqlite

```cpp
#include <hdb/sqlite/open.hpp>

auto [neurons, synapses, dreams] =
    hdb::sqlite::open_sqlite(db_path, sqlite_vec_extension_path, dream_dimension);
```

Returns `std::tuple<shared_ptr<NeuronTable>, shared_ptr<SynapseTable>, shared_ptr<DreamTable>>`.

`dream_dimension` is the number of float32 components in each dream
`payload` vector. It is required and must be non-zero — sqlite-vec's `vec0`
virtual table needs a fixed vector width to build its index, so it cannot be
inferred at runtime from the first inserted row.

### resolve_vec_extension_path

```cpp
std::string path = hdb::sqlite::resolve_vec_extension_path(hint);
```

Resolution order:

1. `hint` argument (if non-empty)
2. `HDB_SQLITE_VEC_EXTENSION` environment variable

Throws `std::runtime_error` if neither is set.

## Schema

```sql
CREATE TABLE neurons (
    name    TEXT PRIMARY KEY,
    actor   BLOB NOT NULL,
    payload BLOB NOT NULL,
    moment  INTEGER NOT NULL,   -- Clock::duration::rep ticks
    meta    BLOB
);

CREATE TABLE synapses (
    name    TEXT PRIMARY KEY,
    actor   BLOB NOT NULL,
    source  TEXT NOT NULL,
    target  TEXT NOT NULL,
    moment  INTEGER NOT NULL,
    meta    BLOB
);

CREATE TABLE dreams (
    name    TEXT PRIMARY KEY,
    actor   BLOB NOT NULL,
    neuron  TEXT NOT NULL,
    moment  INTEGER NOT NULL,
    meta    BLOB
);

-- sqlite-vec vec0 virtual table; joined to `dreams` by rowid.
-- `float[N]` width is fixed at schema-creation time (`dream_dimension`).
CREATE VIRTUAL TABLE dreams_vec USING vec0(
    payload float[N]
);
```

Indices: `neurons(moment)`, `synapses(moment)`, `synapses(source, target)`, `dreams(moment)`, `dreams(neuron)`. `dreams_vec` maintains its own internal ANN index over `payload`.

`synapses(source, target)` and `dreams(neuron)` are not read by any query `SynapseTable`/`DreamTable` currently expose (only `find_by_id` and `find_by_range` on `moment`). They are kept ahead of need because the write-time cost is negligible (single-row inserts, no bulk load path) and they directly enable likely near-term additions:

- `synapses(source, target)`:
  - outgoing/incoming edge lookup (`WHERE source = ?` / `WHERE target = ?`) for graph traversal directly from a neuron, instead of loading a full moment range via `find_by_range` and walking it in memory
  - listing all confirmations recorded for a specific `(source, target)` pair — repeated `Prefrontal::Fire` calls on the same pair are intentional (re-confirmation over time) and each produces a distinct row, so this is a listing, not a dedup/existence check
- `dreams(neuron)`:
  - reverse lookup "all dreams derived from neuron X" (`WHERE neuron = ?`), the mirror direction of the existing vector-KNN → neuron join in `find()`
  - repeated `Thalamus::Consolidate` calls on the same neuron are likewise intentional (distinct perspectives across actors/moments), so this index serves listing, not preventing re-consolidation

## Build

```bash
cmake -S . -B build -DHDB_BUILD_STORE=ON
cmake --build build
```

SQLite3 is vendored via `FetchContent` and statically linked (see `cmake/vendor_sqlite.cmake`) — no system SQLite3 package is required.
