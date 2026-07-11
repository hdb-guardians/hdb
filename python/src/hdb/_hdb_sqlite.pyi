from __future__ import annotations

from ._hdb import DreamTable, NeuronTable, SynapseTable

def open_sqlite(
    db_path: str,
    sqlite_vec_extension_path: str = "",
) -> tuple[NeuronTable, SynapseTable, DreamTable]: ...
