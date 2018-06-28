// Copyright 2018 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TYPES_BASE_H
#define TYPES_BASE_H

#include <fastrtps/rtps/common/Types.h>
#include <string>
#include <map>
#include <vector>
#include <cctype>

namespace eprosima{
namespace fastrtps{

using namespace rtps;

namespace types{

// ---------- Equivalence Kinds ------------------
typedef octet EquivalenceKind;
const octet EK_MINIMAL   = 0xF1; // 0x1111 0001
const octet EK_COMPLETE  = 0xF2; // 0x1111 0010
const octet EK_BOTH      = 0xF3; // 0x1111 0011

// ---------- TypeKinds (begin) ------------------
typedef octet TypeKind;        // Primitive TKs

const octet TK_NONE       = 0x00;
const octet TK_BOOLEAN    = 0x01;
const octet TK_BYTE       = 0x02;
const octet TK_INT16      = 0x03;
const octet TK_INT32      = 0x04;
const octet TK_INT64      = 0x05;
const octet TK_UINT16     = 0x06;
const octet TK_UINT32     = 0x07;
const octet TK_UINT64     = 0x08;
const octet TK_FLOAT32    = 0x09;
const octet TK_FLOAT64    = 0x0A;
const octet TK_FLOAT128   = 0x0B;
const octet TK_CHAR8      = 0x10;
const octet TK_CHAR16     = 0x11;

// String TKs
const octet TK_STRING8    = 0x20;
const octet TK_STRING16   = 0x21;


// Constructed/Named types
const octet TK_ALIAS      = 0x30;

// Enumerated TKs
const octet TK_ENUM       = 0x40;
const octet TK_BITMASK    = 0x41;

// Structured TKs
const octet TK_ANNOTATION = 0x50;
const octet TK_STRUCTURE  = 0x51;
const octet TK_UNION      = 0x52;
const octet TK_BITSET     = 0x53;

// Collection TKs
const octet TK_SEQUENCE   = 0x60;
const octet TK_ARRAY      = 0x61;
const octet TK_MAP        = 0x62;

// ---------- TypeKinds (end) ------------------

// The name of some element (e.g. type, type member, module)
// Valid characters are alphanumeric plus the "_" cannot start with digit

const long MEMBER_NAME_MAX_LENGTH = 256;
typedef std::string MemberName;

// Qualified type name includes the name of containing modules
// using "::" as separator. No leading "::". E.g. "MyModule::MyType"
const long TYPE_NAME_MAX_LENGTH = 256;
typedef std::string QualifiedTypeName;

// Every type has an ID. Those of the primitive types are pre-defined.
typedef octet PrimitiveTypeId;

// First 4 bytes of MD5 of of a member name converted to bytes
// using UTF-8 encoding and without a 'nul' terminator.
// Example: the member name "color" has NameHash {0x70, 0xDD, 0xA5, 0xDF}
typedef std::array<uint8_t, 4> NameHash;

// Mask used to remove the flags that do no affect assignability
// Selects  T1, T2, O, M, K, D
const unsigned short MemberFlagMinimalMask = 0x003f;

/*!
 * @brief This class represents the enumeration ResponseCode.
 */
enum ResponseCode : uint32_t
{
    RETCODE_ERROR = (uint32_t)(-1),
    RETCODE_OK = 0,
    RETCODE_BAD_PARAMETER,
    RETCODE_PRECONDITION_NOT_MET,
    RETCODE_ALREADY_DELETED
};

#define LENGTH_UNLIMITED    0

class TypeSupport
{
public:
    //int register_type(DomainParticipant domain, std::string type_name);
    //std::string get_type_name();
    //DynamicType get_type();
};

class DynamicTypeSupport : public TypeSupport
{
public:
    //static DynamicTypeSupport create_type_support(DynamicType type);
    //static ResponseCode delete_type_support(DynamicTypeSupport type_support);

