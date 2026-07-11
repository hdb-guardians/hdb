from __future__ import annotations

from collections.abc import Callable

class NeuronTable: ...
class SynapseTable: ...
class DreamTable: ...

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
    ) -> dict | None: ...
    def awaken(self, name: str) -> dict | None: ...
    def fire(
        self,
        name: str,
        actor: bytes,
        source: str,
        target: str,
        meta: bytes | None = None,
    ) -> dict | None: ...
    def consolidate(
        self,
        name: str,
        actor: bytes,
        neuron: str,
        payload: bytes,
        meta: bytes | None = None,
    ) -> dict | None: ...
    def resonate(
        self,
        stimulus: bytes,
        limit: int = 10,
    ) -> list[tuple[str, float]]: ...
    def reminisce(
        self,
        since_ticks: int = ...,
        until_ticks: int = ...,
    ) -> dict | None: ...
    def imagine(
        self,
        engram: dict,
        start: str,
        epochs: int,
        creativity: float,
        impulse: Callable[[dict], float] | None = None,
    ) -> list[dict]: ...
