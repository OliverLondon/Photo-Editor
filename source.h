#include <image.h>
#ifndef source_h
#define source_h
class Source
{	
	public:
	   ~Source();
	    Source(void);
		void SetInput(Image*);
		Image *GetOutput(void);
		virtual void Update(void) = 0;
		virtual const char *SourceName() = 0;
		
	protected:
		virtual void Execute(void) = 0;
		Image *i;
		
};
#endif