    //ResponseCode register_type(DomainParticipant participant, std::string type_name);
    //std::string get_type_name();
    //DynamicType get_type();
};

// Long Bound of a collection type
typedef unsigned long LBound;
typedef std::vector<LBound> LBoundSeq;
const LBound INVALID_LBOUND = 0;

// Short Bound of a collection type
typedef octet SBound;
typedef std::vector<SBound> SBoundSeq;
const SBound INVALID_SBOUND = 0;

// Flags that apply to struct/union/collection/enum/bitmask/bitset
// members/elements and DO affect type assignability
// Depending on the flag it may not apply to members of all types

// When not all, the applicable member types are listed
class MemberFlag
{
private:
	std::bitset<16> m_MemberFlag;
public:
	// T1 | 00 = INVALID, 01 = DISCARD
	bool TRY_CONSTRUCT1() const { return m_MemberFlag.test(0); }
	void TRY_CONSTRUCT1(bool b) { b ? m_MemberFlag.set(0) : m_MemberFlag.reset(0); }
	// T2 | 10 = USE_DEFAULT, 11 = TRIM
	bool TRY_CONSTRUCT2() const { return m_MemberFlag.test(1); }
	void TRY_CONSTRUCT2(bool b) { b ? m_MemberFlag.set(1) : m_MemberFlag.reset(1); }
	// X  StructMember, UnionMember,
	//    CollectionElement
	bool IS_EXTERNAL() const { return m_MemberFlag.test(2); }
	void IS_EXTERNAL(bool b) { b ? m_MemberFlag.set(2) : m_MemberFlag.reset(2); }
	// O  StructMember
	bool IS_OPTIONAL() const { return m_MemberFlag.test(3); }
	void IS_OPTIONAL(bool b) { b ? m_MemberFlag.set(3) : m_MemberFlag.reset(3); }
	// M  StructMember
	bool IS_MUST_UNDERSTAND() const { return m_MemberFlag.test(4); }
	void IS_MUST_UNDERSTAND(bool b) { b ? m_MemberFlag.set(4) : m_MemberFlag.reset(4); }
	// K  StructMember, UnionDiscriminator
	bool IS_KEY() const { return m_MemberFlag.test(5); }
	void IS_KEY(bool b) { b ? m_MemberFlag.set(5) : m_MemberFlag.reset(5); }
	// D  UnionMember, EnumerationLiteral
	bool IS_DEFAULT() const { return m_MemberFlag.test(6); }
	void IS_DEFAULT(bool b) { b ? m_MemberFlag.set(6) : m_MemberFlag.reset(6); }
};

typedef MemberFlag CollectionElementFlag;   // T1, T2, X
typedef MemberFlag StructMemberFlag;        // T1, T2, O, M, K, X
typedef MemberFlag UnionMemberFlag;         // T1, T2, D, X
typedef MemberFlag UnionDiscriminatorFlag;  // T1, T2, K
typedef MemberFlag EnumeratedLiteralFlag;   // D
typedef MemberFlag AnnotationParameterFlag; // Unused. No flags apply
typedef MemberFlag AliasMemberFlag;         // Unused. No flags apply
typedef MemberFlag BitflagFlag;             // Unused. No flags apply
typedef MemberFlag BitsetMemberFlag;        // Unused. No flags apply

// Flags that apply to type declarationa and DO affect assignability
// Depending on the flag it may not apply to all types
// When not all, the applicable  types are listed
class TypeFlag
{
private:
	std::bitset<16> m_TypeFlag;
public:
	// F |
	bool IS_FINAL() const { return m_TypeFlag.test(0); }
	void IS_FINAL(bool b) { b ? m_TypeFlag.set(0) : m_TypeFlag.reset(0); }
	// A |-  Struct, Union
	bool IS_APPENDABLE() const { return m_TypeFlag.test(1); }
	void IS_APPENDABLE(bool b) { b ? m_TypeFlag.set(1) : m_TypeFlag.reset(1); }
	// M |   (exactly one flag)
	bool IS_MUTABLE() const { return m_TypeFlag.test(2); }
	void IS_MUTABLE(bool b) { b ? m_TypeFlag.set(2) : m_TypeFlag.reset(2); }
	// N     Struct, Union
	bool IS_NESTED() const { return m_TypeFlag.test(3); }
	void IS_NESTED(bool b) { b ? m_TypeFlag.set(3) : m_TypeFlag.reset(3); }
	// H     Struct
	bool IS_AUTOID_HASH() const { return m_TypeFlag.test(4); }
	void IS_AUTOID_HASH(bool b) { b ? m_TypeFlag.set(4) : m_TypeFlag.reset(4); }
};

typedef TypeFlag   StructTypeFlag;      // All flags apply
typedef TypeFlag   UnionTypeFlag;       // All flags apply
typedef TypeFlag   CollectionTypeFlag;  // Unused. No flags apply
typedef TypeFlag   AnnotationTypeFlag;  // Unused. No flags apply
typedef TypeFlag   AliasTypeFlag;       // Unused. No flags apply
typedef TypeFlag   EnumTypeFlag;        // Unused. No flags apply
typedef TypeFlag   BitmaskTypeFlag;     // Unused. No flags apply
typedef TypeFlag   BitsetTypeFlag;      // Unused. No flags apply

// Mask used to remove the flags that do no affect assignability
const unsigned short TypeFlagMinimalMask = 0x0007; // Selects  M, A, F

// --- Annotation usage: ----------------------------------------------

// ID of a type member
typedef unsigned long MemberId;
const unsigned long ANNOTATION_STR_VALUE_MAX_LEN = 128;
const unsigned long ANNOTATION_OCTETSEC_VALUE_MAX_LEN = 128;

} // namespace types
} // namespace fastrtps
} // namespace eprosima

#endif // TYPES_BASE_H


