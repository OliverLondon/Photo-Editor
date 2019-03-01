#include <filter.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <logging.h>
int Filter::update_depth = 0;
void Filter::Increase_Depth(){update_depth+=1;}
int Filter::Get_Depth(){return update_depth;}
void Filter::Reset_Depth(){update_depth = 0;}

Blender::Blender(void){this->factor = 0;}
Blender::Blender(double d){this->factor = d;}
const char *Blender::SinkName(){return "Blender";}
const char *Blender::SourceName(){return "Blender";}
const char *Blender::FilterName(){return "Blender";}
void Blender::SetFactor(double d){this->factor = d;}
void Blender::SetInput(Image *img){Sink::SetInput(img);}
void Blender::SetInput2(Image *img){Sink::SetInput2(img);}
Image *Blender::GetOutput(void){return Source::GetOutput();}

void Blender::Execute(void)
{	
	Image *image1 = this->GetImg1();
	Image *image2 = this->GetImg2();
	if (this->factor == 0){
		char msg[256];
		sprintf(msg,"%s: Factor cannot be 0, or no factor set\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (this->factor > 1){
		char msg[256];
		sprintf(msg,"%s: Factor too large: %f, must be less than 1.0\n", SinkName(),this->factor);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	
	int w1 = image1->GetWidth();
	int h1 = image1->GetHeight();
	int w2 = image2 ->GetWidth();
	int h2 = image2 ->GetHeight();
	if ((w1 != w2) or (h1 != h2)){
		char msg[256];
		sprintf(msg,"%s: Sizes do not match: image1:%d by %d, image2:%d by %d\n", SinkName(),w1,h1,w2,h2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if((w1==0) or (w2==0) or (h1==0) or (h2==0)){
		char msg[256];
		sprintf(msg,"%s: There should be no 0 dimensions. h1:%d w1:%d h2:%d w2:%d\n", SinkName(),h1,w1,h2,w2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *op = this->GetOutput();
	op->SetWidth(w1);
	op->SetHeight(h1);
	int size = w1*h1;
	op->SetPixelArray(3*size);
	
	unsigned char r,g,b;
	unsigned char *arr = op->GetPixelArray();
	unsigned char *i1Arr = image1->GetPixelArray();
	unsigned char *i2Arr = image2 ->GetPixelArray();
	int j = 0;
	double factorX = this->factor;
	for(int i = 0; i < size; i++){
		j = i*3;
		r = ((factorX * i1Arr[j+0]) + ((1-factorX) * i2Arr[j+0]));
		g = ((factorX * i1Arr[j+1]) + ((1-factorX) * i2Arr[j+1]));
		b = ((factorX * i1Arr[j+2]) + ((1-factorX) * i2Arr[j+2]));
		arr[j+0] = r;
		arr[j+1] = g;
		arr[j+2] = b;
	}
}

void Blender::Update(void)
{
	Filter::Increase_Depth();
	if (Filter::Get_Depth() > 2000){
		char msg[256];
		sprintf(msg,"%s: Update() depth exceeds 2000, stopping.\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *one = Sink::GetImg1();
	Image *two = Sink::GetImg2();
	if (one == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (two == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (one == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (two == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 2\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	
	char msg[256]; 
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	one->Update();
	sprintf(msg, "%s: Updating image 2\n", SinkName());
    Logger::LogEvent(msg);
	two->Update();
	
	sprintf(msg, "%s: Attempting to execute\n", SinkName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SinkName());
    Logger::LogEvent(msg);
}
//-------------------------------------------------
void Shrinker::SetInput(Image *img){Sink::SetInput(img);}
const char *Shrinker::SinkName(){return "Shrinker";}
const char *Shrinker::SourceName(){return "Shrinker";}
const char *Shrinker::FilterName(){return "Shrinker";}
Image *Shrinker::GetOutput(void){return Source::GetOutput();}

void Shrinker::Execute(void)
{	
	int w1 = Sink::GetImg1()->GetWidth();
	int h1 = Sink::GetImg1()->GetHeight();
	if((h1==0) or (w1==0)){
		char msg[256];
		sprintf(msg,"%s: There should be no 0 dimensions. h1:%d w1:%d\n", SinkName(),h1,w1);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	
	Image *op = Source::GetOutput();
	
	op->SetWidth(w1 / 2);
	op->SetHeight(h1 / 2);
	int size = op->GetWidth() * op->GetHeight();
	
	op->SetPixelArray(3*size);
	
	int tall = Source::GetOutput()->GetHeight();
	int wide = Source::GetOutput()->GetWidth();
	int outspot, inspot;
	unsigned char *SourceArr = Source::GetOutput()->GetPixelArray();
	unsigned char *SinkArr = Sink::GetImg1()->GetPixelArray();
	
	for (int i = 0; i < tall; i++){
		for (int j = 0; j < wide; j++){
			outspot = (((i*wide)+j)*3);
			inspot = (((i*2*w1)+(j*2))*3);
			SourceArr[outspot] = SinkArr[inspot];
			SourceArr[outspot+1] = SinkArr[inspot+1];
			SourceArr[outspot+2] = SinkArr[inspot+2];
		}
	}
}

void Shrinker::Update(void)
{
	Filter::Increase_Depth();
	if (Filter::Get_Depth() > 2000){
		char msg[256];
		sprintf(msg,"%s: Update() depth exceeds 2000, stopping.\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *one = Sink::GetImg1();
	if (one == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (one == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	char msg[256]; 
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	one->Update();
	sprintf(msg, "%s: Updating image 2\n", SinkName());
    Logger::LogEvent(msg);

	sprintf(msg, "%s: Attempting to execute\n", SinkName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SinkName());
    Logger::LogEvent(msg);
}
//-------------------------------------------------
void LRCombine::SetInput(Image *img){Sink::SetInput(img);}
const char *LRCombine::SinkName(){return "LRCombine";}
const char *LRCombine::SourceName(){return "LRCombine";}
const char *LRCombine::FilterName(){return "LRCombine";}
void LRCombine::SetInput2(Image *img){Sink::SetInput2(img);}
Image *LRCombine::GetOutput(void){return Source::GetOutput();}

void LRCombine::Execute(void)
{
	Image *image1 = this->GetImg1();
	Image *image2 = this->GetImg2();	
	int w1 = image1->GetWidth();
	int h1 = image1->GetHeight();
	int w2 = image2->GetWidth();
	int h2 = image2->GetHeight();

	if (h1 != h2){
		char msg[256];
		sprintf(msg,"%s: Heights do not match: %d and %d\n", SinkName(),h1,h2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if((w1==0) or (w2==0) or (h1==0) or (h2==0)){
		char msg[256];
		sprintf(msg,"%s: There should be no 0 dimensions. h1:%d w1:%d h2:%d w2:%d\n", SinkName(),h1,w1,h2,w2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *op = this->GetOutput();
	int w = w1+w2;
	op->SetWidth(w);
	op->SetHeight(h1);
	op->SetPixelArray(3*w*h1);
	
	unsigned char *Sink1Arr = image1->GetPixelArray();
	unsigned char *Sink2Arr = image2->GetPixelArray();
	unsigned char *SourceArr = op->GetPixelArray();
	
	for(int i = 0; i < h1; i++){
		std::copy(Sink1Arr+(i*w1*3), Sink1Arr+(((i+1)*w1*3)),SourceArr+(i*w*3));
		std::copy(Sink2Arr+(i*w2*3), Sink2Arr+(((i+1)*w2*3)),SourceArr+((((i+1)*w)-w2)*3));
	}
}

void LRCombine::Update(void)
{
	Filter::Increase_Depth();
	if (Filter::Get_Depth() > 2000){
		char msg[256];
		sprintf(msg,"%s: Update() depth exceeds 2000, stopping.\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *one=Sink::GetImg1();
	Image *two=Sink::GetImg2();
	if (one == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (two == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (one == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (two == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 2\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	char msg[256]; 
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	one->Update();
	sprintf(msg, "%s: Updating image 2\n", SinkName());
    Logger::LogEvent(msg);
	two->Update();

	sprintf(msg, "%s: Attempting to execute\n", SinkName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SinkName());
    Logger::LogEvent(msg);
}
//-------------------------------------------------
void TBCombine::SetInput(Image *img){Sink::SetInput(img);}
const char *TBCombine::SinkName(){return "TBCombine";}
const char *TBCombine::SourceName(){return "TBCombine";}
const char *TBCombine::FilterName(){return "TBCombine";}
void TBCombine::SetInput2(Image *img){Sink::SetInput2(img);}
Image *TBCombine::GetOutput(void){return Source::GetOutput();}

void TBCombine::Execute(void)
{
	Image *image1 = this->GetImg1();
	Image *image2 = this->GetImg2();
	
	int w1 = image1->GetWidth();
	int h1 = image1->GetHeight();
	int w2 = image2->GetWidth();
	int h2 = image2->GetHeight();
	if (w1 != w2){
		char msg[256];
		sprintf(msg,"%s: Widths do not match: %d and %d\n", SinkName(),w1,w2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if((w1==0) or (w2==0) or (h1==0) or (h2==0)){
		char msg[256];
		sprintf(msg,"%s: There should be no 0 dimensions. h1:%d w1:%d h2:%d w2:%d\n", SinkName(),h1,w1,h2,w2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *op = this->GetOutput();
	op->SetWidth(w1);
	op->SetHeight(h1 + h2);
	int size = w1 * (h1+h2);
	op->SetPixelArray(3*size);
	unsigned char *Sink1Arr = image1->GetPixelArray();
	unsigned char *Sink2Arr = image2->GetPixelArray();
	unsigned char *SourceArr = op->GetPixelArray();
	
	std::copy(Sink1Arr, Sink1Arr+(w1 * h1*3), SourceArr);
	std::copy(Sink2Arr, Sink2Arr+(w2 * h2*3), SourceArr+(3*w1*h1));
}

void TBCombine::Update(void)
{
	Filter::Increase_Depth();
	if (Filter::Get_Depth() > 2000){
		char msg[256];
		sprintf(msg,"%s: Update() depth exceeds 2000, stopping.\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *one=Sink::GetImg1();
	Image *two=Sink::GetImg2();
	if (one == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (two == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (one == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (two == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 2\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	char msg[256]; 
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	one->Update();
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	two->Update();

	sprintf(msg,"%s: Attempting to execute\n", SinkName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SinkName());
    Logger::LogEvent(msg);
}
//-------------------------------------------------
CheckSum::CheckSum(void)
{
	this->FName = new char[128];
	this->rSum = 0;
	this->gSum = 0;
	this->bSum = 0;
}

const char *CheckSum::SinkName(){return "CheckSum";}

CheckSum::~CheckSum()
{
	if (this->FName != NULL){
		delete[] this->FName;
		this->FName = NULL;
	}
}

void CheckSum::SetInput(Image *i){Sink::SetInput(i);}

void CheckSum::OutputCheckSum(const char *name)
{
	if ((name == NULL) or (name == "")){
		char msg[256];
		sprintf(msg,"%s: Not a valid output file name\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *img = this->GetImg1();
	if (img == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n",SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	strcpy(this->FName, name);
	
	FILE *f_out;
	f_out = fopen(this->FName,"w");
	
	
	int w = img->GetWidth();
	int h = img->GetHeight();
	int red = this->rSum;
	int green = this->gSum;
	int blue = this->bSum;
	int spot = 0;
	unsigned char *arr = img->GetPixelArray();
	
	for (int y=0; y<h; y++){
		for (int x=0; x<w; x++){
			spot = 3*((y*w) + x);
			red += arr[spot];
			green += arr[spot+1];
			blue += arr[spot+2];
		}
	}
	red %= 256;
	green %= 256;
	blue %= 256;
	fprintf(f_out, "CHECKSUM: %d, %d, %d\n",red,green,blue);
	fclose(f_out);
}
//-------------------------------------------------
void Mirror::SetInput(Image *i){Sink::SetInput(i);}
const char *Mirror::FilterName() {return "Mirror";}
const char *Mirror::SourceName(){return "Mirror";}
const char *Mirror::SinkName(){return "Mirror";}
Image *Mirror::GetOutput(void){return Source::GetOutput();}

void Mirror::Update(void) 
{
	Filter::Increase_Depth();
	if (Filter::Get_Depth() > 2000){
		char msg[256];
		sprintf(msg,"%s: Update() depth exceeds 2000, stopping.\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *one = Sink::GetImg1();
	if (one == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (one == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	char msg[256]; 
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	one->Update();
	
	sprintf(msg,"%s: Attempting to execute\n", SourceName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SourceName());
    Logger::LogEvent(msg);
}

void Mirror::Execute(void)
{
	Image *input = Sink::GetImg1();
	int h = input->GetHeight();
	int w = input->GetWidth();
	unsigned char *inArr = input->GetPixelArray();
	
	Image *output = Source::GetOutput();
	output->SetHeight(h);
	output->SetWidth(w);
	output->SetPixelArray(w*h*3);
	unsigned char *outArr = output->GetPixelArray();
	int inspot, outspot;
	
	for (int y = 0; y<h; y++){
		int x2 = 0;
		for (int x = w-1; x>=0; x--){
			inspot = ((y*w)+x)*3;
			outspot = ((y*w)+x2)*3;
			
			outArr[outspot] = inArr[inspot];
			outArr[outspot+1] = inArr[inspot+1];
			outArr[outspot+2] = inArr[inspot+2];
			x2++;
		}
	}
}
//-------------------------------------------------
void Rotate::SetInput(Image *i){Sink::SetInput(i);}
const char *Rotate::FilterName() {return "Rotate";}
const char *Rotate::SourceName(){return "Rotate";}
const char *Rotate::SinkName(){return "Rotate";}
Image *Rotate::GetOutput(void){return Source::GetOutput();}

void Rotate::Update(void) 
{
	Filter::Increase_Depth();
	if (Filter::Get_Depth() > 2000){
		char msg[256];
		sprintf(msg,"%s: Update() depth exceeds 2000, stopping.\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *one = Sink::GetImg1();
	if (one == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (one == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	char msg[256]; 
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	one->Update();
	
	sprintf(msg,"%s: Attempting to execute\n", SinkName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SinkName());
    Logger::LogEvent(msg);
}

void Rotate::Execute(void)
{
	Image *input = Sink::GetImg1();
	int h = input->GetHeight();
	int w = input->GetWidth();
	unsigned char *inArr = input->GetPixelArray();
	
	Image *output = Source::GetOutput();
	output->SetHeight(w);
	output->SetWidth(h);
	output->SetPixelArray(w*h*3);
	unsigned char *outArr = output->GetPixelArray();
	int inspot, outspot;
	
	for (int y = 0; y<h; y++){
		for (int x = 0; x<w; x++){
			inspot = ((y*w)+x)*3;
			outspot = (((x+1)*h)-(y+1))*3;
			
			outArr[outspot] = inArr[inspot];
			outArr[outspot+1] = inArr[inspot+1];
			outArr[outspot+2] = inArr[inspot+2];
		}
	}
}
//-------------------------------------------------
void Subtract::SetInput(Image *i){Sink::SetInput(i);}
void Subtract::SetInput2(Image *i){Sink::SetInput2(i);}
const char *Subtract::FilterName() {return "Subtract";}
const char *Subtract::SourceName(){return "Subtract";}
const char *Subtract::SinkName(){return "Subtract";}
Image *Subtract::GetOutput(void){return Source::GetOutput();}

void Subtract::Update(void) 
{
	Filter::Increase_Depth();
	if (Filter::Get_Depth() > 2000){
		char msg[256];
		sprintf(msg,"%s: Update() depth exceeds 2000, stopping.\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *one = Sink::GetImg1();
	Image *two = Sink::GetImg2();
	if (one == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (two == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (one == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (two == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 2\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	char msg[256]; 
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	one->Update();
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	two->Update();
	
	sprintf(msg,"%s: Attempting to execute\n", SinkName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SinkName());
    Logger::LogEvent(msg);
}

void Subtract::Execute(void)
{
	Image *input1 = Sink::GetImg1();
	Image *input2 = Sink::GetImg2();
	int h1 = input1->GetHeight();
	int w1 = input1->GetWidth();
	int h2 = input2->GetHeight();
	int w2 = input2->GetWidth();
	if ((w1 != w2) or (h1 != h2)){
		char msg[256];
		sprintf(msg,"%s: Sizes do not match: image1:%d by %d, image2:%d by %d\n", SinkName(),w1,h1,w2,h2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	unsigned char *inArr1 = input1->GetPixelArray();
	unsigned char *inArr2 = input2->GetPixelArray();
	
	Image *output = Source::GetOutput();
	output->SetHeight(h1);
	output->SetWidth(w1);
	output->SetPixelArray(w1*h1*3);
	unsigned char *outArr = output->GetPixelArray();
	int spot;
	
	for (int y = 0; y<h1; y++){
		for (int x = 0; x<w1; x++){
			spot = ((y*w1)+x)*3;
			
			outArr[spot] = inArr1[spot] > inArr2[spot]  ? (inArr1[spot] - inArr2[spot]) : 0;
			outArr[spot+1] = inArr1[spot+1] > inArr2[spot+1] ? (inArr1[spot+1] - inArr2[spot+1]) : 0;
			outArr[spot+2] = inArr1[spot+2] > inArr2[spot+2] ? (inArr1[spot+2] - inArr2[spot+2]): 0;
		}
	}
}
//-------------------------------------------------
const char *Color::SourceName(){return "Color";}
Image *Color::GetOutput(void){return Source::GetOutput();}

Color::Color()
{
	this->w = 0;
	this->h = 0;
	this->r = -1;
	this->g = -1;
	this->b = -1;
}

Color::Color(int width, int height, int red, int green, int blue)
{
	this->r = red;
	this->g = green;
	this->b = blue;
	this->w = width;
	this->h = height;
}

void Color::Update(void) 
{
	if ((this->r >255) or (this->g >255) or (this->b <0) or (this-> r<0) or (this->g <0) or  (this->b <0)){
		char msg[256];
		sprintf(msg,"%s: Color values need to be between 0 and 255. r:%d g:%d b:%d \n", SourceName(),this->r,this->g,this->b);
		DataFlowException e(SourceName(), msg);
		throw e;
	}
	if ((this->w <= 0) or (this->h <= 0)){
		char msg[256];
		sprintf(msg,"%s: width and height must be greater than 0. w:%d h:%d\n", SourceName(),this->w,this->h);
		DataFlowException e(SourceName(), msg);
		throw e;
	}
	char msg[256];
	sprintf(msg,"%s: Attempting to execute\n", SourceName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SourceName());
    Logger::LogEvent(msg);
}

void Color::Execute(void)
{
	Image *output = Source::GetOutput();
	int width = this->w;
	int height = this->h;
	int red = this->r;
	int green = this->g;
	int blue = this->b;

	output->SetHeight(height);
	output->SetWidth(width);
	output->SetPixelArray(width*height*3);
	unsigned char *outArr = output->GetPixelArray();
	
	int outspot;
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			outspot = ((y*w)+x)*3;
			outArr[outspot] = red;
			outArr[outspot+1] = green;
			outArr[outspot+2] = blue;
		}
	}
}
//-------------------------------------------------
void Grayscale::SetInput(Image *i){Sink::SetInput(i);}
const char *Grayscale::FilterName() {return "Grayscale";}
const char *Grayscale::SourceName(){return "Grayscale";}
const char *Grayscale::SinkName(){return "Grayscale";}
Image *Grayscale::GetOutput(void){return Source::GetOutput();}

void Grayscale::Update(void) 
{
	Filter::Increase_Depth();
	if (Filter::Get_Depth() > 2000){
		char msg[256];
		sprintf(msg,"%s: Update() depth exceeds 2000, stopping.\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *one = Sink::GetImg1();
	if (one == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (one == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	char msg[256]; 
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	one->Update();
	
	sprintf(msg,"%s: Attempting to execute\n", SinkName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SinkName());
    Logger::LogEvent(msg);
}

void Grayscale::Execute(void)
{
	Image *input = Sink::GetImg1();
	int h = input->GetHeight();
	int w = input->GetWidth();
	if((w==0) or (h==0)){
		char msg[256];
		sprintf(msg,"%s: There should be no 0 dimensions. h1:%d w1:%d\n", SinkName(),h,w);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	unsigned char *inArr = input->GetPixelArray();
	
	Image *output = Source::GetOutput();
	output->SetHeight(h);
	output->SetWidth(w);
	
	output->SetPixelArray(w*h*3);
	unsigned char *outArr = output->GetPixelArray();
	
	int spot,gray;
	for (int y = 0; y<h; y++){
		for (int x = 0; x<w; x++){
			spot = ((y*w)+x)*3;
			gray = inArr[spot]/5 + inArr[spot+1]/2 + inArr[spot+2]/4;
			outArr[spot] = gray;
			outArr[spot+1] = gray;
			outArr[spot+2] = gray;
		}
	}	
}
//-------------------------------------------------
void Blur::SetInput(Image *i){Sink::SetInput(i);}
const char *Blur::FilterName() {return "Blur";}
const char *Blur::SourceName(){return "Blur";}
const char *Blur::SinkName(){return "Blur";}
Image *Blur::GetOutput(void){return Source::GetOutput();}

void Blur::Update(void) 
{
	Filter::Increase_Depth();
	if (Filter::Get_Depth() > 2000){
		char msg[256];
		sprintf(msg,"%s: Update() depth exceeds 2000, stopping.\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	Image *one = Sink::GetImg1();
	if (one == this->GetOutput()){
		char msg[256];
		sprintf(msg,"%s: can't set output as input\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if (one == NULL){
		char msg[256];
		sprintf(msg,"%s: No image 1\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	char msg[256]; 
	sprintf(msg, "%s: Updating image 1\n", SinkName());
    Logger::LogEvent(msg);
	one->Update();
	
	sprintf(msg,"%s: Attempting to execute\n", SinkName());
    Logger::LogEvent(msg);
	this->Execute();
	sprintf(msg, "%s: Done executing\n", SinkName());
    Logger::LogEvent(msg);
}

void Blur::Execute(void)
{
	Image *input = Sink::GetImg1();
	int h = input->GetHeight();
	int w = input->GetWidth();
	if((w==0) or (h==0)){
		char msg[256];
		sprintf(msg,"%s: There should be no 0 dimensions. h1:%d w1:%d\n", SinkName(),h,w);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	unsigned char *inArr = input->GetPixelArray();
	
	Image *output = Source::GetOutput();
	output->SetHeight(h);
	output->SetWidth(w);
	
	output->SetPixelArray(w*h*3);
	unsigned char *outArr = output->GetPixelArray();
	
	int spot,red,green,blue,u,d,l,r,ul,ur,dl,dr;
	for (int y = 0; y<h; y++){
		for (int x = 0; x<w; x++){
			spot = ((y*w)+x)*3;
			if ((y == 0) or (y == h-1) or (x == 0) or (x == w-1)){
				outArr[spot] = inArr[spot];
				outArr[spot+1] = inArr[spot+1];
				outArr[spot+2] = inArr[spot+2];			
			}
			else
			{
				u=spot-(w*3);
				d=spot+(w*3);
				l=spot-3;
				r=spot+3;
				ul=spot-(w*3)-3;
				ur=spot-(w*3)+3;
				dl=spot+(w*3)-3;
				dr=spot+(w*3)+3;
				
				red = (inArr[ul]/8)+(inArr[u]/8)+(inArr[ur]/8)+(inArr[l]/8)+(inArr[r]/8)+(inArr[dl]/8)+(inArr[d]/8)+(inArr[dr]/8);
				green =(inArr[ul+1]/8)+(inArr[u+1]/8)+(inArr[ur+1]/8)+(inArr[l+1]/8)+(inArr[r+1]/8)+(inArr[dl+1]/8)+(inArr[d+1]/8)+(inArr[dr+1]/8);
				blue = (inArr[ul+2]/8)+(inArr[u+2]/8)+(inArr[ur+2]/8)+(inArr[l+2]/8)+(inArr[r+2]/8)+(inArr[dl+2]/8)+(inArr[d+2]/8)+(inArr[dr+2]/8);
			
				outArr[spot] = red;
				outArr[spot+1] = green;
				outArr[spot+2] = blue;
			}
		}
	}	
}
