#ifndef TBASEMACRO_H_
#define TBASEMACRO_H_

#ifndef		SAFE_DELETE
#define		SAFE_DELETE(p)			if((p) != NULL) {delete (p); (p) = NULL;}
#endif

#ifndef		SAFE_DELETE_ARRAY
#define		SAFE_DELETE_ARRAY(p)	if((p) != NULL) {delete[] (p);(p) = NULL;}
#endif

#ifndef		ARRAY_SIZE
#define		ARRAY_SIZE(x)			(sizeof(x)/sizeof((x)[0]))
#endif

#ifndef		CheckPointer
#define		CheckPointer(p,ret)		{if( (p) == NULL) return (ret);}
#endif

#endif /*TBASEMACRO_H_*/
