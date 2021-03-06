#pragma once

#ifndef KAI_OBJECT_H
#define KAI_OBJECT_H

#include "KAI/BuiltinTypes/Dictionary.h"

KAI_BEGIN

#undef GetObject

template <class T>
struct Pointer;

struct Object
{
private:
	const ClassBase *class_base;
	Registry *registry;
	Handle handle;

#ifdef KAI_CACHE_OBJECT_LOOKUPS
	// these fields are used to cache results for speed
	int gcindex;
	bool valid;
	void *value;
#endif

public:
	enum Switch 
	{ 
		Marked = 1, 
		Managed = 2, 
		Const = 4, 
		Clean = 8, 
		// if set, when marking, will not mark children
		NoRecurse = 16,
		DefaultSwitches = Managed
	};

	Object() : registry(0), class_base(0) { }
	Object(Object const &);
	Object(ObjectConstructParams const &P);

	Object &operator=(Object const &);

	operator bool() const { return Exists(); }

	StorageBase &GetStorageBase() const;
	int GetSwitches() const;

	ObjectColor::Color GetColor() const;
	void SetColor(ObjectColor::Color C) const;
	void SetColorRecursive(ObjectColor::Color C) const;
	void SetColorRecursive(ObjectColor::Color C, boost::unordered_set/*nstd::vector*/<Handle> &) const;
	bool IsWhite() const { return GetColor() == ObjectColor::White; }
	bool IsGrey() const { return GetColor() == ObjectColor::Grey; }
	bool IsBlack() const { return GetColor() == ObjectColor::Black; }
	void SetWhite() const { SetColor(ObjectColor::White); }
	void SetGrey() const { SetColor(ObjectColor::Grey); }
	void SetBlack() const { SetColor(ObjectColor::Black); }

	Type::Number GetTypeNumber() const;
	const ClassBase *GetClass() const { return class_base; }
	Registry *GetRegistry() const { return registry; }

	Object Duplicate() const;
	Object Clone() const { return Duplicate(); }

	bool IsTypeNumber(Type::Number N) const 
	{ 
		if (!Exists())
			return N == Type::Number::None;

		return GetTypeNumber() == N;
	}

	Handle GetParentHandle() const;
	void SetParentHandle(Handle);
	Handle GetHandle() const { return handle; }
	Object GetParent() const;

	template <class T>
	bool IsType() const 
	{
		return Exists() && GetClass()->GetTypeNumber() == Type::Traits<T>::Number; 
	}

	void Delete() const;
	bool Valid() const;
	bool Exists() const;
	bool OnDeathRow() const;

	bool IsConst() const;
	bool IsManaged() const;
	bool IsMarked() const;
	bool IsClean() const;

	void SetSwitch(int, bool) const;
	void SetSwitches(int) const;

	void SetMarked(bool = true) const;
	void SetConst() const;
	void SetManaged(bool = true) const;
	void SetClean(bool = true) const;

	bool IsMutable() const { return !IsConst(); }
	bool IsUnmanaged() const { return !IsManaged(); }
	bool IsUnmarked() const { return !IsMarked(); }
	bool IsDirty() const { return !IsClean(); }

	// use these to get/set/remove a property or a child of a given name
	void Set(const Label &, const Object &) const;
	Object Get(const Label &) const;
	bool Has(const Label &) const;
	void Remove(const Label &) const;

	template <class T>
	void SetValue(const Label &L, const T &V) const
	{
		if (!Valid())
			KAI_THROW_0(NullObject);

		if (HasProperty(L))
			SetProperty<T>(L, V);
		else if (HasChild(L))
			Deref<T>(GetChild(L)) = V;
		else
			SetChild(L, New<T>(V));
	}

	template <class T>
	T const &GetValue(const Label &L) const
	{
		if (!Valid())
			KAI_THROW_0(NullObject);

		if (HasProperty(L))
			return GetProperty<T>(L);

		if (HasChild(L))
			return ConstDeref<T>(GetChild(L));

		KAI_THROW_1(UnknownProperty, L);
	}

	template <class T>
	T &GetValue(const Label &L)
	{
		if (!Valid())
			KAI_THROW_0(NullObject);

		if (HasProperty(L))
			return GetProperty<T>(L);

		if (HasChild(L))
			return Deref<T>(GetChild(L));

		KAI_THROW_2(UnknownProperty, GetClass()->GetLabel(), L);
	}

	void Detach(const Label &L) const { Remove(L); }
	void Detach(const Object &Q) const;
	Dictionary const &GetDictionary() const;

	void SetChild(const Label &L, const Object &Q) const { Set(L, Q); }
	Object GetChild(const Label &L) const { return Get(L); }
	void RemoveChild(const Label &L) const { Remove(L); }
	void DetachChild(const Label &L) const { Remove(L); }
	void DetachChild(const Object &Q) const { Detach(Q); }
	bool HasChild(const Label &L) const { return Has(L); }

	Label GetLabel() const;

	String ToString() const;
	String ToXmlString() const;

	template <class T>
	Object New() const
	{
		if (!registry)
			return Object();
		return registry->New<T>();
	}
	template <class T>
	Object New(const T &X) const
	{
		if (!registry)
			return Object();
		return registry->New<T>(X);
	}

