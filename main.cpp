/* 
 * File:   main.cpp
 * Author: Stan
 *
 * Created on 12 Август 2020 г., 22:38
 */

#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <conio.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <psapi.h>
#include <cctype>

using namespace std;

/*
 * 
 */
//Функция вывода рядов из двумерного массива по первой букве.
//&sm-указатель на входной поток sm, который затем преобразуется в массив.
//s1- строки, s2-столбцы. 
//sym-начальный символ слова (первая буква).
//dl- разделитель (по умолчанию пробел).
void FirstLetter2(stringstream &sm,int s1,int s2, char sym, char dl=' '){

string mas[s1][s2];//Массив, в который переписывается поток sm.
string dc;//Разделитель при показе массива mas[s1][s2].
int qfl=0;//Счётчик выводимых процессов.

//Набор в массив mas из потока sm
for(int i=0;i<s1;i++){
     for(int j=0;j<s2;j++){
     getline(sm,mas[i][j],dl);
     }
}  
   
//Цикл поиска cтрок на с первым элементом на букву sym
for(int i=0;i<s1;i++)
 {
    if (mas[i][0][0]==sym){
    
    for(int j=0;j<s2;j++)
    {
     if (j==s2-1) dc="\n";//Если строка подходит к концу, dc принимает
     else dc=" ";//значение символа перевода строки, чтобы массив выводился в
     //на экран в виде таблицы, если нет то разделитель- пробел.
     cout<<mas[i][j]<<dc;
    }
    qfl++;
    }
 } 
cout<<" "<<endl;
cout<<"Process count:"<<" "<<qfl<<endl;
}
//------------------------------------------------------------------------------
//Функция, сортирующая строки по алфавиту (для двумерных массивов).
//&sm-указатель на входной поток sm, который затем преобразуется в массив.
//s1- строки, s2-столбцы.
//col- столбец, где находится элемент,по первой букве которого нужно сортировать
//strord- порядок сортировки прямой(a-z) или обратный(z-a).
//dl- разделитель в массиве (по умолчанию пробел).

