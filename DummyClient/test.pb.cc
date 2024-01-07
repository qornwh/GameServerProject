// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test.proto

#include "test.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace Protocol {
PROTOBUF_CONSTEXPR TestPlayerInfo::TestPlayerInfo(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.arr_)*/{}
  , /*decltype(_impl_._arr_cached_byte_size_)*/{0}
  , /*decltype(_impl_.vector_)*/nullptr
  , /*decltype(_impl_.type_)*/0
  , /*decltype(_impl_.id_)*/0u
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct TestPlayerInfoDefaultTypeInternal {
  PROTOBUF_CONSTEXPR TestPlayerInfoDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~TestPlayerInfoDefaultTypeInternal() {}
  union {
    TestPlayerInfo _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 TestPlayerInfoDefaultTypeInternal _TestPlayerInfo_default_instance_;
PROTOBUF_CONSTEXPR Vector::Vector(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.x_)*/0
  , /*decltype(_impl_.y_)*/0
  , /*decltype(_impl_.z_)*/0
  , /*decltype(_impl_.yew_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct VectorDefaultTypeInternal {
  PROTOBUF_CONSTEXPR VectorDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~VectorDefaultTypeInternal() {}
  union {
    Vector _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 VectorDefaultTypeInternal _Vector_default_instance_;
}  // namespace Protocol
static ::_pb::Metadata file_level_metadata_test_2eproto[2];
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_test_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_test_2eproto = nullptr;

const uint32_t TableStruct_test_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::TestPlayerInfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Protocol::TestPlayerInfo, _impl_.type_),
  PROTOBUF_FIELD_OFFSET(::Protocol::TestPlayerInfo, _impl_.id_),
  PROTOBUF_FIELD_OFFSET(::Protocol::TestPlayerInfo, _impl_.vector_),
  PROTOBUF_FIELD_OFFSET(::Protocol::TestPlayerInfo, _impl_.arr_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::Vector, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Protocol::Vector, _impl_.x_),
  PROTOBUF_FIELD_OFFSET(::Protocol::Vector, _impl_.y_),
  PROTOBUF_FIELD_OFFSET(::Protocol::Vector, _impl_.z_),
  PROTOBUF_FIELD_OFFSET(::Protocol::Vector, _impl_.yew_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::Protocol::TestPlayerInfo)},
  { 10, -1, -1, sizeof(::Protocol::Vector)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::Protocol::_TestPlayerInfo_default_instance_._instance,
  &::Protocol::_Vector_default_instance_._instance,
};

const char descriptor_table_protodef_test_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\ntest.proto\022\010Protocol\"i\n\016TestPlayerInfo"
  "\022\034\n\004type\030\001 \001(\0162\016.Protocol.Type\022\n\n\002id\030\002 \001"
  "(\r\022 \n\006vector\030\003 \001(\0132\020.Protocol.Vector\022\013\n\003"
  "arr\030\005 \003(\005\"6\n\006Vector\022\t\n\001x\030\001 \001(\002\022\t\n\001y\030\002 \001("
  "\002\022\t\n\001z\030\003 \001(\002\022\013\n\003yew\030\004 \001(\002*<\n\004Type\022\013\n\007DEF"
  "AULT\020\000\022\n\n\006PLAYER\020\001\022\016\n\nTESTPLAYER\020\002\022\013\n\007MO"
  "NSTER\020\003b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_test_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_test_2eproto = {
    false, false, 255, descriptor_table_protodef_test_2eproto,
    "test.proto",
    &descriptor_table_test_2eproto_once, nullptr, 0, 2,
    schemas, file_default_instances, TableStruct_test_2eproto::offsets,
    file_level_metadata_test_2eproto, file_level_enum_descriptors_test_2eproto,
    file_level_service_descriptors_test_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_test_2eproto_getter() {
  return &descriptor_table_test_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_test_2eproto(&descriptor_table_test_2eproto);
namespace Protocol {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Type_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_test_2eproto);
  return file_level_enum_descriptors_test_2eproto[0];
}
bool Type_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}


// ===================================================================

class TestPlayerInfo::_Internal {
 public:
  static const ::Protocol::Vector& vector(const TestPlayerInfo* msg);
};

const ::Protocol::Vector&
TestPlayerInfo::_Internal::vector(const TestPlayerInfo* msg) {
  return *msg->_impl_.vector_;
}
TestPlayerInfo::TestPlayerInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:Protocol.TestPlayerInfo)
}
TestPlayerInfo::TestPlayerInfo(const TestPlayerInfo& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  TestPlayerInfo* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.arr_){from._impl_.arr_}
    , /*decltype(_impl_._arr_cached_byte_size_)*/{0}
    , decltype(_impl_.vector_){nullptr}
    , decltype(_impl_.type_){}
    , decltype(_impl_.id_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_vector()) {
    _this->_impl_.vector_ = new ::Protocol::Vector(*from._impl_.vector_);
  }
  ::memcpy(&_impl_.type_, &from._impl_.type_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.id_) -
    reinterpret_cast<char*>(&_impl_.type_)) + sizeof(_impl_.id_));
  // @@protoc_insertion_point(copy_constructor:Protocol.TestPlayerInfo)
}

