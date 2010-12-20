/** Structures and functions for parsing tokens into a parse tree. The parser
  * reads through a series of tokens (generated by the tokenizer) and adds
  * semantic meaning to them by forming them into a parse tree which can, in
  * turn, be passed on to later processes (such as an interpreter).
  *
  * \file   parser.h
  *
  * \author Justin J. Meza
  *
  * \date   2010 */

/** \page impvar The Implicit Variable
  *
  * The implicit variable in LOLCODE is denoted by the keyword \c IT and stores
  * a copy of the result of the most recently evaluated expression statement,
  * that is, an expression all by itself on a line.  (See
  * http://lolcode.com/specs/1.2#conditionals for an example.) */

/** \page lolebnf The LOLCODE EBNF
  * Presented below is the EBNF (see http://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_Form)
  * for LOLCODE that \c lci parses.  Note that by this stage, the scanner has:
  *   - already removed any whitespace between tokens,
  *   - added in and truncated newline tokens at logical line breaks, and
  *   - added an end-of-file (\c $) token.
  *
  * \section progebnf Program Structure
  *
  * These production rules dictate the overall form of the program.
  *
  * \par
  * MainNode ::= \c TT_HAI \a version \c TT_NEWLINE BlockNode \c $
  *
  * \par
  * BlockNode ::= StmtNode *
  *
  * \section typesebnf Types
  *
  * These production rules specify some general types of parse structures.
  *
  * \par
  * ConstantNode ::= Boolean | Integer | Float | String
  *
  * \par
  * IdentifierNode ::= Identifier
  *
  * \par
  * TypeNode ::= \c TT_NOOB | \c TT_TROOF | \c TT_NUMBR | \c TT_NUMBAR | \c TT_YARN
  *
  * \section stmtebnf Statements
  *
  * These production rules specify the types of statements formed.
  *
  * \par
  * StmtNode ::= CastStmtNode | PrintStmtNode | InputStmtNode | AssignmentStmtNode | DeclarationStmtNode | IfThenElseStmtNode | SwitchStmtNode | BreakStmt | ReturnStmtNode | LoopStmtNode | DeallocationStmtNode | FuncDefStmtNode | ExprStmt
  *
  * \par
  * CastStmtNode ::= IdentifierNode \c TT_ISNOWA TypeNode \c TT_NEWLINE
  *
  * \par
  * PrintStmtNode ::= \c TT_VISIBLE ExprNodeList \c TT_BANG ? TT_NEWLINE
  *
  * \par
  * InputStmtNode ::= \c TT_GIMMEH IdentifierNode TT_NEWLINE
  *
  * \par
  * AssignmentStmtNode ::= IdentifierNode \c TT_R ExprNode \c TT_NEWLINE
  *
  * \par
  * DeclarationStmtNode ::= IdentifierNode \c TT_HASA IdentifierNode Initialization ? \c TT_NEWLINE
  *
  * \par
  * Initialization ::= \c TT_ITZ ExprNode | \c TT_ITZA TypeNode
  *
  * \par
  * IfThenElseStmtNode ::= \c TT_ORLY \c TT_NEWLINE \c TT_YARLY \c TT_NEWLINE BlockNode ElseIf * Else ? \c TT_OIC \c TT_NEWLINE
  *
  * \par
  * ElseIf ::= \c TT_MEBBE ExprNode \c TT_NEWLINE BlockNode
  *
  * \par
  * Else ::= \c TT_NOWAI \c TT_NEWLINE BlockNode
  *
  * \par
  * SwitchStmtNode ::= \c TT_WTF \c TT_NEWLINE Case + DefaultCase ? \c TT_OIC \c TT_NEWLINE
  *
  * \par
  * Case ::= \c TT_OMG ExprNode \c TT_NEWLINE BlockNode
  *
  * \par
  * DefaultCase ::= \c TT_OMGWTF \c TT_NEWLINE BlockNode
  *
  * \par
  * BreakStmt ::= \c TT_GTFO \c TT_NEWLINE
  *
  * \par
  * ReturnStmtNode ::= \c TT_FOUNDYR ExprNode \c TT_NEWLINE
  *
  * \par
  * LoopStmtNode ::= \c TT_IMINYR IdentifierNode LoopUpdate ? LoopGuard ? \c TT_NEWLINE BlockNode \c TT_IMOUTTAYR IdentifierNode \c TT_NEWLINE
  *
  * \par
  * LoopUpdate ::= LoopUpdateOp \c TT_YR IdentifierNode
  *
  * \par
  * LoopUpdateOp ::= \c TT_UPPIN | \c TT_NERFIN | UnaryFunction
  *
  * \par
  * UnaryFunction ::= The name of a previously defined unary function.
  *
  * \par
  * LoopGuard ::= \c TT_TIL ExprNode | \c TT_WILE ExprNode
  *
  * \par
  * DeallocationStmtNode ::= IdentifierNode \c TT_RNOOB
  *
  * \par
  * FuncDefStmtNode ::= \c TT_HOWIZ IdentifierNode IdentifierNode FunctionArgs ? \c TT_NEWLINE BlockNode \c TT_IFUSAYSO \c TT_NEWLINE
  *
  * \par
  * FunctionArgs ::= \c TT_YR IdentifierNode FunctionArg *
  *
  * \par
  * FunctionArg ::= \c TT_ANYR IdentifierNode
  *
  * \par
  * ExprStmt ::= ExprNode \c TT_NEWLINE
  *
  * \section exprebnf Expressions
  *
  * These production rules specify the types of expressions formed.
  *
  * \par
  * ExprNode ::= CastExprNode | ConstantNode | IdentifierNode | FuncCallExprNode | OpExprNode | ImplicitVariable
  *
  * \par
  * CastExprNode ::= \c TT_MAEK ExprNode \c TT_A TypeNode
  *
  * \par
  * FuncCallExprNode ::= IdentifierNode \c TT_IZ IdentifierNode FunctionArgs ? TT_MKAY
  *
  * \par
  * OpExprNode ::= UnaryOp | BinaryOp | NaryOp
  *
  * \par
  * UnaryOp ::= UnaryOpType ExprNode
  *
  * \par
  * UnaryOpType ::= \c TT_NOT
  *
  * \par
  * BinaryOp ::= BinaryOpType ExprNode \c TT_AN ? ExprNode
  *
  * \par
  * BinaryOpType ::= \c TT_SUMOF | \c TT_DIFFOF | \c TT_PRODUKTOF | \c TT_QUOSHUNTOF | \c TT_MODOF | \c BIGGROF | \c SMALLROF | \c TT_BOTHOF | \c TT_EITHEROF | \c TT_WONOF
  *
  * \par
  * NaryOp ::= NaryOpType NaryOpArgs \c TT_MKAY
  *
  * \par
  * NaryOpType ::= \c TT_ALLOF | \c TT_ANYOF | \c TT_SMOOSH
  *
  * \par
  * NaryOpArgs ::= ExprNode NaryOpArg +
  *
  * \par
  * NaryOpArg ::= \c TT_AN ? ExprNode
  *
  * \par
  * ImplicitVariable ::= \c TT_IT */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokenizer.h"

