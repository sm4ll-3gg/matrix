#include "matrix.h"
#include <iostream>

int main()
{
    Matrix<int>* m;
    Matrix<int>* m1;

    try
    {
        m = new Matrix<int>(2,2);
        m1 = new Matrix<int>(2,2);
    }
    catch(MException& ex)
    {
        std::cout << "Произошла ошибка. Сообщение обработчика: "
                  << ex.get_name() << std::endl;
    }

    m1->fill();

    std::cout << "Оператор << для m1:\n" << *m1 << std::endl;

    try
    {
        std::cout << "\nВывод матрицы m1:" << std::endl;
        m1->print();
        std::cout << "\nВывод пустой матрицы m:" << std::endl;
        m->print();
    }
    catch(MException& ex)
    {
        std::cout << "Произошла ошибка. Сообщение обработчика: "
                  << ex.get_name() << std::endl;
    }

    std::cout << "\nМатрица m имеет " << m->lines() << " строк и " << m->columns() << " столбцов" <<std::endl;
    try
    {
        std::cout << "Изменение размера матрицы m:" << std::endl;
        m->resize(2,2);
        std::cout << "\nМатрица m имеет " << m->lines() << " строк и " << m->columns() << " столбцов" <<std::endl;
    }
    catch(MException& ex)
    {
        std::cout << "Произошла ошибка. Сообщение обработчика: "
                  << ex.get_name() << std::endl;
    }

    m->fill();

    try
    {
        Matrix<int> m3 = *m + *m1;
        std::cout << "\nm3 = m + m1" << std::endl;
        m3.print();
    }
    catch(MException& ex)
    {
        std::cout << "Произошла ошибка. Сообщение обработчика: "
                  << ex.get_name() << std::endl;
    }

    try
    {
        bool b = *m1 == *m;
        std::cout << "\nm1 == m = " << b <<std::endl;
    }
    catch(MException& ex)
    {
        std::cout << "Произошла ошибка. Сообщение обработчика: "
                  << ex.get_name() << std::endl;
    }

    m->transposition();
    std::cout << "Транспонированная матрица m :\n" << *m << std::endl;

    try
    {
        std::cout << "\nm.at(0,0) = " << m->at(0,0) << std::endl;
    }
    catch(MException& ex)
    {
        std::cout << "Произошла ошибка. Сообщение обработчика: "
                  << ex.get_name() << std::endl;
    }

    try
    {
        std::cout << "m.clear() и попытка её вывести: " << std::endl;
        m->clear();
        m->print();
    }
    catch(MException& ex)
    {
        std::cout << "Произошла ошибка. Сообщение обработчика: "
                  << ex.get_name() << std::endl;
    }

    try
    {
        std::cout << "Определитель матрицы m1 = " << m1->determinant() << std::endl;
    }
    catch(MException& ex)
    {
        std::cout << "Произошла ошибка. Сообщение обработчика: "
                  << ex.get_name() << std::endl;
    }

    std::cout << "\nHello World!" << std::endl;
    return 0;
}
