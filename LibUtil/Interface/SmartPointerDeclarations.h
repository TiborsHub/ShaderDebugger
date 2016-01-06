/*******************************************************************************\
*                                                                               *
*           Utilities : SmartPointerDeclarations                                *
*           Purpose   : Some utility macros to define smart pointer type names  *
*           File      : SmartPointerDeclarations.h                              *
*           Author    : Tibor den Ouden                                         *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __SMART_POINTER_DECLARATIONS_H__
#define __SMART_POINTER_DECLARATIONS_H__


// Standard headers
#include <memory>


#define POINTER_DECLARATIONS(inClass)                    \
typedef std::shared_ptr<inClass> Sp##inClass;            \
typedef std::unique_ptr<inClass> Up##inClass;            \
typedef std::weak_ptr  <inClass> Wp##inClass;            \
                                                         \
typedef std::shared_ptr<const inClass> SpConst##inClass; \
typedef std::unique_ptr<const inClass> UpConst##inClass; \
typedef std::weak_ptr  <const inClass> WpConst##inClass;


#define CLASS_FORWARD_DECLARATIONS(inClass) \
class inClass; \
POINTER_DECLARATIONS(inClass)


#define STRUCT_FORWARD_DECLARATIONS(inStruct) \
struct inStruct; \
POINTER_DECLARATIONS(inStruct)


#endif // __SMART_POINTER_DECLARATIONS_H__
