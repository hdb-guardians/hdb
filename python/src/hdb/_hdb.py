from __future__ import annotations

from ._hdb_core import (
    Dream,
    DreamTable,
    Engram,
    Neuron,
    NeuronTable,
    Resonance,
    SynapseTable,
    Synapse,
    Thought,
)
from ._hdb_api import Session

try:
    from ._hdb_sqlite import open_sqlite

    def make_session(
        db_path: str,
        sqlite_vec_extension_path: str = "",
    ) -> Session:
        neurons, synapses, dreams = open_sqlite(db_path, sqlite_vec_extension_path)
        return Session(neurons, synapses, dreams)

except ImportError:
    pass

__all__ = [
    "Dream",
    "DreamTable",
    "Engram",
    "Neuron",
    "NeuronTable",
    "Resonance",
    "Session",
    "Synapse",
    "SynapseTable",
    "Thought",
]
