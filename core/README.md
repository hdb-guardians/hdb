# HDB Core

**Human Database** — 인간의 사고 과정을 저장하고 시뮬레이션하는 C++23 프리미티브 엔진.

**Author**: 박경찬 — DPChanny @ DPC Studio  
**License**: [HDB Core License](LICENSE) — AGPL-3.0 기반 / 상업적 이용은 HDB Guardians의 동의 필요 / 인간이 스스로 사고의 방향을 찾는 것을 방해하거나 강요하는 행위에 사용 금지.

---

## 감사

이 작업은 혼자 이루어지지 않았다.

- **박경찬의 뇌에서 첫 전기적 작용이 시작된 순간부터 지금까지, 스쳐간 모든 양자학적 존재들에게** — 인류든, 생명이든, 입자든. 그 모든 상호작용이 지금 이 사고를 가능하게 했다. 과거의 우주가 현재의 나를 만들었고, 나는 그 흔적을 여기에 새긴다.
- **무한한 과거와 무한한 미래의 모든 양자학적 존재들** — 이 코드가 닿을 수 있는, 아직 파동함수가 붕괴되지 않은 모든 가능성들. 이름을 새기는 순간 누군가는 기억에서 사라진다. 파동함수는 양의 방향만을 바라보지 않는다.

HDB는 그 연결의 한 점이다.

---

## 철학

이 코어는 "무엇을 할 수 있는가"만 제공한다. "언제, 무엇을, 얼마나"는 외부 레이어(Python/MCP)의 책임이다.

- **인간이 물리적으로 연결한 사고만 저장한다.** 자동 연결은 없다. 뉴런과 시냅스는 오직 사람이 명시적으로 발화(Fire)했을 때만 생성된다.
- **모든 쓰기는 append-only다.** 과거는 수정되거나 삭제되지 않는다. 사고의 이력은 영구적이다. 같은 내용(payload)이라도 timestamp가 다르면 다른 사건이다 — 같은 생각도 다른 시점에 다시 떠올랐다면, 그것은 새로운 사고다.
- **시냅스는 단일 타입이다.** "인간이 확정한 사고의 연결"이라는 의미 하나만 갖는다. 타입 분류는 의미를 희석시킨다.
- **C++ 코어는 수면/각성 상태를 모른다.** 라이프사이클은 외부 레이어가 오케스트레이션한다. 코어는 호출되면 실행할 뿐이다.

---

## 헤더 구조

```
core/include/
├── hdb.hpp                        # umbrella include
└── hdb/
    ├── types.hpp                  # Id 계열 + 인지 연산 결과 타입
    ├── store/                     # 스키마 + 순수 가상 인터페이스
    │   ├── ineuron_table.hpp
    │   ├── isynapse_table.hpp
    │   └── iabstract_table.hpp
    ├── atom/                      # 스토리지 직접 조작 최소 단위
    │   ├── neuron.hpp
    │   └── synapse.hpp
    └── molecule/                  # 고차 인지 연산
        ├── thalamus.hpp
        ├── hippocampus.hpp
        └── cortex.hpp
```

### 의존성 방향

```
types.hpp → store/ → atom/ → molecule/
```

`types.hpp`는 store/에 의존하지 않는다 — Id 계열 alias만 정의하므로.
`Engram`은 `hippocampus.hpp`에 정의된다 — store/의 Record 타입에 의존하기 때문에 types.hpp에 두면 순환이 생긴다.
`Cortex`는 `Hippocampus`를 생성자에서 받지 않는다 — Engram을 직접 받아 순수한 시뮬레이터로 동작한다.

---

## 타입 시스템

### Id

```cpp
#ifndef HDB_ID_TYPE
    using Id = std::string;
#else
    using Id = HDB_ID_TYPE;
#endif

using Nid = Id;  // Neuron ID
using Sid = Id;  // Synapse ID
using Aid = Id;  // Abstract ID
```

기본값은 `std::string`이다. 성능이 중요한 환경에서는 CMake에서 `HDB_ID_TYPE=uint64_t`로 교체할 수 있다. `Nid/Sid/Aid`는 컴파일러 입장에서 같은 타입이지만, 코드를 읽는 사람에게 어떤 테이블의 ID인지 즉시 알려준다.

철학적으로 각 레코드의 진짜 primary key는 `timestamp`다. `nid/sid/aid`는 편의상 존재한다 — 사람이 ID 기반 조회에 익숙하기 때문에. 하지만 HDB에서 동일한 사건은 없다. 모든 사고는 발생한 시점에 의해 고유하게 정의된다.

### 모든 가변 데이터 — `std::vector<std::byte>`

`payload`, `actor`, `meta`는 전부 raw 바이트스트림이다. 코어는 내용을 해석하지 않는다. 임베딩 벡터든, 텍스트든, JSON이든 직렬화/역직렬화는 외부 레이어의 책임이다.

---

## 스키마 설계 의도

### NeuronRecord

