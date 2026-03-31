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
		
		if (min > max)
		{
			character->SetRestTicket(curTicket + 1);
			return { UpgradeStatus::Error, 0 };

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
		character->SetRestTicket(curTicket + 1);
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

		renderer.RenderRestMenu();
		
		
		InputResult input = Tools<int>::Input(0, (int)RestOption::Heal + 1);
		
		if (input.status == InputStatus::Exit) break;
		if (input.status == InputStatus::Fail) continue;

		// [2] 체력 회복 선택의 경우
		if (character->GetRestTicket() <= 0)
		{
			renderer.RenderTicketInsufficient();
			continue;
		}

		if (input.value == (int)RestOption::Heal)
		{
			RestResult info = Rest(character);

			int hp = character->GetHP();
			renderer.RenderHealResult(info.healValue,  hp - info.healValue, hp, MAX_HP);
			
			continue;
		}
		if (input.value == (int)RestOption::Upgrade)
		{

			if (character->GetRestTicket() <= 0)
			{
				renderer.RenderTicketInsufficient();
				continue;
			}
			std::vector<DiceSlot> storege = character->GetInventory()->GetDiceStorege();
			renderer.RenderDiceUpgradeList(storege);
			InputResult input = Tools<int>::Input(0, (int)RestOption::Heal + 1);

			if (input.status == InputStatus::Exit) continue;
			if (input.status == InputStatus::Fail) continue;
			
			// 강화 종류 선택, min or max
			renderer.RenderDiceUpgradeOption();

			InputResult option = Tools<int>::Input(0, (int)RestOption::Heal + 1);

			if (option.status == InputStatus::Exit) continue;
			if (option.status == InputStatus::Fail) continue;

			UpgradeResult info = UpgradeDice(option.value - 1, (UpgradeType)option.value, character);
			renderer.RenderUpgradeResult(info.status, info.upgradeLevel, info.upgradeLevel + 1);


		}


	}
}