//Краткое пояснение по принципу действия функции:
//В данной функции происходит сортировка строк в двумерном массиве в прямом и
//обратном порядке по первой букве элемента в заданном столбце col. Значения 
//строк в массиве перетасовываются между собой в заданном порядке.
//Происходит циклический перебор массива с целью нахождения максимального или 
//минимального ASCII-значения первого символа в элементе. Если есть строка со 
//значением больше или меньше текущего минимума или максимума, то она принимает
//значение первой. И так пока не будет выбрана первая строка. Эта строка 
//меняется местами c текущей первой строкой в массиве, занимает первое место
//и исключается из выборки. Следующая выбранная строка займёт место, следующее 
//за ней и так далее.
void StringSort2(stringstream &sm,int s1, int s2, int col, 
string strord, char dl=' ')
{
 string mas[s1][s2];//Массив, в который переписывается поток sm.
 int qs=0;//Счётчик выводимых процессов.
 
 //Набор в массив mas из потока sm
 for(int i=0;i<s1;i++){
     for(int j=0;j<s2;j++){
     getline(sm,mas[i][j],dl);
     }
 }   
    
    
 int first=0;//Значение самого первого в списке элемента из оставшейся выборки
 //по умолчанию.
 //Самое большое или маленькое ASCII-значение первой буквы слова в зависимости
 //от заданного порядка strord.
 stringstream fst;//Поток, в который записываются все элементы первой строки.
 string dc;//Разделитель при показе массива mas[s1][s2].
 int max=0;//Максимум при прямом порядке.
 int pos=0;//Позиция. Номер строки, на которой был найден самый первый
 //элемент из оставшейся выборки
 bool order;//Логическая (булева) переменная порядка.
       
 //Условие определения порядка по переменной strord.   
    if (strord=="a-z") 
    {    
    order=true;//Прямой порядок.
    //Цикл определения максимальной по ASCII-значению первой буквы, чтобы, 
    //отталкиваясь от неё определить, минимальную, строка с которой помещается
    //первое место в оставшейся выборке.
    for(int i=0;i<s1;i++)
      {
       if ((int)mas[i][col][0]>max) max=(int)mas[i][col][0];
      }
    first=max;
    }
    else if (strord=="z-a") order=false;//Обратный порядок.
    
    //Если значение строки порядка некорректно, то весь код "перепрыгивается".
    //Происходит переход в самый конец.
    else{ 
        cout<<"Wrong order value"<<endl;
        goto lexit;
    }

    //Основной цикл распределения строк по порядку.
    for(int i=0;i<s1;i++)//Общий цикл перебора по всем строкам.
    {    
    
    for(int j=i;j<s1;j++)//Цикл перебора строк, оставшихся в выборке.
    //По мере прохождения самого внешнего цикла i выборка сжимается, поэтому
    //начальное значение j равно текущему значению i.    
    {
        //Определение дальнейших действий в зависимости от заданного порядка
        //сортировки.
        switch(order)
        {    
         
         case true:
     //Условие для определения наименьшего ASCII-значения первой буквы
     //элемента в заданном столбце и строки с ним.
     if ((int)mas[j][col][0]<=first) 
     {
         fst.str("");//Очищение потока для первой строки в оставшейся выборке.
         first=(int)mas[j][col][0];//Определение наименьшего ASCII-значения.
         
         //Запись найденной строки с наименьшим ASCII-значением первой буквы
         //в заданном столбце в поток для первой строки в текущей оставшейся
         //выборке. Перебор от начала до конца строки
         for (int k=0;k<s2;k++){
         fst<<mas[j][k]<<dl;             
         }

         pos=j;//Фиксация позиции строки, на которой было найдено наименьшее 
         //ASCII-значение первой буквы элемента в заданном столбце.
     }      
        break;
        
        case false:
     //Условие для определения наибольшего ASCII-значения первой буквы
     //элемента в заданном столбце и строки с ним.       
     if ((int)mas[j][col][0]>=first) 
     {
         fst.str("");//Очищение потока для первой строки в оставшейся выборке.
         first=(int)mas[j][col][0];//Определение наибольшего ASCII-значения.
         
         //Запись найденной строки с наибольшим ASCII-значением первой буквы
         //в заданном столбце в поток для первой строки в текущей оставшейся
         //выборке. Перебор от начала до конца строки
         for (int k=0;k<s2;k++){
         fst<<mas[j][k]<<dl;            
         }
         
         pos=j;//Фиксация позиции строки, на которой было найдено наибольшее 
         //ASCII-значение первой буквы элемента в заданном столбце.
     }
        break;
        
        }
            
    }
    
    //Строка под номером i и строка под номером pos меняются местами в массиве
    //с помощью потоков. На текущее место i в массиве становится текущая
    //первая строка из оставшейся выборки под номером pos, а на место pos
    //становится строка, которая в данный момент была под номером i.
    //Таким образом из первых строк из каждого оборота j-цикла формируется новый 
    //порядок элементов в массиве.
    for (int k=0;k<s2;k++){
    mas[pos][k]=mas[i][k];
    getline(fst,mas[i][k],dl);
    }
    
    //Сброс значения ASCII для первой строки (first) на значение по умолчанию
    //перед новым оборотом i-цикла.
    switch (order)
    {
    case false://Для обратного порядка.
    first=0;
    break;
    
    case true://Для прямого.
    first=max;
    break;
    }
    
    
    }
    
 //Вывод перераспределённого массива на экран.
 for(int i=0;i<s1;i++)
 {
    for(int j=0;j<s2;j++)
    {
     if (j==s2-1) dc="\n";//Если строка подходит к концу, dc принимает
     else dc=" ";//значение символа перевода строки, чтобы массив выводился в
     //на экран в виде таблицы, если нет то разделитель- пробел.
     cout<<mas[i][j]<<dc;
    }
 qs++;
 } 
 
cout<<" "<<endl;
cout<<"Process count:"<<" "<<qs<<endl;
lexit:cout<<""<<endl;
           
}