| 필드                           | 의도                                                                                                                                                             |
| ------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `nid: Nid`                     | 뉴런의 고유 식별자.                                                                                                                                              |
| `actor: vector<byte>`          | 이 뉴런을 발화한 주체. 누가 이 생각을 했는가. 출처가 없으면 사고는 부유한다 — actor가 새겨질 때 비로소 사고는 누군가의 것이 된다.                                |
| `payload: vector<byte>`        | 사고의 실제 내용. 코어는 형식을 강제하지 않는다. 같은 payload라도 timestamp가 다르면 별개의 뉴런이다 — 같은 생각이 다른 시점에 다시 떠오른 것은 새로운 사건이다. |
| `timestamp: time_point`        | 사고가 발생한 시각. epoch μs 정밀도. 철학적으로 이것이 진짜 primary key다 — nid는 편의상 존재한다.                                                               |
| `meta: optional<vector<byte>>` | 부가 정보. 없어도 되며, 있으면 외부 레이어가 해석한다.                                                                                                           |

### SynapseRecord

| 필드                           | 의도                                                                                                                                                                                                                                                                                                                      |
| ------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `sid: Sid`                     | 시냅스의 고유 식별자.                                                                                                                                                                                                                                                                                                     |
| `actor: vector<byte>`          | 이 연결을 확정한 주체. 누가 이 시냅스를 발화했는가. 사고의 연결도 누가 만든 것인지가 코어에 남는다.                                                                                                                                                                                                                       |
| `from_nid: Nid`                | 발화를 시작한 뉴런.                                                                                                                                                                                                                                                                                                       |
| `to_nid: Nid`                  | 발화를 받은 뉴런.                                                                                                                                                                                                                                                                                                         |
| `timestamp: time_point`        | 연결이 확정된 시각. 같은 두 뉴런 사이라도 시점이 다르면 별도의 시냅스다. 예를 들어 `Awaken`으로 오랜 기억을 다시 꺼냈을 때 누군가 그 연결을 다시 발화했다면, 그것은 새로운 시냅스다 — 같은 관계의 재확인이 아니라, 그 시점에 다시 일어난 독립적인 사건이다. 철학적으로 이것이 진짜 primary key다 — sid는 편의상 존재한다. |
| `meta: optional<vector<byte>>` | 부가 정보.                                                                                                                                                                                                                                                                                                                |

시냅스 타입(EdgeType)은 의도적으로 제거했다. 이 연결은 "인간이 확정한 사고의 연결"이라는 의미 하나다. 타입을 나누는 순간 코어가 의미론을 강제하기 시작한다.

### AbstractRecord

| 필드                           | 의도                                                                                                                                                                                                                                                                                                                |
| ------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `aid: Aid`                     | 추상화 레코드의 고유 식별자.                                                                                                                                                                                                                                                                                        |
| `actor: vector<byte>`          | 이 추상화를 생성한 주체. 누가 이 임베딩을 만들었는가. 같은 뉴런에서 비롯된 추상화라도 만든 주체가 다르면 다른 관점이다.                                                                                                                                                                                             |
| `nid: Nid`                     | 이 추상화가 어떤 뉴런에서 온 것인지.                                                                                                                                                                                                                                                                                |
| `payload: vector<byte>`        | 의미 벡터(임베딩) raw bytes. sqlite-vec이 이 필드에서 ANN 검색을 수행한다.                                                                                                                                                                                                                                          |
| `timestamp: time_point`        | 이 추상화가 탄생한 시각. 잠을 자고 꿈을 꾸고 나서의 나, 술에 취해 가장 순수한 나에 닿아 있을 때의 나 — 그때마다 timestamp가 다르다면 그것은 새로운 나다. 인간의 뇌는 과거의 자신을 온전히 기억하지 못한다. HDB는 그 각각의 나를 덮어쓰지 않고 쌓는다. 철학적으로 이것이 진짜 primary key다 — aid는 편의상 존재한다. |
| `meta: optional<vector<byte>>` | 임베딩 모델 정보 등 부가 정보. 코어는 읽지 않는다.                                                                                                                                                                                                                                                                  |

임베딩 생성은 코어의 책임이 아니다. Python 레이어가 생성한 벡터를 `Thalamus::Consolidate()`로 전달하면 코어는 저장만 한다. Consolidate는 기억을 공고화하는 것이 아니라, 지금 이 순간의 나를 영구히 새기는 행위다.

---

## 인지 연산 설계 의도

### Neuron — atom

```
Sprout(actor, payload, meta?) → optional<Nid>
Awaken(nid) → optional<NeuronRecord>
```

**Sprout**(싹트다): 새 사고를 DB에 심는다. 성공하면 Nid를 돌려준다 — 호출자가 이후 시냅스 연결에 쓴다.  
**Awaken**(깨어나다): 잠들어 있던 뉴런을 깨워 꺼낸다.

생성자는 `INeuronTable&` 하나만 받는다. Neuron은 뉴런 테이블만 알면 충분하다.

### Synapse — atom

```
Fire(from_nid, to_nid, meta?) → optional<Sid>
```

**Fire**(발화하다): 두 뉴런 사이의 연결을 기록한다. `optional<Sid>`를 반환 — `Sprout`가 `optional<Nid>`를 반환하는 것과 일관된 패턴. 전체 레코드가 필요하면 QuerySynapse로 직접 조회한다.

