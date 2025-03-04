/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

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
