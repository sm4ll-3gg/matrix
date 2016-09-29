#ifndef MATRIX_H
#define MATRIX_H

#include "mexception.h"
#include <iostream>
#include <math.h>

template<typename T>
class Matrix
{
public:
    T** matrix;
    size_t _lines;
    size_t _columns;

    T                       calculate_det(T**, size_t);
    T**                     cut(T**, size_t, size_t) throw(MException);

    void                    qsort(T*, int, int);
    T                       partition(T*, int, int);
public:
    explicit Matrix(size_t = 0, size_t = 0) throw(MException);
    Matrix(const Matrix&) throw(MException);
    ~Matrix();

    void                    fill();
    void                    print() throw(MException);

    size_t                  lines();
    size_t                  columns();

    void                    resize(size_t,size_t) throw(MException);

    void                    sort();

    T&                      at(const size_t, const size_t) throw(MException);

    void                    clear() throw(MException);

    void                    transposition();
    T                       determinant() throw(MException);

    T*                      operator [] (const size_t);

    Matrix<T>               operator + (Matrix<T>&) throw(MException);
    Matrix<T>               operator - (Matrix<T>&) throw(MException);

    bool                    operator == (Matrix<T>&) throw(MException);
    bool                    operator != (Matrix<T>&) throw(MException);

    Matrix<T>               operator = (Matrix<T>&) throw(MException);

    template<typename T1>
    friend std::ostream&    operator << (std::ostream&,Matrix<T1>&);

    template<typename T1>
    friend std::istream&    operator >> (std::istream&,Matrix<T1>&);
};

template<typename T>
T** Matrix<T>::cut(T ** m, size_t size, size_t c) throw(MException) // с - столбец, который мы вычёркиваем
{
    try
    {
        T** cut_m=new T*[size-1];
        for(size_t i = 0; i < size-1; i++)
        {
            cut_m[i] = new T[size-1];
        }

        for(size_t i = 1; i < size; i++)
        {
            for(size_t j = 0, t=0; j < size; j++, t++)
            {
                if(j != c)
                {
                    cut_m[i-1][t] = m[i][j];
                }
                else
                {
                    t--;
                }
            }
        }

        return cut_m;
    }
    catch(std::bad_alloc)
    {
        throw("Bad alloc");
    }
}

template<typename T>
T Matrix<T>::calculate_det(T** m, size_t size)
{
    if(size == 2)
    {
        return m[0][0]*m[1][1] - m[0][1]*m[1][0]; // Произведение элементов на главной диаганали - произведение элементов на побочной
    }
    else
    {
        int result = 0;
        for(size_t i = 0; i < size; i++)
        {
            if(m[0][i] != 0)
            {
                result += pow(-1,i+2)*m[0][i]*calculate_det(cut(m,size,i),size-1);
            }
        }

        for(size_t i = 0; i < size; i++)
        {
            delete[] m;
        }
        m = nullptr;

        return result;
    }
}

template<typename T>
void Matrix<T>::qsort(T* m,int begin, int end)
{
    if(begin < end)
    {
        int q = partition(m,begin,end);
        qsort(m,begin,q-1);
        qsort(m,q+1,end);
    }
}

template<typename T>
T Matrix<T>::partition(T* m,int begin, int end)
{
    T sup = m[end];
    int i = begin - 1;

    for(int j = begin; j < end; j++)
    {
        if(m[j] <= sup)
        {
            i++;

            T x = m[i];
            m[i] = m[j];
            m[j] = x;
        }
    }

    T x = m[i+1];
    m[i+1] = m[end];
    m[end] = x;

    return i+1;
}

template<typename T>
Matrix<T>::Matrix(size_t __lines, size_t __columns) throw(MException)
    :_lines(__lines), _columns(__columns)
{
    if(__lines < 0 || __columns < 0)
    {
        throw MException("Incorrect size of matrix");
    }
    try
    {
        matrix = new T*[__lines];
        for(size_t i = 0; i < __lines; i++)
        {
            matrix[i] = new T[__columns];

            for(size_t j = 0; j < __columns; j++)
            {
                matrix[i][j] = 0;
            }
        }
    }
    catch(std::bad_alloc)
    {
        throw MException("Bad alloc");
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix & _Matrix) throw(MException)
    :_lines(_Matrix._lines), _columns(_Matrix._columns)
{
    try
    {
        matrix = new T*[_Matrix._lines];
        for(size_t i = 0; i < _Matrix._lines; i++)
        {
            matrix[i] = new T[_Matrix._lines];

            for(size_t j = 0; j < _Matrix._lines; j++)
            {
                matrix[i][j] = _Matrix.matrix[i][j];
            }
        }
    }
    catch(std::bad_alloc)
    {
        throw MException("Bad alloc");
    }
}

template<typename T>
Matrix<T>::~Matrix()
{
    for(size_t i = 0; i < _lines; i++)
    {
        delete[] matrix[i];
    }
    matrix = nullptr;
}

template<typename T>
void Matrix<T>::fill()
{
    std::cout << "Введите элементы матрицы построчно (размерность матрицы " << _lines << "x" << _columns << ")" << std::endl;
    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            std::cin >> matrix[i][j];
        }
    }
}

