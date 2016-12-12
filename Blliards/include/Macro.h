#pragma once
#include <comip.h>
#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

#ifdef _UNICODE
typedef std::wstring String;
typedef LPCWSTR LPCGFWCHAR;
typedef WCHAR GFWCHAR;
#else
typedef std::string String;
typedef LPCSTR LPCGFWCHAR;
typedef CHAR GFWCHAR;
#endif

template<typename T>
using SharedPtr = boost::shared_ptr<T>;
template <typename T>
using SharedListPtr = std::list<SharedPtr<T>>;

#define D3DX_PI ((FLOAT) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))

#define SAFE_RELEASE(v) if(v) {v->Release(); v = nullptr;}
#define COMPTR(v) _com_ptr_t<_com_IIID<v,&__uuidof(v)>>
#ifdef _Debug
#define DEBUG(v) v
#define RELEASE(v)
#else
#define DEBUG(v)
#define RELEASE(v) v
#endif