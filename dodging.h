/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef dodging_h
#define dodging_h

#include <vector>
#include <cstdint>
#include <map>

class Dodging {
    private:
        std::vector<std::uint32_t> tracking;
        std::map<std::uint32_t, std::uint32_t> cooling;

    public:
        void clear_lists();

        std::vector<std::uint32_t> get_tracking() const;

        bool is_debris_tracked(std::uint32_t index) const;
        bool is_debris_cooling(std::uint32_t index) const;

        void begin_tracking_debris(std::uint32_t index);
        void begin_cooling_debris(std::uint32_t index);

        void stop_tracking_debris(std::uint32_t index);

        void cooldown();
};

#endif
