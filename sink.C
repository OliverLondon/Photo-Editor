#include <sink.h>
#include <stdlib.h>
void Sink::SetInput(Image *in){this->i1 = in;}

void Sink::SetInput2(Image *in){this->i2 = in;}

Sink::~Sink(){}

Sink::Sink(void)
{
	this->i1 = NULL;
	this->i2 = NULL;
}

Image *Sink::GetImg1(void){return this->i1;}

Image *Sink::GetImg2(void){return this->i2;}
