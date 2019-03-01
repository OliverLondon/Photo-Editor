#include <image.h>
#include <source.h>
#include <cstddef>

Image *Source::GetOutput(void){return this->i;}

Source::~Source()
{
	delete this->i;
	this->i = NULL;
}

Source::Source(void)
{
	this->i = new Image;
	this->i->SetSource(this);
}

void Source::SetInput(Image *img){this->i = img;}
