// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: types.proto

#ifndef PROTOBUF_types_2eproto__INCLUDED
#define PROTOBUF_types_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace FTSFC {
class PiggyBackMessage;
class PiggyBackMessageDefaultTypeInternal;
extern PiggyBackMessageDefaultTypeInternal _PiggyBackMessage_default_instance_;
class PiggyBackState;
class PiggyBackStateDefaultTypeInternal;
extern PiggyBackStateDefaultTypeInternal _PiggyBackState_default_instance_;
class State;
class StateDefaultTypeInternal;
extern StateDefaultTypeInternal _State_default_instance_;
class TimestampState;
class TimestampStateDefaultTypeInternal;
extern TimestampStateDefaultTypeInternal _TimestampState_default_instance_;
}  // namespace FTSFC

namespace FTSFC {

namespace protobuf_types_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static void InitDefaultsImpl();
  static void Shutdown();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_types_2eproto

// ===================================================================

class State : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:FTSFC.State) */ {
 public:
  State();
  virtual ~State();

  State(const State& from);

  inline State& operator=(const State& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const State& default_instance();

  static inline const State* internal_default_instance() {
    return reinterpret_cast<const State*>(
               &_State_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(State* other);

  // implements Message ----------------------------------------------

  inline State* New() const PROTOBUF_FINAL { return New(NULL); }

  State* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const State& from);
  void MergeFrom(const State& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(State* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated string key = 1;
  int key_size() const;
  void clear_key();
  static const int kKeyFieldNumber = 1;
  const ::std::string& key(int index) const;
  ::std::string* mutable_key(int index);
  void set_key(int index, const ::std::string& value);
  #if LANG_CXX11
  void set_key(int index, ::std::string&& value);
  #endif
  void set_key(int index, const char* value);
  void set_key(int index, const char* value, size_t size);
  ::std::string* add_key();
  void add_key(const ::std::string& value);
  #if LANG_CXX11
  void add_key(::std::string&& value);
  #endif
  void add_key(const char* value);
  void add_key(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& key() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_key();

  // repeated string val = 2;
  int val_size() const;
  void clear_val();
  static const int kValFieldNumber = 2;
  const ::std::string& val(int index) const;
  ::std::string* mutable_val(int index);
  void set_val(int index, const ::std::string& value);
  #if LANG_CXX11
  void set_val(int index, ::std::string&& value);
  #endif
  void set_val(int index, const char* value);
  void set_val(int index, const char* value, size_t size);
  ::std::string* add_val();
  void add_val(const ::std::string& value);
  #if LANG_CXX11
  void add_val(::std::string&& value);
  #endif
  void add_val(const char* value);
  void add_val(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& val() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_val();

  // @@protoc_insertion_point(class_scope:FTSFC.State)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::std::string> key_;
  ::google::protobuf::RepeatedPtrField< ::std::string> val_;
  friend struct protobuf_types_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class TimestampState : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:FTSFC.TimestampState) */ {
 public:
  TimestampState();
  virtual ~TimestampState();

  TimestampState(const TimestampState& from);

  inline TimestampState& operator=(const TimestampState& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TimestampState& default_instance();

  static inline const TimestampState* internal_default_instance() {
    return reinterpret_cast<const TimestampState*>(
               &_TimestampState_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(TimestampState* other);

  // implements Message ----------------------------------------------

  inline TimestampState* New() const PROTOBUF_FINAL { return New(NULL); }

  TimestampState* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const TimestampState& from);
  void MergeFrom(const TimestampState& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(TimestampState* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .FTSFC.State state = 2;
  bool has_state() const;
  void clear_state();
  static const int kStateFieldNumber = 2;
  const ::FTSFC::State& state() const;
  ::FTSFC::State* mutable_state();
  ::FTSFC::State* release_state();
  void set_allocated_state(::FTSFC::State* state);

  // required int64 timestamp = 1;
  bool has_timestamp() const;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 1;
  ::google::protobuf::int64 timestamp() const;
  void set_timestamp(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:FTSFC.TimestampState)
 private:
  void set_has_timestamp();
  void clear_has_timestamp();
  void set_has_state();
  void clear_has_state();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::FTSFC::State* state_;
  ::google::protobuf::int64 timestamp_;
  friend struct protobuf_types_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class PiggyBackState : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:FTSFC.PiggyBackState) */ {
 public:
  PiggyBackState();
  virtual ~PiggyBackState();

  PiggyBackState(const PiggyBackState& from);

  inline PiggyBackState& operator=(const PiggyBackState& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PiggyBackState& default_instance();

  static inline const PiggyBackState* internal_default_instance() {
    return reinterpret_cast<const PiggyBackState*>(
               &_PiggyBackState_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    2;

  void Swap(PiggyBackState* other);

  // implements Message ----------------------------------------------

  inline PiggyBackState* New() const PROTOBUF_FINAL { return New(NULL); }

  PiggyBackState* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const PiggyBackState& from);
  void MergeFrom(const PiggyBackState& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(PiggyBackState* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .FTSFC.State state = 4;
  bool has_state() const;
  void clear_state();
  static const int kStateFieldNumber = 4;
  const ::FTSFC::State& state() const;
  ::FTSFC::State* mutable_state();
  ::FTSFC::State* release_state();
  void set_allocated_state(::FTSFC::State* state);

  // required int64 last_commit = 2;
  bool has_last_commit() const;
  void clear_last_commit();
  static const int kLastCommitFieldNumber = 2;
  ::google::protobuf::int64 last_commit() const;
  void set_last_commit(::google::protobuf::int64 value);

  // required int64 timestamp = 3;
  bool has_timestamp() const;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 3;
  ::google::protobuf::int64 timestamp() const;
  void set_timestamp(::google::protobuf::int64 value);

  // required int32 ack = 1;
  bool has_ack() const;
  void clear_ack();
  static const int kAckFieldNumber = 1;
  ::google::protobuf::int32 ack() const;
  void set_ack(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:FTSFC.PiggyBackState)
 private:
  void set_has_ack();
  void clear_has_ack();
  void set_has_last_commit();
  void clear_has_last_commit();
  void set_has_timestamp();
  void clear_has_timestamp();
  void set_has_state();
  void clear_has_state();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::FTSFC::State* state_;
  ::google::protobuf::int64 last_commit_;
  ::google::protobuf::int64 timestamp_;
  ::google::protobuf::int32 ack_;
  friend struct protobuf_types_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class PiggyBackMessage : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:FTSFC.PiggyBackMessage) */ {
 public:
  PiggyBackMessage();
  virtual ~PiggyBackMessage();

  PiggyBackMessage(const PiggyBackMessage& from);

  inline PiggyBackMessage& operator=(const PiggyBackMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PiggyBackMessage& default_instance();

  static inline const PiggyBackMessage* internal_default_instance() {
    return reinterpret_cast<const PiggyBackMessage*>(
               &_PiggyBackMessage_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    3;

  void Swap(PiggyBackMessage* other);

  // implements Message ----------------------------------------------

  inline PiggyBackMessage* New() const PROTOBUF_FINAL { return New(NULL); }

  PiggyBackMessage* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const PiggyBackMessage& from);
  void MergeFrom(const PiggyBackMessage& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(PiggyBackMessage* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated int32 mb_id = 1;
  int mb_id_size() const;
  void clear_mb_id();
  static const int kMbIdFieldNumber = 1;
  ::google::protobuf::int32 mb_id(int index) const;
  void set_mb_id(int index, ::google::protobuf::int32 value);
  void add_mb_id(::google::protobuf::int32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      mb_id() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_mb_id();

  // repeated .FTSFC.PiggyBackState pb_state = 2;
  int pb_state_size() const;
  void clear_pb_state();
  static const int kPbStateFieldNumber = 2;
  const ::FTSFC::PiggyBackState& pb_state(int index) const;
  ::FTSFC::PiggyBackState* mutable_pb_state(int index);
  ::FTSFC::PiggyBackState* add_pb_state();
  ::google::protobuf::RepeatedPtrField< ::FTSFC::PiggyBackState >*
      mutable_pb_state();
  const ::google::protobuf::RepeatedPtrField< ::FTSFC::PiggyBackState >&
      pb_state() const;

  // @@protoc_insertion_point(class_scope:FTSFC.PiggyBackMessage)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > mb_id_;
  ::google::protobuf::RepeatedPtrField< ::FTSFC::PiggyBackState > pb_state_;
  friend struct protobuf_types_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// State

// repeated string key = 1;
inline int State::key_size() const {
  return key_.size();
}
inline void State::clear_key() {
  key_.Clear();
}
inline const ::std::string& State::key(int index) const {
  // @@protoc_insertion_point(field_get:FTSFC.State.key)
  return key_.Get(index);
}
inline ::std::string* State::mutable_key(int index) {
  // @@protoc_insertion_point(field_mutable:FTSFC.State.key)
  return key_.Mutable(index);
}
inline void State::set_key(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:FTSFC.State.key)
  key_.Mutable(index)->assign(value);
}
#if LANG_CXX11
inline void State::set_key(int index, ::std::string&& value) {
  // @@protoc_insertion_point(field_set:FTSFC.State.key)
  key_.Mutable(index)->assign(std::move(value));
}
#endif
inline void State::set_key(int index, const char* value) {
  GOOGLE_DCHECK(value != NULL);
  key_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:FTSFC.State.key)
}
inline void State::set_key(int index, const char* value, size_t size) {
  key_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:FTSFC.State.key)
}
inline ::std::string* State::add_key() {
  // @@protoc_insertion_point(field_add_mutable:FTSFC.State.key)
  return key_.Add();
}
inline void State::add_key(const ::std::string& value) {
  key_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:FTSFC.State.key)
}
#if LANG_CXX11
inline void State::add_key(::std::string&& value) {
  key_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:FTSFC.State.key)
}
#endif
inline void State::add_key(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  key_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:FTSFC.State.key)
}
inline void State::add_key(const char* value, size_t size) {
  key_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:FTSFC.State.key)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
State::key() const {
  // @@protoc_insertion_point(field_list:FTSFC.State.key)
  return key_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
State::mutable_key() {
  // @@protoc_insertion_point(field_mutable_list:FTSFC.State.key)
  return &key_;
}

// repeated string val = 2;
inline int State::val_size() const {
  return val_.size();
}
inline void State::clear_val() {
  val_.Clear();
}
inline const ::std::string& State::val(int index) const {
  // @@protoc_insertion_point(field_get:FTSFC.State.val)
  return val_.Get(index);
}
inline ::std::string* State::mutable_val(int index) {
  // @@protoc_insertion_point(field_mutable:FTSFC.State.val)
  return val_.Mutable(index);
}
inline void State::set_val(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:FTSFC.State.val)
  val_.Mutable(index)->assign(value);
}
#if LANG_CXX11
inline void State::set_val(int index, ::std::string&& value) {
  // @@protoc_insertion_point(field_set:FTSFC.State.val)
  val_.Mutable(index)->assign(std::move(value));
}
#endif
inline void State::set_val(int index, const char* value) {
  GOOGLE_DCHECK(value != NULL);
  val_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:FTSFC.State.val)
}
inline void State::set_val(int index, const char* value, size_t size) {
  val_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:FTSFC.State.val)
}
inline ::std::string* State::add_val() {
  // @@protoc_insertion_point(field_add_mutable:FTSFC.State.val)
  return val_.Add();
}
inline void State::add_val(const ::std::string& value) {
  val_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:FTSFC.State.val)
}
#if LANG_CXX11
inline void State::add_val(::std::string&& value) {
  val_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:FTSFC.State.val)
}
#endif
inline void State::add_val(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  val_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:FTSFC.State.val)
}
inline void State::add_val(const char* value, size_t size) {
  val_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:FTSFC.State.val)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
State::val() const {
  // @@protoc_insertion_point(field_list:FTSFC.State.val)
  return val_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
State::mutable_val() {
  // @@protoc_insertion_point(field_mutable_list:FTSFC.State.val)
  return &val_;
}

// -------------------------------------------------------------------

// TimestampState

// required int64 timestamp = 1;
inline bool TimestampState::has_timestamp() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void TimestampState::set_has_timestamp() {
  _has_bits_[0] |= 0x00000002u;
}
inline void TimestampState::clear_has_timestamp() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void TimestampState::clear_timestamp() {
  timestamp_ = GOOGLE_LONGLONG(0);
  clear_has_timestamp();
}
inline ::google::protobuf::int64 TimestampState::timestamp() const {
  // @@protoc_insertion_point(field_get:FTSFC.TimestampState.timestamp)
  return timestamp_;
}
inline void TimestampState::set_timestamp(::google::protobuf::int64 value) {
  set_has_timestamp();
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:FTSFC.TimestampState.timestamp)
}

// optional .FTSFC.State state = 2;
inline bool TimestampState::has_state() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TimestampState::set_has_state() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TimestampState::clear_has_state() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TimestampState::clear_state() {
  if (state_ != NULL) state_->::FTSFC::State::Clear();
  clear_has_state();
}
inline const ::FTSFC::State& TimestampState::state() const {
  // @@protoc_insertion_point(field_get:FTSFC.TimestampState.state)
  return state_ != NULL ? *state_
                         : *::FTSFC::State::internal_default_instance();
}
inline ::FTSFC::State* TimestampState::mutable_state() {
  set_has_state();
  if (state_ == NULL) {
    state_ = new ::FTSFC::State;
  }
  // @@protoc_insertion_point(field_mutable:FTSFC.TimestampState.state)
  return state_;
}
inline ::FTSFC::State* TimestampState::release_state() {
  // @@protoc_insertion_point(field_release:FTSFC.TimestampState.state)
  clear_has_state();
  ::FTSFC::State* temp = state_;
  state_ = NULL;
  return temp;
}
inline void TimestampState::set_allocated_state(::FTSFC::State* state) {
  delete state_;
  state_ = state;
  if (state) {
    set_has_state();
  } else {
    clear_has_state();
  }
  // @@protoc_insertion_point(field_set_allocated:FTSFC.TimestampState.state)
}

// -------------------------------------------------------------------

// PiggyBackState

// required int32 ack = 1;
inline bool PiggyBackState::has_ack() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void PiggyBackState::set_has_ack() {
  _has_bits_[0] |= 0x00000008u;
}
inline void PiggyBackState::clear_has_ack() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void PiggyBackState::clear_ack() {
  ack_ = 0;
  clear_has_ack();
}
inline ::google::protobuf::int32 PiggyBackState::ack() const {
  // @@protoc_insertion_point(field_get:FTSFC.PiggyBackState.ack)
  return ack_;
}
inline void PiggyBackState::set_ack(::google::protobuf::int32 value) {
  set_has_ack();
  ack_ = value;
  // @@protoc_insertion_point(field_set:FTSFC.PiggyBackState.ack)
}

// required int64 last_commit = 2;
inline bool PiggyBackState::has_last_commit() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PiggyBackState::set_has_last_commit() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PiggyBackState::clear_has_last_commit() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PiggyBackState::clear_last_commit() {
  last_commit_ = GOOGLE_LONGLONG(0);
  clear_has_last_commit();
}
inline ::google::protobuf::int64 PiggyBackState::last_commit() const {
  // @@protoc_insertion_point(field_get:FTSFC.PiggyBackState.last_commit)
  return last_commit_;
}
inline void PiggyBackState::set_last_commit(::google::protobuf::int64 value) {
  set_has_last_commit();
  last_commit_ = value;
  // @@protoc_insertion_point(field_set:FTSFC.PiggyBackState.last_commit)
}

// required int64 timestamp = 3;
inline bool PiggyBackState::has_timestamp() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PiggyBackState::set_has_timestamp() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PiggyBackState::clear_has_timestamp() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PiggyBackState::clear_timestamp() {
  timestamp_ = GOOGLE_LONGLONG(0);
  clear_has_timestamp();
}
inline ::google::protobuf::int64 PiggyBackState::timestamp() const {
  // @@protoc_insertion_point(field_get:FTSFC.PiggyBackState.timestamp)
  return timestamp_;
}
inline void PiggyBackState::set_timestamp(::google::protobuf::int64 value) {
  set_has_timestamp();
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:FTSFC.PiggyBackState.timestamp)
}

// optional .FTSFC.State state = 4;
inline bool PiggyBackState::has_state() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PiggyBackState::set_has_state() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PiggyBackState::clear_has_state() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PiggyBackState::clear_state() {
  if (state_ != NULL) state_->::FTSFC::State::Clear();
  clear_has_state();
}
inline const ::FTSFC::State& PiggyBackState::state() const {
  // @@protoc_insertion_point(field_get:FTSFC.PiggyBackState.state)
  return state_ != NULL ? *state_
                         : *::FTSFC::State::internal_default_instance();
}
inline ::FTSFC::State* PiggyBackState::mutable_state() {
  set_has_state();
  if (state_ == NULL) {
    state_ = new ::FTSFC::State;
  }
  // @@protoc_insertion_point(field_mutable:FTSFC.PiggyBackState.state)
  return state_;
}
inline ::FTSFC::State* PiggyBackState::release_state() {
  // @@protoc_insertion_point(field_release:FTSFC.PiggyBackState.state)
  clear_has_state();
  ::FTSFC::State* temp = state_;
  state_ = NULL;
  return temp;
}
inline void PiggyBackState::set_allocated_state(::FTSFC::State* state) {
  delete state_;
  state_ = state;
  if (state) {
    set_has_state();
  } else {
    clear_has_state();
  }
  // @@protoc_insertion_point(field_set_allocated:FTSFC.PiggyBackState.state)
}

// -------------------------------------------------------------------

// PiggyBackMessage

// repeated int32 mb_id = 1;
inline int PiggyBackMessage::mb_id_size() const {
  return mb_id_.size();
}
inline void PiggyBackMessage::clear_mb_id() {
  mb_id_.Clear();
}
inline ::google::protobuf::int32 PiggyBackMessage::mb_id(int index) const {
  // @@protoc_insertion_point(field_get:FTSFC.PiggyBackMessage.mb_id)
  return mb_id_.Get(index);
}
inline void PiggyBackMessage::set_mb_id(int index, ::google::protobuf::int32 value) {
  mb_id_.Set(index, value);
  // @@protoc_insertion_point(field_set:FTSFC.PiggyBackMessage.mb_id)
}
inline void PiggyBackMessage::add_mb_id(::google::protobuf::int32 value) {
  mb_id_.Add(value);
  // @@protoc_insertion_point(field_add:FTSFC.PiggyBackMessage.mb_id)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
PiggyBackMessage::mb_id() const {
  // @@protoc_insertion_point(field_list:FTSFC.PiggyBackMessage.mb_id)
  return mb_id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
PiggyBackMessage::mutable_mb_id() {
  // @@protoc_insertion_point(field_mutable_list:FTSFC.PiggyBackMessage.mb_id)
  return &mb_id_;
}

// repeated .FTSFC.PiggyBackState pb_state = 2;
inline int PiggyBackMessage::pb_state_size() const {
  return pb_state_.size();
}
inline void PiggyBackMessage::clear_pb_state() {
  pb_state_.Clear();
}
inline const ::FTSFC::PiggyBackState& PiggyBackMessage::pb_state(int index) const {
  // @@protoc_insertion_point(field_get:FTSFC.PiggyBackMessage.pb_state)
  return pb_state_.Get(index);
}
inline ::FTSFC::PiggyBackState* PiggyBackMessage::mutable_pb_state(int index) {
  // @@protoc_insertion_point(field_mutable:FTSFC.PiggyBackMessage.pb_state)
  return pb_state_.Mutable(index);
}
inline ::FTSFC::PiggyBackState* PiggyBackMessage::add_pb_state() {
  // @@protoc_insertion_point(field_add:FTSFC.PiggyBackMessage.pb_state)
  return pb_state_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::FTSFC::PiggyBackState >*
PiggyBackMessage::mutable_pb_state() {
  // @@protoc_insertion_point(field_mutable_list:FTSFC.PiggyBackMessage.pb_state)
  return &pb_state_;
}
inline const ::google::protobuf::RepeatedPtrField< ::FTSFC::PiggyBackState >&
PiggyBackMessage::pb_state() const {
  // @@protoc_insertion_point(field_list:FTSFC.PiggyBackMessage.pb_state)
  return pb_state_;
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


}  // namespace FTSFC

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_types_2eproto__INCLUDED