#undef DEBUG

/** Stores an identifier.  An identifier is the string of characters that are
  * used to uniquely name a particular variable.
  *
  * \see createIdentifierNode(char *, const char *, unsigned int)
  * \see deleteIdentifierNode(IdentifierNode *) */
typedef struct {
	char *image;       /**< An array of characters that name the identifier. */
	const char *fname; /**< A pointer to the name of the file containing the identifier. */
	unsigned int line; /**< The line number from the source file that the identifier occurred on. */
} IdentifierNode;

/** Stores a list of identifiers.  This structure allows sets of identifiers
  * to be grouped together.
  *
  * \see createIdentifierNodeList(void)
  * \see addIdentifierNode(IdentifierNodeList *, IdentifierNode *)
  * \see deleteIdentifierNodeList(IdentifierNodeList *) */
typedef struct {
	unsigned int num;     /**< The number of IdentifierNode structures stored. */
	IdentifierNode **ids; /**< A pointer to the array of IdentifierNode structures. */
} IdentifierNodeList;

/** Denotes the type of statement a StmtNode stores. */
typedef enum {
	ST_CAST,         /**< A CastStmtNode structure. */
	ST_PRINT,        /**< A PrintStmtNode structure. */
	ST_INPUT,        /**< An InputStmtNode structure. */
	ST_ASSIGNMENT,   /**< An AssignmentStmtNode structure. */
	ST_DECLARATION,  /**< A DeclarationStmtNode structure. */
	ST_IFTHENELSE,   /**< An IfThenElseStmtNode structure. */
	ST_SWITCH,       /**< A SwitchStmtNode structure. */
	ST_BREAK,        /**< A break statement (no structure is needed for this type of statement). */
	ST_RETURN,       /**< A ReturnStmtNode structure. */
	ST_LOOP,         /**< A LoopStmtNode structure. */
	ST_DEALLOCATION, /**< A DeallocationStmtNode structure. */
	ST_FUNCDEF,      /**< A FuncDefStmtNode structure. */
	ST_EXPR          /**< An ExprNode structure. */
} StmtType;

