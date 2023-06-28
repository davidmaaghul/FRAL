// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: medium.proto

#include "medium.pb.h"

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

namespace medium {
PROTOBUF_CONSTEXPR Start::Start(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.idx_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct StartDefaultTypeInternal {
  PROTOBUF_CONSTEXPR StartDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~StartDefaultTypeInternal() {}
  union {
    Start _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 StartDefaultTypeInternal _Start_default_instance_;
PROTOBUF_CONSTEXPR Allocation::Allocation(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.allocation_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.idx_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct AllocationDefaultTypeInternal {
  PROTOBUF_CONSTEXPR AllocationDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~AllocationDefaultTypeInternal() {}
  union {
    Allocation _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 AllocationDefaultTypeInternal _Allocation_default_instance_;
PROTOBUF_CONSTEXPR Empty::Empty(
    ::_pbi::ConstantInitialized) {}
struct EmptyDefaultTypeInternal {
  PROTOBUF_CONSTEXPR EmptyDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~EmptyDefaultTypeInternal() {}
  union {
    Empty _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 EmptyDefaultTypeInternal _Empty_default_instance_;
}  // namespace medium
static ::_pb::Metadata file_level_metadata_medium_2eproto[3];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_medium_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_medium_2eproto = nullptr;

const uint32_t TableStruct_medium_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::medium::Start, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::medium::Start, _impl_.idx_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::medium::Allocation, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::medium::Allocation, _impl_.idx_),
  PROTOBUF_FIELD_OFFSET(::medium::Allocation, _impl_.allocation_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::medium::Empty, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::medium::Start)},
  { 7, -1, -1, sizeof(::medium::Allocation)},
  { 15, -1, -1, sizeof(::medium::Empty)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::medium::_Start_default_instance_._instance,
  &::medium::_Allocation_default_instance_._instance,
  &::medium::_Empty_default_instance_._instance,
};

const char descriptor_table_protodef_medium_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\014medium.proto\022\006medium\"\024\n\005Start\022\013\n\003idx\030\001"
  " \001(\005\"-\n\nAllocation\022\013\n\003idx\030\001 \001(\005\022\022\n\nalloc"
  "ation\030\002 \001(\014\"\007\n\005Empty2\216\001\n\006Medium\022)\n\007conne"
  "ct\022\r.medium.Empty\032\r.medium.Start\"\000\022-\n\004sy"
  "nc\022\022.medium.Allocation\032\r.medium.Empty\"\000("
  "\001\022*\n\010shutdown\022\r.medium.Empty\032\r.medium.Em"
  "pty\"\000b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_medium_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_medium_2eproto = {
    false, false, 253, descriptor_table_protodef_medium_2eproto,
    "medium.proto",
    &descriptor_table_medium_2eproto_once, nullptr, 0, 3,
    schemas, file_default_instances, TableStruct_medium_2eproto::offsets,
    file_level_metadata_medium_2eproto, file_level_enum_descriptors_medium_2eproto,
    file_level_service_descriptors_medium_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_medium_2eproto_getter() {
  return &descriptor_table_medium_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_medium_2eproto(&descriptor_table_medium_2eproto);
namespace medium {

// ===================================================================

class Start::_Internal {
 public:
};

Start::Start(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:medium.Start)
}
Start::Start(const Start& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Start* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.idx_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.idx_ = from._impl_.idx_;
  // @@protoc_insertion_point(copy_constructor:medium.Start)
}

inline void Start::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.idx_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

Start::~Start() {
  // @@protoc_insertion_point(destructor:medium.Start)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Start::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void Start::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Start::Clear() {
// @@protoc_insertion_point(message_clear_start:medium.Start)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.idx_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Start::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 idx = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.idx_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
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

uint8_t* Start::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:medium.Start)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 idx = 1;
  if (this->_internal_idx() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_idx(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:medium.Start)
  return target;
}

size_t Start::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:medium.Start)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 idx = 1;
  if (this->_internal_idx() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_idx());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Start::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Start::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Start::GetClassData() const { return &_class_data_; }


void Start::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Start*>(&to_msg);
  auto& from = static_cast<const Start&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:medium.Start)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_idx() != 0) {
    _this->_internal_set_idx(from._internal_idx());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Start::CopyFrom(const Start& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:medium.Start)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Start::IsInitialized() const {
  return true;
}

void Start::InternalSwap(Start* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.idx_, other->_impl_.idx_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Start::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_medium_2eproto_getter, &descriptor_table_medium_2eproto_once,
      file_level_metadata_medium_2eproto[0]);
}

// ===================================================================

class Allocation::_Internal {
 public:
};

Allocation::Allocation(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:medium.Allocation)
}
Allocation::Allocation(const Allocation& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Allocation* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.allocation_){}
    , decltype(_impl_.idx_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.allocation_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.allocation_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_allocation().empty()) {
    _this->_impl_.allocation_.Set(from._internal_allocation(), 
      _this->GetArenaForAllocation());
  }
  _this->_impl_.idx_ = from._impl_.idx_;
  // @@protoc_insertion_point(copy_constructor:medium.Allocation)
}

inline void Allocation::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.allocation_){}
    , decltype(_impl_.idx_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.allocation_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.allocation_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

Allocation::~Allocation() {
  // @@protoc_insertion_point(destructor:medium.Allocation)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Allocation::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.allocation_.Destroy();
}

void Allocation::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Allocation::Clear() {
// @@protoc_insertion_point(message_clear_start:medium.Allocation)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.allocation_.ClearToEmpty();
  _impl_.idx_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Allocation::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 idx = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.idx_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bytes allocation = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_allocation();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
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

uint8_t* Allocation::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:medium.Allocation)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 idx = 1;
  if (this->_internal_idx() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_idx(), target);
  }

