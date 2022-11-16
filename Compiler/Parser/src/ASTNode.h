#ifndef COMPILER_ASTNODE_H
#define COMPILER_ASTNODE_H

#include <string>
#include<iostream>

namespace PARSER {
    class ASTNode {
    public:
        ASTNode(std::string content, int urgency=0):m_content(content),urgency(urgency){
            children=std::vector<ASTNode*>(0);}

        ASTNode(const ASTNode&)=delete;
        ASTNode(const ASTNode&&)=delete;

        inline std::vector<ASTNode*>& getChldren(){
            return children;
        };

        inline std::string getContent(){
            return m_content;
        };

        inline void addChild(ASTNode*newChild){
            children.push_back(newChild);
        };

        inline void setOpFlag(bool Flag){
            if(Flag)flag|=1;
            else flag&=INT32_MAX-1;
        }

        inline void setFuncFlag(bool Flag){
            if(Flag)flag|=(1<<1);
            else flag&=INT32_MAX-(1<<1);
        }

        inline bool getFuncFlag(){
            return flag&(1<<1);
        }

        inline void setIfFlag(bool Flag){
            if(Flag)flag|=(1<<9);
            else flag&=INT32_MAX-(1<<9);
        }

        inline bool getIfFlag(){
            return flag&(1<<9);
        }

        inline void setWhileFlag(bool Flag){
            if(Flag)flag|=(1<<10);
            else flag&=INT32_MAX-(1<<10);
        }

        inline bool getWhileFlag(){
            return flag&(1<<10);
        }

        inline bool getOpFlag(){
            return flag&1;
        }

        inline void setConstFlag(bool Flag){
            if(Flag)flag|=(1<<4);
            else flag&=INT32_MAX-(1<<4);
        }

        inline bool getConstFlag(){
            return flag&(1<<4);
        }

        inline void setConstDeclFlag(bool Flag){
            if(Flag)flag|=(1<<2);
            else flag&=INT32_MAX-(1<<2);
        }

        inline bool getConstDeclFlag(){
            return flag&(1<<2);
        }

        inline void setRoot(bool Flag){
            if(Flag)flag|=(1<<3);
            else flag&=INT32_MAX-(1<<3);
        }

        inline bool getRoot(){
            return flag&(1<<3);
        }

        inline void setArray(bool Flag){
            if(Flag)flag|=(1<<5);
            else flag&=INT32_MAX-(1<<5);
        }

        inline bool getArray(){
            return flag&(1<<5);
        }

        inline void setDecl(bool Flag){
            if(Flag)flag|=(1<<6);
            else flag&=INT32_MAX-(1<<6);
        }

        inline bool getDecl(){
            return flag&(1<<6);
        }
        inline void setFloat(bool Flag){
            if(Flag)flag|=(1<<8);
            else flag&=INT32_MAX-(1<<8);
        }

        inline bool getFloat(){
            return flag&(1<<8);
        }

        inline void setBlock(bool Flag){
            if(Flag)flag|=(1<<11);
            else flag&=INT32_MAX-(1<<11);
        }

        inline bool getBlock(){
            return flag&(1<<11);
        }

        inline void setInt(bool Flag){
            if(Flag)flag|=(1<<7);
            else flag&=INT32_MAX-(1<<7);
        }

        inline bool getInt(){
            return flag&(1<<7);
        }

        inline int getUrgency(){
            return urgency;
        };

    private:
        int flag=0;/*the first bit represent if this is an operator
 *                   the second bit represent if this is a function usage
 *                   the third bit represent if this is a const decl
 *                   the forth bit represent if this is the root
 *                   the fifth bit represent if this is a const val
 *                   the sixth bit represent if this is an array
 *                   the seventh bit represent if this is a var decl
 *                   the eighth bit represent if this is an int
 *                   the ninth bit represent if this is a float
 *                   the tenth bit represent if this is an if stmt
 *                   the eleventh bit represent if this is a while stmt
 *                   the twelfth bit represent if this is a block root*/

        std::vector<ASTNode*>children;

        std::string m_content;

        int urgency;
    };
}
#endif


