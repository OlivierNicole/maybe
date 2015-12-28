#ifndef MAYBE_H
#define MAYBE_H

#include <cstdint>
#include <memory>
#include <new>

template<typename T>
class Maybe
{
public:
   Maybe();
   Maybe(const T& value);
   Maybe(T&& value);
   constexpr Maybe(const Maybe<T>& other);
   Maybe(Maybe<T>&& other);
   ~Maybe();

   Maybe<T>& operator=(const Maybe<T>& other);
   Maybe<T>& operator=(Maybe<T>&& other);

   void clear();

   bool isNothing() const;
   bool isJust() const;

private:
   const T& value_type() const;

public:
   template<typename Functor>
   auto map(Functor f) const -> Maybe<decltype(f(value_type()))>
   {
      using U = decltype(f(value_type()));
      if(isJust())
         return Maybe<U>(f(fromJust()));
      else
         return Maybe<U>();
   }


   T& fromJust();
   const T& fromJust() const;

private:
   bool set;
   uint8_t memory[sizeof(T)];

   T* ptr();
   const T* ptr() const;
};

template<typename T, typename U, typename Functor>
U maybe(const U& defValue, Functor f, const Maybe<T>& m)
{
   if(m.isJust())
      return f(m.fromJust());
   else
      return defValue;
}

template<typename T, typename Functor>
void maybe_do(Functor f, const Maybe<T>& m)
{
   if(m.isJust())
      f(m.fromJust());
}

template<typename T, typename FunctorJust, typename FunctorNothing>
void maybe_do(FunctorJust fJust, FunctorNothing fNothing, const Maybe<T>& m)
{
   if(m.isJust())
      fJust(m.fromJust());
   else
      fNothing();
}

template<typename T>
Maybe<T>::Maybe()
   : set(false), memory()
{
}

template<typename T>
Maybe<T>::~Maybe()
{
   clear();
}

template<typename T>
constexpr Maybe<T>::Maybe(const Maybe<T>& other)
{
   operator=(other);
}

template<typename T>
Maybe<T>::Maybe(Maybe<T>&& other)
{
   operator=(std::move(other));
}

template<typename T>
Maybe<T>::Maybe(const T& value)
{
   new (memory) T(value);
   set = true;
}

template<typename T>
Maybe<T>::Maybe(T&& value)
{
   new (memory) T(std::move(value));
   set = true;
}

template<typename T>
Maybe<T>& Maybe<T>::operator=(const Maybe<T>& other)
{
   clear();
   maybe_do([this](const T& t)
            {
               new (memory) T(t);
               set = true;
            }, [this]()
            {
               set = false;
            }, other);
}

template<typename T>
Maybe<T>& Maybe<T>::operator=(Maybe<T>&& other)
{
   clear();
   maybe_do([this](const T& t)
            {
               new (memory) T(std::move(t));
               set = true;
            }, [this]()
            {
               set = false;
            }, other);
}

template<typename T>
void Maybe<T>::clear()
{
   if(isJust())
      ptr()->~T();
}

template<typename T>
bool Maybe<T>::isNothing() const
{
   return !set;
}

template<typename T>
bool Maybe<T>::isJust() const
{
   return set;
}

template<typename T>
T* Maybe<T>::ptr()
{
   return reinterpret_cast<T*>(&memory);
}

template<typename T>
const T* Maybe<T>::ptr() const
{
   return reinterpret_cast<const T*>(&memory);
}

template<typename T>
const T& Maybe<T>::value_type() const
{
   return *((T*)nullptr);
}

template<typename T>
T& Maybe<T>::fromJust()
{
   return *(ptr());
}

template<typename T>
const T& Maybe<T>::fromJust() const
{
   return *(ptr());
}

template<typename T, typename OutputStream>
OutputStream& operator<<(OutputStream& os, const Maybe<T>& m)
{
   maybe_do([&os](const T& t) { os << t; },
            [&os]() { os << "Nothing"; },
            m);
   return os;
}

#endif

