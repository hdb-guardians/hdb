import os

from hdb._hdb_core import DreamTable, NeuronTable, SynapseTable

try:
    from hdb._hdb_sqlite import open_sqlite as _open_sqlite

    def open_sqlite(
        db_path: str = "human.db",
        sqlite_vec_extension_path: str = "",
        *,
        dream_dimension: int,
    ) -> tuple[NeuronTable, SynapseTable, DreamTable]:
        if (
            not sqlite_vec_extension_path
            and "HDB_SQLITE_VEC_EXTENSION" not in os.environ
        ):
            import sqlite_vec

            sqlite_vec_extension_path = sqlite_vec.loadable_path()
        return _open_sqlite(db_path, sqlite_vec_extension_path, dream_dimension)
except ImportError:
    pass

__all__ = ["DreamTable", "NeuronTable", "SynapseTable", "open_sqlite"]