/** Stores a statement.  A statement is a unit of code which can be executed by
  * itself and may possibly cause side-effects to occur.
  *
  * \see createStmtNode(StmtType, void *)
  * \see deleteStmtNode(StmtNode *) */
typedef struct {
	StmtType type; /**< The type of statement stored in \a node. */
	void *stmt;    /**< A pointer to the particular statement structure. */
} StmtNode;

/** Stores a list of statements.  This structure allows sets of statements to be
  * grouped together.
  *
  * \see createStmtNodeList(void)
  * \see addStmtNode(StmtNodeList *, StmtNode *)
  * \see deleteStmtNodeList(StmtNodeList *) */
typedef struct {
	unsigned int num; /**< The number of StmtNode structures stored. */
	StmtNode **stmts; /**< A pointer to the array of StmtNode structures. */
} StmtNodeList;

/** Denotes the type of expression an ExprNode stores. */
typedef enum {
	ET_CAST,       /**< A CastExprNode structure. */
	ET_CONSTANT,   /**< A ConstantNode structure. */
	ET_IDENTIFIER, /**< An IdentifierNode structure. */
	ET_FUNCCALL,   /**< A FuncCallExprNode structure. */
	ET_OP,         /**< An OpExprNode structure. */
	ET_IMPVAR      /**< An \ref impvar "implicit variable" (no structure is needed for this type of expression). */
} ExprType;

/** Stores an expression.  An expression is a unit of code which evaluates to
  * some value and typically does not cause side-effects to occur.
  *
  * \see createExprNode(ExprType, void *)
  * \see deleteExprNode(ExprNode *) */
typedef struct {
	ExprType type; /**< The type of expression stored in \a expr. */
	void *expr;    /**< A pointer to the particular expression structure. */ 
} ExprNode;

/** Stores a list of expressions.  This structure allows sets of expressions to
  * be grouped together.
  *
  * \see createExprNodeList(void)
  * \see addExprNode(ExprNodeList *, ExprNode *)
  * \see deleteExprNodeList(ExprNodeList *) */
typedef struct {
	unsigned int num; /**< The number of ExprNode structures stored. */
	ExprNode **exprs; /**< A pointer to an array of ExprNode structures. */
} ExprNodeList;

/** Stores a a block of code.  A block of code consists of a set of statements.
  *
  * \see createBlockNode(StmtNodeList *)
  * \see deleteBlockNode(BlockNode *) */
typedef struct {
	StmtNodeList *stmts; /**< A pointer to the list of statements which comprise the block of code. */
} BlockNode;

/** Stores a list of blocks of code.  This structure allows sets of blocks of
  * code to be grouped together.
  *
  * \see createBlockNodeList(void)
  * \see addBlockNode(BlockNodeList *, BlockNode *)
  * \see deleteBlockNodeList(BlockNodeList *) */
typedef struct {
	unsigned int num;   /**< The number of BlockNode structures stored. */
	BlockNode **blocks; /**< A pointer to an array of BlockNode structures. */
} BlockNodeList;