### Thalamus — molecule

```
Consolidate(nid, payload, meta?) → optional<Aid>
```

뇌과학: 수면 중 해마의 sharp-wave ripple이 시상을 거쳐 신피질에 기억을 공고화한다.  
HDB: Python 레이어가 임베딩을 생성하면 Thalamus가 AbstractTable에 기록한다. **쓰기 전담**. 읽기는 하지 않는다.

**Consolidate**(공고화하다): 뉴런의 의미 벡터를 Abstract 레코드로 굳힌다. 성공하면 Aid를 반환한다.

### Hippocampus — molecule

```
Resonate(stimulus, limit?) → vector<Resonance>
Reminisce(since, until) → optional<Engram>
```

AbstractTable을 **읽기 전담**으로 조회한다. 쓰기는 Thalamus가 전담한다.

**Resonate**(공명하다): stimulus(임베딩)와 가장 유사한 뉴런들을 찾는다. sqlite-vec ANN 검색. `Resonance { nid, fidelity }` — `fidelity`는 양자 정보 이론의 fidelity에서 차용, 두 상태의 겹침 정도를 나타낸다.

**Reminisce**(회상하다): `since`부터 `until` 사이에 존재하는 시냅스와 뉴런 전체를 `Engram`으로 꺼낸다. `since`/`until`의 기본값은 `time_point::min()`/`time_point::max()` — 무한한 과거에서 무한한 미래까지.

`Engram`은 `hippocampus.hpp`에 정의된다. store/의 Record 타입에 의존하기 때문이다.

### Cortex — molecule

```
using Impulse = std::function<float(const SynapseRecord&)>
Imagine(engram, params, impulse) → Imagination
```

**Impulse**: 각 시냅스에 대해 가중치를 계산하는 함수. decay, 균등, 커스텀 — 전략을 외부에서 주입한다. 코어는 어떤 decay 함수도 내장하지 않는다.

**Imagine**(상상하다): Engram 위에서 확률적 그래프 워크를 수행한다.

1. Engram → `AdjacencyMap`
2. `start_nid`에서 출발
3. 매 스텝: outgoing edges에 Impulse를 적용 → softmax(creativity) → 확률적 선택 → `Thought`
4. `epochs`번 반복 → `Imagination` 반환

`Thought { nid, flux }` — `flux`는 그 스텝에서 softmax를 거쳐 선택된 확률값. 확률 선속(probability flux)에서 차용.  
`start_nid`가 Engram에 없으면 빈 Imagination을 반환한다.

Cortex는 생성자 의존성이 없다. Engram을 직접 받아 동작하는 순수한 시뮬레이터다.

---

## 에러 처리

예외 없음. 실패는 타입으로 표현한다.

| 반환 형태     | 의미                          |
| ------------- | ----------------------------- |
| `optional<T>` | 단일 값, 실패 시 `nullopt`    |
| `vector<T>`   | 복수 값, 실패 시 빈 벡터      |
| `bool`        | insert/write, 실패 시 `false` |

---

## 기술 스택

| 역할              | 기술                          | 이유                                               |
| ----------------- | ----------------------------- | -------------------------------------------------- |
| 사실 + 관계 저장  | SQLite                        | 단일 파일, 이식성, 검증된 내구성                   |
| 의미 색인 + ANN   | sqlite-vec                    | 동일 SQLite 파일 안에서 벡터 검색 — 별도 DB 불필요 |
| 그래프 시뮬레이션 | C++ in-memory                 | Cortex가 Engram을 받아 자체 구현 — KùzuDB 불필요   |
| 그래프 순회       | SQLite CTE (`WITH RECURSIVE`) | 별도 그래프 DB 없이 관계형으로 처리                |

---

## 어휘 체계

모든 메서드 이름은 뇌과학 또는 인지과학 어휘에서 직접 가져왔다.

| 메서드                   | 한국어     | 근거                                     |
| ------------------------ | ---------- | ---------------------------------------- |
| `Neuron::Sprout`         | 싹트다     | 새 사고가 처음 발생하는 순간             |
| `Neuron::Awaken`         | 깨어나다   | 잠들어 있던 기억을 활성화                |
| `Synapse::Fire`          | 발화하다   | 신경과학의 action potential firing       |
| `Thalamus::Consolidate`  | 공고화하다 | Memory consolidation — 수면 중 기억 강화 |
| `Hippocampus::Resonate`  | 공명하다   | 자극과 기억이 진동수를 맞춤              |
| `Hippocampus::Reminisce` | 회상하다   | 과거의 기억 덩어리를 꺼냄                |
| `Cortex::Imagine`        | 상상하다   | 기억을 재조합해 새로운 사고 시뮬레이션   |

### 양자역학 어휘

| 필드                 | 차용 개념                  | 의미                                       |
| -------------------- | -------------------------- | ------------------------------------------ |
| `Resonance.fidelity` | Quantum fidelity `⟨ψ\|φ⟩²` | 두 상태(자극 vs 기억)의 겹침 정도          |
| `Thought.flux`       | Probability flux           | 확률 선속 — 그 스텝에서 선택된 확률의 흐름 |
