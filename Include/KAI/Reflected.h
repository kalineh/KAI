// (C) 2014 christian.schladetsch@gmal.com

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_REFLECTED_H
	#define KAI_REFLECTED_H

KAI_BEGIN

/// A Reflected type has a Self pointer. This is semantically same as C++'s 'this' pointer,
/// but using the Object Model of the system
///
/// TODO: there doesn't seem to be any point to passing through the reflected type
template <class T>
struct Reflected : ReflectedBase
{
	Registry &Reg() const
	{
		return *Self->GetRegistry();
	}

	template <class U>
	Pointer<U> New() const
	{
		return Reg().New<U>();
	}

	template <class U>
	Pointer<U> NewRetained() const
	{
		return Reg().NewRetained<U>();
	}

	template <class U>
	Pointer<U> New(const U &X) const
	{
		return Reg().New<U>(X);
	}

	template <class U>
	Storage<U> *NewStorage() const
	{
		return Reg().NewStorage<U>();
	}

	Object NewFromTypeNumber(Type::Number type_number)
	{
		return Self->NewFromTypeNumber(type_number);
	}

	Object NewFromClassName(String const &type_name) const
	{
		return Self->NewFromClassName(type_name.c_str());
	}
};

KAI_END

#endif // KAI_REFLECTED_H

//EOF
