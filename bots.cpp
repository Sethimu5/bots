#include "bots.h"
#include <algorithm>

bots::bots(bot::field_size width, bot::field_size height):_width(width),
    _height(height)
{
}

void bots::generate(size_t number_teams, size_t bots_per_team) throw(too_many_bots)
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::mt19937 gen2(rd());
    std::uniform_int_distribution < bot::field_size > random_width(0, _width - 1);
    std::uniform_int_distribution < bot::field_size > random_height(0, _height - 1);

    if (number_teams * bots_per_team > _width * _height) {
        throw new too_many_bots;
    }

    // reasonably efficient for sparse distributions
    // a las vegas algorithm!!
    for (size_t i = 0; i < number_teams; ++i) {

        for (size_t j = 0; j < bots_per_team; j++) {

            std::pair < int, int >position;
            do {
                position.first = random_width(gen);
                position.second = random_height(gen2);
            } while (!empty(position));

            create_bot(position, i);
        }

    }
}

bots::~bots()
{
}


bot *bots::find_at(const bot::position & pos)
{
    auto it = std::find_if(_bots.begin(), _bots.end(), 
            [&pos] (const bot & the_bot) { 
                return the_bot.get_position() == pos; 
            });

    if (it != _bots.end()) {
        return &(*it);
    } else {
        return nullptr;
    }
}

const bot *bots::find_at(const bot::position & pos) const {
    // yeah, "never use const_cast"...
    return const_cast<const bot*>(const_cast<bots*>(this)->find_at(pos));
}

bool bots::empty(const bot::position & pos) const
{
    return find_at(pos) == nullptr;
}

// TODO check whether the other bot is moving (you can move if the other guy is moving
bool bots::can_move(const bot & the_bot, const direction & dir) const
{
    const bot::position & p = bot::new_position(the_bot.get_position(), dir);
    return p.first >= 0 && p.first < _width &&
        p.second >= 0 && p.second < _height && 
        empty(p);
}

void bots::perform_action(bot & the_bot)
{

    bot::position & pos = the_bot._position;
    const direction & dir = the_bot.get_next_direction();

    if (bot * victim = attacks(the_bot, dir)) {
        victim->_energy = std::max(0, 
                victim->_energy - std::max(0, 
                    the_bot.get_attack () - victim->get_defense()));
    } 
    else if (can_move(the_bot, dir)) {
        pos = bot::new_position(pos, dir);
    }
}


bot *bots::attacks(const bot & the_bot, const direction & dir)
{
    bot * b = find_at(bot::new_position(the_bot.get_position(), dir));

    if(nullptr != b && b->get_team() != the_bot.get_team()) {
        return b;
    }
    else {
        return nullptr;
    }
}

// FIXME test implementation, doesn't have any sense at all
void bots::step(int time)
{
    // acting and dying in the same loop changes the semantics
    // currently, a bot can be dead but kill and move during the current
    // turn
    for_each_bot([this] (bot & the_bot) { perform_action(the_bot); });

    // remove dead bots
    for (auto it = _bots.begin(); it != _bots.end(); it++) {
        if ((*it).get_energy() <= 0) {
            _bots.erase(it);
        }
    }
}

std::map<bot::team_id, size_t> bots::bot_count() const {
    std::map <bot::team_id, size_t> result;
    for_each_bot([&result] (const bot & the_bot)  { ++result[the_bot.get_team()]; });

    // move not needed
    return std::move(result);
}

bool bots::game_over() const {
    return bot_count().size() < 2;
}

