#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

struct effect
{
	int dur_;
	int armour_;
	int edamage_;
	int bonus_;
};

struct spell
{
	int cost_;
	int damage_;
	int heal_;
	effect ef_;
};

constexpr spell spells[]
{
	{  53, 4, 0, 0, 0, 0,   0 },
	{  73, 2, 2, 0, 0, 0,   0},
	{ 113, 0, 0, 6, 7, 0,   0},
	{ 173, 0, 0, 6, 0, 3,   0},
	{ 229, 0, 0, 5, 0, 0, 101}
};

struct game_state
{
	int plr_hp_;
	int plr_mana_;
	int plr_spnd_;
	int plr_armour_;
	int boss_hp_;
	int boss_damage_;
	effect ef_[3];
};

bool valid_spell(spell const& sp, game_state const& gs)
{
	if (sp.ef_.dur_ == 0)
		return true;
	for (auto& e : gs.ef_)
		if ((e.dur_ > 1 ) && ((e.armour_ && (e.armour_ == sp.ef_.armour_))
			|| ( e.bonus_ && ( e.bonus_ == sp.ef_.bonus_))
			|| ( e.edamage_ && (e.edamage_ == sp.ef_.edamage_))))
			return false;
	return true;
}

void apply_effects(game_state& gs)
{
	gs.plr_armour_ = 0;
	for (auto& e : gs.ef_)
	{
		if (e.dur_)
		{
			--e.dur_;
			if (e.armour_)
				gs.plr_armour_ = e.armour_;
			gs.boss_hp_ -= e.edamage_;
			gs.plr_mana_ += e.bonus_;
		}
	}
}

void apply_spell(spell const& sp, game_state& gs)
{
	gs.plr_mana_ -= sp.cost_;
	gs.plr_spnd_ += sp.cost_;
	if (gs.plr_mana_ <= 0)
		return;
	gs.boss_hp_ -= sp.damage_;
	gs.plr_hp_ += sp.heal_;

	// install effect
	if (sp.ef_.dur_)
	{
		for (auto& e : gs.ef_)
		{
			if (e.dur_ == 0)
			{
				e = sp.ef_;
				return;
			}
		}
	}
}

bool over(game_state const& gs, int& mana)
{
	if (gs.plr_mana_ < 0)
		return true;
	if (gs.boss_hp_ <= 0)
	{
		if (gs.plr_spnd_ < mana)
		{
			mana = gs.plr_spnd_;

		}
		fmt::println("pwon {}", mana);
		return true;
	}
	if (gs.plr_hp_ <= 0)
		return true;
	return false;
}

void play_spell(spell const& sp, game_state gs, int penalty, int& mana)
{
	gs.plr_hp_ -= penalty;
	if (over(gs, mana))
		return;
	// assume spell is valid...
	apply_effects(gs);
	if (over(gs, mana))
		return;
	// player turn
	apply_spell(sp, gs);
	if (over(gs, mana))
		return;

	// boss turn
	apply_effects(gs);
	if (over(gs, mana))
		return;
	int bp = gs.boss_damage_ - gs.plr_armour_;
	if (bp < 1)
		bp = 1;
	gs.plr_hp_ -= bp;
	if (over(gs, mana))
		return;

	// next move
	for (auto const& s : spells)
	{
		if (valid_spell(s, gs))
			play_spell(s, gs, penalty, mana);
	}
}

auto get_input()
{
	int param[2];
	std::string ln;
	int n = 0;
	while(std::getline(std::cin, ln))
	{
		auto[m, p] = ctre::match<"[a-zA-Z ]+: (\\d+)">(ln);
		if(m)
			param[n] = p.to_number<int>();
		++n;
	}
	return std::make_pair(param[0], param[1]);
}

int pt1(auto hpd)
{
	timer t("p1");
	game_state gs{ 50, 500, 0, 0, hpd.first, hpd.second, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int mana = std::numeric_limits<int>::max();
	for (auto const& s : spells)
		play_spell(s, gs, 0, mana);

	return mana;
}

int pt2(auto b)
{
	timer t("p2");
	return 0;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