//------------------------------------------------------------------------------
//Стандартная функция вывода списка в массиве.
//&sm-указатель на входной поток sm, который затем преобразуется в массив.
//s1- строки, s2-столбцы.
//dl- разделитель в массиве (по умолчанию пробел).
void List(stringstream &sm,int s1, int s2, char dl=' '){

string mas[s1][s2];//Массив, в который переписывается поток sm.
string dc;//Разделитель при показе массива mas[s1][s2].
int ql=0;//Счётчик выводимых процессов.

//Набор в массив mas из потока sm
for(int i=0;i<s1;i++){
     for(int j=0;j<s2;j++){
     getline(sm,mas[i][j],dl);
     }
}  

//Вывод массива на экран.
for(int i=0;i<s1;i++)
 {
    for(int j=0;j<s2;j++)
    {
     if (j==s2-1) dc="\n";//Если строка подходит к концу, dc принимает
     else dc=" ";//значение символа перевода строки, чтобы массив выводился в
     //на экран в виде таблицы, если нет то разделитель- пробел.
     cout<<mas[i][j]<<dc;
    }
 ql++;
 } 
cout<<" "<<endl;
cout<<"Process count:"<<" "<<ql<<endl;    
}
//------------------------------------------------------------------------------
//Функция, проверяющая, является ли её аргумент (строка) десятичным числом.
bool IsNumeric10(string str)
{
    if (str=="") return false;//Не является 10чным числом, если входная строка
    //str пустая.
    
    int ncount=0;//Счётчик численных символов
    for(int i=0;i<=str.length();i++)
    {
        if ((str[i]>=48)&&(str[i]<=57)) ncount++;//Номера десятичных численных
        //символов находятся в диапазоне от 48 до 57.
    }
    if (ncount==str.length()) return true;//Если количество численных символов
    //равно общему числу символов (длине строки), то строка- число.
    else return false;//Если нет, то строка числом не является.
}

//------------------------------------------------------------------------------

//Функция, возвр. слово под номером z из строки x с разделителем y
string SplitFind(string x, char y, int z)
{
    int dcount=0;//Счётчик разделителей

    for(int i=0;i<=x.length();i++)
    {
       if (x[i]==y) dcount++;//Когда символ i из строки х равен y 
       //счётчик прибавляется
    }

    
    if (z>dcount) return "";//Выход из функции в случае слишком большого
    //номера слова
    
    vector <string> vx;//Объявление строкового вектора слов vx 
    
    istringstream iss(x);//Объявление строкового потока iss с входной 
    //строкой x в качестве аргумента
    while(getline(iss,x,y))//Цикл, перебирающий и делящий строку х на слова
    //с разделителем y    
    {
        vx.push_back(x);//Добавление слов из строки x в вектор vx 
    }  
    return vx[z];//Возвращаемое функцией значение в виде слова
    //(элемента вектора vx с номером z)
 }

//------------------------------------------------------------------------------

//Функция, выводящая список процессов на экран
int ProcessList(HANDLE h,PROCESSENTRY32 p32, string p=""){
int q=1;//Счётчик процессов
stringstream s1;
        if (Process32First(h,&p32))//Если аргументы функции Process32First 
        //(функция первого пункта списка процессов)- h (снимок списка) и p32 
        //(пункт из этого списка)
        {
            s1<<p32.szExeFile << "|" << p32.th32ProcessID <<"|" ;//Вывести 
            //первый процесс. Его файл .exe и ID процесса            
            
            while (Process32Next(h, &p32)) //Перебор списка из снимка h, пока 
            //в нём есть пункты p32. Process32Next- функция следующего пункта 
            //списка процессов. Аргументы- конкретный снимок списка (h),
            //конкретный пункт из этого списка (указатель на этот пункт- &p32)
            {
            s1<< p32.szExeFile << "|" << p32.th32ProcessID <<"|";
            //Вывод .exe и ID процессов
            q++;//Счётчик процессов
            }
        }
        else cout<<"no arguments"<<endl;//Проверочное условие
        //Проверка на правильность Process32First
    
    cout<<" "<<endl;
    
    //Условия для сортировки вывода процессов.
    if (p=="") List(s1,q,2,'|');//Запись процессов в массив и вывод в порядке
    //по умолчанию.
    else if ((p=="a-z")||(p=="z-a")) StringSort2(s1,q,2,0,p,'|');
    //Алфавитный вывод массива процессов в прямом (a-z) и обратном (z-a) порядке
    else if (p.length()==1) FirstLetter2(s1,q,2,p[0],'|');
    //Вывод элементов массива процессов на экран по заданной первой букве.
    cout<<" "<<endl;
    
    return 0;    
    
}

