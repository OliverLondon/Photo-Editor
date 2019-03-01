#include <sink.h>
#include <image.h>
#ifndef PNMwriter_h
#define PNMwriter_h
class PNMwriter : public Sink
{
	private:
		const char *FName;
		
	public:
		PNMwriter(void);
	   ~PNMwriter();
		
		virtual void Write(const char *);
		void SetInput(Image *);
		virtual const char *SinkName();
};
#endif