template<typename T>
void Matrix<T>::print() throw(MException)
{
    if(_lines == 0 && _columns == 0)
    {
        throw MException("Matrix is empty");
    }

    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
size_t Matrix<T>::lines()
{
    return _lines;
}

template<typename T>
size_t Matrix<T>::columns()
{
    return _columns;
}

template<typename T>
void Matrix<T>::resize(size_t l, size_t c) throw(MException)
{
    if(l < 0 || c < 0)
    {
        throw MException("Incorrect size of matrix");
    }

    size_t _l = _lines > l ? l : _lines;
    size_t _c = _columns > c ? c : _columns;

    try
    {
        T** temp_m = new T*[l];
        for(size_t i = 0; i < l; i++)
        {
            temp_m[i] = new T[c];
        }

        for(size_t i = 0; i < _l; i++)
        {
            for(size_t j = 0; j < _c; j++)
            {
                temp_m[i][j] = matrix[i][j];
            }
        }

        if(matrix != nullptr)
        {
            for(size_t i = 0; i < _lines; i++)
            {
                delete[] matrix[i];
            }
        }

        matrix = temp_m; // По идее, если bad alloc, то их надо оставить прежними
        _lines = l;  // пока не придумал как
        _columns = c;

        temp_m = nullptr;
    }
    catch(std::bad_alloc)
    {
        throw MException("Bad alloc");
    }
}

template<typename T>
void Matrix<T>::sort()
{
    size_t temp_size = _lines*_columns;
    T* temp_m = new T[temp_size];

    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            temp_m[i*_columns + j] = matrix[i][j];
        }
    }

    qsort(temp_m,0,temp_size - 1);

    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            matrix[i][j] = temp_m[i*_columns + j];
        }
    }

    delete[] temp_m;
    temp_m = nullptr;
}

template<typename T>
T& Matrix<T>::at(const size_t lines, const size_t columns) throw(MException)
{
    if(lines < 0 || columns < 0 || lines >= _lines || columns >= _columns)
    {
        throw MException("Incorrect index");
    }

    return matrix[lines][columns];
}

template<typename T>
void Matrix<T>::clear() throw(MException)
{
    _lines = 0;
    _columns = 0;
    if(matrix != nullptr)
    {
        for(size_t i = 0; i < _lines; i++)
        {
            delete[] matrix[i];
        }

        try
        {
            matrix = new T*;
        }
        catch(std::bad_alloc)
        {
            throw MException("Bad alloc");
        }
    }
}

template<typename T>
void Matrix<T>::transposition()
{
    T temp_m[_lines][_columns];
    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            temp_m[i][j] = matrix[i][j];
        }
    }

    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            matrix[i][j] = temp_m[j][i];
        }
    }
}

template<typename T>
T Matrix<T>::determinant() throw(MException)
{
    if(_lines != _columns)
    {
        throw MException("The determinant of non square matrix is not defined");
    }

    return calculate_det(matrix,_lines); // В данном случае матрица квадратная, поэтому _lines = _cloumns = size
}

template<typename T>
T* Matrix<T>::operator [] (const size_t index)
{
    return matrix[index];
}

template<typename T>
Matrix<T> Matrix<T>::operator + (Matrix<T>& m) throw(MException)
{
    if(_lines != m._lines || _columns != m._columns)
    {
        throw MException("Addition of matrices of different sizes");
    }

    Matrix<T> temp_m(_lines,_columns);
    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            temp_m.matrix[i][j] = matrix[i][j] + m.matrix[i][j];
        }
    }

    return temp_m;
}

template<typename T>
Matrix<T> Matrix<T>::operator - (Matrix<T>& m) throw(MException)
{
    if(_lines != m._lines || _columns != m._columns)
    {
        throw MException("Addition of matrices of different sizes");
    }

    Matrix<T> temp_m(_lines,_columns);
    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            temp_m.matrix[i][j] = matrix[i][j] - m.matrix[i][j];
        }
    }

    return temp_m;
}

template<typename T>
bool Matrix<T>::operator == (Matrix<T>& m) throw(MException)
{
    if(_lines != m._lines || _columns != m._columns)
    {
        throw MException("Compare matrices of different size");
    }

    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            if(matrix[i][j] != m.matrix[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
bool Matrix<T>::operator != (Matrix<T>& m) throw(MException)
{
    if(_lines != m._lines || _columns != m._columns)
    {
        throw MException("Compare matrices of different size");
    }

    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            if(matrix[i][j] != m.matrix[i][j])
            {
                return true;
            }
        }
    }
    return false;
}

template<typename T>
Matrix<T> Matrix<T>::operator = (Matrix<T>& m) throw(MException)
{
    try
    {
        if(matrix != nullptr)
        {
            for(size_t i = 0; i < _lines; i++)
            {
                delete[] matrix[i];
            }
        }
        _lines = m._lines;
        _columns = m._columns;

        T** temp_m = new T*[_lines];
        for(size_t i = 0; i < _lines; i++)
        {
            temp_m[i] = new T[_columns];
        }

        for(size_t i = 0; i < _lines; i++)
        {
            for(size_t j = 0; j < _columns; j++)
            {
                matrix[i][j] = m.matrix[i][j];
            }
        }

        matrix = temp_m;

        return *this;
    }
    catch(std::bad_alloc)
    {
        throw MException("Bad alloc");
    }

}

template<typename T>
std::ostream& operator << (std::ostream& os, Matrix<T>& m)
{
    for(size_t i = 0; i < m._lines; i++)
    {
        for(size_t j = 0; j < m._columns; j++)
        {
            os << m.matrix[i][j] << " ";
        }
        os << std::endl;
    }

    return os;
}

template<typename T>
std::istream& operator >> (std::istream& is, Matrix<T>& m)
{
    for(size_t i = 0; i < m._lines; i++)
    {
        for(size_t j = 0; j < m._columns; j++)
        {
            is >> m.matrix[i][j];
        }
    }

    return is;
}


#endif // MATRIX_H
