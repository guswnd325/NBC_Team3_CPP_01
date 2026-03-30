#include "RefurbishManager.h"
#include "GameManager.h"

RefurbishManager::RefurbishManager()
{

}

RefurbishManager::~RefurbishManager()
{

}

UpgradeResult RefurbishManager::UpgradeDice(int index, UpgradeType type, Character* character)
{
	// 해당 인덱스의 다이스 정보를 불러옴
	std::vector<DiceSlot>& storege = character->GetInventory()->GetDiceStorege();
	DiceSlot info = storege[index]; 
	Dice* curDice = info.dice;

	int upgradeLevel = curDice->GetUpgradeCount();

	int curTicket = character->GetRestTicket();

	if (curTicket >= 1)
	{
		character->SetRestTicket(curTicket - 1);
		// 업그레이드
		

		// 강화된 주사위 min, max 값을 통해 Add Dice
		int min = 0, max = 0;
		
		
		if (type == UpgradeType::Min) 
		{
			min = curDice->GetMin() + upgradeUnit;
			max = curDice->GetMax();
		}
		else // type == UpgradeType::Max
		{
			min = curDice->GetMin();
			max = curDice->GetMax() + upgradeUnit;
		}
		
		DiceID newDiceID = diceManager.MakeDiceId(min, max);

		character->GetInventory()->AddDice(newDiceID);

		// 업그레이드 레벨 설정
		storege[storege.size()-1].dice->SetUpgradeCount(upgradeLevel + 1);

		// 강화 대상 주사위 count-- 
		storege[index].count--;

		if (storege[index].count == 0)
		{
			storege.erase(storege.begin() + index);
		}

		return { UpgradeStatus::Success, upgradeLevel + 1};
	}
	else
	{
		return { UpgradeStatus::TicketInsufficient, 0 };
	}
}

RestResult RefurbishManager::Rest(Character *character)
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

void RefurbishManager::Run()
{
	while (true)
	{
		Renderer& renderer = Renderer::GetInstance();
		renderer.Clear();
		Character* character = GameManager::GetInstance().GetCharacter();

		std::string message = "";

		// renderer.RenderRestSelect() ?
		std::cout << "[0] 메뉴로 돌아가기" << std::endl;
		std::cout << "[1] 강화(주사위를 강화합니다)" << std::endl;
		std::cout << "[2] 체력 회복(주사위로 체력을 회복합니다)" << std::endl << std::endl;
		std::cout << "옵션을 선택 : ";
		int select;

		std::cin >> select;

		// 숫자 이외의 입력 및 0은 종료처리
		if (!select)
		{
			// 메뉴로 돌아감
			break;
		}
		else if (std::cin.fail())
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
				message = "체력이 " + to_string(info.healValue) + "만큼 회복 되었습니다. (" + to_string(character->GetHP() - info.healValue) + " -> " + to_string(character->GetHP()) + ")";
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

			{
				renderer.Clear();

				std::cout << message << std::endl;
				Sleep(2000);
			}
			continue;
		}
		if (select == (int)RestOption::Upgrade)
		{

			if (character->GetRestTicket() <= 0)
			{
				renderer.RenderTicketInsufficient();
				continue;
			}
			std::vector<DiceSlot> storege = character->GetInventory()->GetDiceStorege();
			renderer.RenderDiceUpgradeList(storege);

			int index;
			std::cin >> index;

			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
				continue;
			}
			else if (!index)
			{
				break;
			}
			if (index > storege.size())
			{
				continue;
			}

			// 강화 종류 선택, min or max
			renderer.RenderDiceUpgradeOption();

			std::cin >> select;

			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
				continue;
			}
			else if (!select)
			{
				break;
			}

			UpgradeResult info = UpgradeDice(index - 1, (UpgradeType)select, character);
			renderer.RenderUpgradeResult(info.status, info.upgradeLevel, info.upgradeLevel + 1);


		}


	}
}