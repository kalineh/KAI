
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BUILTIN_TYPES_BOOL_H
#	define KAI_BUILTIN_TYPES_BOOL_H

KAI_BEGIN

StringStream &operator<<(StringStream &, bool);
StringStream &operator>>(StringStream &, bool &);
BinaryStream &operator<<(BinaryStream &, bool);
BinaryPacket &operator>>(BinaryPacket &, bool &);

KAI_TYPE_TRAITS(bool, Type::Number::Bool
	, Type::Properties::Streaming 
	| Type::Properties::Assign
	| Type::Properties::Relational);

KAI_END

#include KAI_TEST(X)

#endif // KAI_BUILTIN_TYPES_BOOL_H

//EOF