inline void TestPlayerInfo::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.arr_){arena}
    , /*decltype(_impl_._arr_cached_byte_size_)*/{0}
    , decltype(_impl_.vector_){nullptr}
    , decltype(_impl_.type_){0}
    , decltype(_impl_.id_){0u}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

TestPlayerInfo::~TestPlayerInfo() {
  // @@protoc_insertion_point(destructor:Protocol.TestPlayerInfo)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void TestPlayerInfo::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.arr_.~RepeatedField();
  if (this != internal_default_instance()) delete _impl_.vector_;
}

void TestPlayerInfo::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void TestPlayerInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:Protocol.TestPlayerInfo)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.arr_.Clear();
  if (GetArenaForAllocation() == nullptr && _impl_.vector_ != nullptr) {
    delete _impl_.vector_;
  }
  _impl_.vector_ = nullptr;
  ::memset(&_impl_.type_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.id_) -
      reinterpret_cast<char*>(&_impl_.type_)) + sizeof(_impl_.id_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* TestPlayerInfo::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .Protocol.Type type = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_type(static_cast<::Protocol::Type>(val));
        } else
          goto handle_unusual;
        continue;
      // uint32 id = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _impl_.id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .Protocol.Vector vector = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_vector(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // repeated int32 arr = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 42)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedInt32Parser(_internal_mutable_arr(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<uint8_t>(tag) == 40) {
          _internal_add_arr(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* TestPlayerInfo::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Protocol.TestPlayerInfo)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .Protocol.Type type = 1;
  if (this->_internal_type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      1, this->_internal_type(), target);
  }

  // uint32 id = 2;
  if (this->_internal_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(2, this->_internal_id(), target);
  }

  // .Protocol.Vector vector = 3;
  if (this->_internal_has_vector()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(3, _Internal::vector(this),
        _Internal::vector(this).GetCachedSize(), target, stream);
  }

  // repeated int32 arr = 5;
  {
    int byte_size = _impl_._arr_cached_byte_size_.load(std::memory_order_relaxed);
    if (byte_size > 0) {
      target = stream->WriteInt32Packed(
          5, _internal_arr(), byte_size, target);
    }
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Protocol.TestPlayerInfo)
  return target;
}

size_t TestPlayerInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Protocol.TestPlayerInfo)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated int32 arr = 5;
  {
    size_t data_size = ::_pbi::WireFormatLite::
      Int32Size(this->_impl_.arr_);
    if (data_size > 0) {
      total_size += 1 +
        ::_pbi::WireFormatLite::Int32Size(static_cast<int32_t>(data_size));
    }
    int cached_size = ::_pbi::ToCachedSize(data_size);
    _impl_._arr_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // .Protocol.Vector vector = 3;
  if (this->_internal_has_vector()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.vector_);
  }

  // .Protocol.Type type = 1;
  if (this->_internal_type() != 0) {
    total_size += 1 +
      ::_pbi::WireFormatLite::EnumSize(this->_internal_type());
  }

  // uint32 id = 2;
  if (this->_internal_id() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_id());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData TestPlayerInfo::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    TestPlayerInfo::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*TestPlayerInfo::GetClassData() const { return &_class_data_; }


void TestPlayerInfo::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<TestPlayerInfo*>(&to_msg);
  auto& from = static_cast<const TestPlayerInfo&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:Protocol.TestPlayerInfo)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_impl_.arr_.MergeFrom(from._impl_.arr_);
  if (from._internal_has_vector()) {
    _this->_internal_mutable_vector()->::Protocol::Vector::MergeFrom(
        from._internal_vector());
  }
  if (from._internal_type() != 0) {
    _this->_internal_set_type(from._internal_type());
  }
  if (from._internal_id() != 0) {
    _this->_internal_set_id(from._internal_id());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void TestPlayerInfo::CopyFrom(const TestPlayerInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Protocol.TestPlayerInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TestPlayerInfo::IsInitialized() const {
  return true;
}

void TestPlayerInfo::InternalSwap(TestPlayerInfo* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.arr_.InternalSwap(&other->_impl_.arr_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(TestPlayerInfo, _impl_.id_)
      + sizeof(TestPlayerInfo::_impl_.id_)
      - PROTOBUF_FIELD_OFFSET(TestPlayerInfo, _impl_.vector_)>(
          reinterpret_cast<char*>(&_impl_.vector_),
          reinterpret_cast<char*>(&other->_impl_.vector_));
}

::PROTOBUF_NAMESPACE_ID::Metadata TestPlayerInfo::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_test_2eproto_getter, &descriptor_table_test_2eproto_once,
      file_level_metadata_test_2eproto[0]);
}

// ===================================================================

class Vector::_Internal {
 public:
};

Vector::Vector(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:Protocol.Vector)
}
Vector::Vector(const Vector& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Vector* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.x_){}
    , decltype(_impl_.y_){}
    , decltype(_impl_.z_){}
    , decltype(_impl_.yew_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&_impl_.x_, &from._impl_.x_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.yew_) -
    reinterpret_cast<char*>(&_impl_.x_)) + sizeof(_impl_.yew_));
  // @@protoc_insertion_point(copy_constructor:Protocol.Vector)
}

