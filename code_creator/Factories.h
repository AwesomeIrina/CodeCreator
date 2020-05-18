#ifndef FACTORIES_H
#define FACTORIES_H
#include "Abstractions.h"
#include "Pluses.h"
#include "CSharp.h"
#include "Java.h"

//Абстрактная фабрика
//Она создаёт абстрактные "продукты" с типами ClassUnit, MethodUnit, PrintOperatorUnit,
//чтобы в дальнейшем создать "конкретные продукты" для каждого из трёх предсталенных в программе языков программирования
class AbstractFactory {
public:
    virtual std::shared_ptr<ClassUnit> ClassCreator(const std::string& name, Unit::Flags accessFlags = 0, Unit::Flags modificatorFlags = 0) const = 0;
    virtual std::shared_ptr<MethodUnit> MethodCreator(const std::string& name, const std::string& returnType, Unit::Flags flags) const = 0;
    virtual std::shared_ptr<PrintOperatorUnit> PrintOperatorCreator(const std::string& text) const = 0;
    //Виртуальный деструктор
    virtual ~AbstractFactory() = default;
};

//Конкретная фабрика для C++
class PlussesFactory: public AbstractFactory
{
public:
    std::shared_ptr<ClassUnit> ClassCreator(const std::string& name, Unit::Flags, Unit::Flags) const override
    {
        return shared_ptr<ClassUnit>(new PlussesClassUnit(name));
    }
    std::shared_ptr<MethodUnit> MethodCreator(const std::string& name, const std::string& returnType, Unit::Flags flags) const override
    {
        return shared_ptr<MethodUnit>(new PlussesMethodUnit(name, returnType, flags));
    }
    std::shared_ptr<PrintOperatorUnit> PrintOperatorCreator(const std::string& text) const override
    {
        return shared_ptr<PrintOperatorUnit>(new PlussesPrintOperatorUnit(text));
    }
    //Деструктор
    ~PlussesFactory() = default;
};

//Конкретная фабрика для C#
class CSharpFactory: public AbstractFactory
{
public:
    std::shared_ptr<ClassUnit> ClassCreator(const std::string& name, Unit::Flags, Unit::Flags) const override
    {
        return shared_ptr<ClassUnit>(new CSharpClassUnit(name));
    }
    std::shared_ptr<MethodUnit> MethodCreator(const std::string& name, const std::string& returnType, Unit::Flags flags) const override
    {
        return shared_ptr<MethodUnit>(new CSharpMethodUnit(name, returnType, flags));
    }
    std::shared_ptr<PrintOperatorUnit> PrintOperatorCreator(const std::string& text) const override
    {
        return shared_ptr<PrintOperatorUnit>(new CSharpPrintOperatorUnit(text));
    }
    //Деструктор
    ~CSharpFactory() = default;
};

//Конкретная фабрика для Java
class JavaFactory: public AbstractFactory
{
public:
    std::shared_ptr<ClassUnit> ClassCreator(const std::string& name, Unit::Flags classAccess = ClassUnit::PUBLIC, Unit::Flags classModifier = 0) const override
    {
        return shared_ptr<ClassUnit>(new JavaClassUnit(name, classAccess, classModifier));
    }
    std::shared_ptr<MethodUnit> MethodCreator(const std::string& name, const std::string& returnType, Unit::Flags flags) const override
    {
        return shared_ptr<MethodUnit>(new JavaMethodUnit(name, returnType, flags));
    }
    std::shared_ptr<PrintOperatorUnit> PrintOperatorCreator(const std::string& text) const override
    {
        return shared_ptr<PrintOperatorUnit>(new JavaPrintOperatorUnit(text));
    }
    //Деструктор
    ~JavaFactory() = default;
};


#endif // FACTORIES_H
