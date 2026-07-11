# HDB SQLite

**Parent project**: [HDB](../README.md)  
**Depends on**: [core](../core)  
**License**: [GNU AGPL-3.0](../LICENSE)

## Responsibility

`sqlite` provides SQLite-backed implementations of the three core store interfaces.

It owns:

- database connection lifecycle (`SqliteContext`)
- schema initialization
- sqlite-vec extension loading (required for Dream vector search)
- the `open_sqlite` factory function

It does not define:

- domain semantics (owned by `core`)
- session orchestration (owned by `api`)

Enabled via CMake option `HDB_BUILD_SQLITE` (default: `ON`).

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

| Class                | Interface      | Notable detail                                       |
| -------------------- | -------------- | ---------------------------------------------------- |
| `SqliteNeuronTable`  | `NeuronTable`  | Primary key on `name`; range query on `moment`       |
| `SqliteSynapseTable` | `SynapseTable` | Primary key on `name`; indexed on `(from_id, to_id)` |
| `SqliteDreamTable`   | `DreamTable`   | `find()` uses `vec_distance_l2` from sqlite-vec      |

### Resonance Formula

Dream similarity search (implemented in `SqliteDreamTable::find`) uses:

$$\text{fidelity} = \frac{1}{1 + \text{vec\_distance\_l2}(\text{payload},\, \text{stimulus})}$$

`fidelity` is in `(0, 1]`. Higher values mean closer vectors.

### open_sqlite

```cpp
#include <hdb/sqlite/open.hpp>

auto [neurons, synapses, dreams] =
    hdb::sqlite::open_sqlite(db_path, sqlite_vec_extension_path);
```

Returns `std::tuple<shared_ptr<NeuronTable>, shared_ptr<SynapseTable>, shared_ptr<DreamTable>>`.

Pass these directly to `hdb::api::Session`:

```cpp
hdb::api::Session session(neurons, synapses, dreams);
```

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
    from_id TEXT NOT NULL,
    to_id   TEXT NOT NULL,
    moment  INTEGER NOT NULL,
    meta    BLOB
);

CREATE TABLE dreams (
    name    TEXT PRIMARY KEY,
    actor   BLOB NOT NULL,
    neuron  TEXT NOT NULL,
    payload BLOB NOT NULL,   -- vector blob consumed by sqlite-vec
    moment  INTEGER NOT NULL,
    meta    BLOB
);
```

Indices: `neurons(moment)`, `synapses(moment)`, `synapses(from_id, to_id)`, `dreams(moment)`, `dreams(neuron)`.

## Build

```bash
cmake -S . -B build -DHDB_BUILD_SQLITE=ON
cmake --build build
```

Requires: SQLite3 development package (`find_package(SQLite3 REQUIRED)`).
