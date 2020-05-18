#ifndef JAVA_H
#define JAVA_H
#include "Abstractions.h"

//Класс для генерации конкретного класса на языке Java
class JavaClassUnit: public ClassUnit
{
private:
    //тип доступа для класса
    Flags accessesModifier_class;
    //модификатор для класса
    Flags Modifier;
public:
    explicit JavaClassUnit( const std::string& name, Flags classAccess = PUBLIC /*public, private, protected*/, Flags classModifier = 0 /*Final, abstract*/ ): ClassUnit(name)
    {
        //У Java имеется 3 типа доступа, поэтому изменяем размер на 3
        m_fields.resize(3);
        Modifier = 0;
        //Определение модификатора
        if (classModifier & MethodUnit::ABSTRACT)
        {
           Modifier = MethodUnit::ABSTRACT;
        }

        else if(classModifier & MethodUnit::FINAL)
        {
            Modifier = MethodUnit::FINAL;
        }
        //определение типа доступа класса
        if(classAccess<2)
        {
            accessesModifier_class = classAccess;
        }
        else
        {
            accessesModifier_class = PUBLIC;
            throw std::runtime_error("In Java there is no accessModifire for classes like this");
        }

    }

    void add(const std::shared_ptr< Unit >& unit, Flags flags) override
    {
        //Проверка существования объекта
        if(unit == nullptr)
        {
            return;
        }
        //Определение типа доступа функции
        int accessModifier = PUBLIC;
        if(flags < 3)
        {
            accessModifier = flags;
        }
        else
        {
            throw std::runtime_error("In Java there is no accessModifire for classes like this");
        }
        //Добавление в вектор метода и его типа доступа
        m_fields[accessModifier].push_back(unit);
    }

    std::string compile( unsigned int level = 0 ) const override
    {
        //Сначала объявляем тип доступа и модификатор класса
        std::string classAccess = "";
        std::string Modifier = "";

        classAccess = ACCESS_MODIFIERS[accessesModifier_class] + ' ';

        if(accessesModifier_class & MethodUnit::FINAL)
        {
            Modifier = "final";
        }
        else if (accessesModifier_class & MethodUnit::ABSTRACT)
        {
            Modifier = "abstract";
        }
        //После того, как определили тип доступа класса, переходим к непосредственному объявлению класса
        std::string result = generateShift(level) + classAccess + Modifier + "class " + m_name + " {\n";
        //Определяем методы и их тела
        for( size_t i = 0; i < m_fields.size(); ++i )
        {
            if( m_fields[ i ].empty() )
            {
                continue;
            }

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

//Класс для генерации методов на языке Java
class JavaMethodUnit: public MethodUnit
{
public:
    JavaMethodUnit( const std::string& name, const std::string& returnType, Flags flags ): MethodUnit(name, returnType, flags){}

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

        if( m_flags & SYNCHRONIZED) {
            result += "synchronized ";
        }

        if(m_flags & STATIC)
        {
            result += "static ";
        }

        if(m_flags & ABSTRACT)
        {
            result += "abstract ";
        }
        // при объявлении метода не могут стоять одновременно abstract и final
        else if( m_flags & FINAL )
        {
            result += "final ";
        }

        result += m_returnType + " ";
        result += m_name + "()";
        result += " {\n";
        //Сборка тела метода
        for( const auto& b : m_body )
        {
            result += b->compile( level + 1 );
        }
        result += generateShift( level ) + "}\n";
        return result;
    }
};

//Класс, имитирующий операцию печати на языке Java
class JavaPrintOperatorUnit: public PrintOperatorUnit
{
public:
    explicit JavaPrintOperatorUnit( const std::string& text ): PrintOperatorUnit(text){}
    std::string compile( unsigned int level = 0 ) const override
    {
        return generateShift( level ) + "System.out.print( \"" + m_text + "\" );\n";
    }
};


#endif // JAVA_H
