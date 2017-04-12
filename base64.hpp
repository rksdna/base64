#pragma once
#include <array>
#include <algorithm>

namespace base64
{

const std::array<char, 0x40> charset =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

struct context
{
    using buffer_type = std::uint_fast16_t;

    buffer_type buffer = 0;
    std::size_t count = 0;
};

template <typename InputIterator,
          typename OutputIterator>
OutputIterator encode_more(base64::context &context, InputIterator first, InputIterator last, OutputIterator output)
{
    while (first != last)
    {
        const auto input = *first++;

        const base64::context::buffer_type data = base64::context::buffer_type(input) & 0xFF;
        context.buffer = context.buffer << 8 | data;
        context.count += 8;

        while (context.count >= 6)
        {
            context.count -= 6;
            *output++ = charset.at((context.buffer >> context.count) & 0x3F);
        }
    }

    if (context.count)
    {
        context.count = 6 - context.count;
        context.buffer <<= context.count;
        *output++ = charset.at(context.buffer & 0x3F);

        while (context.count)
        {
            context.count -= 2;
            *output++ = '=';
        }
    }

    return output;
}

template <typename InputIterator,
          typename OutputIterator>
OutputIterator encode(InputIterator first, InputIterator last, OutputIterator output)
{
    base64::context context;
    return encode_more(context, first, last, output);
}

template <typename InputIterator,
          typename OutputIterator>
OutputIterator decode_more(base64::context &context, InputIterator first, InputIterator last, OutputIterator output)
{
    while (first != last)
    {
        const auto input = *first++;

        if (input == '=')
        {
            context = {};
            continue;
        }

        const auto it = std::find(charset.begin(), charset.end(), input);
        if (it == charset.end())
            throw std::invalid_argument("Unexpected character");

        context.buffer = context.buffer << 6 | base64::context::buffer_type(it - charset.begin());
        context.count += 6;

        while (context.count >= 8)
        {
            context.count -= 8;
            *output++ = (context.buffer >> context.count) & 0xFF;
        }
    }

    return output;
}

template <typename InputIterator,
          typename OutputIterator>
OutputIterator decode(InputIterator first, InputIterator last, OutputIterator output)
{
    base64::context context;
    return decode_more(context, first, last, output);
}

}

