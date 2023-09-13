#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

void defineAst(const string& outputDir, const string& baseName, const vector<string>& types);
vector<string> splitString(const string &input, const char &delimiter);
void defineType(ofstream &writer, string baseName, string className, string fieldList);
void defineVisitor(ofstream &writer, const string& baseName, const vector<string>& types);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: generate_ast <output directory>";
        return -1;
    }

    string outputDir = argv[1];
    defineAst(outputDir, "Expr",
              {"Binary : Expr left, Token oper, Expr right", "Grouping : Expr expression", "Literal : void value, TokenType type",
               "Unary : Token oper, Expr right"});
}

vector<string> splitString(const string &input, const char &delimiter)
{
    vector<string> strings;
    istringstream f(input);
    string s;
    while (getline(f, s, delimiter))
    {
        strings.push_back(s);
    }
    return strings;
}

void defineAst(const string& outputDir,const string& baseName, const vector<string>& types)
{
    string path = outputDir + "/" + baseName + ".hpp";
    ofstream writer(path);
    writer<<"#pragma once\n";
    writer<<"#include <memory>\n";
    writer<<"#include \"../scanner/IScanner.hpp\"\n";
    writer<<"struct Binary;\n";
    writer<<"struct Grouping;\n";
    writer<<"struct Literal;\n";
    writer<<"struct Unary;\n";
    defineVisitor(writer, baseName, types);
    writer << "struct " + baseName + " {\nvirtual void accept(std::shared_ptr<Visitor> visitor) = 0;};\n\n";
    // The AST classes.
    for (string type : types)
    {
        auto splitedString = splitString(type, ':');
        string className = splitedString[0];
        string fields = splitedString[1];
        defineType(writer, baseName, className, fields);
    }
}

void defineVisitor(ofstream& writer,const string& baseName, const vector<string>& types)
{
    writer<<"struct Visitor{\n";
    for (string type : types) 
    {
        auto splitedString = splitString(type, ':');
        string typeName = splitedString[0];
        writer<<"virtual void visit(std::shared_ptr<" + typeName + "> " + ") = 0;\n";
    }
    writer<<" };\n";
}

void defineType(ofstream &writer, string baseName, string className, string fieldList)
{
    writer << "struct " + className + " : public " + baseName +
    ", public std::enable_shared_from_this<" + className +">" + "\n{\n";
    // Constructor.
    auto fields = splitString(fieldList, ',');
    writer << className + "(";
    bool isFirst=true;
    for(auto field: fields)
    {
        string typeName = splitString(string(field.begin()+1, field.end()), ' ')[0];
        string name = splitString(string(field.begin()+1, field.end()), ' ')[1];
        if(!isFirst)
        {
            writer<<", ";
        }
        isFirst=false;
        writer<<"const std::shared_ptr<"+typeName+">& " + name;
    }
    writer<<")\n{\n";
    // Store parameters in fields.
    for (string field : fields)
    {
        string name = splitString(string(field.begin()+1, field.end()), ' ')[1];
        writer << "this->" << name << " = " << name << ";\n";
    }
    writer << ("}\n");

    writer<<"void accept(std::shared_ptr<Visitor> visitor) override {\n";
    writer<<"visitor->visit(shared_from_this());\n}\n";
    // Fields.
    for (string field : fields)
    {
        string typeName = splitString(string(field.begin()+1, field.end()), ' ')[0];
        string name = splitString(string(field.begin()+1, field.end()), ' ')[1];
        writer << "std::shared_ptr<"+typeName+"> "<<name << ";\n";
    }
    writer << "};\n\n";
}