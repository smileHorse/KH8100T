#ifndef TEXTELEMENT_H
#define TEXTELEMENT_H

#include <sstream>
#include <string>
#include <vector>

using namespace std;

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