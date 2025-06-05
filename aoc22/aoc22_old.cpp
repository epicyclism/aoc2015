
#include <iostream>

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

void report(game_state const& gs)
{
	std::cout << "Player has " << gs.plr_hp_ << " hit points, " << gs.plr_armour_ << " armour, " << gs.plr_mana_ << "mana.\n";
	std::cout << "Boss has " << gs.boss_hp_ << " hit points.\n";
}

int mana_ = 100000;

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
#if 0
				// apply effect
				--e.dur_;
				if (e.armour_)
					gs.plr_armour_ = e.armour_;
				gs.boss_hp_ -= e.edamage_;
				gs.plr_mana_ += e.bonus_;
#endif
				return;
			}
		}
	}
}

bool over(game_state const& gs)
{
	if (gs.plr_mana_ < 0)
		return true;
	if (gs.boss_hp_ <= 0)
	{
//		std::cout << "plr has " << gs.plr_hp_ << " hit points, " << gs.plr_mana_ << "\n";		
//		if (gs.plr_mana_ > mana_max)
//			mana_max = gs.plr_mana_;
		if (gs.plr_spnd_ < mana_)
			mana_ = gs.plr_spnd_;
		return true;
	}
	if (gs.plr_hp_ <= 0)
		return true;
	return false;
}

void play_spell(spell const& sp, game_state gs, int penalty)
{
	gs.plr_hp_ -= penalty;
	if (over(gs))
		return;
	// assume spell is valid...
	apply_effects(gs);
	if (over(gs))
		return;
	// player turn
	apply_spell(sp, gs);
	if (over(gs))
		return;

	// boss turn
	apply_effects(gs);
	if (over(gs))
		return;
	int bp = gs.boss_damage_ - gs.plr_armour_;
	if (bp < 1)
		bp = 1;
	gs.plr_hp_ -= bp;
	if (over(gs))
		return;

	// next move
	for (auto const& s : spells)
	{
		if (valid_spell(s, gs))
			play_spell(s, gs, penalty);
	}
}

void play_spell_i(spell const& sp, game_state& gs)
{
	report(gs);
	// assume spell is valid...
	apply_effects(gs);
	if (over(gs))
		return;
	// player turn
	apply_spell(sp, gs);
	if (over(gs))
		return;
	report(gs);
	// boss turn
	apply_effects(gs);
	if (over(gs))
		return;
	int bp = gs.boss_damage_ - gs.plr_armour_;
	if (bp < 1)
		bp = 1;
	gs.plr_hp_ -= bp;
	if (over(gs))
		return;
}

void test()
{
	game_state gs{ 10, 250, 0, 0, 13, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	mana_ = 10000;
	for (auto const& s : spells)
		play_spell(s, gs, 0);
	std::cout << "Least spend = " << mana_ << "\n";
}

void test1()
{
	game_state gs{ 10, 250, 0, 0, 14, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	mana_ = 10000;
	for (auto const& s : spells)
		play_spell(s, gs, 0);
	std::cout << "Least spend = " << mana_ << "\n";
}

void test2()
{
	game_state gs{ 10, 250, 0, 0, 14, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	mana_ = 10000;
	play_spell_i(spells[4], gs);
	play_spell_i(spells[2], gs);
	play_spell_i(spells[1], gs);
	play_spell_i(spells[3], gs);
	play_spell_i(spells[0], gs);
}

void pt1()
{
	game_state gs{ 50, 500, 0, 0, 71, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	mana_ = 10000;
	for (auto const& s : spells)
		play_spell(s, gs, 0);
	std::cout << "Least spend = " << mana_ << "\n";
}

void pt2()
{
	game_state gs{ 50, 500, 0, 0, 71, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	mana_ = 10000;
	for (auto const& s : spells)
		play_spell(s, gs, 1);
	std::cout << "Least spend = " << mana_ << "\n";
}

int main()
{
	test();
	test1();
	pt1();
	pt2();
}

