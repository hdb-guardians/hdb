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

class Session:
    def __init__(
        self,
        neurons: NeuronTable,
        synapses: SynapseTable,
        dreams: DreamTable,
    ) -> None: ...
    def sprout(
        self,
        name: str,
        actor: bytes,
        payload: bytes,
        meta: bytes | None = None,
    ) -> Neuron | None: ...
    def awaken(self, name: str) -> Neuron | None: ...
    def fire(
        self,
        name: str,
        actor: bytes,
        source: str,
        target: str,
        meta: bytes | None = None,
    ) -> Synapse | None: ...
    def consolidate(
        self,
        name: str,
        actor: bytes,
        neuron: str,
        payload: bytes,
        meta: bytes | None = None,
    ) -> Dream | None: ...
    def resonate(
        self,
        stimulus: bytes,
        limit: int = 10,
    ) -> list[Resonance]: ...
    def reminisce(
        self,
        since: datetime | None = None,
        until: datetime | None = None,
    ) -> Engram | None: ...
    def imagine(
        self,
        engram: Engram,
        start: str,
        epochs: int,
        creativity: float,
        impulse: Callable[[Synapse], float] | None = None,
    ) -> list[Thought]: ...