	// KAI TODO delete the abomination that is 'retained object'
	template <class T>
	Object NewRetained() const
	{
		if (!registry)
			return Object();
		return registry->NewRetained<T>();
	}

	template <class T>
	Object NewRetained(const T &X) const
	{
		if (!registry)
			return Object();
		return registry->NewRetained<T>(X);
	}

	template <class T>
	T const &GetAttribute(Label const &label) const
	{
		return ConstDeref<T>(Get(label));
	}

	template <class T>
	T &GetAttribute(Label const &label)
	{
		return Deref<T>(Get(label));
	}

	bool HasAttribute(Label const &label)
	{
		return Get(label).Exists();
	}

	template <class T>
	void SetAttribute(Label const &label, T const &value)
	{
		Deref<T>(Get(label)) = value;
	}

	template <class T>
	T const &GetChildValue(Label const &label)
	{
		return ConstDeref<T>(GetChild(label));
	}

	template <class T>
	void SetChildValue(Label const &label, T const &value)
	{
		Deref<T>(GetChild(label)) = value;
	}

	Object NewFromTypeNumber(Type::Number type_number) const;
	Object NewFromClassName(String const &type_name) const;

	void SetPropertyValue(Label const &, Object const &) const;
	Object GetPropertyValue(Label const &) const;

	void SetPropertyObject(Label const &, Object const &) const;
	Object GetPropertyObject(Label const &) const;

	bool HasProperty(Label const &name) const;

	template <class T>
	T const &GetProperty(Label const &L) const
	{
		return ConstDeref<T>(GetPropertyValue(L));
	}
	template <class T>
	T &GetProperty(Label const &L)
	{
		return Deref<T>(GetPropertyValue(L));
	}

	template <class T>
	void SetProperty(Label const &name, T const &value) const
	{ 
		SetPropertyValue(name, New(value)); 
	}

	/// detach from parent
	void Detach();

	static void Register(Registry &);

	/// detach from container
	void RemovedFromContainer(Object const &container) const;
	
	/// attach to container
	void AddedToContainer(Object const &container) const;

	StorageBase *GetBasePtr() const;
	StorageBase *GetParentBasePtr() const;

	typedef nstd::list<Object> ObjectList;
	void GetPropertyObjects(ObjectList &contained) const;
	void GetContainedObjects(ObjectList &contained) const;
	void GetChildObjects(ObjectList &contained) const;
	void GetAllReferencedObjects(ObjectList &contained) const;

	class ChildProxy
	{
		friend struct Object;
		Registry *registry;
		Handle handle;
		Label label;
		Constness konst;
		ChildProxy(Object const &Q, const char *);
		ChildProxy(Object const &Q, Label const &L);
		Object GetObject() const;
	public:
		template <class T>
		ChildProxy &operator=(T const &value)
		{
			GetObject().SetValue(label, value);
			return *this;
		}

		template <class T>
		ChildProxy &operator=(Pointer<T> const &value)
		{
			GetObject().Set(label, value);
			return *this;
		}
		ChildProxy &operator=(Object const &child)
		{
			GetObject().Set(label, child);
			return *this;
		}
		operator Object() const
		{
			return GetObject().Get(label);
		}
		operator bool() const
		{
			return GetObject();
		}
	};

	ChildProxy operator[](const char *L) 
	{ 
		return ChildProxy(*this, L);
	}
	ChildProxy operator[](Label const &L)
	{
		return ChildProxy(*this, L);
	}

protected:
	Dictionary &GetDictionaryRef();
};

StringStream &operator<<(StringStream &S, Object const &Q);
StringStream &operator>>(StringStream &S, Object &Q);
BinaryStream &operator<<(BinaryStream &S, Object const &Q);
BinaryPacket &operator>>(BinaryPacket &S, Object &Q);

Object operator*(Object const &A);
bool operator<(Object const &A, Object const &B);
bool operator==(Object const &A, Object const &B);
inline bool operator!=(Object const &A, Object const &B) { return !(A == B); }
bool operator>(Object const &A, Object const &B);

Object operator+(Object const &A, Object const &B);
Object operator-(Object const &A, Object const &B);
Object operator*(Object const &A, Object const &B);
Object operator/(Object const &A, Object const &B);

Object Absolute(Object const &A);

KAI_TYPE_TRAITS(Object, Type::Number::Object
	, Type::Properties::StringStreamInsert 
	| Type::Properties::BinaryStreamInsert 
	| Type::Properties::BinaryStreamExtract);

/// hash function which can be overridden for any type.
/// default to using the type number.
template <class T>
HashValue GetHash(const T &)
{
	return Type::Traits<T>::Number;
}

HashValue GetHash(Object const  &);

void MarkObject(Object const &, bool = true);
void MarkObjectAndChildren(Object const &, bool = true);

void MarkObject(StorageBase &, bool = true);
void MarkObjectAndChildren(StorageBase &, bool = true);

Object Duplicate(Object const &);
Object Duplicate_(Object const &);

template <class T>
T const &GetDefaultValue(Object const &object, Label const &name, T const &default_value)
{
	if (!object)
		return default_value;
	Pointer<const T> override = object.Get(name);
	return override ? *override : default_value;
}

KAI_END

namespace boost
{
	inline size_t hash_value(KAI_NAMESPACE(Object) const &H)
	{
		return H.GetHandle().GetValue();
	}
}

#endif 

