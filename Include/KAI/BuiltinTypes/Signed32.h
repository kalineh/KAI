
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BUILTIN_TYPES_SIGNED32_H
#	define KAI_BUILTIN_TYPES_SIGNED32_H

KAI_BEGIN

StringStream &operator<<(StringStream &S, nstd::size_t N);
StringStream &operator<<(StringStream &S, int N);
StringStream &operator>>(StringStream &S, int &N);
BinaryStream &operator<<(BinaryStream &S, int N);
BinaryPacket &operator>>(BinaryPacket &S, int &N);

inline HashValue GetHash(int N) { return N; }

KAI_TYPE_TRAITS(int, Type::Number::Signed32
	, Type::Properties::Arithmetic 
	| Type::Properties::Streaming 
	| Type::Properties::Assign
	| Type::Properties::Relational
	| Type::Properties::Absolute
	);

KAI_END

#endif // KAI_BUILTIN_TYPES_SIGNED32_H

//EOF
