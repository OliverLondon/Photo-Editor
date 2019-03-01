#include <source.h>
#include <sink.h>
#ifndef filter_h
#define filter_h

class Filter : public Source, public Sink 
{
	public:
		virtual const char *FilterName() = 0;
		static void Increase_Depth();
		static int update_depth;
		int Get_Depth();
		void Reset_Depth();
};

class Blender : public Filter
{
	private:
		double factor;
		
	public:
		Blender(void);
		Blender(double);
		
		void SetFactor(double);
		void SetInput(Image *);
		void SetInput2(Image *);
		
		Image *GetOutput(void);
		virtual void Update(void);
		virtual const char *FilterName();
		virtual const char *SourceName();
		virtual const char *SinkName();
		
	protected:
		virtual void Execute(void);
};

class Shrinker : public Filter
{
	public:
		void SetInput(Image *);
		
		Image *GetOutput(void);
		virtual void Update(void);
		virtual const char *FilterName();
		virtual const char *SourceName();
		virtual const char *SinkName();
		
	protected:
		virtual void Execute(void);
};

class LRCombine : public Filter
{
	public:
		void SetInput(Image *);
		void SetInput2(Image *);
		
		Image *GetOutput(void);
		virtual void Update(void);
		virtual const char *FilterName();
		virtual const char *SourceName();
		virtual const char *SinkName();
		
	protected:
		virtual void Execute(void);
};

class TBCombine : public Filter
{
	public:
		void SetInput(Image *);
		void SetInput2(Image *);
		
		Image *GetOutput(void);
		virtual void Update(void);
		virtual const char *FilterName();
		virtual const char *SourceName();
		virtual const char *SinkName();
		
	protected:
		virtual void Execute(void);
};

class CheckSum : public Sink
{
	public:
		CheckSum(void);
	   ~CheckSum();
		void SetInput(Image *);
		void OutputCheckSum(const char *);
		virtual const char *SinkName();
		
	protected:
		int rSum;
		int gSum;
		int bSum;
		char *FName;
};

class Mirror : public Filter
{
	public:
		void SetInput(Image *);
		virtual const char *FilterName();
		virtual const char *SourceName();
		virtual const char *SinkName();
		Image *GetOutput(void);
		virtual void Update(void);
		
	protected:
		virtual void Execute(void);
};

class Rotate : public Filter
{
	public:
		void SetInput(Image *);
		virtual const char *FilterName();
		virtual const char *SourceName();
		virtual const char *SinkName();
		Image *GetOutput(void);
		virtual void Update(void);
		
	protected:
		virtual void Execute(void);
};

class Subtract : public Filter
{
	public:
		void SetInput(Image *);
		void SetInput2(Image *);
		virtual const char *FilterName();
		virtual const char *SourceName();
		virtual const char *SinkName();
		Image *GetOutput(void);
		virtual void Update(void);
		
	protected:
		virtual void Execute(void);
};

class Color : public Source
{
	private:
		int r;
		int g;
		int b;
		int w;
		int h;
	public:
		Color();
		Color(int,int,int,int,int);
		virtual const char *SourceName();
		Image *GetOutput(void);
		virtual void Update(void);
	
	protected:
		virtual void Execute(void);
};

class Grayscale : public Filter
{
	public:
		void SetInput(Image *);
		virtual const char *FilterName();
		virtual const char *SourceName();
		virtual const char *SinkName();
		Image *GetOutput(void);
		virtual void Update(void);
		
	protected:
		virtual void Execute(void);
};

class Blur : public Filter
{
	public:
		void SetInput(Image *);
		virtual const char *FilterName();
		virtual const char *SourceName();
		virtual const char *SinkName();
		Image *GetOutput(void);
		virtual void Update(void);
		
	protected:
		virtual void Execute(void);
};

#endif