#ifndef MIRO_IMAGE_TYPES
#define MIRO_IMAGE_TYPES

#ifdef STL_SAVVY
#include <string>
#include <iostream>
#endif

namespace MiroImage {

typedef	unsigned char			uint8;		//	8 bit
typedef	unsigned short int		uint16;		//	16 bit
typedef	unsigned long int		uint32;		//	32 bit
typedef	unsigned long long int	uint64;		//	64 bit

typedef	char				int8;	//	8 bit
typedef	short int			int16;	//	16 bit
typedef	long int			int32;	//	32 bit
typedef	long long int		int64;	//	64 bit

typedef	uint8				byte;	//	8 bit
typedef	uint16				word;	//	16 bit
typedef	uint32				word32;	//	32 bit
typedef	uint64				word64;	//	64 bit


//	introduce some commonly used types:
typedef bool	booleanType;
typedef char	characterType;
typedef uint16	unicharType;

typedef uint16	shortcardType;
typedef int16	shortintType;
typedef uint32	cardinalType;
typedef int32	integerType;
typedef float	shortRealType;
typedef double	realType;

class shared
	{
	public:
		shared();
		virtual	~shared();

		void	lock();
		void	unlock();

		void	attach();
		void	detach();
		uint32	CountAttached();

	private:
		uint32	iSharedCount;
	};

}	// namespace

#endif