//------------------------------------------------------------------------------

//Функция поиска строки
bool StrSearch(string str,string sstr, int n=0)
{
 int pos;
 int q; 
 pos=sstr.find(str,n);   
 if (pos!=-1) return true;
}

//------------------------------------------------------------------------------

//Функция поиска процессов
int Find(HANDLE h, PROCESSENTRY32 p32, string str, string p=""){

DWORD dstr;//Переменная типа DWORD на случай, если аргумент (строка str)
   //окажется числом.
char chstr[str.length()];//массив типа char, исп. если str- число 
int q=0;//Счётчик процесов

            switch(IsNumeric10(str))//Является ли str числом?
            {
                case true: //Если да, то
                for(int i=0; i<=str.length();i++)
                {
                 chstr[i]=str[i];//в массив типа char chstr набираются
                 //символы строки str
                }//(преобразование str в chstr)
                dstr=strtol(chstr,0,10);//и затем этот массив преобразуется
                //в DWORD-число dstr с помощью функции strtol()
                                
                while (Process32Next(h, &p32))//До тех пор, пока HANDLE со
                //снимком списка процессов h и указатель на пункт этого списка
                //&p32 являются аргументами функции Process32Next,    
                {    
                if (dstr==p32.th32ProcessID)//Выполняется условие- если dstr
                //равно ID-номеру процесса из пункта p32, 
                {
        cout<<p32.szExeFile << " " << p32.th32ProcessID << endl;//то exe-файл
        //и ID процесса из этого пункта выводятся на экран,
        q++;//и счётчик процессов прибавляется на 1.        
                }
                }
                break; 
                
                case false://Если нет, то
                while (Process32Next(h, &p32))//До тех пор, пока HANDLE со
                //снимком списка процессов h и указатель на пункт этого списка
                //&p32 являются аргументами функции Process32Next,
                {
                if (str==p32.szExeFile)//Выполняется условие- если str
                //равно имени exe-файла из пункта p32, 
                {
        cout<<p32.szExeFile << " " << p32.th32ProcessID << endl;//то exe-файл
        //и ID процесса из этого пункта выводятся на экран, 
        q++;//и счётчик процессов прибавляется на 1.
                }
                
                //Произвольный поиск
                if (p=="-f") 
                   {
                    if (StrSearch(str,p32.szExeFile)==true) 
                    {   
                     cout<<p32.szExeFile << " " << p32.th32ProcessID << endl;
                    //exe-файл и ID процесса из этого пункта выводятся на экран,
                     q++;//и счётчик процессов прибавляется на 1.
                    }
                   }
                }
                
                cout<<" "<<endl;
                cout<<"Process count:"<<" "<<q<<endl;//Вывод числа процессов.
                break;    
            }
   
  if (q==0) cout<<"Process not found"<<endl;//Если счётчик равен нулю, то
 //процесс не найден.

}

//------------------------------------------------------------------------------

