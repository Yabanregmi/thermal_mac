#pragma once
/******************************************************************************
 * Copyright (c) 2012-2019 All Rights Reserved, http://www.evocortex.com      *
 *  Evocortex GmbH                                                            *
 *  Emilienstr. 10                                                            *
 *  90489 Nuremberg                                                           *
 *  Germany                                                                   *
 *****************************************************************************/
#include <cstddef>
#include <stdexcept>
#include <sstream>

namespace evo
{

/**
 * @class IRArray
 * @brief C-Array wrapper for handle size, copy and memory
 * @author Helmut Engelhardt (Evocortex GmbH)
 */
template <class T>
class IRArray
{
private:
    //Array pointer
    T* _data;

    //Size of array
    std::size_t _size;
public:


   /**
   * Initialize array with given size
   * @param[in] size Size of array
   */
    IRArray(std::size_t size);

    /**
     * Initialize array with given size and data.
     * @param[in] size Size of array
     * @param[in] data Pointer to array to copy from. Has to be at least the given size!
     */
    IRArray(std::size_t size, const T* const data);

    /**
     * Copy constructor to copy given IRArray
     * @param[in] obj IRArray to copy
     */
    IRArray(const IRArray<T> &obj);

    /**
     * Deconstructor. Handles deletion of array
    */
    ~IRArray();

    /**
     * Assignment operator for copy IRArray
     * @param[in] obj IRArray to copy
     */
    IRArray<T>& operator=(const IRArray<T>& obj);
    
    IRArray<T>& operator=(IRArray<T>&& obj);

    /**
     * Index operator. Throws std::out_of_range if index out of range.
     * @param[in] index Index of required data
     */
    T& operator[](const std::size_t index);

    /**
     * Const index operator. Throws std::out_of_range if index out of range.
     * @param[in] index Index of required data
     */
	const T& operator[](const std::size_t index) const;

    /**
     * Returns size of array
     * @return Size of array
     */
    std::size_t size() const;

    /**
     * Returns pointer to array
     * @return Array pointer
     */
    T* const data() const;
};

} //namespace evo

template <class T>
evo::IRArray<T>::IRArray(std::size_t size)
{
    _size = size;
    if(_size > 0)
    {
        _data = new T[_size];
    }
    else
    {
        _data = nullptr;
    }
}

template <class T>
evo::IRArray<T>::IRArray(std::size_t size, const T* const data)
{
    _size = size;
    if(_size > 0)
    {
        _data = (T*)(new char[size * sizeof(T)]);
        std::copy(data, data + _size, _data);
    }
    else
    {
        _data = nullptr;
    }
}

template <class T>
evo::IRArray<T>::IRArray(const evo::IRArray<T> &obj) : IRArray(obj._size, obj._data)
{
}

template <class T>
evo::IRArray<T>::~IRArray()
{
    if(_data != nullptr)
    {
        delete[] _data;
        _data = nullptr;
    }
}

template <class T>
evo::IRArray<T>& evo::IRArray<T>::operator=(const evo::IRArray<T>& obj)
{
    IRArray<T> tmp(obj);
    _size = tmp._size;
    delete[] _data;
    _data = tmp._data;

    tmp._size = 0;
    tmp._data = nullptr;
    return *this;
}

template <class T>
evo::IRArray<T>& evo::IRArray<T>::operator=(evo::IRArray<T>&& obj)
{
    _size = obj._size;
    delete[] _data;
    _data = obj._data;

    obj._size = 0;
    obj._data = nullptr;
    return *this;
}

template <class T>
T& evo::IRArray<T>::operator[](const std::size_t index) 
{ 
    if (index < 0 || index >= _size) 
    { 
        std::stringstream ss;
        ss << "Index " << index << " out of range. [0.." << _size << "[";
        throw std::out_of_range(ss.str());
    }
    return _data[index]; 
}

template <class T>
const T& evo::IRArray<T>::operator[](const std::size_t index) const
{
    if (index < 0 || index >= _size) 
	{
		std::stringstream ss;
		ss << "Index " << index << " out of range. [0.." << _size << "[";
		throw std::out_of_range(ss.str());
	}
	return _data[index];
}

template <class T>
std::size_t evo::IRArray<T>::size() const
{
    return _size;
}

template <class T>
T* const evo::IRArray<T>::data() const
{
    return _data;
}