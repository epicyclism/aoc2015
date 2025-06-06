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
	int key_;
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
	{  53, 4, 0, 0, 0, 0,   0, -1},
	{  73, 2, 2, 0, 0, 0,   0, -1},
	{ 113, 0, 0, 6, 7, 0,   0,  0},
	{ 173, 0, 0, 6, 0, 3,   0,  1},
	{ 229, 0, 0, 5, 0, 0, 101,  2}
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
	if (sp.cost_ > gs.plr_mana_)
		return false;
	if (sp.ef_.key_ == -1)
		return true;
	return gs.ef_[sp.ef_.key_].dur_ == 0;
}

void apply_effects(game_state& gs)
{
	gs.plr_armour_ = 0;
	for (auto& e : gs.ef_)
	{
		if (e.dur_ > 0)
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
	gs.boss_hp_ -= sp.damage_;
	gs.plr_hp_ += sp.heal_;

	// install effect
	if (sp.ef_.dur_)
		gs.ef_[sp.ef_.key_] = sp.ef_;
}

bool over(game_state const& gs, int& mana)
{
//	if (gs.plr_mana_ <= 0)
//		return true;
	if (gs.boss_hp_ <= 0)
	{
		if (gs.plr_spnd_ < mana)
			mana = gs.plr_spnd_;
		return true;
	}
	if (gs.plr_hp_ <= 0)
		return true;
	return false;
}

void play_spell(spell const& sp, game_state gs, int penalty, int& mana)
{
//	fmt::println("Player has {} hit points, {} armour, {} mana.", gs.plr_hp_, gs.plr_armour_, gs.plr_mana_);
//	fmt::println("Boss has   {} hit points.", gs.boss_hp_);

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

void test2()
{
	game_state gs{ 10, 250, 0, 0, 14, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int mana = 10000;
	play_spell(spells[4], gs, 0, mana);
	play_spell(spells[2], gs, 0, mana);
	play_spell(spells[1], gs, 0, mana);
	play_spell(spells[3], gs, 0, mana);
	play_spell(spells[0], gs, 0, mana);
}

int pt1(auto hpd)
{
	timer t("p1");
	game_state gs{ 50, 500, 0, 0, hpd.first, hpd.second, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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
//	test2();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
