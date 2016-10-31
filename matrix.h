#ifndef MATRIX_H
#define MATRIX_H

#include "mexception.h"
#include <iostream>
#include <math.h>

template<typename T>
class Matrix
{
    T* matrix;
    size_t _lines;
    size_t _columns;

    size_t size;

    T                       calculate_det(T**, size_t) noexcept;
    T**                     cut(T**, size_t, size_t) throw(MException);

    void                    qsort(T*, int, int) noexcept;
    T                       partition(T*, int, int) noexcept;
public:
    explicit Matrix(size_t = 0, size_t = 0) throw(MException);
    Matrix(const Matrix&) throw(MException);
    ~Matrix() noexcept;

    void                    fill() noexcept;
    void                    print() const throw(MException);

    const size_t&           lines() const noexcept;
    const size_t&           columns() const noexcept;

    void                    resize(const size_t&, const size_t&) throw(MException);

    void                    sort() noexcept;

    T&                      at(const size_t&, const size_t&) const throw(MException);

    void                    clear() throw(MException);

    void                    transposition() noexcept;
    T                       determinant() throw(MException);

    Matrix<T>               operator + (const Matrix<T>&) throw(MException);
    Matrix<T>               operator - (const Matrix<T>&) throw(MException);

    bool                    operator == (const Matrix<T>&) throw(MException);
    bool                    operator != (const Matrix<T>&) throw(MException);

    Matrix<T>               operator = (const Matrix<T>&) throw(MException);

    template<typename T1>
    friend std::ostream&    operator << (std::ostream&,const Matrix<T1>&) noexcept;

    template<typename T1>
    friend std::istream&    operator >> (std::istream&,const Matrix<T1>&) noexcept;
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
T Matrix<T>::calculate_det(T** m, size_t size) noexcept
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
void Matrix<T>::qsort(T* m,int begin, int end) noexcept
{
    if(begin < end)
    {
        int q = partition(m,begin,end);
        qsort(m,begin,q-1);
        qsort(m,q+1,end);
    }
}

template<typename T>
T Matrix<T>::partition(T* m,int begin, int end) noexcept
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
Matrix<T>::Matrix(size_t lines, size_t columns) throw(MException)
    :_lines(lines), _columns(columns), size(lines * columns)
{
    if( lines < 0 || columns < 0)
    {
        throw MException("Incorrect size of matrix");
    }
    try
    {
        matrix = new T[lines * columns];

        for(size_t i = 0; i < lines * columns; i++)
        {
            matrix[i] = T();
        }
    }
    catch(std::bad_alloc)
    {
        throw MException("Bad alloc");
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix & _Matrix) throw(MException)
    :_lines(_Matrix._lines), _columns(_Matrix._columns), size(_Matrix.size)
{
    try
    {
        size_t size = _Matrix.size;
        matrix = new T[size];

        for(size_t i = 0; i < size; i++)
        {
            matrix[i] = _Matrix.matrix[i];
        }
    }
    catch(std::bad_alloc)
    {
        throw MException("Bad alloc");
    }
}

template<typename T>
Matrix<T>::~Matrix() noexcept
{
    _lines = 0;
    _columns = 0;
    size = 0;

    delete[] matrix;
    matrix = nullptr;
}

template<typename T>
void Matrix<T>::fill() noexcept
{
    std::cout << "Введите элементы матрицы построчно (размерность матрицы " << _lines << "x" << _columns << ")" << std::endl;
    for(size_t i = 0; i < size; i++)
    {
        std::cin >> matrix[i];
    }
}

template<typename T>
void Matrix<T>::print() const throw(MException)
{
    if(_lines == 0 && _columns == 0)
    {
        throw MException("Matrix is empty");
    }

    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            std::cout << matrix[i*_columns + j] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
const size_t& Matrix<T>::lines() const noexcept
{
    return _lines;
}

template<typename T>
const size_t& Matrix<T>::columns() const noexcept
{
    return _columns;
}

template<typename T>
void Matrix<T>::resize(const size_t& l, const size_t& c) throw(MException)
{
    if(l < 0 || c < 0)
    {
        throw MException("Incorrect size of matrix");
    }

    size_t _l = _lines > l ? l : _lines;
    size_t _c = _columns > c ? c : _columns;

    try
    {
        T* temp_m = new T[l*c];

        for(size_t i = 0; i < l; i++)
        {
            for(size_t j = 0; j < c; j++)
            {
                if(i < _l && j < _c)
                    temp_m[i*_l + j] = matrix[i*_columns + j];
                else
                    temp_m[i*_l + j] = T();

            }
        }

        if(matrix != nullptr)
        {
            delete[] matrix;
        }

        matrix = temp_m;
        _lines = l;
        _columns = c;
        size = l*c;

        temp_m = nullptr;
    }
    catch(std::bad_alloc)
    {
        throw MException("Bad alloc");
    }
}

template<typename T>
void Matrix<T>::sort() noexcept
{
    qsort(matrix,0,size - 1);
}

template<typename T>
T& Matrix<T>::at(const size_t& lines, const size_t& columns) const throw(MException)
{
    if(lines < 0 || columns < 0 || lines >= _lines || columns >= _columns)
    {
        throw MException("Incorrect index");
    }

    return matrix[lines*_columns + columns];
}

template<typename T>
void Matrix<T>::clear() throw(MException)
{
    if(size == 0)
    {
        throw MException("Matrix already empty");
    }
    _lines = 0;
    _columns = 0;
    size = 0;

    if(matrix != nullptr)
    {
        delete[] matrix;
        matrix = nullptr;
    }
}

template<typename T>
void Matrix<T>::transposition() noexcept
{
    T temp_m[_lines][_columns];
    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            temp_m[i][j] = matrix[i*_columns + j];
        }
    }

    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            matrix[i*_columns + j] = temp_m[j][i];
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

    T** temp_m = new T*[_lines];
    for(size_t i = 0; i < _lines; i++)
    {
        temp_m[i] = new T[_columns];
    }

    for(size_t i = 0; i < _lines; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            temp_m[i][j] = matrix[i*_columns + j];
        }
    }

