from __future__ import annotations

from collections.abc import Callable
from datetime import datetime

from ._hdb_core import (
    Dream,
    DreamTable,
    Engram,
    Neuron,
    NeuronTable,
    Resonance,
    Synapse,
    SynapseTable,
    Thought,
)
from ._hdb_api import Session

def open_sqlite(
    db_path: str,
    sqlite_vec_extension_path: str = "",
) -> tuple[NeuronTable, SynapseTable, DreamTable]: ...
def make_session(
    db_path: str,
    sqlite_vec_extension_path: str = "",
) -> Session: ...
