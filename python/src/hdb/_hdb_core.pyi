from __future__ import annotations

from collections.abc import Callable
from datetime import datetime

class Neuron:
    name: str
    actor: bytes
    payload: bytes
    moment: datetime
    meta: bytes | None
    def __init__(self) -> None: ...

class Synapse:
    name: str
    actor: bytes
    source: str
    target: str
    moment: datetime
    meta: bytes | None
    def __init__(self) -> None: ...

class Dream:
    name: str
    actor: bytes
    neuron: str
    payload: bytes
    moment: datetime
    meta: bytes | None
    def __init__(self) -> None: ...

class Resonance:
    neuron: str
    fidelity: float
    def __init__(self) -> None: ...

class Thought:
    neuron: Neuron
    flux: float
    def __init__(self) -> None: ...

class Engram:
    @property
    def neurons(self) -> list[Neuron]: ...
    @property
    def synapses(self) -> list[Synapse]: ...
    def __init__(
        self,
        neurons: list[Neuron] = ...,
        synapses: list[Synapse] = ...,
    ) -> None: ...

class NeuronTable:
    def __init__(self) -> None: ...
    def insert(self, neuron: Neuron) -> Neuron | None: ...
    def find_by_id(self, id: str) -> Neuron | None: ...
    def find_by_range(self, since: datetime, until: datetime) -> list[Neuron]: ...

class SynapseTable:
    def __init__(self) -> None: ...
    def insert(self, synapse: Synapse) -> Synapse | None: ...
    def find_by_id(self, id: str) -> Synapse | None: ...
    def find_by_range(self, since: datetime, until: datetime) -> list[Synapse]: ...

class DreamTable:
    def __init__(self) -> None: ...
    def insert(self, dream: Dream) -> Dream | None: ...
    def find(self, payload: bytes, limit: int = 10) -> list[Resonance]: ...

class Prefrontal:
    def __init__(self, neurons: NeuronTable, synapses: SynapseTable) -> None: ...
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

class Thalamus:
    def __init__(self, dreams: DreamTable) -> None: ...
    def consolidate(
        self,
        name: str,
        actor: bytes,
        neuron: str,
        payload: bytes,
        meta: bytes | None = None,
    ) -> Dream | None: ...

class Hippocampus:
    def __init__(
        self,
        neurons: NeuronTable,
        synapses: SynapseTable,
        dreams: DreamTable,
    ) -> None: ...
    def resonate(self, stimulus: bytes, limit: int = 10) -> list[Resonance]: ...
    def reminisce(
        self,
        since: datetime | None = None,
        until: datetime | None = None,
    ) -> Engram | None: ...

class Cortex:
    def __init__(self) -> None: ...
    def imagine(
        self,
        engram: Engram,
        start: str,
        epochs: int,
        creativity: float,
        impulse: Callable[[Synapse], float] | None = None,
    ) -> list[Thought]: ...
