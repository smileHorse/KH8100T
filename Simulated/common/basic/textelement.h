#ifndef TEXTELEMENT_H
#define TEXTELEMENT_H

#include <sstream>
#include <string>
#include <vector>

using namespace std;

template<class inType, class outType>
class ConvertType
{
public:
	outType convert(const inType& inValue)
	{
		outType outValue;
		stringstream str;
		str << inValue;
		str >> outValue;
		return outValue;
	}
};

template<class T>
class ConvertTypeToString : public ConvertType<T, string>
{
public:
	string convertToString(const T& value)
	{
		return convert(value);
	}
};

class TextElement
{
public:
	TextElement(const string& _label, const string& _text, TextElement* _parent = 0);
	~TextElement();

	void	insertChild(TextElement* element);

	string	toString();

private:
	string	label;
	string	text;

	TextElement*	parent;
	vector<TextElement*>	children;
};


#endif