#ifndef A_HASHMAP_H
#define A_HASHMAP_H
#include <algorithm>
#include <iostream>
#include <optional>
#include <stdint.h>
#include <vector>

/*
 * HashMap
 * =========
 * This is a implementation of a hash table, using open addressing collision resolution
 * Design choices
 * 1) No iterators - For simplicity and faster access, this implementation does not implement
 * iterators 2) Supports three main operations, along with three other operations
 *     * find(Key) - Returns an optional value
 *     * insert(Key, Value) - Inserts the Key-Value pair into the hash table
 *     * erase(Key) - Deletes the key if it exists
 *     * contains(Key) - Returns true if the key exists
 *     * size() - Returns the number of keys in the hash table
 *     * clear() - Remove all keys from the table
 * 3) To improve cache efficiency, the table uses open addressing collision resolution mechanism
 * 4) The current implementation requires Key and Value to be DefaultConstructible
 */
template <typename Key, typename Value, typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class HashMap
{
  private:
    KeyEqual key_equal_;
    Hash hasher_;
    size_t sz, number_of_slots_used, number_of_slots_total;
    float max_load_factor_, growth_factor_;

    struct Slot
    {
        Key k;
        Value v;
        enum class State
        {
            FILLED,
            TOMBSTONE,
            EMPTY
        };
        State state = State::EMPTY;
    };

    std::vector<Slot> slots;

    auto grow() -> void
    {
        size_t new_size;
        if (number_of_slots_total == 0)
            new_size = DEFAULT_START_BUCKETS_SIZE;
        else
            new_size =
                static_cast<size_t>(growth_factor_ * static_cast<float>(number_of_slots_total)) +
                number_of_slots_total;

        std::vector<Slot> new_slots(new_size);
        // Move all elements from the old table to the new one
        for (size_t i = 0; i < slots.size(); ++i)
        {
            if (slots[i].state == Slot::State::FILLED)
            {
                auto new_index = hasher_(slots[i].k) % new_size;
                new_slots[new_index] = slots[i];
            }
        }
        std::swap(slots, new_slots);
        number_of_slots_total = new_size;
    }

  public:
    using key_type = Key;
    using mapped_type = Value;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    static constexpr float DEFAULT_MAX_LOAD_FACTOR = 0.7f;
    static constexpr float DEFAULT_GROWTH_FACTOR = 1.0f;
    static constexpr size_t DEFAULT_START_BUCKETS_SIZE = 8;

    // using value_type = std::pair<const Key, Value>;
    // using reference = value_type &;
    // using const_reference = const value_type &;

    HashMap()
        : sz(0), number_of_slots_used(0), number_of_slots_total(0),
          max_load_factor_(DEFAULT_MAX_LOAD_FACTOR), growth_factor_(DEFAULT_GROWTH_FACTOR)
    {
    }

    auto find(const Key &key) -> std::optional<Value>
    {
        auto index = hasher_(key) % number_of_slots_total;
        // TODO: Implement wrap around
        while (index < number_of_slots_total)
        {
            if (slots[index].state == Slot::State::FILLED && slots[index].k == key)
            {
                return slots[index].v;
            }
            ++index;
        }
        return std::nullopt;
    }

    auto insert(const Key &key, const Value &value) -> void
    {
        if (number_of_slots_total == 0 || load_factor() >= max_load_factor())
            grow();

        // Perform linear probing until a free slot is found
        size_t index = 0;
        while (true)
        {
            index = hasher_(key) % number_of_slots_total;
            while (index < number_of_slots_total)
            {
                // Overwrite the value if there is an existing key
                if (slots[index].state == Slot::State::FILLED && slots[index].k == key)
                {
                    slots[index].v = value;
                    return;
                }
                if (slots[index].state == Slot::State::EMPTY)
                    break;
                ++index;
            }
            // TODO: Also wrap around and check
            if (index >= number_of_slots_total)
            {
                // No free slot was found, grow the table and try again
                grow();
            }
            else
                break;
        }

        // We found an empty slot
        slots[index].k = key;
        slots[index].v = value;
        slots[index].state = Slot::State::FILLED;
        ++number_of_slots_used;
        ++sz;
    }

    auto erase(const Key &key) -> void {}

    auto contains(const Key &key) const -> bool {}

    auto size() const -> size_type { return sz; }

    auto clear() -> void {}

    auto load_factor() const -> float
    {
        if (number_of_slots_total)
            return static_cast<float>(number_of_slots_used) /
                   static_cast<float>(number_of_slots_total);
        return 0;
    }

    auto max_load_factor() const -> float { return max_load_factor_; }

    auto max_load_factor(float new_max_load_factor) -> void
    {
        if (new_max_load_factor > 1.0f || new_max_load_factor < 0.0f)
            throw std::logic_error("Invalid Max load factor, it should be between 0 and 1");
        max_load_factor_ = new_max_load_factor;
    }

    auto growth_factor() const -> float { return growth_factor_; }

    auto growth_factor(float new_growth_factor) -> void
    {
        if (new_growth_factor > 1.0f)
            throw std::logic_error("Growth factor is greater than 1");
        growth_factor_ = new_growth_factor;
    }
};

#endif // A_HASHMAP_H