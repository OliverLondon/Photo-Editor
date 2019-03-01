#include <image.h>
#ifndef sink_h
#define sink_h

class Sink
{
	protected:
		Image *i1;
		Image *i2;
		
	public:
	   ~Sink();
		Sink(void);
		
		void SetInput(Image *);
		void SetInput2(Image *);

		Image *GetImg1(void);
		Image *GetImg2(void);
		virtual const char *SinkName() = 0;
};
#endif