#ifndef CMINUSMINUS_AST_HPP
#define CMINUSMINUS_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"
#include <cassert>


/* You absolutely will need to add a bunch of ASTNode subclasses */

namespace cminusminus{

/* You may find it useful to forward declare AST subclasses
   here so that you can use a class before it's full definition
*/
class DeclNode;
class TypeNode;
class StmtNode;
class IDNode;
class FormalDeclNode;

/**
* \class ASTNode
* Base class for all other AST Node types
**/
class ASTNode{
public:
ASTNode(Position * p) : myPos(p){ }
virtual void unparse(std::ostream& out, int indent) = 0;
Position * pos() { return myPos; }
std::string posStr() { return pos()->span(); }
protected:
Position * myPos;
};

/**
* \class ProgramNode
* Class that contains the entire abstract syntax tree for a program.
* Note the list of declarations encompasses all global declarations
* which includes (obviously) all global variables and struct declarations
* and (perhaps less obviously), all function declarations
**/
class ProgramNode : public ASTNode{
public:
ProgramNode(std::list<DeclNode *> * globalsIn) ;
void unparse(std::ostream& out, int indent) override;
private:
std::list<DeclNode * > * myGlobals;
};

class StmtNode : public ASTNode{
public:
StmtNode(Position * p) : ASTNode(p){ }
void unparse(std::ostream& out, int indent) override = 0;
};


/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to
* declare a struct, function, variable, etc).  This base class will
**/
class DeclNode : public StmtNode{
public:
DeclNode(Position * p) : StmtNode(p) { }
void unparse(std::ostream& out, int indent) override = 0;
};

/**  \class ExpNode
* Superclass for expression nodes (i.e. nodes that can be used as
* part of an expression).  Nodes that are part of an expression
* should inherit from this abstract superclass.
**/
class ExpNode : public ASTNode{
protected:
ExpNode(Position * p) : ASTNode(p){ }
};

class TrueNode : public ExpNode{
public:
TrueNode(Position * p) : ExpNode(p) { }
void unparse(std::ostream& out, int indent) override;
};

class FalseNode : public ExpNode{
public:
FalseNode(Position * p) : ExpNode(p){ }
void unparse(std::ostream& out, int indent) override;
};

class StrLitNode : public ExpNode{
public:
StrLitNode(Position * p, std::string Val)
: ExpNode(p), stringVal(Val){ }
void unparse(std::ostream& out, int indent) override;
private:
std::string stringVal;
};

class IntLitNode : public ExpNode{
public:
IntLitNode(Position * p, int Val)
: ExpNode(p), numval(Val){ }
void unparse(std::ostream& out, int indent) override;
private:
int numval;
};


class ShortLitNode : public ExpNode{
public:
ShortLitNode(Position * p, int Val)
: ExpNode(p), shortVal(Val){ }
void unparse(std::ostream& out, int indent) override;
private:
short shortVal;
};


class UnaryExpNode : public ExpNode{
public:
UnaryExpNode(Position * p, ExpNode * Expression)
: ExpNode(p), expression(Expression) { }
void unparse(std::ostream& out, int indent) override = 0;
private:
ExpNode * expression;
};

class NegNode : public UnaryExpNode{
public:
NegNode(Position * p, ExpNode * Expression) : UnaryExpNode(p, Expression) { }
void unparse(std::ostream& out, int indent) override;
};

class NotNode : public UnaryExpNode{
public:
NotNode(Position * p, ExpNode * Expression) : UnaryExpNode(p, Expression) { }
void unparse(std::ostream& out, int indent) override;
};

class RefNode : public UnaryExpNode{
public:
RefNode(Position * p, ExpNode * Expression) : UnaryExpNode(p, Expression) { }
void unparse(std::ostream& out, int indent) override;
};

class CallExpNode : public ExpNode{
public:
CallExpNode(Position * p, IDNode * Name) : ExpNode(p), nameFunc(Name) { }
CallExpNode(Position * p, IDNode * Name, std::list<ExpNode *> * Arguments) : ExpNode(p), nameFunc(Name), arguments(Arguments) { }
void unparse(std::ostream& out, int indent) override;
private:
IDNode * nameFunc;
std::list<ExpNode * > * arguments;
};