//Функция завершения процессов
int Kill(HANDLE h, PROCESSENTRY32 p32, string str){

DWORD dstr;//Переменная типа DWORD на случай, если аргумент (строка str)
   //окажется числом.
char chstr[str.length()];//массив типа char, исп. если str- число 
HANDLE kill;//Объект типа HANDLE, отвечающий за само завершение процесса.
   //туда записываются данные, полученные функция OpenProcess(), и затем он
   //используется в качестве аргумента функции TerminateProcess().
int q=0;//Счётчик процесов

            switch(IsNumeric10(str))//Является ли str числом?
            {
                case true://Если да, то
                for(int i=0; i<=str.length();i++)
                {
                 chstr[i]=str[i];//в массив типа char chstr набираются
                 //символы строки str
                }//(преобразование str в chstr)
                dstr=strtol(chstr,0,10);//и затем этот массив преобразуется
                //в DWORD-число dstr с помощью функции strtol()    
                
                while (Process32Next(h, &p32))//До тех пор, пока HANDLE со
                //снимком списка процессов h и указатель на пункт этого списка
                //&p32 являются аргументами функции Process32Next, 
                {    
                if (dstr==p32.th32ProcessID)//Выполняется условие- если dstr
                //равно ID-номеру процесса из пункта p32, 
                {
        q++;//то счётчик процессов прибавляется на 1,        
        kill=OpenProcess(PROCESS_ALL_ACCESS, TRUE, p32.th32ProcessID);
        //В объект типа HANDLE kill записывается результат функции OpenProcess()
        //в виде процесса с его ID-номером и правами полного доступа на него
        //(захват процесса).
        TerminateProcess(kill, 0);//Непосредственно само завершение полученного 
        //процесса.
        CloseHandle(kill);//Закрытие handle (с захваченным процессом).
        //(Проверить работу.)
                }
                }
                break; 
                
                case false://Если нет, то
                while (Process32Next(h, &p32))//До тех пор, пока HANDLE со
                //снимком списка процессов h и указатель на пункт этого списка
                //&p32 являются аргументами функции Process32Next,
                {
                if (str==p32.szExeFile)//Выполняется условие- если str
                //равно имени exe-файла из пункта p32, 
                {
        q++;//то счётчик процессов прибавляется на 1,
        kill=OpenProcess(PROCESS_ALL_ACCESS, TRUE, p32.th32ProcessID);
        //В объект типа HANDLE kill записывается результат функции OpenProcess()
        //в виде процесса с его ID-номером и правами полного доступа на него.
        //(захват процесса).
        TerminateProcess(kill, 0);//Непосредственно само завершение полученного 
        //процесса.
        CloseHandle(kill);//Закрытие handle (с захваченным процессом).
        //(Проверить работу.)
                }
                }
                break;    
            }
    
 if (q==0)//Если счётчик равен нулю, процесс не найден.
 {
     cout<<" "<<endl;
     cout<<"Process not found"<<endl;
 }   

}

//------------------------------------------------------------------------------

//Общая функция обработки процессов
int Process(string str){

   DWORD dstr;//Переменная типа DWORD на случай, если аргумент (строка str)
   //окажется числом.
   char chstr[str.length()];//массив типа char, исп. если str- число 
   //int q=0;//Счётчик процесов
   HANDLE h;//Объявление абстрактного объекта типа HANDLE
   //HANDLE- дескриптор объекта
   h=CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);//Присвоение объекту h 
   //значения снимка списка процессов, полученного в функции 
   //CreateToolhelp32Snapshot с аргументами TH32CS_SNAPALL и 0
   //(вывести все процессы)
   PROCESSENTRY32 p32;//Объявление объекта типа PROCESSENTRY32- пункт списка 
   //процессов
   p32.dwSize = sizeof( PROCESSENTRY32 );//Выделение размера (памяти) 
   //для пункта процесса p32 и приравнивание его к стандартному размеру пункта 
   //процесса- обязательно сделать, иначе не будет работать Process32First 
   //(см. microsoft.com) или будет работать, но с возможностью вывести список 
   //только один раз, в дальнейшем будет выводиться пустота, потому, что 
   //аргументы h и &p32 перестанут передаваться в функцию Process32First
 
