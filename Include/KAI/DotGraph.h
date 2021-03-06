
#ifdef KAI_HAS_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_DOTGRAPH_H
#	define KAI_DOTGRAPH_H

KAI_BEGIN

// Generates GraphViz script showing the depedancy graph given an object root
struct DotGraph : StringStream
{
private:
	nstd::set<Label> excluded_names;
	nstd::set<Type::Number> excluded_types;

public:
	DotGraph();
	DotGraph(Object, String const &filename);

	void Generate(Object, String const &filename);

	void ExcludeLabel(Label const &);
	template <class T>
	void ExcludeType()
	{
		excluded_types.insert(Type::Traits<T>::Number);
	}
	void WriteHeader();
	void WriteFooter();
	void WriteToFile(const char *);

	void Add(Object);
	String GetText();

	friend DotGraph &operator<<(DotGraph &graph, Object const &object);

protected:
	bool IsExcluded(Object const&) const;
};

DotGraph &operator<<(DotGraph &graph, Object const &object);

KAI_END

#endif // KAI_DOTGRAPH_H

//EOF
