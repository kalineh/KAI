
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_CLASS_BASE_H
#	define KAI_CLASS_BASE_H

KAI_BEGIN

/// Base for all Class<T> types. ClassBase defines the type-independent 
/// interface that all Class<T>s must define
class ClassBase
{
public:
#ifndef DEBUG
	typedef boost::unordered_map<Label, MethodBase *> Methods;
	typedef boost::unordered_map<Label, PropertyBase *> Properties;
#else
	typedef std::map<Label, MethodBase *> Methods;
	typedef std::map<Label, PropertyBase *> Properties;
#endif

protected:
	Type::Number type_number;
	Properties properties;
	Methods methods;
	Label name;

public:
	ClassBase(Label const &N, Type::Number T) : name(N), type_number(T) { }
	virtual ~ClassBase();

	/// identification
	const Label &GetName() const { return name; }
	const Label &GetLabel() const { return GetName(); }
	Type::Number GetTypeNumber() const { return type_number; }

	/// tri-color marking: set reference objects color
	virtual void SetReferencedObjectsColor(StorageBase &base, ObjectColor::Color color, StorageBase::Handles& handles) const
	{
		if (properties.empty())
			return;
		ClassBase::Properties::const_iterator iter = properties.begin(), end = properties.end();
		for (; iter != end; ++iter)
		{
			PropertyBase const &prop = *iter->second;
			if (!prop.IsSystemType())
				continue;
			Object property = prop.GetObject(base);
			if (!property.Exists())
				continue;
			property.SetColorRecursive(color, handles);
		}
	}

	virtual void MakeReachableGrey(StorageBase &base) const = 0;

	typedef nstd::list<Object> ObjectList;
	
	// get all objects which are contained by this one
	virtual void GetContainedObjects(StorageBase &object, ObjectList &contained) const = 0;

	// get all property objects; these are C++ fields exposed to the class
	void GetPropertyObjects(StorageBase &object, ObjectList &contained) const;

	/// methods
	void AddMethod(const Label &L, MethodBase *M) { methods[L] = M; }
	const Methods &GetMethods() const { return methods; }
	MethodBase *GetMethod(const Label &L) const
	{
		Methods::const_iterator A = methods.find(L);
		return A == methods.end() ? 0 : A->second;
	}

	/// properties
	void AddProperty(Label const &L, PropertyBase *P) { properties[L] = P; }
	bool HasProperty(Label const &name) const
	{
		return properties.find(name) != properties.end();
	}
	Properties const &GetProperties() const { return properties; }
	PropertyBase const &GetProperty(Label const &L) const
	{
		Properties::const_iterator A = properties.find(L);
		if (A == properties.end())
			KAI_THROW_2(UnknownProperty, GetName(), L);
		return *A->second;
	}
	virtual void CreateProperties(StorageBase &object) const = 0;
	virtual Object Duplicate(StorageBase const &) const = 0;

	virtual void DetachFromContainer(StorageBase &, Object const &) const = 0;

	// compile-time traits
	virtual int GetTraitsProperties() const = 0;
	virtual bool HasTraitsProperty(int N) const = 0;
	bool HasOperation(int N) const { return HasTraitsProperty(N); }

	virtual StorageBase *NewStorage(Registry *, Handle)const  = 0;
	virtual void Create(StorageBase &) const = 0;
	virtual bool Destroy(StorageBase &) const = 0;
	virtual void Delete(StorageBase &) const = 0;

	virtual void Assign(StorageBase &, StorageBase const &) const = 0;
	virtual void SetSwitch(StorageBase &Q, int S, bool M) const = 0;
	void SetMarked(StorageBase &Q, bool M) const;
	virtual void SetMarked2(StorageBase &Q, bool M) const = 0;

	virtual Object UpCast(StorageBase &) const = 0;
	virtual Object CrossCast(StorageBase &, Type::Number) const = 0;
	virtual Object DownCast(StorageBase &, Type::Number) const = 0;

	virtual HashValue GetHashValue(const StorageBase &) const = 0;

	Object Absolute(Object const &object) const
	{
		return Absolute(object.GetStorageBase());
	}
	bool Less(Object const &lhs, Object const &rhs) const
	{
		return Less(lhs.GetStorageBase(), rhs.GetStorageBase());
	}
	bool Equiv(Object const &lhs, Object const &rhs) const
	{
		return Equiv(lhs.GetStorageBase(), rhs.GetStorageBase());
	}
	bool Greater(Object const &lhs, Object const &rhs) const
	{
		return Greater(lhs.GetStorageBase(), rhs.GetStorageBase());
	}

	virtual Object Absolute(const StorageBase &) const = 0;
	virtual bool Less(const StorageBase &, const StorageBase &) const = 0;
	virtual bool Greater(const StorageBase &, const StorageBase &) const = 0;
	virtual bool Equiv(const StorageBase &, const StorageBase &) const = 0;

	virtual void Insert(StringStream &, const StorageBase &) const = 0;
	virtual StorageBase *Extract(Registry &, StringStream &) const = 0;
	virtual void ExtractValue(Object &object, StringStream &strstream) const = 0;

	virtual void Insert(BinaryStream &, const StorageBase &) const = 0;
	virtual StorageBase *Extract(Registry &, BinaryPacket &) const = 0;
	
	//virtual void Insert(XmlStream &, const StorageBase &) = 0;
	//virtual StorageBase *Extract(Registry &, XmlStream &) = 0;

	void Assign(Object A, Object B) const
	{
		Assign(A.GetStorageBase(), B.GetStorageBase());
	}
	Object Plus(Object A, Object B) const
	{
		return Plus(A.GetStorageBase(), B.GetStorageBase());
	}
	Object Minus(Object A, Object B) const
	{
		return Minus(A.GetStorageBase(), B.GetStorageBase());
	}
	Object Multiply(Object A, Object B) const
	{
		return Multiply(A.GetStorageBase(), B.GetStorageBase());
	}
	Object Divide(Object A, Object B) const
	{
		return Divide(A.GetStorageBase(), B.GetStorageBase());
	}

	virtual StorageBase *Plus(StorageBase const &, StorageBase const &) const = 0;
	virtual StorageBase *Minus(StorageBase const &, StorageBase const &) const = 0;
	virtual StorageBase *Multiply(StorageBase const &, StorageBase const &) const = 0;
	virtual StorageBase *Divide(StorageBase const &, StorageBase const &) const = 0;

	bool Equiv2(const Object &A, const Object &B) const
	{
		return Equiv(A.GetStorageBase(), B.GetStorageBase());
	}
	bool Less2(const Object &A, const Object &B) const
	{
		return Less(A.GetStorageBase(), B.GetStorageBase());
	}
	bool Greater2(const Object &A, const Object &B) const
	{
		return Greater(A.GetStorageBase(), B.GetStorageBase());
	}
};

StringStream &operator<<(StringStream &, const ClassBase *);

KAI_TYPE_TRAITS_NAMED(const ClassBase *, Type::Number::Class, "Class", Type::Properties::StringStreamInsert);

KAI_END

#endif // KAI_CLASS_BASE_H

//EOF
