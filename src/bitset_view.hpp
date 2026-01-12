#pragma once

#include <bitset>
#include <cstddef>
#include <iterator>
#include <ranges>

namespace bitset_view
{

// ============================================================
// 1. bits_view  -> iterate over all bits (bool / proxy)
// ============================================================

template <std::size_t N, bool IsConst>
class bits_iterator
{
public:
    using bitset_type = std::conditional_t<
        IsConst,
        const std::bitset<N>,
        std::bitset<N>>;

    using value_type = bool;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    // Proxy reference for mutable access
    using reference = std::conditional_t<
        IsConst,
        bool,
        typename std::bitset<N>::reference>;

    bits_iterator() = default;

    bits_iterator(bitset_type* bs, std::size_t pos)
        : bs_(bs), pos_(pos) {}

    reference operator*() const
    {
        return (*bs_)[pos_];
    }

    bits_iterator& operator++()
    {
        ++pos_;
        return *this;
    }

    bits_iterator operator++(int)
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator==(const bits_iterator& a,
                           const bits_iterator& b)
    {
        return a.pos_ == b.pos_;
    }

private:
    bitset_type* bs_{nullptr};
    std::size_t pos_{0};
};

template <std::size_t N>
class bits_view
{
public:
    explicit bits_view(std::bitset<N>& bs) : bs_(&bs) {}
    explicit bits_view(const std::bitset<N>& bs) : cbs_(&bs) {}

    auto begin()
    {
        return bits_iterator<N, false>(bs_, 0);
    }

    auto end()
    {
        return bits_iterator<N, false>(bs_, N);
    }

    auto begin() const
    {
        return bits_iterator<N, true>(cbs_, 0);
    }

    auto end() const
    {
        return bits_iterator<N, true>(cbs_, N);
    }

private:
    std::bitset<N>* bs_{nullptr};
    const std::bitset<N>* cbs_{nullptr};
};

// helper
template <std::size_t N>
bits_view<N> bits(std::bitset<N>& bs)
{
    return bits_view<N>(bs);
}

template <std::size_t N>
bits_view<N> bits(const std::bitset<N>& bs)
{
    return bits_view<N>(bs);
}

// ============================================================
// 2. set_bits_view  -> iterate over indices of set bits
// ============================================================

template <std::size_t N>
class set_bits_iterator
{
public:
    using value_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    set_bits_iterator() = default;

    set_bits_iterator(const std::bitset<N>* bs, std::size_t pos)
        : bs_(bs), pos_(pos)
    {
        advance_to_next_set();
    }

    value_type operator*() const
    {
        return pos_;
    }

    set_bits_iterator& operator++()
    {
        ++pos_;
        advance_to_next_set();
        return *this;
    }

    set_bits_iterator operator++(int)
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator==(const set_bits_iterator& a,
                           const set_bits_iterator& b)
    {
        return a.pos_ == b.pos_;
    }

private:
    void advance_to_next_set()
    {
        while (pos_ < N && !bs_->test(pos_))
        {
            ++pos_;
        }
    }

    const std::bitset<N>* bs_{nullptr};
    std::size_t pos_{N};
};

template <std::size_t N>
class set_bits_view
{
public:
    explicit set_bits_view(const std::bitset<N>& bs) : bs_(&bs) {}

    auto begin() const
    {
        return set_bits_iterator<N>(bs_, 0);
    }

    auto end() const
    {
        return set_bits_iterator<N>(bs_, N);
    }

private:
    const std::bitset<N>* bs_;
};

// helper
template <std::size_t N>
set_bits_view<N> set_bits(const std::bitset<N>& bs)
{
    return set_bits_view<N>(bs);
}

} // namespace bitset_view
