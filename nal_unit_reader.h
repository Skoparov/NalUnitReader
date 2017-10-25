#ifndef __NAL_UNIT_READER_H__
#define __NAL_UNIT_READER_H__

#include <cstddef>
#include <utility>
#include <iterator>

template< typename _iterator >
struct nal_unit
{
public:
    using iterator = _iterator;

private:
    using iterator_diff = typename std::iterator_traits< iterator >::difference_type;

public:
    nal_unit( iterator data, iterator_diff size ) noexcept;
    nal_unit( const nal_unit& o ) noexcept;
    nal_unit( nal_unit&& o ) noexcept;
    nal_unit& operator=( const nal_unit& o ) noexcept;
    nal_unit& operator=( nal_unit&& o ) noexcept;

    iterator const data() const noexcept{ return m_data; }
    iterator_diff size() const noexcept{ return m_size; }
    explicit operator bool() const noexcept{ return m_size; }

    bool key_frame() const noexcept{ return type() == 0x5; }
    uint8_t type() const noexcept{ return m_data[ 0 ] & 0x1f; }
    bool has_start_code() const noexcept{ return m_data[ 0 ] & 0x80; }

private:
    iterator m_data;
    iterator_diff m_size{ 0 };
};

template< typename _iterator >
class nal_unit_reader
{
public:
    using iterator = _iterator;

private:
    using iterator_diff = typename std::iterator_traits< iterator >::difference_type;

public:
    nal_unit_reader() = default;
    nal_unit_reader( iterator data_begin, iterator data_end );
    nal_unit_reader( const nal_unit_reader& o ) noexcept;
    nal_unit_reader( nal_unit_reader&& o ) noexcept;
    nal_unit_reader& operator=( const nal_unit_reader& o ) noexcept;
    nal_unit_reader& operator=( nal_unit_reader&& o ) noexcept;

    void set_data( iterator data_begin, iterator data_end );
    void reset() noexcept{ m_initialized = false; }

    nal_unit< iterator > get_next();
    bool depleted() const noexcept{ return !m_initialized || m_next_nal_unit >= m_end; }

private:
    std::pair< iterator, iterator_diff > next_start_code_pos_len( iterator begin, iterator end ) const;

private:
    bool m_initialized{ false };
    iterator m_next_nal_unit;
    iterator m_end;
};

#endif

#include "nal_unit_reader.impl"
