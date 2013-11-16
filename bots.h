#ifndef _BOTS_H_INCLUDED_
#define _BOTS_H_INCLUDED_

#include <map>
#include <iostream>
#include <random>
#include <memory>
#include <algorithm>
#include <list>
#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <boost/serialization/list.hpp> 
#include "bot.h"


/**
 * bots
 * ====
 *
 * container for the bots playground. all the game logic is here.
 *
 */
class bots {

    public:

    typedef std::list < bot > field_bots;


    void perform_action(bot & the_bot);

    bot::field_size _width;
    bot::field_size _height;
    field_bots _bots;

    /**
     * beware, there is no emptiness checking!
     */
    inline void create_bot(bot::position position, bot::team_id team) {
        bot new_bot(team, position);
        // move is not really needed here
        _bots.push_back(std::move(new_bot));
    } 
    
    field_bots::iterator iterator_at(const bot::position & pos);

  public:
  class too_many_bots:std::exception {
    };

    bots(bot::field_size width, bot::field_size height);

    bots() = default;

    bots(const bots &b) = default;

    virtual ~ bots();

    void generate(size_t number_teams, size_t bots_per_team) throw(too_many_bots);

    bot::team_id generate_team(size_t number_of_bots) throw(too_many_bots);

    bot *find_at(const bot::position & pos);

    const bot *find_at(const bot::position & pos) const;

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
    bot *attacks(const bot & the_bot, const direction & dir);
    
    const bot *attacks(const bot & the_bot, const direction & dir) const;

    /**
     * a loop.
     */
    void step();

    /**
     * iterates over the full vector of bots.
     *
     * this is the <code>const</code> version. non-const version also available.
     */
    inline void for_each_bot(std::function < void (const bot & the_bot) > fun) const {
        for_each(_bots.begin(), _bots.end(), fun);
    }

    inline void for_each_bot(std::function < void (bot & the_bot) > fun) {
        for_each(_bots.begin(), _bots.end(), fun);
    }

    std::map<bot::team_id, size_t> bot_count() const;

    bool game_over() const;

    private:
    
    friend class boost::serialization::access; 

    template <typename Archive> 
        void serialize(Archive &ar, const unsigned int version) 
        { 
            ar & _width;
            ar & _height;
            ar & _bots;
        } 

};

#endif
