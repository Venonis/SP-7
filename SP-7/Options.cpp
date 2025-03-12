// Файл Options.cpp
#include "EvryThng.h"
#include <stdarg.h>
DWORD Options(int argc, LPTSTR argv[], LPCTSTR OptStr, ...)
	/* argv - командная строка. Опции, начинающиеся с "-", помещаются в argv[1], argv[2], ...
	OptStr - текстовая строка, содержащая все возможные опции во взаимно-однозначном 
	соответствии с адресом булевых переменных в списке параметров-переменных (...). 
	Эти флаги устанавливаются тогда и только тогда, когда символ соответствующей опции встречается в argv[1], argv[2], ... 
	Возвращаемое значение - индекс в argv первого параметра, не принадлежащего к опциям.  */
{
	va_list pFlagList;
	LPBOOL pFlag;
	int iFlag = 0, iArg;
	va_start(pFlagList, OptStr);
	while ((pFlag = va_arg(pFlagList, LPBOOL)) != NULL
		&& iFlag < (int)_tcslen(OptStr))
	{
		*pFlag = FALSE;
		for (iArg = 1;
			!(*pFlag) && iArg < argc && argv[iArg][0] == '-';
			iArg++)
			*pFlag = _memtchr(argv[iArg], OptStr[iFlag],
				_tcslen(argv[iArg])) != NULL;
		iFlag++;
	}
	va_end(pFlagList);
	for (iArg = 1; iArg < argc && argv[iArg][0] == '-';
		iArg++);
	return iArg;
}