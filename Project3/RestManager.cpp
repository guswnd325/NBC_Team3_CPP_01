#include "RestManager.h"
#include "GameManager.h"

RestManager::RestManager()
{

}

RestManager::~RestManager()
{

}

UpgradeResult RestManager::UpgradeDice(int index, UpgradeType type, Character* character)
{
	// 해당 인덱스의 다이스 정보를 불러옴
	std::vector<DiceSlot> storege = character->GetInventory()->GetDiceStorege();
	DiceSlot info = storege[index]; 
	Dice* curDice = info.dice;

	int upgradeLevel = curDice->GetUpgradeCount();

	int curTicket = character->GetRestTicket();
	if (upgradeLevel >= 3)
	{
		return { UpgradeStatus::MaxUpgrade, 0 };
	}

	if (curTicket >= 1)
	{
		character->SetRestTicket(curTicket - 1);
		// 업그레이드
		

		// 강화된 주사위 min, max 값을 통해 Add Dice
		int min = 0, max = 0;
		
		
		if (type == UpgradeType::Min) 
		{
			min = curDice->GetMin() + upgradeUnit;
		}
		else // type == UpgradeType::Max
		{
			max = curDice->GetMax() + upgradeUnit;
		}
		
		DiceID newDiceID = diceManager.MakeDiceId(min, max);

		character->GetInventory()->AddDice(newDiceID);

		// 업그레이드 레벨 설정
		storege[storege.size()].dice->SetUpgradeCount(upgradeLevel + 1);

		if (info.count == 1)
		{
			// 1개면 강화 후 해당 주사위는 없는 주사위고, 새로 태어남... ㅎㅎ
			storege.erase(storege.begin() + index);
		}
		else if (info.count >= 2)
		{
			info.count--;
		}

		else if (info.count <= 0)
		{
			// Noting
		}

		return { UpgradeStatus::Success, upgradeLevel + 1};
	}
	else
	{
		return { UpgradeStatus::TicketInsufficient, 0 };
	}
}

RestResult RestManager::Rest(Character *character)
{
	RestResult result;

	if (character->GetRestTicket() <= 0)
	{
		// 티켓 부족
		result.result = HealStatus::TicketInsufficient;
	}
	else if (character->GetHP() >= 100)
	{
		// 최대 체력임
		result.result = HealStatus::MaxHP;
	}
	else
	{
		// 티켓 소모
		character->SetRestTicket(character->GetRestTicket() - 1);

		int value = diceManager.Roll(character);
		int prevHp = character->GetHP();

		int newHP = Tools<int>::Clamp(character->GetHP() + value, 0, 100);
		character->SetHP(newHP);

		result.result = HealStatus::Success;
		result.healValue = abs(prevHp - newHP);
	}

	return result;
}

void RestManager::Run()
{
	while (true)
	{
		Renderer& renderer = Renderer::GetInstance();
		renderer.Clear();
		Character* character = GameManager::GetInstance().GetCharacter();
		
		std::string message = "";

		// renderer.RenderRestSelect() ?
		// [0] 메뉴로 돌아가기
		// [1] 강화(주사위를 강화합니다)
		// [2] 체력 회복(주사위로 체력을 회복합니다)

		int select;

		std::cin >> select;

		// 숫자 이외의 입력 및 0은 종료처리
		if (!select)
		{
			// 메뉴로 돌아감
			break;
		}
		else if(std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

			message = "잘못된 입력입니다.";
			// renderer.RenderMessage(message);
			// renderer.Delay(4);
			continue;
		}

		
		// [2] 체력 회복 선택의 경우
		if (select == (int)RestOption::Heal)
		{
			RestResult info = Rest(character);

			std::string message = "";

			switch (info.result)
			{
				case HealStatus::Success:
				{
					message = "체력이 " + to_string(info.healValue) + "만큼 회복 되었습니다.";
					break;
				}
				case HealStatus::TicketInsufficient:
				{
					message = "회복 티켓이 부족합니다.";
					break;
				}
				case HealStatus::MaxHP:
				{
					message = "이미 최대 체력입니다.";
					break;
				}
			}
			
			// renderer.RenderMessage(message);
			// renderer.Delay(5);

			continue;
		}
		if (select == (int)RestOption::Upgrade)
		{

			if (character->GetRestTicket() <= 0)
			{
				message = "강화 티켓이 부족합니다.";
				// renderer.RenderMessage(message);
				// renderer.Delay(5);
				break;
			}

			// 주사위 인덱스
			message = "강화할 주사위를 선택\n";
			// renderer.RenderDiceStorege();

			int index;
			std::cin >> index;

			// 숫자 이외의 입력 및 0은 종료처리
			
			if(std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

				message = "잘못된 입력입니다.";
				// Renderer::GetInstance().RenderMessage(message);
				// Renderer::Delay(4);

				continue;
			}
			else if (!index)
			{
				// 메뉴로 쫒아내기
				break;
			}

			// 인덱스 오버 체크
			//if (indexOver)

			// 강화 종류 선택
			message = "강화 옵션을 선택해주세요(1=최소값 증가, 2=최대값 증가)";
			// renderer.RenderMessage(message);

			std::cin >> select;

			// 숫자 이외의 입력 및 0은 종료처리
			 
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

				message = "잘못된 입력입니다.";
				// Renderer::GetInstance().RenderMessage(message);
				// Renderer::Delay(4);

				continue;
			}
			else if (!select)
			{
				// 메뉴로 나가!!
				break;
			}

			UpgradeResult info = UpgradeDice(index, (UpgradeType)select, character);

			switch (info.status)
			{
				case UpgradeStatus::MaxUpgrade:
				{
					message = "이미 최대 업그레이드 되었습니다.";
					break;
				}
				case UpgradeStatus::Success:
				{
					message = "강화 성공!" + to_string(info.upgradeLevel - 1) + " -> " + to_string(info.upgradeLevel);
					// or message = "강화 성공!" + to_string(info.upgradeLevel) + " -> " + to_string(info.upgradeLevel+1);
					break;
				}
			}
		
			
			// 처리 결과 출력
			// renderer.RenderMessage(message);
			// renderer.Delay(4);

		}


	}
}