class CallStmtNode : public StmtNode{
public:
CallStmtNode(Position * p, CallExpNode * func)
: StmtNode(p), Function(func) { }
void unparse(std::ostream& out, int indent) override;
private:
CallExpNode * Function;
};

/**  \class TypeNode
* Superclass of nodes that indicate a data type. For example, in
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
TypeNode(Position * p) : ASTNode(p){
}
public:
virtual void unparse(std::ostream& out, int indent) = 0;
};

class LValNode : public ExpNode{
public:
LValNode(Position * p) : ExpNode(p){}
void unparse(std::ostream& out, int indent) override = 0;
};

class PostDecStmtNode : public StmtNode{
public:
PostDecStmtNode(Position * p, LValNode * Variable) : StmtNode(p), variable(Variable) { }
void unparse(std::ostream& out, int indent) override;
private:
LValNode * variable;
};

class PostIncStmtNode : public StmtNode{
public:
PostIncStmtNode(Position * p, LValNode * Variable) : StmtNode(p), variable(Variable) { }
void unparse(std::ostream& out, int indent) override;
private:
LValNode * variable;
};

class ReadStmtNode : public StmtNode{
public:
ReadStmtNode(Position * p, LValNode * Variable) : StmtNode(p), variable(Variable) { }
void unparse(std::ostream& out, int indent) override;
private:
LValNode * variable;
};

class WriteStmtNode : public StmtNode{
public:
WriteStmtNode(Position * p, ExpNode * Expression) : StmtNode(p), expression(Expression) { }
void unparse(std::ostream& out, int indent) override;
private:
ExpNode * expression;
};

class ReturnStmtNode : public StmtNode{
public:
ReturnStmtNode(Position * p, ExpNode * Expression) : StmtNode(p), expression(Expression) { }
ReturnStmtNode(Position * p) : StmtNode(p) {}
void unparse(std::ostream& out, int indent) override;
private:
ExpNode * expression;
};

class WhileStmtNode : public StmtNode{
public:
WhileStmtNode(Position * p, ExpNode * Condition, std::list<StmtNode *> * body)
: StmtNode(p), condition(Condition), WhileBody(body) { }
void unparse(std::ostream& out, int indent) override;
private:
ExpNode * condition;
std::list<StmtNode* > * WhileBody;
};

class IfStmtNode : public StmtNode{
public:
IfStmtNode(Position * p, ExpNode * Condition, std::list<StmtNode *> * body)
: StmtNode(p), condition(Condition), IfBody(body) { }
void unparse(std::ostream& out, int indent) override;
private:
ExpNode * condition;
std::list<StmtNode * > * IfBody;
};

class IfElseStmtNode : public StmtNode{
public:
IfElseStmtNode(Position *p, ExpNode * Condition, std::list<StmtNode *> * tbody, std::list<StmtNode *> * fbody)
: StmtNode(p), condition(Condition), IfTrueBody(tbody), IfFalseBody(fbody) { }
void unparse(std::ostream& out, int indent) override;
private:
ExpNode * condition;
std::list<StmtNode * > * IfTrueBody;
std::list<StmtNode * > * IfFalseBody;
};

/** An identifier. Note that IDNodes subclass
 * ExpNode because they can be used as part of an expression.
**/
class IDNode : public LValNode{
public:
IDNode(Position * p, std::string nameIn)
: LValNode(p), name(nameIn){ }
void unparse(std::ostream& out, int indent);
private:
/** The name of the identifier **/
std::string name;
};

class DerefNode : public LValNode{
public:
DerefNode(Position * p, std::string nameIn)
: LValNode(p), name(nameIn){ }
void unparse(std::ostream& out, int indent);
private:
/** The name of the identifier **/
std::string name;
};

class IndexNode : public LValNode{
public:
IndexNode(Position * p, IDNode * id, IDNode * name)
: LValNode(p), Id_being_accessed(id), field_Name_being_accessed(name) { }
void unparse(std::ostream& out, int indent) override;
private:
IDNode * Id_being_accessed;
IDNode * field_Name_being_accessed;
};


