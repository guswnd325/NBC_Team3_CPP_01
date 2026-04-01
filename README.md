# 🎲 Dice Agent
**"모든 운명은 주사위의 눈에 달려 있다."** 주사위를 굴려 운명을 결정하고, 전략적으로 적을 제압하는 텍스트 기반 콘솔 TRPG입니다.

---

## <a id="intro"></a>🎯 게임 소개
- **장르** : 콘솔 TRPG (Text-based Role Playing Game)
- **플랫폼** : Windows (Console)
- **핵심 특징** :
  - **운과 전략의 조화**: 주사위 결과에 따라 공격의 성공 여부와 위력이 결정되는 긴장감 넘치는 전투 시스템.
  - **몰입감 있는 연출**: ASCII ART와 MCI API를 활용한 생생한 주사위 애니메이션 및 배경음악/효과음 삽입.
  - **성장 시스템**: 상점과 인벤토리 시스템을 통해 캐릭터를 강화하고 더 강력한 몬스터에 도전.

---

## <a id="dev-env"></a>⚙️ 개발 환경 및 개발 기간

### 🛠 개발 환경
- **Language** : C++ 14
- **API** : Windows API (MCI, Console Buffer Control)
- **IDE** : Visual Studio 2022

### ⏱ 개발 기간
- **2026.03.27 ~ 2026.04.01** (총 6일)

---

## <a id="features"></a>🚀 주요 기능
- **Dice Animation**: 주사위가 굴러가는 시각적 연출을 콘솔 화면에 구현.
- **Audio Management**: MCI(Media Control Interface)를 활용한 BGM 및 상황별 효과음 재생.
- **Battle System**: 주사위 값에 따른 크리티컬, 회피 로직이 포함된 턴제 전투.
- **Entity System**: OOP를 활용한 플레이어, 몬스터, 아이템의 체계적인 상태 관리.

---

## <a id="structure"></a>🏗 프로젝트 구조
- **Manager**: Game, Battle, Combat, Sound 매니저 클래스
- **Entity**: Character, Monster, Inventory, Shop 클래스
- **Util**: Dice Animation, UI Rendering, Constants
- **Main.cpp**: 게임 엔트리 포인트

---

## <a id="developer"></a>👨‍💻 개발자 (Team 3)

| 이름 | 역할 |
| :--- | :--- |
| **조현민** | 팀장, 게임 매니저 / 배틀 매니저 |
| **조현묵** | 기획, 몬스터 / 콘솔 UI 렌더링 및 ASCII Art 관리 |
| **김현중** | 팀원, 캐릭터 / 인벤토리 / 장비 / 상점 및 강화 시스템 / 장비 |
| **김준수** | 팀원, 주사위 로직 및 애니메이션 연출 / 컴뱃 매니저 |
| **김하승** | 팀원, 회복권을 통한 체력 회복 메커니즘 구현 |

---

## <a id="preview"></a>📸 게임 화면
<img width="914" height="540" alt="image" src="https://github.com/user-attachments/assets/30e03af6-7b78-4594-87ef-95c8ad729102" />
<img width="908" height="532" alt="image" src="https://github.com/user-attachments/assets/b3a42816-e7ef-4900-891d-6492ea89abb3" />
<img width="902" height="571" alt="image" src="https://github.com/user-attachments/assets/49d6b34f-cac5-4810-a711-21e5b491baeb" />
<img width="966" height="570" alt="image" src="https://github.com/user-attachments/assets/d0e2d75a-4864-40db-9c42-0ab98c84bb35" />
<img width="911" height="530" alt="image" src="https://github.com/user-attachments/assets/54b947d9-4ee9-4835-9bee-48e0592ed8a9" />
<img width="908" height="436" alt="image" src="https://github.com/user-attachments/assets/de99f558-a829-4130-aef6-52fac66d9d56" />
<img width="909" height="532" alt="image" src="https://github.com/user-attachments/assets/17092dd0-75d1-4735-bfac-7c3c5df41560" />
<img width="905" height="527" alt="image" src="https://github.com/user-attachments/assets/3d365a12-a022-4e10-8ae7-a99abd51198b" />
<img width="910" height="529" alt="image" src="https://github.com/user-attachments/assets/8464857d-5181-49e4-a32e-9dd1a7ce9b74" />
<img width="916" height="531" alt="image" src="https://github.com/user-attachments/assets/a5053e01-d9dc-45a0-9c25-5dd4cf15e75d" />
<img width="910" height="533" alt="image" src="https://github.com/user-attachments/assets/5b095211-d6e4-4106-bb3f-6a545e2a5114" />
<img width="910" height="536" alt="image" src="https://github.com/user-attachments/assets/f963fa39-5535-4ad1-a373-e4be0d68c49b" />



