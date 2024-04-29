#pragma once
#include <iostream>
#include <atomic>
#include <boost/asio.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>

using namespace std;

using BYTE = boost::uint8_t;
using int8 = boost::int8_t;
using int16 = boost::int16_t;
using int32 = boost::int32_t;
using int64 = boost::int64_t;
using uint8 = boost::uint8_t;
using uint16 = boost::uint16_t;
using uint32 = boost::uint32_t;
using uint64 = boost::uint64_t;

template <typename T>
using Atomic = std::atomic<T>;

using SessionRef = boost::shared_ptr<class Session>;
using SendBufferRef = boost::shared_ptr<class SendBuffer>;
using SendBufferChunkRef = boost::shared_ptr<class SendBufferChunk>;
using SendBufferManagerRef = boost::shared_ptr<class SendBufferManager>;
using ServiceRef = boost::shared_ptr<class Service>;