/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#ifndef __HEADER_DEMO_CALCULATOR_GRAMMAR_HXX__
# define __HEADER_DEMO_CALCULATOR_GRAMMAR_HXX__



inline std::ostream& operator << (std::ostream& out, const ::Demo::Calculator::Node& node)
{
	::Yuni::Clob content;
	::Demo::Calculator::Node::Export(content, node);out << content;
	return out;
}



inline std::ostream& operator << (std::ostream& out, const ::Demo::Calculator::Node::Ptr node)
{
	if (!(!node))
		out << *node;
	else
		out << "<invalid ast node>";
	return out;
}



namespace Demo
{
namespace Calculator
{


	inline Node::Node()
		: rule(rgUnknown)
		, offset()
		, offsetEnd()
	{}


	inline Node::Node(const Node& rhs)
		: inherited()
		, rule(rhs.rule)
		, offset(rhs.offset)
		, offsetEnd(rhs.offsetEnd)
		, text(rhs.text)
	{
		if (not rhs.children.empty())
		{
			children.resize(rhs.children.size());
			for (unsigned int i = 0; i != (unsigned) rhs.children.size(); ++i)
				children[i] = new Node(*rhs.children[i]);
		}
	}


	inline Node::~Node()
	{
	}


	template<class F> inline bool Node::each(const F& callback)
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			if (not callback(*children[i]))
				return false;
		}
		return true;
	}


	template<class F> inline bool Node::each(const F& callback) const
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			if (not callback(*children[i]))
				return false;
		}
		return true;
	}


	template<class F> inline bool Node::each(enum Rule rule, const F& callback)
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			Node& subnode = *children[i];
			if (subnode.rule == rule and not callback(subnode))
				return false;
		}
		return true;
	}


	template<class F> inline bool Node::each(enum Rule rule, const F& callback) const
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			const Node& subnode = *children[i];
			if (subnode.rule == rule and not callback(subnode))
				return false;
		}
		return true;
	}


	template<class StringT> inline bool Node::extractFirstChildText(StringT& out, enum Rule rule) const
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			const Node& subnode = *children[i];
			if (subnode.rule == rule)
			{
				out += subnode.text;
				return true;
			}
		}
		return false;
	}


	template<class StringT> inline bool Node::extractChildText(StringT& out, enum Rule rule, const AnyString& separator) const
	{
		bool somethingFound = false;

		if (separator.empty())
		{
			for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
			{
				const Node& subnode = *children[i];
				if (subnode.rule == rule)
				{
					out += subnode.text;
					somethingFound = true;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
			{
				const Node& subnode = *children[i];
				if (subnode.rule == rule)
				{
					if (not out.empty())
						out += separator;
					out += subnode.text;
					somethingFound = true;
				}
			}
		}
		return somethingFound;
	}


	inline uint Node::findFirst(enum Rule rule) const
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			if (children[i]->rule == rule)
				return i;
		}
		return (uint)-1;
	}


	#ifdef __HEADER_DEMO_CALCULATOR_GRAMMAR_HAS_CXX_INITIALIZER_LIST
	inline Node::Ptr  Node::xpath(std::initializer_list<enum Rule> path) const
	{
		if (path.size() > 0)
		{
			auto it = path.begin();
			for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
			{
				if (children[i]->rule == *it)
				{
					Node::Ptr result = children[i];
					++it;
					for (; it != path.end(); ++it)
					{
						bool found = false;
						auto& subnode = *result;
						for (unsigned int j = 0; j != (unsigned) subnode.children.size(); ++j)
						{
							if (subnode.children[j]->rule == *it)
							{
								result = subnode.children[j];
								found = true;
								break;
							}
						}
						if (not found)
							return nullptr;
					}
					return result;
				}
			}
		}
		return nullptr;
	}
	#endif







} // namespace Calculator
} // namespace Demo

#endif // __HEADER_DEMO_CALCULATOR_GRAMMAR_HXX__
