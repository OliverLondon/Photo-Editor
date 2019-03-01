#include <PNMreader.h>
#include <image.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <logging.h>

PNMreader::PNMreader(const char *input)
{
	this->FName = new char[512];
	strcpy(this->FName, input);
	this->SetUTD(0);
}

const char *PNMreader::SourceName(){return "PNMreader";}

PNMreader::~PNMreader()
{
	if (this->FName != NULL){
		delete[] this->FName;
	}
	this->FName = NULL;
}

void PNMreader::SetInput(const char *input)
{
	delete[] this->FName;
	this->FName = new char[512];
	strcpy(this->FName, input);
	this->SetUTD(0);
}

void PNMreader::Execute(void)
{	
	char msg[256]; 
	sprintf(msg, "%s: opening file\n", SourceName());
    Logger::LogEvent(msg);
	
	FILE *f_in;
	f_in = fopen(this->FName,"rb");
	
	if (f_in == NULL){
		char msg[256];
		sprintf(msg,"%s: No valid input file! File name is:%s\n", SourceName(),this->FName);
		DataFlowException e(SourceName(), msg);
		throw e;
	}
	char magicNum[128];
	int width, height, maxval,size;
		
	sprintf(msg, "%s: reading file header\n", SourceName());
	Logger::LogEvent(msg);
	fscanf(f_in, "%s\n%d %d\n%d\n\n", magicNum, &width, &height, &maxval); 
	size = width*height;
	
	this->GetOutput()->SetHeight(height);
	this->GetOutput()->SetWidth(width);
	this->GetOutput()->SetPixelArray(3*size);
		
	sprintf(msg, "%s: copying image\n", SourceName());
	Logger::LogEvent(msg);
		
	fread(this->GetOutput()->GetPixelArray(),sizeof(unsigned char)*3,size,f_in);
	this->SetUTD(1);
	fclose(f_in);	
}
void PNMreader::Update(void)
{	
	char msg[256]; 
	sprintf(msg, "%s : attempting to execute\n", SourceName());
    Logger::LogEvent(msg);	
	if(!this->GetUTD()){
		this->Execute();
	}
	sprintf(msg, "%s : done executing\n", SourceName());
    Logger::LogEvent(msg);
}

Image *PNMreader::GetOutput(void)
{
	return Source::GetOutput();
}

void PNMreader::SetUTD(int i){this->utd = i;}
int PNMreader::GetUTD(void){return this->utd;}