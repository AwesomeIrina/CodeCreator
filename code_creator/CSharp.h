#ifndef CSHARP_H
#define CSHARP_H
#include "Abstractions.h"

class CSharpClassUnit: public ClassUnit
{
private:
    //Тип доступа для класса
    Flags accessesModifier_class;
public:
    explicit CSharpClassUnit( const std::string& name, Flags flag = PRIVATE ): ClassUnit(name)
    {
        //У C# имеется 6 типов доступа, поэтому изменяем размер на 6
        m_fields.resize(6);
        //Также у C# есть тип доступа самого класса, где также имеется 6 вариантов
        //Если пользователь ввёл не тот символ, программа выдаст соответствующее сообщение об этом
        if(flag >= 6)
        {
            throw std::runtime_error("In C# there is no accessModifire like this");
        }
        else
        {
            accessesModifier_class = flag;
        }

    }

    void add(const std::shared_ptr< Unit >& unit, Flags flags) override
    {
        //Проверка, существует ли объект
        if(unit == nullptr)
        {
            return;
        }
        //Определение типа доступа
        int accessModifier = PRIVATE;
        if(flags < 6)
        {
            accessModifier = flags;
        }
        //В случае некорректного ввода, программа выдаст соответствующее сообщение
        else
        {
            throw std::runtime_error("In C# there is no accessModifire like this");
        }
        //Добавление метода и его типа доступа
        m_fields[accessModifier].push_back(unit);
    }

    std::string compile( unsigned int level = 0 ) const override
    {
        //Сначала объявляем тип доступа класса
        std::string classAccess = "";
        if(accessesModifier_class != PRIVATE)
        {
            classAccess = ACCESS_MODIFIERS[accessesModifier_class] + ' ';
        }
        //После того, как определили тип доступа класса, переходим к непосредственному объявлению класса
        std::string result = generateShift(level) + classAccess + "class " + m_name + " {\n";
        //Аналогично С++, определяем методы и их тела
        for( size_t i = 0; i < m_fields.size(); ++i )
        {
            if( m_fields[ i ].empty() )
            {
                continue;
            }
            result += ACCESS_MODIFIERS[ i ] + ":\n";

            //Поскольку у функций также много различных типов доступа, необходимо их добавить
            for( const auto& f : m_fields[ i ] )
            {
                result += generateShift( level + 1 ) + ACCESS_MODIFIERS[i] + ' ' + f->compile(level+1);
            }
            result += "\n";
        }
        result += generateShift( level ) + "};\n";
        return result;
    }
};

class CSharpMethodUnit: public MethodUnit
{
public:
    CSharpMethodUnit( const std::string& name, const std::string& returnType, Flags flags ): MethodUnit(name, returnType, flags){}

    void add( const std::shared_ptr< Unit >& unit, Flags /* flags */ = 0 ) override
    {
        //Допускаем, что тело функции может быть пустым
        if(unit != nullptr){
            m_body.push_back( unit );
        }
    }

    std::string compile( unsigned int level = 0 ) const override
    {
        //Модификаторы
        std::string result = "";

        if( m_flags & VIRTUAL) {
            result += "virtual ";
        }
        // при объявлении метода не могут стоять одновременно virtual и static/abstract
        else
        {
            if(m_flags & STATIC)
            {
                result += "static ";
            }
            if(m_flags & ABSTRACT)
            {
                result += "abstract ";
            }
        }

        if( m_flags & ASYNC )
        {
            result += "async ";
        }

        if( m_flags & UNSAVE )
        {
            result += "unsave ";
        }
        //Инициализация метода
        result += m_returnType + " ";
        result += m_name + "()";

        result += " {\n";
        //Соборка тела метода
        for( const auto& b : m_body )
        {
            result += b->compile( level + 1 );
        }
        result += generateShift( level ) + "}\n";
        return result;
    }
};

//Класс, имитирующий операцию вывода на языке C#
class CSharpPrintOperatorUnit: public PrintOperatorUnit
{
public:
    explicit CSharpPrintOperatorUnit( const std::string& text ): PrintOperatorUnit(text){}
    std::string compile( unsigned int level = 0 ) const override
    {
        return generateShift( level ) + "System.Console.WriteLine( \"" + m_text + "\" );\n";
    }
};


#endif // CSHARP_H