inline void Vector::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.x_){0}
    , decltype(_impl_.y_){0}
    , decltype(_impl_.z_){0}
    , decltype(_impl_.yew_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

Vector::~Vector() {
  // @@protoc_insertion_point(destructor:Protocol.Vector)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Vector::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void Vector::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Vector::Clear() {
// @@protoc_insertion_point(message_clear_start:Protocol.Vector)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&_impl_.x_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.yew_) -
      reinterpret_cast<char*>(&_impl_.x_)) + sizeof(_impl_.yew_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Vector::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // float x = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 13)) {
          _impl_.x_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else
          goto handle_unusual;
        continue;
      // float y = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 21)) {
          _impl_.y_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else
          goto handle_unusual;
        continue;
      // float z = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 29)) {
          _impl_.z_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else
          goto handle_unusual;
        continue;
      // float yew = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 37)) {
          _impl_.yew_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Vector::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Protocol.Vector)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // float x = 1;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_x = this->_internal_x();
  uint32_t raw_x;
  memcpy(&raw_x, &tmp_x, sizeof(tmp_x));
  if (raw_x != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteFloatToArray(1, this->_internal_x(), target);
  }

  // float y = 2;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_y = this->_internal_y();
  uint32_t raw_y;
  memcpy(&raw_y, &tmp_y, sizeof(tmp_y));
  if (raw_y != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteFloatToArray(2, this->_internal_y(), target);
  }

  // float z = 3;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_z = this->_internal_z();
  uint32_t raw_z;
  memcpy(&raw_z, &tmp_z, sizeof(tmp_z));
  if (raw_z != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteFloatToArray(3, this->_internal_z(), target);
  }

  // float yew = 4;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_yew = this->_internal_yew();
  uint32_t raw_yew;
  memcpy(&raw_yew, &tmp_yew, sizeof(tmp_yew));
  if (raw_yew != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteFloatToArray(4, this->_internal_yew(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Protocol.Vector)
  return target;
}

size_t Vector::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Protocol.Vector)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // float x = 1;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_x = this->_internal_x();
  uint32_t raw_x;
  memcpy(&raw_x, &tmp_x, sizeof(tmp_x));
  if (raw_x != 0) {
    total_size += 1 + 4;
  }

  // float y = 2;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_y = this->_internal_y();
  uint32_t raw_y;
  memcpy(&raw_y, &tmp_y, sizeof(tmp_y));
  if (raw_y != 0) {
    total_size += 1 + 4;
  }

  // float z = 3;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_z = this->_internal_z();
  uint32_t raw_z;
  memcpy(&raw_z, &tmp_z, sizeof(tmp_z));
  if (raw_z != 0) {
    total_size += 1 + 4;
  }

  // float yew = 4;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_yew = this->_internal_yew();
  uint32_t raw_yew;
  memcpy(&raw_yew, &tmp_yew, sizeof(tmp_yew));
  if (raw_yew != 0) {
    total_size += 1 + 4;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Vector::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Vector::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Vector::GetClassData() const { return &_class_data_; }


void Vector::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Vector*>(&to_msg);
  auto& from = static_cast<const Vector&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:Protocol.Vector)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_x = from._internal_x();
  uint32_t raw_x;
  memcpy(&raw_x, &tmp_x, sizeof(tmp_x));
  if (raw_x != 0) {
    _this->_internal_set_x(from._internal_x());
  }
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_y = from._internal_y();
  uint32_t raw_y;
  memcpy(&raw_y, &tmp_y, sizeof(tmp_y));
  if (raw_y != 0) {
    _this->_internal_set_y(from._internal_y());
  }
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_z = from._internal_z();
  uint32_t raw_z;
  memcpy(&raw_z, &tmp_z, sizeof(tmp_z));
  if (raw_z != 0) {
    _this->_internal_set_z(from._internal_z());
  }
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_yew = from._internal_yew();
  uint32_t raw_yew;
  memcpy(&raw_yew, &tmp_yew, sizeof(tmp_yew));
  if (raw_yew != 0) {
    _this->_internal_set_yew(from._internal_yew());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Vector::CopyFrom(const Vector& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Protocol.Vector)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Vector::IsInitialized() const {
  return true;
}

void Vector::InternalSwap(Vector* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Vector, _impl_.yew_)
      + sizeof(Vector::_impl_.yew_)
      - PROTOBUF_FIELD_OFFSET(Vector, _impl_.x_)>(
          reinterpret_cast<char*>(&_impl_.x_),
          reinterpret_cast<char*>(&other->_impl_.x_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Vector::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_test_2eproto_getter, &descriptor_table_test_2eproto_once,
      file_level_metadata_test_2eproto[1]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::Protocol::TestPlayerInfo*
Arena::CreateMaybeMessage< ::Protocol::TestPlayerInfo >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::TestPlayerInfo >(arena);
}
template<> PROTOBUF_NOINLINE ::Protocol::Vector*
Arena::CreateMaybeMessage< ::Protocol::Vector >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::Vector >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