/** Denotes the type of a constant. */
typedef enum {
	CT_INTEGER, /**< An integer value. */
	CT_FLOAT,   /**< A floating point decimal value. */
	CT_BOOLEAN, /**< A true/false value. */
	CT_STRING,  /**< A character string value. */
	CT_NIL      /**< Represents no value. */
} ConstantType;

/** Stores the data associated with a ConstantNode structure. */
typedef union {
	int i;   /**< Integer data. */
	float f; /**< Floating point data. */
	char *s; /**< Character string data. */
} ConstantData;

/** Stores a constant value.  A constant value evaluates to its contents,
  * depending on its \a type.
  *
  * \see createBooleanConstantNode(int)
  * \see createIntegerConstantNode(int)
  * \see createFloatConstantNode(float)
  * \see createStringConstantNode(char *)
  * \see deleteConstantNode(ConstantNode *) */
typedef struct {
	ConstantType type; /**< The type of the constant. */
	ConstantData data; /**< The stored data of type \a type. */
} ConstantNode;

/** Stores a function definition statement.  A function definition statement
  * defines the prototype and contents of a function.
  *
  * \see createFuncDefStmtNode(IdentifierNode *, IdentifierNode *, IdentifierNodeList *, BlockNode *)
  * \see deleteFuncDefStmtNode(FuncDefStmtNode *) */
typedef struct {
	IdentifierNode *scope;    /**< A pointer to the scope to define the function in. */
	IdentifierNode *name;     /**< A pointer to the name of the function. */
	IdentifierNodeList *args; /**< A pointer to a list of the names of the arguments of the function. */
	BlockNode *body;          /**< A pointer to the block of code defined by the function. */
} FuncDefStmtNode;

/** Stores the main block of code a program executes.  This structure could be
  * accomplished using only a BlockNode instead, but its logical importance to
  * program control flow (namely, it is the first portion of code executed)
  * merits its own structure.
  *
  * \see createMainNode(BlockNode *)
  * \see deleteMainNode(MainNode *) */
typedef struct {
	BlockNode *block;       /**< A pointer to the block of code to execute first. */
} MainNode;

/** Stores a variable type.
  *
  * \see createTypeNode(ConstantType)
  * \see deleteTypeNode(TypeNode *) */
typedef struct {
	ConstantType type; /**< The type of the variable. */
} TypeNode;

/** Stores a cast statement.  A cast statement changes the type of a variable
  * identified by \a target to the type given by \a newtype.
  *
  * \see createCastStmtNode(IdentifierNode *, TypeNode *)
  * \see deleteCastStmtNode(CastStmtNode *) */
typedef struct {
	IdentifierNode *target; /**< A pointer to the name of the variable whose type is to be changed to \a newtype. */
	TypeNode *newtype;      /**< A pointer to the type to change \a target to. */
} CastStmtNode;

/** Stores a print statement.  A print statement prints its arguments to some
  * output device (by default standard output).
  *
  * \see createPrintStmtNode(ExprNodeList *, int)
  * \see deletePrintStmtNode(PrintStmtNode *) */
typedef struct {
	ExprNodeList *args; /**< A pointer to the list of expressions to evaluate and print. */
	int nonl;           /**< Denotes an ending newline should be surpressed if not \c 0 and printed if \c 0. */
} PrintStmtNode;

/** Stores an input statement.  An input statement accepts a line of input from
  * the use on an input device (by default standard input) and stores it in a
  * variable.
  *
  * \see createInputStmtNode(IdentifierNode *)
  * \see deleteInputStmtNode(InputStmtNode *) */
typedef struct {
	IdentifierNode *target; /**< A pointer to the name of the variable to store the input in. */
} InputStmtNode;

/** Stores an assignment statement.  An assignment statement updates the value
  * of a variable, \a target, to the result of an expression, \a expr.
  *
  * \see createAssignmentStmtNode(IdentifierNode *, ExprNode *)
  * \see deleteAssignmentStmtNode(AssignmentStmtNode *) */
typedef struct {
	IdentifierNode *target; /**< A pointer to the name of the variable to store the evaluated contents of \a expr into. */
	ExprNode *expr;         /**< A pointer to the expression to evaluate and store in \a target. */
} AssignmentStmtNode;

