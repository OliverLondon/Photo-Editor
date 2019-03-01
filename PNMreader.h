#include <source.h>
#include <image.h>
#ifndef PNMreader_h
#define PNMreader_h
class PNMreader : public Source
{
	private:
		char *FName;
		int utd;
		
	public:
		PNMreader(const char *);
	   ~PNMreader();
	   
		virtual void Update(void);
		
		void SetInput(const char *);
		void SetUTD(int);
		
		Image *GetOutput(void);
		int GetUTD(void);
		virtual const char *SourceName();

	protected:
		virtual void Execute(void);
};		
#endif