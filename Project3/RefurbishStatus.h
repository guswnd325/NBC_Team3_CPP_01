#pragma once

enum class RestOption
{
    Upgrade = 1,
    Heal
};

enum class UpgradeType
{
    Max,
    Min,
};

enum class UpgradeStatus
{
    Success,
    TicketInsufficient,
    MaxUpgrade,
};

enum class HealStatus
{
    Success,
    TicketInsufficient,
    MaxHP,
};

struct UpgradeResult
{
    UpgradeStatus status;
    int upgradeLevel;
};
struct RestResult
{
    HealStatus result;
    int healValue;
};