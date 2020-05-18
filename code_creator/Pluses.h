#ifndef PLUSES_H
#define PLUSES_H
#include "Abstractions.h"

//Класс, генерирующий конкретный класс на языке С++
class PlussesClassUnit: public ClassUnit
{
public:
    explicit PlussesClassUnit( const std::string& name ): ClassUnit(name)
    {
        //У С++ иммется три типа доступа, поэтому размер меняем на три
        m_fields.resize(3);
    }

    void add(const std::shared_ptr< Unit >& unit, Flags flags) override
    {
        //Проверка существования объекта
        if(unit == nullptr)
        {
            return;
        }

        //По умолчанию тип доступа в С++ - PRIVATE
        int accessModifier = PRIVATE;
        //Если указан другой тип доступа, меняем его
        if(flags < 3)
        {
            accessModifier = flags;
        }
        //Если пришёл флаг, который больше, чем количество типов доступа,
        //программа выдаёт соответствующее сообщение пользователю
        else
        {
            throw std::runtime_error("In C++ there is no accessModifire like this");
        }
        //Добавляем метод и его тип доступа
        m_fields[accessModifier].push_back(unit);
    }

    std::string compile( unsigned int level = 0 ) const override
    {
        //Объявляем сам класс
        std::string result = generateShift( level ) + "class " + m_name + " {\n";
        //объявление метода, также в этом цикле определяются тела методов
        //Здесь происходит сборка методов с одинаковыми типами доступа
        for( size_t i = 0; i < m_fields.size(); ++i )
        {
            //тело метода может быть пустым
            //и если это так, просто выходим из цикла
            if( m_fields[ i ].empty() )
            {
                continue;
            }
            //добавляем тип доступа
            result += ACCESS_MODIFIERS[ i ] + ":\n";
            //Собираем методы с таким типом доступа
            for( const auto& f : m_fields[ i ] )
            {
                result += f->compile( level + 1 );
            }
            result += "\n";
        }
        //закрываем сам класс
        result += generateShift( level ) + "};\n";
        return result;
    }
};

//Генерация методов на языке С++
class PlussesMethodUnit: public MethodUnit
{
public:
    //конструктор
    explicit PlussesMethodUnit( const std::string& name, const std::string& returnType, Flags flags ): MethodUnit(name, returnType, flags){}
    void add( const std::shared_ptr< Unit >& unit, Flags /* flags */ = 0 ) override
    {
        //Допускаем, что тело функции может быть пустым
        if(unit != nullptr){
            m_body.push_back( unit );
        }
    }

    std::string compile( unsigned int level = 0 ) const override
    {
        std::string result = generateShift( level );
        //здесь добавляется имя метода и соответствующий модификатор
        if( m_flags & STATIC )
        {
            result += "static ";
        }

        else if( m_flags & VIRTUAL )
        {
            result += "virtual ";
        }

        result += m_returnType + " ";
        result += m_name + "()";

        if( m_flags & CONST )
        {
            result += " const";
        }
        result += " {\n";
        //Собираем тело метода
        for( const auto& b : m_body )
        {
            result += b->compile( level + 1 );
        }
        result += generateShift( level ) + "}\n";
        return result;
    }
};

//Класс, имитирующий вывод на языке С++
class PlussesPrintOperatorUnit: public PrintOperatorUnit
{
public:
    explicit PlussesPrintOperatorUnit( const std::string& text ): PrintOperatorUnit(text){}

    std::string compile( unsigned int level = 0 ) const override
    {
        return generateShift( level ) + "printf( \"" + m_text + "\" );\n";
    }
};

#endif // PLUSES_H
