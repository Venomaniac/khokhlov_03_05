using namespace std;

#include <iostream>
#include <Windows.h>

//Глобальный дескриптор для доступа из обработчика
HANDLE hSemaphore = NULL;

//Обработчик закрытия консоли
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (hSemaphore != NULL) {
        ReleaseSemaphore(hSemaphore, 1, NULL); //Освобождаем семафор
        CloseHandle(hSemaphore);
        hSemaphore = NULL;
    }
    return FALSE; //Передаём управление стандартному обработчику
}

int main()
{
    setlocale(LC_ALL, "Russian");
    //Регистрируем обработчик события
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
    const wchar_t* semaphoreName = L"Global\\khohklov_01_05";
    hSemaphore = CreateSemaphoreW(NULL, 2, 2, semaphoreName);
    if (hSemaphore == NULL)
    {
        wcerr << L"Ошибка создания семафора. Код: " << GetLastError << endl;
        return 1;
    }
    DWORD waitResult = WaitForSingleObject(hSemaphore, 0);
    if (waitResult == WAIT_OBJECT_0)
    {
        cout << "Программа успешно запущено" << endl;
        cout << "Нажмите Enter для завершения";
        cin.get();
        ReleaseSemaphore(hSemaphore, 1, NULL);
    }
    else if (waitResult == WAIT_TIMEOUT)
    {
        //Автоматическое закрытие программы
        cout << "Превышено допустимое количество копий" << endl;
        cout << "Завершение работы";
    }
    else
    {
        cerr << "Ошибка ожидание. Код: " << GetLastError() << endl;
    }
    CloseHandle(hSemaphore);
    hSemaphore = NULL;
    return 0;
}