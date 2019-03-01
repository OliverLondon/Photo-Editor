#ifndef image_h
#define image_h
class Source;

class Image
{	
	private:
		int width;
		int height;
		unsigned char *PixelArray;
		Source *source;
		
	public:
		Image(void);
		Image(int, int);
		Image(Image &);
	   ~Image();
		
		void ResetSize(int w, int h);
		void Update(void);
		
		int GetWidth(void);
		int GetHeight(void);
		unsigned char *GetPixelArray(void);
		
		void SetWidth(int);
		void SetHeight(int);
		void SetPixelArray(int);
		void SetSource(Source *);
};
#endif