if (h!=NULL)//Если список процессов не пуст
    {
        
   	//Вызовы функций при вводе тех или иных команд, работающих с процессами.
        //0- первое слово (команда), 1- второе слово (объект команды).
        if (SplitFind(str,' ',0)=="spl") {
            ProcessList(h, p32,SplitFind(str,' ',1));
        }
   	else if (SplitFind(str,' ',0)=="find") 
        Find(h,p32,SplitFind(str,' ',1),SplitFind(str,' ',2));
   	else if (SplitFind(str,' ',0)=="kill") 
        Kill(h,p32,SplitFind(str,' ',1));
   		
        
    }

    CloseHandle(h);//Закрытие объекта HANDLE со снимком всех процессов.
    //Очистка памяти.

}
//------------------------------------------------------------------------------
void Snap(stringstream *ps, int *q){
 HANDLE h;//Объявление абстрактного объекта типа HANDLE
   //HANDLE- дескриптор объекта
HANDLE hp=0;//Handle для пути к файлу
char Path[MAX_PATH]={0};//char-массив для пути к файлу
string p;//строка для пути к файлу
   h=CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);//Присвоение объекту h 
   //значения снимка списка процессов, полученного в функции 
   //CreateToolhelp32Snapshot с аргументами TH32CS_SNAPALL и 0
   //(вывести все процессы)
   PROCESSENTRY32 p32;//Объявление объекта типа PROCESSENTRY32- пункт списка 
   //процессов
   p32.dwSize = sizeof( PROCESSENTRY32 );//Выделение размера (памяти) 
   //для пункта процесса p32 и приравнивание его к стандартному размеру пункта 
   //процесса- обязательно сделать, иначе не будет работать Process32First 
   //(см. microsoft.com) или будет работать, но с возможностью вывести список 
   //только один раз, в дальнейшем будет выводиться пустота, потому, что 
   //аргументы h и &p32 перестанут передаваться в функцию Process32First
   int e;//Значение ошибки
         
   if (h!=NULL)//Если список процессов не пуст
    {
        if (Process32First(h,&p32))//Если аргументы функции Process32First 
        //(функция первого пункта списка процессов)- h (снимок списка) и p32 
        //(пункт из этого списка)
        {
           hp=OpenProcess(PROCESS_ALL_ACCESS,false,p32.th32ProcessID);//Запись
           //пути в hp по ID.
           e=GetLastError();
           GetModuleFileNameEx(hp,NULL,Path,256);//Перепись пути из hp в Path.
           p=Path;//Перепись пути из Path в p.
           if(e==5) p="Access denied";
           *ps<<p32.szExeFile<<"|"<<to_string(p32.th32ProcessID)<<"|"<<p<<"|";
           *q+=1; 
            while (Process32Next(h, &p32)) //Перебор списка из снимка h, пока 
            //в нём есть пункты p32. Process32Next- функция следующего пункта 
            //списка процессов. Аргументы- конкретный снимок списка (h),
            //конкретный пункт из этого списка (указатель на этот пункт- &p32)
            {
           hp=OpenProcess(PROCESS_ALL_ACCESS,false,p32.th32ProcessID);//Запись
           //пути в hp по ID.
            e=GetLastError();
           GetModuleFileNameEx(hp,NULL,Path,256);//Перепись пути из hp в Path.
           p=Path;//Перепись пути из Path в p.
           if(e==5) p="Access denied";
           *ps<<p32.szExeFile<<"|"<<to_string(p32.th32ProcessID)<<"|"<<p<<"|";
            //Своеобразные отношения указателей с оператором ++
           *q+=1;
           //Вывод .exe и ID процессов
           }
        }
        else cout<<"no arguments"<<endl;//Проверочное условие
        //Проверка на правильность Process32First
       cout<<" "<<endl;
      }
   CloseHandle(h);//Закрытие объекта HANDLE со снимком всех процессов.
    //Очистка памяти.
   CloseHandle(hp);//Закрытие объекта HANDLE с "пойманным" для определения пути 
   //процессом.
    //Очистка памяти.
}

