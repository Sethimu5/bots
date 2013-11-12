#ifndef _BOTS_H_INCLUDED_
#define _BOTS_H_INCLUDED_

#include <map>
#include <iostream>
#include <random>
#include <memory>
#include <algorithm>
#include <list>

#include "bot.h"


/**
 * bots
 * ====
 *
 * container for the bots playground. all the game logic is here.
 *
 */
class bots {

  private:

    typedef std::list < std::shared_ptr<bot> > field_bots;



    void perform_action(std::shared_ptr<bot> the_bot);
    //void perform_action(bot & the_bot);

    bot::field_size _width;
    bot::field_size _height;
    field_bots _bots;

    /**
     * beware, there is no emptiness checking!
     */
    inline void create_bot(bot::position position, bot::team_id team) {
        _bots.push_back(std::make_shared<bot>(team, position));
    } 
    
    field_bots::iterator iterator_at(const bot::position & pos);

  public:
  class too_many_bots:std::exception {
    };

    bots(bot::field_size width, bot::field_size height);

    virtual ~bots();

    /**
     * generates bots. this is actually the only way to populate the playground.
     */
    void generate(size_t number_teams, size_t bots_per_team) throw(too_many_bots);

    std::shared_ptr<bot> find_at(const bot::position & pos);

    const std::shared_ptr<bot> find_at(const bot::position & pos) const;
    //bot *find_at(const bot::position & pos);

    //const bot *find_at(const bot::position & pos) const;

    /**
     * @warning returns a reference and assumes that the bot <b>exists</b>.
     */
    bot & operator[](const bot::position & pos);

    /**
     * <code>const</code> version of the operator[].
     */
    const bot & operator[](const bot::position & pos) const;

    bool empty(const bot::position & p) const;

    bool can_move(const bot & the_bot, const direction & dir) const;

    /**
     * @param the_bot the bot that could be attacking
     * @param dir the dir the bot is currently moving towards
     * @return a pointer to the bot `the_bot` would attack or nullptr otherwise
     */
    //bot *attacks(const bot & the_bot, const direction & dir);
    std::shared_ptr<bot> attacks(const std::shared_ptr<bot> the_bot, const direction & dir);
    
    const std::shared_ptr<bot> attacks(const std::shared_ptr<bot> the_bot, const direction & dir) const;
    //const bot *attacks(const bot & the_bot, const direction & dir) const;

    /**
     * a loop.
     */
    void step();

    /**
     * iterates over the full vector of bots.
     *
     * this is the <code>const</code> version. non-const version also available.
     */
    inline void for_each_bot(std::function < void (const std::shared_ptr<bot> the_bot) > fun) const {
        for_each(_bots.begin(), _bots.end(), fun);
    }

    inline void for_each_bot(std::function < void (std::shared_ptr<bot> the_bot) > fun) {
        for_each(_bots.begin(), _bots.end(), fun);
    }

    std::map<bot::team_id, size_t> bot_count() const;

    bool game_over() const;
    
};

#endif
