
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2012 Francois Beaune, Jupiter Jazz Limited
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// Interface header.
#include "connectableentity.h"

// appleseed.renderer headers.
#include "renderer/global/globallogger.h"
#include "renderer/global/globaltypes.h"
#include "renderer/modeling/input/source.h"

// Standard headers.
#include <cassert>

namespace renderer
{

bool ConnectableEntity::is_uniform_zero(const Source* source)
{
    assert(source);

    if (source->is_uniform())
    {
        Spectrum spectrum;
        Alpha alpha;
        source->evaluate_uniform(spectrum, alpha);

        if (spectrum == Spectrum(0.0f))
            return true;
    }

    return false;
}

bool ConnectableEntity::is_uniform_zero(const char* input_name) const
{
    return is_uniform_zero(m_inputs.source(input_name));
}

bool ConnectableEntity::is_uniform_zero(const Source* source, const Source* multiplier)
{
    return is_uniform_zero(source) || is_uniform_zero(multiplier);
}

bool ConnectableEntity::is_uniform_zero(const char* input_name, const char* multiplier_name) const
{
    return is_uniform_zero(m_inputs.source(input_name), m_inputs.source(multiplier_name));
}

bool ConnectableEntity::check_uniform(const char* input_name) const
{
    const Source* source = m_inputs.source(input_name);

    assert(source);

    if (source->is_uniform())
        return true;

    RENDERER_LOG_ERROR(
        "the \"%s\" input of \"%s\" must be bound to a scalar or a color.",
        input_name,
        get_name());

    return false;
}

void ConnectableEntity::check_non_zero_exitance(const Source* source) const
{
    if (is_uniform_zero(source))
        warn_zero_exitance();
}

void ConnectableEntity::check_non_zero_exitance(const char* input_name) const
{
    if (is_uniform_zero(input_name))
        warn_zero_exitance();
}

void ConnectableEntity::check_non_zero_exitance(const Source* source, const Source* multiplier) const
{
    if (is_uniform_zero(source, multiplier))
        warn_zero_exitance();
}

void ConnectableEntity::check_non_zero_exitance(const char* input_name, const char* multiplier_name) const
{
    if (is_uniform_zero(input_name, multiplier_name))
        warn_zero_exitance();
}

void ConnectableEntity::warn_zero_exitance() const
{
    RENDERER_LOG_WARNING(
        "\"%s\" has a zero exitance and will slow down rendering "
        "without contributing to the lighting.",
        get_name());
}

}   // namespace renderer
