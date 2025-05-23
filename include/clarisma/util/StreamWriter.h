// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <clarisma/util/BufferWriter.h>

namespace clarisma {

template<typename S>
class AbstractStreamWriter : public BufferWriter
{
public:
	AbstractStreamWriter() = default;
  	explicit AbstractStreamWriter(Buffer* buf) :  BufferWriter(buf) {}

	S& operator<<(const char* s)
  	{
  		writeString(s);
  		return static_cast<S&>(*this);
  	}

	S& operator<<(std::string_view s)
  	{
  		writeString(s);
  		return static_cast<S&>(*this);
  	}

	S& operator<<(std::string s)
  	{
  		writeString(s);
  		return static_cast<S&>(*this);
  	}

	BufferWriter& operator<<(char ch)
  	{
  		writeByte(ch);
  		return *this;
  	}

	S& operator<<(int n)
  	{
  		formatInt(n);
  		return static_cast<S&>(*this);
  	}

	S& operator<<(uint64_t n)
  	{
  		formatUnsignedInt(n);
  		return static_cast<S&>(*this);
  	}

	S& operator<<(int64_t n)
  	{
  		formatInt(n);
  		return static_cast<S&>(*this);
  	}
};

class StreamWriter : public AbstractStreamWriter<StreamWriter>
{
public:
	using AbstractStreamWriter::AbstractStreamWriter;
};

} // namespace clarisma
