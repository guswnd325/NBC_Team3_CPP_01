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
		return UpgradeResult::MaxUpgrade;
	}

	if (curTicket >= 1)
	{
		character->SetRestTicket(curTicket - 1);
		// 업그레이드
		

		// 강화된 주사위 min, max 값을 통해 Add Dice
		int min, max;
		
		
		if (type == UpgradeType::Min) 
		{
			min = curDice->GetMin() + upgradeUnit;
		}
		else // type == UpgradeType::Max
		{
			min = curDice->GetMin() + upgradeUnit;
		}
		
		//max = curDice->GetMax()
		//diceManager.MakeDiceID();

		//character->GetInventory()->AddDice();
		
		if (info.count == 1)
		{
			// 1개면 강화 후 해당 주사위는 없는 주사위고, 새로 태어남... ㅎㅎ
			storege.erase(storege.begin() + index);
		}
		else
		{
			info.count--;
		}

		return UpgradeResult::Success;
	}
	else
	{
		return UpgradeResult::TicketInsufficient;
	}
}

void RestManager::Rest(Character *character)
{
	/*if (character->GetRestTicket() <= 0)
	{
		cout << "No rest tickets left!" << endl;
		return;
	}
	else if (character->GetHP() >= 100)
	{
		cout << "HP is already full!" << endl;
		return;
	}
	else
	{
		character->SetRestTicket(character->GetRestTicket() - 1);
		rollResult = DiceManager::Roll();
		character->SetHp((Tools<int>::Clamp(character->GetHP() + rollResult, 0, 100)));
		cout << "Rest successful! HP increased by " << rollResult << "." << endl;
	}*/

}

void RestManager::Run()
{
	while (true)
	{
		//Renderer::GetInstance().RenderDiceStorge();

		int select;

		std::cin >> select;

		bool fail = std::cin.fail();

		// 숫자 이외의 입력 및 0은 종료처리
		if (!select || fail)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}


		// 강화 종류 선택
		std::string message = "강화 옵션을 선택해주세요(1=최소값 증가, 2=최대값 증가";
		// Renderer::GetInstance().RenderMessage(message);

		std::cin >> select;

		// 숫자 이외의 입력 및 0은 종료처리
		if (!select || fail)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}

		/*UpgradeDice(select, )
		switch (info.status)
		{
		case EquipStatus::Equip:
		{
			message = "[" + changedItem + "]" + "아이템을 장착하였습니다.";
		}
		break;
		case EquipStatus::Changed:
		{
			message = "[" + prevItem + "]" + "아이템을 " + "[" + changedItem + "]으로 교체하였습니다.";
		}
		break;
		}*/

		// 처리 결과 출력
		// Renderer::GetInstance().RenderMessage(message);
	}
}