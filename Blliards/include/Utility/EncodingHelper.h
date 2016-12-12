#include <locale.h>
#include <stdio.h>
#include <string>
#include <wchar.h>
#include <Windows.h>
#include <memory>
#include <vector>

namespace oguna
{
	/// CP932,UTF8,UTF16�𑊌ݕϊ�����
	class EncodingConverter
	{
	protected:
		std::vector<char> buffer;

	public:
		/// �w�肵���T�C�Y�̃o�b�t�@���������R���o�[�^������������(�����l:MAX_PATH)
		EncodingConverter(int initial_buffer_size = MAX_PATH)
		{
			buffer.resize(initial_buffer_size);
		}

		/// UTF8����CP932(std::string)�֕ϊ�����
		int Utf8ToCp932(const char* src, int size, std::string *out)
		{
			std::wstring unicode;
			Utf8ToUtf16(src, size, &unicode);
			return Utf16ToCp932(unicode.data(), unicode.length(), out);
		}

		/// CP932����UTF8(std::string)�֕ϊ�����
		int Cp932ToUtf8(const char* src, int length, std::string *out)
		{
			std::wstring unicode;
			Cp932ToUtf16(src, length, &unicode);
			return Utf16ToUtf8(unicode.c_str(), unicode.length(), out);
		}

		/// CP932����UTF16(std::wstring)�֕ϊ�����
		int Cp932ToUtf16(const char *src, int length, std::wstring *out)
		{
			int size;
			size = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, length, NULL, NULL);
			buffer.resize(size * sizeof(wchar_t) * 2);
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, length, (LPWSTR) buffer.data(), buffer.size() * 2);
			out->assign((wchar_t*) buffer.data(), size);
			return size;
		}

		/// UTF16����CP932(std::string)�֕ϊ�����
		int Utf16ToCp932(const wchar_t *src, int length, std::string *out)
		{
			int size;
			size = WideCharToMultiByte(CP_ACP, NULL, src, length, NULL, NULL, NULL, NULL);
			buffer.resize(size);
			WideCharToMultiByte(CP_ACP, NULL, src, length, (LPSTR) buffer.data(), buffer.size(), NULL, NULL);
			out->assign(buffer.data(), size);
			return size;
		}

		/// UTF8����UTF16(std::wstring)�֕ϊ�����
		int Utf8ToUtf16(const char *src, int length, std::wstring *out)
		{
			int size;
			size = ::MultiByteToWideChar(CP_UTF8, 0, src, length, NULL, NULL);
			buffer.resize(size * sizeof(wchar_t));
			MultiByteToWideChar(CP_UTF8, 0, src, length, (LPWSTR) buffer.data(), buffer.size());
			out->swap(std::wstring((wchar_t*) buffer.data(), size));
			return size;
		}

		/// UTF16����UTF8(std::string)�֕ϊ�����
		int Utf16ToUtf8(const wchar_t *src, int length, std::string *out)
		{
			int size;
			size = WideCharToMultiByte(CP_UTF8, NULL, src, length, NULL, NULL, NULL, NULL);
			buffer.resize(size);
			WideCharToMultiByte(CP_UTF8, NULL, src, length, (LPSTR) buffer.data(), buffer.size(), NULL, NULL);
			out->assign(buffer.data(), size);
			return size;
		}
	};
}