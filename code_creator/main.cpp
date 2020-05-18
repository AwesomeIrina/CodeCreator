#include <QCoreApplication>
#include "Abstractions.h"
#include "Factories.h"
#include "Pluses.h"
#include "CSharp.h"

std::string generateProgram( const std::shared_ptr< AbstractFactory >& factory ) {
    auto myClass = factory->ClassCreator( "MyClass" );
    myClass->add( factory->MethodCreator( " testFunc1 ", " void ", 0), ClassUnit::PUBLIC );
    myClass->add( factory->MethodCreator( " testFunc2 ", " void ", MethodUnit::STATIC ), ClassUnit::PRIVATE);
    myClass->add( factory->MethodCreator( " testFunc3 ", " void ", MethodUnit::VIRTUAL| MethodUnit::CONST ), ClassUnit::PUBLIC);
    std::shared_ptr< MethodUnit >method = factory->MethodCreator(" testFunc4 ", " void ", MethodUnit::STATIC);
    method->add(factory->PrintOperatorCreator(R"(Hello, world!\n)"));
    myClass->add(method, ClassUnit::PROTECTED);
    return myClass->compile();
}

int main(int argc, char *argv[])
{
    std::cout<<"C++_code:"<<"\n"<<endl;
    std::cout<<generateProgram(std::make_shared<PlussesFactory>())<<std::endl;
    std::cout<<"C#_code:"<<"\n"<<endl;
    std::cout<<generateProgram(std::make_shared<CSharpFactory>())<<std::endl;
    std::cout<<"Java_code:"<<"\n"<<endl;
    std::cout<<generateProgram(std::make_shared<JavaFactory>())<<std::endl;
    QCoreApplication a(argc,argv);
    return a.exec();
}