//------------------------------------------------------------------------------

//Функция вывода справки.
string Help(){
    cout<<" "<<endl;
    cout<<"help - list all commands and their descriptions."<<endl;
    cout<<" "<<endl;
    cout<<"spl - show process list."<<endl;
    cout<<"Syntax: spl [parameter]"<<endl;
    cout<<"List of parameters:"<<endl;
    cout<<"no parameter - show process list in the default order."<<endl;
    cout<<"a-z - show process list in alphabet order."" "
    "Capital letters first."<<endl;
    cout<<"z-a - show process list in reverse alphabet order."" "
    "Capital letters last."<<endl;
    cout<<"spl [first letter] - show processes beginning with the symbol"" "
    "stated as first letter."<<endl;
    cout<<" "<<endl;
    cout<<"find - find the process by ID or .exe-file name."<<endl;
    cout<<"Syntax: find [filename].exe, find [Process ID number]," " "
    "find [argument] [parameter]."<<endl;
    cout<<"List of parameters:"<<endl;
    cout<<"-f - find by amount of symbols in process filename"<<endl;
    cout<<" "<<endl;
    cout<<"kill - terminate the process by ID or .exe-file name."<<endl;
    cout<<"Syntax: kill [filename].exe, kill [Process ID number] " <<endl;
    cout<<" "<<endl;
    cout<<"snap- takes a snapshot of the current process list"<<endl;
    cout<<"Syntax: snap(without any argument)- takes a snapshot of the current "
    "process list."<<endl;
    cout<<"snap view- shows the process list in the snapshot"<<endl;
    cout<<"snap record [filepath]- records the process list in the snapshot "
    "into a file"<<endl;
    cout<<"snap read [filepath]- reads the process list in the specified file "
    "to the snapshot"<<endl;
    cout<<"start [[file].exe or [process ID]]- starts the exe file of the "
    "process saved in the snapshot"<<endl;
    cout<<"exit - exit program"<<endl;
    cout<<" "<<endl;
    cout<<"cls - clear screen"<<endl;
    return "";
    }

//------------------------------------------------------------------------------