  // bytes allocation = 2;
  if (!this->_internal_allocation().empty()) {
    target = stream->WriteBytesMaybeAliased(
        2, this->_internal_allocation(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:medium.Allocation)
  return target;
}

size_t Allocation::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:medium.Allocation)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes allocation = 2;
  if (!this->_internal_allocation().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_allocation());
  }

  // int32 idx = 1;
  if (this->_internal_idx() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_idx());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Allocation::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Allocation::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Allocation::GetClassData() const { return &_class_data_; }


void Allocation::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Allocation*>(&to_msg);
  auto& from = static_cast<const Allocation&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:medium.Allocation)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_allocation().empty()) {
    _this->_internal_set_allocation(from._internal_allocation());
  }
  if (from._internal_idx() != 0) {
    _this->_internal_set_idx(from._internal_idx());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Allocation::CopyFrom(const Allocation& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:medium.Allocation)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Allocation::IsInitialized() const {
  return true;
}

void Allocation::InternalSwap(Allocation* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.allocation_, lhs_arena,
      &other->_impl_.allocation_, rhs_arena
  );
  swap(_impl_.idx_, other->_impl_.idx_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Allocation::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_medium_2eproto_getter, &descriptor_table_medium_2eproto_once,
      file_level_metadata_medium_2eproto[1]);
}

// ===================================================================

class Empty::_Internal {
 public:
};

Empty::Empty(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase(arena, is_message_owned) {
  // @@protoc_insertion_point(arena_constructor:medium.Empty)
}
Empty::Empty(const Empty& from)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase() {
  Empty* const _this = this; (void)_this;
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:medium.Empty)
}





const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Empty::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::CopyImpl,
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::MergeImpl,
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Empty::GetClassData() const { return &_class_data_; }







::PROTOBUF_NAMESPACE_ID::Metadata Empty::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_medium_2eproto_getter, &descriptor_table_medium_2eproto_once,
      file_level_metadata_medium_2eproto[2]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace medium
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::medium::Start*
Arena::CreateMaybeMessage< ::medium::Start >(Arena* arena) {
  return Arena::CreateMessageInternal< ::medium::Start >(arena);
}
template<> PROTOBUF_NOINLINE ::medium::Allocation*
Arena::CreateMaybeMessage< ::medium::Allocation >(Arena* arena) {
  return Arena::CreateMessageInternal< ::medium::Allocation >(arena);
}
template<> PROTOBUF_NOINLINE ::medium::Empty*
Arena::CreateMaybeMessage< ::medium::Empty >(Arena* arena) {
  return Arena::CreateMessageInternal< ::medium::Empty >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
