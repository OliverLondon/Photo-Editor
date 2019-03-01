#include <PNMwriter.h>
#include <stdio.h>
#include <logging.h>

PNMwriter::PNMwriter(void){}

PNMwriter::~PNMwriter(){}

const char *PNMwriter::SinkName(){return "PNMwriter";}

void PNMwriter::Write(const char *fn)
{
	if ((fn == NULL) or (fn == "")){
		char msg[256];
		sprintf(msg,"%s: No valid output file name\n", fn);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	FILE *f_out;
	f_out = fopen(fn,"wb");
	if (this->GetImg1() == NULL){
		char msg[256];
		sprintf(msg,"%s: No image to write\n", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	int w = this->GetImg1()->GetWidth();
	int h = this->GetImg1()->GetHeight();
	int size = w * h;

	fprintf(f_out, "P6\n%d %d\n255\n", w, h);
	fwrite(this->GetImg1()->GetPixelArray(), sizeof(unsigned char)*3, size, f_out);

	fclose(f_out);
}

void PNMwriter::SetInput(Image *img)
{
	Sink::SetInput(img);
}