//Основная функция
int main() {

string c;//Команда
stringstream ps;//Поток для процессов
int pc=0;//Число процессов
string start;//Переменная для запуска

lcls:
cout<<"MyOwnProcessExplorer 1.6.0 2020 Maxim Badaev"<<endl;
//Наименование, версия, год и автор программы.
cout<<" "<<endl;//Разделительная пустая строка.
cout<<"Type 'help' to view all the commands and their descriptions"<<endl;
cout<<" "<<endl;

//Получение потока со снимком процессов.
SnapArray://Обычный захват массива
Snap(&ps,&pc);
ReadSnapArray://Захват массива при чтении из файла
//Объявление двумерного динамического массива снимка процессов
string **snap=new string* [pc];
for(int i=0;i<pc;i++){
    snap[i]=new string[3];
}

//Считывание содержимого потока в динамический массив
for(int i=0;i<pc;i++){
    for(int j=0;j<3;j++){
       getline (ps,snap[i][j],'|');
    }
}


lencom://Метка для возврата к запросу ввода команды.
cout<<" "<<endl;//Разделительная пустая строка.
cout<<"Enter command:"<<endl;
getline(cin, c);//Ввод команды из нескольких слов с пробелами


if ((SplitFind(c,' ',0)=="spl")||(SplitFind(c,' ',0)=="find")||
(SplitFind(c,' ',0)=="kill"))
//Если выполняется одно из условий:
//Значение строки команды c равно spl или первое слово в ней равно find 
//или kill, то
{
    cout<<" "<<endl;//Разделительная пустая строка.
    Process(c);//Строка с командой и её объектом передаются в функцию 
    //обработки процессов 
    goto lencom;//Переход к запросу ввода команды.
}
else if (c=="help")//Если c равно help,
{    
    cout<<Help()<<endl;//то выполняется функция вывода справки.
    goto lencom;//Переход к запросу ввода команды. 
}
else if (SplitFind(c,' ',0)=="snap")//Работа со снимками процессов
{
 if(SplitFind(c,' ',1)==""){//Получение нового снимка 
 //Сброс параметров:    
  ps.str("");//Очистка потока с процессами
  pc=0;//Сброс счётчика процессов до начального значения  
  //Удаление прошлого динамического массива
  for(int i=0;i<pc;i++){
      delete [] snap[i];
  }
  cout<<" "<<endl;
  cout<<"Snap performed"<<endl;
  cout<<" "<<endl;
  goto SnapArray;//Переход к получению нового снимка процессов
  }
 else if(SplitFind(c,' ',1)=="view"){//Просмотр снимка процессов
   //Вывод массива на экран
  for(int i=0;i<pc;i++){    
   cout<<snap[i][0]<<" "<<snap[i][1]<<" "<<snap[i][2]<<endl;    
  }

  cout<<" "<<endl;
  cout<<pc<<endl;  
   }
 else if(SplitFind(c,' ',1)=="record"){//Запись снимка процессов в файл
      ofstream r(SplitFind(c,' ',2).c_str());
      char *d=new char;//Разделитель
      *d='|';
      if (SplitFind(c,' ',2).find(".csv",(SplitFind(c,' ',2).length()-4))!=-1) 
          *d=';';/*Определитель csv для распределения значений по полям*/
      for(int i=0;i<pc;i++){
          r<<snap[i][0]<<*d<<snap[i][1]<<*d<<snap[i][2]<<*d<<endl;
      } 
      r.close();
      delete d;
   }
 else if(SplitFind(c,' ',1)=="read"){//Чтение нового снимка процессов из файла
   ps.str("");//Очистка потока с процессами
   pc=0;//Сброс счётчика процессов до начального значения
   //Удаление прошлого динамического массива
   for(int i=0;i<pc;i++){
      delete [] snap[i];
   }
   ifstream in(SplitFind(c,' ',2).c_str());
   string cts;
   while (getline(in,cts)){
       pc++;
       ps<<cts;
   }
   in.close();
   cout<<pc<<" "<<"processes read"<<endl;
   cout<<"ReadSnap performed"<<endl;
   goto ReadSnapArray;
   }
 goto lencom;
}
//Запуск по пути из массива снимка.
else if(SplitFind(c,' ',0)=="start"){
    for(int i=0;i<pc;i++){
       if ((SplitFind(c,' ',1)==snap[i][0])||(SplitFind(c,' ',1)==snap[i][1])){
        if (snap[i][2]=="Access denied"){
            cout<<"Access denied"<<endl;
            break;
        }
        start+=34;//Особые взаимоотношения с кавычками
        start+=snap[i][2];//Запись пути в переменную для запуска
        start+=34;//Особые взаимоотношения с кавычками
        cout<<start<<endl;
        
//        system(start.c_str());//Запуск
        
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        TCHAR czCommandLine[start.length()];
        for(int i=0;i<=start.length();i++){
        czCommandLine[i]=start[i];    
        }
        CreateProcess(NULL, czCommandLine, NULL, NULL,
        FALSE, 0, NULL, NULL, &si, &pi);
        
        start="";
        break;
        }
       
    } 
 goto lencom;//Переход к запросу ввода команды.    
}
else if (c=="exit")//Команда выхода.
{
    exit(0);
    goto lencom;//Переход к запросу ввода команды.
}
else if (c=="cls")//Очистка экрана.
{
    system("cls");
    goto lcls;//Переход к запросу ввода команды.
}

else//Если вводится что-либо иное,
{
    cout<<" "<<endl;
    cout<<"Incorrect command"<<endl;//то выводится ссобщение
    //об ошибке ввода.
    goto lencom;//Переход к запросу ввода команды.
}

getch();
return 0;

}