/** Stores a declaration statement.  A declaration statement creates a new
  * variable named by \a target, optionally initializing it to the evaluated
  * contents of \a expr.  \a scope determines which level of scope the variable
  * is to be created in.
  *
  * \note \a expr and \a type are mutually exclusive.  If both are non-NULL,
  *       results are undefined.
  *
  * \see createDeclarationStmtNode(IdentifierNode *, IdentifierNode *, ExprNode *)
  * \see deleteDeclarationStmtNode(DeclarationStmtNode *) */
typedef struct {
	IdentifierNode *scope;  /**< A pointer to the scope to create the variable in. */
	IdentifierNode *target; /**< A pointer to the name of the variable to create. */
	ExprNode *expr;         /**< An optional pointer to the expression to initialize \a target to. */
	TypeNode *type;         /**< An optional pointer to the type to initialize \a target to. */
} DeclarationStmtNode;

/** Stores an if/then/else statement.  A conditional statement checks the value
  * of the \ref impvar "implicit variable" and executes \a yes if it casts to \c
  * true.  If the value of the \ref impvar "implicit variable" casts to \c
  * false, each of the optional expressions in \a guards is evaluated and if it
  * casts to true, the corresponding block in \a blocks is executes.  If the
  * value of the \ref impvar "implicit variable" casts to false \b and all of
  * the guards cast to false, the contents of \a no is executed.
  *
  * \see createIfThenElseStmtNode(BlockNode *, BlockNode *, ExprNodeList *, BlockNodeList *)
  * \see deleteIfThenElseStmtNode(IfThenElseStmtNode *) */
typedef struct {
	BlockNode *yes;        /**< A pointer to the block of code to execute if the \ref impvar "implicit variable" casts to false. */
	BlockNode *no;         /**< A pointer to the block of code to execute if the \ref impvar "implicit variable" casts to false \b and the evaluations of all of the \a guards cast to false. */
	ExprNodeList *guards;  /**< A pointer to the expressions to test if the \ref impvar "implicit variable" casts to false. */
	BlockNodeList *blocks; /**< A pointer to the respective blocks of code to execute if one of the evaluated \a guards casts to true. */
} IfThenElseStmtNode;

/** Stores a switch statement.  A switch statement compares the value of the
  * \ref impvar "implicit variable" to each of the \a guards and executes the
  * respective block of code in \a blocks if they match.  If no matches are
  * found between the \ref impvar "implicit variable" and one of the \a guards,
  * the optional default block of code, \a def, is executed.
  *
  * \see createSwitchStmtNode(ExprNodeList *, BlockNodeList *, BlockNode *)
  * \see deleteSwitchStmtNode(SwitchStmtNode *) */
typedef struct {
	ExprNodeList *guards;  /**< A pointer to the expressions to evaluate and compare to the \ref impvar "implicit variable". */
	BlockNodeList *blocks; /**< A pointer to the respective blocks of code to execute if one of the \a guards matches the \ref impvar "implicit variable". */
	BlockNode *def;        /**< A pointer to the default block of code to execute if none of the \a guards match the \ref impvar "implicit variable". */
} SwitchStmtNode;

/** Stores a return statement.  A return statement signals that the current
  * function is to be returned from with value \a value.
  *
  * \see createReturnStmtNode(ExprNode *)
  * \see deleteReturnStmtNode(ReturnStmtNode *) */
typedef struct {
	ExprNode *value; /**< A pointer to the value to return. */
} ReturnStmtNode;

/** Stores a loop statement.  A loop statement repeatedly executes its \a body
  * while \a guard evaluates to true, executing \a update at the end of each
  * cycle.
  *
  * \see createLoopStmtNode(IdentifierNode *, IdentifierNode *, ExprNode *, ExprNode *, BlockNode *)
  * \see deleteLoopStmtNode(LoopStmtNode *) */
