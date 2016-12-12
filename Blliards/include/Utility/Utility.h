#pragma once
#include <Macro.h>
#include <Windows.h>
#pragma warning(disable:4996)

//マルチバイト文字列からワイド文字列
static void widen(const std::string &src, std::wstring &dest) {
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs(wcs, src.c_str(), src.length() + 1);
	dest = wcs;
	delete [] wcs;
}
static void narrow(const std::wstring &src, std::string &dest) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	dest = mbs;
	delete [] mbs;
}

//非推奨関数
static std::string UTF8toSjis(std::string srcUTF8){
  //Unicodeへ変換後の文字列長を得る
  int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(),srcUTF8.size() + 1, NULL, 0);
 
  //必要な分だけUnicode文字列のバッファを確保
  wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
 
  //UTF8からUnicodeへ変換
  MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1,bufUnicode, lenghtUnicode);
 
  //ShiftJISへ変換後の文字列長を得る
  int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);
 
  //必要な分だけShiftJIS文字列のバッファを確保
  char* bufShiftJis = new char[lengthSJis];
 
  //UnicodeからShiftJISへ変換
  WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);
 
  std::string strSJis(bufShiftJis);
 
  delete bufUnicode;
  delete bufShiftJis;
 
  return strSJis;
}
static std::string SjistoUTF8(std::string srcSjis){
  //Unicodeへ変換後の文字列長を得る
  int lenghtUnicode = (int)MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);
 
  //必要な分だけUnicode文字列のバッファを確保
  wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
 
  //ShiftJISからUnicodeへ変換
  MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);
 
 
  //UTF8へ変換後の文字列長を得る
  int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);
 
  //必要な分だけUTF8文字列のバッファを確保
  char* bufUTF8 = new char[lengthUTF8];
 
  //UnicodeからUTF8へ変換
  WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8, NULL, NULL);
 
  std::string strUTF8(bufUTF8);
 
  delete bufUnicode;
  delete bufUTF8;
 
  return strUTF8;
}
//非推奨関数
static std::wstring UTF8toSjis(std::wstring srcUTF8){
  //Unicodeへ変換後の文字列長を得る
  int lenghtUnicode = WideCharToMultiByte(CP_UTF8, 0, srcUTF8.c_str(),srcUTF8.size() + 1, NULL, 0,NULL,NULL);
 
  //必要な分だけUnicode文字列のバッファを確保
  char* bufUnicode = new char[lenghtUnicode];
 
  //UTF8からUnicodeへ変換
  WideCharToMultiByte(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1,bufUnicode, lenghtUnicode,NULL,NULL);
 
  //ShiftJISへ変換後の文字列長を得る
  int lengthSJis = MultiByteToWideChar(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0);
 
  //必要な分だけShiftJIS文字列のバッファを確保
  wchar_t* bufShiftJis = new wchar_t[lengthSJis];
 
  //UnicodeからShiftJISへ変換
  MultiByteToWideChar(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis);
 
  std::wstring strSJis(bufShiftJis);
 
  delete bufUnicode;
  delete bufShiftJis;
 
  return strSJis;
}
static std::wstring SjistoUTF8(std::wstring srcSjis){
  //Unicodeへ変換後の文字列長を得る
  int lenghtUnicode = (int)WideCharToMultiByte(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0,NULL,NULL);
 
  //必要な分だけUnicode文字列のバッファを確保
  char* bufUnicode = new char[lenghtUnicode];
 
  //ShiftJISからUnicodeへ変換
  WideCharToMultiByte(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode,NULL,NULL);
 
 
  //UTF8へ変換後の文字列長を得る
  int lengthUTF8 = MultiByteToWideChar(CP_UTF8, 0, bufUnicode, -1, NULL, 0);
 
  //必要な分だけUTF8文字列のバッファを確保
  wchar_t* bufUTF8 = new wchar_t[lengthUTF8];
 
  //UnicodeからUTF8へ変換
  MultiByteToWideChar(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8);
 
  std::wstring strUTF8(bufUTF8);
 
  delete bufUnicode;
  delete bufUTF8;
 
  return strUTF8;
}