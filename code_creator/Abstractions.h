#ifndef ABSTRACTIONS_H
#define ABSTRACTIONS_H
#include <string>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

//В этом классе содержатся те функции, который нужны при наследовании
class Unit
{
public:
    //keyword using необходимо для того, чтобы зарезервировать слово для использования типа данных unsigned int
    //в свою очередь, это необходимо для сокращения количества слов для удобства и быстроты написания кода
    using Flags = unsigned int;
    //Деструктор, который нужен наследникам, чтобы они могли определить собственный декструктор
    //Только так обеспечивается корректное разрушение объекта производного класса через указатель на соответствующий базовый класс
    virtual ~Unit() = default;
    //add(...) - виртуальная функция, предназначена для добавления вложенных элементов
    //Передача происходит через умный указатель shared_ptr
    virtual void add( const std::shared_ptr< Unit >& , Flags )
    {
        throw std::runtime_error( "Not supported" );
    }
    //compile(...) - чисто виртуальная функция
    //Эта функция генерирует код на том или ином языке
    //Возвращает результат в виде строки std::string
    //level - параметр, который указывает уровень вложенности узла дерева
    //Этот параметр необходим для корректной расстановки отступов в начале строк генерируемого кода
    virtual std::string compile( unsigned int level = 0 ) const = 0;
protected:
    //generateShift(...) - возвращает строку, состоящую из нужного числа пробелов
    //принимает параметр level, от которого зависит число пробелов (уровень вложенности)
    virtual std::string generateShift( unsigned int level ) const
    {
        static const auto DEFAULT_SHIFT = '\t';
        std::string result;
        for( unsigned int i = 0; i < level; ++i )
        {
            result += DEFAULT_SHIFT;
        }
        return result;
    }
};

//Абстрактный класс, производящий генерацию класса, наследник класса Unit
class ClassUnit : public Unit
{
public:
    //enum - перечисляемый тип данных
    //Здесь интересно то, что каждому из членов данного перечисления присваивается свой номер(индекс)
    //Так тип доступа PUBLIC имеет индекс 0, что позволяет работать с этими перечислениями как с int
    //Также enum обеспечивает уникальность каждого из своих членов
    //enum AccessModifier содержит в себе типы доступа
    //Первые три - используют все три языка, представленных в программе
    //Последние три НЕ используют С++ и Java, то есть использует только C#
    enum AccessModifier {PUBLIC, PROTECTED, PRIVATE, INTERNAL, PROTECTED_INTERNAL, PRIVATE_PROTECTED};
    //Статический вектор типов доступа
    //const - гарантирует неизменяемость
    static const std::vector< std::string > ACCESS_MODIFIERS;
    //Просто конструктор
    //Ключевое слово explicit - неявный конструктор
    //то есть мы не можем создать объект класса как MyClass a = 10
    //MyClass a(10); - только так
    explicit ClassUnit( const std::string& name ): m_name(name)
    {
        m_fields.resize( ACCESS_MODIFIERS.size() );
    }
    //Чисто виртуальная функция добавления
    //Поскольку данный класс наследуется от Unit, содержащего виртуальную функцию add(...), она не может не быть реализована в классе - наследнике
    virtual void add(const std::shared_ptr< Unit >& unit, Flags flags) override = 0;
    //Чисто виртуальная функция генерации кода
    //Здесь ситуация, аналогичная чисто виртуальной функции add(...)
    virtual std::string compile( unsigned int level = 0 ) const override = 0;
    //виртуальный деструктор
    virtual ~ClassUnit() = default;
protected:
    //строка с названием класса
    std::string m_name;
    //Аналогично Flags, Fields используется для сокращения типа данных
    using Fields = std::vector< std::shared_ptr< Unit > >;
    //Вектор списка функций, которые распределены по типам доступа
    std::vector< Fields > m_fields;
};
//Статическое поле определяется вне класса
const std::vector<std::string> ClassUnit::ACCESS_MODIFIERS = {"public", "protected", "private", "internal", "protected internal", "private protected"};

//Абтрактный класс для генерации методов, наследник класса Unit
class MethodUnit : public Unit
{
public:
    //C++, C#, Java: STATIC, CONST
    //C++, C#: VIRTUAL
    //C#, Java: ABSTRACT
    //C#: ASYNC, UNSAVE
    //Java: FINAL, SYNCHRONIZED
    //аналогичное AccessModifier перечисление, но содержащее в себе модификаторы функций
    //Перечисления опеределены в виде битовых флагов
    enum Modifier { STATIC = 1, CONST = 1 << 1, VIRTUAL = 1 << 2, ABSTRACT = 1 << 3, ASYNC = 1 << 4, UNSAVE = 1 << 5, FINAL = 1 << 6, SYNCHRONIZED = 1 << 7};
    //конструктор класса
    explicit MethodUnit( const std::string& name, const std::string& returnType, Flags flags ):
        m_name( name ), m_returnType( returnType ), m_flags( flags ) {}
    virtual void add( const std::shared_ptr< Unit >& unit, Flags /* flags */ = 0 ) override = 0;
    virtual std::string compile( unsigned int level = 0 ) const override = 0;
    //виртуальный деструктор
    virtual ~MethodUnit() = default;

protected:
    //строка с названием метода
    std::string m_name;
    //тип, возвращаемый методом
    std::string m_returnType;
    //сами модификаторы
    Flags m_flags;
    //содержимое метода (тело метода)
    std::vector< std::shared_ptr< Unit > > m_body;
};

//Класс, который имитирует операцию вывода
class PrintOperatorUnit : public Unit
{
public:
    //конструктор класса
    explicit PrintOperatorUnit( const std::string& text ): m_text( text ) { }
    virtual std::string compile( unsigned int level = 0 ) const override = 0;
    //виртуальный деструктор
    virtual ~PrintOperatorUnit() = default;
protected:
    //Текст, который будет выводиться
    std::string m_text;
};

#endif // ABSTRACTIONS_H
