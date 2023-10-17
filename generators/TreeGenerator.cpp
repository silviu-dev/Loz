#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

void defineAst(const string& outputDir, const string& baseName, const vector<string>& types, const string& aditionalBeginingCode);
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
    string additionalExprIncludes = "#pragma once\n"
               "#include <memory>\n"
               "#include <any>\n"
               "#include \"../scanner/IScanner.hpp\"\n"
               "struct Assign;\n"
               "struct Binary;\n"
               "struct Call;\n"
               "struct Grouping;\n"
               "struct Literal;\n"
               "struct Unary;\n"
               "struct Variable;\n"
               "struct Logical;\n"
               "struct Get;\n"
               "struct Set;\n"
               "struct This;\n";
    defineAst(outputDir, "Expr",
              {"Assign = Token name, Expr value", "Binary = Expr left, Token oper, Expr right",
              "Call = Expr callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments",
              "Grouping = Expr expression", "Literal = std::any value", "Unary = Token oper, Expr right",
              "Variable = Token name","Logical = Expr left, Token oper, Expr right",
              "Get = Expr object, Token name", "Set = Expr object, Token name, Expr value",
              "This = Token keyword"}, additionalExprIncludes);

    string additionalStmtIncludes = "#pragma once\n"
               "#include <memory>\n"
               "#include <any>\n"
               "#include \"../scanner/IScanner.hpp\"\n"
               "#include \"Expr.hpp\"\n"
               "struct Block;\n"
               "struct Expression;\n"
               "struct Function;\n"
               "struct Print;\n"
               "struct Var;\n"
               "struct If;\n"
               "struct While;\n"
               "struct Return;\n"
               "struct Class;\n";
    defineAst(outputDir, "Stmt",
          {"Block = std::vector<std::shared_ptr<Stmt>> statements", "Expression = Expr expression", "Print = Expr expression",
           "Var = Token name, Expr initializer", "If = Expr condition, Stmt thenBranch, Stmt elseBranch",
           "While = Expr condition, Stmt body", "Function = Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body",
           "Return = Token keyword, Expr value",
           "Class = Token name, std::vector<std::shared_ptr<Stmt>> methods"},
          additionalStmtIncludes);
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

void defineAst(const string& outputDir,const string& baseName, const vector<string>& types,
    const string& aditionalBeginingCode)
{
    string path = outputDir + "/" + baseName + ".hpp";
    ofstream writer(path);
    writer<<aditionalBeginingCode;
    defineVisitor(writer, baseName, types);
    writer << "struct " + baseName + " {\nvirtual std::any accept(std::shared_ptr<"+baseName+"Visitor> visitor) = 0;\n"
                                     "virtual bool isEqual(std::shared_ptr<"+baseName+">) = 0;};\n\n";
    // The AST classes.
    for (string type : types)
    {
        auto splitedString = splitString(type, '=');
        string className = splitedString[0];
        string fields = splitedString[1];
        defineType(writer, baseName, className, fields);
    }
    writer.close();
}

void defineVisitor(ofstream& writer,const string& baseName, const vector<string>& types)
{
    writer<<"struct "+baseName+"Visitor{\n";
    for (string type : types) 
    {
        auto splitedString = splitString(type, '=');
        string typeName = splitedString[0];
        writer<<"virtual std::any visit(std::shared_ptr<" + typeName + "> " + ") = 0;\n";
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
        if(typeName=="Expr" || typeName == "Stmt")
        {
        writer<<"const std::shared_ptr<"+typeName+">& " + name;
        }
        else
        {
            writer<<"const "<<typeName+"& " + name;
        }
    }
    isFirst=true;
    writer<<"):\n";
    // Store parameters in fields.
    for (string field : fields)
    {
        if(!isFirst)
        {
            writer<<",\n";
        }
        isFirst=false;
        string name = splitString(string(field.begin()+1, field.end()), ' ')[1];
        writer << name << "(" << name << ")";
    }
    writer << ("{}\n");

    writer<<"std::any accept(std::shared_ptr<"+baseName+"Visitor> visitor) override {\n";
    writer<<"return visitor->visit(shared_from_this());\n}\n";
    //isEqual 
    writer<<"bool isEqual(std::shared_ptr<"+baseName+"> elem)\n"
    "{\n   if(typeid(*elem)!=typeid(*this)) return false;\n"
    "    auto convertedPtr = std::static_pointer_cast<"+className+">(elem);\n";
    for (string field : fields)
    {
        string typeName = splitString(string(field.begin()+1, field.end()), ' ')[0];
        string name = splitString(string(field.begin()+1, field.end()), ' ')[1];
        if(typeName == "Expr")
        {
            writer<<"if(!this->"+name+"->isEqual(convertedPtr->"+name+"))\n"
            "   return false;\n";
        }
        else
        if(typeName == "std::any")
        {
            writer<<"if (this->"+name+".type() != convertedPtr->"+name+".type()) "
            "return false;\n"
            "if (this->"+name+".type() == typeid(std::string))\n"
            "{\n   if(std::any_cast<std::string>(this->"+name+") != std::any_cast<std::string>(convertedPtr->"+name+")) "
            "return false;\n}\n"
            "else if (this->"+name+".type() == typeid(double))\n"
            "{\n   if(std::any_cast<double>(this->"+name+") != std::any_cast<double>(convertedPtr->"+name+")) "
            "return false;\n}\n"
            "else if (this->"+name+".type() == typeid(bool))"
            "{\n   if(std::any_cast<bool>(this->"+name+") != std::any_cast<bool>(convertedPtr->"+name+")) "
            "return false;\n}\n";
        }
        else
        {
            writer<<"if(!(this->"+name+" == convertedPtr->"+name+")) "
                    "return false;\n";
        }
    }
    writer<<"return true;\n}\n";
    // Fields.
    for (string field : fields)
    {
        string typeName = splitString(string(field.begin()+1, field.end()), ' ')[0];
        string name = splitString(string(field.begin()+1, field.end()), ' ')[1];
        if(typeName=="Expr" || typeName =="Stmt")
        {
            writer << "const std::shared_ptr<"+typeName+"> "<<name << ";\n";
        }
        else
        {
            writer << "const "<<typeName+" "<<name << ";\n";
        }
    }
    writer << "};\n\n";
}