    return calculate_det(temp_m,_lines); // В данном случае матрица квадратная, поэтому _lines = _cloumns = size
}

template<typename T>
Matrix<T> Matrix<T>::operator + (const Matrix<T>& m) throw(MException)
{
    if(_lines != m._lines || _columns != m._columns)
    {
        throw MException("Addition of matrices of different sizes");
    }

    Matrix<T> temp_m(_lines,_columns);
    for(size_t i = 0; i < size; i++)
    {
        temp_m.matrix[i] = matrix[i] + m.matrix[i];
    }

    return temp_m;
}

template<typename T>
Matrix<T> Matrix<T>::operator - (const Matrix<T>& m) throw(MException)
{
    if(_lines != m._lines || _columns != m._columns)
    {
        throw MException("Addition of matrices of different sizes");
    }

    Matrix<T> temp_m(_lines,_columns);
    for(size_t i = 0; i < size; i++)
    {
        temp_m.matrix[i] = matrix[i] - m.matrix[i];
    }

    return temp_m;
}

template<typename T>
bool Matrix<T>::operator == (const Matrix<T>& m) throw(MException)
{
    if(_lines != m._lines || _columns != m._columns)
    {
        throw MException("Compare matrices of different size");
    }

    for(size_t i = 0; i < size; i++)
    {
        if(matrix[i] != m.matrix[i])
        {
            return false;
        }
    }
    return true;
}

template<typename T>
bool Matrix<T>::operator != (const Matrix<T>& m) throw(MException)
{
    if(_lines != m._lines || _columns != m._columns)
    {
        throw MException("Compare matrices of different size");
    }

    for(size_t i = 0; i < size; i++)
    {
        if(matrix[i] != m.matrix[i])
        {
            return true;
        }
    }
    return false;
}

template<typename T>
Matrix<T> Matrix<T>::operator = (const Matrix<T>& m) throw(MException)
{
    try
    {
        if(matrix != nullptr)
        {
            delete[] matrix;
        }

        _lines = m._lines;
        _columns = m._columns;

        size_t temp_size = _lines*_columns;
        T* temp_m = new T[temp_size];

        for(size_t i = 0; i < temp_size; i++)
        {
            matrix[i] = m.matrix[i];
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
std::ostream& operator << (std::ostream& os, const Matrix<T>& m) noexcept
{
    for(size_t i = 0; i < m._lines; i++)
    {
        for(size_t j = 0; j < m._columns; j++)
        {
            os << m.matrix[i*m._columns + j] << " ";
        }
        os << std::endl;
    }

    return os;
}

template<typename T>
std::istream& operator >> (std::istream& is, const Matrix<T>& m) noexcept
{
    for(size_t i = 0; i < m.size; i++)
    {
        is >> m.matrix[i];
    }

    return is;
}


#endif // MATRIX_H
