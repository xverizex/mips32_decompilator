mips 32 дизассемблер. пока в разработке. хочется сделать из него потом декомпилятор, но не знаю, получиться ли. Я вдохновился программой radare2, но мне не хватает некоторых функций в radare2. хотя я не знаю как в совершенстве пользоваться radare2, и может быть там есть такой функционал.

Моя программа находится на начальной стадии разработки, но уже можно дизассемблировать. Правда программа пока не все команды распознает.

Функционал:
* pd [lines] - дизассемблировать несколько строк. 
* pf - показать список функций, которые есть в программе.
* pfn [name of function] - показать все функции в этой программе.
* pfa [address] - показать адреса функций, которые ссылаются на этот адрес ( адрес функции ) и отобразить в какой функции это происходит.
* pff [address of function] - показать все функции, которые должны выполняться по этому адресу.
* pfaf [address] - узнать к какой функции принадлежит адрес.
* s [address] - перейти по адресу. адрес должен быть шестнадцатеричным числом. например 0x4046fff.
