from hdb._hdb_core import DreamTable, NeuronTable, SynapseTable

def open_sqlite(
    db_path: str = "human.db",
    sqlite_vec_extension_path: str = "",
    *,
    dream_dimension: int,
) -> tuple[NeuronTable, SynapseTable, DreamTable]: ...