typedef struct {
	IdentifierNode *name; /**< A pointer to the name of the loop. */
	IdentifierNode *var;  /**< A pointer to the name of the variable to be updated by \a update. */
	ExprNode *guard;      /**< A pointer to the expression to determine if the loop will continue. */
	ExprNode *update;     /**< A pointer to the expression to evaluate to update \a var. */
	BlockNode *body;      /**< A pointer to the block of code to be executed with each iteration of the loop. */
} LoopStmtNode;

/** Deallocates a variable.  This means freeing the memory and structures used
  * by the variable.
  *
  * \see createDeallocationStmtNode(IdentifierNode *)
  * \see deleteDeallocationStmtNode(DeallocationStmtNode *) */
typedef struct {
	IdentifierNode *target; /**< A pointer to the name of the variable. */
} DeallocationStmtNode;

/** Stores a cast expression.  A cast expression evaluates an expression and
  * casts it to a particular type.
  *
  * \see createCastExprNode(ExprNode *, TypeNode *)
  * \see deleteCastExprNode(CastExprNode *) */
typedef struct {
	ExprNode *target;  /**< A pointer to the expression to cast. */
	TypeNode *newtype; /**< A pointer to the type to cast the copy of \a target to. */
} CastExprNode;

/** Stores a function call expression.  A function call expression evaluates to
  * the return value of the function defined in \a def called with the arguments
  * listed in \a args.
  *
  * \see createFuncCallExprNode(IdentifierNode *, IdentifierNode *, ExprNodeList *)
  * \see deleteFuncCallExprNode(FuncCallExprNode *) */
typedef struct {
	IdentifierNode *scope; /**< A pointer to the scope the function is defined in. */
	IdentifierNode *name;  /**< A pointer to the name of the function. */
	ExprNodeList *args;    /**< A pointer to a list of ExprNode structure arguments to be supplied to the function. */
} FuncCallExprNode;

/** Denotes the type of operation an OpExprNode performs. */
typedef enum {
	OP_ADD,  /**< Addition. */
	OP_SUB,  /**< Subtraction. */
	OP_MULT, /**< Multiplication. */
	OP_DIV,  /**< Division. */
	OP_MOD,  /**< Modulo. */
	OP_MAX,  /**< Maximum. */
	OP_MIN,  /**< Minimum. */

	OP_AND,  /**< Logical AND. */
	OP_OR,   /**< Logical OR. */
	OP_XOR,  /**< Logical XOR. */
	OP_NOT,  /**< Logical NOT. */

	OP_EQ,   /**< Equality. */
	OP_NEQ,  /**< Inequality. */

	OP_CAT   /**< String concatenation. */
} OpType;

/** Stores an operation expression.  An operation expression evaluates to
  * the result of the operation performed on its arguments.
  *
  * \see createOpExprNode(OpType, ExprNodeList *)
  * \see deleteOpExprNode(OpExprNode *) */
typedef struct  {
	OpType type;        /**< The type of operation to perform on \a args. */
	ExprNodeList *args; /**< A pointer to the arguments to perform the operation on. */
} OpExprNode;

MainNode *createMainNode(BlockNode *);
void deleteMainNode(MainNode *);

BlockNode *createBlockNode(StmtNodeList *);
void deleteBlockNode(BlockNode *);
BlockNodeList *createBlockNodeList(void);
int addBlockNode(BlockNodeList *, BlockNode *);
void deleteBlockNodeList(BlockNodeList *);

IdentifierNode *createIdentifierNode(char *, const char *, unsigned int);
void deleteIdentifierNode(IdentifierNode *);

TypeNode *createTypeNode(ConstantType);
void deleteTypeNode(TypeNode *);

StmtNode *createStmtNode(StmtType, void *);
void deleteStmtNode(StmtNode *);
StmtNodeList *createStmtNodeList(void);
int addStmtNode(StmtNodeList *, StmtNode *);
void deleteStmtNodeList(StmtNodeList *);

CastStmtNode *createCastStmtNode(IdentifierNode *, TypeNode *);
void deleteCastStmtNode(CastStmtNode *);

PrintStmtNode *createPrintStmtNode(ExprNodeList *, int);
void deletePrintStmtNode(PrintStmtNode *);

InputStmtNode *createInputStmtNode(IdentifierNode *);
void deleteInputStmtNode(InputStmtNode *);

