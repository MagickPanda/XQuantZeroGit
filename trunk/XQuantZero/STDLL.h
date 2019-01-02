#pragma once
#ifdef WIN32  
#ifdef __cplusplus   
	#define STDLL_EXPORT_C_DECL __declspec(dllexport)  
	#define STDLL_IMPORT_C_DECL __declspec(dllimport)
	#define STDLL_EXPORT_DECL extern __declspec(dllexport)  
	#define STDLL_IMPORT_DECL extern __declspec(dllimport)  
	#define STDLL_EXPORT_CLASS_DECL __declspec(dllexport)  
	#define STDLL_IMPORT_CLASS_DECL __declspec(dllimport)  
#else  
	#define STDLL_EXPORT_DECL __declspec(dllexport)  
	#define STDLL_IMPORT_DECL __declspec(dllimport)  
#endif  
#else  
#ifdef __cplusplus  
	#define extern "C"  
	#define STDLL_IMPORT_C_DECL extern "C"  
	#define STDLL_EXPORT_DECL extern  
	#define STDLL_IMPORT_DECL extern  
	#define STDLL_EXPORT_CLASS_DECL  
	#define STDLL_IMPORT_CLASS_DECL  
#else  
	#define STDLL_EXPORT_DECL extern  
	#define STDLL_IMPORT_DECL extern  
#endif  
#endif 

