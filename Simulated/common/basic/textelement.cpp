#include "textelement.h"

TextElement::TextElement( const string& _label, const string& _text, TextElement* _parent /*= 0*/ )
	: label(_label), text(_text), parent(_parent)
{
	if (parent)
	{
		parent->insertChild(*this);
	}
}

void TextElement::insertChild( const TextElement& element )
{
	children.push_back(element);
}

string TextElement::toString()
{
	stringstream str;
	str << label << ": " << text;
	for (vector<TextElement>::iterator iter = children.begin(); iter != children.end(); ++iter)
	{
		str << "\n\t";
		str << iter->toString();
	}

	return str.str();
}