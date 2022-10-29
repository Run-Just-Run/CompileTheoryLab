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

        inline bool getOpFlag(){
            return flag&1;
        }

        inline void setConstFlag(bool Flag){
            if(Flag)flag|=(1<<2);
            else flag&=INT32_MAX-(1<<2);
        }

        inline bool getConstFlag(){
            return flag&(1<<2);
        }

        inline int getUrgency(){
            return urgency;
        };

    private:
        int flag=0;/*the first bit represent if this is an operator
 *                   the second bit represent if this is a function usage
 *                   the third bit represent if this is a const decl*/

        std::vector<ASTNode*>children;

        std::string m_content;

        int urgency;
    };
}
#endif


