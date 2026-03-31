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
	std::vector<DiceSlot>& storege = character->GetInventory()->GetDiceStorege();
	DiceSlot info = storege[index]; 
	Dice* curDice = info.dice;

	int upgradeLevel = curDice->GetUpgradeCount();

	int curTicket = character->GetRestTicket();

	if (curTicket >= 1)
	{
		character->SetRestTicket(curTicket - 1);
	
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

		storege[storege.size()-1].dice->SetUpgradeCount(upgradeLevel + 1);

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
		result.result = HealStatus::TicketInsufficient;
	}

	else if (character->GetHP() >= MAX_HP)
	{
		result.result = HealStatus::MaxHP;
	}
	else
	{
		character->SetRestTicket(character->GetRestTicket() - 1);

		int value = diceManager.Roll(character);
		diceManager.GetDiceFrame(value);
		int prevHp = character->GetHP();

		int newHP = Tools<int>::Clamp(character->GetHP() + value, 0, 100);
		character->SetHP(newHP);

		result.result = HealStatus::Success;
		result.healValue = abs(prevHp - newHP);
	}

	return result;
}

void RefurbishManager::DrawDiceDirectly(int num) {
	std::vector<std::string> frame = diceManager.GetDiceFrame(num);

	int startX = 63;
	int startY = Renderer::ZONE_SCREEN_Y + 4;

	for (int i = 0; i < (int)frame.size(); i++) {
		Renderer::GetInstance().MoveCursor(startX, startY + i);
		int vLen = Renderer::GetInstance().GetVisualLength(frame[i]);
		std::cout << frame[i] << std::string(std::max(0, 35 - vLen), ' ');
	}
	Renderer::GetInstance().MoveCursor(0, 35);
}

void RefurbishManager::Run()
{
	while (true)
	{
		Renderer& renderer = Renderer::GetInstance();
		renderer.Clear();
		Character* character = GameManager::GetInstance().GetCharacter();

		renderer.RenderRestMenu(std::vector<std::string>());
		
		
		InputResult input = Tools<int>::Input(0, (int)RestOption::Heal + 1);
		
		if (input.status == InputStatus::Exit) break;
		if (input.status == InputStatus::Fail) continue;

		if (character->GetRestTicket() <= 0)
		{
			renderer.RenderTicketInsufficient();
			continue;
		}

		if (input.value == (int)RestOption::Heal)
		{
			RestResult info = Rest(character);

			if (info.result == HealStatus::Success)
			{
				AudioManager::GetInstance().PlaySFX(SFXList::Dice_Roll);

				int playerRoll = diceManager.Roll(character);
				for (int i = 0; i < 10; i++) {
					DrawDiceDirectly(rand() % 20 + 1);
					Sleep(40 + (i * 10));
				}
				
				//DrawDiceDirectly(info.healValue);

				AudioManager::PlaySFX(SFXList::Heal);
			}
			else 
			{
				AudioManager::PlaySFX(SFXList::Error);
				info.healValue = 0;
			}
			
			int hp = character->GetHP();
			renderer.RenderHealResult(info.healValue,  hp - info.healValue, hp, MAX_HP, diceManager.GetDiceFrame(info.healValue));
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
			renderer.RenderDiceUpgradeList(storege, std::vector<std::string>());
			InputResult input = Tools<int>::Input(0, (int)RestOption::Heal + 1);

			if (input.status == InputStatus::Exit) continue;
			if (input.status == InputStatus::Fail) continue;
			
			renderer.RenderDiceUpgradeOption(std::vector<std::string>());

			InputResult option = Tools<int>::Input(0, (int)RestOption::Heal + 1);

			if (option.status == InputStatus::Exit) continue;
			if (option.status == InputStatus::Fail) continue;

			UpgradeResult info = UpgradeDice(input.value - 1, (UpgradeType)option.value, character);
			
			if (info.status == UpgradeStatus::Success) AudioManager::PlaySFX(SFXList::Upgrade_Dice);
			else AudioManager::PlaySFX(SFXList::Error);

			renderer.RenderUpgradeResult(info.status, info.upgradeLevel, info.upgradeLevel + 1);


		}


	}
}