AssignmentStmtNode *createAssignmentStmtNode(IdentifierNode *, ExprNode *);
void deleteAssignmentStmtNode(AssignmentStmtNode *);

DeclarationStmtNode *createDeclarationStmtNode(IdentifierNode *, IdentifierNode *, ExprNode *, TypeNode *);
void deleteDeclarationStmtNode(DeclarationStmtNode *);

IfThenElseStmtNode *createIfThenElseStmtNode(BlockNode *, BlockNode *, ExprNodeList *, BlockNodeList *);
void deleteIfThenElseStmtNode(IfThenElseStmtNode *);

SwitchStmtNode *createSwitchStmtNode(ExprNodeList *, BlockNodeList *, BlockNode *);
void deleteSwitchStmtNode(SwitchStmtNode *);

ReturnStmtNode *createReturnStmtNode(ExprNode *);
void deleteReturnStmtNode(ReturnStmtNode *);

LoopStmtNode *createLoopStmtNode(IdentifierNode *, IdentifierNode *, ExprNode *, ExprNode *, BlockNode *);
void deleteLoopStmtNode(LoopStmtNode *);

DeallocationStmtNode *createDeallocationStmtNode(IdentifierNode *);
void deleteDeallocationStmtNode(DeallocationStmtNode *);

FuncDefStmtNode *createFuncDefStmtNode(IdentifierNode *, IdentifierNode *, IdentifierNodeList *, BlockNode *);
void deleteFuncDefStmtNode(FuncDefStmtNode *);

ExprNode *createExprNode(ExprType, void *);
void deleteExprNode(ExprNode *);
ExprNodeList *createExprNodeList(void);
int addExprNode(ExprNodeList *, ExprNode *);
void deleteExprNodeList(ExprNodeList *);

CastExprNode *createCastExprNode(ExprNode *, TypeNode *);
void deleteCastExprNode(CastExprNode *);

FuncCallExprNode *createFuncCallExprNode(IdentifierNode *, IdentifierNode *, ExprNodeList *);
void deleteFuncCallExprNode(FuncCallExprNode *);

OpExprNode *createOpExprNode(OpType, ExprNodeList *);
void deleteOpExprNode(OpExprNode *);

int acceptToken(Token ***, TokenType);
int peekToken(Token ***, TokenType);
int nextToken(Token ***, TokenType);

void error(const char *, Token **);

ConstantNode *parseConstantNode(Token ***);
TypeNode *parseTypeNode(Token ***);
IdentifierNode *parseIdentifierNode(Token ***);
ExprNode *parseExprNode(Token ***);
StmtNode *parseStmtNode(Token ***);
BlockNode *parseBlockNode(Token ***);
MainNode *parseMainNode(Token **);

ExprNode *parseConstantExprNode(Token ***);
ExprNode *parseIdentifierExprNode(Token ***);
ExprNode *parseFuncCallExprNode(Token ***);
ExprNode *parseOpExprNode(Token ***);

StmtNode *parsePrintStmtNode(Token ***);
StmtNode *parseInputStmtNode(Token ***);
StmtNode *parseAssignmentStmtNode(Token ***);
StmtNode *parseDeclarationStmtNode(Token ***);
StmtNode *parseIfThenElseStmtNode(Token ***);
StmtNode *parseSwitchStmtNode(Token ***);
StmtNode *parseBreakStmtNode(Token ***);
StmtNode *parseReturnStmtNode(Token ***);
StmtNode *parseLoopStmtNode(Token ***);
StmtNode *parseDeallocationStmtNode(Token ***);
StmtNode *parseFuncDefStmtNode(Token ***);

ConstantNode *createBooleanConstantNode(int);
ConstantNode *createIntegerConstantNode(int);
ConstantNode *createFloatConstantNode(float);
ConstantNode *createStringConstantNode(char *);
void deleteConstantNode(ConstantNode *);

IdentifierNodeList *createIdentifierNodeList(void);
int addIdentifierNode(IdentifierNodeList *, IdentifierNode *);
void deleteIdentifierNodeList(IdentifierNodeList *);

#endif /* __PARSER_H__ */
