#pragma once
#include <Macro.h>
#include <Windows.h>
#pragma warning(disable:4996)

//�}���`�o�C�g�����񂩂烏�C�h������
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

//�񐄏��֐�
static std::string UTF8toSjis(std::string srcUTF8){
  //Unicode�֕ϊ���̕����񒷂𓾂�
  int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(),srcUTF8.size() + 1, NULL, 0);
 
  //�K�v�ȕ�����Unicode������̃o�b�t�@���m��
  wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
 
  //UTF8����Unicode�֕ϊ�
  MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1,bufUnicode, lenghtUnicode);
 
  //ShiftJIS�֕ϊ���̕����񒷂𓾂�
  int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);
 
  //�K�v�ȕ�����ShiftJIS������̃o�b�t�@���m��
  char* bufShiftJis = new char[lengthSJis];
 
  //Unicode����ShiftJIS�֕ϊ�
  WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);
 
  std::string strSJis(bufShiftJis);
 
  delete bufUnicode;
  delete bufShiftJis;
 
  return strSJis;
}
static std::string SjistoUTF8(std::string srcSjis){
  //Unicode�֕ϊ���̕����񒷂𓾂�
  int lenghtUnicode = (int)MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);
 
  //�K�v�ȕ�����Unicode������̃o�b�t�@���m��
  wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
 
  //ShiftJIS����Unicode�֕ϊ�
  MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);
 
 
  //UTF8�֕ϊ���̕����񒷂𓾂�
  int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);
 
  //�K�v�ȕ�����UTF8������̃o�b�t�@���m��
  char* bufUTF8 = new char[lengthUTF8];
 
  //Unicode����UTF8�֕ϊ�
  WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8, NULL, NULL);
 
  std::string strUTF8(bufUTF8);
 
  delete bufUnicode;
  delete bufUTF8;
 
  return strUTF8;
}
//�񐄏��֐�
static std::wstring UTF8toSjis(std::wstring srcUTF8){
  //Unicode�֕ϊ���̕����񒷂𓾂�
  int lenghtUnicode = WideCharToMultiByte(CP_UTF8, 0, srcUTF8.c_str(),srcUTF8.size() + 1, NULL, 0,NULL,NULL);
 
  //�K�v�ȕ�����Unicode������̃o�b�t�@���m��
  char* bufUnicode = new char[lenghtUnicode];
 
  //UTF8����Unicode�֕ϊ�
  WideCharToMultiByte(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1,bufUnicode, lenghtUnicode,NULL,NULL);
 
  //ShiftJIS�֕ϊ���̕����񒷂𓾂�
  int lengthSJis = MultiByteToWideChar(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0);
 
  //�K�v�ȕ�����ShiftJIS������̃o�b�t�@���m��
  wchar_t* bufShiftJis = new wchar_t[lengthSJis];
 
  //Unicode����ShiftJIS�֕ϊ�
  MultiByteToWideChar(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis);
 
  std::wstring strSJis(bufShiftJis);
 
  delete bufUnicode;
  delete bufShiftJis;
 
  return strSJis;
}
static std::wstring SjistoUTF8(std::wstring srcSjis){
  //Unicode�֕ϊ���̕����񒷂𓾂�
  int lenghtUnicode = (int)WideCharToMultiByte(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0,NULL,NULL);
 
  //�K�v�ȕ�����Unicode������̃o�b�t�@���m��
  char* bufUnicode = new char[lenghtUnicode];
 
  //ShiftJIS����Unicode�֕ϊ�
  WideCharToMultiByte(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode,NULL,NULL);
 
 
  //UTF8�֕ϊ���̕����񒷂𓾂�
  int lengthUTF8 = MultiByteToWideChar(CP_UTF8, 0, bufUnicode, -1, NULL, 0);
 
  //�K�v�ȕ�����UTF8������̃o�b�t�@���m��
  wchar_t* bufUTF8 = new wchar_t[lengthUTF8];
 
  //Unicode����UTF8�֕ϊ�
  MultiByteToWideChar(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8);
 
  std::wstring strUTF8(bufUTF8);
 
  delete bufUnicode;
  delete bufUTF8;
 
  return strUTF8;
}