/** A variable declaration.
**/
class VarDeclNode : public DeclNode{
public:
VarDeclNode(Position * p, TypeNode * type, IDNode * id)
: DeclNode(p), myType(type), myId(id){
assert (myType != nullptr);
assert (myId != nullptr);
}
void unparse(std::ostream& out, int indent);
protected:
TypeNode * myType;
IDNode * myId;
};

class FormalDeclNode : public VarDeclNode{
public:
FormalDeclNode(Position * p, TypeNode * type, IDNode * id)
: VarDeclNode(p, type, id) { }
void unparse(std::ostream& out, int indent) override;
//private:
//TypeNode * myType;
//IDNode * myId;
};

class FnDeclNode : public DeclNode{
public:
FnDeclNode(Position * p, TypeNode * type, IDNode * id, std::list<StmtNode * > * funcBody)
: DeclNode(p), myType(type), myId(id), parameters(nullptr), functionBody(funcBody) { }
FnDeclNode(Position * p, TypeNode * type, IDNode * id, std::list<FormalDeclNode * > * paramIn, std::list<StmtNode * > * funcBody)
: DeclNode(p), myType(type), myId(id), parameters(paramIn), functionBody(funcBody) { }
void unparse(std::ostream& out, int indent) override;
private:
TypeNode * myType;
IDNode * myId;
std::list<FormalDeclNode * > * parameters;
std::list<StmtNode * > * functionBody;
};

class AssignExpNode : public ExpNode{
public:
AssignExpNode(Position * p, LValNode * Variable, ExpNode * Expression) : ExpNode(p), variable(Variable), expression(Expression) { }
void unparse(std::ostream& out, int indent) override;
private:
LValNode * variable;
ExpNode * expression;
};

class AssignStmtNode : public StmtNode{
public:
AssignStmtNode(Position * p, AssignExpNode * Assignment) : StmtNode(p), assignment(Assignment) { }
void unparse(std::ostream& out, int indent) override;
private:
AssignExpNode * assignment;
};

class IntTypeNode : public TypeNode{
public:
IntTypeNode(Position * p) : TypeNode(p){ }
void unparse(std::ostream& out, int indent);
};

class BoolTypeNode : public TypeNode{
public:
BoolTypeNode(Position * p) : TypeNode(p){ }
void unparse(std::ostream& out, int indent) override;
};

class VoidTypeNode : public TypeNode{
public:
VoidTypeNode(Position * p) : TypeNode(p) { }
void unparse(std::ostream& out, int indent) override;
};

class StringTypeNode : public TypeNode{
public:
StringTypeNode(Position * p) : TypeNode(p) { }
void unparse(std::ostream& out, int indent) override;
};

class BinaryExpNode : public ExpNode {
public:
BinaryExpNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : ExpNode(p), leftNode(leftNode), rightNode(rightNode) {}
void unparse(std::ostream& out, int indent) override = 0;
protected:
ExpNode * leftNode;
ExpNode * rightNode;
};

class AndNode : public BinaryExpNode {
public:
AndNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream&, int indent) override;
};

class DivideNode : public BinaryExpNode {
public:
DivideNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class EqualsNode : public BinaryExpNode {
public:
EqualsNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class GreaterEqNode : public BinaryExpNode {
public:
GreaterEqNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class GreaterNode : public BinaryExpNode {
public:
GreaterNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class LessEqNode : public BinaryExpNode {
public:
LessEqNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class LessNode : public BinaryExpNode {
public:
LessNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class MinusNode : public BinaryExpNode {
public:
MinusNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class NotEqualsNode : public BinaryExpNode {
public:
NotEqualsNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class OrNode : public BinaryExpNode {
public:
OrNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class PlusNode : public BinaryExpNode {
public:
PlusNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class TimesNode : public BinaryExpNode {
public:
TimesNode(Position * p, ExpNode * leftNode, ExpNode * rightNode) : BinaryExpNode(p, leftNode, rightNode) {}
void unparse(std::ostream& out, int indent) override;
};

class PtrTypeNode : public TypeNode{
public:
PtrTypeNode(Position * p) : TypeNode(p){ }
  void unparse(std::ostream& out, int indent) override;
};

class ShortTypeNode : public TypeNode{
public:
ShortTypeNode(Position * p) : TypeNode(p){ }
  void unparse(std::ostream& out, int indent) override;
};


} //End namespace cminusminus

#endif
