#include <image.h>
#include <stdlib.h>
#include <algorithm>
#include <source.h>

Image::Image(void){
	this->SetWidth(0); 
	this->SetHeight(0);
	this->PixelArray = NULL;
}

Image::Image(int w, int h){
	this->PixelArray = NULL;
	this->SetWidth(w); 
	this->SetHeight(h);
	this->SetPixelArray(3*w*h);
}

Image::Image(Image &i){
	int w = i.GetWidth();
	int h = i.GetHeight();
	this->SetWidth(w);
	this->SetHeight(h);
	this->SetPixelArray(3*w*h);
	std::copy(i.GetPixelArray(), i.PixelArray+(w*h),this->GetPixelArray());
}

Image::~Image(){
	free(this->GetPixelArray());
	this->PixelArray = NULL;
	this->source = NULL;
}

void Image::ResetSize(int w, int h){
	this->SetWidth(w);
	this->SetHeight(h);
	this->SetPixelArray(3*w*h);
}

int Image::GetWidth(void){return this->width;}
int Image::GetHeight(void){return this->height;}
unsigned char *Image::GetPixelArray(void){return this->PixelArray;}

void Image::SetWidth(int i){this->width = i;}
void Image::SetHeight(int i){this->height = i;}

void Image::SetPixelArray(int size)
{
	if (this->PixelArray == NULL){
		this->PixelArray = (unsigned char *)malloc(sizeof(unsigned char)*size);
	}
	else{
		free(this->GetPixelArray());
		this->PixelArray = (unsigned char *)malloc(sizeof(unsigned char)*size);
	}
}

void Image::Update(void)
{
	this->source->Update();
}

void Image::SetSource(Source *s)
{
	this->source = s;
}